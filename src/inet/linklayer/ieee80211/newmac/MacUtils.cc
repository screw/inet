//
// Copyright (C) 2015 Andras Varga
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
//
// Author: Andras Varga
//

#include "MacUtils.h"
#include "IMacParameters.h"
#include "IRateSelection.h"
#include "inet/linklayer/ieee80211/mac/Ieee80211Consts.h"
#include "inet/linklayer/ieee80211/mac/Ieee80211Frame_m.h"
#include "inet/physicallayer/ieee80211/packetlevel/Ieee80211ControlInfo_m.h"

namespace inet {
namespace ieee80211 {

MacUtils::MacUtils(IMacParameters *params, IRateSelection *rateSelection) : params(params), rateSelection(rateSelection)
{
}

simtime_t MacUtils::getAckDuration() const
{
    return rateSelection->getResponseControlFrameMode()->getDuration(LENGTH_ACK);
}

simtime_t MacUtils::getCtsDuration() const
{
    return rateSelection->getResponseControlFrameMode()->getDuration(LENGTH_CTS);
}

simtime_t MacUtils::getAckEarlyTimeout() const
{
    // Note: This excludes ACK duration. If there's no RXStart indication within this interval, retransmission should begin immediately
    return params->getPhyRxStartDelay() + params->getSifsTime() + params->getSlotTime();
}

simtime_t MacUtils::getAckFullTimeout() const
{
    return params->getPhyRxStartDelay() + params->getSifsTime() + params->getSlotTime() + getAckDuration();
}

simtime_t MacUtils::getCtsEarlyTimeout() const
{
    return params->getPhyRxStartDelay() + params->getSifsTime() + params->getSlotTime();
}

simtime_t MacUtils::getCtsFullTimeout() const
{
    return params->getPhyRxStartDelay() + params->getSifsTime() + params->getSlotTime() + getCtsDuration();
}

Ieee80211RTSFrame *MacUtils::buildRtsFrame(Ieee80211DataOrMgmtFrame *dataFrame) const
{
    return buildRtsFrame(dataFrame, getFrameMode(dataFrame));
}

Ieee80211RTSFrame *MacUtils::buildRtsFrame(Ieee80211DataOrMgmtFrame *dataFrame, const IIeee80211Mode *dataFrameMode) const
{
    // protect CTS + Data + ACK
    simtime_t duration =
            3 * params->getSifsTime()
            + rateSelection->getResponseControlFrameMode()->getDuration(LENGTH_CTS)
            + dataFrameMode->getDuration(dataFrame->getBitLength())
            + rateSelection->getResponseControlFrameMode()->getDuration(LENGTH_ACK);
    return buildRtsFrame(dataFrame->getReceiverAddress(), duration);
}

Ieee80211RTSFrame *MacUtils::buildRtsFrame(const MACAddress& receiverAddress, simtime_t duration) const
{
    Ieee80211RTSFrame *rtsFrame = new Ieee80211RTSFrame("RTS");
    rtsFrame->setTransmitterAddress(params->getAddress());
    rtsFrame->setReceiverAddress(receiverAddress);
    rtsFrame->setDuration(duration);
    setFrameMode(rtsFrame, rateSelection->getModeForControlFrame(rtsFrame));
    return rtsFrame;
}

Ieee80211CTSFrame *MacUtils::buildCtsFrame(Ieee80211RTSFrame *rtsFrame) const
{
    Ieee80211CTSFrame *frame = new Ieee80211CTSFrame("CTS");
    frame->setReceiverAddress(rtsFrame->getTransmitterAddress());
    frame->setDuration(rtsFrame->getDuration() - params->getSifsTime() - rateSelection->getResponseControlFrameMode()->getDuration(LENGTH_CTS));
    setFrameMode(rtsFrame, rateSelection->getModeForControlFrame(rtsFrame));
    return frame;
}

Ieee80211ACKFrame *MacUtils::buildAckFrame(Ieee80211DataOrMgmtFrame *dataFrame) const
{
    Ieee80211ACKFrame *ackFrame = new Ieee80211ACKFrame("ACK");
    ackFrame->setReceiverAddress(dataFrame->getTransmitterAddress());

    if (!dataFrame->getMoreFragments())
        ackFrame->setDuration(0);
    else
        ackFrame->setDuration(dataFrame->getDuration() - params->getSifsTime() - rateSelection->getResponseControlFrameMode()->getDuration(LENGTH_ACK));
    setFrameMode(ackFrame, rateSelection->getModeForControlFrame(ackFrame));
    return ackFrame;
}

Ieee80211Frame *MacUtils::setFrameMode(Ieee80211Frame *frame, const IIeee80211Mode *mode) const
{
    ASSERT(frame->getControlInfo() == nullptr);
    Ieee80211TransmissionRequest *ctrl = new Ieee80211TransmissionRequest();
    ctrl->setMode(mode);
    frame->setControlInfo(ctrl);
    return frame;
}

const IIeee80211Mode *MacUtils::getFrameMode(Ieee80211Frame *frame) const
{
    Ieee80211TransmissionRequest *ctrl = check_and_cast<Ieee80211TransmissionRequest*>(frame->getControlInfo());
    return ctrl->getMode();
}

bool MacUtils::isForUs(Ieee80211Frame *frame) const
{
    return frame->getReceiverAddress() == params->getAddress() || (frame->getReceiverAddress().isMulticast() && !isSentByUs(frame));
}

bool MacUtils::isSentByUs(Ieee80211Frame *frame) const
{
    if (auto dataOrMgmtFrame = dynamic_cast<Ieee80211DataOrMgmtFrame *>(frame))
        return dataOrMgmtFrame->getAddress3() == params->getAddress();
    else
        return false;
}


bool MacUtils::isBroadcastOrMulticast(Ieee80211Frame *frame) const
{
    return frame && frame->getReceiverAddress().isMulticast();  // also true for broadcast frames
}

bool MacUtils::isBroadcast(Ieee80211Frame *frame) const
{
    return frame && frame->getReceiverAddress().isBroadcast();
}

bool MacUtils::isFragment(Ieee80211DataOrMgmtFrame *frame) const
{
    return frame->getFragmentNumber() != 0 || frame->getMoreFragments() == true;
}

bool MacUtils::isCts(Ieee80211Frame *frame) const
{
    return dynamic_cast<Ieee80211CTSFrame *>(frame);
}

bool MacUtils::isAck(Ieee80211Frame *frame) const
{
    return dynamic_cast<Ieee80211ACKFrame *>(frame);
}

int MacUtils::cmpMgmtOverData(Ieee80211DataOrMgmtFrame *a, Ieee80211DataOrMgmtFrame *b)
{
    int aPri = dynamic_cast<Ieee80211ManagementFrame*>(a) ? 1 : 0;  //TODO there should really exist a high-performance isMgmtFrame() function!
    int bPri = dynamic_cast<Ieee80211ManagementFrame*>(b) ? 1 : 0;
    return bPri - aPri;
}

int MacUtils::cmpMgmtOverMulticastOverUnicast(Ieee80211DataOrMgmtFrame *a, Ieee80211DataOrMgmtFrame *b)
{
    int aPri = dynamic_cast<Ieee80211ManagementFrame*>(a) ? 2 : a->getReceiverAddress().isMulticast() ? 1 : 0;
    int bPri = dynamic_cast<Ieee80211ManagementFrame*>(a) ? 2 : b->getReceiverAddress().isMulticast() ? 1 : 0;
    return bPri - aPri;
}

} // namespace ieee80211
} // namespace inet

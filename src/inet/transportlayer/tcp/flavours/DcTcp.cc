//
// Copyright (C) 2004-2005 Andras Varga
// Copyright (C) 2009 Thomas Reschka
// Copyright (C) 2019 Marcel Marek
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, see <http://www.gnu.org/licenses/>.
//

#include <algorithm>    // min,max
#include "inet/transportlayer/tcp/flavours/DcTcp.h"
#include "inet/transportlayer/tcp/Tcp.h"

namespace inet {

namespace tcp {

Register_Class(DcTcp);

DcTcp::DcTcp() : TcpReno()
{
}


void DcTcp::receivedDataAck(uint32 firstSeqAcked)
{
      TcpTahoeRenoFamily::receivedDataAck(firstSeqAcked);

      //how to calculate marks?
      bool markingCalc = true; // true -> based on bytes; false based on number of acks
      /*
       * Bytes - works better with unequal segment size but not with packet reordering and 'delayed' Acks.
       *         The 'delayed' Ack (presumably because the DATA packet was stuck in queue and marked)
       *         acknowledges all outstanding data as "marked".
       * Number - assumes equal segment size - counts the number of acks as each should represent the same
       *          number of bytes.
       */

      uint32 bytes_acked = state->snd_una - firstSeqAcked;
      state->dctcp_bytesAcked += state->snd_una - firstSeqAcked;

      if(state->eceBit) {
        state->dctcp_bytesMarked += (state->snd_una - firstSeqAcked);
      }


    if (state->dupacks >= state->DUPTHRESH) {    // DUPTHRESH = 3
        //
        // Perform Fast Recovery: set cwnd to ssthresh (deflating the window).
        //
        EV_INFO << "Fast Recovery: setting cwnd to ssthresh=" << state->ssthresh << "\n";
        state->snd_cwnd = state->ssthresh;

        conn->emit(cwndSignal, state->snd_cwnd);
    }
    else {

      state->dctcp_total++;

      if (state->eceBit) {
        state->dctcp_marked++;
          conn->emit(markingProbSignal, 1);
      } else {
          conn->emit(markingProbSignal, 0);
      }

      simtime_t now = simTime();
      bool cut = false;

      if(now - state->dctcp_lastCalcTime >= state->minrtt) {

        double ratio;
        if(markingCalc){
        // state->dctcp_marked / state->dctcp_total;
         ratio = (state->dctcp_bytesMarked / state->dctcp_bytesAcked);
        }else{
          ratio = state->dctcp_marked / state->dctcp_total;
        }


        conn->emit(loadSignal, ratio);



        state->dctcp_alpha = (1 - state->dctcp_gamma) * state->dctcp_alpha + state->dctcp_gamma * ratio;

        conn->emit(calcLoadSignal, state->dctcp_alpha);

        if (state->dctcp_marked && state->dctcp_CWR == false)
          cut = true;

        state->dctcp_lastCalcTime = now;
        state->dctcp_marked = 0;
        state->dctcp_total = 0;

        state->dctcp_windEnd = state->snd_nxt;
        state->dctcp_bytesAcked = state->dctcp_bytesMarked = 0;
        state->dctcp_CWR = false;

      }

      if ((state->dctcp_bytesMarked && !state->dctcp_CWR)) {
        //            if(cut || (state->dctcp_bytesMarked && !state->dctcp_CWR) ) {
        state->dctcp_CWR = true;

        state->snd_cwnd = state->snd_cwnd * (1 - state->dctcp_alpha / 2);

        conn->emit(cwndSignal, state->snd_cwnd);

        uint32 flight_size = std::min(state->snd_cwnd, state->snd_wnd); // FIXME TODO - Does this formula computes the amount of outstanding data?
        state->ssthresh = std::max(3 * flight_size / 4, 2 * state->snd_mss);

        conn->emit(ssthreshSignal, state->ssthresh);



      } else {

        //
        // Perform slow start and congestion avoidance.
        //
        if (state->snd_cwnd < state->ssthresh) {
            EV_INFO << "cwnd <= ssthresh: Slow Start: increasing cwnd by one SMSS bytes to ";

            // perform Slow Start. RFC 2581: "During slow start, a TCP increments cwnd
            // by at most SMSS bytes for each ACK received that acknowledges new data."
            state->snd_cwnd += state->snd_mss;

            // Note: we could increase cwnd based on the number of bytes being
            // acknowledged by each arriving ACK, rather than by the number of ACKs
            // that arrive. This is called "Appropriate Byte Counting" (ABC) and is
            // described in RFC 3465. This RFC is experimental and probably not
            // implemented in real-life TCPs, hence it's commented out. Also, the ABC
            // RFC would require other modifications as well in addition to the
            // two lines below.
            //
            // int bytesAcked = state->snd_una - firstSeqAcked;
            // state->snd_cwnd += bytesAcked * state->snd_mss;

            conn->emit(cwndSignal, state->snd_cwnd);

            EV_INFO << "cwnd=" << state->snd_cwnd << "\n";
        }
        else {
//            // perform Congestion Avoidance (RFC 2581)
//            uint32 incr = state->snd_mss * state->snd_mss / state->snd_cwnd;
          uint32 incr = bytes_acked * state->snd_mss / state->snd_cwnd;

          // perhaps move it somewhere in TcpTahoeRenoFamily so it doesn't have to be copied to every flavour?
          if(state->coupledIncrease){
            //coupled increase (RFC 6356 equation 1)
            if(state->snd_cwnd_total == 0){
              state->snd_cwnd_total = state->snd_cwnd;
            }
            uint32 incr2 = state->alpha * bytes_acked * state->snd_mss / state->snd_cwnd_total;
            if(incr2 < incr){
              incr = incr2;
            }
          }

            if (incr == 0)
                incr = 1;

            state->snd_cwnd += incr;

            conn->emit(cwndSignal, state->snd_cwnd);

//            if(state->coupledIncrease){
//              state->snd_cwnd_total += incr;
//
//              conn->emit(cwndTotalSignal, state->snd_cwnd_total);
//            }

            //
            // Note: some implementations use extra additive constant mss / 8 here
            // which is known to be incorrect (RFC 2581 p5)
            //
            // Note 2: RFC 3465 (experimental) "Appropriate Byte Counting" (ABC)
            // would require maintaining a bytes_acked variable here which we don't do
            //

            EV_INFO << "cwnd > ssthresh: Congestion Avoidance: increasing cwnd linearly, to " << state->snd_cwnd << "\n";
        }
    }
    }

    if (state->sack_enabled && state->lossRecovery) {
        // RFC 3517, page 7: "Once a TCP is in the loss recovery phase the following procedure MUST
        // be used for each arriving ACK:
        //
        // (A) An incoming cumulative ACK for a sequence number greater than
        // RecoveryPoint signals the end of loss recovery and the loss
        // recovery phase MUST be terminated.  Any information contained in
        // the scoreboard for sequence numbers greater than the new value of
        // HighACK SHOULD NOT be cleared when leaving the loss recovery
        // phase."
        if (seqGE(state->snd_una, state->recoveryPoint)) {
            EV_INFO << "Loss Recovery terminated.\n";
            state->lossRecovery = false;
        }
        // RFC 3517, page 7: "(B) Upon receipt of an ACK that does not cover RecoveryPoint the
        //following actions MUST be taken:
        //
        // (B.1) Use Update () to record the new SACK information conveyed
        // by the incoming ACK.
        //
        // (B.2) Use SetPipe () to re-calculate the number of octets still
        // in the network."
        else {
            // update of scoreboard (B.1) has already be done in readHeaderOptions()
            conn->setPipe();

            // RFC 3517, page 7: "(C) If cwnd - pipe >= 1 SMSS the sender SHOULD transmit one or more
            // segments as follows:"
            if (((int)state->snd_cwnd - (int)state->pipe) >= (int)state->snd_mss) // Note: Typecast needed to avoid prohibited transmissions
                conn->sendDataDuringLossRecoveryPhase(state->snd_cwnd);
        }
    }

    // RFC 3517, pages 7 and 8: "5.1 Retransmission Timeouts
    // (...)
    // If there are segments missing from the receiver's buffer following
    // processing of the retransmitted segment, the corresponding ACK will
    // contain SACK information.  In this case, a TCP sender SHOULD use this
    // SACK information when determining what data should be sent in each
    // segment of the slow start.  The exact algorithm for this selection is
    // not specified in this document (specifically NextSeg () is
    // inappropriate during slow start after an RTO).  A relatively
    // straightforward approach to "filling in" the sequence space reported
    // as missing should be a reasonable approach."
    sendData(false);
}



} // namespace tcp

} // namespace inet


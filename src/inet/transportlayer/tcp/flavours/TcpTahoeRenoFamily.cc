//
// Copyright (C) 2004 Andras Varga
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

#include "inet/transportlayer/tcp/Tcp.h"
#include "inet/transportlayer/tcp/flavours/TcpTahoeRenoFamily.h"

namespace inet {
namespace tcp {

TcpTahoeRenoFamilyStateVariables::TcpTahoeRenoFamilyStateVariables()
{
    // The initial value of ssthresh SHOULD be set arbitrarily high (e.g.,
    // to the size of the largest possible advertised window)
    // Without user interaction there is no limit...
    ssthresh = 0xFFFFFFFF;

    //DCTCP

    dctcp_alpha = 0;
    dctcp_windEnd = snd_una;
    dctcp_bytesAcked = 0;
    dctcp_bytesMarked = 0;
    dctcp_CWR = false;
    dctcp_gamma = 0.16;
    dctcp_lastCalcTime = 0;
    dctcp_marked = 0;
    dctcp_total = 0;
    dctcp_totalSent = 0;
}

void TcpTahoeRenoFamilyStateVariables::setSendQueueLimit(uint32 newLimit){
    // The initial value of ssthresh SHOULD be set arbitrarily high (e.g.,
    // to the size of the largest possible advertised window) -> defined by sendQueueLimit
    sendQueueLimit = newLimit;
    ssthresh = sendQueueLimit;
}

std::string TcpTahoeRenoFamilyStateVariables::str() const
{
    std::stringstream out;
    out << TcpBaseAlgStateVariables::str();
    out << " ssthresh=" << ssthresh;
    return out.str();
}

std::string TcpTahoeRenoFamilyStateVariables::detailedInfo() const
{
    std::stringstream out;
    out << TcpBaseAlgStateVariables::detailedInfo();
    out << "ssthresh=" << ssthresh << "\n";
    return out.str();
}

//---

TcpTahoeRenoFamily::TcpTahoeRenoFamily() : TcpBaseAlg(),
    state((TcpTahoeRenoFamilyStateVariables *&)TcpAlgorithm::state)
{
}

} // namespace tcp
} // namespace inet


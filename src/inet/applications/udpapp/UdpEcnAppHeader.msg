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
import inet.common.INETDefs;
import inet.common.packet.chunk.Chunk;

namespace inet;


cplusplus {{
const B UDP_ECN_APP_HEADER_LENGTH = B(6);
}}

enum Type
{
  UDP_ECN_DATA = 0; // DATA 
  UDP_ECN_ACK = 1; // ACK

}



class UdpEcnAppHeader extends FieldsChunk
{
//    @existingClass;
	chunkLength = UDP_ECN_APP_HEADER_LENGTH;

	bool eceBit; //Congestion Experienced - makes sense to set it only on Acks; PDUs on the forward path (Data) are marked in IP ECN
	Type type = UDP_ECN_DATA; //0 = Data, 1 - Ack
	unsigned int sequenceNo;
};
//
// Copyright (C) 2020 Marcel Marek
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

namespace inet;

cplusplus {{
  
const int IP_AH_HEADER_BYTES = 24;  
}}


packet IPAH
{
    uint8_t nextHeader; // IP Protocol number defined by IANA (4 = IPv4; 41 IPv6; 6 = TCP)
    uint8_t payloadLength; // length of AH in multiple of 4B  minus 2. So the length including fixed fields is (payloadLength + 2) * 4 = Bytes
    uint16_t reserved; 
    uint32_t spi; // security parameters index
    uint32_t sequenceNumber; //TODO: 64b is Extended Seq Num but it is now default with IKE2
    uint32_t esn;
    uint32_t icv[]; // integrity check value has to be multiple of 32bits
    
}

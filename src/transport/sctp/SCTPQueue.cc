//
// Copyright (C) 2005-2009 Irene Ruengeler
// Copyright (C) 2009-2010 Thomas Dreibholz
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, see <http://www.gnu.org/licenses/>.
//


#include "SCTPQueue.h"
#include "SCTPAssociation.h"

Register_Class(SCTPQueue);


SCTPQueue::SCTPQueue()
{
    assoc = NULL;
}

SCTPQueue::~SCTPQueue()
{
    for (PayloadQueue::iterator iterator = payloadQueue.begin();
            iterator != payloadQueue.end(); iterator++) {
        SCTPDataVariables* chunk = iterator->second;
        delete chunk->userData;
    }
    if (!payloadQueue.empty()) {
        payloadQueue.clear();
    }
}

bool SCTPQueue::checkAndInsertChunk(const uint32 key, SCTPDataVariables* chunk)
{
    PayloadQueue::iterator found = payloadQueue.find(key);
    if (found != payloadQueue.end()) {
        return false;
    }
    payloadQueue[key] = chunk;
    return true;
}

uint32 SCTPQueue::getQueueSize() const
{
    return payloadQueue.size();
}

SCTPDataVariables* SCTPQueue::extractMessage()
{
    if (!payloadQueue.empty()) {
        PayloadQueue::iterator iterator = payloadQueue.begin();
        SCTPDataVariables*    chunk = iterator->second;
        payloadQueue.erase(iterator);
        return chunk;
    }
    return NULL;
}

SCTPDataVariables* SCTPQueue::getAndExtractChunk(const uint32 tsn)
{
    if (!payloadQueue.empty()) {
        PayloadQueue::iterator iterator = payloadQueue.find(tsn);
        SCTPDataVariables*    chunk = iterator->second;
        payloadQueue.erase(iterator);
        return chunk;
    }
    return NULL;
}

void SCTPQueue::printQueue() const
{
    sctpEV3 << "Queue contents:\n";
    for (PayloadQueue::const_iterator iterator = payloadQueue.begin();
            iterator != payloadQueue.end(); ++iterator) {
        const uint32                 key = iterator->first;
        const SCTPDataVariables* chunk = iterator->second;
        sctpEV3 << key << ":\t"
                << "lastDestination=" << chunk->getLastDestination()
                << " nextDestination=" << chunk->getNextDestination()
                << " hasBeenAcked=" << chunk->hasBeenAcked
                << " countsAsOutstanding=" << chunk->countsAsOutstanding
                << " numberOfRetransmissions=" << chunk->numberOfRetransmissions
                << endl;
    }
    sctpEV3 << endl;
}

SCTPDataVariables* SCTPQueue::getFirstChunk() const
{
    PayloadQueue::const_iterator iterator = payloadQueue.begin();
    SCTPDataVariables* chunk = iterator->second;
    return chunk;
}

cMessage* SCTPQueue::getMsg(const uint32 tsn) const
{
    PayloadQueue::const_iterator iterator = payloadQueue.find(tsn);
    if (iterator != payloadQueue.end()) {
        SCTPDataVariables* chunk = iterator->second;
        cMessage* msg = check_and_cast<cMessage*>(chunk->userData);
        return msg;
    }
    return NULL;
}

SCTPDataVariables* SCTPQueue::getChunk(const uint32 tsn) const
{
    PayloadQueue::const_iterator iterator = payloadQueue.find(tsn);
    if (iterator != payloadQueue.end()) {
        SCTPDataVariables* chunk = iterator->second;
        return chunk;
    }
    return NULL;
}

SCTPDataVariables* SCTPQueue::getChunkFast(const uint32 tsn, bool& firstTime)
{
    if (!firstTime) {
        if (GetChunkFastIterator != payloadQueue.end()) {
            SCTPDataVariables* chunk = GetChunkFastIterator->second;
            if (chunk->tsn == tsn) {
                GetChunkFastIterator++;
                return (chunk);    // Found the right TSN!
            }
        }
        // TSN not found -> needs regular TSN lookup.
    }

    GetChunkFastIterator = payloadQueue.find(tsn);
    if (GetChunkFastIterator != payloadQueue.end()) {
        SCTPDataVariables* chunk = GetChunkFastIterator->second;
        GetChunkFastIterator++;
        firstTime = false;
        return (chunk);
    }

    return (NULL);
}


void SCTPQueue::removeMsg(const uint32 tsn)
{
    PayloadQueue::iterator iterator = payloadQueue.find(tsn);
    payloadQueue.erase(iterator);
}

bool SCTPQueue::deleteMsg(const uint32 tsn)
{
    PayloadQueue::iterator iterator = payloadQueue.find(tsn);
    if (iterator != payloadQueue.end())
    {
        SCTPDataVariables* chunk = iterator->second;
        cMessage* msg = check_and_cast<cMessage*>(chunk->userData);
        delete msg;
        payloadQueue.erase(iterator);
        return true;
    }
    return false;
}

int32 SCTPQueue::getNumBytes() const
{
    int32 qb = 0;
    for (PayloadQueue::const_iterator iterator = payloadQueue.begin();
            iterator != payloadQueue.end(); iterator++) {
        qb += (iterator->second->len / 8);
    }
    return qb;
}

SCTPDataVariables* SCTPQueue::dequeueChunkBySSN(const uint16 ssn)
{
    for (PayloadQueue::iterator iterator = payloadQueue.begin();
            iterator != payloadQueue.end(); iterator++) {
        SCTPDataVariables* chunk = iterator->second;
        if ((iterator->second->ssn == ssn) &&
                (iterator->second->bbit) &&
                (iterator->second->ebit) ) {
            payloadQueue.erase(iterator);
            return chunk;
        }
    }
    return NULL;
}



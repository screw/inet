/*
 *  Copyright (C) 2007 Christoph Sommer <christoph.sommer@informatik.uni-erlangen.de>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "DYMO_TokenBucket.h"

using namespace inet;

DYMO_TokenBucket::DYMO_TokenBucket(double tokensPerTick, double maxTokens, simtime_t currentTime) : tokensPerTick(tokensPerTick), maxTokens(maxTokens), availableTokens(maxTokens), lastUpdate(currentTime)
{
}

bool DYMO_TokenBucket::consumeTokens(double tokens, simtime_t currentTime)
{
    // calculate number of ticks that passed
    double ticksPassed = SIMTIME_DBL(currentTime - lastUpdate);
    lastUpdate = currentTime;

    // refill bucket
    availableTokens = std::min(maxTokens, availableTokens + (ticksPassed * tokensPerTick));

    // check if bucket is full enough
    if (tokens > availableTokens) return false;

    // drain bucket a bit
    availableTokens -= tokens;
    return true;
}

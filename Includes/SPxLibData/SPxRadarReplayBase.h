/*********************************************************************
*
* (c) Copyright 2007 - 2018, Cambridge Pixel Ltd.
*
* Purpose:
*	Base class for SPxRadarReplay object that allows
*	SPxDecoderRepeater to avoid dependence on zlib.
*
*
* Revision Control:
*   22/11/18 v1.1    AGC	Initial version.
*
* Previous Changes:
**********************************************************************/

#ifndef _SPX_RADAR_REPLAY_BASE_H
#define _SPX_RADAR_REPLAY_BASE_H

/* We need the base class header. */
#include "SPxLibData/SPxRadarSource.h"

/* Forward declarations. */
class SPxPacketDecoderFile;

/* Interface for getting packet decoder. */
class SPxRadarReplayBase : public SPxRadarSource
{
public:
    SPxRadarReplayBase(SPxRIB *rib) : SPxRadarSource(rib) {}
    virtual ~SPxRadarReplayBase(void) {}
    virtual SPxPacketDecoderFile *GetPacketDecoder(void) = 0;
};

#endif /* _SPX_RADAR_REPLAY_BASE_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/


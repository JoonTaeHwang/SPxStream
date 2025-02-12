/*********************************************************************
 *
 * (c) Copyright 2020 - 2022, Cambridge Pixel Ltd.
 *
 * Purpose:
 *	Header for SPxTargetDataOutputSPxTracks class which supports
 *	outputting a received stream of SPx Extended tracks.
 *
 * Revision Control:
 *   11/04/22 v1.5   BTB	Use filtered rather than decoded data as a source.
 *
 * Previous Changes:
 *   14/09/21 1.4   BTB	Provide UniTrackDB to class rather than filter filename.
 *   10/09/21 1.3   BTB	Support track filtering.
 *   19/08/21 1.2   BTB	Support track annotation.
 *   02/10/20 1.1   BTB	Initial Version.
 **********************************************************************/

#ifndef _SPX_TARGET_DATA_OUTPUT_SPX_TRACKS_H
#define _SPX_TARGET_DATA_OUTPUT_SPX_TRACKS_H

/*
 * Other headers required.
 */
#include "SPxLibData/SPxPackets.h"

/* We need the SPxTargetDataOutput base class. */
#include "SPxLibNet/SPxTargetDataOutput.h"

/* Forward declarations. */

class SPxTargetDataOutputSPxTracks : public SPxTargetDataOutput
{
public:
    /*
     * Public functions.
     */

    /* Constructor and destructor. */
    SPxTargetDataOutputSPxTracks(SPxTargetDataFilter *dataSrc,
                                 SPxExtHeartbeatSender *hbSender);
    virtual ~SPxTargetDataOutputSPxTracks(void);

private:
    /*
     * Private variables.
     */

    /*
     * Private functions.
     */
    void handleExtTrack(SPxPacketTrackExtended *extReport);
    void handleTrackerStatus(SPxPacketTrackerStatus_t *statusReport);

    /*
     * Private static functions.
     */
    static int extTrackHandler(void *invokingObj,
                               void *userArg,
                               void *extTrackMsg);
    static int trackerStatusHandler(void *invokingObj,
                                    void *userArg,
                                    void *statusMsg);

}; /* SPxTargetDataOutputSPxTracks */

#endif /* _SPX_TARGET_DATA_OUTPUT_SPX_TRACKS_H */

/*********************************************************************
 *
 * End of file
 *
 **********************************************************************/

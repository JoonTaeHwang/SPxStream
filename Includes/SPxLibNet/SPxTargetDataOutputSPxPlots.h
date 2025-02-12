/*********************************************************************
 *
 * (c) Copyright 2020 - 2022, Cambridge Pixel Ltd.
 *
 * Purpose:
 *	Header for SPxTargetDataOutputSPxPlots class which supports
 *	encoding a received stream of SPx Extended tracks as SPx Plots
 *	then outputting the encoded stream.
 *
 * Revision Control:
 *   11/04/22 v1.2   BTB	Use filtered rather than decoded data as a source.
 *
 * Previous Changes:
 *   02/10/20 1.1   BTB	Initial Version.
 **********************************************************************/

#ifndef _SPX_TARGET_DATA_OUTPUT_SPX_PLOTS_H
#define _SPX_TARGET_DATA_OUTPUT_SPX_PLOTS_H

/*
 * Other headers required.
 */
#include "SPxLibData/SPxPackets.h"

/* We need the SPxTargetDataOutput base class. */
#include "SPxLibNet/SPxTargetDataOutput.h"

class SPxTargetDataOutputSPxPlots : public SPxTargetDataOutput
{
public:
    /*
     * Public functions.
     */

    /* Constructor and destructor. */
    SPxTargetDataOutputSPxPlots(SPxTargetDataFilter *dataSrc,
                                SPxExtHeartbeatSender *hbSender);
    virtual ~SPxTargetDataOutputSPxPlots(void);

private:
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

}; /* SPxTargetDataOutputSPxPlots */

#endif /* _SPX_TARGET_DATA_OUTPUT_SPX_PLOTS_H */

/*********************************************************************
 *
 * End of file
 *
 **********************************************************************/

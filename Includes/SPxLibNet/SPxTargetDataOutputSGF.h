/*********************************************************************
 *
 * (c) Copyright 2020 - 2022, Cambridge Pixel Ltd.
 *
 * Purpose:
 *	Header for SPxTargetDataOutputSGF class which supports
 *	encoding a received stream of SPx Extended tracks as track
 *	data wrapped in an SGF 1.1 header then outputting it.
 *
 *	SGF 1.1 header message content comes from a specification
 *	(confidential) obtained as part of project P478.
 *
 * Revision Control:
 *   11/04/22 v1.3   BTB	Use filtered rather than decoded data as a source.
 *
 * Previous Changes:
 *   24/05/21 1.2   BTB	Add option to calculate Mode C in TPS messages.
 *   21/10/20 1.1   BTB	Initial Version.
 **********************************************************************/

#ifndef _SPX_TARGET_DATA_OUTPUT_SGF_H
#define _SPX_TARGET_DATA_OUTPUT_SGF_H

/*
 * Other headers required.
 */
#include "SPxLibData/SPxPackets.h"

/* We need the SPxTargetDataOutput base class. */
#include "SPxLibNet/SPxTargetDataOutput.h"

class SPxTargetDataOutputSGF : public SPxTargetDataOutput
{
public:
    /*
     * Public functions.
     */

    /* Constructor and destructor. */
    SPxTargetDataOutputSGF(SPxTargetDataFilter *dataSrc,
                           SPxExtHeartbeatSender *hbSender);
    virtual ~SPxTargetDataOutputSGF(void);

    /* Generic parameter assignment. */
    virtual int SetParameter(char *parameterName, char *parameterValue);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);

private:
    /*
     * Private variables.
     */
    UINT8 m_dataFormat;
    UINT8 m_sourceID;
    UINT8 m_channelID;
    UINT8 m_sequenceNum;
    int m_calcModeC;

    /*
     * Private functions.
     */
    SPxErrorCode setDataFormat(UINT8 format);
    void handleExtTrack(SPxPacketTrackExtended *extReport);

    /*
     * Private static functions.
     */
    static int extTrackHandler(void *invokingObj,
                               void *userArg,
                               void *extTrackMsg);

}; /* SPxTargetDataOutputSGF */

#endif /* _SPX_TARGET_DATA_OUTPUT_SGF_H */

/*********************************************************************
 *
 * End of file
 *
 **********************************************************************/

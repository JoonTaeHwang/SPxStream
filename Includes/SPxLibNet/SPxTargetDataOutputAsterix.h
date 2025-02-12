/*********************************************************************
 *
 * (c) Copyright 2020 - 2022, Cambridge Pixel Ltd.
 *
 * Purpose:
 *	Header for SPxTargetDataOutputAsterix class which supports
 *	encoding a received stream of SPx Extended tracks as ASTERIX
 *	CAT-48 tracks then outputting the encoded stream.
 *
 * Revision Control:
 *   10/06/22 v1.5   BTB	Support configuration of category for output message.
 *
 * Previous Changes:
 *   11/04/22 1.4   BTB	Use filtered rather than decoded data as a source.
 *   14/10/20 1.3   BTB	Manage ASTERIX encoder fully internally.
 *   07/10/20 1.2   BTB	Add missing forward declaration.
 *   02/10/20 1.1   BTB	Initial Version.
 **********************************************************************/

#ifndef _SPX_TARGET_DATA_OUTPUT_ASTERIX_H
#define _SPX_TARGET_DATA_OUTPUT_ASTERIX_H

/*
 * Other headers required.
 */
#include "SPxLibData/SPxPackets.h"

/* We need the SPxTargetDataOutput base class. */
#include "SPxLibNet/SPxTargetDataOutput.h"

/* Forward declarations. */
class SPxAsterixEncoder;

class SPxTargetDataOutputAsterix : public SPxTargetDataOutput
{
public:
    /*
     * Public functions.
     */

    /* Constructor and destructor. */
    SPxTargetDataOutputAsterix(SPxTargetDataFilter *dataSrc,
                               SPxExtHeartbeatSender *hbSender);
    virtual ~SPxTargetDataOutputAsterix(void);

    /* Generic parameter assignment. */
    virtual int SetParameter(char *parameterName, char *parameterValue);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);

private:
    /*
     * Private variables.
     */
    SPxAsterixEncoder *m_encObj;
    int m_havePrimary;
    int m_haveSecondary;
    unsigned int m_category;

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

}; /* SPxTargetDataOutputAsterix */

#endif /* _SPX_TARGET_DATA_OUTPUT_ASTERIX_H */

/*********************************************************************
 *
 * End of file
 *
 **********************************************************************/

/*********************************************************************
 *
 * (c) Copyright 2020 - 2022, Cambridge Pixel Ltd.
 *
 * Purpose:
 *	Header for SPxTargetDecoderAsterix class of object for
 *	receiving and decoding ASTERIX CAT-48 track and/or plot data
 *	and outputting SPx extended tracks.
 *
 *	This class effectively wraps SPxAsterixDecoder in a way which
 *	fits in with the architecture of the SPxTargetDecoder objects.
 *
 *
 * Revision Control:
 *   08/06/22 v1.2   BTB	Handle sensor lat/long better.
 *
 * Previous Changes:
 *   07/02/20 1.1   BTB	Initial Version.
 **********************************************************************/

#ifndef _SPX_TARGET_DECODER_ASTERIX_H
#define _SPX_TARGET_DECODER_ASTERIX_H

/*
 * Other headers required.
 */
#include "SPxLibData/SPxPackets.h"
#include "SPxLibNet/SPxAsterixDecoder.h"

/* We need the SPxTargetDecoder base class. */
#include "SPxLibNet/SPxTargetDecoder.h"

/*********************************************************************
 *
 *   Constants
 *
 *********************************************************************/

class SPxTargetDecoderAsterix : public SPxTargetDecoder
{
public:
    /*
     * Public functions.
     */

    /* Constructor and destructor. */
    explicit SPxTargetDecoderAsterix(int initiallyDisabled);
    virtual ~SPxTargetDecoderAsterix(void);

    /* Generic parameter assignment. */
    virtual int SetParameter(char *parameterName, char *parameterValue);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);

private:
    /*
     * Private variables.
     */
    SPxAsterixDecoder *m_decObj;

    /*
     * Private functions.
     */
    void handlePacket(const unsigned char *payload, unsigned int numBytes);

    void handleSPxTrack(SPxPacketTrackExtended *rpt);
    void handleSPxTrackerStatus(SPxPacketTrackerStatus_t *rpt);

    /*
     * Private static functions.
     */
    static void handlePacketWrapper(SPxPacketDecoder *packetDecoder,
                                    void *userArg,
                                    UINT32 packetType,
                                    struct sockaddr_in *from,
                                    SPxTime_t *timestamp,
                                    const unsigned char *payload,
                                    unsigned int numBytes);

    static void handleSPxTrackWrapper(const SPxAsterixDecoder *decoder,
                                      void *userArg,
                                      struct SPxPacketTrackExtended_tag *rpt,
                                      const SPxAsterixMsg *asterixMsg);
    static void handleSPxTrackerStatusWrapper(const SPxAsterixDecoder *decoder,
                                              void *userArg,
                                              struct SPxPacketTrackerStatus_tag *rpt,
                                              const SPxAsterixMsg *asterixMsg);

}; /* SPxTargetDecoderAsterix */

#endif /* _SPX_TARGET_DECODER_ASTERIX_H */

/*********************************************************************
 *
 * End of file
 *
 **********************************************************************/

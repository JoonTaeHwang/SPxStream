/*********************************************************************
 *
 * (c) Copyright 2020, Cambridge Pixel Ltd.
 *
 * Purpose:
 *	Header for SPxTTCDecoder class which supports the receiving
 *	of NMEA TTC track messages and translating them into
 *	SPx extended track messages.
 *
 *	TTC track message content comes from a specification (confidential)
 *	obtained as part of project P386.
 *
 *
 * Revision Control:
 *   04/02/20 v1.2   BTB 	Add note explaining source of message data information.
 *
 * Previous Changes:
 *   21/01/20 1.1   BTB 	Initial Version.
 **********************************************************************/

#ifndef _SPX_TTC_DECODER_H
#define _SPX_TTC_DECODER_H

/*
 * Other headers required.
 */

/* We need the SPxTargetDecoder base class. */
#include "SPxLibNet/SPxTargetDecoder.h"

/*********************************************************************
 *
 *   Constants
 *
 *********************************************************************/

/* Optional fields. */
#define SPX_TTC_DECODER_FIELD_QUALITY_METRIC    0x00000001
#define SPX_TTC_DECODER_FIELD_CONFIDENCE        0x00000002
#define SPX_TTC_DECODER_FIELD_CLASSIFICATION    0x00000004
#define SPX_TTC_DECODER_FIELD_THREAT_LEVEL      0x00000008

class SPxTTCDecoder : public SPxTargetDecoder
{
public:
    /*
     * Public functions.
     */

    /* Constructor and destructor. */
    SPxTTCDecoder(int initiallyDisabled, UINT32 optionalFieldFlags);
    virtual ~SPxTTCDecoder(void);

private:
    /*
     * Private variables.
     */

    /* Information related to optional fields. */
    int m_optionalFieldFlags;
    int m_optionalFieldCount;
    UINT32 m_extendedFlagsMask;

    /* Information on previous messages and scans. */
    int m_havePOS;
    int m_havePreviousScan;
    unsigned int m_previousScanNumber;
    UINT32 m_previousScanTimeStamp;
    UINT32 m_previousReportTimeStamp;
    REAL32 m_updatePeriod;

    /* Variables used to build sentences from packets. */
    char m_sentenceBuffer[500];
    unsigned int m_bufferIndex;
    int m_parsingSentence;
    int m_haveCR;

    /*
     * Private functions.
     */
    void handlePacket(const unsigned char *payload, unsigned int numBytes);
    void translateSentence(const char *sentence, unsigned int sentenceSize);

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

}; /* SPxTTCDecoder */

#endif /* _SPX_TTC_DECODER_H */

/*********************************************************************
 *
 * End of file
 *
 **********************************************************************/

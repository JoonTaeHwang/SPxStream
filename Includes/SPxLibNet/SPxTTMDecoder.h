/*********************************************************************
 *
 * (c) Copyright 2020, Cambridge Pixel Ltd.
 *
 * Purpose:
 *	Header for SPxTTMDecoder class which supports the receiving
 *	of NMEA TTM track messages and translating them into
 *	SPx extended track messages.
 *
 *
 * Revision Control:
 *   06/10/20 v1.2   BTB 	Make constructor explicit.
 *
 * Previous Changes:
 *   05/02/20 1.1   BTB 	Initial Version.
 **********************************************************************/

#ifndef _SPX_TTM_DECODER_H
#define _SPX_TTM_DECODER_H

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

class SPxTTMDecoder : public SPxTargetDecoder
{
public:
    /*
     * Public functions.
     */

    /* Constructor and destructor. */
    explicit SPxTTMDecoder(int initiallyDisabled);
    virtual ~SPxTTMDecoder(void);

private:
    /*
     * Private variables.
     */

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

}; /* SPxTTMDecoder */

#endif /* _SPX_TTM_DECODER_H */

/*********************************************************************
 *
 * End of file
 *
 **********************************************************************/

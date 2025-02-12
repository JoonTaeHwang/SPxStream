/*********************************************************************
 *
 * (c) Copyright 2021, Cambridge Pixel Ltd.
 *
 * Purpose:
 *	Header for SPxJY11Decoder class which supports the receiving
 *	of track messages from a JY-11 radar and translating
 *	them into SPx extended track messages.
 *
 *	JY-11 track message content comes from reverse engineering
 *	of captured data from the radar as part of project P444.
 *
 *
 * Revision Control:
 *   17/08/21 v1.1   AJH 	Initial Version.
 *
 * Previous Changes:
 **********************************************************************/

#ifndef _SPX_JY11_DECODER_H
#define _SPX_JY11_DECODER_H

/*
 * Other headers required.
 */

/* We need the SPxTargetDecoder base class. */
#include "SPxLibNet/SPxTargetDecoder.h"

class SPxJY11Decoder : public SPxTargetDecoder
{
public:
   /*
    * Public functions.
    */

    /* Constructor and destructor. */
    explicit SPxJY11Decoder(int initiallyDisabled);
    virtual ~SPxJY11Decoder(void);

private:
    /*
     * Private variables.
     */

    /*
     * Private functions.
     */
    void handlePacket(const unsigned char *payload, unsigned int numBytes);

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

}; /* SPxJY11Decoder */

#endif /* _SPX_JY11_DECODER_H */

/*********************************************************************
 *
 * End of file
 *
 **********************************************************************/

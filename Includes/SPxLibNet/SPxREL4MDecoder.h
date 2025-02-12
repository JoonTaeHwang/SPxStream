/*********************************************************************
 *
 * (c) Copyright 2021, Cambridge Pixel Ltd.
 *
 * Purpose:
 *	Header for SPxREL4MDecoder class which supports the receiving
 *	of track messages from a REL4M radar and translating
 *	them into SPx extended track messages.
 *
 *	REL4M track message content comes from reverse engineering
 *	of captured data from the radar as part of project P444.
 *
 *
 * Revision Control:
 *   23/11/21 v1.1   AJH 	Initial Version.
 *
 * Previous Changes:
 **********************************************************************/

#ifndef _SPX_REL4M_DECODER_H
#define _SPX_REL4M_DECODER_H

/*
 * Other headers required.
 */

/* We need the SPxTargetDecoder base class. */
#include "SPxLibNet/SPxTargetDecoder.h"

class SPxREL4MDecoder : public SPxTargetDecoder
{
public:
   /*
    * Public functions.
    */

    /* Constructor and destructor. */
    explicit SPxREL4MDecoder(int initiallyDisabled);
    virtual ~SPxREL4MDecoder(void);

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

}; /* SPxREL4MDecoder */

#endif /* _SPX_REL4M_DECODER_H */

/*********************************************************************
 *
 * End of file
 *
 **********************************************************************/

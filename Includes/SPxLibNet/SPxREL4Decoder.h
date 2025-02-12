/*********************************************************************
 *
 * (c) Copyright 2020-2021, Cambridge Pixel Ltd.
 *
 * Purpose:
 *	Header for SPxREL4Decoder class which supports the receiving
 *	of plot messages from a REL-4 radar and translating them into
 *	SPx extended track messages.
 *
 *	REL-4 plot message content reverse engineered from packet data
 *	obtained as part of project P444.
 *
 *
 * Revision Control:
 *   05/08/21 v1.4   AJH 	Add m_prevAzimuth.
 *
 * Previous Changes:
 *   06/10/20 1.3   BTB 	Make constructor explicit.
 *   04/02/20 1.2   BTB 	Add note explaining source of message data information.
 *   29/01/20 1.1   BTB 	Initial Version.
 **********************************************************************/

#ifndef _SPX_REL4_DECODER
#define _SPX_REL4_DECODER

/*
 * Other headers required.
 */

/* We need the SPxTargetDecoder base class. */
#include "SPxLibNet/SPxTargetDecoder.h"

class SPxREL4Decoder : public SPxTargetDecoder
{
public:
   /*
    * Public functions.
    */

    /* Constructor and destructor. */
    explicit SPxREL4Decoder(int initiallyDisabled);
    virtual ~SPxREL4Decoder(void);

private:
    /*
     * Private variables.
     */
    REAL32 m_prevAzimuth;		/* Keep track of the last plot azimuth sent. */

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

}; /* SPxREL4Decoder */

#endif /* _SPX_REL4_DECODER */

/*********************************************************************
 *
 * End of file
 *
 **********************************************************************/

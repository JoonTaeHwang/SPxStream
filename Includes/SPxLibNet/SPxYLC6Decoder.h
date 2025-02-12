/*********************************************************************
 *
 * (c) Copyright 2020-2021, Cambridge Pixel Ltd.
 *
 * Purpose:
 *	Header for SPxYLC6Decoder class which supports the receiving
 *	of track and plot messages from a YLC-6 radar and translating
 *	them into SPx extended track messages.
 *
 *	YLC-6 track and plot message content comes from a partial
 *	specification (confidential) obtained as part of project P444.
 *
 *
 * Revision Control:
 *   05/08/21 v1.5   AJH 	Add m_maxTrackID.
 *
 * Previous Changes:
 *   06/10/20 1.4   BTB 	Make constructor explicit.
 *   04/02/20 1.3   BTB 	Add note explaining source of message data information.
 *   28/01/20 1.2   BTB 	Remove now unused variables.
 *   24/01/20 1.1   BTB 	Initial Version.
 **********************************************************************/

#ifndef _SPX_YLC6_DECODER_H
#define _SPX_YLC6_DECODER_H

/*
 * Other headers required.
 */

/* We need the SPxTargetDecoder base class. */
#include "SPxLibNet/SPxTargetDecoder.h"

class SPxYLC6Decoder : public SPxTargetDecoder
{
public:
   /*
    * Public functions.
    */

    /* Constructor and destructor. */
    explicit SPxYLC6Decoder(int initiallyDisabled);
    virtual ~SPxYLC6Decoder(void);

private:
    /*
     * Private variables.
     */
    unsigned int m_maxTrackID;	    /* Track IDs above this value will be discarded. */

    /*
     * Private functions.
     */
    void handlePacket(const unsigned char *payload, unsigned int numBytes);
    void translateMessage(UINT16 *message, unsigned int messageSizeWords);

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

}; /* SPxYLC6Decoder */

#endif /* _SPX_YLC6_DECODER_H */

/*********************************************************************
 *
 * End of file
 *
 **********************************************************************/

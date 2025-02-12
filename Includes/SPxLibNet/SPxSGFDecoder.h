/*********************************************************************
 *
 * (c) Copyright 2020, Cambridge Pixel Ltd.
 *
 * Purpose:
 *	Header for SPxSGFDecoder class which supports the receiving
 *	of track and plot messages wrapped in an SGF1.1 header then
 *	translating them into SPx extended track messages.
 *
 *	SGF 1.1 header message content comes from a specification
 *	(confidential) obtained as part of project P478.
 *
 *
 * Revision Control:
 *   12/10/20 v1.1   BTB 	Initial Version.
 *
 * Previous Changes:
 **********************************************************************/

#ifndef _SPX_SGF_DECODER_H
#define _SPX_SGF_DECODER_H

/*
 * Other headers required.
 */

/* We need the SPxTargetDecoder base class. */
#include "SPxLibNet/SPxTargetDecoder.h"

class SPxSGFDecoder : public SPxTargetDecoder
{
public:
   /*
    * Public functions.
    */

    /* Constructor and destructor. */
    explicit SPxSGFDecoder(int initiallyDisabled);
    virtual ~SPxSGFDecoder(void);

private:
    /*
     * Private variables.
     */

    /*
     * Private functions.
     */
    void handlePacket(const unsigned char *payload, unsigned int numBytes);
    void translateMessage(UINT32 *message, unsigned int messageSizeWords);

    const char *getFormatString(UINT8 format);
    const char *getTypeString(UINT8 type);

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

}; /* SPxSGFDecoder */

#endif /* _SPX_SGF_DECODER_H */

/*********************************************************************
 *
 * End of file
 *
 **********************************************************************/

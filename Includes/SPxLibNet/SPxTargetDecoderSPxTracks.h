/*********************************************************************
 *
 * (c) Copyright 2021, Cambridge Pixel Ltd.
 *
 * Purpose:
 *	Header for SPxTargetDecoderSPxTracks class of object for
 *	receiving and decoding SPx track reports and passing
 *	them along.
 *
 *	This class effectively wraps the static functionality on
 *	SPxRemoteServer in a way which fits in with the architecture
 *	of the SPxTargetDecoder objects.
 *
 *
 * Revision Control:
 *   18/08/21 v1.1   BTB 	Initial Version.
 *
 * Previous Changes:	
 **********************************************************************/

#ifndef _SPX_TARGET_DECODER_SPX_TRACKS_H
#define _SPX_TARGET_DECODER_SPX_TRACKS_H

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

class SPxTargetDecoderSPxTracks : public SPxTargetDecoder
{
public:
    /*
     * Public functions.
     */

    /* Constructor and destructor. */
    explicit SPxTargetDecoderSPxTracks(int initiallyDisabled);
    virtual ~SPxTargetDecoderSPxTracks(void);

private:
    /*
     * Private variables.
     */

    /*
     * Private functions.
     */
    void handleMinTrack(const unsigned char *payload, unsigned int numBytes);
    void handleNormTrack(const unsigned char *payload, unsigned int numBytes);
    void handleExtTrack(const unsigned char *payload, unsigned int numBytes);
    void handleTrackerStatus(const unsigned char *payload, unsigned int numBytes);

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

}; /* SPxTargetDecoderSPxTracks */

#endif /* _SPX_TARGET_DECODER_SPX_TRACKS_H */

/*********************************************************************
 *
 * End of file
 *
 **********************************************************************/

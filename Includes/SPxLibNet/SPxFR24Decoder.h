/*********************************************************************
*
* (c) Copyright 2020, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxFR24Decoder class which supports the receipt
*	of FR24 ADSB data as Gzipped JSON strings.
*
*
* Revision Control:
*   04/08/20 v1.1    BTB	Initial Version.
*
* Previous Changes:
**********************************************************************/

#ifndef _SPX_FR24_DECODE_H
#define _SPX_FR24_DECODE_H

/* Include our base class. */
#include "SPxLibNet/SPxADSBDecoder.h"

/*********************************************************************
*
*   Constants
*
**********************************************************************/

/*********************************************************************
*
*   Macros
*
**********************************************************************/

/*********************************************************************
*
*   Type definitions
*
**********************************************************************/

/* Forward declarations. */
class SPxCompress;

/*
 * Define our class, derived from the generic ADSB decoder.
 */
class SPxFR24Decoder :public SPxADSBDecoder
{
public:
    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    SPxFR24Decoder(int initiallyDisabled=FALSE);
    virtual ~SPxFR24Decoder(void);

protected:
    /*
     * Protected functions.
     */
    virtual SPxErrorCode decodePacket(UINT32 packetType,
                                      struct sockaddr_in *from,
                                      SPxTime_t *timestamp,
                                      const unsigned char *payload,
                                      unsigned int numBytes);

private:
    /*
     * Private variables.
     */
    struct impl;
    SPxAutoPtr<impl> m_p;

    /*
     * Private functions.
     */
    SPxErrorCode handleJSONMessage(const char *jsonMesg);

}; /* SPxFR24Decoder */

#endif /* _SPX_FR24_DECODE_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/

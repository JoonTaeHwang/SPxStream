/*********************************************************************
 *
 * (c) Copyright 2018, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   Header for SPxRFTargetDtrRedrone class which supports the decoding
 *   of RF target network messages from a ReDrone detector.
 *
 * Revision Control:
 *   25/10/18 v1.2   SP 	Support changes to SPxRFTargetDetector.
 *
 * Previous Changes:
 *   15/10/18 1.1   SP 	Initial version created from SPxTargetDetector.
 *
 *********************************************************************/

#ifndef _SPX_RF_TARGET_DTR_REDRONE_H
#define _SPX_RF_TARGET_DTR_REDRONE_H

/*
 * Other headers required.
 */

/* Other classes that we need. */
#include "SPxLibUtils/SPxAutoPtr.h"
#include "SPxLibNet/SPxRFTargetDetector.h"
#include "SPxLibNet/SPxPacketDecoderNet.h"

/* Forward declare any classes required. */

/*********************************************************************
 *
 *   Constants
 *
 *********************************************************************/

/*********************************************************************
 *
 *   Macros
 *
 *********************************************************************/

/*********************************************************************
 *
 *   Type definitions
 *
 *********************************************************************/

/*
 * Define our class.
 */
class SPxRFTargetDtrRedrone : public SPxRFTargetDetector
{
public:

    /*
     * Public types.
     */

    /*
     * Public variables.
     */

    /*
     * Public functions.
     */

    /* Constructor and destructor. */
    SPxRFTargetDtrRedrone(SPxRFTargetDatabase *database);
    virtual ~SPxRFTargetDtrRedrone(void);
    virtual SPxErrorCode Create(void);

protected:

    /*
     * Protected variables.
     */

    /*
     * Protected functions.
     */

    /* Generic parameter assignment. */
    virtual int SetParameter(char *parameterName, char *parameterValue);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);

private:

    /*
     * Private types.
     */

    /*
     * Private variables.
     */

    /* Private member variables. */
    struct impl;
    SPxAutoPtr<impl> m_p;

    /* General. */
    SPxPacketDecoderNet *m_packetDecoder;

    /*
     * Private functions.
     */

    /* Message handling. */
    void decodePacket(SPxTime_t *timestamp,
                      const unsigned char *payload,
                      unsigned int numBytes);

    /*
     * Private static functions.
     */

    static void packetHandler(SPxPacketDecoder *decoder,
                              void *userArg,
                              UINT32 packetType,
                              struct sockaddr_in *from,
                              SPxTime_t *timestamp,
                              const unsigned char *payload,
                              unsigned int numBytes);

}; /* SPxRFTargetDtrRedrone */

/*********************************************************************
 *
 *   Function prototypes
 *
 *********************************************************************/

#endif /* SPX_RF_TARGET_DTR_REDRONE_H */

/*********************************************************************
 *
 * End of file
 *
 *********************************************************************/

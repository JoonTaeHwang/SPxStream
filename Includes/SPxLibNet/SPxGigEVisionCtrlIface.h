/*********************************************************************
*
* (c) Copyright 2030, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxGigEVisionCtrlIface class - an interface for
*	sending/receiving serial control messages to/from GigE Vision
*	cameras.
*
* Revision Control:
*   13/11/20 v1.0    AGC	Initial Version.
*
* Previous Changes:
**********************************************************************/
#ifndef _SPX_GIGE_VISION_CTRL_IFACE_H
#define _SPX_GIGE_VISION_CTRL_IFACE_H

#include "SPxLibUtils/SPxTypes.h"
#include "SPxLibUtils/SPxError.h"

/* Forward declarations. */
class PvDevice;

class SPxGigEVisionCtrlIface
{
public:
    typedef unsigned int (*SerialPortFn)(const unsigned char *payload,
					 unsigned int payloadBytes,
					 void *userArg);

    /* Construction and destruction. */
    SPxGigEVisionCtrlIface() {}
    virtual ~SPxGigEVisionCtrlIface() {}

    virtual int IsSerialConnected(void) const = 0;
    virtual SPxErrorCode SetSerialBaud(unsigned int serialBaud) = 0;
    virtual SPxErrorCode SendSerial(const void *command, unsigned int numBytes)=0;
    virtual SPxErrorCode SetSerialRecvFn(SerialPortFn fn, void *userArg)=0;

}; /* SPxGigEVisionCtrlIface */

#endif /* _SPX_GIGE_VISION_CTRL_IFACE_H */

/*********************************************************************
*
*   End of File
*
**********************************************************************/

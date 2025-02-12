/*********************************************************************
*
* (c) Copyright 2015, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxSerial.h,v $
* ID: $Id: SPxSerial.h,v 1.1 2015/03/26 09:42:07 rew Exp $
*
* Purpose:
*	Header for SPxSerial interface.
*
*	This is an interface only, which classes may derive from
*	in addition to deriving from a single base class.
*
* Revision Control:
*   26/03/15 v1.1    SP 	Initial version.
*
* Previous Changes:
*
**********************************************************************/
#ifndef _SPX_SERIAL_H
#define _SPX_SERIAL_H

/* For common types. */
#include "SPxLibUtils/SPxCommon.h"

/* For SPxErrorCode enumeration. */
#include "SPxLibUtils/SPxError.h"

/*********************************************************************
*
*   Type definitions
*
**********************************************************************/

/* Define our interface. */
class SPxSerial
{
public:
    /* Constructor/destructor do nothing. */
    SPxSerial(void)		{ return; }
    virtual ~SPxSerial(void)	{ return; }

    /* Required functions (All MUST be pure virtual). */
    virtual SPxErrorCode SetSerialName(const char *name)=0;
    virtual const char *GetSerialName(void) const=0;
    virtual SPxErrorCode SetSerialBaud(unsigned int baud)=0;
    virtual unsigned int GetSerialBaud(void) const=0;

}; /* SPxSerial */

#endif /* _SPX_SERIAL_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/

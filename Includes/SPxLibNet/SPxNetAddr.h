/*********************************************************************
*
* (c) Copyright 2012 - 2022, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxNetAddr interface.
*
*	All functions in this class MUST be pure virtual.
*	This is an interface only, which classes may derive from
*	in addition to deriving from a single base class.
*
* Revision Control:
*   21/06/22 v1.8    AGC	Add silent option to GetSenderPort().
*
* Previous Changes:
*   06/01/22 1.7    AGC	Add optional GetSubtype().
*   07/02/20 1.6    SP 	Add GetSenderPort().
*   23/06/15 1.5    AGC	Add more TCP control.
*   06/03/15 1.4    SP 	Add IsTCP().
*   22/11/13 1.3    AGC	Make get functions const.
*   31/08/12 1.2    REW	Add virtual destructor to keep icc happy.
*   31/08/12 1.1    AGC	Initial Version.
**********************************************************************/
#ifndef _SPX_NET_ADDR_H
#define _SPX_NET_ADDR_H

/* For common types. */
#include "SPxLibUtils/SPxCommon.h"

/* For SPxErrorCode enumeration. */
#include "SPxLibUtils/SPxError.h"

/* For SPxSdbStream::Subtype_t enumeration. */
#include "SPxLibUtils/SPxSdbStream.h"

/*********************************************************************
*
*   Type definitions
*
**********************************************************************/

/* Define our interface. */
class SPxNetAddr
{
public:
    /* Constructor/destructor do nothing. */
    SPxNetAddr(void)		{ return; }
    virtual ~SPxNetAddr(void)	{ return; }

    /* Required functions (All MUST be pure virtual). */
    virtual SPxErrorCode SetAddress(const char *address, int port=0, const char *ifAddr=NULL)=0;
    virtual SPxErrorCode SetAddress(UINT32 address, int port=0, const char *ifAddr=NULL)=0;
    virtual UINT32 GetAddress(void) const=0;
    virtual const char *GetAddressString(void) const=0;
    virtual int GetPort(void) const=0;
    virtual UINT32 GetIfAddress(void) const=0;
    virtual const char *GetIfAddressString(void) const=0;

    /* Optional functions (must return a sensible default, but may be overriden). */
    virtual int IsTCPSupported(void) const { return FALSE; }
    virtual SPxErrorCode SetTCP(int /*isTCP*/) { return SPX_ERR_NOT_SUPPORTED; }
    virtual int IsTCP(void) const { return FALSE; }
    virtual int GetSenderPort(int /*silent*/ = FALSE) const { return 0; }
    virtual SPxErrorCode GetSubtype(SPxSdbStream::Subtype_t * /*subtype*/) const { return SPX_ERR_NOT_SUPPORTED; }

}; /* SPxNetAddr */

#endif /* _SPX_NET_ADDR_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/

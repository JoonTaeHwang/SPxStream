/*********************************************************************
*
* (c) Copyright 2019 - 2020, Cambridge Pixel Ltd.
*
*
* Purpose:
*	Header of SPxMultiNetAddr class - a class which derives from
*	SPxNetAddr and allows SPxNetAddr object pointers to be installed.
*	This makes it possible to control multiple objects deriving
*	from SPxNetAddr using the same calls/GUI items/config parameters.
*
* Revision Control:
*   19/02/20 v1.3    BTB	Move parameter definitions to source file.
*
* Previous Changes:
*   07/03/19 1.2    BTB	Fix line endings.
*   05/03/19 1.1    BTB	Initial Version.
**********************************************************************/
#ifndef _SPX_MULTI_NET_ADDR_H
#define _SPX_MULTI_NET_ADDR_H

/* For SPxAutoPtr class. */
#include "SPxLibUtils/SPxAutoPtr.h"

/* For base classes. */
#include "SPxLibNet/SPxNetAddr.h"
#include "SPxLibUtils/SPxObj.h"

/*********************************************************************
*
*   Constants
*
*********************************************************************/

/*
 * Define our class.
 */
class SPxMultiNetAddr : public SPxNetAddr, public SPxObj
{
public:
    /* Constructor and destructor. */
    SPxMultiNetAddr(void);
    virtual ~SPxMultiNetAddr(void);

    /* Function for installing SPxNetAddr object pointers. */
    void InstallNetAddrObject(SPxNetAddr *obj);

    /* SPxNetAddr interface. */
    virtual SPxErrorCode SetAddress(const char *address, int port, const char *ifAddr);
    virtual SPxErrorCode SetAddress(UINT32 address, int port, const char *ifAddr);
    virtual UINT32 GetAddress(void) const;
    virtual const char *GetAddressString(void) const;
    virtual int GetPort(void) const;
    virtual UINT32 GetIfAddress(void) const;
    virtual const char *GetIfAddressString(void) const;

    /* Generic get/set parameter interface. */
    virtual int SetParameter(char *parameterName, char *parameterValue);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);

private:
    struct impl;
    SPxAutoPtr<impl> m_p;

}; /* SPxMultiNetAddr */

#endif /* _SPX_MULTI_NET_ADDR_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/

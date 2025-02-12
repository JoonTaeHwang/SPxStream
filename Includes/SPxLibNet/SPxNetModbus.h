/*********************************************************************
*
* (c) Copyright 2018, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for network Modbus class.
*
*
* Revision Control:
*   05/08/18 v1.3    AJH    Add Disconnect().
*
* Previous Changes:
*   20/06/18 1.2    AGC	Add Connect() function.
*   15/06/18 1.1    AGC	Initial Version.
**********************************************************************/

#ifndef _SPX_NET_MODBUS_H
#define _SPX_NET_MODBUS_H

/* For base classes. */
#include "SPxLibUtils/SPxObj.h"
#include "SPxLibNet/SPxNetAddr.h"

class SPxNetModbus : public SPxObj, public SPxNetAddr
{
public:
    typedef void(*CompletionFn)(void *userArg,
				SPxErrorCode err);

    /* Constructor, destructor etc. */
    SPxNetModbus();
    virtual ~SPxNetModbus();

    virtual SPxErrorCode SetAddress(const char *address, int port = 0, const char *ifAddr = NULL);
    virtual SPxErrorCode SetAddress(UINT32 address, int port = 0, const char *ifAddr = NULL);
    virtual UINT32 GetAddress() const;
    virtual const char *GetAddressString() const;
    virtual int GetPort() const;
    virtual UINT32 GetIfAddress() const;
    virtual const char *GetIfAddressString() const;

    int IsConnected() const;
    void Connect();
    void Disconnect();

    SPxErrorCode ForceCoil(CompletionFn fn, void *userArg,
			   UINT16 coil, int enable, UINT8 unitID=255);

private:
    struct impl;
    SPxAutoPtr<impl> m_p;

    void connect();
    static void forceCoilComplete(SPxNetTCP *tcp,
				  SPxErrorCode errorCode,
				  SPxNetTCPAction action,
				  void* userArg,
				  const unsigned char* buffer,
				  unsigned int bufLen,
				  UINT32 fromAddr,
				  SOCKET_TYPE sock);

}; /* SPxNetModbus class */

#endif /* _SPX_NET_MODBUS_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/

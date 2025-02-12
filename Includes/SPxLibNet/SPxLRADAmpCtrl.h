/*********************************************************************
*
* (c) Copyright 2019 - 2020, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxLRADAmpCtrl class - a class
*	to allow control of the SAM (amplifier) on an LRAD.
*
* Revision Control:
*   19/02/20 v1.3   BTB	Move parameter definitions to source file.
*
* Previous Changes:
*   07/03/19 1.2   BTB	Make destructor virtual.
*   01/03/19 1.1   BTB	Initial Version.
*
**********************************************************************/
#ifndef _SPX_LRAD_AMP_CTRL_H
#define _SPX_LRAD_AMP_CTRL_H

/*
 * Other headers required.
 */

/* For SPxNetTCPAction enum. */
#include "SPxLibNet/SPxNetTCP.h"

/* For base classes. */
#include "SPxLibNet/SPxNetAddr.h"
#include "SPxLibUtils/SPxObj.h"

/*********************************************************************
*
*   Constants
*
*********************************************************************/

/*********************************************************************
*
*   Type definitions
*
**********************************************************************/

/* Forward declarations. */
class SPxNetTCP;

/*
 * Define our class.
 */
class SPxLRADAmpCtrl : public SPxObj, public SPxNetAddr
{
public:
    /* Construction and destruction. */
    SPxLRADAmpCtrl(void);
    virtual ~SPxLRADAmpCtrl(void);

    int IsConnected(void) const;

    virtual SPxErrorCode SetAddress(const char *address, int port = 0, const char *ifAddr = NULL);
    virtual SPxErrorCode SetAddress(UINT32 address, int port = 0, const char *ifAddr=NULL);
    virtual UINT32 GetAddress(void) const;
    virtual const char *GetAddressString(void) const;
    virtual int GetPort(void) const;
    virtual UINT32 GetIfAddress(void) const;
    virtual const char *GetIfAddressString(void) const;
    virtual int IsTCPSupported(void) const { return TRUE; }
    virtual int IsTCP(void) const { return TRUE; }

    /* Generic get/set parameter interface. */
    virtual int SetParameter(char *parameterName, char *parameterValue);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);

    SPxErrorCode SetVolume(int volume);
    int GetVolume(int *volumeRtn) const;

    SPxErrorCode SetIsMuted(int muted);
    int GetIsMuted(int *mutedRtn) const;

    int GetTemperature(double *tempRtn) const;

protected:

private:
    /* Private variables. */
    struct impl;
    SPxAutoPtr<impl> m_p;	/* Private class structure. */

    /* Private functions. */
    void checkConnection(void);
    void requestStatusUpdate(void);
    void processResponse(const unsigned char *responseBuffer, unsigned int bufLen);

    /* Private static functions. */
    static void cmdSendComplete(SPxNetTCP *tcp, SPxErrorCode errorCode,
				SPxNetTCPAction action,
				void* userArg,
				const unsigned char* buffer,
				unsigned int bufLen,
				UINT32 fromAddr,
				SOCKET_TYPE sock);

}; /* SPxLRADAmpCtrl */

#endif /* _SPX_LRAD_AMP_CTRL_H */

/*********************************************************************
*
*   End of File
*
**********************************************************************/

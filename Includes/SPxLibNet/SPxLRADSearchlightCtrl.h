/*********************************************************************
*
* (c) Copyright 2019 - 2020, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxLRADSearchlightCtrl class - a class
*	to allow control of the (optional) searchlight on an LRAD.
*
* Revision Control:
*   19/02/20 v1.4   BTB	Move parameter definitions to source file.
*
* Previous Changes:
*   07/03/19 1.3   BTB	Make destructor virtual.
*   15/02/19 1.2   BTB	Support fetching and reporting of state.
*   14/02/19 1.1   BTB	Initial Version.
*
**********************************************************************/
#ifndef _SPX_LRAD_SEARCHLIGHT_CTRL_H
#define _SPX_LRAD_SEARCHLIGHT_CTRL_H

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

enum SPxLRADSearchlightBeamLevel
{
    SPX_LRAD_SEARCHLIGHT_BEAM_LEVEL_LOW = 0,
    SPX_LRAD_SEARCHLIGHT_BEAM_LEVEL_NORMAL = 1,
    SPX_LRAD_SEARCHLIGHT_BEAM_LEVEL_HIGH = 2
};

/* Forward declarations. */
class SPxNetTCP;

/*
 * Define our class.
 */
class SPxLRADSearchlightCtrl : public SPxObj, public SPxNetAddr
{
public:
    /* Construction and destruction. */
    SPxLRADSearchlightCtrl(void);
    virtual ~SPxLRADSearchlightCtrl(void);

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

    SPxErrorCode EnableBeam(int enable);
    int GetBeamEnableStatus(int *enableStatusRtn);

    SPxErrorCode SetBeamLevel(SPxLRADSearchlightBeamLevel beamLevel);
    int GetBeamLevelStatus(SPxLRADSearchlightBeamLevel *beamLvlStatusRtn);

    SPxErrorCode SetZoomPercentage(unsigned int zoomPercent);
    int GetZoomPercentageStatus(unsigned int *percentStatusRtn);

    SPxErrorCode EnableStrobe(int enable);
    int GetStrobeEnableStatus(int *enableStatusRtn);

    SPxErrorCode SetStrobeRate(unsigned int strobeRateHz);
    int GetStrobeRateStatus(unsigned int *rateStatusRtn);

protected:

private:
    /* Private variables. */
    struct impl;
    SPxAutoPtr<impl> m_p;	/* Private class structure. */

    /* Private functions. */
    void checkConnection(void);
    void requestStatusUpdate(void);
    void processStatusUpdate(const char *statusBuffer, unsigned int bufLen);

    /* Private static functions. */
    static void requestStatusComplete(SPxNetTCP *tcp, SPxErrorCode errorCode,
				      SPxNetTCPAction action,
				      void* userArg,
				      const unsigned char* buffer,
				      unsigned int bufLen,
				      UINT32 fromAddr,
				      SOCKET_TYPE sock);

}; /* SPxLRADSearchlightCtrl */

#endif /* _SPX_LRAD_SEARCHLIGHT_CTRL_H */

/*********************************************************************
*
*   End of File
*
**********************************************************************/

/*********************************************************************
*
* (c) Copyright 2022, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxCamCtrlWavelength class - a library
*	class for Wavelength UC-5 control.
*
* Revision Control:
*   11/10/22 v1.4    AGC	Handle AreChecksEnabled() calls during shutdown.
*
* Previous Changes:
*   07/10/22 1.3    AGC	Support second address/port.
*   27/09/22 1.2    AGC	Suppress spurious cppcheck warnings.
*   26/09/22 1.1    AGC	Initial Version.
**********************************************************************/
#ifndef _SPX_CAM_CTRL_WAVELENGTH_H
#define _SPX_CAM_CTRL_WAVELENGTH_H

/* For SPxAutoPtr. */
#include "SPxLibUtils/SPxAutoPtr.h"

/* For base class. */
#include "SPxLibData/SPxCamCtrlBase.h"

/*********************************************************************
*
*   Type/constant definitions
*
**********************************************************************/

/* Forward declarations. */
class SPxCamera;
class SPxCamCtrlBase;

/*
 * Define our class.
 */
class SPxCamCtrlWavelength : public SPxCamCtrlBase
{
public:
    /* Construction and destruction. */
    explicit SPxCamCtrlWavelength(SPxCamera *camera, SPxCamCtrlBase *lrf);
    virtual ~SPxCamCtrlWavelength(void);
    virtual int IsTcpSendAsync(void) const { return FALSE; }
    virtual SPxErrorCode SetMesgType(SPxMesgIOType_t mesgType);
    
    virtual int IsLaserDazzlerSupported(void) const { return TRUE; }

    // cppcheck-suppress virtualCallInConstructor
    virtual SPxErrorCode SetLaserDazzlerOn(int on);
    virtual int IsLaserDazzlerOn(void) const;

    // cppcheck-suppress virtualCallInConstructor
    virtual SPxErrorCode SetLaserDazzlerStrobe(int on);
    virtual int IsLaserDazzlerStrobe(void) const;

    virtual const char *GetLaserDazzlerMessage(void) const;

    SPxErrorCode SetAddress2(const char *address, int port = 0, const char *ifAddr = NULL);
    SPxErrorCode SetAddress2(UINT32 address, int port = 0, const char *ifAddr = NULL);
    UINT32 GetAddress2(void) const;
    const char *GetAddressString2(void) const;
    int GetPort2(void) const;
    UINT32 GetIfAddress2(void) const;
    const char *GetIfAddressString2(void) const;

    SPxErrorCode SetEnableChecks(int enableChecks);
    int AreChecksEnabled(void) const;

    SPxErrorCode SetMinDistanceMetres(double minGroundDistance);
    double GetMinDistanceMetres(void) const;
    SPxErrorCode SetMaxDazzleSecs(double maxDazzleSecs);
    double GetMaxDazzleSecs(void) const;
    SPxErrorCode SetMinPanAngleDegs(double minPanAngleDegs);
    SPxErrorCode SetMaxPanAngleDegs(double maxPanAngleDegs);
    SPxErrorCode SetMinTiltAngleDegs(double minTiltAngleDegs);
    SPxErrorCode SetMaxTiltAngleDegs(double maxTiltAngleDegs);
    double GetMinPanAngleDegs(void) const;
    double GetMaxPanAngleDegs(void) const;
    double GetMinTiltAngleDegs(void) const;
    double GetMaxTiltAngleDegs(void) const;


    /* Generic get/set parameter interface. */
    virtual int SetParameter(char *parameterName, char *parameterValue);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);

private:
    /* Private variables. */
    struct impl;
    int m_shutdown;
    SPxAutoPtr<impl> m_p;

    /* Private functions. */
    int isDazzleAllowed(void) const;
    SPxErrorCode setLaserDazzlerOn(int on);
    SPxErrorCode sendMessage(const char *mesgID, unsigned int val);
    void timer(void);

    static int cameraDeleted(void *invokingObject,
			     void *userObject,
			     void *arg);
    static int lrfDeleted(void *invokingObject,
			  void *userObject,
			  void *arg);

}; /* SPxCamCtrlWavelength */

#endif /* _SPX_CAM_CTRL_WAVELENGTH_H */

/*********************************************************************
*
*   End of File
*
**********************************************************************/

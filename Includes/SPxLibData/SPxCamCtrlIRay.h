/*********************************************************************
*
* (c) Copyright 2020, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxCamCtrlIRay class - a library
*	class for IRay camera control.
*
* Revision Control:
*   13/11/20 V1.13   AGC	Support control over GigE Vision.
*
* Previous Changes:
*   22/06/20 1.12   AJH	Put NUC control in base class.
*   18/06/20 1.11   AJH	Add support for NUC control.
*   05/06/20 1.10   AJH	Read sensor size from camera.
*   03/06/20 1.9    AJH	Add black body enable.
*   01/06/20 1.8    AJH	Add connect timer handler.
*   29/05/20 1.7    AJH	Add isotherm control.
*   20/05/20 1.6    AJH	Add retrieval of sensor image size.
*   19/05/20 1.5    AJH	Move some functionality to SPxCamera class.
*   18/05/20 1.4    AJH	Build fixes.
*   15/05/20 1.3    AJH	Further implementation.
*   14/05/20 1.2    AJH	Further implementation.
*   12/05/20 1.1    AJH	Initial Version.
**********************************************************************/
#ifndef _SPX_CAM_CTRL_IRAY_H
#define _SPX_CAM_CTRL_IRAY_H

/*
 * Other headers required.
 */
#include "SPxLibUtils/SPxTimer.h"

/* For base class. */
#include "SPxLibData/SPxCamCtrlBase.h"

/*********************************************************************
*
*   Type/constant definitions
*
**********************************************************************/

#define IRAY_NUM_SPOT	10
#define IRAY_NUM_AREA	12

struct IRayCoords
{
    unsigned int x;
    unsigned int y;
};

/* Forward declarations. */
class SPxGigEVisionCtrlIface;
#ifdef USE_SDK
class CHyvDataApp;
#endif

/*
 * Define our class.
 */
class SPxCamCtrlIRay : public SPxCamCtrlBase
{
    /* Types. */
    typedef enum
    {
	PRESET_AUTO_FOCUS = 1,
	PRESET_FREEZE = 2,
	PRESET_PALETTE = 3,
	PRESET_AUTO_FOCUS_ONESHOT = 4
    } IRayPresets;

public:
    /* Construction and destruction. */
    explicit SPxCamCtrlIRay(void);
    virtual ~SPxCamCtrlIRay(void);
    virtual SPxErrorCode Create(const char *serialName = NULL,
	unsigned int serialBaud = 2400);

    SPxErrorCode SetGigEVisionCtrl(SPxGigEVisionCtrlIface *gevCtrl,
				   SPxObj *obj);
    virtual int IsConnected(void) const;

    /* Camera control functions. */
    virtual int IsRelFocusSupported(void) const { return TRUE; }
    virtual SPxErrorCode Focus(float /*focusSpeed*/, int /*continuePanTilt*/ = FALSE);
    virtual int IsAutoFocusSupported(void) const { return TRUE; }
    virtual SPxErrorCode AutoFocus(int /*autoFocus*/);
    virtual int IsAutoFocusEnabled(void) const { return m_autoFocus; }
    virtual int IsAutoFocusOneshotSupported(void) const { return TRUE; }
    virtual SPxErrorCode AutoFocusOneshot(void);
    virtual SPxErrorCode GoToPreset(UINT8 id);
    virtual SPxErrorCode SetPalette(unsigned int palette);
    virtual SPxErrorCode GetPalette(unsigned int *palette) const;
    virtual SPxErrorCode SetFreeze(int freeze);
    virtual SPxErrorCode GetFreeze(int *freeze) const;
    virtual SPxErrorCode SetBlackBodyTemperature(double temp);
    virtual SPxErrorCode GetBlackBodyTemperature(double *temp) const;
    virtual SPxErrorCode SetBlackBodyPosition(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2);
    virtual SPxErrorCode GetBlackBodyPosition(unsigned int *x1, unsigned int *y1, unsigned int *x2, unsigned int *y2) const;
    virtual SPxErrorCode SetBlackBodyEnabled(int enabled);
    virtual SPxErrorCode GetBlackBodyEnabled(int *enabled) const;
    virtual SPxErrorCode GetSensorSizePixels(unsigned int *width, unsigned int *height) const;
    virtual SPxErrorCode SetNUCAuto(int nucAuto);
    virtual SPxErrorCode GetNUCAuto(int *nucAuto) const;
    virtual SPxErrorCode SetNUCInterval(int interval);
    virtual SPxErrorCode GetNUCInterval(int *interval) const;

    SPxErrorCode SetAGC(unsigned int agc);
    unsigned int GetAGC(void) { return m_agc; }
    SPxErrorCode SetBrightness(unsigned int brightness);
    unsigned int GetBrightness(void) { return m_brightness; }
    SPxErrorCode SetContrast(unsigned int contrast);
    unsigned int GetContrast(void) { return m_contrast; }
    SPxErrorCode SetDigitalZoom(double zoom);
    double GetDigitalZoom(void) { return m_digitalZoom; }
    SPxErrorCode SetSpotCoordinates(unsigned int index, IRayCoords spot);
    SPxErrorCode GetSpotCoordinates(unsigned int index, IRayCoords *spot);
    SPxErrorCode GetSpotTemperature(unsigned int index, double *temp);
    SPxErrorCode SetAreaCoordinates(unsigned int index, IRayCoords c1, IRayCoords c2);
    SPxErrorCode GetAreaCoordinates(unsigned int index, IRayCoords *c1, IRayCoords *c2);
    SPxErrorCode GetAreaHighTemperature(unsigned int index, double *temp);
    SPxErrorCode SetIsotherm(int isotherm);
    int GetIsotherm(void) { return m_isotherm; }

    /* Generic get/set parameter interface. */
    virtual int SetParameter(char *parameterName, char *parameterValue);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);

private:
    /* Private variables. */
    SPxGigEVisionCtrlIface *m_gevCtrl;
    SPxObj *m_gevObj;
    mutable unsigned int m_ackReceived;
    mutable unsigned int m_responseReceived;
    mutable unsigned int m_commandWordReply;
    mutable unsigned int m_operationWordReply;
    mutable unsigned char m_returnedValue[128];
    mutable unsigned int m_returnedValueCount;
    float m_focusSpeed;
    unsigned int m_autoFocus;
    int m_freeze;
    unsigned int m_palette;
    unsigned int m_agc;
    unsigned int m_brightness;
    unsigned int m_contrast;
    double m_digitalZoom;
    double m_blackBodyTemperature;
    unsigned int m_sensorWidth;
    unsigned int m_sensorHeight;
    IRayCoords m_blackBodyCoordinates[2];
    int m_blackBodyEnabled;
    IRayCoords m_areaCoordinates[IRAY_NUM_AREA][2];
    IRayCoords m_spotCoordinates[IRAY_NUM_SPOT];
    int m_isotherm;
    int m_nucAuto;
    int m_nucMode;
    int m_nucInterval;
#ifdef USE_SDK   
    mutable CHyvDataApp *m_hyvDataApp;
#endif
    unsigned int m_connected;
    UINT32 m_reconnectTimer;
    SPxTimer m_timer;
    mutable SPxCriticalSection m_mutex;

    /* Private functions. */
    int isConnected(void) const;
    virtual unsigned int SerialPortFn(const unsigned char *payload, unsigned int payloadBytes);
    SPxErrorCode sendCommand(unsigned int commandWord,
	unsigned char operationWord,
	const unsigned char *param,
	unsigned int numParam);
    SPxErrorCode waitForResponse(unsigned int commandWord, UINT32 timeoutMS);
    static UINT8 calcChecksum(const unsigned char *msg, unsigned int msgBytes);
    void handleFocusTimer(void);
    SPxErrorCode updateFocus(void);
    void handleConnectTimer(void);
    SPxErrorCode readSensorSizePixels(void);
    static unsigned int gevSerialPortFn(const unsigned char *payload,
					unsigned int payloadBytes,
					void *userArg);
    static int gevCtrlDeletedFn(void *invokingObject,
				void *userObject,
				void *arg);

}; /* SPxCamCtrlIRay */

#endif /* _SPX_CAM_CTRL_IRAY_H */

/*********************************************************************
*
*   End of File
*
**********************************************************************/

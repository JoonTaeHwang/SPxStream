/*********************************************************************
*
* (c) Copyright 2018, 2019, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for project-408 network receiving module.
*
*
* Revision Control:
*   25/06/19 v1.2    AGC	Derive from SPxPlotSourceIface.
*
* Previous Changes:
*   19/10/18 1.1    AGC	Initial Version.
**********************************************************************/

#ifndef _SPX_NETRECVP408_H
#define _SPX_NETRECVP408_H

/*
 * Other headers required.
 */
 /* We need the base class headers (which includes several others as well). */
#include "SPxLibNet/SPxNetRecv.h"
#include "SPxLibData/SPxPlotSourceIface.h"

/*********************************************************************
*
*   Constants
*
**********************************************************************/


/*********************************************************************
*
*   Macros
*
**********************************************************************/


/*********************************************************************
*
*   Type definitions
*
**********************************************************************/

/* Forward declare any classes we use. */
class SPxRIB;
class SPxNetTCP;
class SPxCallbackList;

/*
 * Define our specific network receiver class, derived from the
 * standard one.
 */
class SPxNetworkReceiveP408 :public SPxNetworkReceive, public SPxPlotSourceIface
{
public:
    enum AziRes
    {
	AZI_RES_15 = 0,
	AZI_RES_30 = 1,
	AZI_RES_60 = 2,
	AZI_RES_NONE = 3
    };

    /* Constructor, destructor etc. */
    explicit SPxNetworkReceiveP408(SPxRIB *buffer);
    virtual ~SPxNetworkReceiveP408(void);
    virtual SPxErrorCode Create(const char *addr=NULL, int port=0,
				const char *ifAddr=NULL);
    virtual UINT32 GetMsecsSinceLastReturn(void) { return 0; }
    virtual UINT32 GetMsecsSinceLastAziChange(void) { return 0; }
    virtual SPxErrorCode GetInitialised(void);

    /* Control address. */
    virtual SPxErrorCode SetControlAddress(const char *addr, int port = 0,
					   const char *ifAddr = NULL);
    virtual const char *GetControlAddress(void) const;
    virtual int GetControlPort(void) const;
    virtual const char *GetControlIfAddress(void) const;

    /* Configuration. */
    SPxErrorCode SetFrameRate(unsigned int frameRate);
    unsigned int GetFrameRate(void) const;
    SPxErrorCode SetMaxRangeMetres(double maxRangeMetres);
    double GetMaxRangeMetres(void) const;
    SPxErrorCode SetMaxRadialVelocityMps(double maxRadialVelocityMps);
    double GetMaxRadialVelocityMps(void) const;
    SPxErrorCode SetAzimuthResolution(AziRes aziRes);
    AziRes GetAzimuthResolution(void) const;
    SPxErrorCode SetRemoveClutter(int removeClutter);
    int GetRemoveClutter(void) const;
    SPxErrorCode SetCfarThreshold(double cfarThreshold);
    double GetCfarThreshold(void) const;
    SPxErrorCode SetDopplerThreshold(double dopplerThreshold);
    double GetDopplerThreshold(void) const;

    /* Callback handling. */
    virtual SPxErrorCode AddPlotCallback(SPxCallbackListFn_t fn, void *userObj);
    virtual SPxErrorCode RemovePlotCallback(SPxCallbackListFn_t fn, void *userObj);
    virtual SPxErrorCode AddStatusCallback(SPxCallbackListFn_t fn, void *userObj);
    virtual SPxErrorCode RemoveStatusCallback(SPxCallbackListFn_t fn, void *userObj);

    /* Generic parameter handling. */
    virtual int SetParameter(char *name, char *value);
    virtual int GetParameter(char *name, char *valueBuf, int bufLen);

    virtual SPxErrorCode SetStateFromConfig(void);

private:
    struct impl;
    SPxAutoPtr<impl> m_p;

    /*
     * Private functions.
     */
    SPxErrorCode setConfig(void);
    void setChannelConfig(unsigned int numTx,
			  unsigned int numRx);
    void setProfileConfig(double interChirpDuration,
			  double chirpDuration,
			  double rampSlope,
			  double numAdcSamples,
			  double adcSamplingRate);
    void setChirpConfig(unsigned int numTx);
    void setFrameConfig(unsigned int numTx);
    void setPeakGroupingConfig(double numAdcSamples);
    SPxErrorCode setCfarConfig(unsigned int numTx,
			       unsigned int numRx,
			       double cfarThreshold,
			       double dopplerThreshold);
    SPxErrorCode setClutterConfig(int removeClutter);
    void setCQRxSatMonitorConfig(double numAdcSamples,
				 double adcSamplingRate);
    void setCQSigImgConfig(double numAdcSamples);
    SPxErrorCode sendControl(const char *msg);
    unsigned int getNumTx(void);
    unsigned int getNumRx(void);

    /* Overwrite the base class functions we want to handle ourselves. */
    virtual int ThreadStateChangedFn(void);
    virtual int ThreadHandleDataFn(unsigned char *data,
				   unsigned int dataSizeBytes);
    unsigned int processDetections(const unsigned char *data,
				   unsigned int numBytes);
    SPxErrorCode decodeDetection(const unsigned char *buf,
				 unsigned int numBytes,
				 UINT16 xyzQFormat);

    /* Radar video functions. */
    SPxErrorCode outputSpoke(UINT16 azi16,
			     int includeTarget,
			     double rangeMetres);

}; /* SPxNetworkReceiveP408 class */

/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* _SPX_NETRECVP408_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/

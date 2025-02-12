/*********************************************************************
*
* (c) Copyright 2013 - 2019, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for Radar Simulator target input classes.
*
*	These classes can be used to manage the input of external
*	targets to a Radar Simulator. External targets can be SPx,
*	Asterix, AIS or ADSB by using the appropriate derived class.
*
*
* Revision Control:
*   25/11/21 v1.21   AGC	Use current platform pos when sensor lat/long not supplied.
*
* Previous Changes:
*   20/11/19 1.20   AGC	Support AIS receiver nav data configuration.
*   14/10/19 1.19   AGC	Suppress cppcheck warning.
*   06/06/19 1.18   AGC	Avoiding binding inactive input sockets.
*   25/06/18 1.17   AGC	Correctly suppress cppcheck warnings.
*   15/06/18 1.16   AGC	Set thread name.
*   06/03/18 1.15   AGC	Update ASTERIX status handler.
*   18/01/18 1.14   AGC	Use private structure for SPxSimTargetRecvAIS.
*   11/12/17 1.13   AGC	Update ASTERIX track handler.
*   06/10/17 1.12   AGC	Fix N/C 480 - detect track input/output loop.
*   15/03/17 1.11   AGC	Add functions for testing.
*   31/10/16 1.10   AGC	Support TTM input.
*   23/08/16 1.9    AGC	Improve mutex locking.
*   24/03/16 1.8    AGC	Preserve more AIS information.
*   07/11/14 1.7    AGC	Update radar sim event handler.
*   14/10/14 1.6    AGC	Support simulated/test bits.
*   16/05/14 1.5    AGC	Support applying received radar period to radars.
*   04/10/13 1.4    AGC	Simplify headers.
*   06/06/13 1.3    AGC	Update ADS-B decoder lat/long.
*   07/05/13 1.2    AGC	Use lat/long from tracker status messages.
*   03/05/13 1.1    AGC	Initial Version.
**********************************************************************/

#ifndef _SPX_SIM_TARGET_RECV_H
#define _SPX_SIM_TARGET_RECV_H

/*
 * Other headers required.
 */

/* For SPxAutoPtr. */
#include "SPxLibUtils/SPxAutoPtr.h"

/* For SPxSimTargetFlags enumeration. */
#include "SPxLibData/SPxSimTarget.h"

/* For SPxTimerDelay enumeration. */
#include "SPxLibUtils/SPxTimer.h"

/* For base class. */
#include "SPxLibUtils/SPxObj.h"

/*
 * Types
 */

/* Forward declarations. */
class SPxRadarSimulator;
class SPxRemoteServer;
struct SPxTime_tag;
struct SPxPacketTrackMinimal_tag;
struct SPxPacketTrackNormal_tag;
struct SPxPacketTrackExtended_tag;
class SPxAsterixDecoder;
class SPxAsterixMsg;
class SPxAISDecoder;
class SPxADSBDecoder;
struct SPxRadarSimEventParams;

/* Base class for handling track receipt. */
class SPxSimTargetRecv : public SPxObj, public SPxLock
{
public:
    /* Public functions. */
    explicit SPxSimTargetRecv(SPxRadarSimulator *radarSim,
	SPxSimTargetFlags inputType);
    virtual ~SPxSimTargetRecv(void);

    virtual SPxErrorCode Enable(int enable)=0;
    int IsEnabled(void) const;

    virtual int IsNetworkInput(void) const = 0;
    virtual UINT32 GetAddress(void) const = 0;
    virtual int GetPort(void) const = 0;

    /* Generic parameter setting/getting. */
    virtual int SetParameter(char *parameterName, char *parameterValue);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);

protected:
    /* Protected functions. */
    SPxSimTarget *GetTarget(void);
    unsigned int GetPlatformIndex(void) const;
    static void trackFnWrapper(SPxRemoteServer *remoteSvr, void *userArg,
	sockaddr_in *from, SPxTime_tag *timestamp,
	SPxPacketTrackMinimal_tag *minRpt, SPxPacketTrackNormal_tag *normRpt,
	SPxPacketTrackExtended_tag *extRpt);
    static void statusFnWrapper(SPxRemoteServer *remoteSvr, void *userArg,
	struct sockaddr_in *from, SPxTime_tag *timestamp,
	struct SPxPacketTrackerStatus_tag *status);

    /* SPxLock interface. */
    virtual SPxErrorCode Enter(void);
    virtual SPxErrorCode Leave(void);

    void SetTimerName(const char *timerName);

    /* Allow derived classes to install functions into the base class timer. */
    template<typename F, typename O>
    SPxErrorCode Schedule(F task, SPxTimerDelay delayType,
			  UINT32 initialDelayMsecs, double intervalMsecs,
			  O *obj);
    template<typename F, typename O>
    SPxErrorCode Unschedule(F task, O *obj, int inFunc = FALSE);

private:
    /* Private variables. */
    struct impl;
    SPxAutoPtr<impl> m_p;   /* Private class structure. */

    /* Private functions. */
    void trackFn(SPxPacketTrackMinimal_tag *minRpt,
		 SPxPacketTrackNormal_tag *normRpt,
		 SPxPacketTrackExtended_tag *extRpt);
    void statusFn(SPxPacketTrackerStatus_tag *status);
    void delTargetFn(void);
    static SPxErrorCode checkTargetFn(unsigned int index,
				      SPxSimTarget *target,
				      void *userArg);
    static SPxErrorCode radarFn(unsigned int index,
				SPxSimRadar *radar,
				void *userArg);

}; /* SPxSimTargetRecv */

/* Class for handling SPx track receipt. */
class SPxSimTargetRecvSPx : public SPxSimTargetRecv
{
public:
    /* Public functions. */
    explicit SPxSimTargetRecvSPx(SPxRadarSimulator *radarSim);
    virtual ~SPxSimTargetRecvSPx(void);

    // cppcheck-suppress virtualCallInConstructor
    virtual SPxErrorCode Enable(int enable);

    virtual int IsNetworkInput(void) const { return TRUE; }
    virtual UINT32 GetAddress(void) const;
    virtual int GetPort(void) const { return m_port; }

    /* Generic parameter setting/getting. */
    virtual int SetParameter(char *parameterName, char *parameterValue);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);

private:
    /* Private variables. */
    SPxAutoPtr<SPxRemoteServer> m_remoteSvr;	/* Remote server for receiving tracks. */
    SPxAtomic<int> m_enabled;			/* Is the receiver enabled? */
    char m_addr[64];				/* IP address. */
    UINT16 m_port;				/* Port. */

    /* Private functions. */
    SPxErrorCode enablePriv(int enable, int reset,
			    int mutexAlreadyLocked);
    void handleReports(void);

}; /* SPxSimTargetRecvSPx */

/* Class for handling Asterix track receipt. */
class SPxSimTargetRecvAsterix : public SPxSimTargetRecv
{
public:
    /* Public functions. */
    explicit SPxSimTargetRecvAsterix(SPxRadarSimulator *radarSim);
    virtual ~SPxSimTargetRecvAsterix(void);

    // cppcheck-suppress virtualCallInConstructor
    virtual SPxErrorCode Enable(int enable);

    virtual int IsNetworkInput(void) const { return TRUE; }
    virtual UINT32 GetAddress(void) const;
    virtual int GetPort(void) const;

    /* Generic parameter setting/getting. */
    virtual int SetParameter(char *parameterName, char *parameterValue);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);

private:
    /* Private variables. */
    SPxAutoPtr<SPxAsterixDecoder> m_asterix;	/* Asterix decoder for receiving tracks. */

    /* Private functions. */
    static void asterixFn(const SPxAsterixDecoder *decoder,
			  void *userArg,
			  struct SPxPacketTrackExtended_tag *extRpt,
			  const SPxAsterixMsg *asterixMsg);
    static void asterixStatusFn(const SPxAsterixDecoder *decoder,
				void *userArg,
				struct SPxPacketTrackerStatus_tag *status,
				const SPxAsterixMsg *asterixMsg);

}; /* SPxSimTargetRecvAsterix */

/* Class for handling AIS track receipt. */
class SPxSimTargetRecvAIS : public SPxSimTargetRecv
{
public:
    /* Public functions. */
    explicit SPxSimTargetRecvAIS(SPxRadarSimulator *radarSim);
    virtual ~SPxSimTargetRecvAIS(void);

    // cppcheck-suppress virtualCallInConstructor
    virtual SPxErrorCode Enable(int enable);

    void SetNavData(SPxNavData *navData, unsigned int navDataID=0);
    SPxNavData *GetNavData(unsigned int navDataID=0);

    virtual int IsNetworkInput(void) const;
    virtual UINT32 GetAddress(void) const;
    virtual int GetPort(void) const;

    /* Generic parameter setting/getting. */
    virtual int SetParameter(char *parameterName, char *parameterValue);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);

protected:
    /* Protected functions for testing. */
    SPxSimTargetRecvAIS(SPxRadarSimulator *radarSim,
			SPxAISDecoder *aisDecoder);

private:
    /* Private variables. */
    struct impl;
    SPxAutoPtr<impl> m_p;

    /* Private functions. */
    void init(void);
    static int aisFn(void *invokingObject,
		     void *userObject,
		     void *arg);

}; /* SPxSimTargetRecvAIS */

/* Class for handling ADSB track receipt. */
class SPxSimTargetRecvADSB : public SPxSimTargetRecv
{
public:
    /* Public functions. */
    SPxSimTargetRecvADSB(SPxRadarSimulator *radarSim);
    virtual ~SPxSimTargetRecvADSB(void);

    // cppcheck-suppress virtualCallInConstructor
    virtual SPxErrorCode Enable(int enable);

    virtual int IsNetworkInput(void) const;
    virtual UINT32 GetAddress(void) const;
    virtual int GetPort(void) const;

    /* Generic parameter setting/getting. */
    virtual int SetParameter(char *parameterName, char *parameterValue);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);

private:
    /* Private variables. */
    SPxRadarSimulator *m_radarSim;		/* Radar Simulator. */
    SPxAutoPtr<SPxADSBDecoder> m_adsbDecoder;	/* ADSB decoder for receiving tracks. */
    int m_enabled;				/* Is this receiver enabled? */
    SPxMesgIOType_t m_srcType;			/* Source type for receiver. */
    SPxAtomic<int> m_latLongUpdateRequired;	/* Does the ADSB decoder lat/long require an update? */
    unsigned int m_maxAgeSecs;			/* Max age of data to treat as valid target. */

    /* Private functions. */
    static int adsbFn(void *invokingObject,
		      void *userObject,
		      void *arg);
    static SPxErrorCode eventFn(UINT32 flags,
				const SPxRadarSimEventParams *params, void *userArg);

}; /* SPxSimTargetRecvADSB */

/* Class for handling TTM track receipt. */
class SPxSimTargetRecvTTM : public SPxSimTargetRecv
{
public:
    /* Public functions. */
    SPxSimTargetRecvTTM(SPxRadarSimulator *radarSim);
    virtual ~SPxSimTargetRecvTTM(void);

    // cppcheck-suppress virtualCallInConstructor
    virtual SPxErrorCode Enable(int enable);

    virtual int IsNetworkInput(void) const;
    virtual UINT32 GetAddress(void) const;
    virtual int GetPort(void) const;

    /* Generic parameter setting/getting. */
    virtual int SetParameter(char *parameterName, char *parameterValue);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);

private:
    /* Private variables. */
    SPxAutoPtr<SPxPacketDecoderNet> m_ttmRecvNet;
    SPxAutoPtr<SPxSerialPort> m_ttmRecvSerial;
    SPxAtomic<int> m_enabled;
    SPxMesgIOType_t m_srcType;
    SPxNMEADecoder m_nmeaDecoder;

    /* Private functions. */
    void handleReports(void);
    static void ttmNetFn(SPxPacketDecoder *obj,
			 void *userArg,
			 UINT32 packetType,
			 struct sockaddr_in *from,
			 struct SPxTime_tag *timePtr,
			 const unsigned char *payload,
			 unsigned int numBytes);
    static void ttmSerialFn(SPxSerialPort *serial,
			    void *userArg,
			    SPxTime_tag *timePtr,
			    const unsigned char *payload,
			    unsigned int numBytes);
    void ttmFn(const unsigned char *payload,
	       unsigned int numBytes);
};

#endif /* _SPX_SIM_TARGET_RECV_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/

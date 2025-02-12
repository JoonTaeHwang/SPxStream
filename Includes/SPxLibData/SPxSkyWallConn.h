/*********************************************************************
*
* (c) Copyright 2022, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxSkyWallConn class - a class
*	to communicate with a SkyWall system.
*
* Revision Control:
*   11/03/22 v1.5    AGC	Fix build warnings.
*
* Previous Changes:
*   11/03/22 1.4    AGC	Support updating NavData.
*   11/03/22 1.3    AGC	Pass track altitude to SendTrack().
*   08/03/22 1.2    AGC	Fix warnings.
*   04/03/22 1.1    AGC	Initial Version.
**********************************************************************/
#ifndef _SPX_SKY_WALL_CONN
#define _SPX_SKY_WALL_CONN

/* For base class. */
#include "SPxLibNet/SPxVideoTrackerExt.h"

/* For SPxNetTCPAction etc. */
#include "SPxLibNet/SPxNetTCP.h"

/* Forward declarations. */
class SPxNavData;

class SPxSkyWallConn : public SPxVideoTrackerExt
{
public:
    /* Construction and destruction. */
    explicit SPxSkyWallConn(int port, const char *ifAddr, SPxNavData *navData=NULL);
    virtual ~SPxSkyWallConn(void);

    /* Address/port. */
    virtual SPxErrorCode SetAddress(const char *address, int port=0, const char *ifAddr=NULL) SPX_VIRTUAL_OVERRIDE;
    virtual SPxErrorCode SetAddress(UINT32 address, int port=0, const char *ifAddr=NULL) SPX_VIRTUAL_OVERRIDE;
    virtual UINT32 GetAddress(void) const SPX_VIRTUAL_OVERRIDE;
    virtual const char *GetAddressString(void) const SPX_VIRTUAL_OVERRIDE;
    virtual int GetPort(void) const SPX_VIRTUAL_OVERRIDE;
    virtual UINT32 GetIfAddress(void) const SPX_VIRTUAL_OVERRIDE;
    virtual const char *GetIfAddressString(void) const SPX_VIRTUAL_OVERRIDE;
    virtual int IsTCP(void) const SPX_VIRTUAL_OVERRIDE{ return TRUE; }

    int IsConnected(void) const;

    float GetAziDegs(void) const;
    float GetEleDegs(void) const;
    float GetFOVDegs(unsigned int index) const;
    float GetFocusRangeMetres(unsigned int index) const;

    virtual int AlwaysSendSelectedTrack(void) const SPX_VIRTUAL_OVERRIDE { return TRUE; }

    /* Video tracking control. */
    virtual int IsTrackingEnabled(UINT8 /*subSystemIndex*/) const SPX_VIRTUAL_OVERRIDE { return FALSE; }
    virtual int IsTracking(UINT8 /*subSystemIndex*/) const SPX_VIRTUAL_OVERRIDE { return FALSE; }
    virtual SPxErrorCode SendTrack(UINT32 id,
				   double rangeMetres,
				   double bearingDegs,
				   double elevationDegs,
				   double altitudeMetres,
				   const char *name,
				   int isRadarTrackingEnabled,
				   int isVideoTrackingEnabled) SPX_VIRTUAL_OVERRIDE;
 
    /* Generic get/set parameter interface. */
    virtual int SetParameter(char *parameterName, char *parameterValue) SPX_VIRTUAL_OVERRIDE;
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen) SPX_VIRTUAL_OVERRIDE;

    void SetLogFile(const char *logFileName);
    const char *GetLogFile(void) const;
    void SetVerbosity(UINT32 verbosity);
    UINT32 GetVerbosity(void) const;

private:
    /* Private variables. */
    struct impl;
    SPxAutoPtr<impl> m_p;

    static SPxErrorCode validateChecksum(const unsigned char *payload,
					 unsigned int numBytes);
    static void addChecksum(unsigned char *payload,
			    unsigned int numBytes);
    static UINT16 calcChecksum(const unsigned char *payload,
			       unsigned int numBytes);

    static void connectWrapper(SPxNetTCP *tcp,
			       void *userArg,
			       UINT32 fromAddr,
			       SOCKET_TYPE sock);
    static void disconnectWrapper(SPxNetTCP *tcp,
				  void *userArg,
				  SOCKET_TYPE sock);
    static void recvDataWrapper(SPxNetTCP *tcp,
				SPxErrorCode errorCode,
				SPxNetTCPAction action,
				void* userArg,
				const unsigned char* buffer,
				unsigned int bufLen,
				UINT32 fromAddr,
				SOCKET_TYPE sock);

    void processRecvData(void);
    void processMessage(const unsigned char *data, unsigned int dataLen);
    void processAck(const unsigned char *data, unsigned int dataLen);
    void processErr(const unsigned char *data, unsigned int dataLen);
    void processCfg(const unsigned char *data, unsigned int dataLen);
    void processStatus(const unsigned char *data, unsigned int dataLen);
    void processLocation(const unsigned char *data, unsigned int dataLen);
    void processUIData(const unsigned char *data, unsigned int dataLen);

    void buildC2Config(void);
    void buildC2Status(void);
    void buildC2ReqLocation(void);
    void buildC2CfgUIData(void);
    void sendMesg(void);

    void clearSendBuffer(void);
    template<typename T>
    void addToSendBuffer(T val);

    void timer(void);

    void
#if defined(__GNUC__) || defined(__clang__)
	__attribute__((format(printf, 3, 4)))
#endif
    writeDebug(UINT32 verbosity, const char *format, ...) const;

    static int navDataDeleted(void *invokingObject,
			      void *userObject,
			      void *arg);


}; /* SPxSkyWallConn */

#endif /* _SPX_SKY_WALL_CONN */

/*********************************************************************
*
*   End of File
*
**********************************************************************/

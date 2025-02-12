/*********************************************************************
 *
 * (c) Copyright 2018-2022, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   Class for accepting P322 control commands and forwarding them
 *   to the appropriate controller object.
 *
 *
 * Revision Control:
 *  26/08/22 v1.10   AGC	Suppress spurious cppcheck warnings.
 *
 * Previous Changes:
 *  29/03/21 1.9    AGC	Avoid always binding to default control port.
 *  12/07/19 1.8    AJH	Remove setInhibitTransmit().
 *  15/05/19 1.7    AJH	Support default serial number.
 *			Automatically save IP setup to flash and restart.
 *  09/05/19 1.6    AJH	Remove redundant serial number.
 *  07/08/18 1.5    AGC	Improve blind sector application.
 *  20/07/18 1.4    AGC	Add saveAddresses param to Save callback.
 *  20/07/18 1.3    AGC	Support more radar functionality.
 *  18/07/18 1.2    AGC	Report errors in status message.
 *  16/07/18 1.1    AGC	Initial version.
 **********************************************************************/
#ifndef SPX_NET_CTRL_P322_H_
#define SPX_NET_CTRL_P322_H_

#include "SPxLibUtils/SPxCommon.h"
#include "SPxLibUtils/SPxObj.h"
#include "SPxLibNet/SPxNetTCP.h"
#include "SPxLibNet/SPxNetAddr.h"

/* Forward declarations. */
class SPxP383RadarController;
class SPxSimRadar;
class SPxNetworkSendP322;
class SPxNetTCP;

class SPxNetworkCtrlP322 : public SPxObj
{
public:
    typedef SPxErrorCode(*SetNetworkFn)(UINT32 address1, UINT32 netmask1, UINT32 gateway1,
					UINT32 address2, UINT32 netmask2, UINT32 gateway2,
					void *userArg);
    typedef SPxErrorCode(*SetAntennaOffsetFn)(double degs, void *userArg);
    typedef double(*GetAntennaOffsetFn)(void *userArg);
    typedef SPxErrorCode(*SaveFn)(int saveAddress, void *userArg);
    typedef void(*RestartFn)(void *userArg);

    /* Constructor and destructor. */
    SPxNetworkCtrlP322(void);
    virtual ~SPxNetworkCtrlP322(void);

    SPxErrorCode SetRadar(SPxP383RadarController *radar);
    SPxErrorCode SetRadar(SPxSimRadar *radar);
    SPxErrorCode SetNetSend(SPxNetworkSendP322 *netSend1,
			    SPxNetworkSendP322 *netSend2);

    SPxErrorCode SetUseDiscovery(int useDiscovery);
    int GetUseDiscovery(void) const;

    /* Set network interfaces for all message output. */
    SPxErrorCode SetIfAddress1(const char *address);
    UINT32 GetIfAddress1(void) const;
    const char *GetIfAddressString1(void) const;
    SPxErrorCode SetIfAddress2(const char *address);
    UINT32 GetIfAddress2(void) const;
    const char *GetIfAddressString2(void) const;

    /* TCP port for control connections. */
    SPxErrorCode SetControlPort(UINT16 port);
    UINT16 GetControlPort(void) const;

    /* Network config for status messages. */
    SPxNetAddr *GetStatusSender1(void);
    SPxNetAddr *GetStatusSender2(void);
    SPxErrorCode SetStatusPort(int port);
    int GetStatusPort(void) const;
    SPxErrorCode SetStatusAddress1(UINT32 address);
    SPxErrorCode SetStatusAddress1(const char *address);
    UINT32 GetStatusAddress1(void) const;
    const char *GetStatusAddressString1(void) const;
    SPxErrorCode SetStatusAddress2(UINT32 address);
    SPxErrorCode SetStatusAddress2(const char *address);
    UINT32 GetStatusAddress2(void) const;
    const char *GetStatusAddressString2(void) const;

    SPxErrorCode SetRadarName(const char *radarName);
    const char *GetRadarName(void) const;
    SPxErrorCode SetHeightMetres(double heightMetres);
    double GetHeightMetres(void) const;

    SPxErrorCode SetAntennaOffset(UINT16 antennaOffset);

    SPxErrorCode SetSerialNumber(UINT32 serialNumber);
    UINT32 GetSerialNumber(void) const;

    SPxErrorCode SetNetworkCallback(SetNetworkFn fn, void *userArg);
    SPxErrorCode SetAntennaOffsetCallback(SetAntennaOffsetFn setFn,
					  GetAntennaOffsetFn getFn,
					  void *userArg);
    SPxErrorCode SetSaveCallback(SaveFn fn, void *userArg);
    SPxErrorCode SetRestartCallback(RestartFn fn, void *userArg);

    /* Generic parameter assignment. */
    virtual int SetParameter(char *parameterName, char *parameterValue);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);
    virtual SPxErrorCode SetStateFromConfig(void);

protected:


    /* Functions that may be overridden for testing. */
    virtual void SendDiscoveryPacket(void);
    virtual void SendStatusPacket(void);
    // cppcheck-suppress virtualCallInConstructor
    virtual void SendDiscoveryData(const void *buf,
				   unsigned int bufSizeBytes);
    // cppcheck-suppress virtualCallInConstructor
    virtual void SendStatusData(const void *buf,
				unsigned int bufSizeBytes);

private:
    struct SectorInfo;
    struct impl;
    SPxAutoPtr<impl> m_p;

    /* Private functions. */
    static void clientConnectWrapper(SPxNetTCP *tcp,
				     void *userArg,
				     SOCKET_TYPE sock);
    void clientConnect(SOCKET_TYPE sock);
    static void clientDisconnectWrapper(SPxNetTCP *tcp,
					void *userArg,
					SOCKET_TYPE sock);
    void clientDisconnect(SOCKET_TYPE sock);
    static void clientReceiveWrapper(SPxNetTCP *tcp,
				     SPxErrorCode errorCode,
				     SPxNetTCPAction action,
				     void* userArg,
				     const unsigned char* buffer,
				     unsigned int bufLen,
				     UINT32 fromAddr,
				     SOCKET_TYPE sock);
    void clientReceive(SPxErrorCode errorcode,
		       const unsigned char *buffer,
		       unsigned int bufLen,
		       UINT32 fromAddr,
		       SOCKET_TYPE sock);
    void purgeClients(void);

    unsigned int clientMesg(SOCKET_TYPE sock,
			    UINT32 fromAddr,
			    const unsigned char *buffer,
			    unsigned int bufLen);
    void processHello(SOCKET_TYPE sock,
		      UINT32 fromAddr,
		      const unsigned char *payload,
		      unsigned int payloadLen);
    void processHeartbeat(SOCKET_TYPE sock,
			  const unsigned char *payload,
			  unsigned int payloadLen);
    void processTimingControl(SOCKET_TYPE sock,
			      const unsigned char *payload,
			      unsigned int payloadLen);
    void processTransmitControl(SOCKET_TYPE sock,
				const unsigned char *payload,
				unsigned int payloadLen);
    void processIPSetup(SOCKET_TYPE sock,
			const unsigned char *payload,
			unsigned int payloadLen);
    void processSetup(SOCKET_TYPE sock,
		      const unsigned char *payload,
		      unsigned int payloadLen);
    void processMaintenance(SOCKET_TYPE sock,
			    const unsigned char *payload,
			    unsigned int payloadLen);
    void processCourse(SOCKET_TYPE sock,
		      const unsigned char *payload,
		      unsigned int payloadLen);
    void processFEStc(SOCKET_TYPE sock,
		      const unsigned char *payload,
		      unsigned int payloadLen);
    void processIFStc(SOCKET_TYPE sock,
		      const unsigned char *payload,
		      unsigned int payloadLen);
    void processStcCorrection(SOCKET_TYPE sock,
			      const unsigned char *payload,
			      unsigned int payloadLen);
    void processSave(SOCKET_TYPE sock,
		     const unsigned char *payload,
		     unsigned int bufLen);
    void saveIPConfig(void);

    void sendDiscoveryPacket(void);
    void sendStatusPacket(void);

    int inBlindSector(UINT16 aziNum);

    SPxErrorCode setHeightMetres(double heightMetres);
    SPxErrorCode setAntennaOffset(UINT16 antennaOffset);
    SPxErrorCode setBlindSectors(const SectorInfo& sectorInfo);
    SPxErrorCode setTestPatternEnabled(void);
    SPxErrorCode setStcMode(void);
    SPxErrorCode setTriggerDelay(void);
    SPxErrorCode setZeroRange(void);
    SPxErrorCode setTuningMode(void);
    SPxErrorCode setTransmitStatus(void);
    SPxErrorCode setTransmitMode(void);
    SPxErrorCode setNumRangeBins(void);
    SPxErrorCode setNumAzimuths(void);
    SPxErrorCode setAntennaRpm(void);
    SPxErrorCode setSectorBlankMode(void);
    SPxErrorCode setWidebandMode(void);
    SPxErrorCode setInterferenceRejection(void);
    SPxErrorCode setManualTuneVoltage(void);

    SPxErrorCode setConfigFromState(const char *paramList[]);

};

#endif /* SPX_NET_CTRL_P322_H_ */

/*********************************************************************
 *
 * End of file
 *
 **********************************************************************/

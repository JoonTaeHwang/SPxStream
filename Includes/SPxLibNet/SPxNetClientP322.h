/*********************************************************************
 *
 * (c) Copyright 2018, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   Class for sending P322 control commands.
 *
 * Revision Control:
 *  20/07/18 v1.2    AGC	Add functions for testing.
 *
 * Previous Changes:
 *  20/07/18 1.1    AGC	Initial version.
 **********************************************************************/
#ifndef SPX_NET_CLIENT_P322_H_
#define SPX_NET_CLIENT_P322_H_

#include "SPxLibUtils/SPxCommon.h"
#include "SPxLibUtils/SPxObj.h"
#include "SPxLibNet/SPxNetP322.h"
#include "SPxLibNet/SPxNetTCP.h"

/* Forward declarations. */
class SPxNetworkReceiveP322;
class SPxNetAddr;
class SPxPacketDecoder;
struct sockaddr_in;
struct SPxTime_tag;
class SPxNetTCP;

class SPxNetworkClientP322 : public SPxObj
{
public:
    typedef void(*IterateFn)(UINT32 fromAddress, const SPxNetP322MsgDiscovery *msg, void *userArg);

    /* Constructor and destructor. */
    SPxNetworkClientP322(void);
    virtual ~SPxNetworkClientP322(void);

    SPxErrorCode SetNetRecv(SPxNetworkReceiveP322 *netRecv);
    SPxNetworkReceiveP322 *GetNetRecv(void) const;

    SPxErrorCode SetDiscoveryAddress(const char *address, int port = 0, const char *ifAddr = NULL);
    SPxErrorCode SetDiscoveryAddress(UINT32 address, int port = 0, const char *ifAddr = NULL);
    UINT32 GetDiscoveryAddress(void) const;
    const char *GetDiscoveryAddressString(void) const;
    int GetDiscoveryPort(void) const;
    UINT32 GetDiscoveryIfAddress(void) const;
    const char *GetDiscoveryIfAddressString(void) const;
    SPxNetAddr *GetDiscoveryReceiver(void);

    SPxErrorCode SetUseDiscovery(int useDiscovery);
    int GetUseDiscovery(void) const;
    SPxErrorCode SetSerialNumber(unsigned int serialNumber);
    unsigned int GetSerialNumber(void) const;

    UINT32 GetUsedDiscoveryAddress(void) const;

    SPxErrorCode IterateDiscovered(IterateFn fn, void *userArg);
    SPxErrorCode GetDiscovered(UINT32 address,
			       SPxNetP322MsgDiscovery *msg) const;

    SPxErrorCode SetStatusAddress(const char *address, int port = 0, const char *ifAddr = NULL);
    SPxErrorCode SetStatusAddress(UINT32 address, int port = 0, const char *ifAddr = NULL);
    UINT32 GetStatusAddress(void) const;
    const char *GetStatusAddressString(void) const;
    int GetStatusPort(void) const;
    UINT32 GetStatusIfAddress(void) const;
    const char *GetStatusIfAddressString(void) const;
    SPxNetAddr *GetStatusReceiver(void);

    SPxErrorCode StartStatusThread(void);
    SPxErrorCode StopStatusThread(void);

    SPxErrorCode GetStatus(SPxNetP322MsgStatus *status) const;

    SPxErrorCode SetDisplayName(const char *displayName);
    const char *GetDisplayName(void) const;
    SPxErrorCode Connect(const char *address,
			 int port = 0,
			 const char *ifAddr = NULL);
    SPxErrorCode Disconnect(void);
    int IsConnected(void) const;

    SPxErrorCode SendTimingControl(const SPxNetP322MsgTimingControl *timingControl);
    SPxErrorCode SendTransmitControl(const SPxNetP322MsgTransmitControl *transmitControl);
    SPxErrorCode SendIPSetup(const SPxNetP322MsgIPSetup *ipSetup);
    SPxErrorCode SendSetup(const SPxNetP322MsgSetup *setup);
    SPxErrorCode SendSave(const SPxNetP322MsgSave *save);
    SPxErrorCode SendFEStc(const SPxNetP322MsgFEStc *feStc);
    SPxErrorCode SendIFStc(const SPxNetP322MsgIFStc *ifStc);
    SPxErrorCode SendStcCorrection(const SPxNetP322MsgStcCorrection *stcCorrection);
    SPxErrorCode SendMaintenance(const SPxNetP322MsgMaintenance *maintenance);

    /* Generic parameter assignment. */
    virtual int SetParameter(char *parameterName, char *parameterValue);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);

protected:
    /* Functions that may be called by testing classes. */
    SPxErrorCode handleDiscoveryPacket(UINT32 fromAddr,
				       const unsigned char *data,
				       unsigned int dataSizeBytes);
    SPxErrorCode handleStatusPacket(const unsigned char *data,
				    unsigned int dataSizeBytes);

private:
    struct impl;
    SPxAutoPtr<impl> m_p;

    static void handleDiscovery(SPxPacketDecoder *pktDecoder,
				void *userArg,
				UINT32 packetType,
				struct sockaddr_in *from,
				struct SPxTime_tag *time,
				const unsigned char *payload,
				unsigned int numBytes);
    void purgeDiscovered(void);

    static void handleStatus(SPxPacketDecoder *pktDecoder,
			     void *userArg,
			     UINT32 packetType,
			     struct sockaddr_in *from,
			     struct SPxTime_tag *time,
			     const unsigned char *payload,
			     unsigned int numBytes);

    static void connectComplete(SPxNetTCP *tcp,
				SPxErrorCode errorCode,
				SPxNetTCPAction action,
				void* userArg,
				const unsigned char* buffer,
				unsigned int bufLen,
				UINT32 fromAddr,
				SOCKET_TYPE sock);

    void sendHeartbeat(void);
    static void sendHeartbeatComplete(SPxNetTCP *tcp,
				      SPxErrorCode errorCode,
				      SPxNetTCPAction action,
				      void* userArg,
				      const unsigned char* buffer,
				      unsigned int bufLen,
				      UINT32 fromAddr,
				      SOCKET_TYPE sock);

    static int netRecvDeleted(void *invokingObject,
			      void *userObject,
			      void *arg);

};

#endif /* SPX_NET_CLIENT_P322_H_ */

/*********************************************************************
 *
 * End of file
 *
 **********************************************************************/

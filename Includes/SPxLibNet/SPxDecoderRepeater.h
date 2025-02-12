/*********************************************************************
 *
 * (c) Copyright 2011 - 2022, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   Header for SPxDecoderRepeater class.
 *
 * Revision Control:
 *  24/01/22 v1.32   AGC	Support DestNetTTL configuration.
 *
 * Previous Changes:
 *  18/05/21 1.31   REW	Add GetSrcTypeFixed().
 *  02/12/20 1.30   SP 	Stop decode thread when live input disabled.
 *  07/09/20 1.29   AJH	Add wrappers for network/serial address handling.
 *  06/02/20 1.28   BTB	Support TCP server connection/disconnection handling.
 *  05/09/19 1.27   AGC	Add missing forward declarations.
 *  25/04/19 1.26   REW	Add GetCurrentMessage().
 *  28/02/19 1.25   SP 	Add SetDestTypeFixed().
 *  22/11/18 1.24   AGC	Remove dependency on zlib.
 *  10/08/18 1.23   AGC	Support use as TCP server.
 *  17/04/18 1.22   AGC	Handle replayed changes packets.
 *  06/10/17 1.21   AGC	Make various functions const.
 *  15/03/17 1.20   AGC	Improve behaviour with testing constructor.
 *  27/09/16 1.19   AGC	Use atomics for src/dest type.
 *  04/08/16 1.18   SP 	Add SetLiveSrcEnabled().
 *  22/02/16 1.17   SP 	Add SetSrcTypeFixed().
 *  09/11/15 1.16   AGC	Add constructor overload for testing.
 *  03/11/15 1.15   AGC	Packet decoder callback data now const.
 *  23/06/15 1.14   AGC	Support TCP send/receive.
 *			Support device manufacturers.
 *  11/05/15 1.13   AGC	Support manual input of data.
 *  26/03/15 1.12   SP 	Make SetHandler() protected.
 *                      Add SetDirectHandler() for public use.
 *  26/01/15 1.11   SP 	Add GetPacketSenderNet().
 *  23/12/13 1.10   REW	Add GetPacketDecoderNet().
 *  03/05/13 1.9    AGC	Add GetSrcNetAddrString().
 *  19/04/13 1.8    REW	Support asterix derived class.
 *  20/03/13 1.7    SP 	Fix file replay not working when serial src selected. 
 *  25/01/13 1.6    AGC	Support raw network input/output. 
 *  14/01/13 1.5    AGC	Allow receipt of 2 packet types.
 *  02/08/12 1.4    AGC	Forward declare SPxRunProcess.
 *  23/12/11 1.3    REW	Add GetSerialPort().
 *  13/09/11 1.2    AGC	SPxSerialRecv renamed to SPxSerialPort.
 *  09/08/11 1.1    SP	Initial Version.
 **********************************************************************/

#ifndef _SPX_DECODER_REPEATER_H
#define _SPX_DECODER_REPEATER_H

/*
 * Other headers required.
 */

/* We need the various decoders and sources that we use. */
#include "SPxLibNet/SPxPacketSend.h"
#include "SPxLibNet/SPxPacketDecoderNet.h"
#include "SPxLibNet/SPxNetTCP.h"
#include "SPxLibData/SPxSerialPort.h"
#include "SPxLibProc/SPxProcStd.h"	    /* For recording support */
#include "SPxLibUtils/SPxCallbackList.h"

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

/* Forward declarations. */
class SPxRunProcess;
class SPxRadarReplayBase;
class SPxPacketDecoderNet;
class SPxNetTCP;
class SPxDecoderDestNetWrapper;
class SPxDecoderSrcNetWrapper;
class SPxDecoderSrcSerialWrapper;

/* Source type for messages */
typedef enum
{
    SPX_MESG_IO_TYPE_DISABLED = 0,
    SPX_MESG_IO_TYPE_NETWORK = 1,
    SPX_MESG_IO_TYPE_SERIAL = 2,
    SPX_MESG_IO_TYPE_NETWORK_RAW = 3,
    SPX_MESG_IO_TYPE_NETWORK_SERVER = 4

} SPxMesgIOType_t;

/*
 * Define our class.
 */
class SPxDecoderRepeater : public SPxObj
{
public:
    /* Recording callback function. */
    typedef SPxErrorCode(*RecordFn)(SPxRunProcess *rp,
                                    UINT16 packetType,
                                    const void *payload,
                                    unsigned int numBytes,
                                    UCHAR channelIndex);

    /*
     * Public functions.
     */

    /* Create and destroy */
    SPxDecoderRepeater(void);
    virtual ~SPxDecoderRepeater(void);
    virtual SPxErrorCode Create(UINT32 packetType, UINT32 packetType2=0);

    /* Handler function */
    virtual SPxErrorCode SetDirectHandler(SPxPacketDecoderFn_t fn, 
                                          void *userArg);

    /* Source control. */
    virtual SPxErrorCode SetSrcType(SPxMesgIOType_t srcType);
    virtual SPxMesgIOType_t GetSrcType(void) { return static_cast<SPxMesgIOType_t>(static_cast<int>(m_srcType)); }
    virtual void SetSrcTypeFixed(int isFixed);
    virtual int GetSrcTypeFixed(void) const { return(m_isSrcTypeFixed); }
    virtual SPxErrorCode SetSrcSerial(const char *name=NULL,
                                      unsigned int baud=4800);
    virtual const char *GetSrcSerialName(void) const;
    virtual unsigned int GetSrcSerialBaud(void) const;
    virtual SPxErrorCode SetSrcSerialManufacturer(SPxDeviceManufacturer_t manufacturer);
    virtual SPxDeviceManufacturer_t GetSrcSerialManufacturer(void) const;
    virtual SPxErrorCode SetSrcNetAddr(const char *addr,
                                       int port=0,
                                       const char *ifAddr=NULL);
    virtual SPxErrorCode SetSrcNetAddr(UINT32 addr,
                                       int port=0,
                                       const char *ifAddr=NULL);
    virtual SPxErrorCode SetSrcNetPort(int port);
    virtual SPxErrorCode SetSrcNetManufacturer(SPxDeviceManufacturer_t manufacturer);
    virtual SPxDeviceManufacturer_t GetSrcNetManufacturer(void);
    virtual SPxErrorCode SetSrcNetTCP(int isTCP);
    virtual UINT32 GetSrcNetAddr(void) const;
    virtual const char *GetSrcNetAddrString(void) const;
    virtual int GetSrcNetPort(void) const;
    virtual UINT32 GetSrcNetIfAddr(void) const;
    virtual const char *GetSrcNetIfAddrString(void) const;
    virtual int IsSrcNetTCP(void) const;
    virtual SPxErrorCode SetLiveSrcEnabled(int isEnabled);
    virtual int IsLiveSrcEnabled(void) const { return m_isLiveSrcEnabled; }

    /* Destination control. */
    virtual SPxErrorCode SetDestType(SPxMesgIOType_t destType);
    virtual SPxMesgIOType_t GetDestType(void) const { return static_cast<SPxMesgIOType_t>(static_cast<int>(m_destType)); }
    virtual void SetDestTypeFixed(int isFixed);
    virtual SPxErrorCode SetDestNetAddr(const char *addr,
                                        int port=0,
                                        const char *ifAddr=NULL);
    virtual SPxErrorCode SetDestNetAddr(UINT32 addr,
                                        int port=0,
                                        const char *ifAddr=NULL);
    virtual SPxErrorCode SetDestNetPort(int port);
    virtual SPxErrorCode SetDestNetTCP(int isTCP);
    virtual SPxErrorCode SetDestNetTTL(int ttl);
    virtual UINT32 GetDestNetAddr(void) const;
    virtual const char *GetDestNetAddrString(void) const;
    virtual int GetDestNetPort(void) const;
    virtual UINT32 GetDestNetIfAddr(void) const;
    virtual const char *GetDestNetIfAddrString(void) const;
    virtual int IsDestNetTCP(void) const;
    virtual int GetDestNetTTL(void) const;

    /* Access to wrappers for network control. */
    SPxNetAddr *GetDestNetAddrWrapper(void);
    SPxNetAddr *GetSrcNetAddrWrapper(void);
    SPxSerial *GetSrcSerialWrapper(void);

    /* Input from file replay. */
    virtual SPxErrorCode AddReplaySource(SPxRadarReplayBase *replaySource);

    /* Output to recording process. */
    virtual SPxErrorCode AddRecordingProcess(SPxRunProcess *recProc,
                                             RecordFn recordFn=SPxProRecordPacketB);

    /* Manual input. */
    void ManualInput(SPxTime_t *time,
                     const unsigned char *data,
                     unsigned int numBytes);

    /* Access to underlying packet decoder net class (required if adding
     * this to SPxNetworkRecord class).
     */
    SPxPacketDecoderNet *GetPacketDecoderNet(void) { return(m_netPacketDecoder); }

    SPxNetTCP *GetNetTCP(void) { return(m_netTCP); }

    /* Get network packet sender. */
    SPxPacketSender *GetPacketSenderNet(void) { return(m_netPacketSender); }

    /* Access to the message currently being decoded, for use where installed
     * functions need to come back to the original data (e.g. fusion sensor
     * wants the original message rather than the decoded AIS/ADS-B info).
     */
    SPxErrorCode GetCurrentMessage(const void **msgPtr,
					unsigned int *msgSizeBytesPtr);

    /* Parameters set/get */
    virtual int SetParameter(char *parameterName, char *parameterValue);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);

protected:
    /* Constructor for testing. */
    explicit SPxDecoderRepeater(SPxPacketDecoderNet *netPacketDecoder);
    explicit SPxDecoderRepeater(SPxNetTCP *netTCP);

    /* Let derived classes get access to some of the underlying objects. */
    SPxSerialPort *GetSerialPort(void)	{ return(m_serialRecv); }

    /* Let derived classes install message Handler function */
    virtual SPxErrorCode SetHandler(SPxPacketDecoderFn_t fn, void *userArg);

    /* Callbacks for connection and disconnection when acting as a TCP server. */
    SPxErrorCode AddTCPServerConnectCallback(SPxCallbackListFn_t fn, void *userObj);
    SPxErrorCode RemoveTCPServerConnectCallback(SPxCallbackListFn_t fn, void *userObj);
    SPxErrorCode AddTCPServerDisconnectCallback(SPxCallbackListFn_t fn, void *userObj);
    SPxErrorCode RemoveTCPServerDisconnectCallback(SPxCallbackListFn_t fn, void *userObj);

    SOCKET_TYPE GetCurrentSocket(void) const { return(m_currentSocket); }

private:

    /*
     * Private types.
     */

    /* Used to store replay sources in a linked list */
    typedef struct _ReplaySource
    {
        SPxRadarReplayBase *obj;
        SPxPacketDecoder *decoder;
        _ReplaySource *next;

    } ReplaySource_t;

    /* Used to store recording processes in a linked list */
    typedef struct _RecProc
    {
        SPxRunProcess *obj;
        RecordFn recordFn;
        _RecProc *next;

    } RecProc_t;

    /*
     * Private variables.
     */

    /* Our message type(s) */
    UINT32 m_packetType;
    UINT32 m_packetType2;

    /* Source */
    SPxAtomic<int> m_srcType;
    int m_isSrcTypeFixed;
    SPxPacketDecoderNet *m_netPacketDecoderSys;
    SPxPacketDecoderNet *m_netPacketDecoder;
    SPxSerialPort *m_serialRecv;
    SPxNetTCP *m_netTCPSys;
    SPxNetTCP *m_netTCP;
    int m_serialSupported;
    SPxAtomic<int> m_isLiveSrcEnabled;

    /* Dest */
    SPxAtomic<int> m_destType;
    int m_isDestTypeFixed;
    SPxPacketSender *m_netPacketSender;

    /* Wrappers for network address and serial interfaces. */
    SPxDecoderDestNetWrapper *m_destNetAddrWrapper;	/* Wrapper */
    SPxDecoderSrcNetWrapper *m_srcNetAddrWrapper;	/* Wrapper */
    SPxDecoderSrcSerialWrapper *m_srcSerialWrapper;	/* Wrapper */

    /* Callback functions */
    SPxPacketDecoderFn_t m_fn;
    void *m_fnUserArg;
    SPxPacketDecoderFn_t m_directFn;
    void *m_directFnUserArg;
    SOCKET_TYPE m_currentSocket;
    SPxCallbackList *m_srvConnCBList;
    SPxCallbackList *m_srvDisconnCBList;

    /* Linked list of replay sources */
    ReplaySource_t *m_replaySourceList;

    /* Linked list of recording processes */
    RecProc_t *m_recProcList;

    /* Current message being decoded. */
    const void *m_currentMsg;
    unsigned int m_currentMsgSizeBytes;

    /*
     * Private functions.
     */
    void init(SPxPacketDecoderNet *netPacketDecoder, SPxNetTCP *netTCP);
    SPxErrorCode setSrcTypeInternal(int srcType, int isLiveSrcEnabled);

    void handleTCPConnection(void);
    void handleTCPDisconnection(void);

    /* Message handler */
    void MesgHandler(SPxPacketDecoder *packetDecoder,
                     struct sockaddr_in *sockaddr,
                     SPxTime_t *time,
                     const unsigned char *data,
                     unsigned int numBytes);

    void ReplayChangesHandler(SPxPacketDecoder *packetDecoder,
                              const unsigned char *data,
                              unsigned int numBytes);


    /*
     * Private static functions.
     */
    static void NetRawMesgHandler(SPxPacketDecoder *source,
                                  void *userArg,
                                  UINT32 packetType,
                                  struct sockaddr_in *addr,
                                  SPxTime_t *time,
                                  const unsigned char *data,
                                  unsigned int numBytes);

    static void NetMesgHandler(SPxPacketDecoder *source,
                               void *userArg,
                               UINT32 packetType,
                               struct sockaddr_in *addr,
                               SPxTime_t *time,
                               const unsigned char *data,
                               unsigned int numBytes);

    static void SerialMesgHandler(SPxSerialPort *source,
                                  void *userArg,
                                  SPxTime_t *time,
                                  const unsigned char *data,
                                  unsigned int numBytes);

    static void TcpRawMesgHandler(SPxNetTCP *tcp,
                                  SPxErrorCode errorCode,
                                  SPxNetTCPAction action,
                                  void* userArg,
                                  const unsigned char* buffer,
                                  unsigned int bufLen,
                                  UINT32 fromAddr,
                                  SOCKET_TYPE sock);

    static void ReplayMesgHandler(SPxPacketDecoder *source,
                                  void *userArg,
                                  UINT32 packetType,
                                  struct sockaddr_in *sockAddr,
                                  SPxTime_t *time,
                                  const unsigned char *data,
                                  unsigned int numBytes);

    static void TCPConnectHandler(SPxNetTCP *tcp,
                                  void *userArg,
                                  SOCKET_TYPE sock);

    static void TCPDisconnectHandler(SPxNetTCP *tcp,
                                     void *userArg,
                                     SOCKET_TYPE sock);

}; /* SPxDecoderRepeater */

#endif /* _SPX_DECODER_REPEATER_H */

/*********************************************************************
 *
 * End of file
 *
 **********************************************************************/

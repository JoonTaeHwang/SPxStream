/*********************************************************************
 *
 * (c) Copyright 2012 - 2021, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   P172 radar control transport layer. Provides send and receive
 *   functions for P172 NMEA style messages that are transport
 *   independent. Currently serial, TCP network and CANBus transport is
 *   supported.
 *
 * Revision Control:
 *  04/11/21 v1.17   SP 	Fix formatting.
 *
 * Previous Changes:
 *  09/06/20 1.16   SP 	Fix build warning.
 *  08/06/20 1.15   SP 	Add IsConnected() and IsConnectionPending().
 *  22/06/18 1.14   AGC	Fix new cppcheck warnings.
 *  12/02/18 1.13   BTB	Add support for Upmast series radars.
 *                     	Add support for CANBus connection.
 *                     	Add Data <-> PCAN conversion code.
 *                     	Move CalcChecksum() from child class.
 *                     	Make CalcChecksum() public.
 *  26/11/15 1.12   SP 	Add Connect() for backward compatibility.
 *  03/11/15 1.11   AGC	Packet decoder callback data now const.
 *  23/09/15 1.10   SP 	Rename some functions.
 *                     	Make IsConnected() protected.
 *                     	Add ASTERIX Cat-253 setup functions.
 *  21/09/15 1.9    SP 	Add lock/unlock functions.
 *  01/04/14 1.8    SP 	Rename a function.
 *  17/02/14 1.7    SP 	Add support for SxV series radars.
 *  24/10/13 1.6    SP 	Add improved debug support.
 *  21/10/13 1.5    SP 	Support inclusion of time-of-day.
 *  15/10/13 1.4    SP 	Add functions to get connection settings.
 *  04/10/13 1.3    AGC	Simplify headers.
 *  08/04/13 1.2    SP 	Add support for network control.
 *  18/10/12 1.1    S P	Initial version.
 *
 *********************************************************************/

#pragma once

/*
 * Other headers required.
 */
#include <string.h>

/* The various classes that we use. */
#include "SPxLibUtils/SPxCallbackList.h"
#include "SPxLibUtils/SPxObj.h"
#include "SPxLibUtils/SPxCriticalSection.h"

/*********************************************************************
 *
 *   Constants
 *
 *********************************************************************/

/* Longest sentence length that is supported. Note that this 
 * limit is enforced by the ASTERIX Cat-253 network transport.
 */
#define SPX_P172_MAX_SENTENCE_LEN 256

/*********************************************************************
 *
 *   Macros
 *
 *********************************************************************/


/*********************************************************************
 *
 *   Type definitions
 *
 *********************************************************************/

/* Forward declarations. */
class SPxSerialPort;
class SPxNetCat253Client;
class SPxNetCat253;
class SPxCANBus;
struct SPxTime_tag;

/*
 * Define our class.
 */
class SPxP172RcTransportWin : public SPxObj
{
public:

    /*
     * Public types.
     */
    
    /* Text message handler function. */
    typedef void (*TextFn_t)(void *userArg, const char *text);

    /* Connection type. */
    typedef enum
    {
        CONNECTION_SERIAL,  /* Serial port. */
        CONNECTION_NETWORK,  /* TCP/IP network socket. */
        CONNECTION_CANBUS   /* CANBus via adapter (serial device). */

    } ConnectionType_t;

    /* Network message destination. */
    typedef enum
    {
        MESG_DEST_RDU,      /* Route to RDU. */
        MESG_DEST_XBAND,    /* Route to X-Band transceiver. */
        MESG_DEST_SBAND     /* Route to S-Band transceiver. */
    
    } MesgDest_t;
    
    /*
     * Public functions.
     */

    /* Create and destroy */
    SPxP172RcTransportWin(void);
    virtual ~SPxP172RcTransportWin(void);

    /* Serial transport setup. */
    virtual SPxErrorCode SetConnectionType(ConnectionType_t type);
    virtual ConnectionType_t GetConnectionType(void) { return m_connectionType; }
    virtual SPxErrorCode SetSerial(const char *name, unsigned int baud);
    virtual const char *GetSerialName(void) { return m_serialName ? m_serialName : ""; }
    virtual int GetSerialBaud(void) { return m_serialBaud; }

    /* Network transport setup. */
    virtual SPxErrorCode SetNetwork(const char *addr, int port);
    virtual const char *GetNetworkAddr(void) { return m_netAddr ? m_netAddr : ""; }
    virtual int GetNetworkPort(void) { return m_netPort; }
    virtual void SetNetworkSAC(UINT8 sac) { m_netSAC = sac; }
    virtual UINT8 GetNetworkSAC(void) { return m_netSAC; }
    virtual void SetNetworkSIC(UINT8 sic) { m_netSIC = sic; }
    virtual UINT8 GetNetworkSIC(void) { return m_netSIC; }
    virtual void SetNetworkLocalID(UINT8 localID) { m_netLocalID = localID; }
    virtual UINT8 GetNetworkLocalID(void) { return m_netLocalID; }
    virtual void SetNetworkRadarSAC(UINT8 sac) { m_netRadarSAC = sac; }
    virtual UINT8 GetNetworRadarSAC(void) { return m_netRadarSAC; }
    virtual void SetNetworkRadarXBandSIC(UINT8 sic) { m_netRadarXBandSIC = sic; }
    virtual UINT8 GetNetworRadarXBandSIC(void) { return m_netRadarXBandSIC; }
    virtual void SetNetworkRadarSBandSIC(UINT8 sic) { m_netRadarSBandSIC = sic; }
    virtual UINT8 GetNetworRadarSBandSIC(void) { return m_netRadarSBandSIC; }
    virtual void SetNetworkRadarLocalID(UINT8 localID) { m_netRadarLocalID = localID; }
    virtual UINT8 GetNetworkRadarLocalID(void) { return m_netRadarLocalID; }
    virtual void SetNetworkTODIncluded(BOOL isIncluded) { m_netIncludeTOD = isIncluded; }
    virtual BOOL IsNetworkTODIncluded(void) { return m_netIncludeTOD; }

    /* CANBus transport setup. */
    virtual unsigned int GetCANBusAdapterID(void) { return m_canbusAdapterID; }
    virtual void SetCANBusAdapterID(unsigned int adapterID) { m_canbusAdapterID = adapterID; }

    /* Connection. */
    virtual SPxErrorCode OpenConnection(void);
    virtual SPxErrorCode CloseConnection(void);
    virtual int IsConnected(void);
    virtual int IsConnectionPending(void);

    /* For backward compatibility... */
    virtual SPxErrorCode Connect(void) { return OpenConnection(); }

    /* Message sending and receiving. */
    virtual SPxErrorCode SendSentence(MesgDest_t dest, const char *sentence);
    virtual SPxErrorCode PollForMesgs(void);
    virtual SPxErrorCode Lock(void);
    virtual SPxErrorCode Unlock(void);

    /* Event and alarm reporting. */
    virtual void ReportEvent(const char *text);
    virtual void ReportAlarm(const char *text);

    /* Alarm and event handler callbacks. */
    virtual SPxErrorCode AddEventTextHandler(void *userArg, 
                                             SPxCallbackListFn_t fn);
    virtual SPxErrorCode AddAlarmTextHandler(void *userArg, 
                                             SPxCallbackListFn_t fn);

    virtual const char *GetPCANSentence(unsigned int sentenceIndex)
	{ return m_sentences[sentenceIndex].GetBuffer(); }

    /*
     * Public static functions.
     */

    /* Checksum calculation. */
    static UINT8 CalcChecksum(const char *sentence);

protected:

    /*
     * Protected variables.
     */

    UINT32 m_debugFlags;
    FILE *m_netDebugFile;

    /*
     * Protected functions.
     */

    /* Message handling. */
    virtual const char *HandleMesg(const char *sentence);

    /* Parameters set/get. */
    virtual int SetParameter(char *name, char *value);
    virtual int GetParameter(char *name, char *valueBuf, int bufLen);

    /* PCAN <-> Data message conversion. */
    unsigned int PCANtoData(const char *sentence, unsigned char *data);
    unsigned int DatatoPCAN(const unsigned char *data, unsigned int dataBytes);

private:

    /*
     * Private types.
     */

    /*
     * Private variables.
     */

    /* Connection settings. */
    ConnectionType_t m_connectionType;
    BOOL m_isConnected;

    /* Serial connection settings. */
    const char *m_serialName;
    unsigned int m_serialBaud;
    SPxSerialPort *m_serialPort;

    /* Network connection settings. */
    SPxNetCat253Client *m_network;
    const char *m_netAddr;
    int m_netPort;
    UINT8 m_netSAC;
    UINT8 m_netSIC;
    UINT8 m_netLocalID;
    UINT8 m_netRadarSAC;
    UINT8 m_netRadarXBandSIC;
    UINT8 m_netRadarSBandSIC;
    UINT8 m_netRadarLocalID;
    BOOL m_netIncludeTOD;

    /* CANBus connection settings. */
    SPxCANBus *m_canbus;
    unsigned int m_canbusAdapterID;

    /* Variables for PCAN to Data conversion. */
    unsigned int m_ptdMesgID;
    unsigned int m_ptdMesgToFollow;

    /* Variables for Data to PCAN conversion. */
    UINT8 m_dtpMesgNo;
    UINT8 m_dtpSID;
    UINT8 m_dtpDID;
    UINT16 m_dtpData;
    unsigned int m_dtpSentenceCount;
    /* Buffered PCAN sentences. */
    CStringA m_sentences[10];

    /* Alarm & event handlers. */
    SPxCallbackList *m_eventTextCbList;
    SPxCallbackList *m_alarmTextCbList;

    /* Mutex. */
    SPxCriticalSection m_mutex;

    /*
     * Private functions.
     */

    SPxErrorCode CloseConnectionInternal(void);

    /*
     * Private static functions.
     */

    /* Message handlers. */
    static void serialMesgHandler(SPxSerialPort *serialPort,
                                  void *userArg,
                                  SPxTime_tag *time,
                                  const unsigned char *data,
                                  unsigned int numBytes);

    static void networkMesgHandler(SPxNetCat253 *obj,
                                   void *arg,
                                   const UINT8 *appData,
                                   unsigned int numAppDataBytes);

    static unsigned int canBusMesgHandler(SPxSerialPort *serialPort,
					  void *userArg,
					  SPxTime_tag *time,
					  const unsigned char *data,
					  unsigned int numBytes);

}; /* SPxP172RcTransportWin */

/*********************************************************************
 *
 * End of file
 *
 *********************************************************************/

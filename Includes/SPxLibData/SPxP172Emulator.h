/*********************************************************************
*
* (c) Copyright 2015 - 2016, 2018, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxP172Emulator.
*
* Revision Control:
*   30/11/18 v1.14   BTB	Store all strings from parameters in own memory.
*
* Previous Changes:
*   14/03/18 1.13   AGC	Support format warnings with clang on Windows.
*   06/03/18 1.12   AGC	Support log() format warnings.
*   11/01/16 1.11   AJH	Add m_lastCommandBytesLeft.
*   06/01/16 1.10   AJH	Remove m_pkinfString.
*   23/12/15 1.9    AJH	Add vars for periodic licence check.
*   16/12/15 1.8    AJH	Add support for UDP multicast status.
*			Add CAT 253 SAC/SIC/ID config parameters.
*   14/12/15 1.7    AJH	Made m_interface const.
*   10/12/15 1.6    REW	Make Create() arg a const.
*   10/12/15 1.5    AJH	Add client-specified config file name.
*			Renamed network address and port parameters.
*   04/12/15 1.4    AJH	Regrouped radar parameters.
*			Added new conversion functions.
*   30/12/15 1.3    AJH	Add ConvertPcanProcRangeToMetres().
*   27/11/15 1.2    AJH	Change Sxv function names to Pcan.
*   26/11/15 1.1    AJH	Initial Version.
**********************************************************************/

#ifndef _SPX_P172_EMULATOR_H
#define _SPX_P172_EMULATOR_H

/* Standard headers. */

/*
 * Other headers required.
 */
#include "SPxLibUtils/SPxTimer.h"
#include "SPxLibUtils/SPxTime.h"

/* For SPxCriticalSection. */
#include "SPxLibUtils/SPxCriticalSection.h"

/* For base class. */
#include "SPxLibUtils/SPxObj.h"

/* For CAT 253 server. */
#include "SPxLibNet/SPxNetCat253Server.h"

/* For CAT 253 multicast. */
#include "SPxLibNet/SPxPacketSend.h"

/* The number of true transmit and relative blanking sectors supported. */
static const unsigned int P172_EMULATOR_MAX_TRUE_SECTOR = 1;
static const unsigned int P172_EMULATOR_MAX_REL_SECTOR = 3;

/* The number of processing sectors supported. */
static const unsigned int P172_EMULATOR_MAX_PROC_SECTOR = 16;

/* The maximum range enumerator. */
static const unsigned int P172_EMULATOR_MAX_RANGE = 9;

/*********************************************************************
*
*   Type definitions
*
**********************************************************************/

/* Radar configuration. */
struct SPxP172EmulatorTxBlankSector
{
    unsigned int StartAngle;
    unsigned int StopAngle;
    unsigned int Enabled;
};

struct SPxP172EmulatorProcSector
{
    unsigned int Priority;
    unsigned int Active;
    unsigned int North;
    unsigned int Ranged;
    unsigned int CCZHigh;
    unsigned int SectorType;
    unsigned int StartAngle;
    unsigned int StopAngle;
    unsigned int MinRange;
    unsigned int MaxRange;
};

struct SPxP172EmulatorRadarParameters
{
    SPxP172EmulatorTxBlankSector TrueSectorTx[P172_EMULATOR_MAX_TRUE_SECTOR];
    SPxP172EmulatorTxBlankSector RelSectorBlank[P172_EMULATOR_MAX_REL_SECTOR];
    SPxP172EmulatorProcSector ProcSector[P172_EMULATOR_MAX_PROC_SECTOR];

    /* Operating variables. */
    unsigned int SourceNode;
    unsigned int DestinationNode;
    unsigned int EquipmentTypeIdent;

    /* Status type 0. */
    unsigned int Run;
    unsigned int Mute;
    unsigned int SART;
    unsigned int TWS;
    unsigned int Range;
    unsigned int Frequency;
    unsigned int ReverseChirp;
    unsigned int HighPower;
    unsigned int RPM;
    unsigned int RPI;
    unsigned int VideoRange;
    unsigned int EngBuild;
    unsigned int EnhancedPulses;
    unsigned int TestByte;

    /* Status type 1 - alarms. */
    unsigned int RxSensitivity;
    unsigned int VSWR;
    unsigned int TxPower;
    unsigned int PLOLock;
    unsigned int SynthesizerLock;
    unsigned int OverTemperature;
    unsigned int TurnInfoLost;
    unsigned int HLNotDetected;
    unsigned int AzimuthStatus;
    unsigned int FPGA2Failed;
    unsigned int FibreLinkFailed;
    unsigned int EthernetFailed;
    
    /* Status type 2 - filters. */
    unsigned int Gain;
    unsigned int Sea;
    unsigned int Rain;
    unsigned int AutoSeaEnabled;
    unsigned int InterferenceRej;
    unsigned int DopplerFilterMode;

    /* Status type 3 - versions. */
    unsigned int ZmNumber;
    unsigned int FPGAMajorVersion;
    unsigned int FPGAMinorVersion;
    unsigned int SoftwareMajorVersion;
    unsigned int SoftwareMinorVersion;
    unsigned int CustomerID;

    /* Status type 5 - ontime. */
    unsigned int OnTimeHours;
    unsigned int OnTimeMinutes;
    unsigned int RunTimeHours;
    unsigned int RunTimeMinutes;

    /* Status type 8 - core temperature. */
    unsigned int FPGA1Temp;
    unsigned int FPGA2Temp;

    /* Status type 9/11 - platform. */
    unsigned int Gyro;
    unsigned int COG;
    unsigned int SOG;
    unsigned int HLS;

    /* Other items. */
    unsigned int Squint;
    unsigned int Video;
    unsigned int Racon;
};

/* Forward declaration of command handler type. */
struct SPxP172EmulatorCommandHandler;

/* Command strings. */
typedef enum { P172_MSG_PCANE = 0, P172_MSG_PCANT = 1, P172_MSG_PCANM = 2,
    P172_MSG_PCAND = 3, P172_MSG_INVALID = 4 } SPxP172EmulatorMessageType;

/* Message sending method. */
typedef enum { P172_MSG_TCP = 0, P172_MSG_UDP = 1 } SPxP172EmulatorSenderType;

/*
 * Define our class.
 */
class SPxP172Emulator : public SPxObj
{
public:
    /* Construction and destruction. */
    SPxP172Emulator(void);
    virtual ~SPxP172Emulator(void);
    SPxErrorCode Create(unsigned int radarNum = 0,
			unsigned int standAlone = FALSE,
			const char *configFile = NULL);

    /* Allows the dialog to set and retrieve radar parameters. */
    void SetParameters(const SPxP172EmulatorRadarParameters *params);
    const SPxP172EmulatorRadarParameters *GetParameters(void) const;

    static double ConvertPcanAngleToDegrees(unsigned int pcan);
    static double ConvertPcanSpeedToMetresPerSecond(unsigned int pcan);
    static double ConvertPcanEnumRangeToMetres(unsigned int pcan);
    static double ConvertPcanProcRangeToMetres(unsigned int pcan);
    static double ConvertPcanSquintToDegrees(unsigned int pcan);

    static unsigned int ConvertDegreesToPcanAngle(double degrees);
    static unsigned int ConvertMetresPerSecondToPcanSpeed(double mps);

private:
    /* Private variables. */
    struct impl;
    SPxAutoPtr<impl> m_p; /* Private structure. */

    mutable SPxCriticalSection m_mutex;	/* Mutex protection. */

    /* Our CAT 253 network connection. */
    SPxAutoPtr<SPxNetCat253Server> m_network;

    /* Our CAT 253 multicast connection for sending status heartbeats. */
    SPxAutoPtr<SPxPacketSender> m_networkStatus;
    
    /* Level of reporting. */
    unsigned int m_verbosity;

    /* The radar parameters. */
    SPxP172EmulatorRadarParameters m_radarParams;

    /* Are we running standaline (i.e. no connection to simulator)? */
    unsigned int m_standAlone;

    /* Which instance are we (radar number)? */
    unsigned int m_radarNum;

    /* The prefix for configuration file entries, including radar ID. */
    char m_configPrefix[128];

    /* The network port on which to listen. */
    int m_port;

    /* The network port on which to send UDP status. */
    int m_portUDP;

    /* The message buffer and current length for outgoing messages. */
    unsigned char m_mesgBuf[4096];
    unsigned int m_mesgBufLen;

    /* The source SAC, SIC and local ID for CAT-253 header. */
    unsigned int m_srcSAC;
    unsigned int m_srcSIC;
    unsigned int m_srcLocalID;

    /* Timer for heartbeats. */
    SPxTimer m_timer;

    /* Last heartbeat time. */
    UINT32 m_lastHeartbeatTime;

    /* Last time heartbeat received from display. */
    UINT32 m_lastDisplayHeartbeatTime;

    /* Last licence check time. */
    UINT32 m_lastLicenceCheckTime;

    /* Are we correctly licensed? */
    unsigned int m_licensed;

    /* The start time (since epoch). */
    SPxTime_t m_startTime;

    /* The last command received.  We use this to ensure that any PCAND
     * messages have their data associated with the correct data items. */
    SPxP172EmulatorCommandHandler *m_lastCommand;

    /* The number of bytes outstanding for an expected sequence of PCAND
     * messages.
     */
    int m_lastCommandBytesLeft;

    /* The values for antenna rotation rate for normal and TWS mode. */
    UINT32 m_antennaSpeedTWS;
    UINT32 m_antennaSpeedNormal;

    /* The values for background noise for normal and auto sea clutter modes. */
    double m_backgroundNoiseNormal;
    double m_backgroundNoiseAutoSeaClutter;

    /* Private functions. */
    void setupConnections(void);
    void handleHeartbeatTimer(void);
    void resetEmulator(void);
    void handleMsg(const char *data, unsigned int numDataBytes);
    void buildCmd(const char *nmeaIdent, int numParams, ...);
    void sendCmd(void);
    void sendStatusCmd(int statusCmd, int numParams, ...);
    void sendAcknowledgeCmd(int msg, SPxP172EmulatorMessageType msgType);
    void sendStatusHeartbeat(void);
    void checkLicence(void);

    void setSimulatorAll(void);
    SPxErrorCode setSimulatorBlanking(void);
    SPxErrorCode setSimulatorPeriod(void);
    SPxErrorCode setSimulatorRange(void);
    SPxErrorCode setSimulatorMute(void);
    SPxErrorCode setSimulatorPlatformData(void);
    SPxErrorCode setSimulatorBackgroundNoise(void);

    void
#if defined(__GNUC__) || defined(__clang__)
	__attribute__((format(printf, 3, 4)))
#endif
    log(unsigned int verbosity, const char *format, ...);
static void networkDataFnWrapper(SPxNetCat253 *obj,
                      void *arg,
                      const UINT8 *data,
                      unsigned int numDataBytes);
static void handleHeartbeatTimerWrapper(void *userArg);

public:
    /* The handler routines need to be defined as public to allow the
     * initializer to work, though they would ideally be private.
     */
    void handleCmdHeartbeat(const char *params[], unsigned int numParams);
    void handleCmdGotoMode(const char *params[], unsigned int numParams);
    void handleCmdSetFreq(const char *params[], unsigned int numParams);
    void handleCmdSetRange(const char *params[], unsigned int numParams);
    void handleCmdSetOutputPower(const char *params[], unsigned int numParams);
    void handleCmdReqStatus(const char *params[], unsigned int numParams);
    void handleCmdReqTrueSector(const char *params[], unsigned int numParams);
    void handleCmdReqRelSector(const char *params[], unsigned int numParams);
    void handleCmdSetTxMute(const char *params[], unsigned int numParams);
    void handleCmdSetVideoMode(const char *params[], unsigned int numParams);
    void handleCmdGyroData(const char *params[], unsigned int numParams);
    void handleCmdCOGData(const char *params[], unsigned int numParams);
    void handleCmdSOGData(const char *params[], unsigned int numParams);
    void handleCmdSetTrueSector(const char *params[], unsigned int numParams);
    void handleCmdSetRelSector(const char *params[], unsigned int numParams);
    void handleCmdEnableTrueSector(const char *params[], unsigned int numParams);
    void handleCmdEnableRelSector(const char *params[], unsigned int numParams);
    void handleCmdSetHeadingSkew(const char *params[], unsigned int numParams);
    void handleCmdSetAntennaSquint(const char *params[], unsigned int numParams);
    void handleCmdSetRadarFilter(const char *params[], unsigned int numParams);
    void handleCmdDefineProcSector(const char *params[], unsigned int numParams);
    void handleCmdReqProcSector(const char *params[], unsigned int numParams);
    void handleCmdSetAutoSeaClutter(const char *params[], unsigned int numParams);
    void handleCmdEmergency(const char *params[], unsigned int numParams);
    void handleCmdSetAntennaSpeed(const char *params[], unsigned int numParams);
    void handleCmdSetNodeAddr(const char *params[], unsigned int numParams);
}; /* SPxP172Emulator */

#endif /* _SPX_P172_EMULATOR_H */

/*********************************************************************
*
*   End of File
*
**********************************************************************/

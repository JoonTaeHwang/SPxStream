/*********************************************************************
 *
 * (c) Copyright 2018, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   Class for controlling Raytheon Mk2 radar via HPx-346.
 *
 *
 * Revision Control:
 *  06/09/18 v1.11    AJH	Allow transceiver to be released.
 *
 * Previous Changes:
 *  30/08/18 1.10   AJH	Updates to match re-write of main code.
 *  23/08/18 1.9    AJH	Add request timeout variable.
 *  08/08/18 1.8    AJH	Get TPG in Create().
 *			Separate timers for status and control.
 *  25/07/18 1.7    AJH	Add IsTransceiverMaster().
 *  19/07/18 1.6    AJH	Add interference rejection.
 *  13/07/18 1.5    REW	Fix case of SPxPim.h inclusion.
 *  12/07/18 1.4    AJH	Add HPx-346/PIM access functions.
 *  10/07/18 1.3    AJH	Add GetSectorCtrl().
 *  06/07/18 1.2    AGC	Make destructor virtual.
 *  30/06/18 1.1    AJH	Initial version.
 **********************************************************************/

/* SPx headers. */
#include "SPxLibData/SPxHPx100Source.h"
#include "SPxLibData/SPxPim.h"
#include "SPxLibData/SPxTestGen.h"
#include "SPxLibUtils/SPxError.h"
#include "SPxLibUtils/SPxTimer.h"

/* The number of blanking sectors supported. */
#define SPX_P383_NUM_SECTOR		8

/* The maximum message length. */
#define SPX_P383_MAX_MESSAGE_LEN	8

/* The types of display to transceiver message supported. */
enum SPxP383MsgType_t
{
    SPX_P383_MSG_REQ_TRANS,
    SPX_P383_MSG_SECTOR_ADD,
    SPX_P383_MSG_SECTOR_CONTROL,
    SPX_P383_MSG_HOUR_METER,
    SPX_P383_MSG_SEND_TRANSCEIVER_STATUS,
    SPX_P383_MSG_TRANSCEIVER_CONTROL,
    SPX_P383_MSG_NONE
};

enum SPxP383RespType_t
{
    SPX_P383_RESP_TRANSCEIVER_STATUS,
    SPX_P383_RESP_TIME_MESSAGE,
    SPX_P383_RESP_SECTOR_ADD,
    SPX_P383_RESP_SECTOR_CONTROL,
    SPX_P383_RESP_NONE
};

/* The sector blanking operations. */
enum SPxP383SectorCtrl_t
{
    SPX_P383_SECT_CTRL_OFF = 0,
    SPX_P383_SECT_CTRL_ON = 1,
    SPX_P383_SECT_CTRL_CLEAR = 2
};

/* Hour meter requests. */
enum SPxP383HourMeter_t
{
    SPX_P383_HOUR_METER_TRANSMIT = 0,
    SPX_P383_HOUR_METER_POWER = 1
};

/* Data for a blanking sector. */
struct SPxP383Sector_t
{
    unsigned int aziStart;
    unsigned int aziEnd;
};

/* A request for message to transceiver. */
struct SPxP383Request_t
{
    SPxP383MsgType_t type;
    unsigned int arg[3];
};

/* Contents of tranceiver control message. */
struct SPxP383TransControl_t
{
    unsigned int commMode;
    unsigned int pulseLength;
    unsigned int txOnOff;
    unsigned int tune;
    unsigned int perfMonOnOff;
    unsigned int perfMonCalStore;
    unsigned int widebandMode;
    unsigned int loopTest;
    unsigned int perfMonDirection;
    unsigned int perfMonIncrement;
    unsigned int perfMonTransmitCal;
    unsigned int perfMonReceiveCal;
    unsigned int stcOnOff;
    unsigned int afcAutotuneOnOff;
};

/* Contents of transceiver status message. */
struct SPxP383TransStatus_t
{
    unsigned int blanked;
    unsigned int tune;
    unsigned int displayID;
    unsigned int magLevel;
    unsigned int standbyTimer;
    unsigned int txOnOff;
    unsigned int uplinkStatus;
    unsigned int transOwned;
    unsigned int pulseLength;
    unsigned int temp;
    unsigned int antennaSafety;

    UINT32 rawData;
};

class SPxP383RadarController : public SPxObj
{
public:
    /* Constructor and destructor. */
    SPxP383RadarController(void);
    virtual ~SPxP383RadarController(void);

    /* Public methods. */
    SPxErrorCode Create(SPxHPx100Source *hpx346, SPxPIM *pim, SPxTestGenerator *tpg=NULL);
    SPxErrorCode RequestTransceiver(unsigned int request);
    unsigned int IsTransceiverMaster(void) { return m_isTransceiverMaster; }
    SPxErrorCode SetSector(unsigned int sectNum,
	unsigned int aziStart, unsigned int aziEnd);
    SPxErrorCode GetSector(unsigned int sectNum,
	unsigned int *aziStart, unsigned int *aziEnd);
    SPxErrorCode SectorCtrl(SPxP383SectorCtrl_t ctrl);
    unsigned int GetSectorCtrl(void) { return m_sectorCtrlCmd; }
    SPxErrorCode HourMeterRequest(SPxP383HourMeter_t meter);
    double GetHourMeterPower(void) { return m_hourMeterPower; }
    double GetHourMeterTransmit(void) { return m_hourMeterTransmit; }
    SPxErrorCode RunLoopTest(void);
    SPxErrorCode SetLogFile(const char *logfile);
    const char *GetLogFile(void) { return(m_logFile ? m_logFile : ""); }

    /* Member access functions. */
    SPxErrorCode SetTransmitEnable(unsigned int enable);
    unsigned int GetTransmitEnable(void) { return m_transmitEnabled; }
    SPxErrorCode SetDisplayID(unsigned int dispID) { m_displayID = dispID; return SPX_NO_ERROR; }
    unsigned int GetDisplayID(void) { return m_displayID; }
    SPxErrorCode SetCommMode(unsigned int commMode) { m_transControl.commMode = commMode; return SPX_NO_ERROR; }
    unsigned int GetCommMode(void) { return m_transControl.commMode; }
    SPxErrorCode SetPulseLength(unsigned int pulseLength) { m_transControl.pulseLength = pulseLength; return SPX_NO_ERROR; }
    unsigned int GetPulseLength(void) { return m_transControl.pulseLength; }
    SPxErrorCode SetTxOnOff(unsigned int txOnOff) { m_transControl.txOnOff = txOnOff; return SPX_NO_ERROR; }
    unsigned int GetTxOnOff(void) { return m_transControl.txOnOff; }
    SPxErrorCode SetTune(unsigned int tune) { m_transControl.tune = tune; return SPX_NO_ERROR; }
    unsigned int GetTune(void) { return m_transControl.tune; }
    SPxErrorCode SetPerfMonOnOff(unsigned int perfMonOnOff) { m_transControl.perfMonOnOff = perfMonOnOff; return SPX_NO_ERROR; }
    unsigned int GetPerfMonOnOff(void) { return m_transControl.perfMonOnOff; }
    SPxErrorCode SetPerfMonCalStore(unsigned int perfMonCalStore) { m_transControl.perfMonCalStore = perfMonCalStore; return SPX_NO_ERROR; }
    unsigned int GetPerfMonCalStore(void) { return m_transControl.perfMonCalStore; }
    SPxErrorCode SetWidebandMode(unsigned int widebandMode) { m_transControl.widebandMode = widebandMode; return SPX_NO_ERROR; }
    unsigned int GetWidebandMode(void) { return m_transControl.widebandMode; }
    SPxErrorCode SetLoopTest(unsigned int loopTest) { m_transControl.loopTest = loopTest; return SPX_NO_ERROR; }
    unsigned int GetLoopTest(void) { return m_transControl.loopTest; }
    SPxErrorCode SetPerfMonDirection(unsigned int perfMonDirection) { m_transControl.perfMonDirection = perfMonDirection; return SPX_NO_ERROR; }
    unsigned int GetPerfMonDirection(void) { return m_transControl.perfMonDirection; }
    SPxErrorCode SetPerfMonIncrement(unsigned int perfMonIncrement) { m_transControl.perfMonIncrement = perfMonIncrement; return SPX_NO_ERROR; }
    unsigned int GetPerfMonIncrement(void) { return m_transControl.perfMonIncrement; }
    SPxErrorCode SetPerfMonTransmitCal(unsigned int perfMonTransmitCal) { m_transControl.perfMonTransmitCal = perfMonTransmitCal; return SPX_NO_ERROR; }
    unsigned int GetPerfMonTransmitCal(void) { return m_transControl.perfMonTransmitCal; }
    SPxErrorCode SetPerfMonReceiveCal(unsigned int perfMonReceiveCal) { m_transControl.perfMonReceiveCal = perfMonReceiveCal; return SPX_NO_ERROR; }
    unsigned int GetPerfMonReceiveCal(void) { return m_transControl.perfMonReceiveCal; }
    SPxErrorCode SetStcOnOff(unsigned int stcOnOff) { m_transControl.stcOnOff = stcOnOff; return SPX_NO_ERROR; }
    unsigned int GetStcOnOff(void) { return m_transControl.stcOnOff; }
    SPxErrorCode SetAfcAutotuneOnOff(unsigned int afcAutotuneOnOff) { m_transControl.afcAutotuneOnOff = afcAutotuneOnOff; return SPX_NO_ERROR; }
    unsigned int GetAfcAutotuneOnOff(void) { return m_transControl.afcAutotuneOnOff; }

    /* Status returns from transceiver. */
    unsigned int GetBlankedStatus(void) { return m_transStatus.blanked; }
    unsigned int GetTuneStatus(void) { return m_transStatus.tune; }
    unsigned int GetDisplayIDStatus(void) { return m_transStatus.displayID; }
    unsigned int GetMagLevelStatus(void) { return m_transStatus.magLevel; }
    unsigned int GetStandbyTimerStatus(void) { return m_transStatus.standbyTimer; }
    unsigned int GetTxOnOffStatus(void) { return m_transStatus.txOnOff; }
    unsigned int GetUplinkStatus(void) { return m_transStatus.uplinkStatus; }
    unsigned int GetTransOwnedStatus(void) { return m_transStatus.transOwned; }
    unsigned int GetPulseLengthStatus(void) { return m_transStatus.pulseLength; }
    unsigned int GetTempStatus(void) { return m_transStatus.temp; }
    unsigned int GetAntennaSafetyStatus(void) { return m_transStatus.antennaSafety; }

    /* Functions for operating on HPx-346/PIM. */
    unsigned int GetNumSamples(void);
    unsigned int GetNumAzimuths(void);
    SPxErrorCode SetAzimuthOffset(double offsetDegs);
    double GetAzimuthOffset(void);
    double GetAntennaRPM(void);
    SPxErrorCode SetInterferenceRejectionEnabled(unsigned int enabled);
    unsigned int GetInterferenceRejectionEnabled(void);
    SPxErrorCode SetTestPatternEnabled(unsigned int enabled);
    unsigned int GetTestPatternEnabled(void);

    /* Generic parameter assignment. */
    virtual int SetParameter(char *parameterName, char *parameterValue);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);

private:
    /* The HPx-346 and TPG objects. */
    SPxHPx100Source *m_hpx346;
    SPxTestGenerator *m_tpg;

    /* The main PIM. */
    SPxPIM *m_pim;

    /* The data bytes for the message currently being sent. */
    unsigned char m_msgData[SPX_P383_MAX_MESSAGE_LEN];

    /* Protect access to the request queue. */
    SPxCriticalSection m_mutex;

    /* The request timer. */
    SPxTimer m_timerRequest;

    /* The display ID. */
    unsigned int m_displayID;

    /* Transceiver requested? */
    unsigned int m_transceiverRequested;

    /* Are we the transceiver master? */
    unsigned int m_isTransceiverMaster;

    /* Is transmit enabled? */
    unsigned int m_transmitEnabled;

    /* The returned hour meters. */
    double m_hourMeterPower;
    double m_hourMeterTransmit;

    /* Blanking sector definitions. */
    SPxP383Sector_t m_blankSector[SPX_P383_NUM_SECTOR];
    unsigned int m_sectorCtrlCmd;

    /* Current transceiver control. */
    SPxP383TransControl_t m_transControl;

    /* Current transceiver status. */
    SPxP383TransStatus_t m_transStatus;

    /* How long to wait after sending extended request. */
    unsigned int m_requestTimeoutMs;

    /* The time of the last request that expects a response. */
    SPxTime_t m_lastRequestTime;

    /* The debug log file name. */
    char *m_logFile;

    /* Internal data. */
    struct impl;
    SPxAutoPtr<impl> m_p;

    /* Private functions. */
    SPxErrorCode addRequest(SPxP383MsgType_t type, unsigned int arg0=0, unsigned int arg1=0, unsigned int arg2=0);
    void doRequestTransceiver(SPxP383Request_t request);
    void doSetSector(SPxP383Request_t request);
    void doSectorControl(SPxP383Request_t request);
    void doHourMeterRequest(SPxP383Request_t request);
    void doTransceiverControl(SPxP383Request_t request);
    void doResumeTransceiverStatus(SPxP383Request_t request);
    void clearMessage(void);
    void handleRequestTimer(void);
    void handleHourMeterTimer(void);
    void sendMessage(SPxP383MsgType_t msgType, unsigned int msgLen);

    void
#if defined(__GNUC__) || defined(__clang__)
	__attribute__((format(printf, 2, 3)))
#endif
    log(const char *format, ...);
};

/*********************************************************************
 *
 * End of file
 *
 **********************************************************************/

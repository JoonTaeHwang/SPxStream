/*********************************************************************
*
* (c) Copyright 2012 - 2021, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for the P172 radar control dialog.
*
* Revision Control:
*   04/11/21 v1.18   SP 	Support Mk5 X-Band.
*
* Previous Changes:
*   28/09/20 1.17   SP 	Improve dual-redundancy support when embedded.
*   08/06/20 1.16   SP 	Support controller dual-redundancy.
*   22/11/18 1.15   AGC	Do not call OnTimer from OnInitDialog.
*   13/09/17 1.14   SP 	Remove unused icons.
*   11/05/16 1.13   SP 	Support multi-language.
*   27/11/15 1.12   SP 	Add Connect() for backward compatibility.
*                      	Support navigation data.
*   26/11/15 1.11   SP 	Add command dialog.
*   22/10/15 1.10   SP 	Add LogRadarStatus().
*   23/09/15 1.9    SP 	Rename some functions.
*   26/01/15 1.8    AGC	Remove unused HICON and antenna speed label.
*   01/09/14 1.7    AGC	VS2015 format string changes.
*   17/02/14 1.6    SP 	Add support for SxV series radars.
*   17/12/13 1.5    SP 	Make WriteToLogFile() public.
*   27/09/13 1.4    AGC	Add forward declaration for CTabCtrl.
*   10/04/13 1.3    SP 	Add m_lastConfigFilename.
*   26/10/12 1.2    SP 	Further development.
*   18/10/12 1.1    SP 	Initial version.
*
**********************************************************************/

#pragma once

/* Include the SPx resource header */
#include "SPxResource.h"

/* Include the classes we need. */
#include "SPxLibUtils/SPxTime.h"
#include "SPxLibWin/SPxP172RcBaseDlgWin.h"
#include "SPxLibWin/SPxP172RcStatusDlgWin.h"
#include "SPxLibWin/SPxP172RcR7TxRxDlgWin.h"
#include "SPxLibWin/SPxP172RcR8R9TxRxDlgWin.h"
#include "SPxLibWin/SPxP172RcTuneDlgWin.h"
#include "SPxLibWin/SPxP172RcHelpDlgWin.h"
#include "SPxLibWin/SPxP172RcCommandDlgWin.h"

/* Forward declarations. */
class CTabCtrl;
class SPxNavData;
class SPxExtHeartbeatSender;
class SPxOutputManager;

/*
 * SPxP172RcDlgWin dialog.
 */
class SPxP172RcDlgWin : public SPxP172RcBaseDlgWin
{
    DECLARE_DYNAMIC(SPxP172RcDlgWin)

public:

    /*
     * Public functions.
     */

    /* Constructor and destructor. */
    SPxP172RcDlgWin(BOOL isStandAlone=TRUE, SPxOutputManager *outputMgr=NULL);
    virtual ~SPxP172RcDlgWin(void);
    virtual BOOL Create(CWnd* pParent);

    /* Interface to the radar. */
    virtual SPxP172RcInterfaceWin *GetInterface(void) { return m_interface; }

    /* Output manager for controller dual-redundancy. */
    virtual SPxOutputManager *GetOutputManager(void) { return m_outputMgr; }
    virtual int IsInStandby(void) { return m_isInStandby; }

    /* Configuration. */
    virtual SPxErrorCode OpenConnection(void);
    virtual SPxErrorCode CloseConnection(void);
    virtual SPxErrorCode SetStateFromConfig(void);
    virtual SPxErrorCode SetConfigFromState(void);

    /* Top-level status. */
    virtual BOOL IsOk(void) { return m_isOk; } 
    virtual SPxErrorCode GetSystemName(char *buf, unsigned int bufLen);

    /* Log file. */
    void WriteToLogFile(const char *text);

    /* For backward compatibility. */
    virtual SPxErrorCode Connect(void) { return OpenConnection(); }

    /* Dialog Data */
    enum { IDD = IDD_SPX_P172_RC_DIALOG };

protected:

    /*
     * Protected functions.
     */

    DECLARE_MESSAGE_MAP()
    virtual void DoDataExchange(CDataExchange* pDX); 
    virtual BOOL OnInitDialog();
    virtual afx_msg void OnTimer(UINT_PTR nIDEvent);
    virtual afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpdis);
    virtual afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    virtual afx_msg void OnDestroy(void);
    virtual void OnOK(void){ /* Don't call the CDialog handler */ };
    virtual void OnCancel(void){ /* Don't call the CDialog handler */ };
    virtual int SetParameter(char *name, char *value);
    virtual int GetParameter(char *name, char *valueBuf, int bufLen); 

private:

    /* 
     * Private types. 
     */

    /* Status panel. */
    typedef struct
    {
        /* Unit associated with this status panel. */
        SPxP172RcStatusDlgWin::UnitType_t unitType;

        /* Is this unit available? */
        BOOL isAvailable;

        /* Overall status. */
        BOOL isOk;

        /* The detailed status pop-up dialog. */
        SPxP172RcStatusDlgWin *detailsDlg;

        /* GUI items. */
        CStatic *groupbox; /* Panel groupbox. */
        CWnd *detailsBn;   /* Button used to open details pop-up. */
        CStatic *text;     /* Status text. */
        CStatic *led;      /* Status LED. */

        /* Extra transceiver GUI items. */
        CStatic *txText;  /* Tx mode text. */
        CStatic *txLed;   /* Tx mode LED. */

    } StatusPanel_t;

    /* 
     * Private functions. 
     */

    void UpdateTimer(UINT_PTR nIDEvent);
    void MonitorDualRedundancy(void);
    void UpdateHeartbeatIndicators(void);
    void UpdateInfoPanel(void);
    void UpdateMiscControls(void);
    void UpdateAntennaControlPanel(void);
    void SyncAntennaControlPanel(void);
    void UpdateTxRxControlPanel(void);
    void SyncTxRxControlPanel(void);
    void UpdateRDUStatusPanel(void);
    void UpdateMiscStatusPanel(void);
    void UpdateTxRxActiveXStatusPanel(void);
    void UpdateTxRxReserveSStatusPanel(void);
    void UpdateTxRxStatusPanel(StatusPanel_t *panel, 
                               BOOL isAvailable,
                               BOOL isTransmitting,
                               const char *logicalName,
                               const char *physicalName);
    void UpdateStatusPanel(StatusPanel_t *panel, 
                           BOOL isAvailable);
    void UpdateBackupRestorePanel(void);
    void UpdateAlarmWindow(void);
    void UpdateEventWindow(void);
    void MonitorRadarState(void);
    void LogRadarStatus(void);
    void UpdateOverallStatus(void);
    void ShowTxRxTab(int tabIndex);
    void ReportEvent(const char *text);
    void ReportAlarm(const char *text);

    /* GUI callback functions */
    afx_msg void OnBnClickedHelpButton(void);
    afx_msg void OnBnClickedOfflineCheck(void);
    afx_msg void OnBnClickedSaveConfigButton(void);
    afx_msg void OnBnClickedTrafficButton(void);
    afx_msg void OnBnClickedCommandButton(void);
    afx_msg void OnCbnSelchangeAntennaSpeedCombo(void);
    afx_msg void OnCbnSelchangeAntennaPolCombo(void);
    afx_msg void OnCbnSelchangeBrUnitCombo(void);
    afx_msg void OnBnClickedBrBackupButton(void);
    afx_msg void OnBnClickedBrRestoreButton(void);
    afx_msg void OnBnClickedEmergencyStopButton(void);
    afx_msg void OnBnClickedRDUStatusButton(void);
    afx_msg void OnBnClickedMiscStatusButton(void);
    afx_msg void OnBnClickedTxRxActiveXStatusButton(void);
    afx_msg void OnBnClickedTxRxReserveSStatusButton(void);
    afx_msg void OnBnClickedAlarmsClearButton(void);
    afx_msg void OnBnClickedEventsClearButton(void);
    afx_msg void OnTcnSelchangeTxRxTabCtrl(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnBnClickedTxRxActiveTuneButton(void);

    /* 
     * Private static functions. 
     */

    /* Interface message, event and alarm text handlers. */
    static int txMesgTextHandler(void *invArg, void *userArg, void *textArg);
    static int rxMesgTextHandler(void *invArg, void *userArg, void *textArg);
    static int eventTextHandler(void *invArg, void *userArg, void *textArg);
    static int alarmTextHandler(void *invArg, void *userArg, void *textArg);
    static void spxErrorHandler(SPxErrorType errType, SPxErrorCode errCode,
                                int arg1, int arg2, 
                                const char *arg3, const char *arg4);

    /* 
     * Private variables. 
     */

    /* Controller start time. */
    SPxTime_t m_startTime;

    /* Stand-alone application? */
    BOOL m_isStandAlone;

    /* Connection. */
    BOOL m_isConnectRequested;

    /* General GUI objects. */
    HICON m_smallGreenLedIcon;
    HICON m_smallGreyLedIcon;
    CWnd *m_alarmWin;
    CWnd *m_eventWin;
    CStatic *m_identityText;
    CStatic *m_systemTypeText;
    CStatic *m_radarModeText;
    CStatic *m_displayModeText;
    CStatic *m_userModeText;
    CStatic *m_timeText;
    CStatic *m_heartbeatTxLed;
    CStatic *m_heartbeatRxLed;
    CTabCtrl *m_txRxTabCtrl;
    CButton *m_helpBn;
    CButton *m_commandBn;
    CButton *m_trafficBn;
    CButton *m_offlineBn;
    CButton *m_emergencyStopBn;
    CStatic *m_emergencyStopBg;
    CButton *m_txRxActiveTuneBn;
    CComboBox *m_brUnitCombo;
    CButton *m_backupBn;
    CButton *m_restoreBn;
    CButton *m_saveBn;

    /* The full system type name. */
    CStringA m_systemName;

    /* Name of the last config file. */
    CStringA m_lastConfigFilename;

    /* The on-line help file. */
    CStringA m_helpFilename;

    /* Control and status information. Note that these
     * structures are linked together to form a linked-list.
     */
    Control_t m_antennaPol;
    Control_t m_antennaSpeed;

    /* Transceiver control dialogs. */
    SPxP172RcR7TxRxDlgWin *m_r7TxRxDlg;             /* SBS-700 X-band. */
    SPxP172RcR8R9TxRxDlgWin *m_r8r9XBandTxRxDlg;    /* SBS-800/900 & PCAN X-band. */
    SPxP172RcR8R9TxRxDlgWin *m_r8r9SBandTxRxDlg;    /* SBS-800/900 & PCAN S-band. */

    /* Status panels for each unit. */
    StatusPanel_t m_rduStatus;          /* RDU. */
    StatusPanel_t m_miscStatus;         /* Miscellaneous status. */
    StatusPanel_t m_txRxActiveXStatus;  /* Active or X-band transceiver. */
    StatusPanel_t m_txRxReserveSStatus; /* Reserve or S-band transceiver. */

    /* Backup and restore variables. */
    BOOL m_isBackupRestoreAvailable;

    /* Events and alarms. */
    CStringA m_eventBuf;          /* Event text buffer. */
    BOOL m_isEventMsgPending;     /* Event message pending flag. */
    CStringA m_alarmBuf;          /* Alarm text buffer. */
    BOOL m_isAlarmMsgPending;     /* Alarm message pending flag. */

    /* Log file. */
    CStringA m_logFilePrefix;    /* Filename prefix. */
    FILE *m_logFile;             /* File handle. */
    BOOL m_isLogRadarStatusPending; /* Need to log radar status? */
    BOOL m_isLogRadarStatusDone;    /* Has radar status been logged? */
    UINT32 m_logRadarStatusHoldoffStartMsecs;

    /* Child and pop-up dialogs. */
    SPxP172RcTrafficDlgWin *m_trafficDlg; /* Shows message traffic. */
    SPxP172RcTuneDlgWin *m_txRxActiveTuneDlg; /* TxRx tune indicator. */
    SPxP172RcHelpDlgWin *m_helpDlg;       /* Help information. */
    SPxP172RcCommandDlgWin *m_commandDlg; /* Send command dialog. */

    /* Heartbeat indicators. */
    BOOL m_isHeartbeatTx;        /* Heartbeat sent. */
    UINT32 m_heartbeatTxOnTime;  /* Time TX LED switched on. */
    BOOL m_isHeartbeatRx;        /* Heartbeat received. */
    UINT32 m_heartbeatRxOnTime;  /* Time RX LED switched on. */

    /* Overall status. */
    BOOL m_isAntennaCtrlOK;      /* Antenna control status. */
    BOOL m_isXBandTxRxCtrlOK;    /* X-band transceiver control status. */
    BOOL m_isSBandTxRxCtrlOK;    /* S-band transceiver control status. */
    BOOL m_isOk;                 /* Top-level status. */

    /* Transceiver tabs. */
    int m_xBandTabIndex;         /* Index of X-band transceiver tab. */
    int m_sBandTabIndex;         /* Index of S-band transceiver tab. */

    /* Nav data (created internally). */
    SPxNavData *m_internalNavData; /* Nav data object. */

    /* Controller dual-redundancy. */
    SPxExtHeartbeatSender *m_internalExtHBSender;   /* Extended H/B sender created internally. */
    SPxOutputManager *m_internalOutputMgr;          /* Output manager for created internally. */
    SPxOutputManager *m_outputMgr;                  /* The output manager to use. */
    BOOL m_isInStandby;                             /* Is controller in standby? */
    BOOL m_wasInStandy;                             /* Was controller in standby? */
    BOOL m_isSynced;                                /* Is controller synchronised to radar? */

    /* Labels. */
    CStringA m_identityLabel;
    CStringA m_systemTypeLabel;
    CStringA m_radarModeLabel;
    CStringA m_displayModeLabel;
    CStringA m_userModeLabel;
    CStringA m_dualModeLabel;
    CStringA m_emergencyStopLabel;
    CStringA m_restartLabel;
    CStringA m_activeLabel;
    CStringA m_reserveLabel;
    CStringA m_transceiverLabel;
    CStringA m_xBandLabel;
    CStringA m_sBandLabel;

}; /* class SPxP172RcDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/

/*********************************************************************
 *
 * (c) Copyright 2019, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   Header for the P395 radar control dialog.
 *
 * Revision Control:
 *   12/09/19 v1.11  SP 	Display track notifications.
 *
 * Previous Changes:
 *   01/07/19 1.10  SP 	Use edit control for track ID.
 *   28/06/19 1.9   SP 	Support multiple burn-through positions.
 *   27/06/19 1.8   SP 	Add set button to positions, sectors and zones.
 *   13/06/19 1.7   SP 	Various changes and fixes from testing.
 *   11/06/19 1.6   SP 	Default to not a stand-alone controller.
 *   11/06/19 1.5   SP 	Further development.
 *   07/06/19 1.4   SP 	Further development.
 *   05/06/19 1.3   SP 	Further development.
 *   24/05/19 1.2   AGC	Fix minor warnings.
 *   23/05/19 1.1   SP 	Initial version.
 *
 **********************************************************************/

#pragma once

/* Include the SPx resource header */
#include "SPxResource.h"

/* Include the classes we need. */
#include "SPxLibUtils/SPxObj.h"
#include "SPxLibUtils/SPxAutoPtr.h"
#include "SPxLibUtils/SPxTime.h"
#include "SPxLibNet/SPxP395Ctrl.h"
#include "SPxLibWin/SPxWinSpinCtrl.h"
#include "SPxLibWin/SPxWinEditCtrl.h"
#include "SPxLibWin/SPxWinListCtrl.h"
#include "SPxLibWin/SPxWinMessageCtrl.h"

/*
 * SPxP395CtrlDlgWin dialog.
 */
class SPxP395CtrlDlgWin : public CDialog, public SPxObj
{
    DECLARE_DYNAMIC(SPxP395CtrlDlgWin)

public:

    /*
     * Public functions.
     */

    /* Constructor and destructor. */
    SPxP395CtrlDlgWin(CWnd *parent, BOOL isStandAlone=FALSE);
    virtual ~SPxP395CtrlDlgWin(void);
    virtual BOOL Create(void);

    /* Configuration. */
    virtual SPxErrorCode SetRangeLimits(REAL32 minKm, REAL32 maxKm);
    virtual REAL32 GetMinRange(void) { return m_minRangeKm; }
    virtual REAL32 GetMaxRange(void) { return m_maxRangeKm; }
    virtual SPxErrorCode SetStateFromConfig(void);
    virtual SPxErrorCode SetConfigFromState(void);

    /* Status. */
    virtual SPxStatus_t GetOverallStatus(UINT32 expiryMsecs=0);

    /* Dialog Data */
    enum { IDD = IDD_SPX_P395_CTRL_DIALOG };

protected:

    /*
     * Protected functions.
     */

    DECLARE_MESSAGE_MAP()
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog(void);
    virtual afx_msg void OnTimer(UINT_PTR nIDEvent);
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

    /* Combined control/status pair. */
    struct ControlStatus_t
    {
        CStringA name;              /* Name of the control. */
        CSPxWinSpinCtrl spinCtrl;   /* Spin control. */
        CButton checkCtrl;          /* Checkbox control. */
        CComboBox comboCtrl;        /* Combo control. */
        CWnd *ctrl;                 /* Generic control pointer. */
        SPxTime_t ctrlUpdateTime;   /* Last control update time. */
        CStatic status;             /* Status text field. */
        SPxErrorCode statusErr;     /* Status error code. */
        SPxErrorCode pendingErr;    /* Pending status error code? */
        CStringA units;             /* Units text. */
    };

    /* Position controls and status. */
    struct PositionCS_t
    {
        CComboBox idCombo;
        ControlStatus_t enableCS;
        ControlStatus_t azimuthCS;
        ControlStatus_t elevationCS;
        ControlStatus_t rangeCS;
    };

    /* Zone controls and status. */
    struct ZoneCS_t
    {
        CComboBox idCombo;
        ControlStatus_t enableCS;
        ControlStatus_t startAziCS;
        ControlStatus_t stopAziCS;
        ControlStatus_t startRangeCS;
        ControlStatus_t stopRangeCS;
    };

    /* Sector controls and status. */
    struct SectorCS_t
    {
        CComboBox idCombo;
        ControlStatus_t enableCS;
        ControlStatus_t startAziCS;
        ControlStatus_t stopAziCS;
    };

    /*
     * Private variables.
     */

    struct impl;
    SPxAutoPtr<impl> m_p;

    /* General. */
    CWnd *m_parent;                     /* Parent window. */
    BOOL m_isStandAlone;                /* Stand-alone application? */
    SPxP395Ctrl *m_interface;           /* Interface to the radar. */
    int m_nextRow;                      /* Used when writing to list. */
    BOOL m_wasMonitor;                  /* Last monitor state. */
    BOOL m_wasController;               /* Last control state. */
    SPxStatus_t m_overallStatus;        /* Overall radar status. */
    REAL32 m_minRangeKm;                /* Minimum range supported. */
    REAL32 m_maxRangeKm;                /* Maximum range supported. */
    SPxTime_t m_scheduledCtrlSyncTime;  /* Time of scheduled control sync. */

    /* Colours to use for status indicators. */
    COLORREF m_blackBGR;
    COLORREF m_statusOkBGR;
    COLORREF m_statusErrorBGR;
    COLORREF m_statusPendingBGR;
    COLORREF m_statusWarningBGR;
    COLORREF m_statusDisabledBGR;

    /* Brushes used to paint status indicators. */
    CBrush *m_statusOkBrush;
    CBrush *m_statusErrorBrush;
    CBrush *m_statusPendingBrush;
    CBrush *m_statusDisabledBrush;

    /* GUI controls. */
    SPxWinMessageCtrl m_mesgCtrl;
    CButton m_connectBtn;
    CButton m_controlBtn;
    CButton m_restoreDefaultsBtn;
    CStatic m_connectIcon;
    CStatic m_controlIcon;
    CStatic m_connectLabel;
    CStatic m_controlLabel;
    ControlStatus_t m_heartbeatCS;
    ControlStatus_t m_profileTypeCS;
    ControlStatus_t m_profileIDCS;
    CButton m_profileSetBtn;
    ControlStatus_t m_opStateCS;
    ControlStatus_t m_rangeCS;
    ControlStatus_t m_freqCS;
    ControlStatus_t m_powerLPICS;
    ControlStatus_t m_powerDbmCS;
    ControlStatus_t m_antSpeedCS;
    ControlStatus_t m_transmitCS;
    ControlStatus_t m_freqHopCS;
    ControlStatus_t m_antAzimuthCS;
    ControlStatus_t m_scanModeCS;
    PositionCS_t m_sasPosCS;
    CButton m_sasPosSetBtn;
    PositionCS_t m_burnThroPosCS;
    CButton m_burnThroPosSetBtn;
    ZoneCS_t m_guardZoneCS;
    CButton m_guardZoneSetBtn;
    ZoneCS_t m_nonAcqZoneCS;
    CButton m_nonAcqZoneSetBtn;
    SectorCS_t m_txBlankSecCS;
    CButton m_txBlankSecSetBtn;
    CSPxWinEditCtrl m_dropTrackIDEdit;
    CButton m_dropTrackBtn;
    ControlStatus_t m_radarTracksCS;
    ControlStatus_t m_iffTracksCS;
    ControlStatus_t m_trackCorrCS;
    ControlStatus_t m_cuedTrackCS;
    ControlStatus_t m_rainModeCS;
    SPxWinEditCtrl m_terrainMapEdit;
    CButton m_terrainMapLoadBtn;
    SPxWinListCtrl m_commsLinkListCtrl;
    CStatic m_commsLinkStatusIcon;
    SPxWinListCtrl m_lruListCtrl;
    CStatic m_lruStatusIcon;
    SPxWinListCtrl m_moduleAlarmListCtrl;
    CStatic m_moduleAlarmStatusIcon;
    SPxWinListCtrl m_trackNotificationListCtrl;

    /*
     * Private functions.
     */

    /* General. */
    void syncGUI(BOOL updateCtrls);

    void syncConnectionPanel(void);
    void syncProfilePanel(BOOL updateCtrls);
    void syncRadarPanel(BOOL updateCtrls);
    void syncTrackerPanel(BOOL updateCtrls);
    void syncScanModePanel(BOOL updateCtrls);
    void syncBurnThroPositionPanel(BOOL updateCtrls);
    void syncTxBlankingSectorsPanel(BOOL updateCtrls);
    void syncGuardZonesPanel(BOOL updateCtrls);
    void syncNonAcqZonesPanel(BOOL updateCtrls);
    void syncTerrainMapPanel(void);
    void syncCommsLinkStatusPanel(void);
    void syncLRUStatusPanel(void);
    void syncModuleAlarmsPanel(void);
    void syncTrackNotificationsPanel(void);

    void syncSectorCS(SectorCS_t *sectorCS,
                      BOOL updateCtrl,
                      BOOL isCtrlEnabled,
                      BOOL isStatusEnabled,
                      SPxErrorCode statusErr,
                      BOOL isEnabled,
                      REAL32 startDegs,
                      REAL32 stopDegs);

    void syncZoneCS(ZoneCS_t *zoneCS,
                    BOOL updateCtrl,
                    BOOL isCtrlEnabled,
                    BOOL isStatusEnabled,
                    SPxErrorCode statusErr,
                    BOOL isEnabled,
                    REAL32 startDegs,
                    REAL32 stopDegs,
                    REAL32 startMetres,
                    REAL32 stopMetres);

    void syncPositionCS(PositionCS_t *posCS,
                        BOOL updateCtrl,
                        BOOL isCtrlEnabled,
                        BOOL isStatusEnabled,
                        SPxErrorCode statusErr,
                        BOOL isEnabled,
                        REAL32 azimuthDegs,
                        REAL32 elevationDegs,
                        REAL32 rangeMetres);

    void syncSpinCS(ControlStatus_t *cs,
                    BOOL updateCtrl,
                    BOOL isCtrlEnabled,
                    BOOL isStatusEnabled,
                    SPxErrorCode statusErr,
                    REAL32 value);

    void syncComboCS(ControlStatus_t *cs,
                     BOOL updateCtrl,
                     BOOL isCtrlEnabled,
                     BOOL isStatusEnabled,
                     SPxErrorCode statusErr,
                     UINT32 value);

    void syncCheckCS(ControlStatus_t *cs,
                     BOOL updateCtrl,
                     BOOL isCtrlEnabled,
                     BOOL isStatusEnabled,
                     SPxErrorCode statusErr,
                     BOOL isChecked);

    void syncCS(ControlStatus_t *cs,
                BOOL isCtrlEnabled,
                BOOL isStatusEnabled,
                SPxErrorCode statusErr,
                const char *statusText);

    void setScanMode(void);
    void setBurnThroPosition(void);
    void setTxBlankingSector(void);
    void setGuardZone(void);
    void setNonAcqZone(void);

    SPxStatus_t getOverallStatus(void);

    void logToMesgWindow(const char *text);

    void getStatusString(SPxP395Ctrl::Status_t status,
                         CStringA *str);

    void getCommsLinkStatusString(SPxP395Ctrl::CommsLinkStatus_t status,
                                  CStringA *str);

    /* GUI callback functions */
    afx_msg void OnConnectButtonClicked(void);
    afx_msg void OnControlButtonClicked(void);
    afx_msg void OnRestoreDefaultsButtonClicked(void);
    afx_msg void OnSetProfileButtonClicked(void);
    afx_msg void OnDropTrackButtonClicked(void);
    afx_msg void OnLoadTerrainMapButtonClicked(void);
    afx_msg void OnSetStopAndStarePositon(void);
    afx_msg void OnSetBurnThroPosition(void);
    afx_msg void OnSetTxBlankingSector(void);
    afx_msg void OnSetGuardZone(void);
    afx_msg void OnSetNonAcqZone(void);

    afx_msg void OnComboSelectionChanged(UINT nID);
    afx_msg void OnCheckboxChanged(UINT nID);
    afx_msg LRESULT OnSpinChanged(WPARAM wParam, LPARAM lParam);

    /*
     * Private static functions.
     */

    static void commsLinkIterator(SPxP395Ctrl *itf,
                                  void *userArg,
                                  const SPxP395Ctrl::CommsLink_t *commsLink);

    static void lruIterator(SPxP395Ctrl *itf,
                            void *userArg,
                            const SPxP395Ctrl::LRU_t *lru);

    static void moduleAlarmIterator(SPxP395Ctrl *itf,
                                    void *userArg,
                                    const SPxP395Ctrl::ModuleAlarm_t *moduleAlarm);

    static void trackNotificationIterator(SPxP395Ctrl *itf,
                                          void *userArg,
                                          const SPxP395Ctrl::TrackNotification_t *trackNot);

    static void mesgHandler(void *userArg, const char *text);

    static void spxErrorHandler(SPxErrorType errType,
                                SPxErrorCode errCode,
                                int arg1, int arg2,
                                const char *arg3, 
                                const char *arg4);

    using CDialog::Create;

}; /* class SPxP395CtrlDlgWin */

/*********************************************************************
 *
 * End of file
 *
 **********************************************************************/

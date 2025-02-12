/*********************************************************************
*
* (c) Copyright 2015, 2016, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxP172EmuRadarDlgWin.h,v $
* ID: $Id: SPxP172EmuRadarDlgWin.h,v 1.5 2016/02/22 14:56:40 rew Exp $
*
* Purpose:
*	Header for radar tabbed dialog for SPxP172Emulator - a
*	class for emulating a Kelvin Hughes SharpEye radar.
*
* Revision Control:
*   22/02/16 v1.5    AGC	Use const strings.
*
* Previous Changes:
*   10/12/15 1.4    AJH	Add extra control variables.
*   04/12/15 1.3    AJH	Add convenience functions.
*			Add new widget handlers and control variables.
*   01/12/15 1.2    AJH	Add display of processing sector.
*   24/11/15 1.1    AJH	Initial Version.
**********************************************************************/
#pragma once

/* Include the SPx resource header */
#include "SPxResource.h"

/* Other SPx headers. */
#include "SPxLibWin/SPxTabbedDlgWin.h"
#include "SPxLibWin/SPxWinSpinCtrl.h"
#include "SPxLibData/SPxP172Emulator.h"
#include "afxwin.h"

/*********************************************************************
*
*   Type definitions
*
**********************************************************************/

class SPxP172EmuRadarDlgWin : public SPxTabbedItemDlg
{
    DECLARE_DYNAMIC(SPxP172EmuRadarDlgWin)

public:
    SPxP172EmuRadarDlgWin(CWnd* pParent, SPxP172Emulator *obj);
    virtual ~SPxP172EmuRadarDlgWin();
    BOOL Create(void);
    virtual void SyncFromObj(void);

    /* Dialog Data. */
    enum { IDD = IDD_SPX_P172_EMU_RADAR_DIALOG };
    virtual int GetIDD(void) const { return IDD; }
    virtual int ShowCheck(void) { return FALSE; }
    //virtual void Enable(int enable);
    //virtual int GetEnable(void);

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual BOOL OnInitDialog(void);
    virtual void OnOK(void){ /* Don't call the CDialog handler */ };
    virtual void OnCancel(void){ /* Don't call the CDialog handler */ };
    virtual BOOL PreTranslateMessage(MSG *pMsg);
    DECLARE_MESSAGE_MAP()

private:
    /* The parent window. */
    CWnd *m_parentWin;

    /* The object we are controlling. */
    SPxP172Emulator *m_obj;

    /* Boolean to indicate whether we are currently syncing from object. */
    int m_inSyncFromObj;

    //mutable SPxCriticalSection m_mutex;	/* Mutex protection. */

    void showProcSect(const SPxP172EmulatorRadarParameters *radarParams);
    void showAlarms(const SPxP172EmulatorRadarParameters &radarParams);
    void setAlarmsFromWidget(SPxP172EmulatorRadarParameters &radarParams);
    void syncToObj(void);
    unsigned int alarmSelected(const char *alarmName);
    void selectAlarm(const char *alarmName, unsigned int selected);

    using CDialog::Create;

public:
    /* Widget message handlers. */
    afx_msg void OnEnChangeSpxP172EmuButton(UINT ctrlId);
    afx_msg void OnEnChangeSpxP172EmuEditBox(UINT ctrlId);

    afx_msg void OnCbnSelchangeSpxP172EmuRadarRange(void);
    afx_msg void OnBnClickedSpxP172EmuAlarmClear(void);
    afx_msg void OnLbnSelchangeSpxP172EmuAlarmList(void);
    LRESULT OnSpinChange(WPARAM wParam, LPARAM lParam);

private:
    /* Widget handles. */
    CStatic m_radarStatusCtrl;
    CStatic m_radarNodeCtrl;
    CStatic m_displayNodeCtrl;

    CEdit m_radarRPMCtrl;
    CComboBox m_radarRangeCtrl;
    CEdit m_radarFrequencyCtrl;
    CEdit m_radarSeaCtrl;
    CEdit m_radarRainCtrl;
    CEdit m_radarSquintCtrl;
    CButton m_radarRaconCtrl;
    CEdit m_radarVideoRangeCtrl;

    CButton m_radarRunCtrl;
    CButton m_radarMuteCtrl;
    CButton m_radarPowerCtrl;
    CButton m_radarTWSCtrl;
    CButton m_radarSARTCtrl;
    CButton m_radarChirpCtrl;
    CButton m_radarEnhancedPulsesCtrl;
    CButton m_radarEngBuildCtrl;
    CButton m_radarRPICtrl;
    CButton m_radarAutoSeaCtrl;

    CEdit m_platHeadingCtrl;
    CEdit m_platSkewCtrl;
    CEdit m_platCOGCtrl;
    CEdit m_platSOGCtrl;

    CEdit m_trueSectStartCtrl;
    CEdit m_trueSectStopCtrl;
    CButton m_trueSectEnabCtrl;

    CEdit m_relSectStartCtrl[P172_EMULATOR_MAX_REL_SECTOR];
    CEdit m_relSectStopCtrl[P172_EMULATOR_MAX_REL_SECTOR];
    CButton m_relSectEnabCtrl[P172_EMULATOR_MAX_REL_SECTOR];

    SPxWinSpinCtrl m_procSectNumCtrl;
    CEdit m_procSectPriorityCtrl;
    CButton m_procSectActiveCtrl;
    CButton m_procSectNorthCtrl;
    CButton m_procSectRangedCtrl;
    CButton m_procSectCCZCtrl;
    CEdit m_procSectStartAngleCtrl;
    CEdit m_procSectStopAngleCtrl;
    CEdit m_procSectMinRangeCtrl;
    CEdit m_procSectMaxRangeCtrl;
    CListBox m_alarmListCtrl;
    CButton m_alarmClearCtrl;

    CEdit m_FPGAVersionCtrl;
    CEdit m_softwareVersionCtrl;
    CEdit m_zmNumberCtrl;
    CEdit m_customerIDCtrl;
    CEdit m_FPGA1TempCtrl;
    CEdit m_FPGA2TempCtrl;
    CEdit m_onTimeCtrl;
    CEdit m_runTimeCtrl;

    CToolTipCtrl m_toolTip;
};

/*********************************************************************
*
*   End of File
*
**********************************************************************/

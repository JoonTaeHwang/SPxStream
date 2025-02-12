/*********************************************************************
*
* (c) Copyright 2020, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for general-purpose remote radar control dialog.
*
*
* Revision Control:
*   25/11/20 v1.3    BTB	Fix line endings.
*
* Previous Changes:
*   24/11/20 1.2    BTB	Support enumerated range scale.
*   19/11/20 1.1    BTB	Initial version.
*
**********************************************************************/

#pragma once

/* Include the SPx resource header. */
#include "SPxResource.h"

/* For SPxRemoteRadarCtrl. */
#include "SPxLibNet/SPxRemoteRadarCtrl.h"

/* For SPxAutoPtr. */
#include "SPxLibUtils/SPxAutoPtr.h"

/* Include base class header. */
#include "SPxLibWin/SPxTabbedPopupDlgWin.h"

/*
 * SPxRemoteRadarCtrlDlgWin dialog
 */
class SPxRemoteRadarCtrlDlgWin : public SPxTabbedItemDlg
{
    DECLARE_DYNAMIC(SPxRemoteRadarCtrlDlgWin)

public:
    /* Constructor and destructor. */
    SPxRemoteRadarCtrlDlgWin(CWnd* pParent, SPxRemoteRadarCtrl *radarConn);
    virtual ~SPxRemoteRadarCtrlDlgWin(void);
    BOOL Create(void);

    void SetController(SPxRemoteRadarCtrl *radarConn);

    virtual void SyncFromObj(void);

    virtual int GetIDD(void) const { return IDD; }

    /* Dialog Data */
    enum { IDD = IDD_SPX_REMOTE_RADAR_CTRL_DIALOG };

protected:
    /*
     * Protected variables.
     */

    /*
     * Protected functions.
     */

    DECLARE_MESSAGE_MAP()
    virtual void DoDataExchange(CDataExchange* pDX); 
    virtual BOOL OnInitDialog(void);
    virtual void OnOK(void){};
    virtual void OnCancel(void){};

    /* GUI callback functions. */
    virtual void OnTimer(UINT_PTR nIDEvent);

private:
    /*
     * Private variables.
     */

    struct impl;
    SPxAutoPtr<impl> m_p;

    /* Parent window. */
    CWnd *m_parentWin;

    /* Remote control connection. */
    SPxRemoteRadarCtrl *m_radarConn;

    /* Whether to sync controls to the radar state on updating. */
    int m_radarSyncRequired;

    /* Whether or not we are using the range scale combo rather than the slider. */
    int m_usingRangeCombo;

    /* Child controls. */
    CButton m_txEnableBn;
    CButton m_autoGainBn;
    CComboBox m_rangeScaleList;
    CComboBox m_seaModeList;
    CSliderCtrl m_rangeSlider;
    CSliderCtrl m_rainSlider;
    CSliderCtrl m_gainSlider;
    CSliderCtrl m_seaSlider;
    CStatic m_idLabel;
    CStatic m_descLabel;
    CStatic m_txStatus;
    CStatic m_rangeStatus;
    CStatic m_rainStatus;
    CStatic m_gainStatus;
    CStatic m_seaStatus;

    /*
     * Private functions.
     */

     /* General. */
    void syncToRadarState(void);
    void syncGUI(void);

    /* GUI callback functions. */
    afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar);
    afx_msg void OnTransmitEnable(void);
    afx_msg void OnGainAuto(void);
    afx_msg void OnRangeScale(void);
    afx_msg void OnSeaClutterMode(void);

    using CDialog::Create;

};  /* class SPxRemoteRadarCtrlDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/

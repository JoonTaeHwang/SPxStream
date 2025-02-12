/*********************************************************************
*
* (c) Copyright 2018 - 2020, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for remote P226 radar control dialog.
*
*
* Revision Control:
*   20/11/20 v1.12  BTB 	Rename class.
*
* Previous Changes:
*   19/11/20 1.11  BTB 	Make constructor explicit.
*   16/11/20 1.10  BTB 	Update with changes to remote controller interface.
*   10/12/18 1.9   BTB 	Make initial controls sync optional.
*   01/08/18 1.8   BTB 	Simplify dialog status.
*			Support syncing controls during runtime.
*   03/07/18 1.7   BTB 	Add multi-language support.
*   23/05/18 1.6   BTB 	Improve dialog status functionality.
*			Add control for radar type.
*   22/05/18 1.5   BTB 	Add IsConnected().
*   22/05/18 1.4   BTB 	Support changing text colour for status fields.
*   21/05/18 1.3   BTB 	Fix spin controls.
*   16/05/18 1.2   BTB 	Override OnOK and OnCancel.
*   15/05/18 1.1   BTB 	Initial version.
*
**********************************************************************/

#pragma once

/* Include the SPx resource header. */
#define SPX_AFXWIN
#include "SPxResource.h"

/* For SPxWinSpinCtrl. */
#include "SPxLibWin/SPxWinSpinCtrl.h"

/* For SPxRemoteP226Ctrl. */
#include "SPxLibNet/SPxRemoteP226Ctrl.h"

/*
 * SPxRemoteP226CtrlDlgWin dialog
 */
class SPxRemoteP226CtrlDlgWin : public CDialog
{
public:

    /* Constructor and destructor. */
    explicit SPxRemoteP226CtrlDlgWin(SPxRemoteP226Ctrl *p226Conn, CWnd *parentWin = NULL, int doInitialSync = TRUE);
    virtual ~SPxRemoteP226CtrlDlgWin(void);
    virtual BOOL Create(CWnd *parent);
    virtual SPxStatus_t GetStatus(void){ return m_p226Conn->IsConnected() ? SPX_STATUS_OK : SPX_STATUS_ERROR; }

    void SyncControls(void);

    /* Dialog Data */
    enum { IDD = IDD_SPX_REMOTE_P226_CTRL_DIALOG };

protected:

    DECLARE_MESSAGE_MAP()
    virtual void DoDataExchange(CDataExchange* pDX); 
    virtual BOOL OnInitDialog(void);
    virtual void OnOK(void){ /* Don't call the CDialog handler */ };
    virtual void OnCancel(void){ /* Don't call the CDialog handler */ };

private:
    /* Private variables. */

    /* General. */
    struct impl;
    SPxAutoPtr<impl> m_p;

    SPxRemoteP226Ctrl *m_p226Conn;

    unsigned int m_currSector;

    int m_doInitialSync;

    BOOL m_syncRequired;

    /* Transmitting enable/disable. */
    CButton *m_transmitEnableBn;

    /* Range (metres). */
    SPxWinSpinCtrl m_rangeSpin;

    /* Scan rate mode. */
    CSliderCtrl *m_scanRateModeSlider;

    /* Gain. */
    CButton *m_autoGainBn;
    CSliderCtrl *m_manualGainSlider;

    /* Sea Clutter. */
    CComboBox *m_autoSeaClutterMode;
    CSliderCtrl *m_manualSeaClutterSlider;

    /* Rain. */
    CSliderCtrl *m_rainSlider;

    /* FTC. */
    CSliderCtrl *m_ftcSlider;

    /* Interference rejection. */
    CSliderCtrl *m_interferenceSlider;

    /* LocalIR. */
    CSliderCtrl *m_localIRSlider;

    /* Sector Blanking. */
    CComboBox *m_blankingSectorID;
    CButton *m_blankingEnableBn;
    SPxWinSpinCtrl m_startAngleSpin;
    SPxWinSpinCtrl m_endAngleSpin;

    /* Status Fields. */
    CWnd *m_type;
    CWnd *m_state;
    CWnd *m_rpm;
    CWnd *m_transActual;
    CWnd *m_rangeActual;
    CWnd *m_rateActual;
    CWnd *m_gainActual;
    CWnd *m_seaActual;
    CWnd *m_rainActual;
    CWnd *m_ftcActual;
    CWnd *m_intActual;
    CWnd *m_lirActual;
    CWnd *m_blankActual;
    CWnd *m_stAngleActual;
    CWnd *m_eAngleActual;

    /* Colour of text for status fields. */
    COLORREF m_transColour;
    COLORREF m_rangeColour;
    COLORREF m_rateColour;
    COLORREF m_gainColour;
    COLORREF m_seaColour;
    COLORREF m_rainColour;
    COLORREF m_ftcColour;
    COLORREF m_intColour;
    COLORREF m_lirColour;
    COLORREF m_blankColour;
    COLORREF m_stAngleColour;
    COLORREF m_eAngleColour;

    /* Private functions. */
    void UpdateStatusFields(void);
    void UpdateBlankingStatus(void);

    /* GUI callback functions. */
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg HBRUSH OnCtlColor(CDC *pDC, CWnd *pWnd, UINT nCtlColor);
    afx_msg LRESULT OnSpinChange(WPARAM wParam, LPARAM lParam); 
    afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar);
    afx_msg void OnTransmitEnable(void);
    afx_msg void OnGainAuto(void);
    afx_msg void OnBlankingEnable(void);
    afx_msg void OnSeaClutterMode(void);
    afx_msg void OnBlankingSector(void);

    using CDialog::Create;

}; /* class SPxRemoteP226CtrlDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/

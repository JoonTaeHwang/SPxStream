/*********************************************************************
*
* (c) Copyright 2011 - 2019, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for the Scenario Generator control child dialog.
*
*
* Revision Control:
*   04/04/19 v1.9    AGC	Fix per-monitor DPI issue.
*
* Previous Changes:
*   03/03/16 1.8    AGC	Fix clang warnings.
*   26/01/15 1.7    AGC	Remove unused HICON.
*   21/11/14 1.6    AGC	Add spin controls using resource file.
*   27/09/13 1.5    AGC	Include SPxCommon header.
*   26/10/12 1.4    AGC	Fix 64-bit error.
*   21/08/12 1.3    AGC	Remove unused set header.
*   29/03/12 1.2    AGC	Support hiding the network controls.
*			Add noise controls.
*   25/10/11 1.1    AGC	Initial version.
*
**********************************************************************/

#pragma once

/* Standard headers. */

/* Include the SPx resource header */
#include "SPxResource.h"

/* For SPxSGtargetState. */
#include "SPxLibData/SPxScenGen.h"

/* For SPxWinSpinCtrl. */
#include "SPxLibWin/SPxWinSpinCtrl.h"

#define SPX_AFXWIN
#include "SPxLibUtils/SPxCommon.h"

/*
 * SPxScenGenCtrlDlgWin dialog
 */
class SPxScenGenCtrlDlgWin : public CDialog
{
    DECLARE_DYNAMIC(SPxScenGenCtrlDlgWin)

public:
    /* Constructor and destructor. */
    SPxScenGenCtrlDlgWin(CWnd* pParent, 
			 SPxScenarioGenerator *obj,
			 int showNetCtrls=TRUE);
    virtual ~SPxScenGenCtrlDlgWin();
    BOOL Create(void);
    void SyncFromObj(void);

    /* Dialog Data */
    enum { IDD = IDD_SPX_SRC_SCEN_GEN_CTRL_DIALOG };

    /* Configuration retrieval functions. */
    SPxScenarioGenerator *GetObj(void) { return m_obj; }

protected:
    virtual void DoDataExchange(CDataExchange* pDX); 
    virtual BOOL OnInitDialog();
    virtual void OnOK(void){ /* Don't call the CDialog handler */ };
    virtual void OnCancel(void){ /* Don't call the CDialog handler */ };
    virtual void OnTimer(UINT_PTR nIDEvent);
    virtual void OnSetFont(CFont *pFont);
    DECLARE_MESSAGE_MAP()

private:
    /* Private variables. */

    /* The parent window */
    CWnd *m_parentWin;

    /* The object we are controlling */
    SPxScenarioGenerator *m_obj;

    /* Current target id. */
    int m_targetId;
    int m_targetIdChanged;

    /* Is auto-update enabled? */
    int m_autoUpdateEnabled;

    /* Controls. */
    int m_showNetCtrls;
    CSliderCtrl m_noiseSlider;
    CStatic m_noiseLabel;
    CButton m_nearNoiseCheck;
    SPxWinSpinCtrl m_nearNoiseGain;
    SPxWinSpinCtrl m_nearNoiseRange;
    CButton m_autoUpdateCheck;
    CComboBox m_idCombo;
    CStatic m_idLabel;
    SPxWinSpinCtrl m_posXEdit;
    SPxWinSpinCtrl m_posYEdit;
    SPxWinSpinCtrl m_sizeBreadthEdit;
    SPxWinSpinCtrl m_sizeLengthEdit;
    SPxWinSpinCtrl m_speedEdit;
    SPxWinSpinCtrl m_headingEdit;
    CButton m_deleteBtn;
    CComboBox m_distRecvCombo;
    CIPAddressCtrl m_distRecvAddress;
    CEdit m_distRecvPort;
    CButton m_distRecvSet;
    CStatic m_distRecvComboLabel;
    CStatic m_distRecvAddressLabel;
    CStatic m_distRecvPortLabel;
    CStatic m_controlsGroup;

    /* Private functions */
    void updateAddressCtrls(void);
    static void targetFnWrapper(SPxScenarioGenerator *obj, 
				int targetId,
				SPxSGtargetState state,
				void *userArg);
    void targetFn(SPxScenarioGenerator *obj, 
		  int targetId,
		  SPxSGtargetState state,
		  void *userArg);

    /*
     * Handler functions.
     */
    void OnReadFile(void);
    void OnSave(void);
    void OnDeleteAll(void);
    void OnAdd(void);
    void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar);
    void OnNearNoiseCheckChange(void);
    void OnAutoUpdateCheckChange(void);
    LRESULT OnSpinChange(WPARAM wParam, LPARAM lParam);
    void OnChangeID(void);
    void OnDelete(void);
    void OnDistReceiptChange(void);
    void OnAddressSet(void);

    using CDialog::Create;

}; /* class SPxScenGenCtrlDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/

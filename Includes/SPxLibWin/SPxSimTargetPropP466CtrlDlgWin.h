/*********************************************************************
*
* (c) Copyright 2020, Cambridge Pixel Ltd.
*
*
* Purpose:
*	Header for the Simulated Target P466 properties dialog.
*
*
* Revision Control:
*   29/10/20 v1.2    AGC	Extend P466 support.
*
* Previous Changes:
*   26/08/20 1.1    AGC	Initial version.
**********************************************************************/

#pragma once

/* Standard headers. */

/* Include the SPx resource header */
#include "SPxResource.h"

/* For SPxAutoPtr. */
#include "SPxLibUtils/SPxAutoPtr.h"

/* For SPxWinSpinCtrl. */
#include "SPxLibWin/SPxWinSpinCtrl.h"

/* For base class. */
#include "SPxLibWin/SPxTabbedDlgWin.h"

/* Forward declarations. */
class SPxSimTarget;

/*
 * SPxSimTargetPropP466CtrlDlgWin dialog
 */
class SPxSimTargetPropP466CtrlDlgWin : public SPxTabbedItemDlg
{
    DECLARE_DYNAMIC(SPxSimTargetPropP466CtrlDlgWin)

public:
    /* Constructor and destructor. */
    SPxSimTargetPropP466CtrlDlgWin(CWnd* pParent,
				   SPxSimTarget *obj);
    virtual ~SPxSimTargetPropP466CtrlDlgWin(void);
    BOOL Create(void);
    virtual void SyncFromObj(void);

    /* Dialog Data */
    enum { IDD = IDD_SPX_SIM_TARGET_PROP_P466_CTRL_DIALOG };
    virtual int GetIDD(void) const { return IDD; }

    /* Configuration functions. */
    void SetObj(SPxSimTarget *obj);
    SPxSimTarget *GetObj(void) { return m_obj; }

protected:
    virtual void DoDataExchange(CDataExchange* pDX); 
    virtual BOOL OnInitDialog(void);
    virtual void OnOK(void){ /* Don't call the CDialog handler */ };
    DECLARE_MESSAGE_MAP()

private:
    /* Private variables. */

    /* The parent window */
    CWnd *m_parentWin;

    /* The object we are controlling */
    SPxSimTarget *m_obj;

    /* P466 controls. */
    CButton m_checkIdentity;
    SPxWinSpinCtrl m_spinTrackNum;
    SPxWinSpinCtrl m_spinIdentity;
    CButton m_checkQuality;
    SPxWinSpinCtrl m_spinQuality;
    CButton m_checkMode6;
    CButton m_checkMode6A;
    CButton m_checkMode6B;
    CButton m_checkClassification;
    SPxWinSpinCtrl m_spinClassification;
    CButton m_checkAltitude;
    CButton m_checkPosition;
    CButton m_checkVelocity;
    CButton m_checkCallsign;

    /* Private functions */
    LRESULT OnSpinChange(WPARAM wParam, LPARAM lParam);
    void OnCheckIdentity(void);
    void OnCheckQuality(void);
    void OnCheckMode6(void);
    void OnCheckMode6A(void);
    void OnCheckMode6B(void);
    void OnCheckClassification(void);
    void OnCheckAltitude(void);
    void OnCheckPosition(void);
    void OnCheckVelocity(void);
    void OnCheckCallsign(void);

    using CDialog::Create;

}; /* class SPxSimTargetPropP466CtrlDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/

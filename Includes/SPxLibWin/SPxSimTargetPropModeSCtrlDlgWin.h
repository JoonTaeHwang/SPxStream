/*********************************************************************
*
* (c) Copyright 2017, 2020, Cambridge Pixel Ltd.
*
*
* Purpose:
*	Header for the Simulated Target IFF properties dialog.
*
*
* Revision Control:
*   26/08/20 v1.2    AGC	Remove unused members.
*
* Previous Changes:
*   01/12/17 1.1    AGC	Initial version.
**********************************************************************/

#pragma once

/* Standard headers. */

/* Include the SPx resource header */
#include "SPxResource.h"

/* For SPxAutoPtr. */
#include "SPxLibUtils/SPxAutoPtr.h"

/* For SPxWinSpinCtrl. */
#include "SPxLibWin/SPxWinSpinCtrl.h"

/* For SPxWinEditCtrl. */
#include "SPxLibWin/SPxWinEditCtrl.h"

/* For base class. */
#include "SPxLibWin/SPxTabbedDlgWin.h"

/* Forward declarations. */
class SPxSimTarget;

/*
 * SPxSimTargetPropCtrlDlgWin dialog
 */
class SPxSimTargetPropModeSCtrlDlgWin : public SPxTabbedItemDlg
{
    DECLARE_DYNAMIC(SPxSimTargetPropModeSCtrlDlgWin)

public:
    /* Constructor and destructor. */
    SPxSimTargetPropModeSCtrlDlgWin(CWnd* pParent,
				    SPxSimTarget *obj);
    virtual ~SPxSimTargetPropModeSCtrlDlgWin(void);
    BOOL Create(void);
    virtual void SyncFromObj(void);

    /* Dialog Data */
    enum { IDD = IDD_SPX_SIM_TARGET_PROP_MODE_S_CTRL_DIALOG };
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

    /* Mode-S controls. */
    SPxWinSpinCtrl m_spinAA;

    /* Private functions */
    LRESULT OnSpinChange(WPARAM wParam, LPARAM lParam);

    using CDialog::Create;

}; /* class SPxSimTargetPropModeSCtrlDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/

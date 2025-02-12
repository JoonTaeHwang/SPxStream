/*********************************************************************
*
* (c) Copyright 2020, Cambridge Pixel Ltd.
*
*
* Purpose:
*	Header for the Simulated Target MIL-2525-D properties dialog.
*
*
* Revision Control:
*   26/08/20 v1.1    AGC	Initial version.
*
* Previous Changes:
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
 * SPxSimTargetPropCtrlDlgWin dialog
 */
class SPxSimTargetPropMIL2525DCtrlDlgWin : public SPxTabbedItemDlg
{
    DECLARE_DYNAMIC(SPxSimTargetPropMIL2525DCtrlDlgWin)

public:
    /* Constructor and destructor. */
    SPxSimTargetPropMIL2525DCtrlDlgWin(CWnd* pParent,
				       SPxSimTarget *obj);
    virtual ~SPxSimTargetPropMIL2525DCtrlDlgWin(void);
    BOOL Create(void);
    virtual void SyncFromObj(void);

    /* Dialog Data */
    enum { IDD = IDD_SPX_SIM_TARGET_PROP_MIL2525D_CTRL_DIALOG };
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

    /* MIL-2525-D controls. */
    SPxWinSpinCtrl m_spinIdentity;
    SPxWinSpinCtrl m_spinSet;
    SPxWinSpinCtrl m_spinEntity;
    SPxWinSpinCtrl m_spinEntityType;
    SPxWinSpinCtrl m_spinEntitySubtype;
    SPxWinSpinCtrl m_spinModifier1;
    SPxWinSpinCtrl m_spinModifier2;

    /* Private functions */
    LRESULT OnSpinChange(WPARAM wParam, LPARAM lParam);

    using CDialog::Create;

}; /* class SPxSimTargetPropMIL2525DCtrlDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/

/*********************************************************************
*
* (c) Copyright 2017, Cambridge Pixel Ltd.
*
*
* Purpose:
*	Header for the Simulated Target flags properties  dialog.
*
*
* Revision Control:
*   01/12/17 v1.1    AGC	Initial version.
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

/* For SPxWinEditCtrl. */
#include "SPxLibWin/SPxWinEditCtrl.h"

/* For base class. */
#include "SPxLibWin/SPxTabbedDlgWin.h"

/* Forward declarations. */
class SPxSimTarget;

/*
 * SPxSimTargetPropFlagsCtrlDlgWin dialog
 */
class SPxSimTargetPropFlagsCtrlDlgWin : public SPxTabbedItemDlg
{
    DECLARE_DYNAMIC(SPxSimTargetPropFlagsCtrlDlgWin)

public:
    /* Constructor and destructor. */
    SPxSimTargetPropFlagsCtrlDlgWin(CWnd* pParent, 
				    SPxSimTarget *obj);
    virtual ~SPxSimTargetPropFlagsCtrlDlgWin(void);
    BOOL Create(void);
    virtual void SyncFromObj(void);

    /* Dialog Data */
    enum { IDD = IDD_SPX_SIM_TARGET_PROP_FLAGS_CTRL_DIALOG };
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

    /* The objects we are controlling */
    SPxSimTarget *m_obj;

    /* Other controls. */
    CButton m_checkSim;
    CButton m_checkTest;
    CButton m_checkGround;
    CButton m_checkManInit;
    CButton m_checkFixed;

    /* Private functions */
    void OnCheckSim(void);
    void OnCheckTest(void);
    void OnCheckGround(void);
    void OnCheckManInit(void);
    void OnCheckFixed(void);

    using CDialog::Create;

}; /* class SPxSimTargetPropFlagsCtrlDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/

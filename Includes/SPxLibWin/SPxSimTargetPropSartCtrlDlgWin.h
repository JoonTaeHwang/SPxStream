/*********************************************************************
*
* (c) Copyright 2017, Cambridge Pixel Ltd.
*
*
* Purpose:
*	Header for the Simulated Target SART properties dialog.
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

/* For base class. */
#include "SPxLibWin/SPxTabbedDlgWin.h"

/* Forward declarations. */
class SPxSimTarget;

/*
 * SPxSimTargetPropSartCtrlDlgWin dialog
 */
class SPxSimTargetPropSartCtrlDlgWin : public SPxTabbedItemDlg
{
    DECLARE_DYNAMIC(SPxSimTargetPropSartCtrlDlgWin)

public:
    /* Constructor and destructor. */
    SPxSimTargetPropSartCtrlDlgWin(CWnd* pParent, 
				   SPxSimTarget *obj);
    virtual ~SPxSimTargetPropSartCtrlDlgWin(void);
    BOOL Create(void);
    virtual void SyncFromObj(void);

    /* Dialog Data */
    enum { IDD = IDD_SPX_SIM_TARGET_PROP_SART_CTRL_DIALOG };
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

    /* SART controls. */
    CButton m_checkSart;
    CStatic m_lblSartPulse;
    SPxWinSpinCtrl m_spinSartPulse;

    /* Private functions */
    LRESULT OnSpinChange(WPARAM wParam, LPARAM lParam);
    void OnCheckSart(void);

    using CDialog::Create;

}; /* class SPxSimTargetPropSartCtrlDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/

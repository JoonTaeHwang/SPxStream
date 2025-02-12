/*********************************************************************
*
* (c) Copyright 2020, Cambridge Pixel Ltd.
*
*
* Purpose:
*	Header for the Simulated Target IJMS properties dialog.
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

/* For SPxWinEditCtrl. */
#include "SPxLibWin/SPxWinEditCtrl.h"

/* For base class. */
#include "SPxLibWin/SPxTabbedDlgWin.h"

/* Forward declarations. */
class SPxSimTarget;

/*
 * SPxSimTargetPropIjmsCtrlDlgWin dialog
 */
class SPxSimTargetPropIjmsCtrlDlgWin : public SPxTabbedItemDlg
{
    DECLARE_DYNAMIC(SPxSimTargetPropIjmsCtrlDlgWin)

public:
    /* Constructor and destructor. */
    SPxSimTargetPropIjmsCtrlDlgWin(CWnd* pParent,
				   SPxSimTarget *obj);
    virtual ~SPxSimTargetPropIjmsCtrlDlgWin(void);
    BOOL Create(void);
    virtual void SyncFromObj(void);

    /* Dialog Data */
    enum { IDD = IDD_SPX_SIM_TARGET_PROP_IJMS_CTRL_DIALOG };
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

    /* IJMS controls. */
    SPxWinEditCtrl m_editIJMS;

    /* Private functions */
    void OnEditIjmsChange(NMHDR*, LRESULT*);

    using CDialog::Create;

}; /* class SPxSimTargetPropIjmsCtrlDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/

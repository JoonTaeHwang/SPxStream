/*********************************************************************
*
* (c) Copyright 2017, Cambridge Pixel Ltd.
*
*
* Purpose:
*	Header for the Simulated Target Racon dialog.
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
 * SPxSimTargetPropRaconCtrlDlgWin dialog
 */
class SPxSimTargetPropRaconCtrlDlgWin : public SPxTabbedItemDlg
{
    DECLARE_DYNAMIC(SPxSimTargetPropRaconCtrlDlgWin)

public:
    /* Constructor and destructor. */
    SPxSimTargetPropRaconCtrlDlgWin(CWnd* pParent, 
				    SPxSimTarget *obj);
    virtual ~SPxSimTargetPropRaconCtrlDlgWin(void);
    BOOL Create(void);
    virtual void SyncFromObj(void);

    /* Dialog Data */
    enum { IDD = IDD_SPX_SIM_TARGET_PROP_RACON_CTRL_DIALOG };
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

    /* Racon controls. */
    CComboBox m_comboRacon;
    SPxWinSpinCtrl m_spinRaconOn;
    SPxWinSpinCtrl m_spinRaconOff;

    /* Private functions */
    LRESULT OnSpinChange(WPARAM wParam, LPARAM lParam);
    void OnRaconSelChange(void);

    using CDialog::Create;

}; /* class SPxSimTargetPropRaconCtrlDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/

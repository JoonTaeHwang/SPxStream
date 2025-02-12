/*********************************************************************
*
* (c) Copyright 2017, Cambridge Pixel Ltd.
*
*
* Purpose:
*	Header for the Simulated Target ADS-B properties dialog.
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

/* For base class. */
#include "SPxLibWin/SPxTabbedDlgWin.h"

/* Forward declarations. */
class SPxSimTarget;

/*
 * SPxSimTargetPropAdsbCtrlDlgWin dialog
 */
class SPxSimTargetPropAdsbCtrlDlgWin : public SPxTabbedItemDlg
{
    DECLARE_DYNAMIC(SPxSimTargetPropAdsbCtrlDlgWin)

public:
    /* Constructor and destructor. */
    SPxSimTargetPropAdsbCtrlDlgWin(CWnd* pParent, 
				   SPxSimTarget *obj);
    virtual ~SPxSimTargetPropAdsbCtrlDlgWin(void);
    BOOL Create(void);
    virtual void SyncFromObj(void);

    /* Dialog Data */
    enum { IDD = IDD_SPX_SIM_TARGET_PROP_ADSB_CTRL_DIALOG };
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

    /* ADS-B controls. */
    CComboBox m_comboCapabilities;
    CComboBox m_comboCategory;
    CComboBox m_comboSubCat;

    /* Private functions */
    void OnCapabilitiesSelChange(void);
    void OnCategorySelChange(void);
    void OnSubCatSelChange(void);

    using CDialog::Create;

}; /* class SPxSimTargetPropAdsbCtrlDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/

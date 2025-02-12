/*********************************************************************
*
* (c) Copyright 2018, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for Inland AIS control dialog.
*
*
* Revision Control:
*   25/06/18 v1.3    AGC	Correctly suppress cppcheck warning.
*
* Previous Changes:
*   23/01/18 1.2    AGC	Support multiple objects.
*   08/01/18 1.1    AGC	Initial version.
**********************************************************************/

#pragma once

/* Standard headers. */

/* Include the SPx resource header */
#include "SPxResource.h"

/* For SPxWinSpinCtrl. */
#include "SPxLibWin/SPxWinSpinCtrl.h"

/* For base class. */
#include "SPxLibWin/SPxTabbedDlgWin.h"

/* Forward declarations. */
struct SPxAISInfo;
class SPxAISIface;

/*
 * SPxAISInlandDlgWin dialog
 */
class SPxAISInlandDlgWin : public SPxTabbedItemDlg
{
    DECLARE_DYNAMIC(SPxAISInlandDlgWin)

public:
    /* Constructor and destructor. */
    SPxAISInlandDlgWin(CWnd* pParent,
		       unsigned int numObjects,
		       SPxAISIface *obj[]);
    virtual ~SPxAISInlandDlgWin(void);
    BOOL Create(void);
    virtual void SyncFromObj(void);
    // cppcheck-suppress virtualCallInConstructor
    virtual void SetObjIndex(int index);

    /* Configuration functions. */
    void SetObj(SPxAISIface *obj);
    SPxAISIface *GetObj(void) { return m_obj; }

    /* Dialog Data */
    enum { IDD = IDD_SPX_AIS_INLAND_CTRL_DIALOG };
    virtual int GetIDD(void) const { return IDD; }

protected:
    virtual void DoDataExchange(CDataExchange* pDX); 
    virtual BOOL OnInitDialog(void);
    virtual void OnOK(void) {}
    DECLARE_MESSAGE_MAP()

private:
    /* Private variables. */
    struct impl;
    SPxAutoPtr<impl> m_p;

    /* The parent window */
    CWnd *m_parentWin;

    /* The object we are controlling */
    SPxAISIface *m_obj;

    /* Controls. */
    CButton m_checkAvail;
    SPxWinSpinCtrl m_spinENI;
    SPxWinSpinCtrl m_spinERI;
    CComboBox m_comboHazard;

    /* Other variables. */

    /* Private functions. */
    void OnCheckInland(void);
    LRESULT OnSpinChange(WPARAM wParam, LPARAM lParam);
    void OnHazardSelChange(void);

    using CDialog::Create;

}; /* class SPxAISInlandDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/

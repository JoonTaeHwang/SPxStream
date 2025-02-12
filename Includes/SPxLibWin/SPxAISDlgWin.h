/*********************************************************************
*
* (c) Copyright 2014, 2016, 2018, 2019, 2021, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for AIS control dialog.
*
*
* Revision Control:
*   01/10/21 v1.8    AGC	Support max static draught config.
*
* Previous Changes:
*   29/11/19 1.7    AGC	Support AtoNs.
*   25/06/18 1.6    AGC	Correctly suppress cppcheck warning.
*   23/01/18 1.5    AGC	Support multiple objects.
*   18/01/18 1.4    AGC	Support class selection.
*			Improved ship type selection.
*			Support special manoeuvre indicator.
*			Support use in tabbed dialog.
*   03/03/16 1.3    AGC	Fix clang warnings.
*   06/06/14 1.2    AGC	Override OnOK() to prevent Enter closing dialog.
*   02/06/14 1.1    AGC	Initial version.
**********************************************************************/

#pragma once

/* Standard headers. */

/* Include the SPx resource header */
#include "SPxResource.h"

/* For SPxWinSpinCtrl. */
#include "SPxLibWin/SPxWinSpinCtrl.h"

/* For SPxWinEditCtrl */
#include "SPxLibWin/SPxWinEditCtrl.h"

/* For base class. */
#include "SPxLibWin/SPxTabbedDlgWin.h"

/* Forward declarations. */
struct SPxAISInfo;
class SPxAISIface;

/*
 * SPxAISDlgWin dialog
 */
class SPxAISDlgWin : public SPxTabbedItemDlg
{
    DECLARE_DYNAMIC(SPxAISDlgWin)

public:
    /* Constructor and destructor. */
    SPxAISDlgWin(CWnd* pParent,
		 unsigned int numObjects,
		 SPxAISIface *obj[]);
    virtual ~SPxAISDlgWin(void);
    BOOL Create(void);
    virtual void SyncFromObj(void);
    // cppcheck-suppress virtualCallInConstructor
    virtual void SetObjIndex(int index);

    /* Configuration functions. */
    void SetObj(SPxAISIface *obj);
    SPxAISIface *GetObj(void) { return m_obj; }

    /* Dialog Data */
    enum { IDD = IDD_SPX_AIS_CTRL_DIALOG };
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
    CComboBox m_comboClass;
    SPxWinSpinCtrl m_spinMmsi;
    SPxWinSpinCtrl m_spinImo;
    SPxWinSpinCtrl m_spinNavStatus;
    SPxWinEditCtrl m_editCallSign;
    SPxWinSpinCtrl m_spinShipType;
    CComboBox m_comboShipType1;
    CComboBox m_comboShipType2;
    SPxWinSpinCtrl m_spinDraught;
    CComboBox m_comboSpecManInd;
    SPxWinEditCtrl m_editDest;
    CButton m_checkEta;
    SPxWinSpinCtrl m_spinEtaDay;
    CComboBox m_comboEtaMonth;
    SPxWinSpinCtrl m_spinEtaHour;
    SPxWinSpinCtrl m_spinEtaMin;
    CComboBox m_comboAtoN;

    /* Other variables. */

    /* Private functions. */
    void OnClassSelChange(void);
    LRESULT OnSpinChange(WPARAM wParam, LPARAM lParam);
    void OnSetCallSign(NMHDR*, LRESULT*);
    void OnSetShipType(void);
    void OnSpecManSelChange(void);
    void OnSetDestination(NMHDR*, LRESULT*);
    void OnEtaMonthSelChange(void);
    void OnCheckEta(void);
    void OnAtoNSelChange(void);

    using CDialog::Create;

}; /* class SPxAISDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/

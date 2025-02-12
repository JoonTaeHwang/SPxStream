/*********************************************************************
*
* (c) Copyright 2012 - 2018, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for the Test Generator control child dialog.
*
*
* Revision Control:
*   25/06/18 v1.8    AGC	Correctly suppress cppcheck warnings.
*
* Previous Changes:
*   23/01/18 1.7    AGC	Support multiple objects.
*   31/01/17 1.6    AGC	Support units configuration.
*   03/03/16 1.5    AGC	Fix clang warnings.
*   26/01/15 1.4    AGC	Remove unused HICON.
*   01/09/14 1.3    AGC	Use custom control for spin controls.
*   08/03/13 1.2    AGC	Support use within tabbed dialog.
*   26/10/12 1.1    AGC	Initial version.
*
**********************************************************************/

#pragma once

/* Standard headers. */

/* Include the SPx resource header */
#include "SPxResource.h"

/* For SPxWinSpinCtrl. */
#include "SPxLibWin/SPxWinSpinCtrl.h"

/* For base class. */
#include "SPxLibWin/SPxTabbedPopupDlgWin.h"

/* Forward declarations. */
class SPxTestGenerator;

/*
 * SPxTestGenCtrlDlgWin dialog
 */
class SPxTestGenCtrlDlgWin : public SPxTabbedItemDlg
{
    DECLARE_DYNAMIC(SPxTestGenCtrlDlgWin)

public:
    /* Constructor and destructor. */
    SPxTestGenCtrlDlgWin(CWnd* pParent, SPxTestGenerator *obj);
    SPxTestGenCtrlDlgWin(CWnd* pParent,
			 unsigned int numObjects,
			 SPxTestGenerator *obj[]);
    virtual ~SPxTestGenCtrlDlgWin();
    BOOL Create(void);
    virtual void SyncFromObj(void);

    /* Dialog Data */
    enum { IDD = IDD_SPX_SRC_TEST_GEN_CTRL_DIALOG };
    virtual int GetIDD(void) const { return IDD; }
    virtual CWnd *GetGroupBox(void) { return &m_mainGroup; }
    virtual int ShowCheck(void) { return TRUE; }
    virtual void Enable(int enable);
    virtual int GetEnable(void);

    void SetDisplayUnits(SPxUnitsDist_t distUnits);

    /* Configuration retrieval functions. */
    SPxTestGenerator *GetObj(void) { return m_obj; }
    // cppcheck-suppress virtualCallInConstructor
    virtual void SetObjIndex(int index);

protected:
    virtual void DoDataExchange(CDataExchange* pDX); 
    virtual BOOL OnInitDialog();
    virtual void OnOK(void){ /* Don't call the CDialog handler */ };
    virtual void OnCancel(void){ /* Don't call the CDialog handler */ };
    DECLARE_MESSAGE_MAP()

private:
    /* Private variables. */
    struct impl;
    SPxAutoPtr<impl> m_p;

    /* The parent window */
    CWnd *m_parentWin;
    SPxUnitsDist_t m_unitsDist;

    /* The object we are controlling */
    SPxTestGenerator *m_obj;

    /* Controls. */
    CStatic m_mainGroup;
    SPxWinSpinCtrl m_spinPattern;
    SPxWinSpinCtrl m_spinArg1;
    SPxWinSpinCtrl m_spinPeriod;
    SPxWinSpinCtrl m_spinPrf;
    SPxWinSpinCtrl m_spinRange;
    CStatic m_lblRange;

    /*
     * Handler functions.
     */
    LRESULT OnSpinChange(WPARAM wParam, LPARAM lParam);

    using SPxTabbedItemDlg::Create;
    
}; /* class SPxTestGenCtrlDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/

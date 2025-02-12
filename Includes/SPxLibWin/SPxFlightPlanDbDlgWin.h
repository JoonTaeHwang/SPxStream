/*********************************************************************
*
* (c) Copyright 2014, 2016, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxFlightPlanDbDlgWin.h,v $
* ID: $Id: SPxFlightPlanDbDlgWin.h,v 1.4 2016/03/03 10:59:08 rew Exp $
*
* Purpose:
*	Header for the SPxFlightPlanDb control child dialog.
*
*
* Revision Control:
*   03/03/16 v1.4    AGC	Fix clang warnings.
*
* Previous Changes:
*   16/06/14 1.3    REW	Keep timestamp of last sync.
*   16/01/14 1.2    REW	Keep count of plans.
*   15/01/14 1.1    REW	Initial Version.
**********************************************************************/

#pragma once

/* Include the SPx resource header */
#include "SPxResource.h"

/* Forward declarations. */
class SPxFlightPlan;
class SPxFlightPlanDb;
class SPxFlightPlanPopupDlgWin;

/*
 * SPxFlightPlanListBox class.
 */
class SPxFlightPlanListBox : public CListBox
{
public:
    SPxFlightPlanListBox(void)		{ /* Do nothing */; }
    virtual ~SPxFlightPlanListBox(void)	{ /* Do nothing */; }
    virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
    virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
};

/*
 * SPxFlightPlanDbDlgWin class.
 */
class SPxFlightPlanDbDlgWin : public CDialog
{
    DECLARE_DYNAMIC(SPxFlightPlanDbDlgWin)

public:
    /* Constructor and destructor. */
    SPxFlightPlanDbDlgWin(CWnd* pParent, SPxFlightPlanDb *obj);
    virtual ~SPxFlightPlanDbDlgWin();
    BOOL Create(void);

    /* Dialog Data */
    enum { IDD = IDD_SPX_FLIGHT_PLAN_DB_DIALOG };

    /* Sync functions. */
    virtual void SyncFromObj(void);

    /* Configuration retrieval functions. */
    SPxFlightPlanDb *GetObj(void) { return m_obj; }

protected:
    virtual void DoDataExchange(CDataExchange* pDX); 
    virtual BOOL OnInitDialog();
    virtual void OnTimer(UINT_PTR nIDEvent);
    void OnShowWindow(BOOL bShow, UINT nStatus);
    virtual void OnOK(void){ /* Don't call the CDialog handler */ };
    virtual void OnCancel(void){ /* Don't call the CDialog handler */ };
    afx_msg void OnButtonCreate(void);
    afx_msg void OnButtonEdit(void);
    afx_msg void OnButtonDelete(void);
    afx_msg void OnButtonDeleteAll(void);
    afx_msg void OnButtonLoad(void);
    afx_msg void OnButtonSave(void);
    afx_msg void OnButtonActiveInactive(void);
    afx_msg void OnListBoxSelChanged(void);
    afx_msg void OnListBoxDoubleClick(void);
    DECLARE_MESSAGE_MAP()

private:
    /*
     * Private members.
     */
    /* GUI control variables. */
    SPxFlightPlanListBox m_listBox;
    int m_showActive;
    int m_showInactive;

    /* The parent window */
    CWnd *m_parentWin;

    /* The object we are controlling */
    SPxFlightPlanDb *m_obj;	   

    /* The flight plan editor. */
    SPxFlightPlanPopupDlgWin *m_editorDlg;

    /* Font for list box. */
    CFont m_font;

    /* Convenience copy of certain information. */
    UINT32 m_lastSyncMsecs;
    unsigned int m_numPlansTotal;
    unsigned int m_numPlansActive;
    unsigned int m_numPlansInactive;

    /*
     * Private functions
     */
    int addPlanToListBox(SPxFlightPlan *plan);

    using CDialog::Create;

    /*
     * Handler functions.
     */

    /* Static functions (e.g. for iterating lists). */
    static int staticAddPlanToListBox(void *db, void *arg, void *plan);
}; /* class SPxFlightPlanDbDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/

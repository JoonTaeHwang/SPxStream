/*********************************************************************
*
* (c) Copyright 2014, 2016, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxFlightPlanDlgWin.h,v $
* ID: $Id: SPxFlightPlanDlgWin.h,v 1.4 2016/03/03 10:59:08 rew Exp $
*
* Purpose:
*	Header for the SPxFlightPlan control child dialog.
*
*
* Revision Control:
*   03/03/16 v1.4    AGC	Fix clang warnings.
*
* Previous Changes:
*   16/06/14 1.3    REW	Keep time of last sync.
*   11/03/14 1.2    REW	Free combo box strings in OnDestroy().
*   15/01/14 1.1    REW	Initial Version.
**********************************************************************/

#pragma once

/* Library headers. */
#include "SPxLibUtils/SPxError.h"
#include "SPxLibUtils/SPxTime.h"
#include "SPxLibWin/SPxWinEditCtrl.h"

/* Include the SPx resource header */
#include "SPxResource.h"

/* Forward declarations. */
class SPxFlightPlan;

/*
 * SPxFlightPlanDlgWin class.
 */
class SPxFlightPlanDlgWin : public CDialog
{
    DECLARE_DYNAMIC(SPxFlightPlanDlgWin)

public:
    /* Constructor and destructor. */
    SPxFlightPlanDlgWin(CWnd* pParent);
    virtual ~SPxFlightPlanDlgWin();
    BOOL Create(void);

    /* Dialog Data */
    enum { IDD = IDD_SPX_FLIGHT_PLAN_DIALOG };

    /* Sync functions. */
    virtual void SyncFromObj(void);

    /* Flight plan handling. */
    SPxErrorCode SetFlightPlan(SPxFlightPlan *plan);
    SPxFlightPlan *GetFlightPlan(void) { return m_plan; }

protected:
    virtual void DoDataExchange(CDataExchange* pDX); 
    virtual BOOL OnInitDialog();
    virtual void OnTimer(UINT_PTR nIDEvent);
    virtual void OnDestroy(void);
    virtual void OnOK(void){ /* Don't call the CDialog handler */ };
    virtual void OnCancel(void){ /* Don't call the CDialog handler */ };
    DECLARE_MESSAGE_MAP()

private:
    /* Private functions */

    /* Handler functions.  */
    LRESULT OnEnterEdit(WPARAM wParam, LPARAM lParam);
    afx_msg void OnKillFocusCallsign(void);
    afx_msg void OnFlightRules(void);
    afx_msg void OnFlightType(void);
    afx_msg void OnKillFocusIFF(void);
    afx_msg void OnNumAC(void);
    afx_msg void OnKillFocusACType(void);
    afx_msg void OnWake(void);
    afx_msg void OnRepeat(void);
    afx_msg void OnKillFocusDepAero(void);
    afx_msg void OnETDHours(void);
    afx_msg void OnETDMins(void);
    afx_msg void OnKillFocusRoute(void);
    afx_msg void OnKillFocusSpeed(void);
    afx_msg void OnKillFocusAltitude(void);
    afx_msg void OnKillFocusDestAero(void);
    afx_msg void OnEETHours(void);
    afx_msg void OnEETMins(void);
    afx_msg void OnKillFocusRemarks(void);

    /* GUI control variables. */
    SPxWinEditCtrl m_valCallsign;	/* String */
    char m_valFlightRules[2];		/* Single character option */
    char m_valFlightType[2];		/* Single character option */
    SPxWinEditCtrl m_valIFF;		/* String */
    unsigned int m_valNumAC;		/* Number of aircraft */
    SPxWinEditCtrl m_valACType;		/* String */
    char m_valWake[2];			/* Single character option */
    int m_valRepeat;			/* Flag */
    SPxWinEditCtrl m_valDepAero;	/* String */
    unsigned int m_valETDHours;		/* Estimated time of departure */
    unsigned int m_valETDMins;		/* Estimated time of departure */
    SPxWinEditCtrl m_valRoute;		/* String */
    SPxWinEditCtrl m_valSpeed;		/* String */
    SPxWinEditCtrl m_valAltitude;	/* String */
    SPxWinEditCtrl m_valDestAero;	/* String */
    unsigned int m_valEETHours;		/* Estimated elapsed time */
    unsigned int m_valEETMins;		/* Estimated elapsed time */
    SPxWinEditCtrl m_valRemarks;	/* String */

    /* The parent window */
    CWnd *m_parentWin;

    /* The object we are controlling */
    SPxFlightPlan *m_plan;

    /* The time we last sync'ed the GUI. */
    SPxTime_t m_lastSyncTime;

    using CDialog::Create;

}; /* class SPxFlightPlanDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/

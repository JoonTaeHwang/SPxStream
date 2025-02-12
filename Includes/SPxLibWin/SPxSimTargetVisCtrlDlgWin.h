/*********************************************************************
*
* (c) Copyright 2014 - 2019, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxSimTargetVisCtrlDlgWin - a dialog for displaying
*	the reasons a target may not be visible.
*
*
* Revision Control:
*   22/03/19 v1.6    AGC	Fix minor high DPI font issue.
*
* Previous Changes:
*   06/12/17 1.5    AGC	Do not forward declare enum.
*   15/06/16 1.4    AGC	Support ramping of vertical beamwidth.
*   03/03/16 1.3    AGC	Fix clang warnings.
*   31/07/15 1.2    AGC	Add target terrain display.
*   18/07/14 1.1    AGC	Initial version.
**********************************************************************/
#pragma once

#include "SPxResource.h"

#define SPX_AFXWIN
#include "SPxLibUtils/SPxCommon.h"

#include "SPxLibUtils/SPxAutoPtr.h"

#include "SPxLibData/SPxRadarSimServer.h"

/* Forward declarations. */
class SPxRadarSimServer;
class SPxSimTarget;

// SPxSimTargetVisCtrlDlgWin dialog
class SPxSimTargetVisCtrlDlgWin : public CDialog
{
    DECLARE_DYNAMIC(SPxSimTargetVisCtrlDlgWin)

public:
    struct Item;
    enum ItemState
    {
	STATE_NOTOK = 0,
	STATE_OK = 1,
	STATE_WARN = 2
    };

    explicit SPxSimTargetVisCtrlDlgWin(CWnd* pParent,
	SPxRadarSimServer *server);
    virtual ~SPxSimTargetVisCtrlDlgWin();
    BOOL Create(void);
    void SyncFromObj(int showFirstIssue=FALSE);

    void SetObj(SPxSimTarget *target);
    SPxSimTarget *GetObj(void) const;

    // Dialog Data
    enum { IDD = IDD_SPX_SIM_TARGET_VIS_CTRL_DIALOG };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual void OnOK(void) {}
    virtual BOOL OnInitDialog(void);
    HBRUSH OnCtlColor(CDC *pDC, CWnd *pWnd, UINT nCtlColor);
    virtual INT_PTR OnToolHitTest(CPoint point, TOOLINFO* pTI) const;
    BOOL OnNeedToolTip(UINT id, NMHDR *pTTTStruct, LRESULT *pResult);
    void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar);
    void OnDpiChanged(WPARAM, LPARAM);
    DECLARE_MESSAGE_MAP()

private:
    struct impl;
    SPxAutoPtr<impl> m_p;

    void addItem(const char *label, const char *msg,
	SPxRadarSimTgtVis vis, unsigned int row, unsigned int col);
    void addItem(const char *label, const char *msg,
	SPxRadarSimTgtVisRadar vis, unsigned int row, unsigned int col);
    HBRUSH getBrush(ItemState state);

    using CDialog::Create;
};

/*********************************************************************
*
* End of file
*
**********************************************************************/

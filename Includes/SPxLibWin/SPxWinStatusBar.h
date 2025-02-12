/*********************************************************************
*
* (c) Copyright 2013 - 2016, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxWinStatusBar.h,v $
* ID: $Id: SPxWinStatusBar.h,v 1.7 2016/03/03 10:59:08 rew Exp $
*
* Purpose:
*	Header for a generic status bar (used by re-usable SPxServerDlgWin
*	dialog).
*
*	This class requires GDI+ to be initialised by the application.
*
*
* Revision Control:
*   03/03/16 v1.7    AGC	Fix clang warnings.
*
* Previous Changes:
*   09/09/14 1.6    AGC	Support callback when status bar item clicked.
*   23/07/14 1.5    AGC	Support static radar display.
*   20/06/14 1.4    AGC	Support using parameter for active state.
*   26/11/13 1.3    AGC	Separate updates from repositioning.
*   04/11/13 1.2    AGC	Support mouse interaction.
*			Support forced redraw.
*   21/10/13 1.1    AGC	Initial Version.
**********************************************************************/
#pragma once

#define SPX_AFXWIN
#include "SPxLibUtils/SPxCommon.h"

#include "SPxLibUtils/SPxAutoPtr.h"
#include "SPxLibUtils/SPxEventHandler.h"

/* Forward declarations. */
class SPxServerDlgWin;
class SPxNavData;

/* SPxWinStatusBar */
class SPxWinStatusBar : public CStatusBar
{
    DECLARE_DYNAMIC(SPxWinStatusBar)

public:
    typedef SPxEventHandler<void(*)()> Handler;

    /* Construction and destruction. */
    explicit SPxWinStatusBar(SPxServerDlgWin *parentDlg);
    explicit SPxWinStatusBar(CDialog *parentDlg);
    virtual ~SPxWinStatusBar(void);
    virtual BOOL Create(CWnd* pParentWnd,
			DWORD dwStyle = WS_CHILD | WS_VISIBLE | CBRS_BOTTOM,
			UINT nID = AFX_IDW_STATUS_BAR);

    void ShowTime(int show);
    void SetNavData(SPxNavData *navData, int showStatic=FALSE);
    void Add(const char *label, SPxObj *obj, SPxObj *extraObj=NULL);
    void Add(const char *label, SPxObj *obj, const char *activeParam);
    template<typename F, typename O>
    void Add(const char *label, SPxObj *obj, F fn, O *fnObj)
    {
	SPxSharedPtr<Handler> handler = SPxMakeShared<Handler>();
	handler->Add(fn, fnObj);
	add(label, obj, &handler);
    }
    int GetHeight(void) const;
    int GetMinWidth(void) const;

    /* Update the status bar - must be called when parent dialog is resized. */
    void Update(int forceRedraw=FALSE);
    void Reposition(void);

protected:
    /* Protected functions. */
    void OnLButtonUp(UINT nFlags, CPoint point);
    virtual void DrawItem(LPDRAWITEMSTRUCT lpDIS);
    DECLARE_MESSAGE_MAP()

private:
    /* Private variables. */
    struct impl;
    SPxAutoPtr<impl> m_p;

    /* Private functons. */
    void updateIndicators(void);
    void add(const char *label, SPxObj *obj, const SPxSharedPtr<Handler>* handler);
    
}; /* SPxWinStatusBar */

/*********************************************************************
*
* End of file
*
**********************************************************************/

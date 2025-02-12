/*********************************************************************
*
* (c) Copyright 2016 - 2020, Cambridge Pixel Ltd.
*
* Purpose:
*	Dialog to allow customisation of window title bar.
*
* Revision Control:
*   03/04/20 v1.7    AGC        Derive from SPxTabbedItemDlg.
*
* Previous Changes:
*   23/03/20 1.6    AGC Re-layout when enabling custom title bar.
*   01/05/19 1.5    REW	Support Get/SetCollapsed().
*   12/04/19 1.4    AGC	Support collapse/restore button.
*   26/09/18 1.3    REW	Make GetClientRect() public, not protected.
*   04/06/18 1.2    AGC	Support show/hide callback.
*			Support querying title bar height.
*   16/11/16 1.1    AGC	Initial Version.
**********************************************************************/

#pragma once

#define SPX_AFXWIN
#include "SPxLibUtils/SPxCommon.h"

#include "SPxLibUtils/SPxAutoPtr.h"

#include "SPxLibWin/SPxTabbedItemDlg.h"

class SPxWinCustomDlg : public SPxTabbedItemDlg
{
    DECLARE_DYNAMIC(SPxWinCustomDlg)

public:
    typedef void(*ShowHideCallback)(int shown, void *userArg);

    explicit SPxWinCustomDlg(UINT nIDTemplate, CWnd* pParentWnd=NULL);
    virtual ~SPxWinCustomDlg(void);

    void GetClientRect(LPRECT rect);

    void EnableCustomTitleBar(int enable);
    int IsCustomTitleBarEnabled(void) const;
    void EnableCollapseButton(int enable);
    int IsCollapseButtonEnabled(void) const;
    void SetTitleBackColour(COLORREF colour);
    COLORREF GetTitleBackColour(void) const;
    void SetTitleTextColour(COLORREF colour);
    COLORREF GetTitleTextColour(void) const;
    virtual void SetBackColour(COLORREF colour);
    COLORREF GetBackColour(void) const;
    virtual void SetTextColour(COLORREF colour);
    COLORREF GetTextColour(void)const;

    void SetShowHideCallback(ShowHideCallback fn, void *userArg);

    void SetCollapsed(int collapsed=TRUE);
    int GetCollapsed(void) const;

    virtual void ReLayout(void) {}

    virtual void SyncFromObj(void) {};
    virtual int GetIDD(void) const { return m_idd; };

protected:
    /* Message handlers. */
    DECLARE_MESSAGE_MAP()
    virtual void OnCancel(void);
    void OnShowWindow(BOOL bShow, UINT nStatus);
    void OnPaint(void);
    HBRUSH OnCtlColor(CDC *pDC, CWnd *pWnd, UINT nCtlColor);
    void OnSize(UINT nType, int cx, int cy);
    void OnActivate(UINT nState, CWnd *pWndOther, BOOL bMinimized);
    void OnNcCalcSize(BOOL validate, NCCALCSIZE_PARAMS *lpncsp);
    LRESULT OnNcHitTest(CPoint point);
    void OnNcMouseMove(UINT nFlags, CPoint point);
    void OnNcLButtonDown(UINT nFlags, CPoint point);
    void OnNcLButtonUp(UINT nFlags, CPoint point);
    void OnNcRButtonUp(UINT nFlags, CPoint point);
    void OnNcMouseLeave(void);

    int GetTitleBarHeight(void) const;

private:
    enum Button
    {
	BTN_CLOSE = 0,
	BTN_MAX = 1,
	BTN_MIN = 2,
	BTN_COLLAPSE = 3
    };

    UINT m_idd;
    int m_enabled;
    int m_collapseBtnEnabled;
    int m_isMouseOver[4];
    int m_isMouseDownOver[4];
    COLORREF m_titleBackColour;
    COLORREF m_titleTextColour;
    COLORREF m_backColour;
    COLORREF m_textColour;
    SPxAutoPtr<CBrush> m_backBrush;
    ShowHideCallback m_showHideFn;
    void *m_showHideUserArg;
    CRect m_sizeWhenCollapsed;

    void showSystemMenu(CPoint point);
    void handleMouseMove(CPoint point);
    int getIconSize(void) const;
    CRect getIconRect(int iconSize) const;
    CRect getButtonRect(Button btn) const;
    int isButtonEnabled(Button btn) const;
    int isEnabled(void) const;
    int isCollapsed(void) const;
    int isWindows8OrNewer(void) const;
};

/*********************************************************************
*
*	End of file
*
**********************************************************************/

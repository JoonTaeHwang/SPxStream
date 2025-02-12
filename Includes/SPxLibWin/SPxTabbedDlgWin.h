/*********************************************************************
*
* (c) Copyright 2014 - 2022, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for a generic tabbed dialog for displaying
*	multiple copies of the same dialog (for different objects)
*	in tabs or in a row (with optional scrollbars).
*
*
* Revision Control:
*   21/06/22 v1.26   AGC	Add SetBorder().
*
* Previous Changes:
*   21/10/21 1.25   AGC	Add SetTabBorder().
*   26/08/20 1.24   AGC	Support construction with no tabs.
*			Support retrieving tab index in AddTab().
*   03/04/20 1.23   AGC Move SPxTabbedItemDlg to own header.
*                       Derive from SPxWinCustomDlg.
*   26/11/19 1.22   AGC Respond to sync messages.
*   03/09/19 1.21   AGC	Support dynamic tab addition/removal.
*   13/02/19 1.20   AGC	Resize on DPI change if required.
*   25/01/18 1.19   AGC	Avoid possible infinite recursion in resize.
*   23/01/18 1.18   AGC	Support toggling tab objects on same dialog.
*   28/11/17 1.17   AGC	Respond to DPI changes.
*   12/04/17 1.16   SP 	Add SetAutoResize().
*   21/02/17 1.15   AGC	Add show tabs only option.
*   31/01/17 1.14   AGC	Support single base name.
*   03/03/16 1.13   AGC	Fix clang warnings.
*   07/01/16 1.12   AGC	Add ShowTab().
*   01/12/15 1.11   AGC	Add SyncParent() to SPxTabbedItemDlg.
*   26/11/15 1.10   AGC	Support layout change message.
*   10/07/15 1.9    AGC	Add SelectTab() function.
*   02/07/15 1.8    AGC	Simplify requirements for SPxTabbedItemName interface.
*   23/06/15 1.7    AGC	Configurable base for tab indices.
*   22/04/15 1.6    SP 	Support single tab display.
*   22/10/14 1.5    AGC	Close child dialogs when closed.
*   03/06/14 1.4    AGC	Add required SPxError header.
*   02/06/14 1.3    AGC	Support update of tab titles.
*   17/02/14 1.2    AGC	Update layout when style changes.
*   27/01/14 1.1    AGC	Convert SPxTabbedPopupDlgWin into child dialog.
**********************************************************************/

#pragma once

/* Standard headers. */

/* Include the SPx resource header */
#include "SPxResource.h"

#define SPX_AFXWIN
#include "SPxLibUtils/SPxCommon.h"

#include "SPxLibUtils/SPxAutoPtr.h"

#include "SPxLibUtils/SPxError.h"

#include "SPxLibWin/SPxWinCustomDlg.h"

#include "SPxLibWin/SPxTabbedItemDlg.h"

#include "SPxLibWin/SPxWinButton.h"

#define SPX_TABBED_LAYOUT_CHANGE (WM_USER + 1)

class SPxTabbedItemName
{
public:
    typedef void (*NameChangedHandler)(void *userArg);

    explicit SPxTabbedItemName(const char *name=NULL);
    virtual ~SPxTabbedItemName(void);

    virtual SPxErrorCode GetTabName(char *buffer, int bufLen) const;
    virtual SPxErrorCode AddNameChangedHandler(NameChangedHandler fn, void *userArg);
    virtual SPxErrorCode RemoveNameChangedHandler(NameChangedHandler fn, void *userArg);

    SPxErrorCode SignalNameChange(void);

private:
    struct impl;
    SPxAutoPtr<impl> m_p;
};

class SPxTabbedDlgTabCtrl : public CTabCtrl
{
public:
    DECLARE_DYNAMIC(SPxTabbedDlgTabCtrl)

    SPxTabbedDlgTabCtrl(void);
    virtual ~SPxTabbedDlgTabCtrl(void) {}

    void EnableCustomColours(int enable) { m_enableCustomColours = enable; }
    void SetColourNormal(COLORREF colour) { m_colourNormal = colour; }
    void SetColourHot(COLORREF colour) { m_colourHot = colour; }
    void SetColourHighlight(COLORREF colour) { m_colourHighlight = colour; }
    void SetColourBorder(COLORREF colour) { m_colourBorder = colour; }
    void SetColourText(COLORREF colour) { m_colourText = colour; }

protected:
    void OnPaint(void);
    BOOL OnEraseBkgnd(CDC *pDC);
    DECLARE_MESSAGE_MAP()

private:
    int m_enableCustomColours;
    COLORREF m_colourNormal;
    COLORREF m_colourHot;
    COLORREF m_colourHighlight;
    COLORREF m_colourBorder;
    COLORREF m_colourText;
};

/*
 * SPxTabbedDlgWin dialog
 */
class SPxTabbedDlgWin : public SPxWinCustomDlg
{
    DECLARE_DYNAMIC(SPxTabbedDlgWin)

public:
    /* Constructor and destructor. */
    explicit SPxTabbedDlgWin(CWnd* pParent,
	void (*syncFn)(void *)=NULL, void *syncArg=NULL);
    SPxTabbedDlgWin(CWnd* pParent,
	SPxTabbedItemDlg *children[],
	unsigned int numChildren,
	void (*syncFn)(void *)=NULL, void *syncArg=NULL);
    SPxTabbedDlgWin(CWnd* pParent,
	SPxTabbedItemDlg *tabDialog,
	unsigned int numTabs,
	void(*syncFn)(void *)=NULL, void *syncArg=NULL);
    virtual ~SPxTabbedDlgWin(void);
    virtual BOOL Create(void);
    void SyncFromObj(void);

    void SelectTab(int tab);

    void SetTabIndexBase(unsigned int tabIndexBase);
    void SetTabTitle(const char *tabTitle);
    void SetTabTitleObjects(SPxTabbedItemName *objects[]);
    void SetBorder(unsigned int border);
    void SetTabBorder(unsigned int border);
    void SetShowSingleTab(int show);
    void SetShowTabsOnly(int show);
    void SetAutoResize(int enabled, unsigned int minWidth=0, unsigned int minHeight=0);
    void ShowTab(int tab, int show);

    unsigned int GetNumTabs(void) const;
    SPxErrorCode AddTab(SPxTabbedItemDlg *dlg,
			int *tabIndexUsed=NULL);
    SPxErrorCode RemoveAllTabs(void);

    void EnableCustomColours(int enable);
    void SetColourNormal(COLORREF colour);
    void SetColourDisabled(COLORREF colour);
    void SetColourPressed(COLORREF colour);
    void SetColourHot(COLORREF colour);
    void SetColourBorder(COLORREF colour);
    void SetColourText(COLORREF colour);

    virtual void ReLayout(void);

    // Dialog Data
    enum { IDD = IDD_SPX_TABBED_DIALOG };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog(void);
    void OnOK(void){};
    void OnCancel(void);
    void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar);
    void OnStyleChanged(int nStyleType, LPSTYLESTRUCT lpStyleStruct);
    void OnTimer(UINT_PTR nIDEvent);
    void OnSize(UINT nType, int cx, int cy);
    BOOL OnEraseBkgnd(CDC *pDC);
    void OnDpiChanged(WPARAM, LPARAM);
    void OnDpiChangedAfterParent(void);
    void Reinit(int resize=TRUE);
    DECLARE_MESSAGE_MAP()

private:
    struct TabbedItemDesc;
    struct TabbedItem;

    /* Private variables. */
    struct impl;
    SPxAutoPtr<impl> m_p;
    SPxWinButton m_radioTabs;
    SPxWinButton m_radioSide;
    SPxTabbedDlgTabCtrl m_tabCtrl;

    /* Handler functions. */
    void OnLayoutChange(void);
    void OnTabChanged(NMHDR *pNotifyStruct, LRESULT *result);
    void OnBnClickedCheck(UINT nID);
    LRESULT OnLayoutChangeMessage(WPARAM, LPARAM);
    void OnSync(NMHDR*, LRESULT*);
    void initTab(TabbedItemDesc& item, int tabIndex);
    void updateTabTitles(void);
    static void tabNameChanged(void *userArg);
    void adjustWindowRect(RECT *rect);

    using CDialog::Create;

}; /* class SPxTabbedDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/

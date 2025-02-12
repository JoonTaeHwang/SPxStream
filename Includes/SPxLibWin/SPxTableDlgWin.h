/*********************************************************************
*
* (c) Copyright 2015 - 2019, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for a re-usable dialog that lays out child dialogs in
*	a table.
*
*
* Revision Control:
*   26/11/19 v1.12   AGC        Add Add() overload with resize type.
*
* Previous Changes:
*   21/11/18 1.11   AGC	Fix per-monitor DPI max height.
*   08/10/18 1.10   SP 	Support scrolling using mouse wheel.
*   21/08/18 1.9    BTB	Support getting count of shown collapsible child dialogs.
*   02/02/18 1.8    AGC	Support border configuration.
*   23/01/18 1.7    AGC	Support removing items.
*   31/01/17 1.6    AGC	Create() must be virtual.
*   22/02/16 1.5    AGC	Support a maximum height.
*   10/12/15 1.4    AGC	Add tabbed item dialog override for Add().
*   26/11/15 1.3    AGC	Support adding items by ID.
*			Support showing some or all collapsible child dialogs.
*   16/11/15 1.2    AGC	Support headers.
*   09/11/15 1.1    AGC	Initial version.
**********************************************************************/
#pragma once

#include "SPxResource.h"
#include "SPxLibWin/SPxTabbedDlgWin.h"
#include "SPxLibWin/SPxTableLayoutWin.h"

/* Forward declarations. */
class SPxTableHdrDlgWin;
class SPxTabbedItemDlg;

class SPxTableDlgWin : public SPxTabbedItemDlg
{
    DECLARE_DYNAMIC(SPxTableDlgWin)

public:
    /* Constructor and destructor. */
    explicit SPxTableDlgWin(CWnd* pParent);
    virtual ~SPxTableDlgWin(void);
    virtual BOOL Create(void);
    virtual void SyncFromObj(void);

    void Add(unsigned int row, unsigned int col, CWnd *wnd,
	unsigned int colSpan=1,
	const char *hdrText=NULL, int collapsible=FALSE);
    void Add(unsigned int row, unsigned int col, SPxTabbedItemDlg *wnd,
	unsigned int colSpan=1,
	const char *hdrText=NULL, int collapsible=FALSE);
    void Add(unsigned int row, unsigned int col, int id,
	unsigned int colSpan=1,
        SPxTableLayoutWin *layout=NULL,
        SPxTableLayoutWin::Resize resize=SPxTableLayoutWin::RESIZE_AUTO);
    void Remove(unsigned int row, unsigned int col);

    void Layout(void);
    void SetMaxHeight(int maxHeight, int isDpiScaled=TRUE);
    void SetBorder(unsigned int border);

    void ShowItem(unsigned int row, unsigned int col);
    void HideItem(unsigned int row, unsigned int col);
    void ShowAllItems(void);
    void HideAllItems(void);
    void ShowOtherItems(unsigned int row, unsigned int col);
    void HideOtherItems(unsigned int row, unsigned int col);
    unsigned int GetShownItemCount(void);

    /* Dialog Data */
    enum { IDD = IDD_SPX_TABLE_CTRL_DIALOG };
    virtual int GetIDD(void) const { return IDD; }

protected:
    virtual void DoDataExchange(CDataExchange* pDX); 
    virtual BOOL OnInitDialog(void);
    virtual void OnOK(void) {}
    virtual void OnCancel(void) {}
    void OnSize(UINT nType, int cx, int cy);
    void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint point);
    DECLARE_MESSAGE_MAP()

private:
    /* Private variables. */
    struct impl;
    SPxAutoPtr<impl> m_p;

    /* Private functions */
    void createHeader(SPxTableHdrDlgWin *hdr, const char *text);

    using SPxTabbedItemDlg::Create;

}; 

/*********************************************************************
*
* End of file
*
**********************************************************************/

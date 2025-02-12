/*********************************************************************
*
* (c) Copyright 2013 - 2018, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for layout of MFC controls in a table.
*
*
* Revision Control:
*   21/11/18 v1.8    AGC	Fix per-monitor DPI max height.
*
* Previous Changes:
*   04/06/18 1.7    AGC	Support optional setHeight for Layout().
*   23/01/18 1.6    AGC	Support removing items.
*   22/02/16 1.5    AGC	Support a maximum height.
*   10/12/15 1.4    AGC	Add tabbed item dialog override for Add().
*			Add SyncFromObj().
*   16/11/15 1.3    AGC	Support control of resizing child items.
*   21/10/13 1.2    AGC	Support setting individual border widths.
*			Support multiple items in each cell.
*			Support modifying column span.
*   15/10/13 1.1    AGC	Initial version.
**********************************************************************/
#pragma once

/* Standard headers. */
#define SPX_AFXWIN
#include "SPxLibUtils/SPxCommon.h"

/* For SPxAutoPtr. */
#include "SPxLibUtils/SPxAutoPtr.h"

/* Forward declarations. */
class CDialog;
class CWnd;
class SPxTabbedItemDlg;

/*
 * SPxTableLayoutWin class
 */
class SPxTableLayoutWin
{
public:
    enum Resize
    {
	RESIZE_AUTO = 0,
	RESIZE_ALWAYS = 1,
	RESIZE_NEVER = 2
    };

    SPxTableLayoutWin(void);
    ~SPxTableLayoutWin(void);

    /* Configure the table. */
    void SetDialog(CDialog *dlg);
    void SetControl(CWnd *wnd);
    void SetSize(unsigned int rows, unsigned int cols);
    void SetRowSpacing(unsigned int rowSpacing);
    void SetColSpacing(unsigned int colSpacing);
    void SetBorder(unsigned int border);
    void SetLeftBorder(unsigned int leftBorder);
    void SetTopBorder(unsigned int topBorder);
    void SetRightBorder(unsigned int rightBorder);
    void SetBottomBorder(unsigned int bottomBorder);
    void SetMaxHeight(int maxHeight, int isDpiScaled=TRUE);

    /* Add new items to the table. */
    void Add(unsigned int row, unsigned int col, int itemID,
	unsigned int colSpan=1,
	SPxTableLayoutWin *layout=NULL,
	Resize resize=RESIZE_AUTO);
    void Add(unsigned int row, unsigned int col, CWnd *wnd,
	unsigned int colSpan=1,
	SPxTableLayoutWin *layout=NULL,
	Resize resize=RESIZE_AUTO);
    void Add(unsigned int row, unsigned int col, SPxTabbedItemDlg *wnd,
	unsigned int colSpan=1,
	SPxTableLayoutWin *layout=NULL,
	Resize resize=RESIZE_AUTO);
    void Remove(unsigned int row, unsigned int col);
    void SetColSpan(unsigned int row, unsigned int col, unsigned int colSpan);

    void SyncFromObj(void);

    int Layout(int setWidth=TRUE, int setHeight=TRUE);
    int GetHeight(void) const;
    int GetMaxHeight(void) const;
    int GetMinWidth(void) const;

private:
    /* Private variables. */
    struct Item;
    struct Cell;
    struct Row;
    struct impl;
    SPxAutoPtr<impl> m_p;

    /* Private functions. */
    CWnd *getWnd(const Item &item);
    int measureWnd(const CWnd *wnd);

}; /* SPxTableLayoutWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/

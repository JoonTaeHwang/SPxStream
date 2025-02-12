/*********************************************************************
 *
 * (c) Copyright 2013, 2017, 2018, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   A class derived from CMenu so that we can create a customised
 *   menu and change its appearance.
 *
 * Revision Control:
 *  22/06/18 v1.8    AGC	Fix cppcheck warnings.
 *
 * Previous Changes:
 *  11/01/17 1.7    AGC	Support sub-menus.
 *  06/12/13 1.6    AGC	Remove unused replaceChars function.
 *  22/11/13 1.5    AGC	Support prefixes.
 *			Expant tabs.
 *  22/10/13 1.4    SP 	Support user-defined font and padding.
 *  21/10/13 1.3    AGC	Support pop-up menus.
 *  17/09/13 1.2    SP 	Add SetDisabledTextColour().
 *  01/08/13 1.1    SP 	Initial version.
 *
 **********************************************************************/

#pragma once

#define SPX_AFXWIN
#include "SPxLibUtils/SPxCommon.h"
#include "SPxLibUtils/SPxAutoPtr.h"

/* SPxWinMenu class */
class SPxWinMenu : public CMenu
{
public:

    /*
     * Public variables.
     */

    /*
     * Public functions.
     */

    /* Construction and destruction. */
    explicit SPxWinMenu(CMenu *menu=NULL, CWnd *parent=NULL);
    virtual ~SPxWinMenu(void);
    BOOL Attach(HMENU hMenu, int isPopupMenu=FALSE);
    HMENU Detach(void);
    BOOL TrackPopupMenu(UINT nFlags, int x, int y, CWnd *pWnd, LPCRECT lpRect=0);

    /* Configuration. */
    void SetBackgroundColour(COLORREF bgr);
    void SetHighlightColour(COLORREF bgr);
    void SetTextColour(COLORREF bgr);
    void SetDisabledTextColour(COLORREF bgr);
    BOOL HandleParentDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDIS);
    BOOL HandleParentMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMIS);
    void SetFont(CFont *font);
    void SetItemPadding(unsigned int x, unsigned int y);

protected:

    /*
     * Protected variables.
     */

    /*
     * Protected functions.
     */

    virtual void DrawItem(LPDRAWITEMSTRUCT lpDIS);
    virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMIS);

private:

    /*
     * Private types.
     */

    /*
     * Private variables.
     */

    struct impl;
    SPxAutoPtr<impl> m_p;
    COLORREF m_backgroundBGR;   /* Background colour. */
    COLORREF m_highlightBGR;    /* Highlight colour. */
    COLORREF m_textBGR;         /* Text colour when enabled. */
    COLORREF m_disabledTextBGR; /* Text colour when disabled. */
    CBrush *m_backgroundBrush;  /* Background brush. */
    CWnd *m_parent;		/* Parent window. */
    CFont *m_font;              /* Installed font. */
    unsigned int m_itemXPad;    /* Item X padding. */
    unsigned int m_itemYPad;    /* Item Y padding. */
    int m_isPopupMenu;		/* Pop-up menu? */

    /*
     * Private functions.
     */

    void UpdateSubMenus(void);
    BOOL Draw(LPDRAWITEMSTRUCT lpDIS);
    BOOL Measure(LPMEASUREITEMSTRUCT lpMIS);

    /*
     * Private static functions.
     */

}; /* Class SPxWinMenu. */

/*********************************************************************
 *
 * End of file
 *
 **********************************************************************/

/*********************************************************************
*
* (c) Copyright 2010 - 2022, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for a button that supports bitmaps on XP, Vista, 7, 8, 10.
*
*	This class required GDI+ as of 02/09/16. The application must
*	initialise GDI+ before using this class.
*
* Revision Control:
*   02/09/20 v1.23   AGC	Suppress CSPxWinButton cppcheck unitialised member warnings.
*
* Previous Changes:
*   02/09/20 1.22   AGC	Workaround CSPxWinButton cppcheck unitialised member warnings.
*   27/05/20 1.21   BTB	Support different text colour when disabled.
*   04/03/20 1.20   AGC	Support bitmap colour replacement.
*   27/09/19 1.19   AGC	Support scale to fit button bitmap option.
*   04/06/18 1.18   AGC	Improve text drawing.
*   23/01/18 1.17   AGC	Re-use CButton image list.
*   28/11/17 1.16   AGC	Respond to DPI changes.
*   21/09/16 1.15   AGC	Support custom checkbox display.
*   02/09/16 1.14   AGC	Support scaling of bitmaps for high DPI.
*   08/07/16 1.13   AGC	Support setting the border size around a colour button.
*   26/11/15 1.12   AGC	Support not showing focus rectangle.
*   13/02/15 1.11   AGC	Support custom colours.
*   21/11/14 1.10   AGC	More efficient SetIcon() when no change.
*			Improvements to SetBitmap() and colour rendering.
*   22/10/14 1.9    AGC	Add overload if SetBitmap() taking resource ID.
*   14/10/14 1.8    AGC	Add overload of SetIcon() taking resource ID.
*   10/04/14 1.7    AGC	Add IsSettingFocus() function.
*   07/10/13 1.6    AGC	Rename to SPxWinButton.
*			Add SetColour() function.
*   30/07/13 1.5    AGC	Support icons.
*   13/09/11 1.4    AGC	Add support for button up and button down messages.
*   22/12/10 1.3    AGC	Add SetBlackTransparent().
*   17/12/10 1.2    AGC	Rename file to SPxWinButton.h.
*			Support bitmaps on XP.
*   13/12/10 1.1    AGC	Initial Version.
**********************************************************************/

#pragma once

#include <uxtheme.h>

#include "SPxLibUtils/SPxAutoPtr.h"

/* Button down and button up messages. */
#define SPX_WIN_BN_DOWN (WM_USER + 0)
#define SPX_WIN_BN_UP   (WM_USER + 1)

/* Macros for receiving button down and up messages in a message map. */
#define ON_SPX_WIN_BN_DOWN(id, memberFxn) \
	ON_CONTROL(SPX_WIN_BN_DOWN, id, memberFxn)
#define ON_SPX_WIN_BN_UP(id, memberFxn) \
	ON_CONTROL(SPX_WIN_BN_UP, id, memberFxn)

/*
 * SPxWinButton control
 */
class SPxWinButton : public CButton
{
    DECLARE_DYNAMIC(SPxWinButton)

public:
    /* Construction and desctruction. */
    SPxWinButton(void);
    virtual ~SPxWinButton(void);

    /* Overridden functions. */
    void SetCheck(int nCheck);
    HBITMAP SetBitmap(HBITMAP hBitmap,
		      int widthPixels = -1, int heightPixels = -1,
		      int scaleFromBtn=FALSE);
    HBITMAP SetBitmap(int id,
		      int widthPixels = -1, int heightPixels = -1,
		      int scaleFromBtn = FALSE);
    HICON SetIcon(HICON icon);
    HICON SetIcon(int id, int widthPixels = 32, int heightPixels = 32);
    void SetColour(COLORREF colour, int show = TRUE);
    COLORREF GetColour(void) const;
    void SetColourBorderWidth(int colourBorderWidth);
    int IsSettingFocus(void) const { return m_inSetFocus; }

    void SetBlackTransparent(int blackTransparent) { m_blackTransparent = blackTransparent; }
    void SetShowFocusRect(int showFocusRect) { m_showFocusRect = showFocusRect; }

    void EnableCustomColours(int enable);
    void SetColourNormal(COLORREF colour);
    void SetColourDisabled(COLORREF colour);
    void SetColourPressed(COLORREF colour);
    void SetColourHot(COLORREF colour);
    void SetColourBorder(COLORREF colour);
    void SetColourText(COLORREF colour);
    void SetColourTextDisabled(COLORREF colour);
    COLORREF GetColourNormal(void) const { return m_colourNormal; }
    COLORREF GetColourDisabled(void) const { return m_colourDisabled; }
    COLORREF GetColourPressed(void) const { return m_colourPressed; }
    COLORREF GetColourHot(void) const { return m_colourHot; }
    COLORREF GetColourBorder(void) const { return m_colourBorder; }
    COLORREF GetColourText(void) const { return m_colourText; }
    COLORREF GetColourTextDisabled(void) const { return m_colourTextDisabled; }

    void ReplaceBitmapColour(COLORREF oldColour, COLORREF newColour);

protected:
    /* Protected functions. */
    virtual void OnSetFocus(CWnd *pOldWnd);
    void OnCustomDraw(NMHDR *pNMHDR, LRESULT *pResult);
    virtual void OnLButtonDown(UINT nFlags, CPoint point);
    virtual void OnLButtonUp(UINT nFlags, CPoint point);
    virtual void OnLButtonDblClk(UINT nFlags, CPoint point);
    virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
    virtual void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
    LRESULT MouseLeave(WPARAM wParam, LPARAM lParam);
    void OnDpiChangedAfterParent(void);

    DECLARE_MESSAGE_MAP()

private:
    /* Private variables. */
    struct impl;
    SPxAutoPtr<impl> m_p;
    int m_inSetFocus;		/* Are we in the set focus handler? */
    HMODULE m_uxtheme;		/* Handle for UxTheme.DLL. */
    double m_dpi;		/* DPI last used when setting icon. */
    int m_iconID;
    int m_origWidth;
    int m_origHeight;
    CBitmap m_origBitmap;
    int m_bmWidth;
    int m_bmHeight;
    int m_blackTransparent;
    int m_showFocusRect;
    COLORREF m_colour;
    int m_showColour;
    HBRUSH m_colourBrush;
    int m_colourBorderWidth;
    int m_useCustomColours;
    int m_textDisabledColourSet;
    COLORREF m_colourNormal;
    COLORREF m_colourDisabled;
    COLORREF m_colourPressed;
    COLORREF m_colourHot;
    COLORREF m_colourBorder;
    COLORREF m_colourText;
    COLORREF m_colourTextDisabled;
    HBRUSH m_brushNormal;
    HBRUSH m_brushDisabled;
    HBRUSH m_brushPressed;
    HBRUSH m_brushHot;
    HBRUSH m_brushBorder;

    /* Private functions. */
    typedef BOOL (WINAPI *IsAppThemedFn)(void);
    typedef BOOL (WINAPI *IsThemeActiveFn)(void);
    typedef HTHEME (WINAPI *OpenThemeDataFn)(HWND, LPCWSTR);
    typedef HRESULT (WINAPI *GetThemeBackgroundContentRectFn)(HTHEME, HDC, int, int, LPCRECT, LPCRECT);
    typedef HRESULT (WINAPI *GetThemePartSizeFn)(HTHEME, HDC, int, int, LPCRECT, THEMESIZE, SIZE*);
    typedef HRESULT (WINAPI *GetThemeTextExtentFn)(HTHEME, HDC, int, int, LPCWSTR, int, DWORD, LPCRECT, LPRECT);
    typedef HRESULT (WINAPI *DrawThemeParentBackgroundFn)(HWND, HDC, LPCRECT);
    typedef HRESULT (WINAPI *DrawThemeBackgroundFn)(HTHEME, HDC, int, int, LPCRECT, LPCRECT);
    typedef HRESULT (WINAPI *DrawThemeTextFn)(HTHEME, HDC, int, int, LPCWSTR, int, DWORD, DWORD, LPCRECT);
    typedef HRESULT (WINAPI *CloseThemeDataFn)(HTHEME);
    IsAppThemedFn IsAppThemed;
    IsThemeActiveFn IsThemeActive;
    OpenThemeDataFn OpenThemeData;
    GetThemeBackgroundContentRectFn GetThemeBackgroundContentRect;
    GetThemePartSizeFn GetThemePartSize;
    GetThemeTextExtentFn GetThemeTextExtent;
    DrawThemeParentBackgroundFn DrawThemeParentBackground;
    DrawThemeBackgroundFn DrawThemeBackground;
    DrawThemeTextFn DrawThemeText;
    CloseThemeDataFn CloseThemeData;

    void drawText(HTHEME theme, int part, int state,
		  HDC hdc, const RECT *rect,
		  const RECT *backRect, RECT *textRect,
		  RECT *focusRect);

}; /* class SPxWinButton */

/* Old class name for backwards compatibility. */
class CSPxWinButton : public SPxWinButton
{
public:
    /* Construction and desctruction. */
    // cppcheck-suppress uninitDerivedMemberVar
    CSPxWinButton(void) { };
    virtual ~CSPxWinButton(void) {};

}; /* class CSPxWinButton */

/*********************************************************************
*
* End of file
*
**********************************************************************/

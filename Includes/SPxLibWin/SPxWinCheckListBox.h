/*********************************************************************
 *
 * (c) Copyright 2020, 2021, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   Wrapper around a CCheckListBox to fix per-monitor high DPI support.
 *
 * Revision Control:
 *  09/03/21 v1.4    AGC	Fix build issues.
 *
 * Previous Changes:
 *  08/03/21 1.3    AGC	Fix display of disabled items.
 *  24/09/20 1.2    AGC	Dynamically load theme functions.
 *  23/09/20 1.1    AGC	Initial version.
 *
 *********************************************************************/
#pragma once

#define SPX_AFXWIN
#include "SPxLibUtils/SPxCommon.h"

class SPxWinCheckListBox : public CCheckListBox
{
    DECLARE_DYNAMIC(SPxWinCheckListBox)

public:
    SPxWinCheckListBox(void);
    virtual ~SPxWinCheckListBox(void);

    virtual void PreDrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
    bool PreDrawItemThemed(CDC *pDC, DRAWITEMSTRUCT &drawItem, int nCheck, int cyItem);
    void PreDrawItemNonThemed(CDC *pDC, DRAWITEMSTRUCT &drawItem, int nCheck, int cyItem);
    virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
    virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);

protected:
    DECLARE_MESSAGE_MAP()
    LRESULT OnLBSetItemHeight(WPARAM wParam, LPARAM lParam);
    virtual BOOL OnChildNotify(UINT, WPARAM, LPARAM, LRESULT*);
    void OnDpiChangedAfterParent(void);

private:
    typedef BOOL(WINAPI *IsAppThemedFn)(void);
    typedef HTHEME(WINAPI *OpenThemeDataFn)(HWND, LPCWSTR);
    typedef HRESULT(WINAPI *GetThemePartSizeFn)(HTHEME, HDC, int, int, LPCRECT, THEMESIZE, SIZE*);
    typedef HRESULT(WINAPI *DrawThemeBackgroundFn)(HTHEME, HDC, int, int, LPCRECT, LPCRECT);
    typedef HRESULT(WINAPI *CloseThemeDataFn)(HTHEME);
    IsAppThemedFn IsAppThemed;
    OpenThemeDataFn OpenThemeData;
    GetThemePartSizeFn GetThemePartSize;
    DrawThemeBackgroundFn DrawThemeBackground;
    CloseThemeDataFn CloseThemeData;

    HMODULE m_uxtheme;		/* Handle for UxTheme.DLL. */
    HBITMAP m_hbitmapCheck;
    DWORD m_ccMajor;

    int CalcItemHeight();
    CSize GetCheckBoxSize(void);

};

/*********************************************************************
 *
 * End of file
 *
 **********************************************************************/

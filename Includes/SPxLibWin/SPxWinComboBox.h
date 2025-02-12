/*********************************************************************
*
* (c) Copyright 2013, 2015, 2019, 2020, Cambridge Pixel Ltd.
*
* Purpose:
*	A class derived from CComboBox so that we can create a customised
*	combo-box and change its appearance.
*
*	This class required GDI+ as of 02/09/16. The application must
*	initialise GDI+ before using this class.
*
* Revision Control:
*   04/03/20 v1.11   AGC	Support bitmap colour replacement.
*
* Previous Changes:
*   27/09/19 1.10   AGC	Support scale to fit button bitmap option.
*   26/09/19 1.9    AGC	Remove unused OnMeasureItem() prototype.
*   02/09/16 1.8    AGC	Support scaling of bitmaps for high DPI.
*   23/06/15 1.7    AGC	Support a disabled colour.
*   15/05/15 1.6    AGC	Support use as split button.
*   02/12/13 1.5    SP 	Add SetClickSoundEnabled().
*   27/11/13 1.4    AGC	Remove unused m_created variable.
*   07/10/13 1.3    AGC	Support use with or without dialog editor.
*   23/08/13 1.2    SP	Move code from PreSubclassWindow() to 
*			OnCreate() to avoid debug assertion.
*   14/08/13 1.1    AGC	Initial version.
*
**********************************************************************/

#pragma once
#define SPX_AFXWIN
#include "SPxLibUtils/SPxCommon.h"

#include "SPxLibUtils/SPxAutoPtr.h"

#include "SPxLibWin/SPxBitmapWin.h"

/* Forward declarations. */
class SPxWinComboBoxList;

/* Define our class. */
class SPxWinComboBox : public CWnd
{
    DECLARE_DYNAMIC(SPxWinComboBox)

public:
    enum Mode
    {
	MODE_COMBO_BOX = 0,
	MODE_SPLIT_BUTTON = 1
    };

    /* Public functions. */

    /* Construction and destruction. */
    SPxWinComboBox(void);
    virtual ~SPxWinComboBox(void);

    /* Item control. */
    int AddString(const char *text, HBITMAP bitmap=NULL,
		  int widthPixels=-1, int heightPixels=-1,
		  int scaleFromBtn=FALSE);
    int InsertString(int nIndex, const char *text, HBITMAP bitmap=NULL,
		     int widthPixels = -1, int heightPixels = -1,
		     int scaleFromBtn = FALSE);
    int DeleteString(UINT nIndex);
    int GetCount(void) const;
    int SetCurSel(int nIndex);
    int GetCurSel(void) const;
    int GetText(int nIndex, CString& rString);
    int SetItemData(int nIndex, DWORD_PTR dwItemData);
    DWORD_PTR GetItemData(int nIndex) const;
    int SetItemDataPtr(int nIndex, void *pData);
    void *GetItemDataPtr(int nIndex) const;
    void SetItemHeight(int itemHeight) { m_itemHeight = itemHeight; Invalidate(); }

    /* Configuration. */
    void SetDisabledColour(COLORREF bgr);
    void SetBackgroundColour(COLORREF bgr);
    void SetHighlightColour(COLORREF bgr);
    void SetTextColour(COLORREF bgr);
    void SetPrelightColour(COLORREF bgr);
    void SetArrowSize(int size);
    void SetMode(Mode mode);

    void SetFont(CFont *pFont);

    /* Split button settings. */
    void SetCheck(int check) { m_checked = check; Invalidate(); }
    int GetCheck(void) const { return m_checked; }

    /* Enable / disable click sound. */
    void SetClickSoundEnabled(int state) { m_isClickSoundEnabled = state; }

    void ReplaceBitmapColour(COLORREF oldColour, COLORREF newColour);

protected:
    /* Protected functions. */
    virtual void PreSubclassWindow(void);
    void OnSize(UINT nType=0, int cx=-1, int cy=-1);
    void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
    BOOL OnEraseBkgnd(CDC *pDC);
    void OnMouseMove(UINT nFlags, CPoint point);
    LRESULT OnMouseLeave(WPARAM, LPARAM);
    void OnLButtonDown(UINT nFlags, CPoint point);
    void OnLButtonUp(UINT nFlags, CPoint point);
    LRESULT OnGetDlgCode(WPARAM wParam, LPARAM lParam);
    void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
    DECLARE_MESSAGE_MAP()

private:
    /* Private variables. */
    struct impl;
    SPxAutoPtr<impl> m_p;
    SPxAutoPtr<SPxBitmapWin> m_bitmap;	/* Bitmap for double buffering. */
    COLORREF m_disabledBGR;	/* Disabled colour. */
    COLORREF m_backgroundBGR;   /* Background colour. */
    COLORREF m_highlightBGR;    /* Highlight colour. */
    COLORREF m_textBGR;         /* Text colour. */
    COLORREF m_prelightBGR;	/* Prelight colour. */
    int m_arrowSize;		/* Arrow size. */
    Mode m_mode;		/* Mode. */
    int m_listBelow;		/* Is the list below or above the combobox. */
    int m_isClickSoundEnabled;	/* Should a sound be generated on click? */
    CRect m_itemRect;		/* Rectangle for selected item. */
    int m_checked;		/* Is the split button checked? */
    int m_trackingMouse;	/* Is the mouse being tracked for leave events. */
    int m_mouseOverButton;	/* Is the mouse over the split button. */
    int m_itemHeight;		/* Custom item height. */

    /* Private functions. */
    CStatic *GetLabel(void);
    const CStatic *GetLabel(void) const;
    SPxWinComboBoxList *GetListBox(void);
    const SPxWinComboBoxList *GetListBox(void) const;
    static LRESULT WINAPI windowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

}; /* SPxWinComboBox. */

/*********************************************************************
 *
 * End of file
 *
 **********************************************************************/

/*********************************************************************
 *
 * (c) Copyright 2019, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   Base dialog, from which scrolling dialog classes can be derived.
 *
 * Revision Control:
 *   13/03/19 v1.1   SP 	Initial version.
 *
 * Previous Changes:
 *
 *********************************************************************/

#pragma once

/* SPx headers. */
#define SPX_AFXWIN
#include "SPxLibUtils/SPxCommon.h"

/* SPxScrollDlgWin dialog */
class SPxScrollDlgWin : public CDialog
{
public:
    /*
     * Public variables.
     */

    /* Dialog Data. */

    /*
     * Public functions.
     */

    /* Construction and destruction. */
    SPxScrollDlgWin(UINT nIDTemplate);
    virtual ~SPxScrollDlgWin(void);

    /* Configuration. */
    void ScrollToYPixels(int yPixels);

protected:
    /* 
     * Protected variables. 
     */

    /* 
     * Protected functions. 
     */

    DECLARE_MESSAGE_MAP()
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog();

    /* Configuration. */
    virtual void SetTotalSize(int width, int height);
    virtual int GetTotalWidth(void) { return m_totalWidth; }
    virtual int GetTotalHeight(void) { return m_totalHeight; }
    virtual BOOL EnableWindow(BOOL bEnable);

    /* GUI callback functions. */
    afx_msg void OnSize(UINT nType, int width, int height);
    afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar); 
    afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar); 
    afx_msg HBRUSH OnCtlColor(CDC *pDC, CWnd *pWnd, UINT nCtlColor);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnSetFocus(CWnd *pOldWnd);
    afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint point);

private:
    /*
     * Private variables.
     */

    /* Colours and brushes. */
    COLORREF m_bgBGR;
    CBrush *m_bgBrush;
    COLORREF m_bgDisabledBGR;
    CBrush *m_bgDisabledBrush;

    /* Horizontal scrolling. */
    int m_clientWidth;      /* Current client width for the window. */
    int m_totalWidth;       /* Total width that can be scrolled. */
    int m_hScrollPos;       /* Current horizontal scroll position. */
    int m_maxHScrollPos;    /* Maximum horizontal scroll position. */
    int m_hScrollLineSize;  /* Horizontal scroll line size. */
    int m_hScrollPageSize;  /* Horizontal scroll page size. */

    /* Vertical scrolling. */
    int m_clientHeight;     /* Current client height for the window. */
    int m_totalHeight;      /* Total height that can be scrolled. */
    int m_vScrollPos;       /* Current vertical scroll position. */
    int m_maxVScrollPos;    /* Maximum verical scroll position. */
    int m_vScrollLineSize;  /* Vertical scroll line size. */
    int m_vScrollPageSize;  /* Vertical scroll page size. */

    /*
     * Private functions.
     */
    void UpdateHScrollBar(void);
    void UpdateVScrollBar(void);
    void HScrollDialog(UINT nSBCode, UINT nPos);
    void VScrollDialog(UINT nSBCode, UINT nPos);
};

/*********************************************************************
 *
 * End of file
 *
 **********************************************************************/

/*********************************************************************
*
* (c) Copyright 2013 - 2020, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for a control to display a scaled image loaded from
*	a file.
*
*	The Clip Children option should be enable for the parent
*	control to prevent flicker on resizing.
*
* Revision Control:
*   27/05/20 v1.9    REW	Add Clear().
*
* Previous Changes:
*   21/05/20 1.8    AGC	Support SPxImage use.
*   04/03/20 1.7    AGC	Support bitmap colour replacement.
*   28/07/16 1.6    AGC	Support clickable images.
*   03/03/16 1.5    AGC	Fix clang warnings.
*   09/06/14 1.4    AGC	Support not maintaining aspect ratio.
*   24/04/14 1.3    AGC	Use SPxObjPtr for improved deletion handling.
*   14/04/14 1.2    AGC	Add bitmap support.
*   25/07/13 1.1    AGC	Initial Version.
**********************************************************************/
#pragma once

/* Includes. */
#include "SPxLibUtils/SPxAutoPtr.h"
#include "SPxLibUtils/SPxObjPtr.h"
#include "SPxLibUtils/SPxError.h"
#include "SPxLibWin/SPxBitmapWin.h"

/* Forward declarations. */
class SPxImage;
namespace ATL
{
    class CImage;
}

/*
 * SPxWinImageCtrl control
 */
class SPxWinImageCtrl : public CWnd
{
    DECLARE_DYNAMIC(SPxWinImageCtrl)

public:
    /* Construction and desctruction. */
    SPxWinImageCtrl(void);
    virtual ~SPxWinImageCtrl(void);

    virtual BOOL Create(CWnd *pParentWnd);
    SPxErrorCode SetFilename(const char *filename);
    SPxErrorCode SetFilename(const CString &filename);
    SPxErrorCode GetFilename(char *buffer, int bufLen) const;
    SPxErrorCode GetFilename(CString &filename) const;
    SPxErrorCode SetImage(SPxImage *image);
    SPxErrorCode Clear(UINT32 argb);
    SPxErrorCode SetBitmap(HBITMAP bitmap);
    SPxErrorCode SetBitmap(int resourceID);
    SPxErrorCode SetBackColour(COLORREF colour);
    UINT32 GetBackColour(void) const { return m_backColour; }
    SPxErrorCode SetBorder(int border);
    int GetBorder(void) const { return m_border; }
    SPxErrorCode SetMaintainAspectRatio(int maintainAspectRatio);
    int IsMaintainingAspectRatio(void) const { return m_maintainAspectRatio; }
    SPxErrorCode SetClickable(int clickable);
    int IsClickable(void) const { return m_clickable; }

    void ReplaceBitmapColour(COLORREF oldColour, COLORREF newColour);

protected:
    /* Protected functions. */
    virtual void PreSubclassWindow(void);
    void OnSize(UINT nType, int cx, int cy);
    void OnLButtonDown(UINT nFlags, CPoint point);
    void OnLButtonUp(UINT nFlags, CPoint point);
    void OnMouseMove(UINT nFlags, CPoint point);
    void OnMouseLeave(void);
    void OnPaint(void);
    BOOL OnEraseBkgnd(CDC *pDC);
    DECLARE_MESSAGE_MAP()

private:
    /* Private variables. */
    struct impl;
    SPxAutoPtr<impl> m_p;
    CString m_filename;
    SPxObjPtr<SPxBitmapWin> m_bitmap;
    SPxAutoPtr<SPxImage> m_image;
    COLORREF m_backColour;
    int m_border;
    int m_maintainAspectRatio;
    int m_clickable;
    int m_isMouseOver;
    int m_isTrackingMouse;
    int m_isLButtonDown;

    /* Private functions. */
    void recreateBitmap(int width, int height);
    void replaceColours(void);

    using CWnd::Create;

}; /* SPxWinImageCtrl */

/*********************************************************************
*
* End of file
*
**********************************************************************/

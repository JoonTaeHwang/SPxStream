/*********************************************************************
*
* (c) Copyright 2009 - 2018, Cambridge Pixel Ltd.
*
* Purpose:
*	Header file for a Button Box for GUI control.
*
*
* Revision Control:
*   28/09/18 v1.21   AGC	Support state-specific button images.
*
* Previous Changes:
*   03/05/18 1.20   AGC	Support right-click events.
*   13/09/17 1.19   AGC	Support showing image and text on button.
*   11/09/17 1.18   AGC	Support improved button image scaling.
*   22/06/17 1.17   AGC	Support insensitive pen config.
*   23/01/17 1.16   AGC	Improve pending change highlight.
*   16/11/16 1.15   SP 	Add SetImage().
*   11/03/16 1.14   SP 	Support scaling of button icons.
*   03/03/16 1.13   AGC	Fix clang warnings.
*   15/04/14 1.12   AGC	Fix #290: Resolve SetFont() ambiguity.
*   24/02/14 1.11   SP 	Override SetLabelForState().
*   18/11/13 1.10   AGC	Fix clang warning.
*   14/08/13 1.9    SP 	Support button icons.
*                       Support off label.
*   01/07/13 1.8    SP 	Allow pens to set be per-button.
*   24/05/13 1.7    AGC	Fix WINVER undefined warning.
*   14/03/12 1.6    AGC	Override SPxButtonBoxButton colour set functions.
*   13/02/12 1.5    AGC	Re-add AddButton() support.
*   09/12/11 1.4    AGC	Derive from cross-platform base class.
*   12/05/11 1.3    REW	Return this from SetGridRef().
*   15/09/10 1.2    REW	Make destructor virtual.
*   14/09/09 1.1    DGJ	First Version
**********************************************************************/

#ifndef _SPX_BUTTON_BOX_WIN_H
#define _SPX_BUTTON_BOX_WIN_H

/* For CWnd etc. */
#define SPX_AFXWIN
#include "SPxLibUtils/SPxCommon.h"

#include "SPxLibUtils/SPxSharedPtr.h"

#include "SPxLibWin/SPxBitmapWin.h"

/* Header for base class. */
#include "SPxLibUtils/SPxButtonBox.h"

/*********************************************************************
*
*   Constants
*
**********************************************************************/

/* Type of button */
#define SPX_WIN_BB_BUTTON_TYPE_PUSH	(SPX_BB_BUTTON_TYPE_PUSH)
#define SPX_WIN_BB_BUTTON_TYPE_TOGGLE	(SPX_BB_BUTTON_TYPE_TOGGLE)

/* State of button. */
#define SPX_WIN_BB_BUTTON_STATE_INACTIVE 0
#define SPX_WIN_BB_BUTTON_STATE_ACTIVE 1

/*********************************************************************
*
*   Type definitions
*
**********************************************************************/

/* Derive from SPxButtonBox to maintain backwards compatibility. */
class SPxButtonBoxWin : public CWnd, public SPxButtonBox
{
public:
    SPxButtonBoxWin(void);
    virtual ~SPxButtonBoxWin(void);

    /* Create the button box with a specified geometry */
    SPxErrorCode Create(CWnd *parent, int x, int y, int w, int h, 
                        int maxW=0, int maxH=0);

    /* Move and/or resize button box. */
    SPxErrorCode Resize(int x, int y, int w, int h);

    SPxErrorCode Redraw(void);

    /* Resolve ambiguity between SPxButtonBox/CWnd SetFont. */
    SPxErrorCode SetFont(const char *family, int size) { return SPxButtonBox::SetFont(family, size); }
    void SetFont(CFont *pFont, BOOL bRedraw=TRUE) { CWnd::SetFont(pFont, bRedraw); }

    /* Smooth image scaling requires applications to initialise GDI+. */
    SPxErrorCode SetUseSmoothImageScaling(int enable);

protected:
    DECLARE_MESSAGE_MAP()
    afx_msg void OnPaint(void);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnMouseLeave();
    afx_msg void OnTimer(UINT_PTR nIDEvent);

private:
    /* Pens and brushes */
    CBrush *m_backBrush;
    CPen *m_backPen;
    CPen *m_buttonInsensitivePen;
    CBrush *m_buttonOnBrush;
    CBrush *m_buttonPreselectBrush;
    CBrush *m_buttonOffBrush;
    CBrush *m_buttonInsensitiveBrush;
    CPen *m_buttonOffPen;
    CPen *m_buttonOnPen;
    CPen *m_buttonHighlightPen;
    CFont m_buttonFont;

    int m_useSmoothImageScaling;
    int m_trackingMouse;

    SPxBitmapWin m_offScreenBitmap;

    /* Private functions. */
    virtual SPxButtonBoxButton *CreateButton(void);

    void trackMouse(void);

    friend class SPxButtonBoxButtonWin;
    using CWnd::Create;
    using SPxButtonBox::Resize;

}; /* SPxButtonBoxWin */

class SPxButtonBoxButtonWin : public SPxButtonBoxButton
{
public:
    explicit SPxButtonBoxButtonWin(SPxButtonBoxWin *parent);
    virtual ~SPxButtonBoxButtonWin(void);
    virtual SPxErrorCode Draw(void *arg);
    virtual SPxButtonBoxButton *SetColourOn(UINT32 colour, int autoSetOthers=TRUE);
    virtual SPxButtonBoxButton *SetColourOff(UINT32 colour);
    virtual SPxButtonBoxButton *SetColourPreselect(UINT32 colour);
    virtual SPxButtonBoxButton *SetPenColourOn(UINT32 colour, int autoSetOthers=TRUE);
    virtual SPxButtonBoxButton *SetPenColourOff(UINT32 colour);
    virtual SPxButtonBoxButton *SetPenColourPreselect(UINT32 colour);
    virtual SPxButtonBoxButton *SetPenColourHighlight(UINT32 colour);
    virtual SPxButtonBoxButton *SetPenColourInsensitive(UINT32 colour);
    virtual SPxButtonBoxButton *SetPosition(int x, int y, int w, int h);
    virtual SPxButtonBoxButton *SetVisible(int state);
    virtual SPxButtonBoxButton *SetImage(SPxImage *image, int scaleToFit=FALSE);
    virtual SPxButtonBoxButton *SetImageFromResource(int resource, int scaleToFit=FALSE);
    virtual SPxButtonBoxButton *SetImageForState(int state, SPxImage *image);
    virtual SPxButtonBoxButton *SetImageFromResourceForState(int state, int resource);
    virtual SPxButtonBoxButton *SetLabelForState(int state, const char *str);
    virtual SPxButtonBoxButton *SetShowImageAndText(int enable);

private:
    SPxButtonBoxWin *m_winParent;
    CBrush *m_preselectBrush;
    CBrush *m_onBrush;
    CBrush *m_offBrush;
    CPen *m_preselectPen;
    CPen *m_highlightPen;
    CPen *m_onPen;
    CPen *m_offPen;
    CPen *m_insensitivePen;
    SPxSharedPtr<SPxImage> m_image[SPX_BB_BUTTON_MAX_NUM_STATES];
    SPxSharedPtr<SPxImage> m_scaledImage[SPX_BB_BUTTON_MAX_NUM_STATES];
    int m_scaleImageToFit;
    int m_showImageAndText;

    friend class SPxButtonBoxWin;

}; /* SPxButtonBoxButtonWin */

#endif /* _SPX_BUTTON_BOX_WIN_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/

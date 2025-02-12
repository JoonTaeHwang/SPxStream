/*********************************************************************
 *
 * (c) Copyright 2014, 2016, Cambridge Pixel Ltd.
 *
 * File: $RCSfile: SPxSeparatorWin.h,v $
 * ID: $Id: SPxSeparatorWin.h,v 1.5 2016/05/18 14:15:49 rew Exp $
 *
 * Purpose:
 *   A simple reusable control dialog separator / banner.
 *
 * Revision Control:
 *  18/05/16 v1.6    SP 	Fix DPI scaling issues.
 *
 * Previous Changes:
 *  03/03/16 1.4    AGC	Fix clang warnings.
 *  17/02/14 1.3    AGC Forward declare Gdiplus classes.
 *  22/01/14 1.2    SP  Fix build issues.
 *  21/01/14 1.1    SP  Initial version.
 *
 **********************************************************************/

#pragma once
#include "afxcmn.h"

/* Standard Windows and GDI+ headers */
#define SPX_AFXWIN
#include "SPxLibUtils/SPxCommon.h"

/* Library headers. */
#include "SPxResource.h"
#include "SPxLibUtils/SPxObj.h"
#include "SPxLibWin/SPxBitmapWin.h"

/* Forward declare GDI+ classes. */
namespace Gdiplus
{
    class Graphics;
    class FontFamily;
    class Font;
    class Brush;
}

/* SPxSeparatorWin dialog */
class SPxSeparatorWin : public CDialog, public SPxObj
{
    DECLARE_DYNAMIC(SPxSeparatorWin)

public:

    /*
     * Public types.
     */

    /*
     * Public variables.
     */

    /* Dialog Data. */
    enum { IDD = IDD_SPX_AZI_INDICATOR_DIALOG };

    /*
     * Public functions.
     */

    /* Construction and destruction. */
    SPxSeparatorWin(unsigned int maxWidth, unsigned int maxHeight);
    virtual ~SPxSeparatorWin(void);
    virtual BOOL Create(CWnd* pParent);

    /* Configuration. */
    void SetText(const char *text);
    void SetXBorderPixels(unsigned int pixels) {  m_winXBorder = pixels; }
    void SetYBorderPixels(unsigned int pixels) {  m_winYBorder = pixels; }

    /* Configuration. */
    void SetBackgroundARGB(UINT32 argb) { m_backgroundARGB = argb; }
    void SetLineARGB(UINT32 argb) { m_lineARGB = argb; }
    void SetTextARGB(UINT32 argb) { m_textARGB = argb; }

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
    virtual void OnOK(void){ /* Don't call the CDialog handler */ };
    virtual void OnCancel(void){ /* Don't call the CDialog handler */ };
    afx_msg void OnPaint();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnSize(UINT nType, int cx, int cy);

private:

    /*
     * Private variables.
     */
    
    SPxBitmapWin *m_pDIB;   /* Off-screen DIB. */
    CBitmap *m_pBitmap;     /* Off-screen bitmap. */
    CDC *m_pBitmapDC;       /* Bitmap drawing context. */
    Gdiplus::Graphics *m_pGfx; /* Graphics rendering object. */
    CRect m_winRect;        /* Window size. */
    Gdiplus::FontFamily *m_fontFamily;  /* Font family. */
    Gdiplus::Font *m_font;      /* Font to use. */
    const char *m_text;         /* Text to display. */
    unsigned int m_winXBorder;  /* Window X border size. */
    unsigned int m_winYBorder;  /* Window Y border size. */
    int m_maxWidth;             /* Max width. */
    int m_maxHeight;            /* Max height. */

    /* Colours. */
    UINT32 m_backgroundARGB;   /* Background colour. */
    UINT32 m_lineARGB;         /* Line colour. */
    UINT32 m_textARGB;         /* Text colour. */

    /*
     * Private functions.
     */

    void RequestRedraw(void);
    void Redraw(void);
    void DrawSeparator(void);
    void DrawText(const char *pText, Gdiplus::Font *pFont, Gdiplus::Brush *pBrush, int x, int y);

    using CDialog::Create;

    /*
     * Private static functions.
     */

}; /* Class SPxSeparatorWin. */

/*********************************************************************
 *
 * End of file
 *
 **********************************************************************/

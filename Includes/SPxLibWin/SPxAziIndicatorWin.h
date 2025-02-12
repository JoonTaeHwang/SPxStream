/*********************************************************************
 *
 * (c) Copyright 2013 - 2018, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   A reusable azimuth / direction indicator.
 *
 * Revision Control:
 *   19/11/18 v1.4    AGC	Further high DPI improvements.
 *
 * Previous Changes:
 *   03/03/16 1.3    AGC    Fix clang warnings.
 *   17/02/14 1.2    AGC    Forward declare Gdiplus classes.
 *   14/08/13 1.1    SP     Initial version.
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

/* SPxAziIndicatorWin dialog */
class SPxAziIndicatorWin : public CDialog, public SPxObj
{
    DECLARE_DYNAMIC(SPxAziIndicatorWin)

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
    SPxAziIndicatorWin(void);
    virtual ~SPxAziIndicatorWin(void);
    virtual BOOL Create(CWnd* pParent);

    /* Configuration. */
    void SetSector(double startDegs, double endDegs);
    void SetNorthDegs(double degs);
    void NorthMarkerEnabled(int state);

    /* Configuration. */
    void SetBackgroundFillARGB(UINT32 argb) { m_backgroundFillARGB = argb; }
    void SetControlFillARGB(UINT32 argb) { m_controlFillARGB = argb; }
    void SetControlOutlineARGB(UINT32 argb) { m_controlOutlineARGB = argb; }
    void SetSectorARGB(UINT32 argb) { m_sectorARGB = argb; }
    void SetNorthARGB(UINT32 argb) { m_northARGB = argb; }
    void SetLabelARGB(UINT32 argb) { m_labelARGB = argb; }
    void SetXBorderPixels(int pixels) {  m_winXBorder = pixels; }
    void SetYBorderPixels(int pixels) {  m_winYBorder = pixels; }

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
    afx_msg void OnDpiChangedAfterParent(void);

private:

    /*
     * Private variables.
     */
    
    SPxBitmapWin *m_pDIB;   /* Off-screen DIB. */
    CBitmap *m_pBitmap;     /* Off-screen bitmap. */
    CDC *m_pBitmapDC;       /* Bitmap drawing context. */
    Gdiplus::Graphics *m_pGfx; /* Graphics rendering object. */
    CRect m_winRect;        /* Window size. */
    int m_winXBorder;       /* Window X border size. */
    int m_winYBorder;       /* Window Y border size. */
    Gdiplus::FontFamily *m_fontFamily;  /* Font family. */
    Gdiplus::Font *m_font;  /* Font to use. */
    double m_startAziDegs;  /* Start azimuth. */
    double m_endAziDegs;    /* End azimuth. */
    int m_showNorth;        /* Show north marker. */
    double m_northDegs;     /* North position. */

    /* Colours. */
    UINT32 m_backgroundFillARGB;   /* Dialog background brush colour. */
    UINT32 m_controlFillARGB;      /* Control background colour. */
    UINT32 m_controlOutlineARGB;   /* Control outline colour. */
    UINT32 m_sectorARGB;           /* Sector colour. */
    UINT32 m_northARGB;            /* North marker colour. */
    UINT32 m_labelARGB;            /* Label colour. */

    /*
     * Private functions.
     */

    void ResetControls(void);
    void RequestRedraw(void);
    void Redraw(void);
    void DrawIndicator(void);
    void DrawText(CString *pText, Gdiplus::Font *pFont, Gdiplus::Brush *pBrush, int x, int y);

    using CDialog::Create;

    /*
     * Private static functions.
     */

}; /* Class SPxAziIndicatorWin. */

/*********************************************************************
 *
 * End of file
 *
 **********************************************************************/

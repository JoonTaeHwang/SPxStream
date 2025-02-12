/*********************************************************************
 *
 * (c) Copyright 2015, 2016, Cambridge Pixel Ltd.
 *
 * File: $RCSfile: SPxLevelIndicatorWin.h,v $
 * ID: $Id: SPxLevelIndicatorWin.h,v 1.3 2016/03/03 10:59:08 rew Exp $
 *
 * Purpose:
 *   A reusable level indicator.
 *
 * Revision Control:
 *   03/03/16 v1.3    AGC	Fix clang warnings.
 *
 * Previous Changes:
 *   18/03/15 1.2    SP     Remove unused border size variables.
 *   06/03/15 1.1    SP     Initial version.
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

/* SPxLevelIndicatorWin dialog */
class SPxLevelIndicatorWin : public CDialog, public SPxObj
{
    DECLARE_DYNAMIC(SPxLevelIndicatorWin)

public:

    /*
     * Public types.
     */

    /*
     * Public variables.
     */

    /* Dialog Data. */
    enum { IDD = IDD_SPX_LEVEL_INDICATOR_DIALOG };

    /*
     * Public functions.
     */

    /* Construction and destruction. */
    SPxLevelIndicatorWin(void);
    virtual ~SPxLevelIndicatorWin(void);
    virtual BOOL Create(CWnd* pParent);

    /* Configuration. */
    SPxErrorCode SetBackgroundFillARGB(UINT32 argb);
    SPxErrorCode SetOffARGB(UINT32 argb);
    SPxErrorCode SetLowARGB(UINT32 argb);
    SPxErrorCode SetMediumARGB(UINT32 argb);
    SPxErrorCode SetHighARGB(UINT32 argb);
    SPxErrorCode SetRange(double min, double max);
    SPxErrorCode SetLevel(double level);
    SPxErrorCode SetUnits(const char *units);
    SPxErrorCode SetBandsPercent(double medium, double high);
    
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
    
    SPxBitmapWin *m_pDIB;       /* Off-screen DIB. */
    CBitmap *m_pBitmap;         /* Off-screen bitmap. */
    CDC *m_pBitmapDC;           /* Bitmap drawing context. */
    Gdiplus::Graphics *m_pGfx;  /* Graphics rendering object. */
    CRect m_winRect;            /* Window size. */
    Gdiplus::FontFamily *m_fontFamily;  /* Font family. */
    Gdiplus::Font *m_font;      /* Font to use. */
    double m_minLevel;          /* Minimum level. */
    double m_maxLevel;          /* Maximum level. */
    double m_level;             /* Current level. */
    char m_units[32];           /* Units text. */
    double m_medBand;           /* Start of medium band as a fraction of full scale. */
    double m_highBand;          /* Start of high band as a fraction of full scale. */

    /* Colours. */
    UINT32 m_backgroundFillARGB;/* Dialog background brush colour. */
    UINT32 m_offARGB;           /* Off colour. */
    UINT32 m_lowARGB;           /* Low level colour. */
    UINT32 m_medARGB;           /* Medium level colour. */
    UINT32 m_highARGB;          /* High level colour. */

    /*
     * Private functions.
     */

    void Redraw(void);
    void DrawIndicator(void);
    void DrawText(CString *pText, Gdiplus::Font *pFont, 
                  Gdiplus::Brush *pBrush, int x, int y);

    using CDialog::Create;

    /*
     * Private static functions.
     */

}; /* Class SPxLevelIndicatorWin. */

/*********************************************************************
 *
 * End of file
 *
 **********************************************************************/

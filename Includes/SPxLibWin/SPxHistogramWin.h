/*********************************************************************
 *
 * (c) Copyright 2014 - 2018, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   A reusable histogram control.
 *
 * Revision Control:
 *  07/11/18 v1.9    AGC	High DPI improvements.
 *
 * Previous Changes:
 *  03/03/16 1.8    AGC	Fix clang warnings.
 *  01/10/15 1.7    AGC	Move std::vector usage into source file.
 *  29/04/15 1.6    JP 	Add screen size fields.
 *  27/03/15 1.5    JP 	Add on mouse leave event.
 *  07/11/14 1.4    JP 	Add more fields etc.
 *  22/10/14 1.3    JP 	Implemented line following mouse and 
 *			visible sample percentage caption.
 *  15/10/14 1.2    JP 	Fix compiler warnings.
 *  14/10/14 1.1    JP 	Initial version.
 **********************************************************************/
#ifndef _SPXHISTOGRAMWIN_H_
#define	_SPXHISTOGRAMWIN_H_

/* Standard Windows and GDI+ headers */
#define SPX_AFXWIN
#include "SPxLibUtils/SPxCommon.h"

/* Library headers. */
#include "SPxLibUtils/SPxHistogram.h"

/* Forward declarations */
namespace Gdiplus
{
    class Graphics;
    class FontFamily;
    class Font;
};

class SPxRunProcess;
class SPxBitmapWin;

/* SPxHistogramWin dialog */
class SPxHistogramWin : public CDialog, public SPxHistogram
{
    DECLARE_DYNAMIC(SPxHistogramWin)

public:

    /*
     * Public variables.
     */

    /* Dialog Data. */
    enum { IDD = IDD_SPX_HISTOGRAM_DIALOG };

    /*
     * Public functions.
     */
    /* Construction and destruction. */
    explicit SPxHistogramWin(SPxRunProcess* proStats = NULL);
    virtual ~SPxHistogramWin(void);

    // Dialog creation.
    virtual BOOL Create(CWnd* pParent);

private:

    /*
     * Private variables.
     */
    SPxBitmapWin *m_pDIB;   /* Off-screen DIB. */
    CBitmap *m_pBitmap;     /* Off-screen bitmap. */
    CDC *m_pBitmapDC;       /* Bitmap drawing context. */
    Gdiplus::Graphics *m_pGfx; /* Graphics rendering object. */
    CRect m_winRect;        /* Window size. */
    CRect m_graphRect;        /* Graphic size. */
    Gdiplus::FontFamily *m_fontFamily;  /* Font family. */
    Gdiplus::Font *m_font;  /* Font to use. */
    LONG m_xCursorPoint;
    LONG m_yCursorPoint;

    /* Screen size. */
    unsigned int m_wRtn;
    unsigned int m_hRtn;

    /* Mouse over flag. */
    BOOL m_mouseOver;

    /*
     * Private functions.
     */

    // Draw member functions.
    virtual void RequestRedraw(void);
    void Redraw(void);
    virtual void DrawBackground(void) const;
    virtual void DrawBar(const SPxHistogram::Bar& bar) const;
    virtual void DrawGrid(void) const;
    virtual void DrawAxisCaption(double x, int index) const;
    virtual void DrawMean(double x0, double y0, double x1, double y1) const;
    virtual void DrawSd(double x0, double y0, double x1, double y1) const;
    virtual void DrawNormalCurve(double cx, double cy) const;
    virtual void DrawSamplePercentageCaption(const char *caption) const;
    virtual void DrawCursorLine(void) const;
    virtual void DrawCursorCaption(const char *captionPos,
				   const char *captionVal) const;

    virtual double GetX(void) const;
    virtual double GetY(void) const;
    virtual double GetWidth(void) const;
    virtual double GetHeight(void) const;
    virtual double GetCursorX(void) const;
    virtual double GetCursorY(void) const;

    // Dialog events member functions
    DECLARE_MESSAGE_MAP()
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog();
    virtual void OnOK(void){ /* Don't call the CDialog handler */ };
    virtual void OnCancel(void){ /* Don't call the CDialog handler */ };
    afx_msg void OnPaint();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    LRESULT OnMouseLeave(WPARAM wp, LPARAM lp);
    void OnDpiChangedAfterParent(void);

    using CDialog::Create;

}; /* Class SPxHistogramWin. */

#endif

/*********************************************************************
 *
 * End of file
 *
 **********************************************************************/

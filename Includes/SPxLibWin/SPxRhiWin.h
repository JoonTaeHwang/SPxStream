/*********************************************************************
 *
 * (c) Copyright 2020, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   Range/altitude display child dialog.
 *
 * Revision Control:
 *   13/11/20 v1.5   SP 	Rename class.
 *
 * Previous Changes:
 *   28/10/20 1.4   SP 	Remove unused variable.
 *   27/10/20 1.3   SP 	Support addition of multiple renderers.
 *   23/10/20 1.2   SP 	Further development.
 *   22/10/20 1.1   SP 	Initial version.
 *
 **********************************************************************/

#pragma once
#include "afxcmn.h"

/* Standard Windows and GDI+ headers */
#define SPX_AFXWIN
#include "SPxLibUtils/SPxCommon.h"

/* Library headers. */
#include "SPxResource.h"
#include "SPxLibUtils/SPxRhi.h"

/* Forward declare any classes required. */
class SPxBitmapWin;
class SPxExtRendererWin;
class SPxUniTrackDatabase;
class SPxRhiGraphicsRendererWin;
class SPxMouseControlWin;

/* Forward declare GDI+ classes. */
namespace Gdiplus
{
    class Graphics;
    class FontFamily;
    class Font;
    class Brush;
}

/* SPxRhiWin dialog */
class SPxRhiWin : public CDialog, public SPxRhi
{
    DECLARE_DYNAMIC(SPxRhiWin)

public:

    /*
     * Public types.
     */

    /*
     * Public variables.
     */

    /* Dialog Data. */
    enum { IDD = IDD_SPX_RHI_DIALOG };

    /*
     * Public functions.
     */

    /* Construction and destruction. */
    SPxRhiWin(unsigned int maxWPixels, 
              unsigned int maxHPixels);
    virtual ~SPxRhiWin(void);
    virtual BOOL Create(CWnd* pParent);

    /* Renderers. */
    SPxErrorCode AddRenderer(SPxRendererWin *rdr);

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

    /* GUI callback functions. */
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg void OnPaint(void);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnDpiChangedAfterParent(void);

private:

    /*
     * Private variables.
     */

    SPxBitmapWin *m_bitmap;                 /* Off-screen bitmap. */
    Gdiplus::Graphics *m_gfx;               /* Graphics rendering object. */
    SPxCriticalSection m_bitmapMutex;       /* Mutex for the bitmap. */
    SPxMouseControlWin *m_mouseCtrlWin;     /* Mouse control for renderer. */

    /*
     * Private functions.
     */

    void redrawWindow(void);
    using CDialog::Create;

    /*
     * Private static functions.
     */

}; /* Class SPxRhiWin. */

/*********************************************************************
 *
 * End of file
 *
 **********************************************************************/

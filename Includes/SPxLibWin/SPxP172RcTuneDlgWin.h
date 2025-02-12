/*********************************************************************
*
* (c) Copyright 2012 - 2016, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxP172RcTuneDlgWin.h,v $
* ID: $Id: SPxP172RcTuneDlgWin.h,v 1.5 2016/03/03 10:59:08 rew Exp $
*
* Purpose:
*	A pop-up dialog used to show the tune level
*       of the SBS-700 transceiver.
*
* Revision Control:
*   03/03/16 v1.5    AGC	Fix clang warnings.
*
* Previous Changes:
*   26/01/15 1.4    AGC	Remove unused HICON.
*   26/11/13 1.3    AGC	Remove unused m_parentWin variable.
*   04/10/13 1.2    AGC	Simplify headers.
*   18/10/12 1.1    SP 	Initial version.
*
**********************************************************************/

#pragma once

/* Include the SPx resource header */
#include "SPxResource.h"

/* Include the classes we need. */
#include "SPxLibUtils/SPxCommon.h"

/* Forward declarations. */
class SPxP172RcInterfaceWin;
class SPxBitmapWin;

/*
 * SPxP172RcTuneDlgWin dialog.
 */
class SPxP172RcTuneDlgWin : public CDialog
{
    DECLARE_DYNAMIC(SPxP172RcTuneDlgWin)

public:
    /* Constructor and destructor. */
    SPxP172RcTuneDlgWin(SPxP172RcInterfaceWin *rdrIf);
    virtual ~SPxP172RcTuneDlgWin(void);
    virtual BOOL Create(CWnd* pParent);

    /* Dialog Data */
    enum { IDD = IDD_SPX_P172_RC_TUNE_DIALOG };

protected:
    DECLARE_MESSAGE_MAP()
    virtual void DoDataExchange(CDataExchange* pDX); 
    virtual BOOL OnInitDialog(void);
    virtual afx_msg void OnPaint(void);
    virtual afx_msg BOOL OnEraseBkgnd(CDC *pDC);
    virtual afx_msg void OnTimer(UINT_PTR nIDEvent);

private:
    /* Private functions. */
    void DrawIndicator(void);

    using CDialog::Create;

    /* GUI objects. */
    CWnd *m_childWin;

    /* The radar interface. */
    SPxP172RcInterfaceWin *m_interface;

    /* Offscreen bitmap and graphics renderer. */
    SPxBitmapWin *m_bitmap;
    CDC *m_bitmapDC;

    /* Brushes and pens. */
    CBrush *m_bgBrush;
    CPen *m_bgPen;
    CBrush *m_fgBrush;
    CPen *m_fgPen;

    /* Fonts. */
    CFont *m_sFont;
    CFont *m_lFont; 

}; /* class SPxP172RcTuneDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/

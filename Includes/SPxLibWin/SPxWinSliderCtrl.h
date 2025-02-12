/*********************************************************************
 *
 * (c) Copyright 2014 - 2016, Cambridge Pixel Ltd.
 *
 * File: $RCSfile: SPxWinSliderCtrl.h,v $
 * ID: $Id: SPxWinSliderCtrl.h,v 1.4 2016/05/19 13:19:15 rew Exp $
 *
 * Purpose:
 *   A class derived from CSliderCtrl so that we can create a customised
 *   slider and change its appearance.
 *
 * Revision Control:
 *   19/05/16 v1.4   SP 	Add OnEraseBkgnd().
 *
 * Previous Changes:
 *   17/02/14 1.3   SP  Override SetPos().
 *   21/01/14 1.2   SP  Reduce flickering.
 *   15/01/14 1.1   SP  Initial version.
 *
 **********************************************************************/

#pragma once

#define SPX_AFXWIN
#include "SPxLibUtils/SPxCommon.h"
#include "SPxLibUtils/SPxAutoPtr.h"

/* SPxWinSliderCtrl class */
class SPxWinSliderCtrl : public CSliderCtrl
{
public:

    /*
     * Public variables.
     */

    /*
     * Public functions.
     */

    /* Construction and destruction. */
    SPxWinSliderCtrl(void);
    virtual ~SPxWinSliderCtrl(void);

    /* Configuration. */
    virtual void SetPos(int nPos);

    /* Channel highlighting. */
    virtual void SetChannelHighlightEnabled(int state) { m_isChanHighlightEnabled = state; }
    virtual int GetChannelHightlightEnabled(void) { return m_isChanHighlightEnabled; }

    /* Colours. */
    virtual void SetThumbColour(COLORREF cr);
    virtual void SetThumbBorderColour(COLORREF cr);
    virtual void SetChannelColour(COLORREF cr);
    virtual void SetChannelBorderColour(COLORREF cr);
    virtual void SetChannelHighlightColour(COLORREF cr);
    virtual void SetChannelBorderHighlightColour(COLORREF cr);

protected:

    /*
     * Protected variables.
     */

    /*
     * Protected functions.
     */

    void HScroll(UINT nSBCode, UINT nPos);
    void VScroll(UINT nSBCode, UINT nPos);
       
protected:

    /*
     * Private variables.
     */

    int m_isChanHighlightEnabled;
    COLORREF m_thumbCr;
    COLORREF m_thumbBorderCr;
    COLORREF m_chanCr;
    COLORREF m_chanBorderCr;
    COLORREF m_chanHighlightCr;
    COLORREF m_chanBorderHighlightCr;
    CBrush m_thumbNormBrush;
    CBrush m_thumbFocusBrush;
    CBrush m_chanBrush;
    CBrush m_chanHighlightBrush;

    /*
     * Private functions.
     */

    DECLARE_MESSAGE_MAP()
    afx_msg void OnReflectedCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg BOOL OnEraseBkgnd(CDC *pDC);
};

/*********************************************************************
 *
 * End of file
 *
 **********************************************************************/


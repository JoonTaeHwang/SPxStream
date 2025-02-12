/*********************************************************************
*
* (c) Copyright 2012, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxScDestDisplayMixer.h,v $
* ID: $Id: SPxScDestDisplayMixer.h,v 1.3 2012/05/01 08:55:06 rew Exp $
*
* Purpose:
*   Header for class to render scan converted data on Windows
*   with a customisable blending mode.
*
* Revision Control:
*   30/04/12 v1.3    AGC	Preserve masked bits.
*				Fix chained mixer deletion.
*
* Previous Changes:
*   04/04/12 1.2    AGC	Add alpha intensity blend mode.
*   29/03/12 1.1    AGC	Initial version.
**********************************************************************/

#ifndef _SPX_SC_DEST_DISPLAY_MIXER_H
#define _SPX_SC_DEST_DISPLAY_MIXER_H

/*
 * Other headers required.
 */

/* For SPxAutoPtr. */
#include "SPxLibUtils/SPxAutoPtr.h"

/* For our base class. */
#include "SPxLibSc/SPxScDestBitmap.h"

/* Blend modes - keep in sync with GPU code. */
enum SPxBlendMode
{
    SPX_BLEND_MODE_ALPHA_BLEND	    = 0,
    SPX_BLEND_MODE_ALPHA	    = 1,
    SPX_BLEND_MODE_INTENSITY	    = 2,
    SPX_BLEND_MODE_BITS		    = 3
};

/* Blend operators - keep in sync with GPU code. */
enum SPxBlendOp
{
    SPX_BLEND_OP_CLEAR	    = 0,
    SPX_BLEND_OP_SRC	    = 1,
    SPX_BLEND_OP_DST	    = 2,
    SPX_BLEND_OP_SRC_OVER   = 3,
    SPX_BLEND_OP_DST_OVER   = 4,
    SPX_BLEND_OP_SRC_IN	    = 5,
    SPX_BLEND_OP_DST_IN	    = 6,
    SPX_BLEND_OP_SRC_OUT    = 7,
    SPX_BLEND_OP_DST_OUT    = 8,
    SPX_BLEND_OP_SRC_ATOP   = 9,
    SPX_BLEND_OP_DST_ATOP   = 10,
    SPX_BLEND_OP_XOR	    = 11
};

/* Forward declarations. */
struct SPxScDestDisplayMixerPriv;
struct SPxScDestDisplayMixerPrivInt;

/* Define our class. */
class SPxScDestDisplayMixer : public SPxScDestBitmap
{
public:
    /* Construction and destruction. */
    explicit SPxScDestDisplayMixer(void);
    virtual ~SPxScDestDisplayMixer(void);

    /* Check for support for this class. */
    int IsSupported(void);

    /* Initialisation. */
    SPxErrorCode Create(UINT16 maxScreenW, UINT16 maxScreenH,
			HWND outputWindow = 0);

    /* Information retrieval. */
    HWND GetOutputWindow(void) const;
    HBITMAP GetOutputBitmap(void) const;

    /* Update interval. */
    virtual void SetUpdateInterval(int updateIntervalMS);

    /* Graphics configuration. */
    SPxErrorCode SetGraphicsBitmap(HBITMAP hbitmap, int isDibSectionTopDown);
    SPxErrorCode SetGraphicsBitmap(HBITMAP hbitmap, HDC bitmapDC);
    SPxErrorCode SetMixerBitmap(SPxScDestDisplayMixer *mixer);
    HBITMAP GetGraphicsBitmap(void) const;
    SPxErrorCode UseLatestGraphics(int useLatestGraphics);
    int IsUsingLatestGraphics(void) const;
    void GraphicsChanged(void);

    /* Blending configuration. */
    SPxErrorCode SetBlendMode(SPxBlendMode blendMode);
    SPxBlendMode GetBlendMode(void) const;
    SPxErrorCode SetBlendOperators(SPxBlendOp blendOp1, SPxBlendOp blendOp2);
    SPxBlendOp GetBlendOperator1(void) const;
    SPxBlendOp GetBlendOperator2(void) const;
    SPxErrorCode SetBlendMask(UINT32 blendMask);
    UINT32 GetBlendMask(void) const;
    SPxErrorCode SetBlendIntensity(UINT8 intensity);
    UINT8 GetBlendIntensity(void) const;
    
    /* Generic parameter setting interface. */
    virtual int SetParameter(char *parameterName, char *parameterValue);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);

private:
    /* Private variables. */
    SPxAutoPtr<SPxScDestDisplayMixerPriv> m_p;
    SPxAutoPtr<SPxScDestDisplayMixerPrivInt> m_pi;

    /* Private functions. */
    static int mixerDeletionFn(void *invokingObject, void *userObject, void *arg);

    friend class SPxScDestDisplayMixerTest;

}; /* SPxScDestDisplayMixer */

#endif /* _SPX_SC_DEST_DISPLAY_MIXER_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/

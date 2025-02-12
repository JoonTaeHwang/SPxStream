/*********************************************************************
*
* (c) Copyright 2019, Cambridge Pixel Ltd.
*
* Purpose:
*   Header for SPxENCRendererWpf class, used to render charts
*   provided by the SPxENCManagerWin class for WPF.
*
* Revision Control:
*   17/10/19 v1.1    AGC	Initial Version.
*
* Previous Changes:
* 
**********************************************************************/

#ifndef _SPX_ENC_RENDERER_WPF_H
#define _SPX_ENC_RENDERER_WPF_H

/*
 * Other headers required.
 */

/* We need the base class. */
#include "SPxLibWin/SPxRendererWpf.h"

/* We need to Window renderer. */
#include "SPxLibWin/SPxENCRendererWin.h"

/* We need the ENC manager class. */
#include "SPxLibWin/SPxENCManagerWin.h"

/*********************************************************************
*
*   Type definitions
*
**********************************************************************/

/* Need to forward-declare this class as it's used internally. */
class SPxENCRendererWpf;

/* Need to forward-declare other classes in case headers in wrong order. */
class SPxRenderer;
class SPxRendererWpf;
class SPxBitmapWin;

/*
 * Define our class, derived from the base renderer object.
 */
class SPxENCRendererWpf : public SPxRendererWpf
{
public:
    typedef void(*RenderBitmapWinFn)(SPxBitmapWin *bitmap);

    /* Constructor and destructor. */
    SPxENCRendererWpf(void);
    virtual ~SPxENCRendererWpf(void);

    /* Configuration. */
    SPxErrorCode SetManager(SPxENCManagerWin *manager);
    SPxENCManagerWin *GetManager(void);

    unsigned int GetViewingGroupLevel(SPxENCRendererWin::ViewingGroupID_t id);

    SPxErrorCode SetViewingGroupRangeEnabled(SPxENCRendererWin::ViewingGroupID_t firstID,
					     SPxENCRendererWin::ViewingGroupID_t lastID,
                                             BOOL isEnabled);

    SPxErrorCode SetViewingGroupEnabled(SPxENCRendererWin::ViewingGroupID_t id,
                                        BOOL isEnabled);
    BOOL IsViewingGroupEnabled(SPxENCRendererWin::ViewingGroupID_t id);

    SPxErrorCode SetViewingGroupOverlay(SPxENCRendererWin::ViewingGroupID_t id,
                                        BOOL isOverlay);
    BOOL IsViewingGroupOverlay(SPxENCRendererWin::ViewingGroupID_t id);

    SPxErrorCode GetViewingGroupName(SPxENCRendererWin::ViewingGroupID_t id,
                                     char *bufPtr,
                                     unsigned int bufSizeBytes);

    SPxErrorCode GetViewingGroupDesc(SPxENCRendererWin::ViewingGroupID_t id,
                                     char *bufPtr,
                                     unsigned int bufSizeBytes);

    unsigned int GetTextGroupLevel(SPxENCRendererWin::TextGroupID_t id);

    SPxErrorCode SetTextGroupRangeEnabled(SPxENCRendererWin::TextGroupID_t firstID,
					  SPxENCRendererWin::TextGroupID_t lastID,
                                          BOOL isEnabled);
    
    SPxErrorCode SetTextGroupEnabled(SPxENCRendererWin::TextGroupID_t id,
                                     BOOL isEnabled);
    BOOL IsTextGroupEnabled(SPxENCRendererWin::TextGroupID_t id);
    
    SPxErrorCode SetTextGroupOverlay(SPxENCRendererWin::TextGroupID_t id,
                                     BOOL isOverlay);
    BOOL IsTextGroupOverlay(SPxENCRendererWin::TextGroupID_t id);

    SPxErrorCode GetTextGroupName(SPxENCRendererWin::TextGroupID_t id,
                                  char *bufPtr,
                                  unsigned int bufSizeBytes);
    
    SPxErrorCode GetTextGroupDesc(SPxENCRendererWin::TextGroupID_t id,
                                  char *bufPtr,
                                  unsigned int bufSizeBytes);
      
    SPxErrorCode SetPalette(SPxENCRendererWin::Palette_t pal);
    SPxErrorCode GetPalette(SPxENCRendererWin::Palette_t *pal);

    SPxErrorCode SetSaturation(double val);
    SPxErrorCode GetSaturation(double *valRtn);

    SPxErrorCode SetLuminance(double val);
    SPxErrorCode GetLuminance(double *valRtn);

    SPxErrorCode SetDeclutterLevel(SPxENCRendererWin::DeclutterLevel_t level);
    SPxErrorCode GetDeclutterLevel(SPxENCRendererWin::DeclutterLevel_t *levelRtn);

    SPxErrorCode GetDepthMetresAtPixel(int xPixels, 
                                       int yPixels, 
                                       double *depthRtn);

    /* The following functions override functions in the base class. */
    virtual SPxErrorCode SetViewCentreMetres(double xMetres,
					     double yMetres);

    virtual SPxErrorCode SetViewCentreLatLong(double latDegs,
					      double longDegs);

    virtual SPxErrorCode SetViewSizePixels(unsigned int widthPixels,
					   unsigned int heightPixels);

    virtual SPxErrorCode SetViewPixelsPerMetre(double pixelsPerMetre);

    virtual SPxErrorCode SetRadarPositionMetres(double xMetres,
						double yMetres);

    virtual SPxErrorCode SetRadarPositionLatLong(double latDegs,
						 double longDegs);

    virtual SPxErrorCode SetRotationDegs(double degs);

    virtual SPxErrorCode SetViewControl(SPxViewControl *viewCtrl);

    virtual unsigned int GetViewWidthPixels(void) const { return m_renderer->GetViewWidthPixels(); }
    virtual unsigned int GetViewHeightPixels(void) const { return m_renderer->GetViewHeightPixels(); }

    /*
     * Public static functions.
     */

    static unsigned int GetNumViewingGroups(void);
    static SPxErrorCode GetViewingGroupAtIndex(unsigned int index,
					       SPxENCRendererWin::ViewingGroupID_t *id);

    static unsigned int GetNumTextGroups(void);
    static SPxErrorCode GetTextGroupAtIndex(unsigned int index,
					    SPxENCRendererWin::TextGroupID_t *id);

    void InstallRenderBitmapWinFn(RenderBitmapWinFn renderBitmapWinFn);

protected:

    virtual int SetParameter(char *parameterName, char *parameterValue);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);

private:

    SPxAutoPtr<SPxENCRendererWin> m_renderer;
    SPxAutoPtr<SPxBitmapWin> m_bitmap;
    RenderBitmapWinFn m_renderBitmapWinFn;

    virtual SPxErrorCode renderAll(void);

}; /* SPxENCRendererWin */

#endif /* SPX_ENC_RENDERER_WPF_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/

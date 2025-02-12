/*********************************************************************
 *
 * (c) Copyright 2018, 2019, 2022, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   Header for SPx radar window Direct2D class.
 *
 * Revision Control:
 *  07/10/22 v1.7    AJH	Add RemoveRenderer() functions.
 *
 * Previous Changes:
 *  04/12/19 1.6    AGC	Add configuration of DPI level for auto scaling.
 *  30/07/19 1.5    AGC	Add GetAppHwnd().
 *  02/07/19 1.4    AGC	Initial support for ENC renderer.
 *			Fix layer creation.
 *  01/07/19 1.3    AGC	Add more flexible API.
 *  03/06/19 1.2    AGC	Support UniTrackRenderer.
 *  30/11/18 1.1    AGC	Initial version.
 **********************************************************************/

#ifndef _SPX_RADAR_WINDOW_D2D_H
#define _SPX_RADAR_WINDOW_D2D_H

/* Needed for general SPx types. */
#include "SPxLibUtils/SPxCommon.h"
#include "SPxLibUtils/SPxObj.h"

#include "SPxLibSc/SPxSc.h" /* For SPxScSweepLineType_t enum etc. */

/* Forward declarations. */
struct ID2D1RenderTarget;
class SPxScDestBitmapD2D;
class SPxRendererD2D;
class SPxWorldMapRendererD2D;
class SPxUniTrackRenderer;
class SPxENCRendererWin;

class SPxRadarWindowD2D : public SPxObj
{
public:
    /* Constructor and destructor */
    explicit SPxRadarWindowD2D(void);
    virtual ~SPxRadarWindowD2D(void);

    SPxErrorCode Create(HWND appHWin,
			unsigned int maxWidth,
			unsigned int maxHeight);

    HWND GetAppHwnd(void) const;
    ID2D1RenderTarget *GetTarget(void) const;

    SPxErrorCode SetSPx(SPxSc *sc,
			SPxScDestBitmapD2D *scBitmap,
			int layer=0);

    SPxErrorCode AddSPx(SPxSc *sc,
			SPxScDestBitmapD2D *scBitmap,
			int layer=0);

    SPxErrorCode AddUnderlayRenderer(SPxRendererD2D *renderer)
    {
	return AddRenderer(renderer, -1);
    }
    SPxErrorCode AddUnderlayRenderer(SPxWorldMapRendererD2D *renderer)
    {
	return AddRenderer(renderer, -1);
    }
    SPxErrorCode AddOverlayRenderer(SPxRendererD2D *renderer)
    {
	return AddRenderer(renderer, 1);
    }
    SPxErrorCode AddOverlayRenderer(SPxUniTrackRenderer *renderer)
    {
	return AddRenderer(renderer, 1);
    }
    SPxErrorCode RemoveUnderlayRenderer(SPxRendererD2D *renderer)
    {
	return RemoveRenderer(renderer, -1);
    }
    SPxErrorCode RemoveUnderlayRenderer(SPxWorldMapRendererD2D *renderer)
    {
	return RemoveRenderer(renderer, -1);
    }
    SPxErrorCode RemoveOverlayRenderer(SPxRendererD2D *renderer)
    {
	return RemoveRenderer(renderer, 1);
    }
    SPxErrorCode RemoveOverlayRenderer(SPxUniTrackRenderer *renderer)
    {
	return RemoveRenderer(renderer, 1);
    }
    SPxErrorCode EnableOverlayRenderer(SPxRendererD2D *renderer,
				       int enable)
    {
	return EnableRenderer(renderer, enable);
    }
    SPxErrorCode EnableOverlayRenderer(SPxUniTrackRenderer *renderer,
				       int enable)
    {
	return EnableRenderer(renderer, enable);
    }

    SPxErrorCode AddRenderer(SPxRendererD2D *renderer, int layer);
    SPxErrorCode AddRenderer(SPxWorldMapRendererD2D *renderer, int layer);
    SPxErrorCode AddRenderer(SPxUniTrackRenderer *renderer, int layer);
    SPxErrorCode AddRenderer(SPxENCRendererWin *renderer, int layer);
    SPxErrorCode RemoveRenderer(SPxRendererD2D *renderer, int layer);
    SPxErrorCode RemoveRenderer(SPxWorldMapRendererD2D *renderer, int layer);
    SPxErrorCode RemoveRenderer(SPxUniTrackRenderer *renderer, int layer);
    SPxErrorCode RemoveRenderer(SPxENCRendererWin *renderer, int layer);
    SPxErrorCode EnableRenderer(SPxRendererD2D *renderer, int enable);
    SPxErrorCode EnableRenderer(SPxUniTrackRenderer *renderer, int enable);

    int IsWorldMapAvailable(void) const;

    SPxErrorCode SetUnderlayBrightness(int underlayBrightness)
    {
	return SetLayerBrightness(underlayBrightness, -1);
    }
    int GetUnderlayBrightness(void) const
    {
	return GetLayerBrightness(-1);
    }
    SPxErrorCode SetUnderlayContrast(int underlayContrast)
    {
	return SetLayerContrast(underlayContrast, -1);
    }
    int GetUnderlayContrast(void) const
    {
	return GetLayerContrast(-1);
    }

    SPxErrorCode SetLayerBrightness(int brightness, int layer);
    int GetLayerBrightness(int layer) const;
    SPxErrorCode SetLayerContrast(int contrast, int layer);
    int GetLayerContrast(int layer) const;

    SPxErrorCode SetBackgroundColour(UINT32 colour);

    SPxErrorCode CompositeDisplay(int updateUlay, int updateOlay);

    SPxErrorCode CompositeDisplay(const int *updateLayers, unsigned int numLayers);

    UINT32 GetUnderlayUpdateTime(void) const
    {
	return GetLayerUpdateTime(-1);
    }
    UINT32 GetOverlayUpdateTime(void) const
    {
	return GetLayerUpdateTime(1);
    }

    UINT32 GetLayerUpdateTime(int layer) const;

    SPxErrorCode SetAutoScaleDpi(unsigned int autoScaleDpi);
    unsigned int GetAutoScaleDpi(void) const;

    SPxErrorCode SetViewControl(SPxViewControl *obj);

private:
    struct impl;
    SPxAutoPtr<impl> m_p;

    void getBuffers(void);
    SPxErrorCode createDevice(void);
    void destroyDevice(void);
    void createDeviceResources(void);
    void destroyDeviceResources(void);
    void setScanConverterScale(void);
    SPxErrorCode createLayer(int layer);

};

/*********************************************************************
 *
 *   Function prototypes
 *
 **********************************************************************/

#endif /* _SPX_RADAR_WINDOW_D2D_H */

/*********************************************************************
 *
 * End of file
 *
 **********************************************************************/

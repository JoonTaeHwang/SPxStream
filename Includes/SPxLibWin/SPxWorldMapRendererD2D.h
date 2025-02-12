/*********************************************************************
*
* (c) Copyright 2013 - 2018, Cambridge Pixel Ltd.
*
* Purpose:
*   Header for SPxWorldMapRendererD2D class which supports the display
*   of world map in a Windows-based application.
*
*   NOTE: This class differs from other SPx renderering classes
*         as it is not derived from SPxRenderer. Instead all view
*         control is performed using the associated WorldMap object.
*
* Revision Control:
*   29/08/18 v1.6    AGC	Fix crash when high res geometry cannot be calculated.
*
* Previous Changes:
*   26/09/17 1.5    AGC	Clip polys to visible area.
*   15/03/17 1.4    AGC	Simplify private class.
*   11/12/14 1.3    AGC	SetDestination() no longer requires factory.
*			Remove brightness/contrast adjustment.
*   27/09/13 1.2    AGC	Fix compilation errors.
*   27/09/13 1.1    AGC	Initial Version.
* 
**********************************************************************/

#ifndef SPX_WORLD_MAP_RENDERER_D2D_H
#define SPX_WORLD_MAP_RENDERER_D2D_H

/*
 * Other headers required.
 */

/* We need the base classes. */
#include "SPxLibWin/SPxRendererD2D.h"
#include "SPxLibUtils/SPxWorldMapRenderer.h"

/*********************************************************************
*
*   Constants
*
**********************************************************************/


/*********************************************************************
*
*   Macros
*
**********************************************************************/


/*********************************************************************
*
*   Type definitions
*
**********************************************************************/

/* Forward declarations. */
class SPxWorldMapRenderer;
class SPxWorldMap;
class SPxWorldMapFeature;
struct ID2D1Factory;
struct ID2D1RenderTarget;
struct ID2D1Bitmap;
struct ID2D1BitmapRenderTarget;
struct ID2D1SolidColorBrush;
struct ID2D1PathGeometry;
struct D2D_POINT_2F;
struct D2D_SIZE_F;
struct D2D_RECT_F;

/*
 * Define our class, derived from the base renderer object.
 */
class SPxWorldMapRendererD2D : public SPxWorldMapRenderer
{
public:
    /*
     * Public variables.
     */

    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    SPxWorldMapRendererD2D(SPxWorldMap *worldMap);
    virtual ~SPxWorldMapRendererD2D(void);

    /* Set/get destination to render to */
    virtual SPxErrorCode SetDestination(ID2D1RenderTarget *target);
#ifdef SPX_SUPPORT_D2D
    ID2D1Factory *GetFactory(void) { return m_factory; }
    ID2D1RenderTarget *GetDestination(void) { return m_target; }
#endif

    /* Set/get combined high/low res rendering mode */
    SPxErrorCode SetRenderHighAndLowRes(int enable);
    int GetRenderHighAndLowRes(void) { return m_renderHighAndLowRes; }

    /* Rendering function (overrides base class function). */
    SPxErrorCode Render(void);

private:
    /*
     * Private variables.
     */

    /* Handle of the device context to use */
#ifdef SPX_SUPPORT_D2D
    struct impl;
    SPxAutoPtr<impl> m_p;
    SPxComPtr<ID2D1Factory> m_factory;
    SPxComPtr<ID2D1RenderTarget> m_target;
    SPxComPtr<ID2D1Bitmap> m_bitmap;
    SPxComPtr<ID2D1BitmapRenderTarget> m_bmTarget;
    ID2D1RenderTarget *m_targetToUse;
#endif
    int m_renderHighAndLowRes;

    /*
     * Private functions.
     */

    /*
     * Private static functions.
     */
    SPxErrorCode getHighResGeom(ID2D1PathGeometry **path,
				ID2D1PathGeometry **xorPath);
    static void drawFeature(SPxWorldMapFeature *feature, void *arg);
    ID2D1SolidColorBrush *getBrush(UINT32 colour);

#ifdef SPX_SUPPORT_D2D
    D2D_POINT_2F pxToDips(D2D_POINT_2F point);
    D2D_SIZE_F pxToDips(D2D_SIZE_F size);
    D2D_RECT_F pxToDips(D2D_RECT_F rect);
    int getQuadrant(D2D_POINT_2F point) const;
#endif

}; /* SPxWorldMapRendererD2D */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* SPX_WORLD_MAP_RENDERER_D2D_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/

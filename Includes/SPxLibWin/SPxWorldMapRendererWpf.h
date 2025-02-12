/*********************************************************************
*
* (c) Copyright 2019, Cambridge Pixel Ltd.
*
* Purpose:
*   Header for SPxWorldMapRendererWpf class which supports the display
*   of world map in a WPF-based application.
*
*   NOTE: This class differs from other SPx renderering classes
*         as it is not derived from SPxRenderer. Instead all view
*         control is performed using the associated WorldMap object.
*
* Revision Control:
*   31/10/19 v1.3    AGC	Mark SetActive() as override.
*
* Previous Changes:
*   29/10/19 1.2    AGC	Stop rendering when inactive.
*   22/10/19 1.1    AGC	Initial Version.
* 
**********************************************************************/

#ifndef SPX_WORLD_MAP_RENDERER_WPF_H
#define SPX_WORLD_MAP_RENDERER_WPF_H

/*
 * Other headers required.
 */

/* We need SPxRendererXYD_t. */
#include "SPxLibUtils/SPxRenderer.h"

/* We need the base class. */
#include "SPxLibUtils/SPxWorldMapRenderer.h"

/*********************************************************************
*
*   Type definitions
*
**********************************************************************/

/* Forward declarations. */
class SPxWorldMapRenderer;
class SPxWorldMap;

/*
 * Define our class, derived from the base renderer object.
 */
class SPxWorldMapRendererWpf : public SPxWorldMapRenderer
{
public:
    typedef void(*SetActiveFn)(int);
    typedef SPxErrorCode (*RenderAllFn)();

    /* Constructor and destructor. */
    SPxWorldMapRendererWpf(SPxWorldMap *worldMap);
    virtual ~SPxWorldMapRendererWpf(void);

    virtual void SetActive(int active = TRUE) SPX_VIRTUAL_OVERRIDE;

    /* Set/get combined high/low res rendering mode */
    virtual SPxErrorCode SetRenderHighAndLowRes(int enable) SPX_VIRTUAL_OVERRIDE;
    virtual int GetRenderHighAndLowRes(void) SPX_VIRTUAL_OVERRIDE { return m_renderHighAndLowRes; }

    /* Rendering function (overrides base class function). */
    virtual SPxErrorCode Render(void) SPX_VIRTUAL_OVERRIDE;

    void InstallSetActiveFn(SetActiveFn setActiveFn);
    void InstallRenderAllFn(RenderAllFn renderAllFn);
    SPxRendererXYD_t *GetPoints(unsigned int numPoints);
    int GetQuadrant(SPxRendererXYD_t point) const;
    double GetViewPixelsPerMetre(void) const { return m_viewPixelsPerMetre; }
    double GetViewCXMetres(void) const { return m_viewCXMetres; }
    double GetViewCYMetres(void) const { return m_viewCYMetres; }
    unsigned int GetWinWidthPixels(void) const { return m_winWidthPixels; }
    unsigned int GetWinHeightPixels(void) const { return m_winHeightPixels; }

private:
    SetActiveFn m_setActiveFn;
    RenderAllFn m_renderAllFn;
    int m_renderHighAndLowRes;
    unsigned int m_numPoints;
    SPxAutoPtr<SPxRendererXYD_t[]> m_points;


}; /* SPxWorldMapRendererWpf */

#endif /* SPX_WORLD_MAP_RENDERER_WPF_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/

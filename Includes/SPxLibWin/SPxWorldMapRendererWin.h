/*********************************************************************
*
* (c) Copyright 2012, 2013, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxWorldMapRendererWin.h,v $
* ID: $Id: SPxWorldMapRendererWin.h,v 1.5 2014/05/16 11:28:26 rew Exp $
*
* Purpose:
*   Header for SPxWorldMapRendererWin class which supports the display
*   of world map in a Windows-based application.
*
*   NOTE: This class differs from other SPx renderering classes
*         as it is not derived from SPxRenderer. Instead all view
*         control is performed using the associated WorldMap object.
*
* Revision Control:
*   16/05/14 v1.5    AGC	Cache GDI+ brushes/pens.
*
* Previous Changes:
*   04/10/13 1.4    AGC	Simplify headers.
*   06/06/13 1.3    AGC	Add option to render high and low res maps.
*   19/04/12 1.2    SP 	Override base class Render() function.
*   08/02/12 1.1    SP 	Initial Version.
* 
**********************************************************************/

#ifndef _SPX_WORLD_MAP_RENDERER_WIN_H
#define _SPX_WORLD_MAP_RENDERER_WIN_H

/*
 * Other headers required.
 */

/* We need the base classes. */
#include "SPxLibUtils/SPxWorldMapRenderer.h"
#include "SPxLibWin/SPxBitmapWin.h"

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
class SPxWorldMap;
class SPxWorldMapFeature;

namespace Gdiplus
{
    class Graphics;
    class SolidBrush;
    class Pen;
    class PointF;
};

/*
 * Define our class, derived from the base renderer object.
 */
class SPxWorldMapRendererWin : public SPxWorldMapRenderer
{
public:
    /*
     * Public variables.
     */

    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    SPxWorldMapRendererWin(SPxWorldMap *worldMap);
    virtual ~SPxWorldMapRendererWin(void);

    /* Set/get destination to render to */
    SPxErrorCode SetDestination(HDC hdc);
    HDC GetDestination(void) { return m_hdc; }

    /* Set/get combined high/low res rendering mode */
    SPxErrorCode SetRenderHighAndLowRes(int enable);
    int GetRenderHighAndLowRes(void) { return m_renderHighAndLowRes; }

    /* Rendering function (overrides base class function). */
    SPxErrorCode Render(void);

protected:
    /*
     * Protected functions.
     */

    /* Generic parameter assignment. */
    virtual int SetParameter(char *parameterName, 
                             char *parameterValue);

    virtual int GetParameter(char *parameterName, 
                             char *valueBuf, int bufLen);

private:
    /*
     * Private variables.
     */

    struct impl;
    SPxAutoPtr<impl> m_p;

    /* Handle of the device context to use */
    int m_renderHighAndLowRes;
    HDC m_hdc;
    HDC m_hdcToUse;
    SPxBitmapWin m_bitmap;
    int m_bitmapCreated;
    SPxAutoPtr<Gdiplus::Graphics> m_gfx;
    SPxAutoPtr<Gdiplus::PointF[]> m_gdiPoints;
    unsigned int m_numGdiPoints;

    /*
     * Private functions.
     */

    /*
     * Private static functions.
     */
    SPxErrorCode renderHighRes(void);
    static void drawFeature(SPxWorldMapFeature *feature, void *arg);
    Gdiplus::SolidBrush *getBrush(UINT32 colour);
    Gdiplus::Pen *getPen(UINT32 colour, float width);

}; /* SPxWorldMapRendererWin */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* SPX_WORLD_MAP_RENDERER_WIN_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/

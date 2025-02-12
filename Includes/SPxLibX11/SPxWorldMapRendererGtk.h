/*********************************************************************
*
* (c) Copyright 2012, 2013, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxWorldMapRendererGtk.h,v $
* ID: $Id: SPxWorldMapRendererGtk.h,v 1.3 2013/10/04 15:31:08 rew Exp $
*
* Purpose:
*   Header for SPxWorldMapRendererGtk class which supports the display
*   of world map in a Linux GTK application.
*
*   NOTE: This class differs from other SPx renderering classes
*         as it is not derived from SPxRenderer. Instead all view
*         control is performed using the associated WorldMap object.
*
* Revision Control:
*   04/10/13 v1.3    AGC	Simplify headers.
*
* Previous Changes:
*   30/07/13 1.2    AGC	Support rendering high and low res map.
*   21/06/12 1.1    SP 	Initial version.
* 
**********************************************************************/

#ifndef _SPX_WORLD_MAP_RENDERER_GTK_H
#define _SPX_WORLD_MAP_RENDERER_GTK_H

/*
 * Other headers required.
 */

/* We need the base classes. */
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
class SPxWorldMap;
class SPxWorldMapFeature;
struct _GdkDrawable;
struct _cairo;

/*
 * Define our class, derived from the base renderer object.
 */
class SPxWorldMapRendererGtk : public SPxWorldMapRenderer
{
public:
    /*
     * Public variables.
     */

    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    SPxWorldMapRendererGtk(SPxWorldMap *worldMap);
    virtual ~SPxWorldMapRendererGtk(void);

    /* Set/get destination to render to */
    SPxErrorCode SetDestination(struct _GdkDrawable *drawable);
    struct _GdkDrawable *GetDestination(void) { return m_gdkDrawable; }

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

    /* Fields used during rendering.  Use opaque handles where possible
     * so the header doesn't need to drag in cairo.h etc.
     */
    struct _cairo *m_cr;

    /* Handle of the drawable we're rendering into.  Use the opaque one
     * so that this header doesn't need to drag in gdk.h.
     */
    struct _GdkDrawable *m_gdkDrawable;

    /* Off-screen pixmap for combining high and low res world maps. */
    struct _GdkDrawable *m_gdkPixmap;

    /* Render both high and low res maps when appropriate? */
    int m_renderHighAndLowRes;

    /*
     * Private functions.
     */

    void SetColour(UINT32 argb);

    /*
     * Private static functions.
     */

    static void drawFeature(SPxWorldMapFeature *feature, void *arg);
    void createPixmap(void);

}; /* SPxWorldMapRendererGtk */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* SPX_WORLD_MAP_RENDERER_GTK_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/

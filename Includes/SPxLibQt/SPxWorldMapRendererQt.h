/*********************************************************************
*
* (c) Copyright 2015, Cambridge Pixel Ltd.
*
*
* Purpose:
*   Header for SPxWorldMapRendererQt class which supports the display
*   of world map in a Qt application.
*
*   NOTE: This class differs from other SPx renderering classes
*         as it is not derived from SPxRenderer. Instead all view
*         control is performed using the associated WorldMap object.
*
* Revision Control:
*   12/05/15 v1.3    AGC	Remove unused SetColour() function.
*
* Previous Changes:
*   29/04/15 1.2    JP	Add drawing objects to the class.
*   13/04/15 1.1    JP	Initial version.
* 
**********************************************************************/

#ifndef _SPX_WORLD_MAP_RENDERER_QT_H
#define _SPX_WORLD_MAP_RENDERER_QT_H

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
class QPaintDevice;
class QPointF;

/*
 * Define our class, derived from the base renderer object.
 */
class SPxWorldMapRendererQt : public SPxWorldMapRenderer
{
public:
    /*
     * Public variables.
     */

    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    SPxWorldMapRendererQt(SPxWorldMap *worldMap);
    virtual ~SPxWorldMapRendererQt(void);

    /* Set/get destination to render to */
    SPxErrorCode SetDestination(QPaintDevice *paintDevice);
    QPaintDevice *GetDestination(void) { return m_paintDevice; }

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

    /* Handle of the drawable we're rendering into.  Use the opaque one
     * so that this header doesn't need to drag in gdk.h.
     */
    QPaintDevice *m_paintDevice;
    QPixmap *m_tmpPixmap;

    QPainter *m_devicePainter;
    QPainter *m_tmpPixmapPainter;

    /* Off-screen pixmap for combining high and low res world maps. */

    /* Render both high and low res maps when appropriate? */
    int m_renderHighAndLowRes;

    /*
     * Private functions.
     */

    /*
     * Private static functions.
     */

    static void drawFeature(SPxWorldMapFeature *feature, void *arg);
    void createPixmap(void);

}; /* SPxWorldMapRendererQt */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* SPX_WORLD_MAP_RENDERER_QT_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/

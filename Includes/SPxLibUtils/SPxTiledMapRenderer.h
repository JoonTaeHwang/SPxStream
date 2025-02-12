/*********************************************************************
*
* (c) Copyright 2012 - 2020, Cambridge Pixel Ltd.
*
* Purpose:
*   Header for SPxTiledMapRenderer base class which supports the display
*   of a tiled map.
*
* Revision Control:
*   13/10/20 v1.27   AGC	Support hiding key.
*
* Previous Changes:
*   14/04/20 1.26   AGC	Make RequestRedraw() public.
*   05/07/19 1.25   AGC	Add SetUseTransparency() function.
*   01/07/19 1.24   AGC	Add SetUseTransparency().
*   07/12/18 1.23   AGC	Use floating point positions for key.
*   02/10/18 1.22   SP 	Support configurable tile size.
*   02/10/18 1.21   SP 	No longer derived from SPxTiledMap.
*   03/10/16 1.20   AGC	Add mutex protection.
*   02/09/16 1.19   AGC	Use atomic variable for redraw required flag.
*			Add GetDeviceScale().
*   28/07/16 1.18   AGC	Add virtual brightness/contrast functions.
*			Add Shutdown() function.
*   14/07/16 1.17   AGC	Make progress indicator position configurable.
*   08/07/16 1.16   AGC	SPxTiledMapSrc objects now render keys themselves.
*   15/06/16 1.15   AGC	Add force bitmap update option.
*   02/03/14 1.14   SP 	Fix race condition in redraw request.
*   24/09/14 1.13   SP 	Add additional Get...() functions.
*   02/06/14 1.12   SP 	Add isChanged to CheckBitmaps().
*   07/10/13 1.11   AGC	Move rendering to renderAll().
*   04/10/13 1.10   SP 	Support display rotation.
*                       Add SetExtraBufferTiles().
*   05/09/13 1.9    AGC	Optional display of terrain download indicator.
*   15/05/13 1.8    SP 	Add m_mapBitmapTlTileX, etc.
*   25/01/13 1.7    AGC	Support rendering a key for some tiled map sources.
*   09/11/12 1.6    AGC	Remove unused SPxTiledMapDatabase forward declaration.
*   30/08/12 1.5    SP 	Add GetZoom().
*   21/08/12 1.4    SP 	Derive class from SPxTiledMap and
*                       SPxRenderer.
*   18/07/12 1.3    SP 	Move Render() here from base class.
*                       Add pure virtual RenderMap().
*                       Support progress indicator.
*   13/07/12 1.2    SP 	Add GetTileSubArea().
*   06/06/12 1.1    SP 	Initial Version.
* 
**********************************************************************/

#ifndef _SPX_TILED_MAP_RENDERER_H
#define _SPX_TILED_MAP_RENDERER_H

/*
 * Other headers required.
 */

/* Include common types, callbacks, errors etc. */
#include "SPxLibUtils/SPxAtomic.h"
#include "SPxLibUtils/SPxObj.h"
#include "SPxLibUtils/SPxError.h"
#include "SPxLibUtils/SPxStrings.h"
#include "SPxLibUtils/SPxRenderer.h"
#include "SPxLibUtils/SPxTiledMapDatabase.h"

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
class SPxTerrainDatabase;

/*
 * Define our class.
 */
class SPxTiledMapRenderer : virtual public SPxRenderer
{
public:
    /*
     * Public functions.
     */

    /* Constructor and destructor. */
    SPxTiledMapRenderer(SPxTiledMapDatabase *db, SPxTerrainDatabase *tdb=NULL);
    virtual ~SPxTiledMapRenderer(void);

    /* Rendering functions. */
    void RequestRedraw(void);
    virtual int IsRedrawRequired(void);

    /* Configuration. */
    virtual SPxTiledMapDatabase *GetTiledMapDatabase(void) const;
    virtual SPxTerrainDatabase *GetTerrainDatabase(void) const;
    virtual SPxErrorCode SetShowProgressIndicator(int state);
    virtual int GetShowProgressIndicator(void) const
    { 
        return m_showProgressIndicator; 
    }
    virtual SPxErrorCode SetProgressIndicatorPos(int x, int y);
    virtual SPxErrorCode SetShowKey(int state);
    virtual int GetShowKey(void) const;

    virtual int GetZoom(void) const { return m_zoom; }

    virtual SPxErrorCode SetExtraBufferTiles(unsigned int num);
    virtual unsigned int GetExtraOffscreenTiles(void) const { return m_extraBufferTiles; }

    virtual void ForceBitmapUpdate(void) { m_forceBitmapUpdate = TRUE; }
    
    virtual SPxErrorCode SetBrightness(int /*brightness*/) { return SPX_ERR_NOT_SUPPORTED; }
    virtual int GetBrightness(void) const { return 0; }
    virtual SPxErrorCode SetContrast(int /*contrast*/) { return SPX_ERR_NOT_SUPPORTED; }
    virtual int GetContrast(void) const { return 0; }
    virtual void SetUseTransparency(int /*useTransparency*/) {}

    /*
     * Public types.
     */

    /* Interface class that SPxTiledMapSrc::RenderKey() implementations may use. */
    class Iface
    {
    public:
	SPxErrorCode RenderPoly(UINT32 colour,
				unsigned int numPoints,
				SPxRendererXYD_t *points,
				unsigned int filled);
	SPxErrorCode RenderPolyGradient(UINT32 colour[2],
					unsigned int numPoints,
					SPxRendererXYD_t *points);
	SPxErrorCode RenderText(UINT32 colour,
				SPxRendererXYD_t *point,
				const char *text);
	SPxErrorCode GetTextExtents(const char *text,
				    double *widthRtn,
				    double *heightRtn);

	double MetresToDisplayUnits(double metres, int isHeight = FALSE);
	const char *GetDisplayUnitDistText(int isHeight = FALSE);
	
	unsigned int GetViewWidthPixels(void) const;
	unsigned int GetViewHeightPixels(void) const;
	float GetDeviceScale(void) const;

	SPxErrorCode SetFont(const char *name, unsigned int size);

    private:
	Iface(SPxTiledMapRenderer *renderer) : m_renderer(renderer) {}
	virtual ~Iface() {}

	SPxTiledMapRenderer *m_renderer;

	friend class SPxTiledMapRenderer;
    };

protected:
    /*
     * Protected functions.
     */

    /* This function should be called at the start of derived class destructors. */
    void Shutdown(void);

    /* Generic parameter assignment. */
    virtual int SetParameter(char *parameterName, 
                             char *parameterValue);

    virtual int GetParameter(char *parameterName, 
                             char *valueBuf, int bufLen);

    
    /*
     * Protected variables.
     */

private:
    /*
     * Private variables.
     */

    SPxCriticalSection m_mutex;
    
    /* Current view N, S, E and W edges. */
    double m_viewLatN;
    double m_viewLatS;
    double m_viewLonE;
    double m_viewLonW;

    /* Current tile zoom level (0 to 18). */
    int m_zoom;

    /* Flag set when redraw required. */
    SPxAtomic<int> m_isRedrawRequired;

    /* Tiled map database. */
    SPxTiledMapDatabase *m_db;

    /* Terrain database. */
    SPxTerrainDatabase *m_tdb;

    /* Show progress indicator? */
    int m_showProgressIndicator;

    /* Progress indicator position. */
    int m_progressIndicatorX;
    int m_progressIndicatorY;

    /* Show key? */
    int m_showKey;

    /* Tiles currently in the offscreen bitmap. */
    int m_mapBitmapTlTileX;
    int m_mapBitmapTlTileY;
    int m_mapBitmapNumXTiles;
    int m_mapBitmapNumYTiles;
    int m_mapBitmapZoom;

    /* Current rotation of the offscreen bitmap. */
    double m_mapRotationDegs;

    /* Number of extra rows/columns to buffer offscreen. */
    unsigned int m_extraBufferTiles;

    /* Flag to force an update of the bitmaps. */
    SPxAtomic<int> m_forceBitmapUpdate;

    /*
     * Private functions.
     */

    virtual SPxErrorCode renderAll(void);

    SPxErrorCode RenderMap(int forceRedraw);

    int IsBitmapUpdateRequired(int tlTileX,
                               int tlTileY,
                               int numXTiles,
                               int numYTiles,
                               int zoom);

    void RenderMapTiles(int tlTileX, 
                        int tlTileY,
                        int numXTiles,
                        int numYTiles,
                        int zoom);

    void RenderKey(void);

    int RenderProgressIndicator(void);

    void RenderDownArrow(const SPxRendererXY_t *origin,
			 unsigned int width,
			 unsigned int height,
			 UINT32 colour);

    void RenderBucket(const SPxRendererXY_t *origin,
		      unsigned int width,
		      unsigned int height,
		      UINT32 colour);

    void RenderErrorMesg(int clearToGrey, const char *mesg);

    /* 
     * The following O/S specific functions must be implemented 
     * in a derived class. 
     */

    virtual SPxErrorCode CheckBitmaps(unsigned int mapWidthPixels,
                                      unsigned int mapHeightPixels,
                                      int *isChanged)=0;

    virtual SPxErrorCode ClearMapBitmap(unsigned int mapWidthPixels,
                                        unsigned int mapHeightPixels)=0;

    virtual SPxErrorCode RenderTileToBitmap(const char *filename,
                                            unsigned int tileSizePixels,
                                            int srcXPixels,
                                            int srcYPixels,
                                            int destXPixels,
                                            int destYPixels,
                                            double scale)=0;

    virtual SPxErrorCode RotateMap(unsigned int mapWidthPixels,
                                   unsigned int mapHeightPixels)=0;
    
    virtual SPxErrorCode RenderMapToDestination(unsigned int mapWidthPixels,
                                                unsigned int mapHeightPixels,
                                                int mapViewCentreXPixels,
                                                int mapViewCentreYPixels,
                                                double scale)=0;

    /*
     * Private static functions.
     */

    static int EventHandler(void *invokingObj,
                            void *infoPtr,
                            void *eventTypePtr);

    static int DeletionHandler(void *invokingObj,
                               void *userArg,
                               void *notUsed);


}; /* SPxTiledMapRenderer */

/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* SPX_TILED_MAP_RENDERER_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/

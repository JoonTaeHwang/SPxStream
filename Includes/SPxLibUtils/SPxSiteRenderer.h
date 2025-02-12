/*********************************************************************
 *
 * (c) Copyright 2019, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   Header for SPxSiteRenderer base class which supports the 
 *   display of objects contains in a SPxSiteDatabase.
 *
 * Revision Control:
 *   20/12/19 v1.3   SP 	Move image loading to derived class.
 *
 * Previous Changes:
 *   16/12/19 1.2   SP 	Further development.
 *   11/12/19 1.1   SP 	Initial version.
 *
 **********************************************************************/

#ifndef _SPX_SITE_RENDERER_H
#define _SPX_SITE_RENDERER_H

/*
 * Other headers required.
 */

/* Include common types, callbacks, errors etc. */
#include "SPxLibUtils/SPxObj.h"
#include "SPxLibUtils/SPxError.h"
#include "SPxLibUtils/SPxStrings.h"
#include "SPxLibUtils/SPxTime.h"
#include "SPxLibUtils/SPxRenderer.h"
#include "SPxLibUtils/SPxSiteDatabase.h"
#include "SPxLibUtils/SPxImage.h"

/*********************************************************************
 *
 *   Constants
 *
 *********************************************************************/


/*********************************************************************
 *
 *   Macros
 *
 *********************************************************************/


/*********************************************************************
 *
 *   Type definitions
 *
 *********************************************************************/

/* Forward declare other classes required. */
class SPxSiteDatabase;

/*
 * Define our class, derived from the abstract renderer base class.
 */
class SPxSiteRenderer : virtual public SPxRenderer
{
public:
    /*
     * Public variables.
     */

    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    SPxSiteRenderer(SPxSiteDatabase *database);
    virtual ~SPxSiteRenderer(void);

    /* Configuration functions. */

protected:
    /*
     * Protected types.
     */
    enum State_t
    {
        STATE_OK_DISCOVERED = 0,
        STATE_OK_EXPECTED   = 1,
        STATE_WARNING       = 2,
        STATE_ERROR         = 3,
        NUM_STATES
    };

    /*
     * Protected functions.
     */

    /* Generic parameter assignment. */
    virtual int SetParameter(char *parameterName,
                             char *parameterValue);

    virtual int GetParameter(char *parameterName,
                             char *valueBuf, int bufLen);

    /*
     * Protected variables.
     */
    SPxImage m_siteIcons[NUM_STATES];
    SPxImage m_systemIcons[NUM_STATES];

private:
    /*
     * Private types.
     */

    struct Rect_t
    {
        SPxRendererXYD_t tl;
        SPxRendererXYD_t br;
    };

    struct ItemContext_t
    {
        SPxSdbItem *item;
        SPxImage *image;
        Rect_t imageRect;
        char label[SPxSdbItem::NAME_MAX_LEN];
        Rect_t labelRect;
        Rect_t labelBgRect;
        Rect_t boundingRect;
    };

    enum Direction_t
    {
        UP, 
        DOWN, 
        LEFT, 
        RIGHT
    };

    /*
     * Private variables.
     */

    struct impl;
    SPxAutoPtr<impl> m_p;  /* Private data. */
    SPxSiteDatabase *m_database;
    double m_imageSizePx;

    /*
     * Private functions.
     */

    SPxErrorCode renderAll(void);
    void renderSite(SPxSdbSite *site);
    void renderSiteBoundary(SPxSdbSite *site);
    int getSiteLatLong(SPxSdbSite *site, SPxLatLong_t *llRtn);
    int getItemLatLong(SPxSdbItem *item, SPxLatLong_t *llRtn);
    int canRenderItem(SPxSdbItem *item);
    void preRenderItem(SPxSdbItem *item);
    void preRenderItem(SPxSdbItem *item, 
                       const SPxRendererXYD_t *centrePx,
                       double sizePx);
    void renderItems(void);
    void renderItem(ItemContext_t *ctx);
    void updateBoundingRect(Rect_t *bb, const Rect_t *rect);
    int isOverlap(const Rect_t *rect1, const Rect_t *rect2);
    int isOverlapWithAnyItem(const Rect_t *rect);
    int resolvePosition(const SPxRendererXYD_t *centrePx,
                        double sizePx,
                        SPxRendererXYD_t *centrePxRtn);

    /*
     * Private static functions.
     */

    static int renderSiteItr(void *parentVoid,
                             void *objVoid,
                             void *siteVoid);

    static int preRenderItemItr(void *parentVoid,
                                void *objVoid,
                                void *itemVoid);

}; /* SPxSiteRenderer */


#endif /* SPX_SITE_RENDERER_H */

/*********************************************************************
 *
 * End of file
 *
 *********************************************************************/

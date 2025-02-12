/*********************************************************************
*
* (c) Copyright 2013 - 2015, 2019, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxRasterMapRenderer base class which supports the
*	rendering of raster images.
*
* Revision Control:
*   02/05/19 v1.9    REW	Add Get/SetVisibleMaxViewWidthMetres().
*
* Previous Changes:
*   18/05/15 1.8    AGC	Support rotation with multiple images.
*   28/08/14 1.7    REW	Add Get/SetVisible() and IsImageSet().
*   26/06/14 1.6    AGC	Add SetBrightnessAll/SetContrastAll().
*   20/06/14 1.5    SP/AGC Add Set/GetContrast().
*   			Use shared pointer for automatic clean-up.
*   22/11/13 1.4    AGC	Fix various parameter load/save issues.
*   07/10/13 1.3    AGC	Move rendering to renderAll().
*   04/10/13 1.2    SP 	Support display rotation.
*   01/08/13 1.1    AGC	Initial version.
**********************************************************************/
#ifndef _SPX_RASTER_MAP_RENDERER_H
#define _SPX_RASTER_MAP_RENDERER_H

/*
 * Other headers required.
 */

/* For SPxAutoPtr. */
#include "SPxLibUtils/SPxAutoPtr.h"

/* For SPxErrorCode. */
#include "SPxLibUtils/SPxError.h"

/* For base class. */
#include "SPxLibUtils/SPxRenderer.h"

/*
 * Types
 */

/* Forward declarations. */
struct SPxLatLong_tag;
struct SPxRasterMapImage;
class SPxImage;

/* Define our class */
class SPxRasterMapRenderer : virtual public SPxRenderer
{
public:
    /* Constructor and destructor. */
    explicit SPxRasterMapRenderer(void);
    virtual ~SPxRasterMapRenderer(void);

    int SetImage(const char *filename, int index=-1);
    int IsImageSet(int index) const;
    const char *GetShortImagePath(int index) const;
    const char *GetFullImagePath(int index) const;
    SPxErrorCode SetCentreLatLong(int index, double latDegs, double longDegs);
    SPxErrorCode GetCentreLatLong(int index, double *latDegs, double *longDegs) const;
    SPxErrorCode SetCentreMetres(int index, double xMetres, double yMetres);
    SPxErrorCode GetCentreMetres(int index, double *xMetres, double *yMetres) const;
    SPxErrorCode SetWidthMetres(int index, double widthMetres);
    SPxErrorCode GetWidthMetres(int index, double *widthMetres) const;
    SPxErrorCode SetBrightness(int index, double brightness);
    SPxErrorCode GetBrightness(int index, double *brightness) const;
    SPxErrorCode SetContrast(int index, double contrast);
    SPxErrorCode GetContrast(int index, double *contrast) const;
    SPxErrorCode SetBrightnessAll(double brightness);
    SPxErrorCode SetContrastAll(double contrast);
    SPxErrorCode SetVisible(int index, int visible);
    SPxErrorCode GetVisible(int index, int *visible) const;
    SPxErrorCode SetVisibleMaxViewWidthMetres(int index, double metres);
    SPxErrorCode GetVisibleMaxViewWidthMetres(int index, double *metres) const;

    /* Generic parameter assignment. */
    virtual int SetParameter(char *parameterName, char *parameterValue);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);

protected:
    void FreeCachedBitmaps(void);

private:
    /* Private variables. */
    struct impl;
    SPxAutoPtr<impl> m_p;

    /* Private functions. */
    virtual SPxErrorCode renderAll(void);
    const SPxRasterMapImage *getImage(int index, const char *funcName) const;
    SPxRasterMapImage *getImage(int index, const char *funcName);
    SPxErrorCode renderRasterImage(SPxRasterMapImage *image);
    virtual SPxErrorCode SetConfigFromState(void);
    SPxErrorCode setImageParameter(int index,
				   const char *paramName,
				   const char *paramValue) const;

    /* Functions implemented by platform specific child classes. */
    virtual SPxErrorCode renderBitmap(SPxImage *image,
	int x, int y, double scale, double brightness, double contrast,
	void **cachedImagePtr, double *rotationDegs, int changed)=0;
    virtual void freeCachedBitmap(void *cachedBitmap)=0;

}; /* SPxRasterMapRenderer */

#endif /* _SPX_RASTER_MAP_RENDERER_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/

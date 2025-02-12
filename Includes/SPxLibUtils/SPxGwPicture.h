/*********************************************************************
 *
 * (c) Copyright 2013 - 2022, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   SPx graphics widget picture class.
 *
 * Revision Control:
 *  24/02/22 v1.33   AGC	Move non-X11 parts to new SPxGwPictureBase.
 *
 * Previous Changes:
 *  11/05/21 1.32   AJH	Make GetJSON() const.
 *  04/05/21 1.31   AJH	Support retrieval of objects in GeoJSON format.
 *  26/04/21 1.30   AJH	Simplify remote object iteration.
 *  23/04/21 1.29   AJH	Support object iteration as remote parameter.
 *  11/10/20 1.28   AJH	Support auto-reload of picture on change.
 *  08/10/20 1.27   SP 	Move SPxGwAreaType to SPxAreaObj.h.
 *  07/08/20 1.26   AGC	Improve support without X11.
 *  25/06/18 1.25   AGC	Correctly suppress cppcheck warning.
 *  15/05/18 1.24   BTB	Support forcing display of direction indicators.
 *  30/04/18 1.23   BTB	Allow showing labels without editing.
 *  09/02/18 1.22   AGC	Tidy up DXF layer loading.
 *  23/11/17 1.21   AGC	Handle renderer deletion.
 *  03/11/17 1.20   AGC	Support GetAreaType().
 *  31/10/17 1.19   AGC	Support event callbacks.
 *			Support SPxAreaObj add/remove areas.
 *  25/10/17 1.18   SP 	Add IterateWidgetGroups().
 *  23/10/17 1.17   AGC	Support use with multiple view controls.
 *			Support item name rendering.
 *  11/10/17 1.16   AJH	Add support for DXF lat/long reference.
 *  04/08/17 1.15   AJH	Add support for DXF HATCH and rotation.
 *  01/08/17 1.14   AJH	Support import of DXF file.
 *  16/07/14 1.13   SP 	Avoid compiler warning.
 *  15/07/14 1.12   SP 	Support import of XML file.
 *  10/03/14 1.11   AGC	Add DeleteAllWidgetGroups() function.
 *  04/12/13 1.10   AGC	Add DeleteAreas() function.
 *  22/11/13 1.9    AGC	Request redraw on modification.
 *  04/11/13 1.8    AGC	Move SPxRendererObj to new header.
 *  08/10/13 1.7    AGC	Exclude X11 when not supported.
 *  07/10/13 1.6    AGC	Move to SPxLibUtils.
 *			Make cross-platform.
 *			Add SPxAreaObj support.
 *  04/10/13 1.5    AGC	Simplify headers.
 *  29/01/13 1.4    SP 	Add Flush().
 *  25/01/13 1.3    SP 	Add some new functions.
 *  15/01/13 1.2    SP 	Remove namespace std from header.
 *			Add GetRenderer().
 *			Add static colour allocation functions.
 *  11/01/13 1.1    SP 	Initial version.
 *
 *********************************************************************/

#ifndef _SPX_GW_PICTURE_H
#define _SPX_GW_PICTURE_H

/* For SPX_X11_SUPPORTED. */
#include "SPxLibUtils/SPxCommon.h"

#ifndef _WIN32
#if SPX_X11_SUPPORTED
#include <X11/Xlib.h>
#endif
#endif

/* Other headers required. */
#include "SPxLibUtils/SPxAutoPtr.h"
#include "SPxLibUtils/SPxGwPictureBase.h"
#include "SPxLibUtils/SPxRendererObj.h"

/*********************************************************************
 *
 *  Constants
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

/* Forward declarations. */
#ifndef _WIN32
#if SPX_X11_SUPPORTED
struct _GdkDisplay;
struct _GdkDrawable;
struct _GdkGC;
#endif
#endif

/*********************************************************************
 *
 *   Type definitions
 *
 *********************************************************************/

/*********************************************************************
 *
 *   Class definitions
 *
 *********************************************************************/

/* Define our class. */
class SPxGwPicture : public SPxGwPictureBase, public SPxRendererObj
{
public:

    /*
     * Public variables.
     */

    /*
     * Public functions.
     */

    /* Constructor and destructor. */
#ifdef _WIN32
    SPxGwPicture(void);
#else
    SPxGwPicture(int isDoubleBuffered=FALSE);
#endif
    virtual ~SPxGwPicture(void);

#ifndef _WIN32
    /* Functions for backwards compatiblity with original X11 implementation. */
    virtual int IsDoubleBuffered(void) { return m_isDoubleBuffered; }

    virtual unsigned int GetDestinationDepth(void) {return m_depth;}
    virtual unsigned int GetMaxWidth(void) {return m_maxWidth;}
    virtual unsigned int GetMaxHeight(void) {return m_maxHeight;}

#if SPX_X11_SUPPORTED
    virtual SPxErrorCode SetDestination(Display *x11Display,
                                        Drawable x11Drawable);
#endif

    virtual SPxErrorCode SetClipRect(unsigned int width, unsigned int height);
    virtual unsigned int GetClipRectWidth(void);
    virtual unsigned int GetClipRectHeight(void);

    virtual SPxErrorCode FillClipRect(UINT32 argb);
    virtual SPxErrorCode FillClipRect(UINT8 r, UINT8 g, UINT8 b, UINT8 a=255);
    virtual SPxErrorCode FillClipRect(const char *name);

    virtual SPxErrorCode SetBackgroundColour(UINT32 argb);
    virtual SPxErrorCode SetBackgroundColour(UINT8 r, UINT8 g, 
                                             UINT8 b, UINT8 a=0xFF);
    virtual SPxErrorCode SetBackgroundColour(const char *name);
    virtual UINT32 GetBackgroundColour(void);
    virtual UINT8 GetBackgroundColourA(void);
    virtual UINT8 GetBackgroundColourR(void);
    virtual UINT8 GetBackgroundColourG(void);
    virtual UINT8 GetBackgroundColourB(void);

    virtual SPxErrorCode SetClearBackgroundEnabled(int isEnabled);
    virtual int IsClearBackgroundEnabled(void);

    virtual SPxErrorCode SetAntiAliasEnabled(int isEnabled);
    virtual int IsAntiAliasEnabled(void) { return m_isAntiAliasEnabled; }

    virtual SPxErrorCode SetAlphaReplaceEnabled(int isEnabled);
    virtual int IsAlphaReplaceEnabled(void) { return m_isAlphaReplaceEnabled; }

    virtual SPxErrorCode Render(void);

    virtual SPxErrorCode Show(unsigned int x, unsigned int y,
                              unsigned int width, unsigned int height);
    virtual SPxErrorCode Show(void);
#endif

    /* Rendering. */
    virtual SPxErrorCode Render(SPxExtRenderer *renderer) { return Render(renderer, TRUE, FALSE); }
    virtual SPxErrorCode Render(SPxExtRenderer *renderer,
				int editEnabled) { return Render(renderer, editEnabled, FALSE); }
    virtual SPxErrorCode Render(SPxExtRenderer *renderer,
                                int editEnabled,
				int labelsEnabled);

private:
    /* Private variables. */
    struct impl;
    SPxAutoPtr<impl> m_p;	/* Private structure. */

#ifndef _WIN32
#if SPX_X11_SUPPORTED
    /* Use opaque handles where possible so the header 
     * doesn't need to drag in gdk.h, cairo.h etc. 
     */
    struct _GdkDisplay *m_gdkDisplay;       /* Display. */
    struct _GdkDrawable *m_gdkDest;         /* Destination drawble. */
    struct _GdkGC *m_gdkDestGC;             /* Destination drawable GC. */
    struct _GdkDrawable *m_gdkBuffer;       /* Buffer drawable. */
    struct _GdkGC *m_gdkBufferGC;           /* Buffer drawable GC. */
#endif

    /* Destination (and buffer) attributes. */
    unsigned int m_depth;      /* Colour depth. */
    unsigned int m_maxWidth;   /* Maximum width. */
    unsigned int m_maxHeight;  /* Maximum height. */

    /* Renderer configuration. */
    UINT32 m_argbBackground;
    int m_isClearBackgroundEnabled;
    int m_isDoubleBuffered;
    int m_isAntiAliasEnabled;
    int m_isAlphaReplaceEnabled;
#endif

    static SPxErrorCode renderGroup(SPxGwWidgetGroup *widgetGroup,
                                    void *userArg);

#ifndef _WIN32
#if SPX_X11_SUPPORTED
    static int rendererDeleted(void *invokingObject,
			       void *userObject, void *arg);
#endif
#endif

}; /* SPxGwPicture */

#endif /* _SPX_GW_PICTURE_H */

/*********************************************************************
 *
 *      End of file
 *
 *********************************************************************/


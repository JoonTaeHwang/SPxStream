/*********************************************************************
*
* (c) Copyright 2011 - 2014, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxPPIwindowGlx.h,v $
* ID: $Id: SPxPPIwindowGlx.h,v 1.7 2014/06/20 14:12:08 rew Exp $
*
* Purpose:
*   Header file for SPxPPIwindowGlx class. 
*
*   Note that this class is derived from SPxPPIwindow and overrides 
*   several functions in the base class.
*
* Revision Control:
*   20/06/14 v1.7    AGC	Move C++ headers to source file.
*
* Previous Changes:
*   17/02/14 1.6    AGC	Remove unused member variables.
*			Use const in appropriate places.
*   04/10/13 1.5    AGC	Simplify headers.
*   18/02/11 1.4    AGC	Pass supported framebuffer texture format to SPxGlxBitmapTex.
*   11/02/11 1.3    AGC	All calls must be from same thread.
*			Default arguments for renderPixmap when based 
*			on actual pixmap size.
*			Add PixmapADrawing() and PixmapBDrawing().
*   04/02/11 1.2    AGC	Add optional constructor parameter to select acceleration mode.
*			Override base class Create() functions.
*			Support separate creation/composite thread calls.
*   28/01/11 1.1    AGC	Initial version.
**********************************************************************/

#ifndef _SPX_PPI_WINDOW_GLX_H
#define	_SPX_PPI_WINDOW_GLX_H

/* Standard X headers. */
#include "X11/Xlib.h"

/* pthread headers. */
#include <pthread.h>

/* Standard GL/GLX headers. */
#include "GL/gl.h"
#include "GL/glx.h"
#include "GL/glxext.h"

/* SPx headers. */
#include "SPxLibUtils/SPxAutoPtr.h"
#include "SPxLibX11/SPxPPI.h"
#include "SPxGlxFunctions.h"

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

/* Debugging flags. */
#define SPX_PPI_WIN_GLX_DEBUG_NONE	    (0x00000000)	/* None */
#define SPX_PPI_WIN_GLX_DEBUG_GL_EXT	    (0x00000001)	/* Open GL extensions */
#define SPX_PPI_WIN_GLX_DEBUG_RADAR	    (0x00000002)	/* Radar textures */
#define SPX_PPI_WIN_GLX_DEBUG_SUPPORT	    (0x00000004)	/* GLX support */
#define SPX_PPI_WIN_GLX_DEBUG_ALL	    (0xFFFFFFFF)	/* All */  

/*********************************************************************
*
*   Type definitions
*
**********************************************************************/

/* Forward declare classes referenced so headers do not need to be included. */
class SPxPPIwindow;
class SPxGlxBitmapTex;

/*
 * SPxPPIwindowGL class.
 */
class SPxPPIwindowGlx : public SPxPPIwindow
{
public:

    /* Public functions */
    SPxPPIwindowGlx(SPxPPIwindowAccel_t accelMode=SPX_PPI_WIN_ACCEL_AUTO);
    virtual ~SPxPPIwindowGlx(void);

    virtual int Create(Display *display, int screen, Drawable displayWindow,
                       int maxWidth, int maxHeight, Pixmap underlayPixmap,
                       Pixmap overlayPixmap, int numRadarVideos, int createDrawable=FALSE);

    virtual int Create(Display *display, int screen, 
                       Window appGraphics, 
                       int maxWidth, int maxHeight,
                       int numRadarVideos, 
                       Pixmap pixmapA = 0, Pixmap pixmapB = 0,
		       int createDrawable=FALSE);

    static int IsSupported(Display *dpy, int screenNum, 
			   char *errReturnBuf=NULL, int bufLen=0);

    /* Public override functions */
    virtual int SetSPx(SPxSc *sc,
		       SPxScDestBitmap *destBitmap,
		       SPxRunProcess *scProcess=NULL,
		       SPxScFollowX11 *followWin=NULL);

    virtual int AddSPx(SPxSc *sc,
		       SPxScDestBitmap *destBitmap,
		       SPxRunProcess *scProcess=NULL);

    virtual int RemoveSPx(SPxScDestBitmap *destBitmap);

    virtual int SetDrawable(Drawable drawable);

    virtual UINT32 CompositeDisplay(void);

    virtual void PixmapADrawing(void);
    virtual void PixmapBDrawing(void);

    /* Static functions. */
    static void SetDebug(int debug) { m_debug = debug; }
    static int GetDebug(void) { return m_debug; }

    static int CheckGLExtension(const char *extensionName);
    static int CheckGLXExtension(Display *dpy, int screenNum, 
				 const char *extensionName);
protected:

    /* Protected override functions */
    int CommonCreate(Display *display, int screen, 
                     int maxWidth, int maxHeight, 
                     int numRadarVideos);

private:
    /* Private variables. */
    struct impl;
    SPxAutoPtr<impl> m_p;

    SPxPPIwindowAccel_t m_accelMode;/* Acceleration mode. */
    Display *m_display;		    /* X display rendering to. */
    int m_screenNum;		    /* X screen rendering to. */
    GLXFBConfig *m_fbConfigs;	    /* Framebuffer configurations for display/screen. */
    SPxGlxFunctions m_glxFuncs;	    /* GL/GLX function pointers. */
    pthread_t m_threadId;	    /* Thread id of calling thread. */

    Drawable m_inWin;		    /* Input window. */
    Window m_outWin;		    /* Output window. */
    int m_outWinMapState;	    /* Output window map state. */
    Window m_parentWin;		    /* Parent window. */
    GLXWindow m_glxWin;		    /* GLX window wrapping X Drawable. */
    GLXContext m_glxContext;	    /* GLX context for Open GL. */
    int m_viewportW;		    /* Current viewport Width. */
    int m_viewportH;		    /* Current viewport Height. */

    Pixmap m_pixmapGraphics;	    /* Pixmap to hold redirected graphics window. */
    GLXPixmap m_glxPixmapA;	    /* GLX pixmap wrapping Pixmap A. */
    GLXPixmap m_glxPixmapB;	    /* GLX pixmap wrapping Pixmap B. */
    GLXPixmap m_glxPixmapGraphics;  /* GLX pixmap wrapping Pixmap Graphics. */
    GLuint m_pixmapTexA;	    /* GL texture receiving Pixmap A data. */
    GLuint m_pixmapTexB;	    /* GL texture receiving Pixmap B data. */
    GLuint m_pixmapTexGraphics;	    /* GL texture receiving Pixmap Graphics data. */
    static GLint m_fboInternalFormat; /* Internal format to use for FBO texture. */
    static GLint m_fboFormat;	    /* Format to use for FBO texture. */
    static GLint m_fboType;	    /* Data type to use for FBO texture. */

    static const int m_numBindTexTimes = 20;
    UINT32 m_bindTexTimes[m_numBindTexTimes];	/* Times taken to bind texture from pixmap. */
    int m_currentBindTexIndex;	    /* Current index into bind tex array. */
    UINT32 m_bindTexSum;	    /* Current sum of bind times. */
    UINT32 m_lastBindTexErrorTime;  /* Last time a bind slow error output. */
    static const UINT32 m_maxAverageBindTexMS;	/* Maximum allowable average bind time before error. */

    static int m_debug;		    /* Debugging level. */

    int m_successfulCreate;	    /* Has this object been successfully created? */

    /* Private functions. */
    int createContext(GLXContext *glxContext, GLXContext shareContext=NULL);
    void setupGL(void);
    SPxErrorCode addRadarTexture(SPxScDestBitmap *destBitmap);
    int renderPixmap(Pixmap pixmap, GLXPixmap glxPixmap, 
		     GLuint *texture, int width=-1, int height=-1);
    void renderRadarTexture(int texture, int width, int height,
			    int brightness);
    int checkGraphicsWindow(void);
    Window createWindow(void);
    void checkBindTexTime(UINT32 time);
    void completeGL(void);

}; /* SPxPPIwindowGlx class */

#endif /* _SPX_PPI_WINDOW_GLX_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/

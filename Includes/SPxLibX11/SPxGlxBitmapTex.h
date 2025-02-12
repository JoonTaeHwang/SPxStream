/*********************************************************************
*
* (c) Copyright 2011 - 2014, Cambridge Pixel Ltd.
*
* File:  $RCSfile: SPxGlxBitmapTex.h,v $
* ID: $Id: SPxGlxBitmapTex.h,v 1.8 2014/06/20 14:12:08 rew Exp $
*
* Purpose:
*   Header for class to provide OpenGL texture for radar.
*
* Revision Control:
*   20/06/14 v1.8    AGC	Move C++ headers to source file.
*
* Previous Changes:
*   04/12/13 1.7    AGC	Remove unused m_ppiWindow member.
*   04/10/13 1.6    AGC	Simplify headers.
*   20/04/11 1.5    AGC	Use multiple pixel buffer objects to prevent
*			artifacts on slower systems.
*   18/02/11 1.4    AGC	Use configurable framebuffer texture format.
*   11/02/11 1.3    AGC	Add internal SetContextBound() function.
*   04/02/11 1.2    AGC	Make constant functions const.
*			Remove now unused texture, FBO and renderbuffer.
*   28/01/11 1.1    AGC	Initial Version
*
**********************************************************************/

#ifndef _SPX_GLX_BITMAP_TEX_H
#define	_SPX_GLX_BITMAP_TEX_H

/* Standard X headers. */
#include "X11/Xlib.h"

/* Standard GL/GLX headers. */
#include "GL/gl.h"
#include "GL/glx.h"
#include "GL/glxext.h"

/* SPx headers. */
#include "SPxLibUtils/SPxAutoPtr.h"
#include "SPxLibUtils/SPxCriticalSection.h"
#include "SPxLibUtils/SPxObj.h"
#include "SPxLibSc/SPxScDestDisplay.h"
#include "SPxGlxFunctions.h"

/*********************************************************************
*
*   Constants
*
**********************************************************************/

/* Number of Pixel Buffer Objects to use to upload data to graphics card. */
#define SPX_GLX_NUM_PBO (3)

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

/* Forward declare classes referenced so headers do not need to be included. */
class SPxScDestBitmap;
class SPxPPIwindowGlx;

/* Structure for holding a vertex and texture co-ordinate. */
struct SPxGLVertex
{
    GLdouble x, y;	/* Vertex x and y co-ordinate. */
    GLdouble tx, ty;	/* Texture x and y co-ordinate. */
};

struct SPxGlxRegion
{
    unsigned char *data;
    int dataSize;
    UINT32 time;
    DirtyBox box;
    UINT16 firstAz;
    UINT16 endAz;
    UINT16 changes;
};

/*
 * SPxGlxBitmapTex class.
 */
class SPxGlxBitmapTex : public SPxObj
{
public:
    /* Public functions. */

    /* Construction and destruction. */
    SPxGlxBitmapTex(SPxScDestBitmap *destBitmap, GLint fboInternalFormat, 
		    GLint fboFormat, GLint fboType);
    virtual ~SPxGlxBitmapTex(void);
    SPxErrorCode Create();

    int Render(void);

    int GetTexture(void) const;

    void DrawSweepLine(int azimuth);

    /* Get texture size / radar size. */
    int GetTextureWidth(void) const { return m_fboWidth; }
    int GetTextureHeight(void) const { return m_fboHeight; }
    int GetWidth(void) const { return m_width; }
    int GetHeight(void) const { return m_height; }
    int GetBrightness(void) const { return m_brightness; }

    /* Get Bitmap */
    SPxScDestBitmap *GetBitmap(void) const { return m_destBitmap; }

    /* Internal function for proper destruction. */
    void SetContextBound(int contextBound) { m_contextBound = contextBound; }

private:
    /* Private variables. */
    struct impl;
    SPxAutoPtr<impl> m_p;
    SPxScDestBitmap *m_destBitmap;	/* Bitmap to render. */

    SPxGlxFunctions m_glxFuncs;		/* GL/GLX function pointers. */

    SPxCriticalSection m_mutex;		/* Mutex protection. */

    /* Radar settings. */
    UINT8 m_radarColourRed;		/* Red component of radar colour. */
    UINT8 m_radarColourGreen;		/* Green component of radar colour. */
    UINT8 m_radarColourBlue;		/* Blue component of radar colour. */
    int m_brightness;			/* Radar brightness. */
    int m_width;			/* Width of radar image in pixels. */
    int m_height;			/* Height of radar image in pixels. */
    int m_fboWidth;			/* Width of FBO in pixels. */
    int m_fboHeight;			/* Height of FBO in pixels. */
    int m_fadeType;			/* Fade type. */
    UINT32 m_lastFadeMs;		/* Time of last fade in milliseconds. */
    double m_fadeMsecs;			/* Milliseconds required between fades of 1 level. */
    int m_needsClear;			/* Radar needs to be cleared. */
    UINT32 m_lastUpdateTimeMs;		/* Last radar period update time. */
    UINT32 m_scanPeriodMs;		/* Current estimated scan period. */
    int m_fastRedraw;			/* Redraw full window on change. */
    int m_contextBound;			/* Is the GLX context bound on destruction. */
    GLint m_fboInternalFormat;		/* Internal format to use for FBO texture. */
    GLint m_fboFormat;			/* Format to use for FBO texture. */
    GLint m_fboType;			/* Data type to use for FBO texture. */

    /* Current sweep line settings. */
    SPxScSweepLineType_t m_sweepLineType;
    SPxScSweepLineColMode_t m_sweepLineColMode;
    UINT32 m_sweepLineRGB;
    int m_lastSweepLineAz;

    int m_gpuMemInfoAvailable;		/* Is GPU memory info available? */
    UINT32 m_lastMemInfoTimeMsecs;	/* Last time the GPU memory was checked. */

    int m_createdOK;			/* Object has been successfully created. */

    /* X objects. */
    Display *m_display;			/* X display. */
    int m_screenNum;			/* X screen number. */

    /* GLX objects. */
    GLXWindow m_glxWin;			/* GLX window where radar will be rendered. */
    GLXContext m_shareContext;		/* PPI window's GLX context. */
    GLXContext m_glxContext;		/* GLX context. */
    GLXContext m_fadeContext;		/* GLX context for performing fading. */
    GLXFBConfig *m_fbConfigs;		/* Framebuffer configurations. */

    /* Open GL objects. */
    GLuint m_texAlpha[SPX_GLX_NUM_PBO];	/* Textures containing intensity values only. */
    GLuint m_texOut;			/* Texture output by Fade FBO. */
    GLuint m_fboFade;			/* FBO used for fading radar. */
    GLuint m_pbo[SPX_GLX_NUM_PBO];		/* Pixel buffer object used to transfer data to GPU. */
    
    /* Private functions. */
    static void destBitmapUpdatedWrapper(SPxScDestBitmap *bitmap, UINT16 changes,
					 UINT16 firstAzimuth, UINT16 endAzimuth,
					 DirtyBox box, void *userPtr);
    void destBitmapUpdated(SPxScDestBitmap *bitmap, UINT16 changes,
			   UINT16 firstAzimuth, UINT16 endAzimuth,
			   DirtyBox box, void *userPtr);   

    int checkSettings(SPxErrorCode *err=NULL);
    void getBitmapData(int pboIndex, SPxGlxRegion &region);
    void renderFadeFBO(int pboIndex, SPxGlxRegion &region);
    void renderTexture(GLuint texture, DirtyBox box, int texIsBoxSize=FALSE);
    void fadeInternal(UINT32 time);
    void setFadeFBOcolour(void);
    double getLevelsToFade(UINT32 time);
    SPxErrorCode resizeFBOs(void);
    SPxErrorCode initGLXfuncPointers(void);
    SPxErrorCode createFBO(GLuint *fbo, GLuint texture);
    void updatePeriodEstimate(SPxGlxRegion &region);

}; /* SPxGlxBitmapTex class */

#endif /* _SPX_GLX_BITMAP_TEX_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/

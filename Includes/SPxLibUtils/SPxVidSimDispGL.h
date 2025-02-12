/*********************************************************************
 *
 * (c) Copyright 2013 - 2022, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   SPx Video Simulator SPxVidSimDispGL class header.
 *
 * Revision Control:
 *   27/09/22 v1.27   AGC	Rename SPxVidSimCameraMount to SPxVidSimMount.
 *
 * Previous Changes:
 *   09/09/22 1.26  SP 	Allow object type to be set in constructor.
 *   05/08/22 1.25  SP 	Remove CheckCamera().
 *   04/08/22 1.24  SP 	Update comment.
 *   26/07/22 1.23  SP 	Support camera view roll.
 *   26/07/22 1.22  SP 	Rename some events.
 *   22/07/22 1.21  SP 	Changes to simplify creation and deletion.
 *   16/07/19 1.20  SP 	Add optional force flag to SetViewMode().
 *   17/08/18 1.19  AGC	Support sharing video from multiple cameras.
 *   13/08/18 1.18  AGC	Enhance relative focus support.
 *   10/08/18 1.17  AGC	Support focus.
 *                     	Make various functions const.
 *   27/06/18 1.16  SP 	Support multiple platforms.
 *   01/02/18 1.15  SP 	Improve pan and tilt of camera using mouse.
 *   31/10/16 1.14  SP 	Add GetGLWin() and GetGLCtx().
 *   13/10/16 1.13  SP 	Move terrain rendering to separate class.
 *   07/10/16 1.12  SP 	Add GetViewCLatLong().
 *   11/01/16 1.11  AGC	Remove IsViewInverted().
 *   15/12/15 1.10  AGC	Add IsViewInverted().
 *   09/11/15 1.9   SP 	Allow free view centre marker to be hidden.
 *   14/10/15v1.8   SP 	Use camera settings when rendering.
 *   01/10/15 1.7   SP 	Add RGBToDisplay().
 *   04/09/15 1.6   SP 	Support disabling of redraw.
 *   27/08/15 1.5   SP 	Add debug code.
 *   26/08/15 1.4   SP 	Add m_glFboMulti.
 *   06/08/15 1.3   SP 	Support rendering to a texture.
 *   19/07/13 1.2   SP 	Further development.
 *   15/07/13 1.1   SP 	Initial version.
 *
 *********************************************************************/

#ifndef _SPX_VID_SIM_DISP_GL_H
#define _SPX_VID_SIM_DISP_GL_H

/* Other headers required. */
#include "SPxLibUtils/SPxTimer.h"
#include "SPxLibUtils/SPxGL.h"
#include "SPxLibUtils/SPxVidSimObj.h"

/*********************************************************************
 *
 *  Constants
 *
 *********************************************************************/

/* Free view mode. Other view modes are non-zero and identified 
 * by the MSB. Other view modes will contain additional encoded
 * data, including platform, camera mount and camera index.
 */
#define SPX_VID_SIM_DISP_FREE_VIEW       0x00000000

/* Mouse buttons and keys. */
#define SPX_VID_SIM_DISP_MOUSE_LEFT_BN   (1U << 1)
#define SPX_VID_SIM_DISP_MOUSE_MIDDLE_BN (1U << 2)
#define SPX_VID_SIM_DISP_MOUSE_RIGHT_BN  (1U << 3)
#define SPX_VID_SIM_DISP_SHIFT_BN        (1U << 4)
#define SPX_VID_SIM_DISP_CONTROL_BN      (1U << 5)

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

/*********************************************************************
 *
 *   Class definitions
 *
 *********************************************************************/

/* Forward declare any classes required. */
class SPxVidSimScene;
class SPxVidSimTerrainRendererGL;
class SPxVidSimTargetRendererGL;
class SPxVidSimMount;
class SPxVidSimCamera;

class SPxVidSimDispGL : public SPxVidSimObj
{
public:

    /*
     * Public types.
     */

    /*
     * Public variables.
     */

    /* Debug flags. */
    static const UINT32 DEBUG_FRAME = (1U << 0);

    /*
     * Public functions.
     */

    /* Constructor and destructor. */
    SPxVidSimDispGL(ObjectType_t objectType, SPxVidSimScene *scene);
    virtual ~SPxVidSimDispGL(void);
    SPxErrorCode Initialise(void);

    /* Access. */
    SPxGLWin *GetGLWin(void) { return m_glWin; }
    SPxGLCtx *GetGLCtx(void) { return m_glCtx; }

    /* Configuration. */
     SPxErrorCode SetWindowSizePixels(unsigned int width, 
                                      unsigned int height);
    unsigned int GetWindowWidthPixels(void) const { return m_winWidthPixels; }
    unsigned int GetWindowHeightPixels(void) const { return m_winHeightPixels; }

    /* View control. */
    SPxErrorCode SetViewMode(UINT32 viewMode, int forceUpdate=FALSE);
    UINT32 GetViewMode(void) const { return m_viewMode; }

    /* Free view. */
    SPxErrorCode SetFreeViewFramesPerSec(unsigned int fps);
    unsigned int GetFreeViewFramesPerSec(void) const { return m_freeViewFps; }
    void ShowFreeViewCentreMarker(int state);
    int IsFreeViewCentreMarkerVisible(void) const { return m_showFreeViewCentreMarker; }
    SPxErrorCode GetViewCLatLong(double *latDegsRtn,
                                 double *lonDegsRtn) const;

    /* View settings that depend on view mode. */
    ColourMode_t GetColourMode(void) const;
    void GetRGBForDisplay(UINT32 rgbIn, GLfloat fOut[3], UINT32 *rgbOut=NULL) const;
    double GetBackgroundBrightness(void) const;
    double GetTargetBrightness(void) const;
    unsigned int GetFrameWidthPixels(void) const;
    unsigned int GetFrameHeightPixels(void) const;
    unsigned int GetFramesPerSec(void) const;
    double GetFieldOfViewDegs(void) const;
    int IsRelFocusSupported(void) const;
    int IsAbsFocusSupported(void) const;
    int IsAutoFocusEnabled(void) const;
    UINT16 GetFocusPos(void) const;
    double GetFocusRangeMetres(void) const;
    double GetViewCXMetres(void) const;
    double GetViewCYMetres(void) const;
    double GetViewCZMetres(void) const;
    double GetViewPitchDegs(void) const;
    double GetViewRollDegs(void) const;
    double GetViewYawDegs(void) const;

    /* Mouse events. */
    void ReportMouseMoveEvent(UINT32 buttons, 
                              int xPixels, 
                              int yPixels,
                              int wPixels, 
                              int hPixels);

    void ReportMouseWheelEvent(UINT32 buttons,
                               int isScrollFwd,
                               int xPixels, 
                               int yPixels);

    /* Redraw contol. */
    int IsRedrawEnabled(void) { return m_isRedrawEnabled; }
    void SetRedrawEnabled(int isEnabled);

    /*
     * Public static functions.
     */
    static UINT32 EncodeCameraViewMode(UINT8 platformNum,
                                       UINT8 mountNum,
                                       UINT8 cameraNum);

    static SPxErrorCode DecodeCameraViewMode(UINT32 mode,
                                             UINT8 *platformNum,
                                             UINT8 *mountNumRtn,
                                             UINT8 *cameraNumRtn);
    static int IsCameraViewMode(UINT32 mode);
    
protected:

    /*
     * Protected variables.
     */

    SPxGLWin *m_glWin;                   /* OpenGL window. */
    SPxGLCtx *m_glCtx;	                 /* OpenGL context. */
    SPxGLFramebuffer *m_glFbo;           /* OpenGL frame buffer object. */
    SPxGLFramebuffer *m_glFboMulti;      /* OpenGL multi-sample FBO. */
    SPxGLTex *m_glTex;                   /* OpenGL texture. */
    unsigned int m_maxFrameWidthPixels;  /* Max frame width. */
    unsigned int m_maxFrameHeightPixels; /* Max frame height. */
    int m_isImageInverted;               /* Flip image vertically. */
    SPxTimer m_timer;                    /* Timer used to redraw window. */

    /*
     * Protected functions.
     */

    /* OpenGL rendering functions. */
    virtual SPxErrorCode InitialiseGL(void);
    virtual SPxErrorCode RedrawGL(void);

    /* Shutdown function. */
    virtual void Shutdown(void);

    /* Parameter handling. */
    virtual int SetParameter(char *name, char *value);
    virtual int GetParameter(char *name, char *valueBuf, int bufLen);

    /* Event handling. */
    virtual void HandleEvent(const EventInfo_t *eventInfo);

private:

    /*
     * Private types. 
     */

    /* Change flags. */
    typedef struct
    {
        int frameSize;          /* Video frame size has changed. */
        int windowSize;         /* Window size has changed. */
        int sceneSizePosition;  /* Scene size and/or position has changed. */
        int sceneAppearance;    /* Scene appearance has changed. */
        int view;               /* View scale or position has changed. */

    } ChangeFlags_t;

    /* Terrain point. */
    typedef struct 
    {
        GLfloat vertex[3];  /* Vertex X, Y and Z. */
        GLfloat normal[3];  /* Normal X, Y and Z. */

    } TerrainPoint_t;

    /*
     * Private variables.
     */

    /* Change flags. */
    SPxCriticalSection m_changeFlagsMutex;/* Mutex used to protect change flags. */
    ChangeFlags_t m_userChanges;          /* Flags set by user. */
    ChangeFlags_t m_redrawChanges;        /* Flags read by redraw thread. */

    /* Window attributes. */
    unsigned int m_winWidthPixels;  /* Window width. */
    unsigned int m_winHeightPixels; /* Window height. */
    double m_horzFovDegs;           /* Current horizontal field of view. */
    double m_vertFovDegs;           /* Current vertical field of view. */

    /* View control. */
    UINT32 m_viewMode;              /* Encoded view mode. */
    int m_mouseXPixels;             /* Last mouse X position. */
    int m_mouseYPixels;             /* Last mouse Y position. */

    /* Free view mode. */
    double m_freeViewCXMetres;      /* Free view X position of view centre. */
    double m_freeViewCYMetres;      /* Free view Y position of view centre. */
    double m_freeViewCZMetres;      /* Free view Z position of view centre. */
    double m_freeViewDistFromCentreMetres; /* Distance of eye from view centre. */
    double m_freeViewPitchDegs;     /* Free view pitch (elevation) about view centre. */
    double m_freeViewYawDegs;       /* Free view yaw (azimuth) about view centre. */
    double m_freeViewFovDegs;       /* Free view field of view in degrees. */
    unsigned int m_freeViewFps;     /* Free view frame rate. */
    int m_showFreeViewCentreMarker; /* Show free view centre marker? */
    UINT32 m_freeViewLastAdjTime;   /* Time of last free view adjustment. */

    /* Camera view mode. */
    mutable SPxCriticalSection m_cameraMutex;    /* Camera mutex. */
    SPxVidSimMount *m_mount;             /* Mount object to use for camera view. */
    SPxVidSimCamera *m_camera;           /* Camera object to use for camera view. */

    /* Terrain rendering. */
    SPxVidSimTerrainRendererGL *m_terrainRenderer;

    /* Target rendering. */
    SPxVidSimTargetRendererGL *m_targetRenderer;

    /* General. */
    int m_isRedrawEnabled;           /* Redraw enabled? */
    int m_isTimerSet;                /* Has timer been set? */

    /* Debugging. */
    UINT32 m_debugFlags;
    UINT32 m_lastFrameReportTime;
    UINT32 m_frameRenderMsecs;
    UINT32 m_frameCount;

    /*
     * Private functions.
     */

    /* Utility functions. */
    SPxErrorCode SelectCamera(UINT32 mode);
    int IsSupported(char *buffer, int bufLen);

    /* OpenGL rendering thread functions. */
    void ResetMatrixGL(void);
    void ClearBackgroundGL(void);
    void ApplyViewGL(void);   
    void ApplyLightingGL(void);
    void ApplyWeatherGL(void);
    void SetColourGL(UINT32 rgb);
    void DrawRefMarkerGL(void);
    void DrawRefCentreGL(void);
    void DrawRefAxesGL(GLfloat xLength, 
                       GLfloat yLength,
                       GLfloat zLength,
                       GLfloat xRed,
                       GLfloat xGreen,
                       GLfloat xBlue,
                       GLfloat yRed,
                       GLfloat yGreen,
                       GLfloat yBlue,
                       GLfloat zRed,
                       GLfloat zGreen,
                       GLfloat zBlue);
    void DrawBoundariesGL(void);
    void DrawPartitionGL(double xMetres, double yMetres, double zMetres);
    void DrawTerrainGL(void);

    /* Utility function. */


    /*
     * Private static functions.
     */

    static void RedrawTimer(void *userArg);

}; /* SPxVidSimDispGL */

#endif /* _SPX_VID_SIM_DISP_GL_H */

/*********************************************************************
 *
 *      End of file
 *
 *********************************************************************/

/*********************************************************************
*
* (c) Copyright 2011 - 2022, Cambridge Pixel Ltd.
*
* Purpose:
*   Header for SPxTrackRenderer base class which supports the display
*   of radar tracks in an application, but only via a derived class.
*
* Revision Control:
*   02/08/22 v1.54   AJH	Fix clipping of tracks.
*
* Previous Changes:
*   08/07/22 1.53   AJH	Support clipping of tracks outside window.
*   14/03/22 1.52   AGC	Support simple symbols for large views.
*   20/05/21 1.51   AGC	Update ASTERIX colours support for Mode 5.
*   26/11/20 1.50   SP 	Support changes to base class and fix issues.
*   12/11/20 1.49   SP 	Move more functionality into base class.
*			Use unified track ID to avoid issues with plots.
*   06/10/20 1.48   AGC	Support more MIL-2525-D amplifiers.
*   24/09/20 1.47   AGC	Render trail points and interceptor for MIL-2525-D tracks.
*   18/09/20 1.46   AGC	Support showing excluded tracks.
*   15/09/20 1.45   SP 	Suppport track repair.
*   11/09/20 1.44   SP 	Suppport display of lost tracks.
*   27/08/20 1.43   AGC	Support MIL2525D offset amplifiers.
*   26/08/20 1.42   AGC	Support IJMS/MIL2525D symbols.
*   11/07/19 1.41   AGC	Use more floating point positions.
*   20/11/18 1.40   AGC	High DPI improvements.
*   30/10/18 1.39   REW	Support SetAltitudeMode() to control source
*			of altitudes for %h height field.
*   24/07/18 1.38   AGC	Support fuse coast alpha.
*   25/05/18 1.37   REW	Use unified ID for interceptors.
*			Restore label position if drag aborted.
*   20/04/18 1.36   REW	Support strobe rendering too.
*   14/03/18 1.35   AGC	Support ASTERIX SCF colour.
*   14/12/17 1.34   AGC	Support ASTERIX ghost/test track colours.
*   11/12/17 1.33   AGC	Remove show missing ASTERIX fields option.
*   23/11/17 1.32   AGC	Add option for showing missing Asterix fields.
*   20/11/17 1.31   AGC	Support Asterix based track colours.
*   13/07/17 1.30   SP 	Add GetDatabase().
*   12/04/17 1.29   AGC	Support auto track label positioning.
*   13/10/16 1.28   AGC	Skip trail points when appropriate.
*   26/11/15 1.27   REW	Support flashing of highlighted threats.
*   12/03/15 1.26   REW	Support flashing of SPI targets.
*   02/10/14 1.25   REW	Support track label offsets.
*   02/06/14 1.24   AGC	Support captions.
*   16/05/14 1.23   AGC	Improve cursor selection.
*   10/04/14 1.22   AGC	Support double-click track selection.
*   08/04/14 1.21   REW	Support pre-render callback.
*   03/04/14 1.20   REW	Support flashing of emergency targets.
*   01/04/14 1.19   REW	Add Get/SetInterceptorID().
*   27/03/14 1.18   REW	Support different colour for selected targets.
*   09/01/14 1.17   REW	Support colour for track interception.
*   06/01/14 1.16   REW	Support track interception.
*   19/12/13 1.15   REW	Support threat colours.
*   07/10/13 1.14   AGC	Move rendering to renderAll().
*   14/08/13 1.13   SP 	Allow functions to be overridden.
*   14/08/13 1.12   AGC	Support track specific shapes and images.
*   20/03/13 1.11   SP 	Support fusion symbols.
*   21/12/12 1.10   AGC	Use new SPxTrackRendererBase base class.
*   23/11/12 1.9    AGC	Support SPxMouseInterface.
*			Add TrackSelect callbacks.
*   09/11/12 1.8    AGC	Add option to only show selected tracks.
*   29/03/12 1.7    SP 	Rename SPxTrackReport as SPxRadarTrack.
*   08/02/12 1.6    AGC	Add dashed vector and coasted vector support.
*   06/10/11 1.5    SP 	Add control to select filtered position.
*   27/07/11 1.4    SP 	Add control to show/hide labels.
*   07/07/11 1.3    SP 	Move track modes to header.
*                       Use SPxTrackReport.
*   26/04/11 1.2    SP 	Use base class view control.
*                       Render track trails and vectors.
*   12/04/11 1.1    SP 	Initial Version.
* 
**********************************************************************/

#ifndef _SPX_TRACK_RENDERER_H
#define _SPX_TRACK_RENDERER_H

/*
 * Other headers required.
 */

/* We need SPxLibUtils for common types, callbacks, errors etc. */
#include "SPxLibUtils/SPxArea.h"
#include "SPxLibUtils/SPxAutoPtr.h"
#include "SPxLibUtils/SPxCriticalSection.h"

/* We need the SPxTrackDatabase class. */
#include "SPxLibNet/SPxTrackDatabase.h"

/* We need our base class. */
#include "SPxLibUtils/SPxTrackRendererBase.h"

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

/* Need to forward-declare other classes in case headers are in wrong order.*/
class SPxTrackDatabase;
class SPxTrackRendererBase;
class SPxSymbol;

enum SPxMIL2525DSymComponents
{
    SPX_MIL2525D_SYM_COMP_NONE = 0x0000,
    SPX_MIL2525D_SYM_COMP_FRAME = 0x0001,
    SPX_MIL2525D_SYM_COMP_FILL = 0x0002,
    SPX_MIL2525D_SYM_COMP_ICON_COLOUR = 0x0004,
    SPX_MIL2525D_SYM_COMP_ICON_BW = 0x0008,
    SPX_MIL2525D_SYM_COMP_MOD = 0x0010,
    SPX_MIL2525D_SYM_COMP_AMPLIFIERS = 0x0020,
    SPX_MIL2525D_SYM_COMP_OCTAGON = 0x8000,
    SPX_MIL2525D_SYM_COMP_DEFAULT = SPX_MIL2525D_SYM_COMP_FRAME |
				    SPX_MIL2525D_SYM_COMP_FILL |
				    SPX_MIL2525D_SYM_COMP_ICON_COLOUR |
				    SPX_MIL2525D_SYM_COMP_MOD |
				    SPX_MIL2525D_SYM_COMP_AMPLIFIERS
};

enum SPxMIL2525DSymAmplifiers
{
    SPX_MIL2525D_SYM_AMP_NONE = 0x00000000,

    SPX_MIL2525D_SYM_AMP_NUM = 0x00000001,
    SPX_MIL2525D_SYM_AMP_IFF = 0x00000002,
    SPX_MIL2525D_SYM_AMP_AIS = 0x00000004,
    SPX_MIL2525D_SYM_AMP_TYPE = 0x00000008,
    SPX_MIL2525D_SYM_AMP_LOCATION = 0x0000010,
    SPX_MIL2525D_SYM_AMP_SPEED = 0x0000020,
    SPX_MIL2525D_SYM_AMP_ALTITUDE = 0x00000040,
    SPX_MIL2525D_SYM_AMP_DEPTH = 0x00000080,
    SPX_MIL2525D_SYM_AMP_COMMENTS = 0x00001000,
    SPX_MIL2525D_SYM_AMP_INFO = 0x00002000,

    SPX_MIL2525D_SYM_AMP_AOU = 0x00010000,

    SPX_MIL2525D_SYM_AMP_ALL_TEXT = SPX_MIL2525D_SYM_AMP_NUM |
				    SPX_MIL2525D_SYM_AMP_IFF |
				    SPX_MIL2525D_SYM_AMP_AIS |
				    SPX_MIL2525D_SYM_AMP_TYPE |
				    SPX_MIL2525D_SYM_AMP_SPEED |
				    SPX_MIL2525D_SYM_AMP_ALTITUDE |
				    SPX_MIL2525D_SYM_AMP_DEPTH |
				    SPX_MIL2525D_SYM_AMP_COMMENTS |
				    SPX_MIL2525D_SYM_AMP_INFO
};

enum SPxMIL2525DSymBright
{
    SPX_MIL2525D_SYM_BRIGHT_DARK = 0,
    SPX_MIL2525D_SYM_BRIGHT_MEDIUM = 1,
    SPX_MIL2525D_SYM_BRIGHT_LIGHT = 2
};

enum SPxMIL2525DSymClutter
{
    SPX_MIL2525D_SYM_CLUTTER_NONE = 0,
    SPX_MIL2525D_SYM_CLUTTER_DOTS = 1
};

typedef void (*SPxTrackRendererSelectFn_t)(SPxRadarTrack *track, void *userArg);
typedef int (*SPxTrackRendererTestRepairFn_t)(void *userArg, 
					   SPxRadarTrack *track1,
					   SPxRadarTrack *track2);
typedef SPxErrorCode (*SPxTrackRendererDoRepairFn_t)(void *userArg, 
					   SPxRadarTrack *track1,
					   SPxRadarTrack *track2);

/*
 * Define our class, derived from the abstract renderer base class.
 */
class SPxTrackRenderer : public SPxTrackRendererBase
{
public:
    /*
     * Public variables.
     */

    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    SPxTrackRenderer(SPxTrackDatabase *database, 
		     SPxTrackRenderConfig renderConfig = SPX_TRACK_RENDER_CONFIG_SPX);
    virtual ~SPxTrackRenderer(void);

    /* Get database. */
    SPxTrackDatabase *GetDatabase(void) { return m_database; }

    /* Mouse handling. */
    virtual int HandleMousePress(UINT32 flags, int x, int y, SPxCursor_t *cursor);
    virtual int HandleMouseRelease(UINT32 flags, int x, int y, SPxCursor_t *cursor);
    virtual int HandleDoubleClick(UINT32 flags, int x, int y, SPxCursor_t *cursor);
    virtual int HandleMouseMove(UINT32 flags, int x, int y, SPxCursor_t *cursor);
    virtual int HandleMouseLeave(void);

    /* Track selection handlers. */
    SPxErrorCode AddTrackSelectCallback(SPxTrackRendererSelectFn_t fn, void *userArg);
    SPxErrorCode RemoveTrackSelectCallback(SPxTrackRendererSelectFn_t fn, void *userArg);

    /* Track repair callback. */
    SPxErrorCode SetTrackRepairCallback(SPxTrackRendererTestRepairFn_t testFn, 
					SPxTrackRendererDoRepairFn_t doFn, 
					void *userArg);

    /* Interception support. */
    SPxErrorCode SetInterceptorUniID(UINT32 id)
    {
	m_interceptorUniID = id;
	return(SPX_NO_ERROR);
    }
    UINT32 GetInterceptorUniID(void) const { return(m_interceptorUniID); }
    SPxErrorCode SetInterceptionSpeed(double mps);
    double GetInterceptionSpeed(void) const { return(m_interceptionSpeedMps); }
    SPxErrorCode SetInterceptionColour(UINT32 argb);
    UINT32 GetInterceptionColour(void) const;

    /* Cursor caption text. */
    SPxErrorCode SetCaption(const char *title, const char *caption);
    
    /* Simple symbol control. */
    SPxErrorCode SetSimpleViewWidthMetres(double simpleViewWidthMetres);
    double GetSimpleViewWidthMetres(void) const {return(m_simpleViewWidthMetres);}

    /* Remove tracks outside window. */
    SPxErrorCode SetClipTracksToWindow(int clipTracksToWindow) { m_clipTracksToWindow = clipTracksToWindow; return SPX_NO_ERROR; }
    int GetClipTracksToWindow(void) const { return(m_clipTracksToWindow); }

    /* MIL2525D options. */
    SPxErrorCode SetMIL2525DSymSize(double sizeMM);
    double GetMIL2525DSymSize(void) const;
    SPxErrorCode SetMIL2525DSymComponents(UINT32 components);
    UINT32 GetMIL2525DSymComponents(void) const;
    SPxErrorCode SetMIL2525DSymAmplifiers(UINT32 amplifiers);
    UINT32 GetMIL2525DSymAmplifiers(void) const;
    SPxErrorCode SetMIL2525DSymBrightness(SPxMIL2525DSymBright brightness);
    SPxMIL2525DSymBright GetMIL2525DSymBrightness(void) const;
    SPxErrorCode EnableMIL2525DSymTransparency(int enable);
    int IsMIL2525DSymTransparencyEnabled(void) const;
    SPxErrorCode SetMIL2525DSymClutter(SPxMIL2525DSymClutter clutter);
    SPxMIL2525DSymClutter GetMIL2525DSymClutter(void) const;
    SPxErrorCode SetMIL2525DBackgroundDark(int backgroundDark);
    int IsMIL2525DBackgroundDark(void) const;

    /* Generic parameter assignment. */
    virtual int SetParameter(char *parameterName, char *parameterValue);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);
    virtual SPxErrorCode SetStateFromConfig(void);

protected:
    /*
     * Protected functions.
     */
    virtual SPxErrorCode renderAll(void);
    virtual void GetTrackLabel(SPxRadarTrack *track,
                               char *buffer,
                               unsigned int bufferSizeBytes);

    virtual SPxErrorCode RenderTrack(SPxRadarTrack *track);
    virtual SPxErrorCode RenderTrailPoint(const SPxRadarTrackTrailPoint_t *trailPt);
    virtual SPxErrorCode RenderStrobe(struct SPxPacketStrobe_tag *strobe);

private:
    /*
     * Private variables.
     */

    struct TrackSelectInfo_t
    {
	TrackSelectInfo_t *next;
	SPxTrackRendererSelectFn_t fn;
	void *userArg;
    };

    struct impl;
    SPxAutoPtr<impl> m_p;

    /* Mutex protection. */
    mutable SPxCriticalSection m_mutex;

    /* Handle of associated database object. */
    SPxTrackDatabase *m_database;

    /* Track select functions. */
    TrackSelectInfo_t *m_trackSelectFns;

    /* Track interception. */
    UINT32 m_interceptorUniID;		/* NB: Unified ID */
    double m_interceptionSpeedMps;	/* Zero to use track speed */
    UINT32 m_interceptionColour;	/* Zero to use threat colour */

    /* Track label deconfliction. */
    UINT32 m_movingLabelTrackUniID;	/* Unified ID of track we're moving */
    int m_movingLabelOrigX;		/* Original position of label */
    int m_movingLabelOrigY;		/* Original position of label */
    UINT32 m_movingLabelHoverTrackUniID;/* Unified ID of track that label is hovered over. */
    UINT32 m_movingLabelHoverStartTime;	/* Time we started hovered over track. */

    /* Simple track display. */
    double m_simpleViewWidthMetres;	/* View width threshold for simplified symbols */

    /* Remove tracks outside window. */
    int m_clipTracksToWindow;

    /* Cursor caption text. */
    SPxAutoPtr<char[]> m_captionTitle;
    SPxAutoPtr<char[]> m_caption;
    int m_mouseX;
    int m_mouseY;
    int m_mouseValid;

    /* MIL2525D options. */
    double m_MIL2525DSymSizeMM;
    UINT32 m_MIL2525DSymComponents;
    UINT32 m_MIL2525DSymAmplifiers;
    SPxMIL2525DSymBright m_MIL2525DSymBrightness;
    int m_MIL2525DSymTransparencyEnabled;
    SPxMIL2525DSymClutter m_MIL2525DSymClutter;
    int m_MIL2525DBackgroundIsDarker;

    /* Trail point history. */
    double m_prevTrailX;
    double m_prevTrailY;

    /* Track repair callbacks. */
    SPxTrackRendererTestRepairFn_t m_testTrackRepairFn;
    SPxTrackRendererDoRepairFn_t m_doTrackRepairFn;
    void *m_trackRepairFnArg;

    /*
     * Private functions.
     */
    void setCursor(int x, int y, SPxCursor_t *cursor);
    int selectTrack(UINT32 flags, int x, int y, SPxCursor_t *cursor);
    SPxErrorCode getXYForTrack(const SPxRadarTrack *track,
			       double *xPixelsPtr,
			       double *yPixelsPtr,
			       double *xMetresPtr = NULL,
			       double *yMetresPtr = NULL,
			       int *isInWindowPtr = NULL) const;
    SPxErrorCode renderInterceptor(const SPxRadarTrack *track, UINT32 col);
    SPxErrorCode renderTrackMIL2525D(SPxRadarTrack *track);
    SPxAreaD getSymbolAreaMIL2525D(SPxRadarTrack *track);
    SPxAreaD getMIL2525DFrameArea(UINT8 identity,
				  UINT8 set,
				  SPxRendererXYD_t *pos,
				  double size,
				  SPxRendererXYD_t *leftAnchorRtn=NULL,
				  SPxRendererXYD_t *rightAnchorRtn=NULL);
    SPxErrorCode drawMIL2525DFrame(UINT8 identity, UINT8 set,
				   SPxRendererXYD_t *pos,
				   double size, UINT32 col, int frameStyle);
    SPxErrorCode drawMIL2525DOffset(SPxRendererXYD_t *trackPos,
				    SPxRendererXYD_t *leftAnchorPos,
				    SPxRendererXYD_t *rightAnchorPos,
				    UINT32 col);
    SPxErrorCode drawMIL2525DIcon(UINT8 set, UINT8 entity,
				  UINT8 entityType, UINT8 entitySubtype,
				  int drawIconFill,
				  UINT32 colFill, UINT32 colBorder,
				  double size,
				  SPxRendererXYD_t *centre);
    SPxErrorCode drawMIL2525DModifier(UINT8 set, UINT8 modifier1, UINT8 modifier2,
				      UINT32 col, double size,
				      SPxRendererXYD_t *centre);
    SPxErrorCode drawMIL2525DAmplifiers(const SPxPacketTrackExtended_tag *extRpt,
					UINT8 set,
					UINT32 col,
					const SPxAreaD *frameArea);
    SPxErrorCode drawMIL2525DAmplifierText(const char *text,
					   UINT32 col,
					   const SPxRendererXYD_t *tl,
					   double textHeight,
					   int rightAlign=FALSE);
    UINT32 getMIL2525DMainColour(UINT8 identity) const;
    double getSymbolSizeMIL2525D(void) const;
    SPxRadarTrack *getRepairTrack(void);
    SPxErrorCode renderCaption(void);

    /*
     * Private static functions.
     */
    static int staticRenderTrack(void *database, 
                                 void *arg, 
                                 void *track);

    static int staticRenderTrailPoint(void *database, 
                                      void *arg, 
                                      void *trailPt);

    static int staticRenderStrobe(void *database, 
                                 void *arg, 
                                 void *strobe);

    static int staticGetTextOffsets(void *database,
				    void *arg,
				    void *track);
    static int staticUpdateLabelOffsets(void *database,
					void *arg,
					void *track);
    static int staticFindLabelOverlap(void *database,
				      void *arg,
				      void *track);

    static int staticGetTrackAreas(void *database,
				   void *arg,
				   void *track);

}; /* SPxTrackRenderer */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* SPX_TRACK_RENDERER_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/

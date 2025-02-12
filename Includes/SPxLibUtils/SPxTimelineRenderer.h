/*********************************************************************
 *
 * (c) Copyright 2014 - 2020, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   Header for SPxTimelineRenderer base class which supports the
 *   display of a record/replay timeline via a derived class.
 *
 * Revision Control:
 *   01/12/20 v1.31  SP 	Support vertical scrolling.
 *
 * Previous Changes:
 *   08/07/19 1.30  SP 	Improvements to mouse click and drag handling.
 *   08/02/19 1.29  AGC	Fix N/C #664 - add PreRenderSetup().
 *   24/03/17 1.28  SP 	Rename a function.
 *   15/12/16 1.27  SP 	Display date when cursor not over timeline.
 *   08/07/16 1.26  SP 	Support info marker.
 *                      Fix version history.
 *   24/02/16 1.25  SP 	Add GetBestHeightPixels().
 *   22/02/16 1.24  SP 	Rename some functions.
 *   01/02/16 1.23  SP 	Add isSessionInView().
 *   22/01/16 1.22  SP 	Use UINT8 for channel ID.
 *   11/01/16 1.21  SP 	Make thumbnail positions less jittery.
 *   10/12/15 1.20  SP 	Add isThumbnailListInView().
 *   04/12/15 1.19  SP 	Support per session rendering.
 *   01/12/15 1.18  SP 	Support more changes to the database.
 *   26/11/15 1.17  SP 	Fix jitter in auto-scroll mode.
 *                      Add IsRedrawRequired().
 *   16/11/15 1.16  SP 	Support multiple thumbnail channels.
 *   05/06/15 1.15  SP  Add 'go to time' option to menu.
 *                      Add FitToView().
 *   18/05/15 1.14  SP  Show progress as records and images are 
 *                      extracted from a new session.
 *   22/04/15 1.13  REW Avoid warning in last change.
 *   22/04/15 1.12  SP  Support A/B markers.
 *   27/03/15 1.11  SP  Change channel label colour.
 *   26/03/15 1.10  SP  Do periodic channel sync.
 *   23/02/15 1.9   SP  Fix more record selection problems.
 *   20/02/15 1.8   SP  Improvments to rendering.
 *   16/02/15 1.7   SP  Fix cursor and selection issues.
 *   05/02/15 1.6   SP  Support changes to SPxTimelineDatabase.
 *   26/01/15 1.5   SP  Add getStampRecordPosition().
 *   07/01/15 1.4   SP  Support auto-scroll.
 *                      Support stamp record selection.
 *   11/12/14 1.3   SP  Improve height scaling.
 *                      Render linked records on channel line.
 *                      Add thumnail images and many other changes.
 *   24/11/14 1.2   SP  Further development.
 *   21/11/14 1.1   SP  Initial version.
 *
 **********************************************************************/

#ifndef _SPX_TIMELINE_RENDERER_H
#define _SPX_TIMELINE_RENDERER_H

/*
 * Other headers required.
 */

/* Include common types, callbacks, errors etc. */
#include "SPxLibUtils/SPxAutoPtr.h"
#include "SPxLibUtils/SPxObj.h"
#include "SPxLibUtils/SPxError.h"
#include "SPxLibUtils/SPxStrings.h"
#include "SPxLibUtils/SPxTime.h"
#include "SPxLibUtils/SPxRenderer.h"
#include "SPxLibUtils/SPxTimelineDatabase.h"
#include "SPxLibUtils/SPxCriticalSection.h"
#include "SPxLibData/SPxChannelDatabase.h"

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

/* Forward declare other classes required. */
class SPxTimelineDatabase;
class SPxTimelineReplay;
class SPxChannelDatabase;
class SPxImage;

/*
 * Define our class, derived from the abstract renderer base class.
 */
class SPxTimelineRenderer : virtual public SPxRenderer
{
public:
    /*
     * Public variables.
     */

    /*
     * Public functions.
     */

    /* Constructor and destructor. */
    SPxTimelineRenderer(SPxTimelineDatabase *database);
    virtual ~SPxTimelineRenderer(void);

    /* View control. */
    virtual SPxErrorCode SetViewCentreTime(const SPxTime_t *time);
    virtual SPxErrorCode GetViewCentreTime(SPxTime_t *time);
    virtual SPxErrorCode SetViewWidthSecs(double secs);
    virtual double GetViewWidthSecs(void) { return m_viewWidthSecs; }
    virtual SPxErrorCode SetAutoScrollEnabled(int isEnabled);
    virtual int IsAutoScrollEnabled(void) { return m_isAutoScrollEnabled; }
    virtual SPxErrorCode ZoomIn(void);
    virtual SPxErrorCode ZoomOut(void);
    virtual SPxErrorCode Home(void);
    virtual SPxErrorCode FitToView(void);
    virtual int IsRedrawRequired(void);
    virtual int GetTotalHeightPixels(int forceUpdate);
    virtual double GetVertScrollPercent(void) { return m_vertScrollPercent; }
    virtual SPxErrorCode PreRenderSetup(double vertScrollPercent);

    /* Thumbnail channel. */
    virtual SPxErrorCode SetThumbnailChannel(UINT8 chanID);
    virtual UINT8 GetThumbnailChannel(void) { return m_thumbnailChanID; }

    /* Functions for handling mouse control */
    virtual int HandleMouseMove(UINT32 flags, 
                                int x, int y, 
                                SPxCursor_t *cursor);
    virtual int HandleMouseLeave(void);
    virtual int HandleMousePress(UINT32 flags, 
                                 int x, int y, 
                                 SPxCursor_t *cursor);
    virtual int HandleMouseRelease(UINT32 flags, 
                                   int x, int y, 
                                   SPxCursor_t *cursor);
    virtual int HandleDoubleClick(UINT32 flags,
                                  int x, int y,
                                  SPxCursor_t *cursor);
    virtual int HandleMouseWheel(UINT32 flags, 
                                 int x, int y, 
                                 double delta);
   
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
     * Private types.
     */

    /* Horizontal text justification. */
    typedef enum
    {
        XJUSTIFY_LEFT = 0,
        XJUSTIFY_CENTRE = 1,
        XJUSTIFY_RIGHT = 2
    
    } XJustify_t;

    /* Vertical text justification. */
    typedef enum
    {
        YJUSTIFY_TOP = 0,
        YJUSTIFY_CENTRE = 1,
        YJUSTIFY_BOTTOM = 2
    
    } YJustify_t;

    /* Date/time string types. */
    typedef enum
    {
        TIME_STR_TYPE_AUTO = 0,
        TIME_STR_TYPE_FULL = 1,
        TIME_STR_TYPE_DATE = 2

    } TimeStrType_t;

    /* Mouse modes. */
    typedef enum
    {
        MOUSE_MODE_NONE,                      /* No action. */
        MOUSE_MODE_SELECT,                    /* Selection. */
        MOUSE_MODE_VIEW_PAN_PENDING,          /* View pan ready. */
        MOUSE_MODE_VIEW_PAN_IN_PROGRESS,      /* View pan in progress. */
        MOUSE_MODE_MARKER_DRAG_PENDING,       /* Marker drag ready. */
        MOUSE_MODE_MARKER_DRAG_IN_PROGRESS,   /* Marker drag in progress. */
        MOUSE_MODE_MARKER_A_DRAG_PENDING,     /* Marker A drag ready. */
        MOUSE_MODE_MARKER_A_DRAG_IN_PROGRESS, /* Marker A drag in progress. */
        MOUSE_MODE_MARKER_B_DRAG_PENDING,     /* Marker B drag ready. */
        MOUSE_MODE_MARKER_B_DRAG_IN_PROGRESS  /* Marker B drag in progress. */
    
    } MouseMode_t;
            
    /*
     * Private variables.
     */

    /* Forward declate classes. */
    struct ChannelInfo_t;
    struct ThumbnailInfo_t;
    struct SessionInfo_t;
    struct impl;
    SPxAutoPtr<impl> m_p;

    /* General. */
    SPxCriticalSection m_renderMutex;   /* Rendering mutex. */
    int m_nextChannelYPixels;           /* Next channel Y position. */
    int m_recordsYPixels;               /* Records Y position (no chan). */
    int m_thumbnailsYPixels;            /* Thumbnails top Y position. */
    int m_thumbnailsHeightPixels;       /* Thumbnails height pixels. */
    int m_minThumbnailIntervalSecs;     /* Minimum thumbnail interval. */
    int m_timelineHeightPixels;         /* Height of the timeline. */
    UINT32 m_textARGB;                  /* General text colour. */
    UINT32 m_chanTextARGB;              /* Channel name text colour. */
    UINT32 m_backgroundARGB;            /* Background colour. */
    UINT32 m_frameARGB;                 /* Frame colour. */
    UINT32 m_markerARGB;                /* Current time marker colour. */
    UINT32 m_startStopMarkerARGB;       /* Start/stop marker colour. */
    UINT32 m_abMarkerARGB;              /* A/B marker colour. */
    UINT32 m_infoMarkerARGB;            /* Info marker colour. */
    UINT32 m_eventLineARGB;             /* Event line for records and chans.*/
    UINT32 m_outlineARGB;               /* Colour for drawing outlines. */
    UINT32 m_thumbnailStripARGB;        /* Thumbnail strip background. */
    int m_lineSpacingPixels;            /* Spacing between lines of text. */
    int m_isChannelListUpdatePending;   /* Channel list update pending? */
    UINT32 m_lastChannelListUpdateMsecs;/* Last channel list update time. */
    int m_isThumbnailListUpdatePending;     /* Image list update pending? */
    UINT32 m_lastThumbnailListUpdateMsecs;  /* Last image list update time. */
    int m_isThumbnailAdded;             /* Is a thumbnail added to session? */
    int m_topMarginPixels;              /* Timeline top margin. */
    int m_bottomMarginPixels;           /* Timeline bottom margin. */
    int m_leftMarginPixels;             /* Timeline left margin. */
    int m_rightMarginPixels;            /* Timeline right margin. */
    int m_minLeftMarginPixels;          /* Timeline minimum left margin. */
    int m_minorTickIntervalSecs;        /* Minor tick interval in seconds. */
    int m_majorTickIntervalSecs;        /* Major tick interval in seconds. */
    SPxTime_t m_markerTime;             /* Record/replay marker time. */
    int m_markerXPixels;                /* Marker position in pixels. */
    int m_isAMarkerSet;                 /* Is A marker set? */
    SPxTime_t m_aMarkerTime;            /* A marker time. */
    int m_aMarkerXPixels;               /* A marker position in pixels. */
    int m_isBMarkerSet;                 /* Is B marker set? */
    SPxTime_t m_bMarkerTime;            /* B marker time. */
    int m_bMarkerXPixels;               /* B marker position in pixels. */
    int m_isInfoMarkerSet;              /* Is info marker set? */
    SPxTime_t m_infoMarkerTime;         /* Info marker time. */
    int m_infoMarkerXPixels;            /* Info marker position in pixels. */
    int m_isCentreOnRecordEnabled;      /* Allow centre on record? */
    SPxCursor_t m_currentCursor;        /* Current cursor to display. */
    MouseMode_t m_mouseMode;            /* Current mouse mode. */
    SPxTime_t m_mouseAnchorTime;        /* Mouse anchor when panning/dragging. */
    int m_mouseAnchorXPixels;           /* X mouse anchor when panning/dragging.*/
    int m_cursorXPixels;                /* Current cursor X position. */
    int m_cursorYPixels;                /* Current cursor Y position. */
    int m_isStopMarkerPresent;          /* Is session stop marker present? */
    UINT8 m_thumbnailChanID;            /* Thumbnail channel. */
    UINT32 m_lastRedrawMsecs;           /* Last redraw timestamp. */
    int m_isMouseMoved;                 /* Set to TRUE if mouse moved over timeline. */
    unsigned int m_lastViewWidthPixels; /* Last view width in pixels. */
    unsigned int m_lastViewHeightPixels;/* Last view height in pixels. */
    double m_vertScrollPercent;         /* Vertical scroll position. */
    
    /* View. Take care as m_viewWidthPixels from the base class
     * defines the full window width. However m_viewWidthSecs
     * defines the width of the timeline inside the margins.
     */
    SPxTime_t m_viewCentreTime;         /* Rounded view centre epoch time. */
    SPxTime_t m_realViewCentreTime;     /* Unrounded view centre epoch time. */
    double m_viewWidthSecs;             /* View width in seconds. */
    double m_viewPixelsPerSec;          /* View scale in pixels per second. */
    int m_isAutoScrollEnabled;          /* Enable or disable auto-scroll. */

    /* Databases. */
    SPxTimelineDatabase *m_timelineDB;
    SPxChannelDatabase *m_channelDB;
    SPxTimelineReplay *m_replayDB;

    /*
     * Private functions.
     */

    /* Rendering functions. */
    SPxErrorCode renderAll(void);
    void preRenderSetup(void);
    void renderBackground(void);
    void renderSessions(void);
    void renderSession(SPxTimelineSession *session);
    void renderStartStopMarkers(SPxTimelineSession *session);
    void renderStartStopMarker(SPxTime_t time);
    void renderRecords(SPxTimelineSession *session);
    void renderRecord(const SPxPacketRecord *record);
    void renderChannels(SPxTimelineSession *session);
    void renderChannel(ChannelInfo_t *info,
                       const SPxPacketRecord *record);
    void renderThumbnails(SessionInfo_t *info);
    void renderSelectedRecord(void);
    void renderMarker(void);
    void renderABMarkers(void);
    void renderInfoMarker(void);
    void renderFrame(void);
    void renderCursorTime(void);
    void drawLabel(int xPixels, int yPixels, 
                   const char *text, 
                   UINT32 argb,
                   XJustify_t xJustify=XJUSTIFY_LEFT,
                   YJustify_t yJustify=YJUSTIFY_TOP);

    /* Time <-> X pixels conversion functions. */
    int secsToNumPixels(double secs);
    double numPixelsToSecs(int numPixels);
    int timeToXPixels(SPxTime_t time);
    SPxTime_t xPixelsToTime(int xPixels);

    /* General. */
    int isXInTimeline(int xPixels);
    int isYInTimeline(int yPixels);
    int isSessionInTimeline(SPxTimelineSession *session);
    void clearChannelList(void);
    void updateChannelList(void);
    void deleteSessionInfo(SessionInfo_t **info);
    void deleteAllSessionInfo(void);
    void updateThumbnailLists(void);
    void updateThumbnailList(SessionInfo_t *info);
    int isThumbnailListInView(void);
    void processChannelEvent(const SPxChannelDatabase::Event_t *evt);
    void processTimelineEvent(const SPxTimelineDatabase::Event_t *evt);
    void getTimeString(SPxTime_t time, char *buf, 
                       unsigned int bufSizeBytes,
                       TimeStrType_t type=TIME_STR_TYPE_AUTO);
    void getDurationString(UINT32 secs, char *buf, 
                           unsigned int bufSizeBytes);
    int getTickIntervalSecs(int *minorSecsRtn=NULL);
    SPxTime_t getTimeRoundedToMinorTick(SPxTime_t time, int roundUp);
    SPxTime_t getTimeRoundedToMajorTick(SPxTime_t time, int roundUp);
    ChannelInfo_t *getRecordChannelInfo(UINT8 recChanID);
    int getRecordPosition(SPxTime_t recTime, UINT8 recChanID,
                          int *xPixelsRtn, int *yPixelsRtn);
    UINT32 getRecordAtPosition(int xPixels, int yPixels);
    UINT32 getChannelARGB(SPxStatus_t status);
    SPxTime_t getRecordTime(const SPxPacketRecord *rec);
    int isTimeValid(SPxTime_t time);
    int getNearestChannel(int yPixels, int *distYPixelsRtn);
    int getSessionStartXPixels(SPxTimelineSession *session);
    int getSessionEndXPixels(SPxTimelineSession *session);

    /*
     * Private static functions.
     */

    static int channelEventHandler(void *invokingObjPtr,
                                   void *userObjPtr,
                                   void *eventPtr);

    static int timelineEventHandler(void *invokingObjPtr,
                                    void *userObjPtr,
                                    void *eventPtr);

    static int updateSessionIterator(void *timelineDBVoid,
                                     void *objVoid,
                                     void *sessionVoid);

    static int deleteSessionIterator(void *timelineDBVoid,
                                     void *objVoid,
                                     void *sessionVoid);

    static int renderSessionIterator(void *timelineDBVoid,
                                     void *objVoid,
                                     void *sessionVoid);

    static int updateChannelIterator(void *channeDBVoid, 
                                     void *objVoid, 
                                     void *channelVoid);

    static int renderRecordIterator(void *timelineDBVoid,
                                    void *objVoid,
                                    void *recordVoid);

    static int channelRecordIterator(void *timelineDBVoid,
                                     void *objVoid,
                                     void *recordVoid);

    static void menuSetInfoMarkerHandler(SPxPopupMenu *menu,
                                         int id, 
                                         void *userArg);

    static void menuClearInfoMarkerHandler(SPxPopupMenu *menu,
                                           int id, 
                                           void *userArg);

    static void menuSetAMarkerHandler(SPxPopupMenu *menu,
                                      int id, 
                                      void *userArg);

    static void menuClearAMarkerHandler(SPxPopupMenu *menu,
                                        int id, 
                                        void *userArg);
    
    static void menuSetBMarkerHandler(SPxPopupMenu *menu,
                                      int id, 
                                      void *userArg);

    static void menuClearBMarkerHandler(SPxPopupMenu *menu,
                                        int id, 
                                        void *userArg);
    
    static void menuClearABMarkersHandler(SPxPopupMenu *menu,
                                          int id, 
                                          void *userArg);

    static void menuGotoTimeHandler(SPxPopupMenu *menu,
                                    int id, 
                                    void *userArg);

    static void menuGotoRecordHandler(SPxPopupMenu *menu,
                                      int id, 
                                      void *userArg);

}; /* SPxTimelineRenderer */


/*********************************************************************
 *
 *   Function prototypes
 *
 **********************************************************************/

#endif /* SPX_TIMELINE_RENDERER_H */

/*********************************************************************
 *
 * End of file
 *
 **********************************************************************/

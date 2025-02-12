/*********************************************************************
 *
 * (c) Copyright 2020, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   Header for renderer base class which supports the display 
 *   of plots and tracks in a range/altitude window, but only 
 *   via a derived class.
 *
 *   Note that this renderer cannot be used in a PPI window 
 *   and is not compatible with SPxViewControl.
 *
 * Revision Control:
 *   30/11/20 v1.9   SP 	Fix build warning.
 *
 * Previous Changes:
 *   25/11/20 1.8   SP 	Support trail points.
 *   24/11/20 1.7   SP 	Do not correct for reference altitude.
 *   13/11/20 1.6   SP 	Rename class.
 *   12/11/20 1.5   SP 	Use GetSymbol() in base class.
 *   27/10/20 1.4   SP 	Split graphics out into new renderer.
 *   26/10/20 1.3   SP 	Improvements to axes and markers.
 *   23/10/20 1.2   SP 	Further development.
 *   22/10/20 1.1   SP 	Initial version.
 *
 **********************************************************************/

#ifndef _SPX_RHI_TRACK_RENDERER_H
#define _SPX_RHI_TRACK_RENDERER_H

/*
 * Other headers required.
 */

/* We need SPxLibUtils for common types, callbacks, errors etc. */
#include "SPxLibUtils/SPxAutoPtr.h"
#include "SPxLibUtils/SPxCriticalSection.h"

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

/* Need to forward-declare other classes we need.*/
class SPxTrackDatabase;
class SPxRadarTrack;
class SPxRhi;

/* Need to forward-declare other structures we need.*/
struct _SPxRadarTrackTrailPoint;

/*
 * Define our class, derived from the abstract renderer base class.
 */
class SPxRhiTrackRenderer : virtual public SPxTrackRendererBase
{
public:
    /*
     * Public variables.
     */

    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    SPxRhiTrackRenderer(SPxRhi *view, SPxTrackDatabase *trackDB);
    virtual ~SPxRhiTrackRenderer(void);

    /* Utility. */
    UINT32 GetNearestTrack(int xPixels, int yPixels, 
                           unsigned int *distPixelsRtn) const;

    /* Features. */
    SPxErrorCode SetDirectionEnabled(int isEnabled);
    int IsDirectionEnabled(void) { return m_isDirectionEnabled; }

    /* Mouse handling. */
    virtual int HandleMousePress(UINT32 flags, int x, int y, SPxCursor_t *cursor);
    virtual int HandleMouseRelease(UINT32 flags, int x, int y, SPxCursor_t *cursor);
    virtual int HandleDoubleClick(UINT32 flags, int x, int y, SPxCursor_t *cursor);
    virtual int HandleMouseMove(UINT32 flags, int x, int y, SPxCursor_t *cursor);
    virtual int HandleMouseWheel(UINT32 flags, int x, int y, double delta);
    virtual int HandleMouseLeave(void);

    /* Generic parameter assignment. */
    virtual int SetParameter(char *name, char *value);
    virtual int GetParameter(char *name, char *valueBuf, int bufLen);

protected:
    /*
     * Protected functions.
     */

private:

    /*
     * Private types.
     */

    /*
     * Private variables.
     */

    struct impl;
    SPxAutoPtr<impl> m_p;

    /* General. */
    SPxTrackDatabase *m_trackDB;
    SPxRhi *m_view;
    mutable SPxCriticalSection m_mutex;
    int m_isDirectionEnabled;
    double m_prevTrailX;
    double m_prevTrailY;

    /*
     * Private functions.
     */

    /* Rendering functions. */
    SPxErrorCode renderAll(void);
    void renderTracks(void);
    void renderTrack(SPxRadarTrack *track);
    void renderTrailPoint(const _SPxRadarTrackTrailPoint *trailPt);

    /* Misc utility functions. */
    SPxErrorCode getXYForTrack(const SPxRadarTrack *track,
                               double *xPixelsRtn, 
                               double *yPixelsRtn,
                               double *rangeMetresRtn=NULL,
                               double *bearingDegsRtn=NULL,
                               double *altMetresRtn=NULL) const;

    SPxErrorCode getXYForTrailPoint(const _SPxRadarTrackTrailPoint *trailPt,
                                    double *xPixelsRtn, 
                                    double *yPixelsRtn) const;

    void getTrackLabel(SPxRadarTrack *track,
                       char *buffer,
                       unsigned int bufferSizeBytes);

    /*
     * Private static functions.
     */

    static int staticRenderTrack(void *invokingObj,
                                 void *userObj,
                                 void *arg);

    static int staticRenderTrailPoint(void *invokingObj,
                                      void *userObj,
                                      void *arg);

}; /* SPxRhiTrackRenderer */


/*********************************************************************
 *
 *   Function prototypes
 *
 **********************************************************************/

#endif /* _SPX_RHI_TRACK_RENDERER_H */

/*********************************************************************
 *
 * End of file
 *
 **********************************************************************/

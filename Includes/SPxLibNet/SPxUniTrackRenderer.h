/*********************************************************************
 *
 * (c) Copyright 2017 - 2020, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   Header for SPxUniTrackRenderer class which is used to render
 *   radar, fused, AIS and ADS-B tracks from a unified track database.
 *
 * Revision Control:
 *   18/09/20 v1.18   AGC       Support showing excluded tracks.
 *
 * Previous Changes:
 *   07/08/20 1.17  SP 	Add IsTrackVisible().
 *   19/12/19 1.16  BTB	Support configuring track type preference for GetBestTrack().
 *   01/07/19 1.15  AGC	Add functions to retrieve renderers.
 *   03/06/19 1.14  AGC	Fix track label move interaction.
 *   11/01/19 1.13  BTB	Fixes to track caption mouse hovering.
 *			Make track caption border colour configurable.
 *   10/01/19 1.12  BTB	Support showing track caption at mouse position.
 *   09/01/19 1.11  BTB	Make drag/drop caption text box colours configurable.
 *   21/08/18 1.10  REW	Support MaxClickPixels parameter.
 *   07/06/18 1.9   BTB	Improve mouse click handling.
 *   24/05/18 1.8   REW	Allow drag/drop to be disabled.
 *   29/03/18 1.7   SP 	Add isTrackVisible().
 *   20/11/17 1.6   AGC	Support flashing targets.
 *   06/10/17 1.5   SP 	Support drag and drop.
 *   22/08/17 1.4   SP 	Add support for fused tracks.
 *                      Improvements to track selection.
 *   03/08/17 1.3   SP 	Add GetNearestTrack().
 *   31/07/17 1.2   SP 	Set state when selecting tracks.
 *   13/07/17 1.1   SP 	Initial version.
 *
 *********************************************************************/

#ifndef _SPX_UNI_TRACK_RENDERER_H
#define _SPX_UNI_TRACK_RENDERER_H

/*
 * Other headers required.
 */

/* Include common types, callbacks, errors etc. */
#include "SPxLibUtils/SPxObj.h"
#include "SPxLibUtils/SPxError.h"
#include "SPxLibUtils/SPxMouseControl.h"
#include "SPxLibUtils/SPxRenderer.h"

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
class SPxUniTrack;
class SPxUniTrackDatabase;
class SPxTrackRenderer;
class SPxAISRenderer;
class SPxADSBRenderer;

/*
 * Define our class.
 */
class SPxUniTrackRenderer : virtual public SPxObj, public SPxMouseInterface
{
public:
    /*
     * Public variables.
     */

    /*
     * Public functions.
     */

    /* Constructor and destructor. */
    SPxUniTrackRenderer(SPxUniTrackDatabase *database);
    virtual ~SPxUniTrackRenderer(void);

    /* Add track renderers. */
    virtual SPxErrorCode AddRadarTrackRenderer(SPxTrackRenderer *renderer);
    virtual SPxErrorCode AddFusedTrackRenderer(SPxTrackRenderer *renderer);
    virtual SPxErrorCode AddAISTrackRenderer(SPxAISRenderer *renderer);
    virtual SPxErrorCode AddADSBTrackRenderer(SPxADSBRenderer *renderer);
    virtual unsigned int GetNumTrackRenderers(void) const;
    virtual unsigned int GetNumAISTrackRenderers(void) const;
    virtual unsigned int GetNumADSBTrackRenderers(void) const;
    virtual SPxTrackRenderer *GetTrackRenderer(unsigned int index);
    virtual SPxAISRenderer *GetAISTrackRenderer(unsigned int index);
    virtual SPxADSBRenderer *GetADSBTrackRenderer(unsigned int index);

    /* Utility functions. */
    virtual SPxUniTrack *GetNearestTrack(int x, int y);
    virtual SPxUniTrack *GetBestTrack(int x, int y);
    virtual int IsTrackVisible(SPxUniTrack *track);

    /* Configuration. */
    virtual void SetAllowDragDrop(int allow)	{ m_allowDragDrop = allow; }
    virtual int GetAllowDragDrop(void)		{ return(m_allowDragDrop); }
    virtual void SetShowTrackCaption(int show) { m_showTrackCaption = show; }
    virtual int GetShowTrackCaption(void) { return(m_showTrackCaption); }
    virtual void SetTrackCaptionBorderColour(UINT32 col);
    virtual void SetTrackCaptionAlarmBorderColour(UINT32 col);
    virtual void SetMaxClickPixels(unsigned int px) { m_maxClickPixels = px;}
    virtual unsigned int GetMaxClickPixels(void) const { return(m_maxClickPixels); }
    virtual void SetCaptionColours(UINT32 titleCol, UINT32 bodyCol);
    virtual void SetPreferPrimaryTracks(int prefer) { m_preferPrimaryTracks = prefer; }
    virtual int GetPreferPrimaryTracks(void) { return(m_preferPrimaryTracks); }

    /* Rendering. */
    virtual SPxErrorCode Render(void);

    /* Target flash support. */
    SPxErrorCode SetHighlightFlashHz(unsigned int hz);
    unsigned int GetHighlightFlashHz(void) const { return(m_highlightFlashHz); }

    /* Excluded tracks. */
    SPxErrorCode SetShowExcludedTracks(int state);
    int GetShowExcludedTracks(void) const { return m_showExcludedTracks; }

    /* Functions used to implement SPxMouseInterface. */
    virtual int HandleMousePress(UINT32 flags, int x, int y, SPxCursor_t *cursor);
    virtual int HandleMouseRelease(UINT32 flags, int x, int y, SPxCursor_t *cursor);
    virtual int HandleDoubleClick(UINT32 flags, int x, int y, SPxCursor_t *cursor);
    virtual int HandleMouseMove(UINT32 flags, int x, int y, SPxCursor_t *cursor);
    virtual int HandleMouseLeave(void);
    virtual SPxObj *HandleDragDropQueryStart(UINT32 flags, int x, int y);
    virtual void HandleDragDropStart(UINT32 flags, int x, int y, SPxObj *dragObj);
    virtual void HandleDragDropMove(UINT32 flags, int x, int y, SPxObj *dragObj, SPxObj *dropObj);
    virtual SPxObj *HandleDragDropQueryEnd(UINT32 flags, int x, int y, SPxObj *dragObj);
    virtual void HandleDragDropEnd(UINT32 flags, int x, int y, SPxObj *dragObj, SPxObj *dropObj);

protected:
    /*
     * Protected functions.
     */

    /* Generic parameter assignment. */
    virtual int SetParameter(char *parameterName,
                             char *parameterValue);

    virtual int GetParameter(char *parameterName,
                             char *valueBuf, 
                             int bufLen);

private:
    /*
     * Private variables.
     */

    struct impl;
    SPxAutoPtr<impl> m_p;

    mutable SPxCriticalSection m_mutex;     /* Mutex used to protect the various lists. */
    SPxUniTrackDatabase *m_database;        /* Handle of associated database object. */
    SPxRenderer *m_rdr;                     /* Primary track render. */
    SPxRendererXY_t m_cursorPos;            /* Mouse cursor position. */
    SPxRendererXY_t m_pressPos;             /* Mouse press position. */
    SPxUniTrack *m_dragTrack;               /* Track being dragged for drag and drop. */
    SPxUniTrack *m_captionTrack;	    /* Track at mouse location to show a caption for. */
    SPxObj *m_dropObj;                      /* Object on to which track may be dropped. */
    unsigned int m_highlightFlashHz;        /* Target flash support. */
    int m_showExcludedTracks;               /* Exclude tracks. */
    int m_allowDragDrop;		    /* Do we support it? */
    int m_showTrackCaption;		    /* Are we showing track captions at the mouse location? */
    int m_clickWasDouble;		    /* Was the most recent mouse press a double-click? */
    unsigned int m_maxClickPixels;	    /* Max distance in pixels to select track. */
    int m_preferPrimaryTracks;              /* Prefer primary tracks to secondary for GetBestTrack (and selection). */

    /*
     * Private functions.
     */

    void setCursor(int x, int y, SPxCursor_t *cursor);
    int selectTrack(UINT32 flags, 
                    int x, int y,
                    int isDoubleClick);
    void requestRedraw(void);
    int isTrackVisible(SPxUniTrack *track);
    void checkIsMouseOverTrack(int x, int y);
    void showTrackCaption(void);

}; /* SPxUniTrackRenderer */


/*********************************************************************
 *
 *   Function prototypes
 *
 *********************************************************************/

#endif /* SPX_UNI_TRACK_RENDERER_H */

/*********************************************************************
 *
 * End of file
 *
 *********************************************************************/

/*********************************************************************
 *
 * (c) Copyright 2015, Cambridge Pixel Ltd.
 *
 * File: $RCSfile: SPxRecordRenderer.h,v $
 * ID: $Id: SPxRecordRenderer.h,v 1.3 2015/02/16 15:18:35 rew Exp $
 *
 * Purpose:
 *   Header for SPxRecordRenderer base class which supports the display
 *   of records in a PPI window, but only via a derived class.
 *
 * Revision Control:
 *   16/02/15 v1.3    SP     Fix cursor and selection issues.
 *                           Change constructor args.
 *
 * Previous Changes:
 *   05/02/15 1.2    SP     Support changes to SPxTimelineDatabase.
 *   07/01/15 1.1    SP     Initial version.
 *
 **********************************************************************/

#ifndef _SPX_TIMELINE_PPI_RENDERER_H
#define _SPX_TIMELINE_PPI_RENDERER_H

/*
 * Other headers required.
 */

/* We need SPxLibUtils for common types, callbacks, errors etc. */
#include "SPxLibUtils/SPxAutoPtr.h"
#include "SPxLibUtils/SPxCriticalSection.h"

/* We need our base class. */
#include "SPxLibUtils/SPxRenderer.h"

/* Other classes. */
#include "SPxLibUtils/SPxTimelineDatabase.h"
#include "SPxLibUtils/SPxStampDatabase.h"

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
class SPxTimelineRenderer;

/*
 * Define our class, derived from the abstract renderer base class.
 */
class SPxRecordRenderer : virtual public SPxRenderer
{
public:
    /*
     * Public variables.
     */

    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    SPxRecordRenderer(SPxTimelineDatabase *timelineDB,
                      SPxStampDatabase *stampDB);
    virtual ~SPxRecordRenderer(void);

    /* Mouse handling. */
    virtual int HandleMousePress(UINT32 flags, int x, int y, 
                                 SPxCursor_t *cursor);
    virtual int HandleMouseRelease(UINT32 flags, int x, int y, 
                                   SPxCursor_t *cursor);
    virtual int HandleDoubleClick(UINT32 flags, int x, int y, 
                                  SPxCursor_t *cursor);
    virtual int HandleMouseMove(UINT32 flags, int x, int y, 
                                SPxCursor_t *cursor);
    virtual int HandleMouseLeave(void);

    /* Generic parameter assignment. */
    virtual int SetParameter(char *parameterName, char *parameterValue);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);

protected:
    /*
     * Protected functions.
     */

private:

    /* Info for each context menu entry. */
    typedef struct MenuEntryInfo_tag
    {
        SPxRecordRenderer *obj;     /* This object. */
        unsigned int stampID;       /* Stamp ID. */
        MenuEntryInfo_tag *next;    /* Next in list or NULL. */

    } MenuEntryInfo_t;

    /*
     * Private variables.
     */

    /* Handles of associated database objects. */
    SPxTimelineDatabase *m_timelineDB;
    SPxStampDatabase *m_stampDB;

    /* General. */
    MenuEntryInfo_t *m_menuInfoHead;     /* Context menu info list head. */
    int m_cursorXPixels;                 /* Last cursor X in pixels. */
    int m_cursorYPixels;                 /* Last cursor Y in pixels. */
    int m_isCentreOnRecordEnabled;       /* Allow centre on record? */
    SPxCursor_t m_currentCursor;         /* Current cursor to display. */

    /*
     * Private functions.
     */

    /* Rendering functions. */
    SPxErrorCode renderAll(void);
    void renderRecords(void);
    void renderRecord(const SPxPacketRecord *rec);
    void cleanUpContextMenu(void);
    void processTimelineEvent(const SPxTimelineDatabase::Event_t *evt);
    UINT32 getRecordAtCursor(int xPixels, int yPixels);
    
    /*
     * Private static functions.
     */

    static int timelineEventHandler(void *invokingObjPtr,
                                    void *userObjPtr,
                                    void *eventPtr);

    static int recordIterator(void *timelineDBVoid,
                                  void *objVoid,
                                  void *recordVoid);
    
    static void contextMenuHandler(SPxPopupMenu *menu,
                                   int id, void *userArg);

    static int addStampToMenuIterator(void *stampDBVoid, 
                                      void *objVoid, 
                                      void *stampVoid);

        
}; /* SPxRecordRenderer */


/*********************************************************************
 *
 *   Function prototypes
 *
 **********************************************************************/

#endif /* SPX_TIMELINE_PPI_RENDERER_H */

/*********************************************************************
 *
 * End of file
 *
 **********************************************************************/

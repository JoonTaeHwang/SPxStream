/*********************************************************************
* (c) Copyright 2007 - 2010, 2015 - 2016, 2019 Cambridge Pixel Ltd.
*
* Purpose:
*   Header for Target DataBase class
*
* Revision Control:
*   14/01/19 v1.11   REW	Fix build issue in last change.
*
* Previous Changes:
*   14/01/19 1.10   DGJ	New function to leave iterator
*   23/09/16 1.9    AGC	Improve mutex behaviour.
*   24/06/16 1.8    DGJ	Add GetTargetCapacity() funtion
*   13/11/15 1.7    REW	Include required SPxCriticalSection header.
*   15/09/10 1.6    REW	Make destructor virtual.
*   18/06/10 1.5    SP 	Derive SPxTargetDB class from SPxObj.
*   24/08/09 1.4    DGJ	Add user argument to notification (API change)
*   07/01/08 1.3    DGJ	Iteration critical section.
*   03/01/08 1.2    DGJ	General tidy-up and support target deletion
*   08/10/07 1.1    DGJ	First version
**********************************************************************/

#ifndef _SPX_TARGETDB_H
#define _SPX_TARGETDB_H

/* Supporting headers */
#include "SPxLibUtils/SPxObj.h"
#include "SPxLibUtils/SPxCriticalSection.h"

/* Notification masks. */
#define SPX_TDB_EVENT_CLEAR_SECTOR 0x1

/* The default number of entries in the database. Can be overridden in 
 * the constructor as needed.
 */
const int SPX_DEFAULT_MAX_NUM_TRACKS = 1000;

class SPxTargetDB;
class SPxTargetDBnotify;
class SPxTargetRecordLink;

/* Class to store target notification details */
class SPxTargetDBnotify
{
friend class SPxTargetDB;
    SPxTargetDB *m_db;		    /* The target database */
    SPxTargetDBnotify * m_next;	    /* Next notification */
    UINT32 m_event;		    /* The event for this notification */
    void SetNext(SPxTargetDBnotify * next) {m_next = next;}
    void *m_userData;		    /* User-specified pointer */
public:
    void (*m_fn)(SPxTargetDB *db, UINT32 event,
			void *eventData1, int eventData2, void *userData);
				    /* The user-function to invoke */
    SPxTargetDB *GetDB(void) {return m_db;}
				    /* The target database */
    UINT32 GetEvent(void) {return m_event;}
				    /* The event */
    SPxTargetDBnotify *GetNext(void) {return m_next;}
				    /* Next in the linked list */

    /* Constructor */
    SPxTargetDBnotify(SPxTargetDB *, UINT32 event, 
			void (*fn)(SPxTargetDB *db, UINT32 event, 
			    void *eventData1, int eventData2, void *userData),
			void *userData=NULL);
};

void SPxTrackDBTest(void);

/* Define a class that is used to store a target link structure. The target
 * database is constructed from a linked list of SPxTargetRecordLink objects.
 */
class SPxTargetRecordLink
{
public:
    /* Is this link in use ? */
    int m_isUsed;    

    /* The track that this link managed. This is just a pointer to a piece
     * of memory that is allocated from a free store managed by the target
     * database.
     */
    char *m_target;

    /* The sector number (0..N-1) */
    int m_sectorNumber;

    /* Links to next and previous. */
    SPxTargetRecordLink *m_next;
    SPxTargetRecordLink *m_prev;    

    SPxTargetRecordLink(void);
};



/* The TargetDatabase */
class SPxTargetDB : public SPxObj
{
    /* Access control. */
    SPxCriticalSection m_criticalSection;
    SPxCriticalSection m_iteratorCriticalSection;

    /* The number of sectors for track processing. */
    int m_numSectors;

    /* The current space available in the free store for tracks. */
    int m_freeStoreSize;

    /* The memory allocated to store the tracks. */
    char *m_targetStore;

    /* The initial size of the free store. */
    int m_freeStoreInitialSize;

    /* The initial start of the free list */
    SPxTargetRecordLink *m_freeList; 

    /* The start of the free list */
    SPxTargetRecordLink *m_nextFree; 

    /* The array of per-sector lists of track records. */   
    SPxTargetRecordLink **m_sectorList;

    SPxTargetRecordLink *m_iterator;

    /* The number of new tracks being maintained. */
    int *m_targetCount;

    /* The size of a track entry in bytes. */
    int m_itemSize;

    /* Get a new track record from the free list */
    SPxTargetRecordLink *AllocFree(void);
    
    /* Add a new link for the specified sector. */
    int AddTargetLink(SPxTargetRecordLink *targetLink, int sector);

    /* Remove a target.   */
    void RemoveTargetLink(SPxTargetRecordLink *targetLink);

    void ReturnToFreeStore(SPxTargetRecordLink *targetLink);

    SPxTargetDBnotify *m_notify;
public:

    /* Constructor for a new Track DataBase. Make one with a number of
     * sectors, the size of the objects (in bytes) being managed by the
     * database, and the size of the database as the number of objects.
     */
    SPxTargetDB(int numSectors, int dataSize,
				int size = SPX_DEFAULT_MAX_NUM_TRACKS);

    /* Destructor. */
    virtual ~SPxTargetDB(void);

    /* Notification routines. */
    SPxTargetDBnotify *AddNotify(UINT32 event, 
				void (*fn)(SPxTargetDB *db, UINT32 event,
					void *eventData1, int eventData2,
					void *userData),
				void *userData = NULL);
    void RemoveNotify(SPxTargetDBnotify *notify);
    void CheckNotifiers(UINT32 event, void *userData1, int userData2);

    /* Delete a Target. */
    void RemoveTarget(SPxTargetRecordLink *targetRef);

    /* Clear a sector of all targets of a specified type. */
    int ClearSector(int sector);

    /* Add a new target and return the Link structure */
    SPxTargetRecordLink *AddTarget(void *track, int sector);

    /* Retrieve a target from the specified sector. This function returns
     * 1 if a target pointer was assigned and 0 if it wasn't, eg the sector
     * is empty. Note that this function removes the first track from the
     * specified sector or the database.
     */
    int GetTarget(int sector, char *target);

    /* Check out a pointer to a target in the database. The caller may
     * change the target then call ReturnTargetRef() when the track is
     * finished. Call this function with the trackPtr argument set to NULL
     * to initialise the iterator. Subsequent calls will then return the
     * tracks in the sector. It's Ok to call RemoveTarget on a target
     * returned from the last call, but don't hold the links and call
     * RemoveTarget sometime later. 
     */
    SPxTargetRecordLink *GetTargetRef(int sector, void **trackPtr);

    /* Leave iterator early */
    void LeaveIterator(void);

    /* Return a target reference back to the database, possibly changing
     * the sector that contains the target.
     */
    int ReturnTargetRef(SPxTargetRecordLink *targetLink, int newSector);

    /* The number of targets in the sector. */
    int GetTargetCount(int sector);

    /* Return the number of sectors. */
    int GetNumSectors(void);

    /* Return the total number of tracks being managed. */
    int GetTotalTargetCount(void);

    /* Return the capacity of the database - maximum number of plots */
    int GetTargetCapacity(void);
};

#endif /* _TARGETDB */

/*********************************************************************
*
* (c) Copyright 2007 - 2017, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxPimManager.h,v $
* ID: $Id: SPxPimManager.h,v 1.9 2017/02/27 16:17:56 rew Exp $
*
* Purpose:
*   Header file for SPxPIM class and associated code.
*
* Revision Control:
*   27/02/17 v1.9    AGC	Add WaitForNotifications() for testing.
*
* Previous Changes:
*   04/10/13 1.8    AGC	Simplify headers.
*   14/08/13 1.7    REW	Forward declare SPxThread.
*   15/09/10 1.6    REW	Make destructor virtual.
*   22/09/07 1.5    DGJ	Support GetThread() call
*   29/08/07 1.4    DGJ	Support GetFirstPim() API call
*   29/06/07 1.3    DGJ	Derive from SPxObj
*   08/06/07 1.2    REW	Protect linked list with critical section.
*   07/06/07 1.1    REW	Initial Version.
**********************************************************************/

#ifndef _SPX_PIM_MANAGER_H
#define _SPX_PIM_MANAGER_H

/*
 * Other headers required.
 */
/* We need SPXxLibUtils for common types, events, errors etc. */
#include "SPxLibUtils/SPxCriticalSection.h"
#include "SPxLibUtils/SPxError.h"
#include "SPxLibUtils/SPxObj.h"
#include "SPxLibData/SPxRib.h"

/*********************************************************************
*
*   Constants
*
**********************************************************************/

/* Size of buffer we'll work in. */
#define	SPX_PIM_MGR_BUFSIZE	(65536 + sizeof(SPxReturnHeader))


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

/* Forward define the classes we need. */
class SPxRIB;
class SPxPIM;
class SPxThread;

/*
 * Define the PimManager class.
 */
class SPxPimManager :public SPxObj
{
public:
    /*
     * Public fields.
     */

    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    SPxPimManager(SPxRIB *rib);
    virtual ~SPxPimManager(void);

    /* Addition and removal functions. */
    SPxErrorCode AddPim(SPxPIM *pim);
    SPxErrorCode RemovePim(SPxPIM *pim);

    /* Wait until RIB notifications processed. */
    void WaitForNotifications(void);

    /* Thread function. */
    void *ThreadFunc(SPxThread *thread);
    
    SPxPIM *GetFirstPIM(void) {return m_firstPIM;}
    SPxThread* GetThread(void)	{return m_thread;}

private:
    /*
     * Private fields.
     */
    /* Linked list of PIMs being managed by this object. */
    SPxPIM *m_firstPIM;	/* First PIM in the linked list */
    SPxPIM *m_lastPIM;	/* Last PIM in the linked list */
    SPxCriticalSection m_criticalSection;	/* To protect list */

    /* Number of PIMs being managed. */
    int m_numPIMs;	/* Number of PIMs connected */

    /* Worker thread that runs the main loop for the manager. */
    SPxThread *m_thread;

    /* Pointer to RIB feeding these PIMs. */
    SPxRIB *m_rib;

    /* Buffer for working in. */
    unsigned char m_buf[SPX_PIM_MGR_BUFSIZE];

    /* Number of times RIB data has been read. */
    SPxAtomic<UINT32> m_count;

    /*
     * Private functions.
     */
    void processRibData(void);

}; /* SPxPimManager */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/


#endif /* _SPX_PIM_MANAGER_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/

/*********************************************************************
*
* (c) Copyright 2007, 2008, 2010, 2017, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxProcMan.h,v $
* ID: $Id: SPxProcMan.h,v 1.11 2017/03/08 14:43:07 rew Exp $
*
* Purpose:
*   Header for Process Manager class.
*
* Revision Control:
*   08/03/17 v1.11   AGC	Add Add/RemoveRunProcessTo/FromOutput() functions.
*
* Previous Changes:
*   15/09/10 1.10   REW	Make destructor virtual.
*   21/10/08 1.9    DGJ	Provide function to return access mutex.
*   29/10/07 1.8    REW	Add m_mutex, make recursing functions private.
*   12/09/07 1.7    REW	Add NotifyChanges().
*   22/08/07 1.6    DGJ	Add GetFirstRunProcess
*   29/06/07 1.5    DGJ	Derive from SPxObj
*   12/06/07 1.4    REW	Big rework. Separate out the classes.
*   07/06/07 1.3    REW	Fix use of 'default' as param name.
*   24/05/07 1.2    REW	Added header, changed to SPx etc.
*   20/05/07 1.1    DGJ	Initial Version
**********************************************************************/

#ifndef _SPX_PROCMAN_H
#define _SPX_PROCMAN_H

/*
 * Other headers required.
 */
#include "SPxLibData/SPxProcess.h"	 /* Need processes to manage */

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

/* Forward define things as necessary. */
class SPxRunProcess;
class SPxPIM;


/*
 * Class that holds a linked list of processes and manages them.
 */
class SPxProcessManager :public SPxObj
{
public:
    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    SPxProcessManager(void);
    virtual ~SPxProcessManager(void);

    /* Addition/removal of SPxRunProcess objects. */
    void AddNewRunProcess(SPxRunProcess *rp);
    void RemoveRunProcess(SPxRunProcess *rpToRemove);

    void AddRunProcessToOutput(SPxRunProcess *rp, SPxRunProcess *rpToAdd,
			       int (SPxRunProcess::*fn)(SPxRunProcess *));
    void RemoveRunProcessFromOutput(SPxRunProcess *rp, SPxRunProcess *rpToRemove,
				    void (SPxRunProcess::*fn)(SPxRunProcess *));

    /* Function through which a PIM tells us to do more work.  See also
     * the internal variant of this function (below).
     */
    void SignalProcessManager(SPxPIM *pim, 
				SPxReturn *firstNewReturn, 
				unsigned firstNewAzimuthInPim,
				unsigned numberOfAzimuths);

    /* Function through which we are told of source changes.  See also
     * the internal variant of this function (below).
     */
    int NotifyChanges(SPxPIM *pim, UINT32 flags);

    /* Function to retrieve the first run-process we have. */
    SPxRunProcess *GetFirstRunProcess(void) {return m_firstRunProcess;}

    /* Return access mutex */
    SPxCriticalSection *GetAccess(void) {return &m_mutex;}

private:
    /*
     * Private fields.
     */
    /* Handles for list of processes etc. */
    SPxRunProcess *m_firstRunProcess;
    SPxRunProcess *m_lastRunProcess;
    unsigned m_numProcesses;

    /* Mutex for protection of lists. */
    SPxCriticalSection m_mutex;

    /*
     * Private functions.
     */
    /* Internal functions for recursing.  See public variants above.  */
    void signalProcessManagerInternal(SPxRunProcess *proc, 
				SPxReturn *newReturn,
				unsigned firstNewAzimuthInPim,
				unsigned numberOfNewAzimuths);
    int notifyChangesInternal(SPxRunProcess *runProcess, UINT32 flags);
};

/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* _SPX_PROCMAN_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/

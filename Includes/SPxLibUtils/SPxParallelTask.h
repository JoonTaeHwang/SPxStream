/*********************************************************************
*
* (c) Copyright 2015, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxParallelTask.h,v $
* ID: $Id: SPxParallelTask.h,v 1.1 2015/04/29 13:12:19 rew Exp $
*
* Purpose:
*	Header for SPxParallelTask that splits a task across
*	a number of threads.
*
* Revision Control:
*   29/04/15 v1.1    AGC	Initial version.
*
* Previous Changes:
**********************************************************************/
#ifndef _SPX_PARALLEL_TASK_H
#define _SPX_PARALLEL_TASK_H

/* For SPxAutoPtr. */
#include "SPxLibUtils/SPxAutoPtr.h"

/* For SPxSharedPtr. */
#include "SPxLibUtils/SPxSharedPtr.h"

/* For SPxEventHandler. */
#include "SPxLibUtils/SPxEventHandler.h"

/* For base class. */
#include "SPxLibUtils/SPxObj.h"

/* Forward declarations. */

/* Define our class. */
class SPxParallelTask : public SPxObj
{
public:
    typedef SPxErrorCode (*Func)(unsigned int index);

    SPxParallelTask(void);
    explicit SPxParallelTask(unsigned int numCores);
    virtual ~SPxParallelTask(void);

    unsigned int GetNumThreads(void) const;

    template<typename F, typename O>
    SPxErrorCode Run(F fn, O *obj)
    {
	SPxSharedPtr<SPxEventHandler<Func> > handler = SPxMakeShared<SPxEventHandler<Func> >();
	handler->Add(fn, obj);
	return run(handler);
    }

private:
    struct impl;
    SPxAutoPtr<impl> m_p;

    SPxErrorCode run(const SPxSharedPtr<SPxEventHandler<Func> >& handler);
};

#endif /* _SPX_PARALLEL_TASK_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/

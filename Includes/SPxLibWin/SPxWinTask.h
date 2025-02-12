/*********************************************************************
*
* (c) Copyright 2014, 2016, 2018, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPx MFC task class.
*
*	This class allows time-consuming work to be run off the GUI
*	thread. Further completiong work can then be run ON the GUI
*	thread once the time-consuming work is complete.
*
*	Add the SPxWinTask to the MFC class:
*
*	    class Example : public CWnd
*	    {
*	    public:
*		Example();
*		virtual ~Example();
*
*	    private:
*		void invoke(void);
*		void fnThread(void);
*		void fnComplete(void);
*		SPxWinTask m_task;
*	    }
*
*	To start a new task, simply create an SPxWinTask object
*	passing it the required member functions to run:
*
*	    void Example::invoke(void)
*	    {
*		m_task(this, &Example::fnThread, &Example::fnComplete);
*	    }
*
*	The SPxWinTask object will run the fnThread function on a new thread,
*	and then the fnComplete function on the GUI thread.
*
* Revision Control:
*   25/07/22 v1.6    AGC	Add Start() functions.
*
* Previous Changes:
*   10/12/18 1.5    AGC	Allow SPxNullPtr task functions.
*   01/02/16 1.4    AGC	Run tasks using SPxAsync.
*   22/01/16 1.3    AGC	Support installing only completion function.
*   20/06/14 1.2    AGC	Add Stop() function.
*   17/02/14 1.1    AGC	Initial Version.
**********************************************************************/
#pragma once

#define SPX_AFXWIN
#include "SPxLibUtils/SPxCommon.h"

/* For SPxAutoPtr class. */
#include "SPxLibUtils/SPxAutoPtr.h"

/* For SPxErrorCode enumeration. */
#include "SPxLibUtils/SPxError.h"

/* For SPxEventHandler class. */
#include "SPxLibUtils/SPxEventHandler.h"

/* For SPxSharedPtr class. */
#include "SPxLibUtils/SPxSharedPtr.h"

/* Forward declarations. */
class SPxThread;

/* Class used to start tasks. */
class SPxWinTask : public CWnd
{
public:
    /* Function type. */
    typedef SPxErrorCode (*HandlerType)(void);

    /* Public functions. */
    SPxWinTask(void);
    virtual ~SPxWinTask(void);
    void Stop(void);

    template<typename Obj, typename FuncThread, typename FuncComplete>
    void Start(Obj *obj, FuncThread funcThread, FuncComplete funcComplete)
    {
	this->operator ()(obj, funcThread, funcComplete);
    }

    template<typename Obj, typename FuncComplete>
    void Start(Obj *obj, SPxNullPtr_t, FuncComplete funcComplete)
    {
	this->operator ()(obj, SPxNullPtr, funcComplete);
    }

    template<typename Obj, typename FuncThread>
    void Start(Obj *obj, FuncThread funcThread, SPxNullPtr_t)
    {
	this->operator ()(obj, funcThread, SPxNullPtr);
    }

    template<typename Obj, typename FuncComplete>
    void Start(Obj *obj, FuncComplete funcComplete)
    {
	this->operator ()(obj, funcComplete);
    }

    template<typename Obj, typename FuncThread, typename FuncComplete>
    void operator()(Obj *obj, FuncThread funcThread, FuncComplete funcComplete)
    {
	SPxSharedPtr<Task> task = SPxMakeShared<Task>();
	task->funcThread.Add(funcThread, obj);
	task->funcComplete.Add(funcComplete, obj);
	start(task);
    }

    template<typename Obj, typename FuncComplete>
    void operator()(Obj *obj, SPxNullPtr_t, FuncComplete funcComplete)
    {
	SPxSharedPtr<Task> task = SPxMakeShared<Task>();
	task->funcComplete.Add(funcComplete, obj);
	start(task);
    }

    template<typename Obj, typename FuncThread>
    void operator()(Obj *obj, FuncThread funcThread, SPxNullPtr_t)
    {
	SPxSharedPtr<Task> task = SPxMakeShared<Task>();
	task->funcThread.Add(funcThread, obj);
	start(task);
    }

    template<typename Obj, typename FuncComplete>
    void operator()(Obj *obj, FuncComplete funcComplete)
    {
	SPxSharedPtr<Task> task = SPxMakeShared<Task>();
	if(funcComplete)
	{
	    task->funcComplete.Add(funcComplete, obj);
	}
	start(task);
    }

protected:
    DECLARE_MESSAGE_MAP()

private:
    /* Private types. */
    struct Task
    {
	Task()
	    : funcThread(), funcComplete(), funcThreadDone(FALSE)
	{}

	SPxEventHandler<SPxWinTask::HandlerType> funcThread;
	SPxEventHandler<SPxWinTask::HandlerType> funcComplete;
	int funcThreadDone;
    };

    /* Private variables. */
    struct impl;
    SPxAutoPtr<impl> m_p;

    /* Private functions. */
    void start(SPxSharedPtr<Task> task);
    void taskFn(SPxSharedPtr<Task> task);
    LRESULT OnWinTaskComplete(WPARAM wParam, LPARAM lParam);
    unsigned int getNumTasks(void) const;

}; /* SPxWinTask. */

/*********************************************************************
*
* End of file
*
**********************************************************************/

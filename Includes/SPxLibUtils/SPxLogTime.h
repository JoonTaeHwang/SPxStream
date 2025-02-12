/*********************************************************************
*
* (c) Copyright 2011, 2012, 2017, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxLogTime.h,v $
* ID: $Id: SPxLogTime.h,v 1.6 2017/05/09 13:23:46 rew Exp $
*
* Purpose:
*   Header file for logging the time taken for some action of series
*   of actions.
*
* Use:
*   Add this class as a member object (not pointer) to the class you
*   wish to debug e.g. "SPxLogTime m_timer;"
*   To start a timer call Start() with a name to use in the print 
*   statement.
*   To stop a timer call Stop() and a print statement will be written
*   in the form "<name>: <time in ms>\n".
*   Calls to Start() and Stop() may be nested.
*
*   The script "parse-log-time.py", in the Scripts directory, may be
*   used to parse lots of messages printing using this class with
*   the command "python parse-log-time.py <log file>".
*
* Revision Control:
*   09/05/17 v1.6    AGC	Fix cppcheck warnings.
*
* Previous Changes:
*   01/10/12 1.5    AGC	Move implementation to cpp file.
*   17/11/11 1.4    AGC	Add private copy constructor and assigment operator.
*   27/06/11 1.3    AGC	Allow writing to log file instead of stdout.
*   24/01/11 1.2    AGC	Fix warning and add Clear() function.
*   24/01/11 1.1    AGC	Initial Version.
**********************************************************************/

#ifndef _SPX_LOG_TIME_H
#define _SPX_LOG_TIME_H

/* For SPxAutoPtr. */
#include "SPxLibUtils/SPxAutoPtr.h"

/* For SPX_FUNCTION_DELETE. */
#include "SPxLibUtils/SPxTypes.h"

/*********************************************************************
*
*   Constants
*
**********************************************************************/

/*********************************************************************
*
*   Class
*
**********************************************************************/

/* Forward declarations. */
struct SPxLogTimePriv;

class SPxLogTime
{
public:
    /* Public functions. */
    SPxLogTime(void);
    ~SPxLogTime(void);
    void Enable(int enable);
    void Start(const char *name);
    void Stop(void);
    void Clear(void);
    void SetLogFile(const char *fileName);

private:
    /* Private variables. */
    SPxAutoPtr<SPxLogTimePriv> m_p;

    /* Disable copying. */
    SPxLogTime(const SPxLogTime&) SPX_FUNCTION_DELETE;
    SPxLogTime& operator=(const SPxLogTime&) SPX_FUNCTION_DELETE;

}; /* SPxLogTime */

#endif /* _SPX_LOG_TIME_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/

/*********************************************************************
*
* (c) Copyright 2021, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxPlotReporterWeb class, used for sending plot reports
*	over a web interface.
*
*
* Revision Control:
*   10/08/21 v1.2    BTB	Add mutex for web interface usage.
*
* Previous Changes:
*   29/07/21 1.1    BTB	Initial Version.
**********************************************************************/

#ifndef _SPX_PLOT_REPORTER_WEB_H
#define _SPX_PLOT_REPORTER_WEB_H

/*
 * Other headers required.
 */
/* We need the plot reporter base class we are derived from. */
#include "SPxLibData/SPxPlotReporter.h"
/* Include SPxCriticalSection for mutex. */
#include "SPxLibUtils/SPxCriticalSection.h"


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

/* Forward-define classes in case we're included in the wrong order. */
class SPxWorldMap;
class SPxWebInterface;

/*
 * Define our class.
 */
class SPxPlotReporterWeb : public SPxPlotReporter
{
public:
    /*
     * Public variables.
     */

    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    explicit SPxPlotReporterWeb(SPxWebInterface *webIf, SPxWorldMap *worldMap = NULL);
    virtual ~SPxPlotReporterWeb(void);

private:
    /*
     * Private variables.
     */
    SPxWebInterface *m_webIf;
    mutable SPxCriticalSection m_webMutex;
    unsigned int m_plotSendID;
    unsigned int m_statusSendID;

    /*
     * Private functions.
     */
    virtual SPxErrorCode OutputPlot(const SPxTime_tag *time,
                                    const unsigned char *data,
                                    unsigned int dataSize);
    virtual SPxErrorCode OutputStatus(const unsigned char *data,
                                      unsigned int dataSize);
    void clearWebIf(void);

    static int webIfDeletedFn(void *invokingObject,
                              void *userObject,
                              void *arg);

}; /* SPxPlotReporterWeb */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/


#endif /* _SPX_PLOT_REPORTER_WEB_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/

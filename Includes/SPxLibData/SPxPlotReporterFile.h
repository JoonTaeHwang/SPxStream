/*********************************************************************
*
* (c) Copyright 2019, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxPlotReporterFile class, used for recording
*	plot reports to a file.
*
*
* Revision Control:
*   25/06/19 v1.1    AGC	Initial Version.
*
* Previous Changes:
**********************************************************************/

#ifndef _SPX_PLOT_REPORTER_FILE_H
#define _SPX_PLOT_REPORTER_FILE_H

/*
 * Other headers required.
 */
/* We need the plot reporter base class we are derived from. */
#include "SPxLibData/SPxPlotReporter.h"


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
class SPxRunProcess;

/*
 * Define our class.
 */
class SPxPlotReporterFile : public SPxPlotReporter
{
public:
    /* Constructor and destructor. */
    explicit SPxPlotReporterFile(SPxRunProcess *recordProcess,
				 SPxWorldMap *worldMap = NULL);
    virtual ~SPxPlotReporterFile();

    virtual SPxErrorCode SetReportType(int reportType);

private:
    /*
     * Private variables.
     */
    SPxRunProcess *m_recordProcess;

    /*
     * Private functions.
     */
    virtual SPxErrorCode OutputPlot(const SPxTime_tag *time,
				    const unsigned char *data,
				    unsigned int dataSize);
    virtual SPxErrorCode OutputStatus(const unsigned char *data,
				      unsigned int dataSize);

}; /* SPxPlotReporterNet */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/


#endif /* _SPX_PLOT_REPORTER_FILE_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/

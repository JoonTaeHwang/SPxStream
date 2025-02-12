/*********************************************************************
*
* (c) Copyright 2022, Cambridge Pixel Ltd.
*
* Purpose:
*	Used to report coverage data through a web interface.
*
* Revision Control:
 *  14/10/22 v1.2    AGC	Add errCode to getHttpExt().
*
* Previous Changes:
*   22/09/22 1.1    AGC	Initial Version.
**********************************************************************/

#ifndef _SPX_WEB_COVERAGE_REPORTER
#define _SPX_WEB_COVERAGE_REPORTER

/* Includes. */
#include "SPxLibUtils/SPxObj.h"
#include "SPxLibNet/SPxWebProviderIface.h"
#if defined(WIN32) || SPX_X11_SUPPORTED
#include "SPxLibUtils/SPxTiledMapSrcCoverage.h"
#endif

/* Forward declarations. */
class SPxTiledMapDatabase;
class SPxTiledMapSrcCoverage;
class SPxWebInterface;

class SPxWebCoverageReporter : public SPxObj, public SPxWebProviderIface
{
public:
    /*
     * Public functions.
     */

    /* Construction and destruction. */
    explicit SPxWebCoverageReporter(SPxTiledMapDatabase *coverageDB,
				    SPxTiledMapSrcCoverage *coverageSrc,
				    SPxWebInterface *webIf);
    virtual ~SPxWebCoverageReporter(void);

protected:
    /*
     * Protected functions.
     */

private:
    /*
     * Private variables.
     */
    SPxTiledMapDatabase *m_coverageDB;
    SPxTiledMapSrcCoverage *m_coverageSrc;
    SPxWebInterface *m_webIf;
    unsigned int m_coverageTileEvSendID;

    /*
     * Private functions.
     */
#if defined(WIN32) || SPX_X11_SUPPORTED
    static int tiledMapEventWrapper(void *invokingObject,
				    void *userObject,
				    void *arg);
    SPxErrorCode tiledMapEvent(void *arg);
    void radarCovChanged(UINT32 flags,
			 const SPxTiledMapSrcCoverage::VisInfo *visInfo,
			 int index,
			 const SPxTiledMapSrcCoverage::RadarInfo *radarInfo);
#endif

    virtual SPxErrorCode getHttpFilePath(const char *urlExt,
					 char *filePath,
					 unsigned int filePathSize,
					 unsigned int *errCode);

};

#endif /* _SPX_WEB_COVERAGE_REPORTER */

/*********************************************************************
*
* End of file
*
**********************************************************************/

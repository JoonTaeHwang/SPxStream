/*********************************************************************
*
* (c) Copyright 2021, 2022, Cambridge Pixel Ltd.
*
* Purpose:
*	Used to report terrain data asynchronously through a web interface.
*
* Revision Control:
*   14/10/22 v1.3    AGC	Server SRTM tiles directly.
*
* Previous Changes:
*   28/09/22 1.2    AGC	Update terrain when tiles downloaded.
*   07/12/21 1.1    AGC	Initial Version.
**********************************************************************/

#ifndef _SPX_WEB_TERRAIN_REPORTER
#define _SPX_WEB_TERRAIN_REPORTER

/* Includes. */
#include "SPxLibUtils/SPxObj.h"
#include "SPxLibNet/SPxWebProviderIface.h"

/* Forward declarations. */
class SPxTerrainDatabase;
class SPxWebInterface;

class SPxWebTerrainReporter : public SPxObj, public SPxWebProviderIface
{
public:
    /*
     * Public functions.
     */

    /* Construction and destruction. */
    explicit SPxWebTerrainReporter(SPxTerrainDatabase *terrainDB,
				   SPxWebInterface *webIf);
    virtual ~SPxWebTerrainReporter(void);

protected:
    /*
     * Protected functions.
     */

private:
    /*
     * Private variables.
     */
    struct impl;
    SPxAutoPtr<impl> m_p;

    /*
     * Private functions.
     */
    virtual SPxErrorCode getHttpFilePath(const char *urlExt,
					 char *filePath,
					 unsigned int filePathSize,
					 unsigned int *errCode);
};

#endif /* _SPX_WEB_TERRAIN_REPORTER */

/*********************************************************************
*
* End of file
*
**********************************************************************/

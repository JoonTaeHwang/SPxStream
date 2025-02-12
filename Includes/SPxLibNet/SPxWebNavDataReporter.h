/*********************************************************************
*
* (c) Copyright 2021, Cambridge Pixel Ltd.
*
* Purpose:
*	Used to report nav data asynchronously through a web interface.
*
* Revision Control:
*   06/01/22 v1.3    AGC	Optionally report terrain height.
*
* Previous Changes:
*   07/12/21 1.2    AGC	Improve includes.
*   24/11/21 1.1    AGC	Initial Version.
**********************************************************************/

#ifndef _SPX_WEB_NAV_DATA_REPORTER
#define _SPX_WEB_NAV_DATA_REPORTER

/* Includes. */
#include "SPxLibUtils/SPxObj.h"
#include "SPxLibUtils/SPxTimer.h"

/* Forward declarations. */
class SPxNavData;
class SPxWebInterface;
class SPxTerrainDatabase;

class SPxWebNavDataReporter : public SPxObj
{
public:
    /*
     * Public functions.
     */

    /* Construction and destruction. */
    explicit SPxWebNavDataReporter(SPxNavData *navData,
				   SPxWebInterface *webIf,
				   unsigned int index=0,
				   SPxTerrainDatabase *terrainDB=NULL);
    virtual ~SPxWebNavDataReporter(void);

protected:
    /*
     * Protected functions.
     */

private:
    /*
     * Private variables.
     */
    SPxTimer m_timer;
    SPxNavData *m_navData;
    unsigned int m_navDataIndex;
    SPxWebInterface *m_webIf;
    SPxTerrainDatabase *m_terrainDB;
    unsigned int m_navDataOutputID;

    /*
     * Private functions.
     */
    static int navDataFnWrapper(void *invokingObject,
				void *userObject,
				void *arg);
    void navDataFn(void);

};

#endif /* _SPX_WEB_NAV_DATA_REPORTER */

/*********************************************************************
*
* End of file
*
**********************************************************************/

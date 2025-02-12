/*********************************************************************
*
* (c) Copyright 2021, Cambridge Pixel Ltd.
*
* Purpose:
*	Used to report area events asynchronously through a web interface.
*
* Revision Control:
*   30/11/21 v1.2    BTB	Add index to async output stream name.
*
* Previous Changes:
*   01/09/21 1.1    AGC	Initial Version.
**********************************************************************/

#ifndef _SPX_WEB_AREA_EVENT_REPORTER
#define _SPX_WEB_AREA_EVENT_REPORTER

/* Includes. */
#include "SPxLibUtils/SPxAutoPtr.h"
#include "SPxLibNet/SPxWebInterface.h"

/* Forward declarations. */
class SPxAreaObj;
class SPxWebInterface;

class SPxWebAreaEventReporter : public SPxObj
{
public:
    /*
     * Public functions.
     */

    /* Construction and destruction. */
    explicit SPxWebAreaEventReporter(SPxAreaObj *area,
				     SPxWebInterface *webIf,
				     unsigned int index=0);
    virtual ~SPxWebAreaEventReporter(void);

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

    SPxAreaObj *m_area;
    unsigned int m_areaIndex;
    SPxWebInterface *m_webIf;
    unsigned int m_areaEventID;

    /*
     * Private functions.
     */
    static void areaEventFnWrapper(SPxAreaObj *obj, UINT32 flags, void *userArg);
    void areaEventFn(SPxAreaObj *obj, UINT32 flags);

};

#endif /* _SPX_WEB_AREA_EVENT_REPORTER */

/*********************************************************************
*
* End of file
*
**********************************************************************/

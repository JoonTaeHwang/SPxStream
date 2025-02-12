/*********************************************************************
 *
 * (c) Copyright 2017, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   Header for SPxAlarmRenderer class which is used to render
 *   alarm graphics in a PPI window.
 *
 * Revision Control:
 *   11/12/17 v1.2   SP 	Support CPA alarms.
 *                      	Remove support for reference points.
 *
 * Previous Changes:
 *   18/10/17 1.1   SP 	Initial version.
 *
 *********************************************************************/

#ifndef _SPX_ALARM_RENDERER_H
#define _SPX_ALARM_RENDERER_H

/*
 * Other headers required.
 */

/* Include common types, callbacks, errors etc. */
#include "SPxLibUtils/SPxObj.h"
#include "SPxLibUtils/SPxRenderer.h"
#include "SPxLibUtils/SPxError.h"
#include "SPxLibUtils/SPxStrings.h"
#include "SPxLibUtils/SPxTime.h"

/*********************************************************************
 *
 *   Constants
 *
 *********************************************************************/

/* Minimum TCPA that is displayed. */
#define SPX_ALARM_RENDERER_MIN_TCPA (-10.0)

/*********************************************************************
 *
 *   Macros
 *
 *********************************************************************/


/*********************************************************************
 *
 *   Type definitions
 *
 *********************************************************************/

/* Forward declare other classes required. */
class SPxAlarmDatabase;
class SPxAlarm;

/*
 * Define our class.
 */
class SPxAlarmRenderer : virtual public SPxRenderer
{
public:
    /*
     * Public variables.
     */

    /*
     * Public functions.
     */

    /* Constructor and destructor. */
    SPxAlarmRenderer(SPxAlarmDatabase *alarmDB);
    virtual ~SPxAlarmRenderer(void);

    /* Functions used to implement SPxMouseInterface. */
    virtual int HandleMousePress(UINT32 flags, int x, int y, SPxCursor_t *cursor);
    virtual int HandleMouseRelease(UINT32 flags, int x, int y, SPxCursor_t *cursor);
    virtual int HandleDoubleClick(UINT32 flags, int x, int y, SPxCursor_t *cursor);
    virtual int HandleMouseMove(UINT32 flags, int x, int y, SPxCursor_t *cursor);
    virtual int HandleMouseLeave(void);

protected:
    /*
     * Protected functions.
     */

    /* Generic parameter assignment. */
    virtual int SetParameter(char *parameterName,
                             char *parameterValue);

    virtual int GetParameter(char *parameterName,
                             char *valueBuf, 
                             int bufLen);

private:
    /*
     * Private types.
     */

    /* Used during CPA alarm rendering. */
    typedef struct
    {
        SPxLatLong_t startLL1;
        SPxLatLong_t startLL2;

    } CPAPos_t;

    /*
     * Private variables.
     */

    struct impl;
    SPxAutoPtr<impl> m_p;
    SPxAlarmDatabase *m_alarmDB;

    /*
     * Private functions.
     */

    SPxErrorCode renderAll(void);
    void renderCPAAlarm(SPxAlarm *alarm);
    int isCPARendered(const SPxLatLong_t *startLL1,
                      const SPxLatLong_t *startLL2);
    int isEqual(const SPxLatLong_t *ll1,
                const SPxLatLong_t *ll2);

    /*
     * Private static functions.
     */

    static int renderAlarmIterator(void *alarmDBVoid, 
                                   void *objVoid, 
                                   void *alarmVoid);

}; /* SPxAlarmRenderer */


/*********************************************************************
 *
 *   Function prototypes
 *
 *********************************************************************/

#endif /* SPX_ALARM_RENDERER_H */

/*********************************************************************
 *
 * End of file
 *
 *********************************************************************/

/*********************************************************************
*
* (c) Copyright 2012, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxLogicReportAIS.h,v $
* ID: $Id: SPxLogicReportAIS.h,v 1.2 2013/10/04 15:31:08 rew Exp $
*
* Purpose:
*	Header for SPxLogicReportAIS class, used for sending AIS 
*	tracks reports from an AIS decoder to a logic database.
*
*
* Revision Control:
*   04/10/13 v1.2    AGC	Simplify headers.
*
* Previous Changes:
*   21/12/12 1.1    AGC	Initial version.
**********************************************************************/

#ifndef SPX_LOGIC_REPORT_AIS_H
#define SPX_LOGIC_REPORT_AIS_H

/*
 * Other headers required.
 */

#include "SPxLibUtils/SPxObjPtr.h"
#include "SPxLibUtils/SPxObj.h"
#include "SPxLibData/SPxLogicDB.h"

/*********************************************************************
*
*   Type definitions
*
**********************************************************************/

/* Forward declarations. */
class SPxLogicDB;
class SPxAISDecoder;
class SPxAISTrack;

/*
 * Define our class.
 */
class SPxLogicReportAIS : public SPxObj
{
public:
    /* Public functions. */
    SPxLogicReportAIS(SPxLogicDB *logicDB, SPxAISDecoder *aisDecoder, UINT32 id=0);
    virtual ~SPxLogicReportAIS(void);

private:
    /* Private variables. */
    SPxObjPtr<SPxLogicDB> m_logicDB;
    SPxObjPtr<SPxAISDecoder> m_aisDecoder;
    UINT32 m_id;

    /* Private functions. */
    static int aisTrackFnWrapper(void *invokingObject, void *userObject, void *arg);
    int aisTrackFn(SPxAISTrack *track);

}; /* SPxLogicReportAIS */

#endif /* SPX_LOGIC_REPORT_AIS_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/

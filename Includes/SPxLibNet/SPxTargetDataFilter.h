/*********************************************************************
 *
 * (c) Copyright 2022, Cambridge Pixel Ltd.
 *
 * Purpose:
 *	Header for SPxTargetDataFilter class, which is used to
 *	manage flagging or exclusion on data present in a stream
 *	of SPx Extended track reports.
 *
 *
 * Revision Control:
 *   11/05/22 v1.2   BTB	Manage track databases better.
 *
 * Previous Changes:
 *   11/04/22 1.1   BTB	Initial Version.
 **********************************************************************/

#ifndef _SPX_TARGET_DATA_FILTER_H
#define _SPX_TARGET_DATA_FILTER_H

/*
 * Other headers required.
 */

#include "SPxLibData/SPxPackets.h"
#include "SPxLibUtils/SPxCallbackList.h"
#include "SPxLibUtils/SPxObj.h"

/*********************************************************************
 *
 *   Constants
 *
 *********************************************************************/

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

/* Forward declarations. */
class SPxTrackAnnotator;
class SPxTrackDatabase;
class SPxUniTrackDatabase;

/*
 * Define our class.
 */
class SPxTargetDataFilter : public SPxObj
{
public:
    /*
     * Public functions.
     */

    /* Constructor and destructor. */
    SPxTargetDataFilter(SPxTrackAnnotator *dataSrc,
                        SPxTrackDatabase * trackDB,
                        SPxUniTrackDatabase *uniTrackDB);
    virtual ~SPxTargetDataFilter(void);

    /* Callback handling functions. */
    SPxErrorCode AddExtendedTrackCallback(SPxCallbackListFn_t fn, void *userObj);
    SPxErrorCode RemoveExtendedTrackCallback(SPxCallbackListFn_t fn, void *userObj);
    SPxErrorCode AddTrackerStatusCallback(SPxCallbackListFn_t fn, void *userObj);
    SPxErrorCode RemoveTrackerStatusCallback(SPxCallbackListFn_t fn, void *userObj);

    /* Generic parameter assignment. */
    virtual int SetParameter(char *parameterName, char *parameterValue);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);

private:
    /*
     * Private variables.
     */

    SPxTrackAnnotator *m_dataSrc;
    SPxTrackDatabase *m_trackDB;
    SPxUniTrackDatabase *m_uniTrackDB;
    int m_sourceIndex;
    int m_flagWhenFiltered;
    unsigned int m_trackCount;

    /* Callback lists for passing on track messages and status messages. */
    SPxCallbackList *m_extTrackCBList;
    SPxCallbackList *m_trackerStatusCBList;

    /*
     * Private functions.
     */
    void handleExtTrack(SPxPacketTrackExtended *extReport);
    void handleTrackerStatus(SPxPacketTrackerStatus_t *statusReport);

    /*
     * Private static functions.
     */
    static int extTrackHandler(void *invokingObj,
                               void *userArg,
                               void *extTrackMsg);
    static int trackerStatusHandler(void *invokingObj,
                                    void *userArg,
                                    void *statusMsg);
};

#endif /* _SPX_TARGET_DATA_FILTER_H */

/*********************************************************************
 *
 * End of file
 *
 *********************************************************************/

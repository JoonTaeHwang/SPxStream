/*********************************************************************
*
* (c) Copyright 2022, Cambridge Pixel Ltd.
*
* Purpose:
*	Used to report tracks asynchronously through a web interface.
*
* Revision Control:
*   09/09/22 v1.4    AGC	Report tracker status.
*
* Previous Changes:
*   27/05/22 1.3    AGC	Set range/course for AIS reports.
*   19/04/22 1.2    AGC	Initial support for AIS output.
*   28/03/22 1.1    AGC	Initial Version.
**********************************************************************/

#ifndef _SPX_WEB_TRACK_REPORTER
#define _SPX_WEB_TRACK_REPORTER

/* Includes. */
#include "SPxLibUtils/SPxAutoPtr.h"
#include "SPxLibNet/SPxWebInterface.h"

/* Forward declarations. */
class SPxTrackDatabase;
class SPxUniTrackDatabase;
class SPxWebInterface;
class SPxNavData;
class SPxRadarTrack;
class SPxAISTrack;
class SPxADSBTrack;
class SPxUniTrack;
struct SPxPacketTrackerStatus_tag;

class SPxWebTrackReporter : public SPxObj
{
public:
    /*
     * Public functions.
     */

    /* Construction and destruction. */
    explicit SPxWebTrackReporter(SPxTrackDatabase *trackDB,
				 SPxWebInterface *webIf,
				 int index=-1);
    explicit SPxWebTrackReporter(SPxUniTrackDatabase *trackDB,
				 SPxWebInterface *webIf,
				 int index=-1);
    virtual ~SPxWebTrackReporter(void);

    SPxErrorCode ReportAll(void);

    SPxErrorCode SetNavData(SPxNavData *navData);
    SPxNavData *GetNavData(void) const;

    /* Generic parameter assignment. */
    virtual int SetParameter(char *name, char *value);
    virtual int GetParameter(char *name, char *valueBuf, int bufLen);

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

    SPxTrackDatabase *m_trackDB;
    SPxUniTrackDatabase *m_uniTrackDB;
    SPxWebInterface *m_webIf;
    int m_trackReporterIndex;
    unsigned int m_trackReporterID;
    SPxNavData *m_navData;

    /*
     * Private functions.
     */
    static int trackEventFnWrapper(void *invokingObject,
				   void *userObject,
				   void *arg);
    SPxErrorCode trackFn(SPxRadarTrack *track);
    static int uniTrackEventFnWrapper(void *invokingObject,
				      void *userObject,
				      void *arg);
    static int uniTrackIterateFnWrapper(void *invokingObject,
					void *userObject,
					void *arg);
    SPxErrorCode uniTrackFn(SPxUniTrack *track);
    SPxErrorCode outputRadarTrack(SPxRadarTrack *track);
    SPxErrorCode outputAISTrack(SPxAISTrack *track);
    SPxErrorCode outputADSBTrack(SPxADSBTrack *track);

    void reportAllStatus(void);
    void reportStatus(const SPxPacketTrackerStatus_tag *trackerStatus);

};

#endif /* _SPX_WEB_TRACK_REPORTER */

/*********************************************************************
*
* End of file
*
**********************************************************************/

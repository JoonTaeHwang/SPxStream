/*********************************************************************
 *
 * (c) Copyright 2021 - 2022, Cambridge Pixel Ltd.
 *
 * Purpose:
 *	Header for SPxTrackAnnotator class, which is used to modify
 *	or expand the data present in SPx Extended track reports.
 *
 *
 * Revision Control:
 *   11/04/21 v1.6   BTB	Support operating as a discrete stage in a TrackManager.
 *
 * Previous Changes:
 *   29/09/21 1.5   BTB	Separate values and enable states for annotation properties.
 *   20/09/21 1.4   BTB	Add extra functionality for managing annotations.
 *   23/08/21 1.3   BTB	Improve interface.
 *   20/08/21 1.2   BTB	Add more fields for annotation.
 *   19/08/21 1.1   BTB	Initial Version.
 **********************************************************************/

#ifndef _SPX_TRACK_ANNOTATOR_H
#define _SPX_TRACK_ANNOTATOR_H

/*
 * Other headers required.
 */

#include "SPxLibData/SPxPackets.h"
#include "SPxLibUtils/SPxAutoPtr.h"
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
class SPxTargetDecoder;

/*
 * Define our class.
 */
class SPxTrackAnnotator : public SPxObj
{
public:
    /*
     * Public functions.
     */

    /* Constructor and destructor. */
    SPxTrackAnnotator(SPxTargetDecoder *dataSrc = NULL);
    virtual ~SPxTrackAnnotator(void);

    /* Main function to perform track annotation in situ. */
    SPxErrorCode Annotate(SPxPacketTrackExtended *trackReport) const;

    /* Function to purge annotations for dead tracks. */
    void PurgeDeadAnnotations(void);

    /* Callback handling functions. */
    SPxErrorCode AddExtendedTrackCallback(SPxCallbackListFn_t fn, void *userObj);
    SPxErrorCode RemoveExtendedTrackCallback(SPxCallbackListFn_t fn, void *userObj);
    SPxErrorCode AddTrackerStatusCallback(SPxCallbackListFn_t fn, void *userObj);
    SPxErrorCode RemoveTrackerStatusCallback(SPxCallbackListFn_t fn, void *userObj);

    /* Generic parameter assignment. */
    virtual int SetParameter(char *parameterName, char *parameterValue);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);

    /* Timeout for purging dead tracks. */
    SPxErrorCode SetPurgeTimeout(double timeoutSecs);
    double GetPurgeTimeout(void) const { return m_purgeTimeoutSecs; }

    /* Get information on tracks with annotation configured. */
    unsigned int GetNumTracks(void) const;
    SPxErrorCode GetTrackIDList(char *idBuf, unsigned int bufLen) const;

    /* Annotation creation/deletion/transfer. */
    SPxErrorCode CreateAnnotation(unsigned int originalID);
    SPxErrorCode DeleteAnnotation(unsigned int originalID);
    SPxErrorCode TransferAnnotation(unsigned int fromOriginalID, unsigned int toOriginalID);

    /* Get/set properties for annotation */
    SPxErrorCode SetID(unsigned int originalID, UINT32 newID);
    SPxErrorCode GetID(unsigned int originalID, UINT32 *newIDRtn) const;

    SPxErrorCode SetUsingID(unsigned int originalID, int use);
    int GetUsingID(unsigned int originalID) const;

    SPxErrorCode SetSenderID(unsigned int originalID, UINT8 senderID);
    SPxErrorCode GetSenderID(unsigned int originalID, UINT8 *senderIDRtn) const;

    SPxErrorCode SetUsingSenderID(unsigned int originalID, int use);
    int GetUsingSenderID(unsigned int originalID) const;

    SPxErrorCode SetTtmID(unsigned int originalID, UINT8 ttmID);
    SPxErrorCode GetTtmID(unsigned int originalID, UINT8 *ttmIDRtn) const;

    SPxErrorCode SetUsingTtmID(unsigned int originalID, int use);
    int GetUsingTtmID(unsigned int originalID) const;

    SPxErrorCode SetFlags(unsigned int originalID, UINT8 flags);
    SPxErrorCode GetFlags(unsigned int originalID, UINT8 *flagsRtn) const;

    SPxErrorCode SetUsingFlags(unsigned int originalID, int use);
    int GetUsingFlags(unsigned int originalID) const;

    SPxErrorCode SetClassification(unsigned int originalID, UINT16 classification);
    SPxErrorCode GetClassification(unsigned int originalID, UINT16 *classificationRtn) const;

    SPxErrorCode SetUsingClassification(unsigned int originalID, int use);
    int GetUsingClassification(unsigned int originalID) const;

    SPxErrorCode SetAltitude(unsigned int originalID, REAL32 altitudeMetres);
    SPxErrorCode GetAltitude(unsigned int originalID, REAL32 *altitudeMetresRtn) const;

    SPxErrorCode SetUsingAltitude(unsigned int originalID, int use);
    int GetUsingAltitude(unsigned int originalID) const;

    SPxErrorCode SetDescription(unsigned int originalID, const char *description);
    SPxErrorCode GetDescription(unsigned int originalID, char *descriptionBuf, unsigned int bufLen) const;

    SPxErrorCode SetUsingDescription(unsigned int originalID, int use);
    int GetUsingDescription(unsigned int originalID) const;

    SPxErrorCode SetCallsign(unsigned int originalID, const char *callsign);
    SPxErrorCode GetCallsign(unsigned int originalID, char *callsignBuf, unsigned int bufLen) const;

    SPxErrorCode SetUsingCallsign(unsigned int originalID, int use);
    int GetUsingCallsign(unsigned int originalID) const;

    SPxErrorCode SetThreatName(unsigned int originalID, const char *threatName);
    SPxErrorCode GetThreatName(unsigned int originalID, char *threatNameBuf, unsigned int bufLen) const;

    SPxErrorCode SetUsingThreatName(unsigned int originalID, int use);
    int GetUsingThreatName(unsigned int originalID) const;

    SPxErrorCode SetThreatType(unsigned int originalID, UINT8 threatType);
    SPxErrorCode GetThreatType(unsigned int originalID, UINT8 *threatTypeRtn) const;

    SPxErrorCode SetUsingThreatType(unsigned int originalID, int use);
    int GetUsingThreatType(unsigned int originalID) const;

    SPxErrorCode SetThreatLevel(unsigned int originalID, UINT8 threatLevel);
    SPxErrorCode GetThreatLevel(unsigned int originalID, UINT8 *threatLevelRtn) const;

    SPxErrorCode SetUsingThreatLevel(unsigned int originalID, int use);
    int GetUsingThreatLevel(unsigned int originalID) const;

private:
    /*
     * Private variables.
     */

    struct impl;
    SPxAutoPtr<impl> m_p;

    double m_purgeTimeoutSecs;
    SPxTargetDecoder *m_dataSrc;

    /* Callback lists for passing on annotated track messages and status messages. */
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

}; /* SPxTrackAnnotator */

#endif /* _SPX_TRACK_ANNOTATOR_H */

/*********************************************************************
 *
 * End of file
 *
 *********************************************************************/

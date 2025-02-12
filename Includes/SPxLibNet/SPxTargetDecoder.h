/*********************************************************************
 *
 * (c) Copyright 2020 - 2022, Cambridge Pixel Ltd.
 *
 * Purpose:
 *	Header for base class of object for receiving and decoding
 *	track and/or plot data and outputting SPx extended tracks.
 *
 *
 * Revision Control:
 *   17/03/22 v1.14  AGC	Fix WriteDebug() format attribute.
 *
 * Previous Changes:
 *   17/03/22 1.13  AGC	Add WriteDebug().
 *   16/03/22 1.12  AGC	Support roll adjustment.
 *   11/03/22 1.11  AGC	Add connection debug.
 *   11/03/22 1.10  AGC	Support pitch adjustment.
 *			Support sensor height adjustment.
 *			Use sensor height from nav data if available.
 *   23/02/22 1.9   BTB	Support ignoring received status messages.
 *   05/10/21 1.8   BTB	Support sending debug messages over web interface.
 *   28/09/21 1.7   BTB	Support polling for when the last packet was received.
 *   10/09/21 1.6   BTB	Support correction.
 *   21/10/20 1.5   BTB	Support configuration of whether to allow test targets.
 *   07/10/20 1.4   BTB	Make Set/GetParameter() virtual.
 *   19/02/20 1.3   BTB	Move parameter definitions to source file.
 *   24/01/20 1.2   BTB	Add extra functionality needed for ASTERIX and YLC6 conversion.
 *   21/01/20 1.1   BTB	Initial Version.
 **********************************************************************/

#ifndef _SPX_TARGET_DECODER_H
#define _SPX_TARGET_DECODER_H

/*
 * Other headers required.
 */
/* We need SPxLibUtils for common types, callbacks, errors etc. */
#include "SPxLibUtils/SPxError.h"
#include "SPxLibUtils/SPxCallbackList.h"
#include "SPxLibUtils/SPxTime.h"
#include "SPxLibUtils/SPxTimer.h"

/* We need SPxPackets for the reports we are outputting. */
#include "SPxLibData/SPxPackets.h"

/* We need the SPxDecoderRepeater base class. */
#include "SPxLibNet/SPxDecoderRepeater.h"

/* Forward declare any classes required. */
class SPxWebInterface;

/*********************************************************************
 *
 *   Constants
 *
 *********************************************************************/

/* Debug flags. */
#define SPX_TARGET_DECODER_DEBUG_MSG_ERRORS     0x00000001 /* Error messages. */
#define SPX_TARGET_DECODER_DEBUG_MSG_RXTX       0x00000002 /* Receipt and transmission of messages. */
#define SPX_TARGET_DECODER_DEBUG_MSG_CONTENT    0x00000004 /* Information relating to message content. */
#define SPX_TARGET_DECODER_DEBUG_MSG_CONNECT    0x00000008 /* Connection information. */

class SPxTargetDecoder : public SPxDecoderRepeater
{
public:
    /*
     * Public functions.
     */

    /* Constructor and destructor. */
    SPxTargetDecoder(void);
    virtual ~SPxTargetDecoder(void);

    /* Set web interface for reporting debug messages. */
    SPxErrorCode SetWebIf(SPxWebInterface *webIf, unsigned int webMsgKey);

    /* Callback handling functions. */
    SPxErrorCode AddExtendedTrackCallback(SPxCallbackListFn_t fn, void *userObj);
    SPxErrorCode RemoveExtendedTrackCallback(SPxCallbackListFn_t fn, void *userObj);
    SPxErrorCode AddTrackerStatusCallback(SPxCallbackListFn_t fn, void *userObj);
    SPxErrorCode RemoveTrackerStatusCallback(SPxCallbackListFn_t fn, void *userObj);

    /* Generic parameter assignment. */
    virtual int SetParameter(char *parameterName, char *parameterValue);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);

    SPxErrorCode SetAutoStatusIntervalMS(unsigned int interval);
    unsigned int GetAutoStatusIntervalMS(void) const { return m_statusIntervalMS; }

    SPxErrorCode SetIgnoreReceivedStatusMessages(int ignore);
    int GetIgnoreReceivedStatusMessages(void) const { return m_ignoreReceivedStatus; }

    SPxErrorCode SetSensorID(unsigned int sensorID);
    unsigned int GetSensorID(void) const { return m_sensorID; }

    SPxErrorCode SetSensorLatLongAvailable(int available);
    int IsSensorLatLongAvailable(void) const { return m_sensorLatLongAvailable; }
    SPxErrorCode SetSensorLatLong(double latDegs, double longDegs, int setFromStatus = FALSE);
    double GetSensorLatDegs(void) const { return m_sensorLatDegs; }
    double GetSensorLongDegs(void) const { return m_sensorLongDegs; }

    SPxErrorCode SetSensorHeightMetres(double heightMetres);
    double GetSensorHeightMetres(void) const { return m_sensorHeightMetres; }

    SPxErrorCode SetTracksEnabled(int enable);
    int GetTracksEnabled(void) const { return m_tracksEnabled; }

    SPxErrorCode SetPlotsEnabled(int enable);
    int GetPlotsEnabled(void) const { return m_plotsEnabled; }

    SPxErrorCode SetRangeOffsetMetres(double offsetMetres);
    double GetRangeOffsetMetres(void) const { return m_rangeOffsetMetres; }

    SPxErrorCode SetRangeMultiplier(double multiplier);
    double GetRangeMultiplier(void) const { return m_rangeMultiplier; }

    SPxErrorCode SetAziOffsetDegs(double offsetDegs);
    double GetAziOffsetDegs(void) const { return m_aziOffsetDegs; }
    
    SPxErrorCode SetPitchDegs(double pitchDegs);
    double GetPitchDegs(void) const { return m_pitchDegs; }

    SPxErrorCode SetRollDegs(double rollDegs);
    double GetRollDegs(void) const { return m_rollDegs; }

    SPxErrorCode SetSensorHeightCorrectionEnabled(int enable);
    int GetSensorHeightCorrectionEnabled(void) const { return m_sensorHeightCorrectionEnabled; }

    SPxErrorCode SetSlantRangeCorrectionEnabled(int enable);
    int GetSlantRangeCorrectionEnabled(void) const { return m_slantRangeCorrectionEnabled; }

    SPxErrorCode SetTestTargetsAllowed(int allow);
    int GetTestTargetsAllowed(void) const { return m_allowTestTargets; }

    SPxErrorCode SetDebugFlags(UINT32 flags);
    UINT32 GetDebugFlags(void) const { return m_debugFlags; }

    void SetDebugFile(FILE *dbgFile) { m_debugFile = dbgFile; }
    FILE *GetDebugFile(void) const { return m_debugFile; }

    void SetNavData(SPxNavData *navData) { m_navData = navData; }
    SPxNavData *GetNavData(void) const { return m_navData; }

    double GetTimeSinceLastPacketSecs(void) const;

    unsigned int GetPrimaryTracksSinceLastStatus(void) const { return m_primTracksReceived; }
    unsigned int GetSecondaryTracksSinceLastStatus(void) const { return m_secTracksReceived; }
    unsigned int GetCombinedTracksSinceLastStatus(void) const { return m_combTracksReceived; }
    unsigned int GetTotalTracksSinceLastStatus(void) const { return m_primTracksReceived + m_secTracksReceived + m_combTracksReceived; }

protected:
    /*
     * Protected types.
     */
    /* Track type for extended reports received from derived classes. */
    enum TrackReportType
    {
        TRACK_REPORT_TYPE_PRIMARY = 0,
        TRACK_REPORT_TYPE_SECONDARY = 1,
        TRACK_REPORT_TYPE_COMBINED = 2
    };

    /*
     * Protected functions.
     */
    void FlagPacketReceived(void) { SPxTimeGetEpoch(&m_lastData); }
    void SendExtendedTrackReport(SPxPacketTrackExtended *extReport, TrackReportType trackType = TRACK_REPORT_TYPE_PRIMARY);
    void SendTrackerStatusMessage(SPxPacketTrackerStatus_t *statusMessage, int autoGenerated = FALSE);
    void SendDebugMessageToWeb(const char *msg, int isError);

    void ResetTracksSinceLastStatus(void) { m_primTracksReceived = m_secTracksReceived = m_combTracksReceived = 0; }

    void
#if defined(__GNUC__) || defined(__clang__)
        __attribute__((format(printf, 3, 4)))
#endif
        WriteDebug(UINT32 dbgLevel, const char *format, ...);

private:
    /*
     * Private variables.
     */

    /* Timer for generating and sending tracker status messages. */
    SPxAutoPtr<SPxTimer> m_statusTimer;
    unsigned int m_statusIntervalMS;

    /* Wherger to ignore received status messages. */
    int m_ignoreReceivedStatus;

    /* Information about extended tracks received. */
    unsigned int m_primTracksReceived;
    unsigned int m_secTracksReceived;
    unsigned int m_combTracksReceived;
    unsigned int m_lastExtFlagsMask;

    /* Sensor information. */
    unsigned int m_sensorID;
    int m_sensorLatLongAvailable;
    double m_sensorLatDegs;
    double m_sensorLongDegs;
    double m_sensorHeightMetres;

    /* Track/plot output enabled, used by derived classes. */
    int m_tracksEnabled;
    int m_plotsEnabled;

    /* Corrections to apply. */
    double m_rangeOffsetMetres;
    double m_rangeMultiplier;
    double m_aziOffsetDegs;
    double m_pitchDegs;
    double m_rollDegs;
    int m_sensorHeightCorrectionEnabled;
    int m_slantRangeCorrectionEnabled;

    /* Whether or not to send through test targets. */
    int m_allowTestTargets;

    /* Level of debug output. */
    UINT32 m_debugFlags;
    FILE *m_debugFile;

    /* Nav data input. */
    SPxNavData *m_navData;

    /* Time last packet was received. */
    SPxTime_t m_lastData;

    /* Callback lists for passing on extended track and tracker status messages. */
    SPxCallbackList *m_extTrackCBList;
    SPxCallbackList *m_trackerStatusCBList;

    /* Web interface for sending debug messages. */
    SPxWebInterface *m_webIf;
    /* Key of the output stream to use to send the messages. */
    unsigned int m_webMsgKey;

    /*
     * Private functions.
     */
    void generateStatusMessage(void);

    /*
     * Private static functions.
     */
    static void generateStatusMessageWrapper(void *userArg);

}; /* SPxTargetDecoder */

#endif /* _SPX_TARGET_DECODER_H */

/*********************************************************************
 *
 * End of file
 *
 **********************************************************************/

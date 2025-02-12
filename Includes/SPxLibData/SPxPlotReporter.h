/*********************************************************************
*
* (c) Copyright 2019, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxPlotReporter class, base class for objects
*	that output plots.
*
*
* Revision Control:
*   25/06/19 v1.1    AGC	Initial Verison.
*
* Previous Changes:
**********************************************************************/

#ifndef _SPX_PLOT_REPORTER_H
#define _SPX_PLOT_REPORTER_H

/*
 * Other headers required.
 */
#include "SPxLibUtils/SPxCriticalSection.h"

/* We need the SPxObj base class we are derived from. */
#include "SPxLibUtils/SPxObj.h"


/*********************************************************************
*
*   Constants
*
**********************************************************************/


/*********************************************************************
*
*   Macros
*
**********************************************************************/


/*********************************************************************
*
*   Type definitions
*
**********************************************************************/

/* Forward-define classes in case we're included in the wrong order. */
class SPxWorldMap;
class SPxAsterixEncoder;
class SPxRunProcess;
struct SPxBlob_tag;
struct SPxBlobSector_tag;
struct SPxPacketTrackExtended_tag;
struct SPxPacketPlot_tag;
struct SPxTime_tag;

/*
 * Define our class.
 */
class SPxPlotReporter : public SPxObj
{
public:
    /*
     * Public variables.
     */

    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    explicit SPxPlotReporter(SPxWorldMap *worldMap = NULL);
    virtual ~SPxPlotReporter();

    /* NB: use SPxObj::Get/SetActive() for overall state control,
     * and SPxPacketSender::GetSetAddress/Port() for destination control.
     */

    /* Sender ID, 8 bits for SPx reports, 16 for SAC/SIC. */
    void SetSenderID(UINT16 id);
    UINT16 GetSenderID(void) const	{ return(m_senderID); }

    /* Report type (i.e. protocol used). */
    virtual SPxErrorCode SetReportType(int reportType);
    int GetReportType(void) const	{ return(m_reportType); }

    /* Link to plot merging process, if any. */
    SPxErrorCode SetPlotMergeProcess(const SPxRunProcess *rp);
    const SPxRunProcess *GetPlotMergeProcess(void) const
					{ return(m_plotMergeProcess); }

    /* Generic parameter assignment. */
    virtual int SetParameter(char *parameterName, char *parameterValue);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);

    /*
     * Static functions.
     */
    /* These handlers should be installed as callbacks in the plot source. */
    static int StaticPlotReportHandler(void *invokingObject,
				       void *userObject,
				       void *arg);
    static int StaticSectorReportHandler(void *invokingObject,
					 void *userObject,
					 void *arg);

private:
    /*
     * Private variables.
     */
    SPxCriticalSection m_mutex;

    /* Our sender ID, if any. */
    UINT16 m_senderID;			/* Zero, or sender ID */

    /* Type of report to send. */
    int m_reportType;			/* 0 off, 275=0x113 TYPEB_PLOT, 48 */

    /* Asterix encoder object. */
    SPxAsterixEncoder *m_asterixEncoder;

    /* Buffer for building reports. */
    unsigned char *m_buffer;		/* Buffer for reports */
    unsigned int m_bufferSizeBytes;	/* Size of buffer */

    /* Status monitoring. */
    UINT32 m_lastNorthMsecs;		/* Timestamp */
    int m_antiClockwise;		/* Flag for last plot */
    double m_periodSecs;
    SPxWorldMap *m_worldMap;
    UINT8 m_isIFF;			/* Flag for last status */

    /* Plot merging process, if any. */
    const SPxRunProcess *m_plotMergeProcess;	/* May be NULL if none */

    /*
     * Private functions.
     */
    int plotReportHandler(void *invokingObject,
			  struct SPxBlob_tag *blob);
    int sectorReportHandler(void *invokingObject,
			    struct SPxBlobSector_tag *sectorInfo);
    SPxErrorCode buildSPxTrackFromSPxPlot(
			struct SPxPacketTrackExtended_tag *track,
			struct SPxPacketPlot_tag *plot);

    /* Function to be implemented by derived classes. */
    virtual SPxErrorCode OutputPlot(const SPxTime_tag *time,
				    const unsigned char *data,
				    unsigned int dataSize) = 0;
    virtual SPxErrorCode OutputStatus(const unsigned char *data,
				      unsigned int dataSize) = 0;

}; /* SPxPlotReporter */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/


#endif /* _SPX_PLOT_REPORTER_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/

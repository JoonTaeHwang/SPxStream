/*********************************************************************
*
* (c) Copyright 2016, 2020, Cambridge Pixel Ltd.
*
* Purpose:
*	Header file for SPxLogFile functionality.
*
*
* Revision Control:
*   21/07/20 v1.7    REW	Support SetDirectory().
*
* Previous Changes:
*   06/07/20 1.6    AGC	Use const for SPxTime/SPxLatLong arguments.
*   03/06/20 1.5    REW	Support SetInhibitHeadings().
*   24/04/20 1.4    AGC	Separate formatting from logging.
*   10/10/16 1.3    REW	ID is optional in writeToFile().
*			Rename LogOwnPosition() to LogPosition() and
*			add 'whichPosition'.
*   30/09/16 1.2    REW	Support refLatLong in LogTrack().
*   23/09/16 1.1    REW	Initial Version.
**********************************************************************/

#ifndef _SPX_LOGFILE_H
#define _SPX_LOGFILE_H

/*
 * Other headers required.
 */
#include "SPxLibUtils/SPxObj.h"			/* For base class */
#include "SPxLibUtils/SPxTypes.h"		/* For SPxUnits... */

/*********************************************************************
*
*   Constants
*
**********************************************************************/


/*********************************************************************
*
*   Type definitions
*
**********************************************************************/

/* Forward-declare classes we refer to, so header file order doesn't matter. */
class SPxCriticalSection;
class SPxRadarTrack;
class SPxNavData;
struct SPxTime_tag;
struct SPxLatLong_tag;

/*
 * Define our class.
 */
class SPxLogFile :public SPxObj
{
public:
    /*
     * Public functions.
     */
    /* Constructor/destructor. */
    SPxLogFile(const char *appName=NULL);
    virtual ~SPxLogFile(void);

    /* Generic parameter control. */
    virtual int SetParameter(char *parameterName, char *parameterValue);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);

    /* Options. */
    SPxErrorCode SetDirectory(const char *dir);
    SPxErrorCode SetInhibitHeadings(int inhibit);
    int GetInhibitHeadings(void) const { return(m_inhibitHeadings); }
    SPxErrorCode SetUnitsDist(SPxUnitsDist_t option);
    SPxUnitsDist_t GetUnitsDist(void) const { return(m_unitsDist); }
    SPxErrorCode SetUnitsSpeed(SPxUnitsSpeed_t option);
    SPxUnitsSpeed_t GetUnitsSpeed(void) const { return(m_unitsSpeed); }
    SPxErrorCode SetFormatLatLong(SPxFormatLatLong_t option);
    SPxFormatLatLong_t GetFormatLatLong(void) const { return(m_formatLatLong);}

    /* Functions for formatting log messages. */
    SPxErrorCode FormatMsg(const SPxTime_tag *epochTime,
			   const char *msg,
			   const SPxLatLong_tag *latLong,
			   char *buf, unsigned int bufLen) const;
    SPxErrorCode FormatTrack(const SPxTime_tag *epochTime,
			     const SPxRadarTrack *track,
			     const SPxLatLong_tag *refLatLong,
			     const char *reason,
			     char *buf, unsigned int bufLen) const;
    SPxErrorCode FormatPosition(const SPxTime_tag *epochTime,
				const SPxNavData *navData,
				const char *whichPosition,
				char *buf, unsigned int bufLen) const;
    SPxErrorCode FormatPosition(const SPxTime_tag *epochTime,
				const SPxLatLong_tag *latLong,
				const char *whichPosition,
				char *buf, unsigned int bufLen) const;

    /* Functions for logging particular things. */
    SPxErrorCode LogMsg(const SPxTime_tag *epochTime,
			const char *msg,
			const SPxLatLong_tag *latLong=NULL);
    SPxErrorCode LogTrack(const SPxTime_tag *epochTime,
			  const SPxRadarTrack *track,
			  const SPxLatLong_tag *refLatLong=NULL,
			  const char *reason=NULL);
    SPxErrorCode LogPosition(const SPxTime_tag *epochTime,
			     const SPxNavData *navData,
			     const char *whichPosition=NULL);
    SPxErrorCode LogPosition(const SPxTime_tag *epochTime,
			     const SPxLatLong_tag *latLong,
			     const char *whichPosition=NULL);
    SPxErrorCode LogFormattedMsg(const char *msg);

    /*
     * Static functions for class.
     */

private:
    /*
     * Private variables.
     */
    /* Protection mutex. */
    SPxCriticalSection *m_mutex;		/* Safety mutex */

    /* Application name. */
    char *m_appName;			/* Copy of application name, or NULL */

    /* File information. */
    char *m_dir;			/* Directory, or NULL for current */
    FILE *m_file;			/* The file, or NULL if not open. */
    char *m_filename;			/* Name of the file */

    /* Options. */
    int m_inhibitHeadings;		/* Do we inhibit heading lines? */
    SPxUnitsDist_t m_unitsDist;		/* Units to report */
    SPxUnitsSpeed_t m_unitsSpeed;	/* Units to report */
    SPxFormatLatLong_t m_formatLatLong;	/* Formatting to use */

    /* Flags. */
    int m_pendingBanner;		/* Banner line is pending. */
    int m_pendingHeadings;		/* Headings line is pending. */

    /*
     * Private functions.
     */
    SPxErrorCode formatMessage(const SPxTime_tag *epochTime,
			       const char *entryType,
			       int idValid, unsigned int id,
			       const SPxLatLong_tag *latLong,
			       int posValid, double rangeMetres, double aziDegs,
			       int velValid, double speedMps, double courseDegs,
			       const char *comment,
			       char *buf, unsigned int bufLen) const;
    SPxErrorCode writeToFile(const char *msg);


}; /* SPxLogFile */


#endif /* _SPX_LOGFILE_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/

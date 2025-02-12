/*********************************************************************
*
* (c) Copyright 2018, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxTrackNoteDb class for logging and reading
*	of notes and snapshots for tracks.
*
*
* Revision Control:
*   30/10/18 v1.3    BTB	Introduce timeout for trying to get the lockfile.
*
* Previous Changes:
*   17/10/18 1.2    BTB	Support reading track logs as HTML file.
*   12/10/18 1.1    BTB	Initial Version.
**********************************************************************/

#ifndef _SPX_TRACK_NOTE_DB_H
#define _SPX_TRACK_NOTE_DB_H

/*
 * Other headers required.
 */
/* Base class. */
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

/* Need to forward-declare some classes we use. */
class SPxUniTrack;
class SPxImage;
class SPxCriticalSection;

/*
 * Define our class, derived from the normal base class.
 */

class SPxTrackNoteDb : public SPxObj
{
public:
    /*
    * Public functions.
    */
    /* Constructor and destructor. */
    SPxTrackNoteDb(void);
    ~SPxTrackNoteDb(void);

    SPxErrorCode LogNote(SPxUniTrack *track,
			 const char *note,
			 SPxImage *snapshot = NULL,
			 UINT32 timeoutMsecs=50);

    SPxErrorCode GetHTMLFile(SPxUniTrack *track,
			     char *filenameBuf,
			     int bufLen,
			     UINT32 timeoutMsecs=50);

protected:

private:
    struct impl;
    SPxAutoPtr<impl> m_p;

    SPxCriticalSection *m_mutex;

    void GetDbNameForTrack(SPxUniTrack *track,
			   char *dbnameBuf,
			   int bufLen);
    void GetIDForTrack(SPxUniTrack *track,
		       char *idBuf,
		       int bufLen);
    void ParseToHTML(FILE *logFile,
		     FILE* htmlFile);

}; /* SPxTrackNoteDb */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/


#endif /* _SPX_TRACK_NOTE_DB_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/

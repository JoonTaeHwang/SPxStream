/*********************************************************************
*
* (c) Copyright 2013 - 2022, Cambridge Pixel Ltd.
*
* Purpose:
*   Functions to play basic sounds.
*
* Revision Control:
*   27/04/22 v1.7    SP 	Add more alarm sounds.
*
* Previous Changes:
*   21/03/19 1.6    BTB	Expose SPxSoundPlayResource() and SPxSoundPlayFile() under Linux.
*   21/03/19 1.5    BTB	Support custom alarm sounds.
*   26/07/18 1.4    AGC	Add SPxSoundPlayFile().
*   07/06/18 1.3    SP 	Add sync flag to play functions.
*   18/05/18 1.2    SP 	Add SPxSoundPlayAlarm().
*   02/12/13 1.1    SP 	Initial version.
*
**********************************************************************/

#ifndef _SPX_SOUND_H
#define _SPX_SOUND_H

/*********************************************************************
*
*   Headers 
*
**********************************************************************/

/* Need error types. */
#include "SPxLibUtils/SPxError.h"
#include "SPxLibUtils/SPxTypes.h"

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

/* Built in alarm sounds. */
typedef enum
{
    SPX_ALARM_SOUND_1 = 0,
    SPX_ALARM_SOUND_2 = 1,
    SPX_ALARM_SOUND_3 = 2,
    SPX_ALARM_SOUND_4 = 3,
    SPX_ALARM_SOUND_5 = 4,
    SPX_NUM_ALARM_SOUNDS    /* Must be last entry. */

} SPxAlarmSound_t;

#define SPX_CUSTOM_ALARM_START_INDEX 100
#define SPX_NUM_CUSTOM_ALARM_SOUNDS 10

/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

/*
 * Extern the functions we provide.
 */

/* Enable / disable GUI control sounds by default. */
extern void SPxSoundSetGuiDefaultEnabled(int isEnabled);
extern int SPxSoundIsGuiDefaultEnabled(void);

/* Sound playing functions. */
extern SPxErrorCode SPxSoundPlayClick(int noStop=FALSE,
                                      int sync=FALSE);

extern SPxErrorCode SPxSoundPlayAlarm(SPxAlarmSound_t alarm,
                                      int noStop=FALSE,
                                      int sync=FALSE);

/* Windows-only functions. */
SPxErrorCode SPxSoundPlayResource(int resID,
                                  int noStop=FALSE,
                                  int sync=FALSE);

SPxErrorCode SPxSoundPlayFile(const char *filename,
                              int noStop=FALSE,
                              int sync=FALSE);

#endif /* _SPX_SOUND_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/

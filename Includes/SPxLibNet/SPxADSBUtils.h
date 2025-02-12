/*********************************************************************
*
* (c) Copyright 2011 - 2014, 2022, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxADSB utility functions.
*
*
* Revision Control:
*   09/03/22 v1.4    REW	Add SPxMode3AIs...() utility functions too.
*
* Previous Changes:
*   23/10/14 1.3    AGC	Use double type for feet argument.
*   22/10/14 1.2    AGC	Add encode functions.
*   15/10/14 1.1    REW	Initial Version, from part of SPxADSBDecoder.
**********************************************************************/

#ifndef _SPX_ADSB_UTILS_H
#define _SPX_ADSB_UTILS_H

/*
 * Other headers required.
 */
/* We need the definition of UINT16. */
#include "SPxLibUtils/SPxTypes.h"


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


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

/*
 * Extern the public functions.
 */
extern int SPxADSBModeCToFeet(UINT16 modeC);
extern UINT16 SPxADSBModeCFromFeet(double feet);
extern int SPxADSBGillhamToFeet(unsigned int gillhamValue);
extern unsigned int SPxADSBGillhamFromFeet(double feet);

/* Not strictly ADS-B, but related. */
extern int SPxMode3AIsEmergency(UINT16 mode3A);
extern int SPxMode3AIsGeneric(UINT16 mode3A);
extern int SPxMode3AIsSwitchExpected(UINT16 old3A, UINT16 new3A);

#endif /* SPX_ADSB_UTILS_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/

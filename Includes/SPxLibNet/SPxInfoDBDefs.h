/*********************************************************************
*
* (c) Copyright 2020, 2022, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for common definitions of SPxInfoDB item/field names.
*
*
* Revision Control:
*   19/01/22 v1.2    BTB	Update and add ADC no.
*
* Previous Changes:
*   22/10/20 1.1    REW	Initial Version.
**********************************************************************/

#ifndef _SPX_INFODB_DEFS_H
#define _SPX_INFODB_DEFS_H

/*
 * Other headers required.
 */

/*********************************************************************
 *
 *   Constants
 *
 *********************************************************************/

/*
 * Define commonly used item types.
 */
#define	SPX_INFODB_TYPE_NAME_FLIGHT	"FLIGHTINFO"

/*
 * Define commonly used field types.
 */
/* Fields used in TYPE_NAME_FLIGHT types. These must match dialog .rpi files.*/
#define	SPX_INFODB_FLIGHT_FIELD_CALLSIGN	"Call Sign"
#define	SPX_INFODB_FLIGHT_FIELD_DEPARTURE	"Departure"
#define	SPX_INFODB_FLIGHT_FIELD_DESTINATION	"Destination"
#define	SPX_INFODB_FLIGHT_FIELD_ETD		"ETD"
#define	SPX_INFODB_FLIGHT_FIELD_IFF		"IFF"
#define	SPX_INFODB_FLIGHT_FIELD_FLIGHT_TYPE	"Type of Flight"
#define	SPX_INFODB_FLIGHT_FIELD_ROUTE		"Route"
#define	SPX_INFODB_FLIGHT_FIELD_ADC_NUMBER	"ADC No"


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

#endif /* _SPX_INFODB_DEFS_H */

/*********************************************************************
*
*   End of File
*
**********************************************************************/

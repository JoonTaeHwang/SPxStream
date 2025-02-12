/*********************************************************************
*
* (c) Copyright 2012 - 2022, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxAsterixDecoder class.
*
*	This class can be used to decode an Asterix message stream.
*
*	This class parses the messages given to it and invokes 
*	callback functions according to the message categories found.
*
*
* Revision Control:
*   04/10/22 v1.56   REW	Support UAP code in SPxAsterixMsg too.
*
* Previous Changes:
*   03/10/22 1.55   REW	Support UAP code.
*   08/06/22 1.54   BTB	Add CAT-159 support.
*   24/05/22 1.53   AGC	Add separate relative velocity fields.
*   20/05/22 1.52   REW	Add processCat62Ref().
*   01/03/22 1.51   BTB	Add "permissive" mode (ignores certain errors).
*   14/01/22 1.50   AGC	Make GetExpectedSAC/SIC() const.
*   10/01/22 1.49   AGC	Support required CAT-21 V2.6 changes.
*   24/08/21 1.48   REW	Support SetIgnorePrefixBytes() etc.
*   14/05/21 1.47   BTB	Add UnmangleModeC().
*   09/04/21 1.46   AGC	Support CAT-34 SSR/Mode-S jamming strobes.
*   11/02/21 1.45   AGC	Complete P466 SPF support.
*   08/02/21 1.44   BTB	Add CAT-8 support.
*   30/10/20 1.43   AGC	Add lat/long format option to FormatDataItem().
*   29/10/20 1.42   AGC	Support P466 SPF fields.
*   27/10/20 1.41   REW	Add SPX_ASTERIX_ADS_LINK and ADS_MERIT defs.
*   13/10/20 1.40   SP 	Refactor FormatDataItem() and add support for CAT-62.
*   26/08/20 1.39   AGC	Support project code specific behaviour.
*			Support offset for property data access.
*   20/03/20 1.38   SP 	Add IsNogo().
*   20/01/20 1.37   REW	Support version in SPxAsterixMsg etc.
*   17/01/20 1.36   REW	Support CAT247 version receipt.
*   07/10/19 1.35   AGC	Support INT8 property values.
*   26/09/19 1.34   AGC	Support ASTERIX reserved expansion field formats.
*   05/09/19 1.33   AGC	Add protected constructor for testing.
*   05/02/19 1.32   REW	Correct the return value of GetExpectedSAC().
*   07/12/18 1.31   AGC	Fix N/C #642 - derive range/bearing from lat/long
*			when required.
*   22/11/18 1.30   AGC	Add required forward declarations.
*   02/10/18 1.29   AGC	Decode extents 4 and 5 of CAT-62 track status.
*   20/04/18 1.28   REW	Support CAT-34 strobes.
*   29/03/18 1.27   AGC	Extend Cat-62 support.
*   28/03/18 1.26   SP 	Add partial support for Cat-62.
*   09/03/18 1.25   AGC	CAT-17 improvements.
*   06/03/18 1.24   AGC	Initial CAT-17 decode and format support.
*			Improve radar time offset calcs.
*   06/03/18 1.23   AGC	Pass ASTERIX message to SPx status callback.
*			Support separator when formatting ASTERIX items.
*   02/02/18 1.22   AGC	Improve units support.
*   11/12/17 1.21   AGC	Pass ASTERIX message to SPx track callback.
*			Add SPxAsterixMsg::FormatDataItem().
*   22/11/17 1.20   AGC	Support saving SPF.
*   05/10/17 1.19   REW	Support CAT48 Extended Range Reports.
*   09/05/17 1.18   AGC	Add SPxAsterixMsg copy constructor.
*   23/05/16 1.17   AGC	Modify SPX_ASTERIX_DEC_DEBUG_ALL value.
*   16/03/16 1.16   AGC	Support extended IFF mode 1.
*   03/11/15 1.15   AGC	Packet decoder callback data now const.
*   06/10/15 1.14   AGC	Add CAT-21 MES decode support.
*   24/11/14 1.13   REW	Add more Cat4 bitmask definitions.
*   07/11/14 1.12   REW	Add Cat1 and Cat2 support.
*   24/09/14 1.11   REW	Complete Cat4 support.
*   15/09/14 1.10   REW	Start Cat4 support.
*   16/05/14 1.9    AGC	Skip over special purpose and reserved expansion fields.
*   10/04/14 1.8    AGC	Add assignment operator for SPxAsterixMsg.
*   26/09/13 1.7    REW	Add GetMsecsSinceLastMsg().
*   24/05/13 1.6    AGC	Add GetPropertyDataSize() function.
*   15/05/13 1.5    REW	Add Cat19 and Cat20 support.
*   03/05/13 1.4    AGC	Add Cat21 support.
*			Add debugging support.
*   25/04/13 1.3    REW	Add Cat10 support. Fix comment. Change names.
*   19/04/13 1.2    REW	Support network input with SPxDecoderRepeater.
*			Fix some duplicated property values.
*			Add SAC/SIC/Cat filtering and SPx handlers.
*   29/05/12 1.1    AGC	Initial Version.
**********************************************************************/

#ifndef _SPX_ASTERIX_DECODER_H
#define _SPX_ASTERIX_DECODER_H

/*
 * Other headers required.
 */
#include "SPxLibUtils/SPxAutoPtr.h"
#include "SPxLibUtils/SPxError.h"
#include "SPxLibUtils/SPxObj.h"
#include "SPxLibNet/SPxDecoderRepeater.h"	/* Our base class */

/*********************************************************************
*
*   Constants
*
**********************************************************************/

/* Asterix properties. */
enum SPxAsterixProperty
{
    SPX_ASTERIX_PROP_NONE		    = 0,

    /* Cat 34 (and others) */
    SPX_ASTERIX_MESSAGE_TYPE		    = 1,    /* UINT8 */
    SPX_ASTERIX_SAC			    = 2,    /* UINT8 */
    SPX_ASTERIX_SIC			    = 3,    /* UINT8 */
    SPX_ASTERIX_TIME_OF_DAY		    = 4,    /* UINT32 */
    SPX_ASTERIX_SECTOR_NUMBER		    = 5,    /* UINT8 */
    SPX_ASTERIX_ANTENNA_ROTATION_PERIOD	    = 6,    /* UINT16 */
    SPX_ASTERIX_STATUS_COMMON		    = 7,    /* UINT8 */
    SPX_ASTERIX_STATUS_PSR		    = 8,    /* UINT8 */
    SPX_ASTERIX_STATUS_SSR		    = 9,    /* UINT8 */
    SPX_ASTERIX_STATUS_MODE_S		    = 10,   /* UINT8 */
    SPX_ASTERIX_PROC_MODE_COMMON	    = 11,   /* UINT8 */
    SPX_ASTERIX_PROC_MODE_PSR		    = 12,   /* UINT8 */
    SPX_ASTERIX_PROC_MODE_SSR		    = 13,   /* UINT8 */
    SPX_ASTERIX_PROC_MODE_MODE_S	    = 14,   /* UINT8 */
    SPX_ASTERIX_COUNT_MISSES		    = 15,   /* UINT16 */
    SPX_ASTERIX_COUNT_PSR_HITS		    = 16,   /* UINT16 */
    SPX_ASTERIX_COUNT_SSR_HITS		    = 17,   /* UINT16 */
    SPX_ASTERIX_COUNT_SSR_PSR_HITS	    = 18,   /* UINT16 */
    SPX_ASTERIX_COUNT_ALL_CALL_HITS	    = 19,   /* UINT16 */
    SPX_ASTERIX_COUNT_ROLL_CALL_HITS	    = 20,   /* UINT16 */
    SPX_ASTERIX_COUNT_ALL_CALL_PSR_HITS	    = 21,   /* UINT16 */
    SPX_ASTERIX_COUNT_ROLL_CALL_PSR_HITS    = 22,   /* UINT16 */
    SPX_ASTERIX_COUNT_WEATHER_FILTER	    = 23,   /* UINT16 */
    SPX_ASTERIX_COUNT_JAMMING_FILTER	    = 24,   /* UINT16 */
    SPX_ASTERIX_COUNT_PSR_FILTER	    = 25,   /* UINT16 */
    SPX_ASTERIX_COUNT_SSR_FILTER	    = 26,   /* UINT16 */
    SPX_ASTERIX_COUNT_SSR_PSR_FILTER	    = 27,   /* UINT16 */
    SPX_ASTERIX_COUNT_SURV_FILTER	    = 28,   /* UINT16 */
    SPX_ASTERIX_COUNT_PSR_SURV_FILTER	    = 29,   /* UINT16 */
    SPX_ASTERIX_COUNT_PSR_SURV_SSR_FILTER   = 30,   /* UINT16 */
    SPX_ASTERIX_COUNT_PSR_SURV_ALL_SSR_FILTER = 31, /* UINT16 */
    SPX_ASTERIX_POLAR_WINDOW_RANGE_START    = 32,   /* UINT16 */
    SPX_ASTERIX_POLAR_WINDOW_RANGE_END	    = 33,   /* UINT16 */
    SPX_ASTERIX_POLAR_WINDOW_AZI_START	    = 34,   /* UINT16 */
    SPX_ASTERIX_POLAR_WINDOW_AZI_END	    = 35,   /* UINT16 */
    SPX_ASTERIX_FILTER_INVALID		    = 36,   /* UINT8 */
    SPX_ASTERIX_FILTER_WEATHER		    = 37,   /* UINT8 */
    SPX_ASTERIX_FILTER_JAMMING		    = 38,   /* UINT8 */
    SPX_ASTERIX_FILTER_PSR		    = 39,   /* UINT8 */
    SPX_ASTERIX_FILTER_SSR		    = 40,   /* UINT8 */
    SPX_ASTERIX_FILTER_SSR_PSR		    = 41,   /* UINT8 */
    SPX_ASTERIX_FILTER_SURV		    = 42,   /* UINT8 */
    SPX_ASTERIX_FILTER_PSR_SURV		    = 43,   /* UINT8 */
    SPX_ASTERIX_FILTER_PSR_SURV_SSR	    = 44,   /* UINT8 */
    SPX_ASTERIX_FILTER_PSR_SURV_ALL_SSR	    = 45,   /* UINT8 */
    SPX_ASTERIX_POS_HEIGHT		    = 46,   /* UINT16 */
    SPX_ASTERIX_POS_LATITUDE		    = 47,   /* UINT32 */
    SPX_ASTERIX_POS_LONGITUDE		    = 48,   /* UINT32 */
    SPX_ASTERIX_COLLIMATION_ERR_RANGE	    = 49,   /* UINT8 */
    SPX_ASTERIX_COLLIMATION_ERR_AZI	    = 50,   /* UINT8 */

    /* Cat 48 (and others). */
    SPX_ASTERIX_TARGET_REPORT		    = 51,   /* UINT8 */
    SPX_ASTERIX_TARGET_REPORT_TYPE	    = 52,   /* UINT8 */
    SPX_ASTERIX_TARGET_REPORT_EXT	    = 53,   /* UINT8 */
    SPX_ASTERIX_MEASURED_POS_RANGE	    = 54,   /* UINT16 */
    SPX_ASTERIX_MEASURED_POS_BEARING	    = 55,   /* UINT16 */
    SPX_ASTERIX_MODE_3			    = 56,   /* UINT16 */
    SPX_ASTERIX_FLIGHT_LEVEL		    = 57,   /* UINT16 */
    SPX_ASTERIX_SSR_PLOT_RUNLENGTH	    = 58,   /* UINT8 */
    SPX_ASTERIX_SSR_REPLIES_RECEIVED	    = 59,   /* UINT8 */
    SPX_ASTERIX_SSR_REPLY_AMPLITUDE	    = 60,   /* UINT8 */
    SPX_ASTERIX_PSR_PLOT_RUNLENGTH	    = 61,   /* UINT8 */
    SPX_ASTERIX_PSR_PLOT_AMPLITUDE	    = 62,   /* UINT8 */
    SPX_ASTERIX_PSR_SSR_DIFF_RANGE	    = 63,   /* UINT8 */
    SPX_ASTERIX_PSR_SSR_DIFF_AZI	    = 64,   /* UINT8 */
    SPX_ASTERIX_AIRCRAFT_ADDRESS	    = 65,   /* UINT32 */
    SPX_ASTERIX_AIRCRAFT_ID		    = 67,   /* NULL terminated string */
    SPX_ASTERIX_MODE_S_MB_DATA		    = 68,   /* GetPropertyData() */
    SPX_ASTERIX_TRACK_NUMBER		    = 69,   /* UINT16 */
    SPX_ASTERIX_CALC_POS_X		    = 70,   /* UINT16 */
    SPX_ASTERIX_CALC_POS_Y		    = 71,   /* UINT16 */
    SPX_ASTERIX_CALC_SPEED		    = 72,   /* UINT16 */
    SPX_ASTERIX_CALC_HEADING		    = 73,   /* UINT16 */
    SPX_ASTERIX_TRACK_STATUS		    = 74,   /* UINT8 */
    SPX_ASTERIX_TRACK_STATUS_EXT	    = 75,   /* UINT8 */
    SPX_ASTERIX_TRACK_QUAL_SIGMA_X	    = 76,   /* UINT8 */
    SPX_ASTERIX_TRACK_QUAL_SIGMA_Y	    = 77,   /* UINT8 */
    SPX_ASTERIX_TRACK_QUAL_SIGMA_V	    = 78,   /* UINT8 */
    SPX_ASTERIX_TRACK_QUAL_SIGMA_H	    = 79,   /* UINT8 */
    SPX_ASTERIX_WARNING_ERROR		    = 80,   /* GetPropertyData() */
    SPX_ASTERIX_MODE_3_CONF		    = 81,   /* UINT16 */
    SPX_ASTERIX_MODE_C_CONF		    = 82,   /* UINT32 */
    SPX_ASTERIX_MEASURED_3D_HEIGHT	    = 83,   /* UINT16 */
    SPX_ASTERIX_CALC_DOPPLER_SPEED_VALID    = 84,   /* UINT8 */
    SPX_ASTERIX_CALC_DOPPLER_SPEED	    = 85,   /* UINT8 */
    SPX_ASTERIX_RAW_DOPPLER_REP		    = 86,   /* UINT8 */
    SPX_ASTERIX_RAW_DOPPLER_SPEED	    = 87,   /* UINT16 */
    SPX_ASTERIX_RAW_DOPPLER_AMBIGUITY	    = 88,   /* UINT16 */
    SPX_ASTERIX_RAW_DOPPLER_TRANS_FREQ	    = 89,   /* UINT16 */
    SPX_ASTERIX_COMMS_STATUS		    = 90,   /* UINT16 */
    SPX_ASTERIX_ACAS_RES_ADV_REPORT	    = 91,   /* GetPropertyData() */
    SPX_ASTERIX_MODE_1			    = 92,   /* UINT8 */
    SPX_ASTERIX_MODE_1_CONF		    = 93,   /* UINT16 */
    SPX_ASTERIX_MODE_2			    = 94,   /* UINT8 */
    SPX_ASTERIX_MODE_2_CONF		    = 95,   /* UINT16 */
    SPX_ASTERIX_MODE_1_EXT		    = 96,   /* UINT16 */
    SPX_ASTERIX_EXTENDED_RANGE_REPORT       = 97,   /* UINT16 */
    SPX_ASTERIX_MES_MODE5_NAT		    = 98,   /* UINT16 */
    SPX_ASTERIX_MES_MODE5_FOM		    = 99,   /* UINT8 */

    /* Cat 10. */
    SPX_ASTERIX_TARGET_REPORT_EXT2	    = 100,  /* UINT8 */
    SPX_ASTERIX_CALC_VELOCITY_X		    = 101,  /* UINT16 */
    SPX_ASTERIX_CALC_VELOCITY_Y		    = 102,  /* UINT16 */
    SPX_ASTERIX_TRACK_STATUS_EXT2	    = 103,  /* UINT8 */
    SPX_ASTERIX_AIRCRAFT_ID_FLAGS	    = 104,  /* UINT8 */
    SPX_ASTERIX_VEHICLE_FLEET		    = 105,  /* UINT8 */
    SPX_ASTERIX_TARGET_SIZE_LENGTH	    = 106,  /* UINT8 */
    SPX_ASTERIX_TARGET_SIZE_ORIENT	    = 107,  /* UINT8 */
    SPX_ASTERIX_TARGET_SIZE_WIDTH	    = 108,  /* UINT8 */
    SPX_ASTERIX_SYSTEM_STATUS		    = 109,  /* UINT8 */
    SPX_ASTERIX_PREPROG_MSG		    = 110,  /* UINT8 */
    SPX_ASTERIX_TRACK_QUAL_COV		    = 111,  /* UINT16 */
    SPX_ASTERIX_PRESENCE		    = 112,  /* GetPropertyData() */
    SPX_ASTERIX_CALC_ACCEL_X		    = 113,  /* UINT8 */
    SPX_ASTERIX_CALC_ACCEL_Y		    = 114,  /* UINT8 */

    /* Cat 21. */
    SPX_ASTERIX_SERVICE_ID		    = 115,  /* UINT8 */
    SPX_ASTERIX_TIME_POSITION		    = 116,  /* UINT32 */
    SPX_ASTERIX_POS_LATITUDE_HIGH_PREC	    = 117,  /* UINT32 */
    SPX_ASTERIX_POS_LONGITUDE_HIGH_PREC	    = 118,  /* UINT32 */
    SPX_ASTERIX_TIME_VELOCITY		    = 119,  /* UINT32 */
    SPX_ASTERIX_AIR_SPEED		    = 120,  /* UINT16 */
    SPX_ASTERIX_TRUE_AIR_SPEED		    = 121,  /* UINT16 */
    SPX_ASTERIX_MSG_TIME_POSITION	    = 122,  /* UINT32 */
    SPX_ASTERIX_MSG_TIME_POSITION_HIGH_PREC = 123,  /* UINT32 */
    SPX_ASTERIX_MSG_TIME_VELOCITY	    = 124,  /* UINT32 */
    SPX_ASTERIX_MSG_TIME_VELOCITY_HIGH_PREC = 125,  /* UINT32 */
    SPX_ASTERIX_GEOMETRIC_HEIGHT	    = 126,  /* UINT16 */
    SPX_ASTERIX_QUAL_IND		    = 127,  /* UINT8 */
    SPX_ASTERIX_QUAL_IND_EXT		    = 128,  /* UINT8 */
    SPX_ASTERIX_QUAL_IND_EXT2		    = 129,  /* UINT8 */
    SPX_ASTERIX_QUAL_IND_EXT3		    = 130,  /* UINT8 */
    SPX_ASTERIX_MOPS_VERSION		    = 131,  /* UINT8 */
    SPX_ASTERIX_ROLL_ANGLE		    = 132,  /* UINT16 */
    SPX_ASTERIX_MAGNETIC_HEADING	    = 133,  /* UINT16 */
    SPX_ASTERIX_TARGET_STATUS		    = 134,  /* UINT8 */
    SPX_ASTERIX_BAROMETRIC_VERTICAL_RATE    = 135,  /* UINT16 */
    SPX_ASTERIX_GEOMETRIC_VERTICAL_RATE	    = 136,  /* UINT16 */
    SPX_ASTERIX_AIRBORNE_GROUND_SPEED	    = 137,  /* UINT16 */
    SPX_ASTERIX_AIRBORNE_TRACK_ANGLE	    = 138,  /* UINT16 */
    SPX_ASTERIX_TRACK_ANGLE_RATE	    = 139,  /* UINT16 */
    SPX_ASTERIX_EMITTER_CATEGORY	    = 140,  /* UINT8 */
    SPX_ASTERIX_WIND_SPEED		    = 141,  /* UINT16 */
    SPX_ASTERIX_WIND_DIRECTION		    = 142,  /* UINT16 */
    SPX_ASTERIX_TEMPERATURE		    = 143,  /* UINT16 */
    SPX_ASTERIX_TURBULENCE		    = 144,  /* UINT8 */
    SPX_ASTERIX_SELECTED_ALTITUDE	    = 145,  /* UINT16 */
    SPX_ASTERIX_FINAL_STATE_SELECTED_ALTITUDE = 146,/* UINT16 */
    SPX_ASTERIX_SERVICE_MANAGEMENT	    = 147,  /* UINT8 */
    SPX_ASTERIX_AIRCRAFT_OP_STATUS	    = 148,  /* UINT8 */
    SPX_ASTERIX_SURFACE_CAPABILITIES	    = 149,  /* UINT8 */
    SPX_ASTERIX_SURFACE_CAPABILITIES_EXT    = 150,  /* UINT8 */
    SPX_ASTERIX_MSG_AMPLITUDE		    = 151,  /* UINT8 */
    SPX_ASTERIX_RECEIVER_ID		    = 152,  /* UINT8 */
    SPX_ASTERIX_AIRCRAFT_OP_STATUS_AGE	    = 153,  /* UINT8 */
    SPX_ASTERIX_TARGET_REPORT_AGE	    = 154,  /* UINT8 */
    SPX_ASTERIX_MODE_3_AGE		    = 155,  /* UINT8 */
    SPX_ASTERIX_QUAL_IND_AGE		    = 156,  /* UINT8 */
    SPX_ASTERIX_MSG_AMPLITUDE_AGE	    = 157,  /* UINT8 */
    SPX_ASTERIX_GEOMETRIC_HEIGHT_AGE	    = 158,  /* UINT8 */
    SPX_ASTERIX_FLIGHT_LEVEL_AGE	    = 159,  /* UINT8 */
    SPX_ASTERIX_SELECTED_ALTITUDE_AGE	    = 160,  /* UINT8 */
    SPX_ASTERIX_FINAL_STATE_SELECTED_ALTITUDE_AGE = 161, /* UINT8 */
    SPX_ASTERIX_AIR_SPEED_AGE		    = 162,  /* UINT8 */
    SPX_ASTERIX_TRUE_AIR_SPEED_AGE	    = 163,  /* UINT8 */
    SPX_ASTERIX_MAGNETIC_HEADING_AGE	    = 164,  /* UINT8 */
    SPX_ASTERIX_BAROMETRIC_VERTICAL_RATE_AGE = 165, /* UINT8 */
    SPX_ASTERIX_GEOMETRIC_VERTICAL_RATE_AGE = 166,  /* UINT8 */
    SPX_ASTERIX_AIRBORNE_AGE		    = 167,  /* UINT8 */
    SPX_ASTERIX_TRACK_ANGLE_RATE_AGE	    = 168,  /* UINT8 */
    SPX_ASTERIX_AIRCRAFT_ID_AGE		    = 169,  /* UINT8 */
    SPX_ASTERIX_TARGET_STATUS_AGE	    = 170,  /* UINT8 */
    SPX_ASTERIX_MET_INFO_AGE		    = 171,  /* UINT8 */
    SPX_ASTERIX_ROLL_ANGLE_AGE		    = 172,  /* UINT8 */
    SPX_ASTERIX_ACAS_RES_ADV_REPORT_AGE	    = 173,  /* UINT8 */
    SPX_ASTERIX_SURFACE_CAPABILITIES_AGE    = 174,  /* UINT8 */
    SPX_ASTERIX_BAROMETRIC_PRESSURE	    = 175,  /* UINT16 */
    SPX_ASTERIX_SELECTED_HEADING	    = 176,  /* UINT16 */
    SPX_ASTERIX_NAVIGATION_MODE		    = 177,  /* UINT8 */
    SPX_ASTERIX_GPS_OFFSET		    = 178,  /* UINT8 */
    SPX_ASTERIX_SURFACE_GROUND_SPEED	    = 179,  /* UINT16 */
    SPX_ASTERIX_SURFACE_TRACK_ANGLE	    = 180,  /* UINT8 */
    SPX_ASTERIX_AIRCRAFT_STATUS		    = 181,  /* UINT8 */
    SPX_ASTERIX_TRUE_HEADING		    = 182,  /* UINT16 */
    SPX_ASTERIX_MES_MODE5_SUMMARY	    = 183,  /* UINT8 */
    SPX_ASTERIX_MES_MODE5_PIN		    = 184,  /* UINT32 */
    SPX_ASTERIX_MES_MODE1		    = 185,  /* UINT16 */
    SPX_ASTERIX_MES_X_PULSE		    = 186,  /* UINT8 */
    SPX_ASTERIX_MES_MERIT		    = 187,  /* UINT8 */
    SPX_ASTERIX_MES_MODE2		    = 188,  /* UINT16 */
    SPX_ASTERIX_TRAJ_INTENT_STATUS	    = 189,  /* UINT8 */
    SPX_ASTERIX_TRAJ_INTENT_DATA	    = 190,  /* GetPropertyData() */
    SPX_ASTERIX_TRAJ_INTENT_AGE		    = 191,  /* UINT8 */
    SPX_ASTERIX_TARGET_REPORT_OLD	    = 192,  /* UINT16 */
    SPX_ASTERIX_ADS_MERIT                   = 193,  /* UINT16 */
    SPX_ASTERIX_ADS_LINK                    = 194,  /* UINT8 */
    SPX_ASTERIX_AIRCRAFT_STATUS_EXT	    = 195,  /* UINT8 */

    /* Cat 19 (many of the above generic ones are used too). */
    SPX_ASTERIX_PROCESSOR_STATUS	    = 200,  /* UINT8 */
    SPX_ASTERIX_SENSOR_STATUS		    = 201,  /* GetPropertyData() */
    SPX_ASTERIX_REF_TRANSPONDER_STATUS	    = 202,  /* GetPropertyData() */
    SPX_ASTERIX_WGS84_UNDULATION	    = 203,  /* UINT8 */

    /* Cat 20 (many of the above generic ones are used too). */
    SPX_ASTERIX_CALC_POS_X_HIGH_PREC	    = 210,  /* UINT32 */
    SPX_ASTERIX_CALC_POS_Y_HIGH_PREC	    = 211,  /* UINT32 */
    SPX_ASTERIX_POS_ACCURACY_FLAGS	    = 212,  /* UINT8 */
    SPX_ASTERIX_POS_ACCURACY_DOP_X	    = 213,  /* UINT16 */
    SPX_ASTERIX_POS_ACCURACY_DOP_Y	    = 214,  /* UINT16 */
    SPX_ASTERIX_POS_ACCURACY_DOP_XY	    = 215,  /* UINT16 */
    SPX_ASTERIX_POS_ACCURACY_SD_X	    = 216,  /* UINT16 */
    SPX_ASTERIX_POS_ACCURACY_SD_Y	    = 217,  /* UINT16 */
    SPX_ASTERIX_POS_ACCURACY_SD_XY	    = 218,  /* UINT16 */
    SPX_ASTERIX_POS_ACCURACY_SD_GH	    = 219,  /* UINT16 */
    SPX_ASTERIX_CONTRIBUTING_RECEIVERS	    = 220,  /* GetPropertyData() */

    /* Cat 4 (some of the above more generic ones are used too). */
    SPX_ASTERIX_SDPS_IDENTIFIER		    = 230,  /* GetPropertyData() */
    SPX_ASTERIX_SDPS_CONTROLLER_IDENTIFIER  = 231,  /* GetPropertyData() */
    SPX_ASTERIX_ALERT_IDENTIFIER	    = 232,  /* UINT16 */
    SPX_ASTERIX_ALERT_STATUS		    = 233,  /* UINT8 */
    SPX_ASTERIX_SAFETY_NET_STATUS	    = 234,  /* UINT8 */
    SPX_ASTERIX_SAFETY_NET_STATUS_EXT	    = 235,  /* UINT8 */
    SPX_ASTERIX_SN_CONFLICT_FLAGS	    = 240,  /* UINT8 */
    SPX_ASTERIX_SN_CONFLICT_FLAGS_EXT	    = 241,  /* UINT8 */
    SPX_ASTERIX_SN_CONFLICT_CLASS	    = 242,  /* UINT8 */
    SPX_ASTERIX_SN_CONFLICT_PROBABILITY	    = 243,  /* UINT8 */
    SPX_ASTERIX_SN_CONFLICT_DURATION	    = 244,  /* UINT32 */
    SPX_ASTERIX_SN_TIME_TO_CONFLICT	    = 245,  /* UINT32 */
    SPX_ASTERIX_SN_TIME_TO_CPA		    = 246,  /* UINT32 */
    SPX_ASTERIX_SN_CURRENT_HORIZ_SEP	    = 250,  /* UINT32 */
    SPX_ASTERIX_SN_MIN_HORIZ_SEP	    = 251,  /* UINT16 */
    SPX_ASTERIX_SN_CURRENT_VERT_SEP	    = 252,  /* UINT16 */
    SPX_ASTERIX_SN_MIN_VERT_SEP		    = 253,  /* UINT16 */
    SPX_ASTERIX_VERTICAL_DEVIATION	    = 254,  /* UINT16 */
    SPX_ASTERIX_LONGITUDINAL_DEVIATION	    = 255,  /* UINT16 */
    SPX_ASTERIX_TRANSVERSAL_DEVIATION	    = 256,  /* UINT32 */
    SPX_ASTERIX_SN_PRED_POS_LAT_1	    = 260,  /* UINT32 */
    SPX_ASTERIX_SN_PRED_POS_LONG_1	    = 261,  /* UINT32 */
    SPX_ASTERIX_SN_PRED_POS_ALT_1	    = 262,  /* UINT16 */
    SPX_ASTERIX_SN_PRED_POS_X_1		    = 263,  /* UINT32 */
    SPX_ASTERIX_SN_PRED_POS_Y_1		    = 264,  /* UINT32 */
    SPX_ASTERIX_SN_TIME_TO_THRESH_1	    = 265,  /* UINT32 */
    SPX_ASTERIX_SN_DIST_TO_THRESH_1	    = 266,  /* UINT16 */
    SPX_ASTERIX_SN_AC_CHARACTERISTICS_1	    = 267,  /* UINT8 */
    SPX_ASTERIX_SN_AC_CHARACTERISTICS_EXT_1 = 268,  /* UINT8 */
    SPX_ASTERIX_SN_MODE_S_ID_1		    = 269,  /* NULL terminated string */
    SPX_ASTERIX_SN_FLIGHT_PLAN_NUM_1	    = 270,  /* UINT32 */
    SPX_ASTERIX_SN_CLEARED_LEVEL_1	    = 271,  /* UINT32 */
    SPX_ASTERIX_TRACK_NUMBER_2		    = 280,  /* UINT16 */
    SPX_ASTERIX_AIRCRAFT_ID_2		    = 281,  /* NULL terminated string */
    SPX_ASTERIX_MODE_3_2		    = 282,  /* UINT16 */
    SPX_ASTERIX_SN_PRED_POS_LAT_2	    = 283,  /* UINT32 */
    SPX_ASTERIX_SN_PRED_POS_LONG_2	    = 284,  /* UINT32 */
    SPX_ASTERIX_SN_PRED_POS_ALT_2	    = 285,  /* UINT16 */
    SPX_ASTERIX_SN_PRED_POS_X_2		    = 286,  /* UINT32 */
    SPX_ASTERIX_SN_PRED_POS_Y_2		    = 287,  /* UINT32 */
    SPX_ASTERIX_SN_TIME_TO_THRESH_2	    = 288,  /* UINT32 */
    SPX_ASTERIX_SN_DIST_TO_THRESH_2	    = 289,  /* UINT16 */
    SPX_ASTERIX_SN_AC_CHARACTERISTICS_2	    = 290,  /* UINT8 */
    SPX_ASTERIX_SN_AC_CHARACTERISTICS_EXT_2 = 291,  /* UINT8 */
    SPX_ASTERIX_SN_MODE_S_ID_2		    = 292,  /* NULL terminated string */
    SPX_ASTERIX_SN_FLIGHT_PLAN_NUM_2	    = 293,  /* UINT32 */
    SPX_ASTERIX_SN_CLEARED_LEVEL_2	    = 294,  /* UINT32 */
    SPX_ASTERIX_AREA_NAME		    = 300,  /* NULL terminated string */
    SPX_ASTERIX_CROSSING_AREA_NAME	    = 301,  /* NULL terminated string */
    SPX_ASTERIX_RUNWAY_DESIGNATOR_1	    = 302,  /* NULL terminated string */
    SPX_ASTERIX_RUNWAY_DESIGNATOR_2	    = 303,  /* NULL terminated string */
    SPX_ASTERIX_STOP_BAR_DESIGNATOR	    = 304,  /* NULL terminated string */
    SPX_ASTERIX_GATE_DESIGNATOR		    = 305,  /* NULL terminated string */

    /* Cat 1. */
    SPX_ASTERIX_TIME_OF_DAY_TRUNCATED	    = 350,  /* UINT16 */
    SPX_ASTERIX_PLOT_CHARACTERISTICS	    = 351,  /* GetPropertyData() */
    SPX_ASTERIX_MEASURED_DOPPLER	    = 352,  /* UINT8 */
    SPX_ASTERIX_TRACK_QUALITY		    = 353,  /* GetPropertyData() */
    SPX_ASTERIX_X_PULSE			    = 354,  /* UINT8 */

    /* Cat 2. */
    SPX_ASTERIX_STATION_STATUS		    = 360,  /* GetPropertyData() */
    SPX_ASTERIX_STATION_MODE		    = 361,  /* GetPropertyData() */

    /* Cat 17. */
    SPX_ASTERIX_DEST_SAC		    = 370,  /* UINT8 */
    SPX_ASTERIX_DEST_SIC		    = 371,  /* UINT8 */
    SPX_ASTERIX_CLUSTER_NODE_LIST	    = 372,  /* GetPropertyData() */
    SPX_ASTERIX_DUPLICATE_ADDRESS_REF_NUM   = 373,  /* UINT16 */
    SPX_ASTERIX_TRANSPONDER_CAPABILITY	    = 374,  /* UINT8 */
    SPX_ASTERIX_MODE_S_LIST		    = 375,  /* GetPropertyData() */
    SPX_ASTERIX_CLUSTER_STATE		    = 376,  /* UINT8 */

    /* Cat 62. */
    SPX_ASTERIX_STI			    = 380,  /* UINT8 */
    SPX_ASTERIX_TRACK_STATUS_EXT3	    = 381,  /* UINT8 */
    SPX_ASTERIX_TRACK_AGE		    = 382,  /* UINT8 */
    SPX_ASTERIX_PSR_AGE			    = 383,  /* UINT8 */
    SPX_ASTERIX_SSR_AGE			    = 384,  /* UINT8 */
    SPX_ASTERIX_MODE_S_AGE		    = 385,  /* UINT8 */
    SPX_ASTERIX_ADS_C_AGE		    = 386,  /* UINT8 */
    SPX_ASTERIX_ES_AGE			    = 387,  /* UINT8 */
    SPX_ASTERIX_VDL_AGE			    = 388,  /* UINT8 */
    SPX_ASTERIX_UAT_AGE			    = 389,  /* UINT8 */
    SPX_ASTERIX_LOOP_AGE		    = 390,  /* UINT8 */
    SPX_ASTERIX_MULTILAT_AGE		    = 391,  /* UINT8 */
    SPX_ASTERIX_MODE_OF_MOVEMENT	    = 392,  /* UINT8 */
    SPX_ASTERIX_MEASURED_FLIGHT_LEVEL_AGE   = 393,  /* UINT8 */
    SPX_ASTERIX_MODE_1_AGE		    = 394,  /* UINT8 */
    SPX_ASTERIX_MODE_2_AGE		    = 395,  /* UINT8 */
    SPX_ASTERIX_MODE_4_AGE		    = 396,  /* UINT8 */
    SPX_ASTERIX_MODE_5_AGE		    = 397,  /* UINT8 */
    SPX_ASTERIX_AIR_SPEED_MACH_AGE	    = 398,  /* UINT8 */
    SPX_ASTERIX_COMMS_STATUS_AGE	    = 399,  /* UINT8 */
    SPX_ASTERIX_ADSB_STATUS_AGE		    = 400,  /* UINT8 */
    SPX_ASTERIX_TRACK_ANGLE_AGE		    = 401,  /* UINT8 */
    SPX_ASTERIX_GROUND_SPEED_AGE	    = 402,  /* UINT8 */
    SPX_ASTERIX_VELOCITY_UNCERTAINTY_AGE    = 403,  /* UINT8 */
    SPX_ASTERIX_EMITTER_CATEGORY_AGE	    = 404,  /* UINT8 */
    SPX_ASTERIX_POS_AGE			    = 405,  /* UINT8 */
    SPX_ASTERIX_POS_UNCERTAINTY_AGE	    = 406,  /* UINT8 */
    SPX_ASTERIX_MODE_S_MB_DATA_AGE	    = 407,  /* UINT8 */
    SPX_ASTERIX_MACH_NUMBER_AGE		    = 408,  /* UINT8 */
    SPX_ASTERIX_BAROMETRIC_PRESSURE_AGE	    = 409,  /* UINT8 */
    SPX_ASTERIX_MEASURED_FLIGHT_LEVEL	    = 410,  /* UINT16 */
    SPX_ASTERIX_CALC_GEOMETRIC_ALTITUDE	    = 411,  /* UINT16 */
    SPX_ASTERIX_CALC_BAROMETRIC_ALTITUDE    = 412,  /* UINT16 */
    SPX_ASTERIX_CALC_RATE_OF_CLIMB	    = 413,  /* UINT16 */
    SPX_ASTERIX_FLIGHT_PLAN_SAC		    = 414,  /* UINT8 */
    SPX_ASTERIX_FLIGHT_PLAN_SIC		    = 415,  /* UINT8 */
    SPX_ASTERIX_FLIGHT_PLAN_CALLSIGN	    = 416,  /* NULL terminated string */
    SPX_ASTERIX_FLIGHT_PLAN_ID		    = 417,  /* UINT32 */
    SPX_ASTERIX_FLIGHT_PLAN_CATEGORY	    = 418,  /* UINT8 */
    SPX_ASTERIX_FLIGHT_PLAN_AIRCRAFT_TYPE   = 419,  /* NULL terminated string */
    SPX_ASTERIX_FLIGHT_PLAN_WAKE_CATEGORY   = 420,  /* UINT8 */
    SPX_ASTERIX_FLIGHT_PLAN_DEP_AIRPORT	    = 421,  /* NULL terminated string */
    SPX_ASTERIX_FLIGHT_PLAN_DEST_AIRPORT    = 422,  /* NULL terminated string */
    SPX_ASTERIX_FLIGHT_PLAN_RUNWAY	    = 423,  /* NULL terminated string */
    SPX_ASTERIX_FLIGHT_PLAN_FLIGHT_LEVEL    = 424,  /* UINT16 */
    SPX_ASTERIX_FLIGHT_PLAN_CONTROL_POS	    = 425,  /* UINT16 */
    SPX_ASTERIX_FLIGHT_PLAN_TIMES	    = 426,  /* GetPropertyData() */
    SPX_ASTERIX_FLIGHT_PLAN_STAND	    = 427,  /* NULL terminated string */
    SPX_ASTERIX_FLIGHT_PLAN_STAND_STATUS    = 428,  /* UINT8 */
    SPX_ASTERIX_FLIGHT_PLAN_SID		    = 429,  /* NULL terminated string */
    SPX_ASTERIX_FLIGHT_PLAN_STAR	    = 430,  /* NULL terminated string */
    SPX_ASTERIX_FLIGHT_PLAN_PRE_EMERG_MODE_3 = 431,  /* UINT16 */
    SPX_ASTERIX_FLIGHT_PLAN_PRE_EMERG_CALLSIGN = 432,  /* NULL terminated string */
    SPX_ASTERIX_FLEET_ID		    = 433,  /* UINT8 */
    SPX_ASTERIX_MES_MODE5_POS_LATITUDE	    = 434,  /* UINT32 */
    SPX_ASTERIX_MES_MODE5_POS_LONGITUDE	    = 435,  /* UINT32 */
    SPX_ASTERIX_MES_MODE5_ALTITUDE	    = 436,  /* UINT16 */
    SPX_ASTERIX_MES_MODE5_TIME_OFFSET	    = 437,  /* UINT8 */
    SPX_ASTERIX_COMPOSED_TRACK_NUMBER	    = 438,  /* GetPropertyData() */
    SPX_ASTERIX_POS_ACCURACY_SD_LAT	    = 439,  /* UINT16 */
    SPX_ASTERIX_POS_ACCURACY_SD_LON	    = 440,  /* UINT16 */
    SPX_ASTERIX_POS_ACCURACY_SD_GEOM_ALT    = 441,  /* UINT8 */
    SPX_ASTERIX_POS_ACCURACY_SD_BARO_ALT    = 442,  /* UINT8 */
    SPX_ASTERIX_VEL_ACCURACY_SD_X	    = 443,  /* UINT8 */
    SPX_ASTERIX_VEL_ACCURACY_SD_Y	    = 444,  /* UINT8 */
    SPX_ASTERIX_ACC_ACCURACY_SD_X	    = 445,  /* UINT8 */
    SPX_ASTERIX_ACC_ACCURACY_SD_Y	    = 446,  /* UINT8 */
    SPX_ASTERIX_VEL_ACCURACY_SD_VR	    = 447,  /* UINT8 */
    SPX_ASTERIX_SENSOR_SAC		    = 448,  /* UINT8 */
    SPX_ASTERIX_SENSOR_SIC		    = 449,  /* UINT8 */
    SPX_ASTERIX_MEASURED_MODE_C		    = 450,  /* UINT16 */
    SPX_ASTERIX_MEASURED_MODE_3		    = 451,  /* UINT16 */
    SPX_ASTERIX_MEASURED_REPORT_TYPE	    = 452,  /* UINT8 */
    SPX_ASTERIX_TRACK_STATUS_EXT4	    = 453,  /* UINT8 */
    SPX_ASTERIX_TRACK_STATUS_EXT5	    = 454,  /* UINT8 */
    SPX_ASTERIX_CALC_VELOCITY_REL_X	    = 455,  /* UINT16 */
    SPX_ASTERIX_CALC_VELOCITY_REL_Y	    = 456,  /* UINT16 */

    /* General purpose. */
    SPX_ASTERIX_SPF			    = 500,  /* GetPropertyData() */

    /* Cat 62. */
    SPX_ASTERIX_VERSIONS		    = 510,  /* GetPropertyData() */

    /* Cat 8. */
    SPX_ASTERIX_VECTOR_QUALIFIER	    = 600,  /* UINT8 */
    SPX_ASTERIX_VECTOR_QUALIFIER_EXT	    = 601,  /* UINT8 */
    SPX_ASTERIX_CARTESIAN_VECTOR_DATA	    = 602,  /* GetPropertyData() */
    SPX_ASTERIX_POLAR_VECTOR_DATA	    = 603,  /* GetPropertyData() */
    SPX_ASTERIX_CONTOUR_IDENTIFIER	    = 604,  /* UINT16 */
    SPX_ASTERIX_CONTOUR_DATA		    = 605,  /* GetPropertyData() */
    SPX_ASTERIX_PROC_STATUS		    = 606,  /* UINT32 */
    SPX_ASTERIX_STATION_CONFIG_STATUS	    = 607,  /* UINT8 */
    SPX_ASTERIX_TOTAL_PICTURE_ITEMS	    = 608,  /* UINT16 */
    SPX_ASTERIX_WEATHER_VECTOR_DATA	    = 609,  /* GetPropertyData() */

    /* Cat 159. */
    SPX_ASTERIX_TARGET_REPORT_EXT3	    = 700,  /* UINT8 */
    SPX_ASTERIX_DC_RAW_DOPPLER_SPEED	    = 701,  /* UINT16 */

    /* P466 SPF fields. */
    SPX_ASTERIX_SPF_P466_SYS_TRACK	    = 4661, /* UINT16 */
    SPX_ASTERIX_SPF_P466_IDENTITY	    = 4662, /* UINT8 */
    SPX_ASTERIX_SPF_P466_QUALITY	    = 4663, /* UINT8 */
    SPX_ASTERIX_SPF_P466_M6		    = 4664, /* UINT8 */
    SPX_ASTERIX_SPF_P466_CLASSIFICATION	    = 4665, /* UINT8 */
    SPX_ASTERIX_SPF_P466_ALTITUDE	    = 4666, /* UINT16 */
    SPX_ASTERIX_SPF_P466_LATITUDE	    = 4667, /* UINT32 */
    SPX_ASTERIX_SPF_P466_LONGITUDE	    = 4668, /* UINT32 */
    SPX_ASTERIX_SPF_P466_SPEED		    = 4669, /* UINT16 */
    SPX_ASTERIX_SPF_P466_HEADING	    = 4670, /* UINT16 */
    SPX_ASTERIX_SPF_P466_CALLSIGN	    = 4671, /* NULL terminated string */
    SPX_ASTERIX_SPF_P466_PLAT_TYPE	    = 4672, /* NULL terminated string */

    /* Dummy to provide a last entry without a trailing comma. */
    SPX_ASTERIX_PROP_LAST		    = 9999  /* Do not use */
};

/* CAT2 Message Type (I002/000). */
enum SPxAsterix2MsgType
{
    SPX_ASTERIX2_MSG_TYPE_NORTH_MARKER = 1,
    SPX_ASTERIX2_MSG_TYPE_SECTOR_CROSSING = 2,
    SPX_ASTERIX2_MSG_TYPE_SOUTH_MARKER = 3,
    SPX_ASTERIX2_MSG_TYPE_BLIND_ZONE_ACTIVE = 8,
    SPX_ASTERIX2_MSG_TYPE_BLIND_ZONE_STOP = 9
};

/* CAT4 Message Type (I004/000). */
enum SPxAsterix4MsgType
{
    SPX_ASTERIX4_MSG_TYPE_ALIVE = 1,
    SPX_ASTERIX4_MSG_TYPE_RAMLD = 2,
    SPX_ASTERIX4_MSG_TYPE_RAMHD = 3,
    SPX_ASTERIX4_MSG_TYPE_MSAW = 4,
    SPX_ASTERIX4_MSG_TYPE_APW = 5,
    SPX_ASTERIX4_MSG_TYPE_CLAM = 6,
    SPX_ASTERIX4_MSG_TYPE_STCA = 7,
    SPX_ASTERIX4_MSG_TYPE_AFDA = 8,
    SPX_ASTERIX4_MSG_TYPE_RIMCAS_ALM = 9,
    SPX_ASTERIX4_MSG_TYPE_RIMCAS_WRA = 10,
    SPX_ASTERIX4_MSG_TYPE_RIMCAS_OTA = 11,
    SPX_ASTERIX4_MSG_TYPE_RIMCAS_RDM = 12,
    SPX_ASTERIX4_MSG_TYPE_RIMCAS_RCM = 13,
    SPX_ASTERIX4_MSG_TYPE_RIMCAS_TSM = 14,
    SPX_ASTERIX4_MSG_TYPE_RIMCAS_UTMM = 15,
    SPX_ASTERIX4_MSG_TYPE_RIMCAS_SBOA = 16,
    SPX_ASTERIX4_MSG_TYPE_EOC = 17,
    SPX_ASTERIX4_MSG_TYPE_ACASRA = 18,
    SPX_ASTERIX4_MSG_TYPE_NTCA = 19
};

/* CAT8 Message Type (I008/000). */
enum SPxAsterix8MsgType
{
    SPX_ASTERIX8_MSG_TYPE_POLAR_VECTOR = 1,
    SPX_ASTERIX8_MSG_TYPE_CARTESIAN_VECTOR_START_LENGTH = 2,
    SPX_ASTERIX8_MSG_TYPE_CONTOUR_RECORD = 3,
    SPX_ASTERIX8_MSG_TYPE_CARTESIAN_VECTOR_START_END = 4,
    SPX_ASTERIX8_MSG_TYPE_SOP = 254,
    SPX_ASTERIX8_MSG_TYPE_EOP = 255
};

/* CAT10 Message Type (I010/000). */
enum SPxAsterix10MsgType
{
    SPX_ASTERIX10_MSG_TYPE_TARGET = 1,
    SPX_ASTERIX10_MSG_TYPE_CYCLE_START = 2,
    SPX_ASTERIX10_MSG_TYPE_PERIODIC_STATUS = 3,
    SPX_ASTERIX10_MSG_TYPE_EVENT_STATUS = 4
};

/* CAT17 Message Type (I017/000). */
enum SPxAsterix17MsgType
{
    SPX_ASTERIX17_MSG_TYPE_NET_INFO = 0,
    SPX_ASTERIX17_MSG_TYPE_TRACK_DATA = 10,
    SPX_ASTERIX17_MSG_TYPE_TRACK_DATA_REQ = 20,
    SPX_ASTERIX17_MSG_TYPE_TRACK_DATA_STOP = 21,
    SPX_ASTERIX17_MSG_TYPE_CANCEL_TRACK_DATA_REQ = 22,
    SPX_ASTERIX17_MSG_TYPE_TRACK_DATA_STOP_ACK = 23,
    SPX_ASTERIX17_MSG_TYPE_NEW_NODE_FINAL = 31,
    SPX_ASTERIX17_MSG_TYPE_NEW_NODE_INITIAL_REPLY = 32,
    SPX_ASTERIX17_MSG_TYPE_NEW_NODE_FINAL_REPLY = 33,
    SPX_ASTERIX17_MSG_TYPE_MOVE_NODE_TO_NEW_CLUSTER_STATE = 110,
    SPX_ASTERIX17_MSG_TYPE_MOVE_NODE_TO_NEW_CLUSTER_STATE_ACK = 111
};

/* CAT19 Message Type (I019/000). */
enum SPxAsterix19MsgType
{
    SPX_ASTERIX19_MSG_TYPE_CYCLE_START = 1,
    SPX_ASTERIX19_MSG_TYPE_PERIODIC_STATUS = 2,
    SPX_ASTERIX19_MSG_TYPE_EVENT_STATUS = 3
};

/* CAT34 Message Type (I034/000). */
enum SPxAsterix34MsgType
{
    SPX_ASTERIX34_MSG_TYPE_NORTH_MARKER = 1,
    SPX_ASTERIX34_MSG_TYPE_SECTOR_CROSSING = 2,
    SPX_ASTERIX34_MSG_TYPE_GEO_FILTER = 3,
    SPX_ASTERIX34_MSG_TYPE_JAMMING_STROBE = 4,
    SPX_ASTERIX34_MSG_TYPE_SOLAR_STORM = 5,
    SPX_ASTERIX34_MSG_TYPE_SSR_JAMMING_STROBE = 6,
    SPX_ASTERIX34_MSG_TYPE_MODES_JAMMING_STROBE = 7
};

/* CAT48 Target Report Descriptor Type (I048/020). */
enum SPxAsterix48TargetReportType
{
    SPX_ASTERIX48_TGT_RPT_TYPE_NO_DETECTION	= 0x0,
    SPX_ASTERIX48_TGT_RPT_TYPE_SINGLE_PSR	= 0x1,
    SPX_ASTERIX48_TGT_RPT_TYPE_SINGLE_SSR	= 0x2,
    SPX_ASTERIX48_TGT_RPT_TYPE_SSR_PSR		= 0x3,
    SPX_ASTERIX48_TGT_RPT_TYPE_SINGLE_MODES_ALL	= 0x4,
    SPX_ASTERIX48_TGT_RPT_TYPE_SINGLE_MODES_ROLL = 0x5,
    SPX_ASTERIX48_TGT_RPT_TYPE_MODES_ALL_PSR	= 0x6,
    SPX_ASTERIX48_TGT_RPT_TYPE_MODES_ROLL_PSR	= 0x7
};

/* Debug flags. */
enum SPxAsterixDecoderDebug
{
    SPX_ASTERIX_DEC_DEBUG_NONE	    = 0x00000000,
    SPX_ASTERIX_DEC_DEBUG_SET_PROP  = 0x00000001,
    SPX_ASTERIX_DEC_DEBUG_GET_PROP  = 0x00000002,
    SPX_ASTERIX_DEC_DEBUG_ALL	    = 0x7FFFFFFF
};

/*
 * CAT4 bit flags.
 */
/* I004/060 octet 1 bit flags */
#define	SPX_ASTERIX4_060_1_MRVA		0x80
#define	SPX_ASTERIX4_060_1_RAMLD	0x40
#define	SPX_ASTERIX4_060_1_RAMDD	0x20
#define	SPX_ASTERIX4_060_1_MSAW		0x10
#define	SPX_ASTERIX4_060_1_APW		0x08
#define	SPX_ASTERIX4_060_1_CLAM		0x04
#define	SPX_ASTERIX4_060_1_STCA		0x02
#define	SPX_ASTERIX4_060_1_FX		0x01	/* Field extension */

/* I004/060 octet 2 bit flags */
#define	SPX_ASTERIX4_060_2_AFDA		0x80
#define	SPX_ASTERIX4_060_2_RIMCA	0x40
#define	SPX_ASTERIX4_060_2_ACASRA	0x20
#define	SPX_ASTERIX4_060_2_NTCA		0x10
#define	SPX_ASTERIX4_060_2_DG		0x08	/* System degraded */
#define	SPX_ASTERIX4_060_2_OF		0x04	/* Overflow */
#define	SPX_ASTERIX4_060_2_OL		0x02	/* Overload */
#define	SPX_ASTERIX4_060_2_FX		0x01	/* Field extension */

/* I004/120 primary subfield bit flags. */
#define	SPX_ASTERIX4_120_CN		0x80	/* Conflict Nature */
#define	SPX_ASTERIX4_120_CC		0x40	/* Conflict Classification */
#define	SPX_ASTERIX4_120_CP		0x20	/* Conflict Probability */
#define	SPX_ASTERIX4_120_CD		0x10	/* Conflict Duration */
#define	SPX_ASTERIX4_120_FX		0x01	/* Field Extension */

/* I004/120 subfield 1 and first extent bit flags. */
#define	SPX_ASTERIX4_120_1_MAS		0x80	/* Military airspace */
#define	SPX_ASTERIX4_120_1_CAS		0x40	/* Civilian airspace */
#define	SPX_ASTERIX4_120_1_FLD		0x20	/* Fast lateral divergence */
#define	SPX_ASTERIX4_120_1_FVD		0x10	/* Fast vertical divergence */
#define	SPX_ASTERIX4_120_1_TYPE		0x08	/* Major Infringement */
#define	SPX_ASTERIX4_120_1_CROSS	0x04	/* Crossed */
#define	SPX_ASTERIX4_120_1_DIV		0x02	/* Diverging */
#define	SPX_ASTERIX4_120_1_FX		0x01	/* Field Extension */
#define	SPX_ASTERIX4_120_1X_RRC		0x80	/* Runway/Runway Crossing */
#define	SPX_ASTERIX4_120_1X_RTC		0x40	/* Runway/Taxiway Crossing */
#define	SPX_ASTERIX4_120_1X_MRVA	0x20	/* MSAW is MRVA */
#define	SPX_ASTERIX4_120_1X_FX		0x01	/* Field Extension */


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

/* Need to forward-declare the class for use in the handler function type. */
class SPxAsterixDecoder;
class SPxAsterixMsg;
class SPxAsterixVersion;
class SPxDecodeBuf;
class SPxPacketDecoderNet;
struct SPxPacketTrackExtended_tag;
struct SPxPacketTrackerStatus_tag;
struct SPxPacketStrobe_tag;

/* Define the type of the callback packet handler functions. */
typedef void (*SPxAsterixDecoderFn_t)(const SPxAsterixDecoder *, /* this */
				      void *,			/* userArg */
				      UINT8,			/* category */
				      SPxAsterixMsg *);		/* message */

/* Similar for the SPx-report interface. */
typedef void (*SPxAsterixDecoderSPxTrackFn_t)(
				const SPxAsterixDecoder *,	/* this */
				void *,				/* userArg */
				struct SPxPacketTrackExtended_tag *,
				const SPxAsterixMsg *);
typedef void (*SPxAsterixDecoderSPxStatusFn_t)(
				const SPxAsterixDecoder *,	/* this */
				void *,				/* userArg */
				struct SPxPacketTrackerStatus_tag *,
				const SPxAsterixMsg *);
typedef void (*SPxAsterixDecoderSPxStrobeFn_t)(
				const SPxAsterixDecoder *,	/* this */
				void *,				/* userArg */
				struct SPxPacketStrobe_tag *,
				const SPxAsterixMsg *);

enum SPxAsterixItemType
{
    SPX_ASTERIX_ITEM_DATA = 0,
    SPX_ASTERIX_ITEM_REF = 1,
    SPX_ASTERIX_ITEM_SPF = 2
};

/*
 * Define our class.
 */
class SPxAsterixDecoder : public SPxDecoderRepeater
{
public:
    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    SPxAsterixDecoder(int netReceipt=TRUE);
    virtual ~SPxAsterixDecoder(void);

    /* The address/port and other control functions from the base class are
     * also available.
     */

    /* SAC/SIC filtering. */
    void SetExpectedSAC(UINT8 sac)	{ m_expectedSAC = sac; }
    void SetExpectedSIC(UINT8 sic)	{ m_expectedSIC = sic; }
    UINT8 GetExpectedSAC(void) const	{ return(m_expectedSAC); }
    UINT8 GetExpectedSIC(void) const	{ return(m_expectedSIC); }

    /* Category filtering. */
    SPxErrorCode SetCategoryEnable(unsigned int cat, int enable);
    int GetCategoryEnable(unsigned int cat);

    /* Version handling. */
    SPxErrorCode SetVersion(unsigned char cat,
				const SPxAsterixVersion *version);
    SPxErrorCode SetVersion(unsigned char cat, UINT8 major, UINT8 minor) const;
    SPxErrorCode SetVersion(unsigned char cat, const char *versString);
    SPxErrorCode GetVersion(unsigned char cat,
				SPxAsterixVersion *versionPtr) const;

    void SetProjectCode(unsigned int projectCode) { m_projectCode = projectCode; }
    unsigned int GetProjectCode(void) const { return m_projectCode; }
    void SetUAPCode(unsigned int uapCode) { m_uapCode = uapCode; }
    unsigned int GetUAPCode(void) const { return m_uapCode; }

    /* Prefix removal. */
    SPxErrorCode SetIgnorePrefixBytes(unsigned int numBytes);
    unsigned int GetIgnorePrefixBytes(void) const {return(m_ignorePrefixBytes);}

    /* Permissive mode. */
    SPxErrorCode SetPermissiveMode(int enable);
    int GetPermissiveMode(void) const {return(m_permissiveMode);}

    /* Status retrieval. */
    UINT32 GetMsecsSinceLastMsg(void);

    /* Handler functions. */
    SPxErrorCode InstallHandler(UINT8 category,
				SPxAsterixDecoderFn_t fn,
				void *userArg);
    SPxErrorCode RemoveHandler(UINT8 category);

    /* SPx track/status report handlers. */
    SPxErrorCode InstallSPxHandlers(
			    SPxAsterixDecoderSPxTrackFn_t trackFn,
			    SPxAsterixDecoderSPxStatusFn_t statusFn,
			    void *userArg,
			    SPxAsterixDecoderSPxStrobeFn_t strobeFn = NULL);

    /* Decode function. */
    SPxErrorCode Decode(const unsigned char *data, unsigned int numBytes) const;

    /* Debugging. */
    void SetDebug(UINT32 debugFlags);
    UINT32 GetDebug(void) const;

    /* Conversion functions. */
    static SPxErrorCode BuildSPxTrack(const SPxAsterixMsg *msg,
				SPxPacketTrackExtended_tag *track,
				double radarTimeOffsetSecs=0.0,
				int sensorLatLongValid=FALSE,
				double sensorLatDegs=0.0,
				double sensorLongDegs=0.0);
    static SPxErrorCode BuildSPxStatus(const SPxAsterixMsg *msg,
				SPxPacketTrackerStatus_tag *status);
    static SPxErrorCode BuildSPxStrobe(const SPxAsterixMsg *msg,
				SPxPacketStrobe_tag *strobe);

    /* Static functions. */
    static SPxErrorCode GetTimeFrom128thSecsSinceMidnight(
	SPxTime_tag *timestamp, UINT32 oneTwoEighthSecs);
    static UINT16 UnmangleModeC(UINT16 mangledBits);

    /* Generic parameter assignment. */
    virtual int SetParameter(char *name, char *value);
    virtual int GetParameter(char *name, char *valueBuf, int bufLen);

protected:
    /* Constructor for testing. */
    explicit SPxAsterixDecoder(int netReceipt,
			       SPxPacketDecoderNet *netPacketDecoder);

private:
    /*
     * Private variables.
     */
    /* Internal information. */
    struct impl;
    SPxAutoPtr<impl> m_p;

    /* Filters. */
    UINT8 m_expectedSAC;	/* Expected SAC if non-zero */
    UINT8 m_expectedSIC;	/* Expected SIC if non-zero */
    int m_cat1;			/* Is this category enabled? */
    int m_cat2;			/* Is this category enabled? */
    int m_cat4;			/* Is this category enabled? */
    int m_cat8;			/* Is this category enabled? */
    int m_cat10;		/* Is this category enabled? */
    int m_cat17;		/* Is this category enabled? */
    int m_cat19;		/* Is this category enabled? */
    int m_cat20;		/* Is this category enabled? */
    int m_cat21;		/* Is this category enabled? */
    int m_cat34;		/* Is this category enabled? */
    int m_cat48;		/* Is this category enabled? */
    int m_cat62;		/* Is this category enabled? */
    int m_cat159;		/* Is this category enabled? */
    int m_cat247;		/* Is this category enabled? */
    unsigned int m_projectCode;	/* Project code. */
    unsigned int m_uapCode;	/* UAP code. */
    mutable double m_radarTimeOffsetSecs;   /* Offset between "radar time" and "system time". */

    /* Prefix bytes to ignore. */
    unsigned int m_ignorePrefixBytes;	/* Number of bytes */

    /* Run in permissive mode (try to ignore error conditions). */
    int m_permissiveMode;

    /* Callback info. */
    SPxAsterixDecoderSPxTrackFn_t m_fnSPxTracks;
    SPxAsterixDecoderSPxStatusFn_t m_fnSPxStatus;
    SPxAsterixDecoderSPxStrobeFn_t m_fnSPxStrobe;
    void *m_fnSPxUserArg;

    /* Status. */
    mutable UINT32 m_lastMsgMsecs; /* Timestamp for last message accepted */

    /*
     * Private functions.
     */
    static void staticDecodePacket(SPxPacketDecoder *packetDecoder,
					void *userArg,
					UINT32 packetType,
					struct sockaddr_in *from,
					SPxTime_t *timestamp,
					const unsigned char *payload,
					unsigned int numBytes);
    SPxErrorCode decodePacket(UINT32 packetType,
					struct sockaddr_in *from,
					SPxTime_t *timestamp,
					const unsigned char *payload,
					unsigned int numBytes);

    /* Category handlers. */
    SPxErrorCode processRecord(SPxDecodeBuf& buf, UINT8 cat) const;
    SPxErrorCode processCat1(SPxDecodeBuf& buf, 
				const unsigned char *fspecs,
				unsigned int numFspecs, 
				SPxAsterixMsg *msg) const;
    SPxErrorCode processCat2(SPxDecodeBuf& buf, 
				const unsigned char *fspecs,
				unsigned int numFspecs, 
				SPxAsterixMsg *msg) const;
    SPxErrorCode processCat4(SPxDecodeBuf& buf, 
				const unsigned char *fspecs,
				unsigned int numFspecs, 
				SPxAsterixMsg *msg) const;
    SPxErrorCode processCat8(SPxDecodeBuf& buf, 
				const unsigned char *fspecs,
				unsigned int numFspecs, 
				SPxAsterixMsg *msg) const;
    SPxErrorCode processCat10(SPxDecodeBuf& buf, 
				const unsigned char *fspecs,
				unsigned int numFspecs, 
				SPxAsterixMsg *msg) const;
    SPxErrorCode processCat17(SPxDecodeBuf& buf, 
				const unsigned char *fspecs,
				unsigned int numFspecs, 
				SPxAsterixMsg *msg) const;
    SPxErrorCode processCat19(SPxDecodeBuf& buf, 
				const unsigned char *fspecs,
				unsigned int numFspecs, 
				SPxAsterixMsg *msg) const;
    SPxErrorCode processCat20(SPxDecodeBuf& buf, 
				const unsigned char *fspecs,
				unsigned int numFspecs, 
				SPxAsterixMsg *msg) const;
    SPxErrorCode processCat21(SPxDecodeBuf& buf, 
				const unsigned char *fspecs,
				unsigned int numFspecs, 
				SPxAsterixMsg *msg) const;
    SPxErrorCode processCat21Ref(SPxDecodeBuf& buf,
				SPxAsterixMsg *msg) const;
    SPxErrorCode processCat34(SPxDecodeBuf& buf, 
				const unsigned char *fspecs,
				unsigned int numFspecs, 
				SPxAsterixMsg *msg) const;
    SPxErrorCode processCat48(SPxDecodeBuf& buf, 
				const unsigned char *fspecs,
				unsigned int numFspecs, 
				SPxAsterixMsg *msg) const;
    SPxErrorCode processCat62(SPxDecodeBuf& buf, 
				const unsigned char *fspecs,
				unsigned int numFspecs, 
				SPxAsterixMsg *msg) const;
    SPxErrorCode processCat62Ref(SPxDecodeBuf& buf,
				SPxAsterixMsg *msg) const;
    SPxErrorCode processCat159(SPxDecodeBuf& buf, 
				const unsigned char *fspecs,
				unsigned int numFspecs, 
				SPxAsterixMsg *msg) const;
    SPxErrorCode processCat247(SPxDecodeBuf& buf, 
				const unsigned char *fspecs,
				unsigned int numFspecs, 
				SPxAsterixMsg *msg) const;
    SPxErrorCode skipSpfOrRef(SPxDecodeBuf& buf,
			      SPxAsterixMsg *msg=NULL,
			      SPxAsterixProperty prop=SPX_ASTERIX_PROP_NONE) const;

}; /* SPxAsterixDecoder */

/* Class describing Asterix messages. */
class SPxAsterixMsg
{
public:
    explicit SPxAsterixMsg(unsigned char cat=0, UINT32 debugFlags=0,
			   SPxAsterixVersion *versionReceived=NULL,
			   unsigned int projectCode=0);
    ~SPxAsterixMsg(void);
    SPxAsterixMsg(const SPxAsterixMsg &msg);
    SPxAsterixMsg& operator=(const SPxAsterixMsg &msg);

    SPxErrorCode DecodeSpf(void);

    unsigned char GetCategory(void) const;
    void GetVersion(SPxAsterixVersion *version) const;
    unsigned int GetProjectCode(void) const;
    void SetUAPCode(unsigned int uapCode);
    unsigned int GetUAPCode(void) const;
    SPxErrorCode SetProperty(SPxAsterixProperty prop, UINT8 value);
    SPxErrorCode SetProperty(SPxAsterixProperty prop, INT8 value);
    SPxErrorCode SetProperty(SPxAsterixProperty prop, UINT16 value);
    SPxErrorCode SetProperty(SPxAsterixProperty prop, INT16 value);
    SPxErrorCode SetProperty(SPxAsterixProperty prop, UINT32 value);
    SPxErrorCode SetProperty(SPxAsterixProperty prop, const char *value);
    SPxErrorCode GetProperty(SPxAsterixProperty prop, UINT8 *value) const;
    SPxErrorCode GetProperty(SPxAsterixProperty prop, INT8 *value) const;
    SPxErrorCode GetProperty(SPxAsterixProperty prop, UINT16 *value) const;
    SPxErrorCode GetProperty(SPxAsterixProperty prop, INT16 *value) const;
    SPxErrorCode GetProperty(SPxAsterixProperty prop, UINT32 *value) const;
    SPxErrorCode GetProperty(SPxAsterixProperty prop, INT32 *value) const;
    SPxErrorCode GetProperty(SPxAsterixProperty prop, char *value, unsigned int valueLen) const;

    SPxErrorCode SetPropertyData(SPxAsterixProperty prop, const UINT8 *buffer, unsigned int bufLen);
    SPxErrorCode GetPropertyData(SPxAsterixProperty prop, UINT8 *buffer, unsigned int bufLen,
				 unsigned int offset=0) const;
    unsigned int GetPropertyDataSize(SPxAsterixProperty prop) const;

    int IsNogo(void) const;

    SPxErrorCode FormatDataItem(unsigned char category,
				unsigned int dataItemID,
				SPxAsterixItemType itemType,
				SPxUnitsDist_t unitsDist,
				SPxUnitsSpeed_t unitsSpeed,
				SPxUnitsHeight_t unitsHeight,
				SPxFormatLatLong_t formatLatLong,
				int precision,
				char *buf, unsigned int bufLen) const;

private:
    struct impl;
    SPxAutoPtr<impl> m_p;

    SPxErrorCode formatCat17DataItem(unsigned int dataItemID,
				     SPxAsterixItemType itemType,
				     SPxUnitsDist_t unitsDist,
				     SPxUnitsSpeed_t unitsSpeed,
				     SPxUnitsHeight_t unitsHeight,
				     SPxFormatLatLong_t formatLatLong,
				     int precision,
				     char *buf, unsigned int bufLen) const;

    SPxErrorCode formatCat21DataItem(unsigned int dataItemID,
				     SPxAsterixItemType itemType,
				     SPxUnitsDist_t unitsDist,
				     SPxUnitsSpeed_t unitsSpeed,
				     SPxUnitsHeight_t unitsHeight,
				     SPxFormatLatLong_t formatLatLong,
				     int precision,
				     char *buf, unsigned int bufLen) const;

    SPxErrorCode formatCat34DataItem(unsigned int dataItemID,
				     SPxAsterixItemType itemType,
				     SPxUnitsDist_t unitsDist,
				     SPxUnitsSpeed_t unitsSpeed,
				     SPxUnitsHeight_t unitsHeight,
				     SPxFormatLatLong_t formatLatLong,
				     int precision,
				     char *buf, unsigned int bufLen) const;

    SPxErrorCode formatCat48DataItem(unsigned int dataItemID,
				     SPxAsterixItemType itemType,
				     SPxUnitsDist_t unitsDist,
				     SPxUnitsSpeed_t unitsSpeed,
				     SPxUnitsHeight_t unitsHeight,
				     SPxFormatLatLong_t formatLatLong,
				     int precision,
				     char *buf, unsigned int bufLen) const;

    SPxErrorCode formatCat62DataItem(unsigned int dataItemID,
				     SPxAsterixItemType itemType,
				     SPxUnitsDist_t unitsDist,
				     SPxUnitsSpeed_t unitsSpeed,
				     SPxUnitsHeight_t unitsHeight,
				     SPxFormatLatLong_t formatLatLong,
				     int precision,
				     char *buf, unsigned int bufLen) const;

    static void formatTime(UINT32 time32, int precision,
			   char *buf, unsigned int bufLen);
    static void formatHighPrecTime(UINT32 time32, UINT32 timeHighPrec32,
				   int precision,
				   char *buf, unsigned int bufLen);



}; /* SPxAsterixMsg */

#endif /* _SPX_ASTERIX_DECODER_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/

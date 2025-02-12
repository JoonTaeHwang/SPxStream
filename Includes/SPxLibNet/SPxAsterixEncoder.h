/*********************************************************************
*
* (c) Copyright 2013 - 2022, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxAsterixEncoder module.
*
*	This module can be used to encode an Asterix message stream.
*
*
* Revision Control:
*   11/08/22 v1.22   AGC	Support copying fields from another ASTERIX message in BuildCat48().
*
* Previous Changes:
*   10/06/22 1.21   BTB	Add BuildCat159().
*   24/05/22 1.20   AGC	Support building CAT62 REF relative velocity.
*   11/04/22 1.19   BTB	Support leaving track counts out of CAT-34 messages.
*   14/05/21 1.18   BTB	Add MangleModeC().
*   08/03/21 1.17   AGC	Improve CAT8 message type names.
*   02/03/21 1.16   AGC	Support building CAT8 messages.
*   29/10/20 1.15   AGC	P466 identity now set as part of SPF.
*   26/08/20 1.14   AGC	Initial support for P466 specific CAT-48 SPF.
*   17/01/20 1.13   REW	Add alternative SetVersion() function.
*   16/01/20 1.12   REW	Support generating CAT21 V0.23 up to V2.4
*			and add BuildCat247().
*   26/09/19 1.11   AGC	Support specifying Mode 5 options for CAT-48.
*   05/04/18 1.10   AGC	Add BuildCat62().
*   02/02/18 1.9    AGC	Support CAT-34 reduction states and warnings.
*   11/12/17 1.8    AGC	Support extra CAT-48 info.
*   15/09/14 1.7    REW	Add BuildCat4().
*   26/06/14 1.6    REW	Break out BuildCat10Status().
*   21/01/14 1.5    AGC	Support user FSPECS for CAT10/48.
*			Improved information in CAT34 messages.
*   17/05/13 1.4    REW	Add BuildCat19() and BuildCat20().
*   03/05/13 1.3    AGC	Add BuildCat21().
*   30/04/13 1.2    REW	Add BuildCat48() and BuildCat34().
*   25/04/13 1.1    REW	Initial Version.
**********************************************************************/

#ifndef _SPX_ASTERIX_ENCODER_H
#define _SPX_ASTERIX_ENCODER_H

/*
 * Other headers required.
 */
#include "SPxLibUtils/SPxObj.h"
#include "SPxLibUtils/SPxCommon.h"
#include "SPxLibUtils/SPxError.h"

/*********************************************************************
*
*   Constants
*
**********************************************************************/

/* CAT-34 warnings. */
#define SPX_ASTERIX34_WARN_NONE		0x00000000
#define SPX_ASTERIX34_WARN_COM_RDPR	0x00000001
#define SPX_ASTERIX34_WARN_COM_OVL_RDP	0x00000002
#define SPX_ASTERIX34_WARN_COM_OVL_XMT	0x00000004
#define SPX_ASTERIX34_WARN_COM_MSC	0x00000008
#define SPX_ASTERIX34_WARN_COM_TSV	0x00000010
#define SPX_ASTERIX34_WARN_COM_ALL	0x0000001F
#define SPX_ASTERIX34_WARN_PSR_OVL	0x00000020
#define SPX_ASTERIX34_WARN_PSR_MSC	0x00000040
#define SPX_ASTERIX34_WARN_PSR_ALL	0x00000060
#define SPX_ASTERIX34_WARN_SSR_OVL	0x00000080
#define SPX_ASTERIX34_WARN_SSR_MSC	0x00000100
#define SPX_ASTERIX34_WARN_SSR_ALL	0x00000180
#define SPX_ASTERIX34_WARN_MDS_OVL_SUR	0x00000200
#define SPX_ASTERIX34_WARN_MDS_MSC	0x00000400
#define SPX_ASTERIX34_WARN_MDS_OVL_SCF	0x00000800
#define SPX_ASTERIX34_WARN_MDS_OVL_DLF	0x00001000
#define SPX_ASTERIX34_WARN_MDS_ALL	0x00001E00

/* CAT-8 message types. */
#define SPX_ASTERIX8_MSG_POLAR_VECTOR	    1
#define SPX_ASTERIX8_MSG_CART_VECTOR	    2
#define SPX_ASTERIX8_MSG_CONTOUR	    3
#define SPX_ASTERIX8_MSG_WEATHER_VECTOR	    4
#define SPX_ASTERIX8_MSG_SOP		    254
#define SPX_ASTERIX8_MSG_EOP		    255

/*********************************************************************
*
*   Type definitions
*
**********************************************************************/

/* Forward declarations. */
struct SPxTime_tag;
struct SPxPacketTrackMinimal_tag;
struct SPxPacketTrackNormal_tag;
struct SPxPacketTrackExtended_tag;
struct SPxLatLong_tag;
class SPxADSBTrack;
class SPxWeatherPicture;
class SPxAsterixVersion;
class SPxAsterixMsg;

/* SSR mode. */
enum SPxAsterixSsrMode
{
    SPX_ASTERIX_SSR_MODE_NORMAL = 0,
    SPX_ASTERIX_SSR_MODE_ALL_CALL = 1,
    SPX_ASTERIX_SSR_MODE_ROLL_CALL = 2
};

/* Structure for passing additional ASTERIX info. */
struct SPxAsterixExtraInfo
{
    SPxAsterixSsrMode ssrMode;
    UINT8 spfLen;
    unsigned char spfData[255];
    float posXMetresSD;
    float posYMetresSD;
    float speedMpsSD;
    float courseDegsSD;
    unsigned char acasResAdv[7];
    UINT16 commsCapability;
    int plotAmplitudeSet;
    INT8 plotAmplitude;
    UINT16 mode5Flags;
    UINT8 mode5Summary;
    UINT16 mode5PIN;
    UINT16 mode5NationalOrigin;
    int mode5Position;
    int mode5Altitude;
    double mode5TimeOffsetSecs;
    UINT8 mode5XPulse;
    UINT8 mode5FigureOfMerit;
};

enum SPxAsterixWeatherType
{
    SPX_ASTERIX_WEATHER_TYPE_VEC_POLAR = 0,
    SPX_ASTERIX_WEATHER_TYPE_VEC_CART_LENGTH = 1,
    SPX_ASTERIX_WEATHER_TYPE_VEC_CART_END = 2,
    SPX_ASTERIX_WEATHER_TYPE_CONTOUR = 3
};

/*
 * Define our class.
 */
class SPxAsterixEncoder : public SPxObj
{
public:
    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    SPxAsterixEncoder(void);
    virtual ~SPxAsterixEncoder(void);

    /*
     * Fields that use needed for various messages.
     */
    /* SAC/SIC control. */
    void SetSAC(UINT8 sac)	{ m_sac = sac; }
    UINT8 GetSAC(void)		{ return(m_sac); }
    void SetSIC(UINT8 sic)	{ m_sic = sic; }
    UINT8 GetSIC(void)		{ return(m_sic); }

    /* Period. */
    void SetPeriod(double periodSecs)	{ m_periodSecs = periodSecs; }
    double GetPeriod(void)		{ return(m_periodSecs); }

    /* Sensor position. */
    void SetLatLong(double latDegs, double longDegs)
    {
	m_latDegs = latDegs;
	m_longDegs = longDegs;
    }
    void SetHeight(double heightMetres)	{ m_heightMetres = heightMetres; }
    double GetLat(void)			{ return(m_latDegs); }
    double GetLong(void)		{ return(m_longDegs); }
    double GetHeight(void)		{ return(m_heightMetres); }

    /*
     * Message creation.
     */
    SPxErrorCode SetVersion(unsigned char cat,
				const SPxAsterixVersion *version);
    SPxErrorCode SetVersion(unsigned char cat, UINT8 major, UINT8 minor);
    SPxErrorCode SetVersion(unsigned char cat, const char *versString);
    unsigned int BuildCat4(unsigned char *buf, unsigned int bufSizeBytes,
				UINT64 fspec,
				const struct SPxTime_tag *timestamp,
				const struct SPxSafetyNetAlert_tag *alert)
				const;
    unsigned int BuildCat8Sop(unsigned char *buf, unsigned int bufSizeBytes,
			      UINT64 fspec,
			      const struct SPxTime_tag *timestamp,
			      INT8 scalingFactor) const;
    unsigned int BuildCat8Eop(unsigned char *buf, unsigned int bufSizeBytes,
			      UINT64 fspec,
			      const struct SPxTime_tag *timestamp,
			      unsigned int totalNumItems) const;
    unsigned int BuildCat8(unsigned char *buf, unsigned int bufSizeBytes,
			   UINT64 fspec,
			   INT8 scalingFactor,
			   const SPxWeatherPicture *weatherPic,
			   unsigned int contourID,
			   UINT8 messageType,
			   double vectorSpacingDegs,
			   double vectorSpacingMetres,
			   unsigned int *numItems=NULL)
			   const;
    unsigned int BuildCat10(unsigned char *buf, unsigned int bufSizeBytes,
				UINT64 fspec,
				const struct SPxTime_tag *timestamp,
				const struct SPxPacketTrackMinimal_tag *min,
				const struct SPxPacketTrackNormal_tag *norm,
				const struct SPxPacketTrackExtended_tag *ext)
				const;
    unsigned int BuildCat10Status(unsigned char *buf, unsigned int bufSizeBytes,
				UINT64 fspec,
				const struct SPxTime_tag *timestamp,
				int msgType,	/* SPxAsterix10MsgType */
				int overload)	/* TRUE or FALSE */
				const;
    unsigned int BuildCat19(unsigned char *buf, unsigned int bufSizeBytes,
				const struct SPxTime_tag *timestamp,
				int msgType,	/* SPxAsterix19MsgType */
				int overload)	/* TRUE or FALSE */
				const;
    unsigned int BuildCat20(unsigned char *buf, unsigned int bufSizeBytes,
				const struct SPxTime_tag *timestamp,
				const struct SPxPacketTrackMinimal_tag *min,
				const struct SPxPacketTrackNormal_tag *norm,
				const struct SPxPacketTrackExtended_tag *ext,
				int polarValid, int cartesianValid,
				int velocityValid)
				const;
    unsigned int BuildCat21(unsigned char *buf, unsigned int bufSizeBytes,
				const struct SPxTime_tag *timestamp,
				const SPxADSBTrack *track) const;
    unsigned int BuildCat34(unsigned char *buf, unsigned int bufSizeBytes,
				const struct SPxTime_tag *timestamp,
				int msgType,	/* SPxAsterix34MsgType */
				int psr, int ssr, int mds,
				UINT32 warnings, /* See SPX_ASTERIX34_WARN_... */
				unsigned int comRdpLoadReductionState,
				unsigned int comXmtLoadReductionState,
				unsigned int psrLoadReductionState,
				unsigned int ssrLoadReductionState,
				unsigned int mdsLoadReductionState,
				int numPSR,
				int numSSR,
				int numPSSR,
				int numMisses,
				double sectorDegrees)
				const;
    unsigned int BuildCat48(unsigned char *buf, unsigned int bufSizeBytes,
				UINT64 fspec,
				const struct SPxTime_tag *timestamp,
				const struct SPxPacketTrackMinimal_tag *min,
				const struct SPxPacketTrackNormal_tag *norm,
				const struct SPxPacketTrackExtended_tag *ext,
				int velocityValid=TRUE,
				int typOverride=-1,
				const SPxAsterixExtraInfo *extraInfo=NULL,
				const SPxAsterixMsg *asterixMsg=NULL,
				UINT64 asterixMsgFspec=0)
				const;
    unsigned int BuildCat62(unsigned char *buf, unsigned int bufSizeBytes,
				UINT64 fspec,
				const struct SPxTime_tag *timestamp,
				const struct SPxPacketTrackMinimal_tag *min,
				const struct SPxPacketTrackNormal_tag *norm,
				const struct SPxPacketTrackExtended_tag *ext,
				double sensorSpeedMps=-1.0,
				double sensorCourseDegs=-1.0)
				const;
    unsigned int BuildCat159(unsigned char *buf, unsigned int bufSizeBytes,
				UINT64 fspec,
				const struct SPxTime_tag *timestamp,
				const struct SPxPacketTrackMinimal_tag *min,
				const struct SPxPacketTrackNormal_tag *norm,
				const struct SPxPacketTrackExtended_tag *ext,
				int typOverride = -1,
				const SPxAsterixExtraInfo *extraInfo=NULL)
				const;
    unsigned int BuildCat247(unsigned char *buf, unsigned int bufSizeBytes,
				const struct SPxTime_tag *timestamp=NULL)
				const;

    /* Static functions. */
    static UINT32 Get128thSecsSinceMidnight(
				const struct SPxTime_tag *timestamp);
    static UINT16 MangleModeC(UINT16 unmangledBits);

private:
    /*
     * Private variables.
     */
    struct impl;
    SPxAutoPtr<impl> m_p;

    /* SAC/SIC codes to use when building messages. */
    UINT8 m_sac;
    UINT8 m_sic;

    /* Period to use when building messages. */
    double m_periodSecs;

    /* Sensor position. */
    double m_latDegs;
    double m_longDegs;
    double m_heightMetres;

    /*
     * Private functions.
     */
    SPxErrorCode getVersion(UINT8 cat, SPxAsterixVersion *version) const;
    unsigned int buildCat8Start(unsigned char *buf, unsigned int bufSizeBytes,
				UINT64 fspec, UINT8 messageType,
				unsigned int intensity,
				unsigned int contourID,
				unsigned int firstLast,
				unsigned int shadingAngle) const;

}; /* SPxAsterixEncoder */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/


#endif /* _SPX_ASTERIX_ENCODER_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/

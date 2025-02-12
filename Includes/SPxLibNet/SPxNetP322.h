/*********************************************************************
*
* (c) Copyright 2017, 2018, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for common structures used by the project-322
*	network send, receive and control modules.
*
*
* Revision Control:
*   28/11/18 v1.3   AJH	Correct radar/display name sizes.
*   16/07/18 1.2    AGC	Rename and make public.
*			Add more structures.
*
* Previous Changes:
*   23/10/17 1.1    AGC	Initial Version.
**********************************************************************/

#ifndef SPX_NET_P322_H_
#define SPX_NET_P322_H_

/* For UINT32 etc. */
#include "SPxLibUtils/SPxTypes.h"

/*********************************************************************
*
*   Constants
*
**********************************************************************/

/* Default values. */
#define	SPX_NET_P322_DEFAULT_DISCO_ADDR		"239.255.123.1"
#define	SPX_NET_P322_DEFAULT_DISCO_PORT		12345

/* Protocol definitions from the interface specification.
 * (Plus see some default values in the header file).
 */
#define	SPX_NET_P322_PROTO_VERSION		1
#define SPX_NET_P322_HDR_LEN			4
#define	SPX_NET_P322_MAX_RADAR_NAME_LEN		255	/* Includes NULL */
#define SPX_NET_P322_MAX_DISPLAY_NAME_LEN	255	/* Includes NULL */

/* Message IDs */
#define	SPX_NET_P322_MSG_ID_MASTER_HELLO	0x01
#define	SPX_NET_P322_MSG_ID_MASTER_HB		0x02
#define	SPX_NET_P322_MSG_ID_TIMING_CTRL		0x04
#define	SPX_NET_P322_MSG_ID_TRANSMIT_CTRL	0x05
#define	SPX_NET_P322_MSG_ID_IP_SETUP		0x06
#define	SPX_NET_P322_MSG_ID_SETUP		0x07
#define	SPX_NET_P322_MSG_ID_MAINTENANCE		0x08
#define	SPX_NET_P322_MSG_ID_COURSE		0x09
#define	SPX_NET_P322_MSG_ID_FE_STC		0x11
#define	SPX_NET_P322_MSG_ID_IF_STC		0x12
#define	SPX_NET_P322_MSG_ID_STC_CORRECTION	0x13
#define	SPX_NET_P322_MSG_ID_SAVE		0x80
#define	SPX_NET_P322_MSG_ID_STATUS		0x81

/*********************************************************************
*
*   Type definitions
*
**********************************************************************/

/* Discovery message.
 *
 * Note that the last field is defined in the spec as variable length.  It's
 * not clear if that means the size of the packet on the network might be
 * variable because of it, but assume it does.  Be careful with sizeof().
 */
#define	SPX_NET_P322_MSG_DISCOVERY_SIZE_MINUS_NAME	(14*4)	/* Bytes */
struct SPxNetP322MsgDiscovery
{
    UINT32 protocolVersion;		/* Probably 1 */

    UINT32 ipAddress1;			/* IP address of radar interface 1 */
    UINT32 netmask1;			/* IP subnet of i/f 1 */
    UINT32 gatewayAddress1;		/* IP gateway of i/f 1 */
    UINT32 statusAddress1;		/* UDP addr for status from i/f 1 */
    UINT32 videoAddress1;		/* UDP addr for video from i/f 1 */

    UINT32 ipAddress2;			/* IP address of radar interface 2 */
    UINT32 netmask2;			/* IP subnet of i/f 2 */
    UINT32 gatewayAddress2;		/* IP gateway of i/f 2 */
    UINT32 statusAddress2;		/* UDP addr for status from i/f 2 */
    UINT32 videoAddress2;		/* UDP addr for video from i/f 2 */

    UINT16 ctrlPort;			/* Port for control messages */
    UINT16 reserved1;
    UINT16 statusPort;			/* Port for status messages */
    UINT16 videoPort;			/* Port for video messages */

    UINT32 serialNumber;		/* Radar serial number */

    char radarName[SPX_NET_P322_MAX_RADAR_NAME_LEN]; /* Variable len */
};

/* Video message header (excluding the video data itself). */
struct SPxNetP322MsgVideoHdr
{
    UINT16 reserved1;
    UINT16 azimuthNumber;		/* Azimuth number */
    UINT32 rangeBinSize;		/* In 10ths of millimetres */
    UINT32 config;			/* Configuration info */
    UINT16 numAzimuths;			/* Azis per scan, e.g. 512, 1/2/4k */
    UINT16 numSamples;			/* Number of samples in spoke */
					/* Tightly packed radar samples follow */
};

/* Status message.
 *
 * Note that the last field is defined in the spec as variable length.  It's
 * not clear if that means the size of the packet on the network might be
 * variable because of it, but assume it does.  Be careful with sizeof().
 */
#define	SPX_NET_P322_MSG_STATUS_SIZE_MINUS_NAME	(290)	/* Bytes */
struct SPxNetP322MsgStatus
{
    UINT8 messageID;
    UINT8 payloadLenH;
    UINT16 payloadLenL;
    UINT8 tuningMode;
    UINT8 hwVersion;
    UINT8 txStatus;
    UINT8 txMode;
    UINT16 prfMode[6];
    UINT16 pulseDurationMode[6];
    UINT8 secBlankMode;
    UINT8 triggerDelay;
    UINT8 interferRejection;
    UINT8 ifBandwidth;
    UINT8 sartMode;
    UINT8 stcMode;
    UINT8 gearBox;
    UINT8 antennaSafetySwitch;
    UINT8 prfJitter;
    UINT8 radarType;
    UINT8 autoBiasGain;
    UINT8 magnetronCurrent;
    UINT8 reserved1;
    UINT8 antennaRpm;
    UINT8 waveguideLength;
    UINT8 sleepMode;
    UINT8 mute;
    UINT8 inhibitTx;
    UINT8 noiseSrcMode;
    UINT8 rfMonitorMode;
    UINT16 numAzimuths;
    UINT16 refIfNoiseLevelMode[6];
    UINT8 refRfLevelMode[6];
    UINT16 magnetronTxTime;
    UINT16 powerOnTime;
    UINT16 magnetronWarmUpTime;
    UINT16 zeroRangeValue;
    UINT16 logAmpBiasValueMode[6];
    UINT16 logAmpGainValueMode[6];
    UINT16 sectorBlankRangeStart[8];
    UINT16 sectorBlankRangeEnd[8];
    UINT8 sectorNorthRef[8];
    UINT16 antennaOffsetValue;
    UINT8 antennaHeight;
    UINT8 systemHwStatus;
    UINT16 coarseTuneVoltage;
    UINT8 testVideo;
    UINT8 simulationEncoder;
    UINT16 numRangeBins;
    UINT16 homePosition;
    UINT16 pulseVoltage;
    UINT16 highVoltage;
    UINT16 pmuTemp;
    UINT16 heaterVoltage;
    UINT16 modulatorTemp;
    UINT16 tuneIndicatorVoltage;
    UINT16 monitorSignal;
    UINT16 pmuRxPowerVoltage;
    UINT16 actIfNoiseLevel;
    UINT8 actRfLevel;
    UINT8 eepromStatus;
    UINT16 tuneCtrlVoltage;
    UINT16 encoderControlVoltage;
    UINT32 flashUploadStatus;
    UINT32 flashUploadStatusVal1;
    UINT32 flashUploadStatusVal2;
    UINT16 measuredValues[4];
    UINT32 fullMagnetronCurrent;
    UINT16 svnSwVersion;
    UINT16 measuredValues2[16];
    UINT16 extendedSystemStatus[20];
    UINT16 transceiverSnNumH;
    UINT16 transceiverSnNumL;
    UINT16 systemSwVersion;
    UINT16 systemFwVersion;
    UINT16 masterDisplayIPv4AddrH;
    UINT16 masterDisplayIPv4AddrL;
    char masterDisplayName[SPX_NET_P322_MAX_DISPLAY_NAME_LEN];
};

/* Master Hello message.
 *
 * Note that the last field is defined in the spec as variable length.  It's
 * not clear if that means the size of the packet on the network might be
 * variable because of it, but assume it does.  Be careful with sizeof().
 */
#define	SPX_NET_P322_MSG_HELLO_SIZE_MINUS_NAME	(8)	/* Bytes */
struct SPxNetP322MsgHello
{
    UINT8 messageID;
    UINT8 payloadLenH;
    UINT16 payloadLenL;
    UINT32 version;
    char displayName[SPX_NET_P322_MAX_DISPLAY_NAME_LEN];
};

/* Master Heartbeat message. */
struct SPxNetP322MsgHeartbeat
{
    UINT8 messageID;
    UINT8 payloadLenH;
    UINT16 payloadLenL;
};

#define	SPX_NET_P322_MSG_TIMING_CONTROL_SIZE	(12)
struct SPxNetP322MsgTimingControl
{
    UINT8 messageID;
    UINT8 payloadLenH;
    UINT16 payloadLenL;
    UINT32 onTime;
    UINT32 txTime;
    UINT8 enableOnTime;
    UINT8 enableTxTime;
};

struct SPxNetP322MsgTransmitControl
{
    UINT8 messageID;
    UINT8 payloadLenH;
    UINT16 payloadLenL;
    UINT8 tuningMode;
    UINT8 reserved1;
    UINT8 txStatus;
    UINT8 txMode;
    UINT8 secBlankMode;
    UINT8 sartMode;
    UINT8 interferenceRejection;
    UINT8 ifBandwidth;
    UINT8 mute;
    UINT8 inhibitTx;
    UINT8 autoBiasGain;
    UINT8 prfJitter;
    UINT8 noiseSrcMode;
    UINT8 rfMonitorMode;
    UINT16 numRangeBins;
    UINT16 numAzi;
    UINT8 antennaRpm;
    UINT8 sleepMode;
    UINT16 manualTuneVoltage;
    UINT16 reserved2;
    UINT32 reserved3;
};

#define	SPX_NET_P322_MSG_IP_SETUP_SIZE_MINUS_NAME	(15*4)	/* Bytes */
struct SPxNetP322MsgIPSetup
{
    UINT8 messageID;
    UINT8 payloadLenH;
    UINT16 payloadLenL;
    UINT32 address1;
    UINT32 netmask1;
    UINT32 gateway1;
    UINT32 addressStatus1;
    UINT32 addressDiscovery1;
    UINT32 addressVideo1;
    UINT32 address2;
    UINT32 netmask2;
    UINT32 gateway2;
    UINT32 addressStatus2;
    UINT32 addressDiscovery2;
    UINT32 addressVideo2;
    UINT16 portControl;
    UINT16 portStatus;
    UINT16 portDiscovery;
    UINT16 portVideo;
    char radarName[SPX_NET_P322_MAX_RADAR_NAME_LEN];
};

struct SPxNetP322MsgSetup
{
    UINT8 messageID;
    UINT8 payloadLenH;
    UINT16 payloadLenL;
    UINT8 radarType;
    UINT8 antennaHeight;
    UINT8 waveGuideLength;
    UINT8 triggerDelay;
    UINT16 zeroRange;
    UINT16 antennaOffset;
    UINT16 logAmpBiasMode[6];
    UINT16 logAmpGainMode[6];
    UINT16 prfMode[6];
    UINT16 pulseDurationMode[6];
    UINT16 sectorBlankRangeStart[8];
    UINT16 sectorBlankRangeEnd[8];
    UINT8 sectorBlankNorthRef[8];
    UINT16 ifNoiseLevelMode[6];
    UINT8 rfLevel[6];
    UINT16 homePosition;
    UINT8 stcMode;
    UINT8 gearbox;
    UINT8 testVideo;
    UINT8 simulationEncoder;
    UINT32 transceiverSN;
    UINT16 coarseTuneVoltage;
    UINT16 reserved;
};

#define SPX_NET_P322_MSG_SAVE_SIZE	(5)
struct SPxNetP322MsgSave
{
    UINT8 messageID;
    UINT8 payloadLenH;
    UINT16 payloadLenL;
    UINT8 saveCmd;
};

struct SPxNetP322MsgFEStc
{
    UINT8 messageID;
    UINT8 payloadLenH;
    UINT16 payloadLenL;
    UINT8 feStc[6][130];
};

struct SPxNetP322MsgIFStc
{
    UINT8 messageID;
    UINT8 payloadLenH;
    UINT16 payloadLenL;
    UINT16 ifStc[6][130];
};

struct SPxNetP322MsgStcCorrection
{
    UINT8 messageID;
    UINT8 payloadLenH;
    UINT16 payloadLenL;
    UINT16 stcCorrection[6][130];
};

struct SPxNetP322MsgMaintenance
{
    UINT8 messageID;
    UINT8 payloadLenH;
    UINT16 payloadLenL;
    UINT8 operatingMode;
    UINT8 autoBiasPointSearch;
    UINT8 autoTunePointSearch;
    UINT8 eepromCtrlCmd;
};

#endif /* SPX_NET_P322_H_ */

/*********************************************************************
*
* End of file
*
**********************************************************************/

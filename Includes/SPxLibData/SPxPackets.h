/*********************************************************************
*
* (c) Copyright 2007 - 2022, Cambridge Pixel Ltd.
*
* Purpose:
*	Header containing definitions for SPx packets (e.g. for
*	recording files or network transmission etc.).
*
*	If the structures are changed/extended or new constants
*	added check if the corresponding structures/constants
*	require updating in the SPx.NET library (SPxLibDN).
*
*
* Revision Control:
*   17/10/22 v1.203  REW	Used signed values for xOffsets/yOffsets.
*
* Previous Changes:
*   14/10/22 1.202  REW	Add xOffsets and yOffsets to SPxPacketTrackFusion.
*   28/09/22 1.201  AGC	Add expiredFlags to SPxPacketNavData.
*   09/09/22 1.200  AGC	Add uniTrackType/sourceIndex to tracker status.
*   11/08/22 1.199  SP 	Change SPxPacketTrackSecondary targetHeight to REAL32.
*   10/08/22 1.198  AGC	Fix description of SPxPacketTrackerStatus reportType.
*   08/08/22 1.197  SP 	Add targetHeight extent to SPxPacketTrackSecondary.
*   26/07/22 1.196  BTB	Add SPxPacketWebRadarVideoBounds.
*   05/07/22 1.195  BTB	Add SPxPacketWebAScanReturns.
*   16/06/22 1.194  SP 	Add SPX_PACKET_TRACK_CLASS_TYPE_AEROSCOPE.
*   09/05/22 1.193  AGC	Add source index to minimal track report.
*   05/05/22 1.192  AGC	Add uniTrackType to minimal track report.
*   07/04/22 1.191  BTB	Add extra MHT fields to new SPxPacketTrackMht struct in SPxPacketTrackExtended.
*   28/03/22 1.190  SP 	Improve comment in SPxPacketTrackFusion, as part of fix for N/C #92.
*   08/03/22 1.189  REW	Add validity to SPxPacketTrackMinimal.
*   08/03/22 1.188  AGC	Add sensor altitude to plot/tracker status messages.
*   02/03/22 1.187  REW	Add posnFlags to SPxPacketPlot.
*   10/02/22 1.186  SP 	Add SPX_PACKET_TYPEB_GPSA_STATUS.
*   06/01/22 1.185  AGC	Add terrain height to SPxPacketNavData.
*   09/12/21 1.184  AGC	Add extended coast flag to SPxPacketTrackRst.
*   07/12/21 1.183  AGC	Add SPxPacketTerrainData.
*   29/11/21 1.182  BTB	Add parent area name and movable flag to SPxPacketAreaEvent.
*   26/11/21 1.181  AGC	Add vertical rate to SPxPacketTrackRst.
*   24/11/21 1.180  AGC	Add SPxPacketNavData.
*   15/11/21 1.179  BTB	Add mode info to SPxPacketAreaEvent.
*   09/11/21 1.178  SP 	Add SPX_PACKET_TYPEB_TEST.
*   15/10/21 1.177  REW	Add confidence to SPxPacketTrackRst.
*   15/10/21 1.176  REW	Move RST fields to sub structure, and enlarge.
*   28/09/21 1.175  AGC	Add RST track speed gains to SPxPacketTrackExtended.
*   27/09/21 1.174  AGC	Add RST specific fields to SPxPacketTrackExtended.
*   10/09/21 1.173  BTB	Add SPX_PACKET_TRACK_FLAG_FILTERED.
*   02/09/21 1.172  AGC	Add altitude SD field to SPxPacketTrackExtended.
*   31/08/21 1.171  REW	Add SPX_PACKET_THREAT_LEVEL_JUNK.
*   31/08/21 1.170  AGC	Add height fields to SPxPacketAreaEvent.
*   19/08/21 1.169  AGC	Add more fields to SPxPacketAreaEvent.
*   29/07/21 1.168  AGC	Add SPX_PACKET_TYPEB_AREA_EVENT.
*   19/05/21 1.167  AGC	Add SPX_PACKET_TYPEB_RST_PLOT.
*   11/05/21 1.166  AGC	Add SPX_PACKET_TRACK_SECONDARY_IFF_EXT_FLAGS_MODE_6.
*   28/04/21 1.165  SP 	Add SPX_PACKET_MISC_ASTERIX_CAT247.
*   02/03/21 1.164  REW	Add SPX_PACKET_TYPEB_DOPPLER plus structure.
*   01/03/21 1.163  AGC	Add SPX_PACKET_MISC_ASTERIX_CAT8.
*   23/02/21 1.162  REW	Add VR_EXCEED flag.
*   22/02/21 1.161  REW	Add vertical rate and climb/descend support.
*   06/11/20 1.160  SP 	Add wasIdle to SPxPacketRecord.
*   29/10/20 1.159  AGC	Increase sensor specific data size in SPxPacketTrackExtended.
*   08/10/20 1.158  AJH	Add SPX_PACKET_TRACK_CLASS_... enumerations.
*   06/10/20 1.157  AGC	Split MIL2525D fields into structure.
*   05/10/20 1.156  AJH	Add GeoJSON track classification type.
*   26/08/20 1.155  AGC	Add IJMS and MIL2525D fields to extended track.
*   21/08/20 1.154  BTB	Add comment regarding SPxMonitor.
*   04/08/20 1.153  BTB	Add SPX_PACKET_TYPEB_FR24_ADSB
*   15/07/20 1.152  SP 	Add dataSizeBytes to SPxPacketJsonData.
*   30/06/20 1.151  SP 	Add SPX_PACKET_TYPEB_TEWA_STATUS.
*   29/04/20 1.150  BTB	Add SPX_PACKET_TYPEB_INFODB and
*			SPxPacketJsonData.
*   02/01/20 1.149  SP 	Add stamp ID SPxPacketRecord.
*   13/11/19 1.148  SP 	Add SPX_PACKET_MISC_ASTERIX_CAT240.
*   12/11/19 1.147  SP 	Modify SPX_PACKET_TYPEB_MONITOR_STATUS and
*			SPX_PACKET_TYPEB_EXTENDED_HB.
*   07/11/19 1.146  SP 	Add SPX_PACKET_TYPEB_MONITOR_STATUS.
*   22/08/19 1.145  REW	Add SPX_PACKET_TYPEB_LICENSE_ENC and new fields
*			ready for License Manager support.
*   19/08/19 1.144  SP 	Add SPX_PACKET_TYPEB_EXTENDED_HB.
*   25/06/19 1.143  AGC	Add height to SPxPacketPlot.
*   09/05/19 1.142  AJH	Add sourceCode to SPF1 packet.
*   06/03/19 1.141  AJH	Add extra IFF_EXT_FLAGS definitions.
*   20/02/19 1.140  SP 	Add isIdle to SPxPacketRecord.
*   24/01/19 1.139  SP 	Remove SPxPacketProjectHB239 and rename
*			SPX_PACKET_TYPEB_PROJECT_HB to 
*			SPX_PACKET_TYPEB_PROJECT_GENERIC.
*   11/01/19 1.138  AGC	Add SPX_PACKET_TYPEB_PLOT_STATUS.
*   20/12/18 1.137  BTB	Update SPxPacketCameraPos to handle setting FOV limits.
*   02/11/18 1.136  AGC	Add track class type to normal track report.
*   26/10/18 1.135  REW	Add multiple altitudes and psr-ssr diffs to track rpts.
*   19/09/18 1.134  REW	Add speedSD and courseSD to extended track rpt.
*   31/08/18 1.133  AGC	Add isSecondary flag to SPxPacketCameraPos.
*   24/07/18 1.132  AGC	Add flags to SPxPacketTrackFusion.
*   12/07/18 1.131  SP 	Add SPX_PACKET_ALARM_STATUS_OUTSIDE.
*   02/07/18 1.130  AGC	Add SPxPacketProjectHB239.
*   28/06/18 1.129  SP 	Changes to SPxPacketAlarm for P236.
*   17/05/18 1.128  AGC	Add SPxPacketAsterix240SPF1.
*   20/04/18 1.127  REW	Add SPxPacketStrobe.
*   17/04/18 1.126  AGC	Add SPxPacketChanges.
*   05/04/18 1.125  AGC	Add SPX_PACKET_MISC_ASTERIX_CAT62.
*   21/03/18 1.124  REW	Add SPX_PACKET_TRACK_STATUS_PLOT.
*   02/03/18 1.123  REW	Add SPX_PACKET_TYPEB_KAL.
*   26/01/18 1.122  SP 	Add SPX_PACKET_TRACK_FLAG_EXTRAPOLATED.
*   19/01/18 1.121  REW	Add iffExtFlags and mil emergency support.
*   13/12/17 1.120  AGC	Add padding bytes to SPxPacketTrackExtended.
*   29/11/17 1.119  AGC	Add IFF Mode-C to SPxPacketTrackSecondary.
*   22/11/17 1.118  AGC	Add comms capability to SPxPacketTrackSecondary.
*   20/11/17 1.117  AGC	Add event time to extended track report.
*   17/11/17 1.116  AGC	Add IFF mode confidences to SPxPacketTrackSecondary.
*   17/11/17 1.115  REW	Restore track status comment from 1.108.
*   11/09/17 1.114  SP 	Add flags field to SPxPacketTrackThreat.
*   13/07/17 1.113  AGC	Add screen number to SPxPacketImageChunk.
*   23/05/17 1.112  REW	Add version and checksum to license info.
*   17/05/17 1.111  REW	Tweak last change.
*   17/05/17 1.110  REW	Add SPxPacketLicenseInfo.
*   21/04/17 1.109  SP 	Reduce alarmAreaName length to 32 in SPxPacketAlarm.
*   20/04/17 1.108  SP 	Add SPxPacketAlarm.
*   12/04/17 1.107  AGC	Use char for image stream name.
*   20/01/17 1.106  DGJ	Add track class into Normal report 
*   12/12/16 1.105  AGC	Reduce size of description string in extended track report.
*   12/12/16 1.104  REW	Combine two and rename third field in last change.
*   12/12/16 1.103  DGJ	New fields for extended track reports
*   30/11/16 1.102  AGC	Include description size in extended track report.
*   29/11/16 1.101  AGC	Add description to extended track report.
*   22/11/16 1.100  SP 	Replace generic threat types with user threat types.
*   16/11/16 1.99   SP 	Add generic threat types.
*   23/08/16 1.98   AGC	Add SPX_PACKET_TYPEB_CONFIG_FILE and SPxPacketConfigFile.
*   08/07/16 1.97   AGC	Support recording error descriptions.
*   24/06/16 1.96   AGC	Add version to TOC header.
*   20/04/16 1.95   AGC	Change threat structure name to char array.
*   24/03/16 1.94   REW	Add more SPX_PACKET_TRACK_SECONDARY_IFF_FLAGS...
*   14/03/16 1.93   AJH	Add maxRangeDiffMetres to SPxPacketTrackFusion.
*   09/03/16 1.92   AJH	Add sourceStatus to tracker status.
*   11/03/16 1.91   REW	Add iff12BitMode1 field to SPxPacketTrackSecondary.
*			Add iff values to SPxPacketPlot.
*   03/03/16 1.90   AGC	Add SPxPacketRadarReturnExtra.
*   08/02/16 1.89   REW	Re-order recent SPxPacketTrackSecondary changes
*			to preserve existing field offsets.
*   01/02/16 1.88   AJH	Add primary track IDs to SPxPacketTrackFusion.
*			Add further AIS data to SPxPacketTrackSecondary.
*   22/01/16 1.87   SP 	Add numChannels to SPX_PACKET_TYPEB_TOC.
*   10/12/15 1.86   SP 	Add SPX_MAX_NUM_PACKET_LINKS.
*   04/12/15 1.85   SP 	Remove SESSION_START and SESSION_STOP record types.
*   26/11/15 1.84   AGC	Include extracted area in SPxPacketAVTrack.
*			Support camera speed recording.
*   16/11/15 1.83   REW	Add flags to SPxPacketPlot.
*   16/11/15 1.82   AGC	Update SPxPacketAVTrack.
*   09/11/15 1.81   AGC	Add SPX_PACKET_TYPEB_CAMERA_COMMAND and SPxPacketCameraCommand.
*   03/11/15 1.80   AGC	Add SPX_PACKET_TYPEB_CAMERA_POS and SPxPacketCameraPos.
*   15/10/15 1.79   REW	Add lastChangedSecs to SPxPacketTrackThreat.
*   31/07/15 1.78   AGC	Add SPX_PACKET_TRACK_FLAG_ON_GROUND.
*			Add SPX_PACKET_TRACK_FLAG_FIXED.
*   24/06/15 1.77   REW	Add SPX_PACKET_TRACK_FLAG_MANUAL_INIT.
*   05/06/15 1.76   SP 	Add session name to SPxPacketRecord.
*   21/05/15 1.75   SP 	Add extra reserved fields to SPxPacketRecord.
*                       Add prevStatus to SPxPacketRecord.
*   15/05/15 1.74   SP 	Fix alignment in SPxPacketLink.
*                       Remove unused SPX_PACKET_TYPEB_CHAN_CONFIG packet.
*   11/05/15 1.73   SP 	Add SPX_PACKET_TYPEB_LINK and SPxPacketLink.
*   05/05/15 1.72   SP 	Add status field to SPxPacketRecord.
*   29/04/15 1.71   AGC	Add SPX_PACKET_TYPEB_JSON and SPxPacketJson
*   20/02/15 1.70   SP 	Correct reserved field sizes in new structures.
*                       Rename SPxPacketImage as SPxPacketImageFile.
*                       Rename SPxPacketChannel as SPxPacketChanSelect.
*                       Add SPX_PACKET_TYPEB_CHAN_DB_CONFIG and SPxPacketChanDbConfig.
*                       Add SPX_PACKET_TYPEB_CHAN_CONFIG and SPxPacketChanConfig.
*   16/02/15 1.69   SP 	Add SPX_PACKET_TYPEB_CHANNEL and SPxPacketChannel.
*                       Add SPX_PACKET_TYPEB_IMAGE and SPxPacketImage.
*                       Add SPX_PACKET_TYPEB_RECORD and SPxPacketRecord.
*   14/10/14 1.68   AGC	Add SPX_PACKET_TRACK_FLAG_TEST.
*   03/09/14 1.67   REW	Rename BTE2000 to HDLC_IFF.
*   29/07/14 1.66   REW	Add iffFlags to SPxPacketTrackSecondary.
*   17/06/14 1.65   REW	Change SPxPacketFlightPlan isRepeating to flags.
*   06/06/14 1.64   REW	Add lastEditSecs to SPxPacketFlightPlan.
*   08/04/14 1.63   REW	Add SPX_PACKET_THREAT_LEVEL_USER0, 1, 2 etc.
*   01/04/14 1.62   AGC	Improve formatting of SPX_PACKET_AV_TRACK_BEARING.
*   26/03/14 1.61   AGC	Add flags field to SPxPacketTrackMinimal.
*   17/01/14 1.60   REW	Add iffMode3A/2/1 to SPxPacketTrackSecondary.
*   15/01/14 1.59   REW	Add SPX_PACKET_TYPEB_FLIGHT_PLAN.
*   04/12/13 1.58   REW	Tweak SPxPacketTrackThreat, add LEVEL_CIVILIAN.
*			Add SPX_PACKET_TRACK_SENSOR_USER.
*   22/11/13 1.57   AGC	Add SPX_PACKET_TYPEB_AV_TRACK and SPxPacketAVTrack.
*   15/11/13 1.56   SP 	Move threat info to extended track report.
*			Add SPxPacketFile.
*			Add SPX_PACKET_TYPEB_ERROR and 
*			SPX_PACKET_TYPEB_FILE.
*   08/11/13 1.55   SP 	Add threat info.
*   04/10/13 1.54   AGC	Simplify headers.
*   24/05/13 1.53   AGC	Add SPX_PACKET_MISC_ASTERIX_CAT19.
*			Add SPX_PACKET_MISC_ASTERIX_CAT20.
*   09/05/13 1.52   REW	Add SPX_PACKET_MISC_ASTERIX_CAT21.
*   03/05/13 1.51   AGC	Add SPX_PACKET_MISC_ASTERIX_CAT34.
*   19/04/13 1.50   REW	Add SPX_PACKET_TYPEB_ASTERIX.
*   15/04/13 1.49   REW	Add sensorLatDegs/LongDegs to TrackerStatus_t.
*   14/03/13 1.48   REW	Add SPX_PACKET_TRACK_MODE_... definitions.
*   13/02/13 1.47   SP 	Move SR types/defines here from SPxScreen.h.
*   12/02/13 1.46   DGJ	Use reserved field for TTM id.
*   12/02/13 1.45   AGC	Add alertType field to SPxPacketAlert.
*   25/01/13 1.44   REW	Add SPX_PACKET_TYPEB_DROPOUT.
*   21/12/12 1.43   AGC	Add SPX_PACKET_TRACK_EXT_CPA.
*			Add alert packets.
*   18/12/12 1.42   REW	Add SPX_PACKET_TRACK_EXT_GATE.
*   24/10/12 1.41   REW	Add SPX_PACKET_TYPEB_LICENSE.
*   19/07/12 1.40   REW	Add targetFlags to SPxPacketTrackSecondary.
*   20/06/12 1.39   REW	Add SPX_PACKET_TYPEB_BTE2000.
*   30/04/12 1.38   DGJ	Add SPX_PACKET_TRACK_EXT_LATLONG_MEAS
*			Add lat/longDegsMeas to extended reports
*   14/02/12 1.37   REW	Add SPX_PACKET_MISC_ placeholders.
*   26/01/12 1.36   REW	Add SPX_PACKET_TRACK_STATUS_LOST.
*   13/01/12 1.35   AGC	Add SPX_PACKET_TYPEB_METADATA_RAW.
*   21/12/11 1.34   REW	Add TrackFusion and TrackSecondary packet defs.
*   02/12/11 1.33   AGC	Add options to SPxPacketMetadata.
*   25/10/11 1.32   AGC	Add SPxPacketSimTargetList and SPxPacketSimTarget.
*			Add SPX_PACKET_TYPEB_SIM_TARGET_LIST.
*   04/10/11 1.31   REW	Add SPX_PACKET_TYPEB_PIM_DATA and typedef.
*   22/09/11 1.30   AGC	Add streamPort field to SPxPacketAVInfo.
*   16/09/11 1.29   REW	Add numDetectionsPQ to SPxPacketTrackNormal_t.
*   15/09/11 1.28   REW	Add serverVersion to SPxPacketTrackerStatus_t.
*   13/09/11 1.27   AGC	Add durationUsecs field to SPxPacketImageChunk.
*   30/08/11 1.26   SP 	Use pad1 in SPxPacketRadarReturn 
*                       to store dataFlags.
*   19/08/11 1.25   AGC	Add SPX_PACKET_TYPEB_AUDIO_CHUNK.
*			Add audio fields to SPxPacketImageChunk.
*			Add droppedPackets field to SPxPacketAVInfo.
*   27/07/11 1.24   AGC	Tweak comments for SPxPacketImageChunk.
*   27/06/11 1.23   AGC	Add fields to SPxPacketAVInfo.
*			Add fields to SPxPacketImgSndStreamConfig.
*   09/06/11 1.22   AGC	Add SPX_PACKET_TYPEB_AV_INFO.
*			Add fields to SPxPacketImageChunk for AV server.
*   21/01/11 1.21   REW	Add SPX_PACKET_TYPEB_AIS.
*   03/12/10 1.20   REW	Add SPX_PACKET_TYPEB_SERIAL and ADSB.
*   04/10/10 1.19   AGC	Add Image Server packets.
*   24/05/10 1.18   REW	Add SPX_PACKET_TRACK_EXT_MSGTIME and SENSORDATA
*   09/03/10 1.17   REW	Add SPX_PACKET_TRACK_EXT_LATLONG.
*   19/10/09 1.16   REW	Add timeIntervalUsecs to SPxPacketRadarReturn.
*   21/08/09 1.15   REW	Add sweepline fields to WindowConfig struct.
*   30/07/09 1.14   REW	Add SPX_PACKET_TRACK_STATUS_... fields.
*   25/06/09 1.13   REW	Add SPX_PACKET_TYPEB_PLOT.
*   18/06/09 1.12   REW	Add SPX_PACKET_TYPEB_TOC.
*   23/02/09 1.11   REW	Add SPX_PACKET_TYPEB_NMEA.
*   09/02/09 1.10   REW	Add SPX_PACKET_TRACK_EXT_AGE
*			and SPX_PACKET_TYPEB_TRACKER_STATUS.
*   07/01/09 1.9    REW	Add SPX_PACKET_TYPEB_NET and structure.
*   27/11/08 1.8    REW	Add SPX_PACKET_TYPEB_SR_... definitions.
*   10/11/08 1.7    REW	Add more fields to track report definitions.
*   16/09/08 1.6    REW	Add track report definitions.
*   19/08/08 1.5    REW	Add definitions and types for the TypeB
*			WINDOW_CONFIG, WINDOW_PATCHES, ACK and
*			PARAM_LIST packets.
*   02/07/08 1.4    REW	Add SPX_PACKET_TYPEB_NEXT_NAME.
*   14/12/07 1.3    REW	Add SPxPacketHeaderUnion.
*   11/12/07 1.2    REW	Tweak packet contents, IDs etc.
*   10/12/07 1.1    REW	Initial Version.
**********************************************************************/

#ifndef _SPX_PACKETS_H
#define _SPX_PACKETS_H

/*
 * Other headers required.
 */
/* We need SPxLibUtils for common types etc. */
#include "SPxLibUtils/SPxTypes.h"


/*********************************************************************
*
*   Constants
*
**********************************************************************/

/*
 * Define magic values for the packet headers.
 */
#define	SPX_PACKET_MAGIC_A	(('C' << 8) | ('A'))	/* Short packet */
#define	SPX_PACKET_MAGIC_B	(('C' << 8) | ('B'))	/* Longer packet */


/*
 * Define the packetType values for SPxPacketHeaderA headers.
 *
 * These need to be in the range 0 to 255.
 *
 * Future possibilities are file comments (labels, content), short
 * network packets such as ping/respond, etc. etc.
 */
#define SPX_PACKET_TYPEA_ASTERIX_CAT240_SPF1	241	/* 0xF1 */

/* Currently, there are very few packet-A's defined, although be aware that
 * the following numbers are used in similar meanings (e.g. track reporter)
 * but without the SPx packet header.
 */
#define	SPX_PACKET_MISC_ASTERIX_CAT8		8	/* 0x08 */
#define	SPX_PACKET_MISC_ASTERIX_CAT10		10	/* 0x0A */
#define	SPX_PACKET_MISC_ASTERIX_CAT19		19	/* 0x13 */
#define	SPX_PACKET_MISC_ASTERIX_CAT20		20	/* 0x14 */
#define	SPX_PACKET_MISC_ASTERIX_CAT21		21	/* 0x15 */
#define	SPX_PACKET_MISC_ASTERIX_CAT34		34	/* 0x22 */
#define	SPX_PACKET_MISC_ASTERIX_CAT48		48	/* 0x30 */
#define	SPX_PACKET_MISC_ASTERIX_CAT62		62	/* 0x3E */
#define	SPX_PACKET_MISC_ASTERIX_CAT240		240	/* 0xF0 */
#define	SPX_PACKET_MISC_ASTERIX_CAT247		247	/* 0xF7 */
#define	SPX_PACKET_MISC_NMEA_0183		183	/* 0xB7 */

/*
 * Define the packetType values for SPxPacketHeaderB headers.
 *
 * These can be in the range 0 to 65535, but to keep them unique with the
 * type A headers we don't use the 0 to 255 range.
 *
 * Future possibilities are endless but include index/table-of-contents etc.
 *
 * - Java and .NET may require update if new packet types added.
 *
 * When adding a new packet type, please review 
 * SPxSdbStream::GetSPxPacketBType() and add to this function if 
 * appropriate so that it can be detected by SPxMonitor.
 */
#define	SPX_PACKET_TYPEB_RADAR_CONFIG		0x101	/* SPxPacketRadarConfig */
#define	SPX_PACKET_TYPEB_RADAR_RETURN		0x102	/* SPxPacketRadarReturn */
#define	SPX_PACKET_TYPEB_NEXT_NAME		0x103	/* NULL-terminated string */
#define	SPX_PACKET_TYPEB_WINDOW_CONFIG		0x104	/* SPxPacketWindowConfig */
#define	SPX_PACKET_TYPEB_WINDOW_PATCHES		0x105	/* SPxPacketWindowPatches */
#define	SPX_PACKET_TYPEB_PARAM_LIST		0x106	/* SPxPacketParamList */
#define	SPX_PACKET_TYPEB_ACK			0x107	/* SPxPacketAck */
#define	SPX_PACKET_TYPEB_HEARTBEAT		0x108	/* SPxScNetHeartbeatStruct */
#define	SPX_PACKET_TYPEB_PIM_DATA		0x109	/* SPxPacketPimData */
#define	SPX_PACKET_TYPEB_TRACK_MIN		0x110	/* SPxPacketTrackMinimal */
#define	SPX_PACKET_TYPEB_TRACK_NORM		0x111	/* SPxPacketTrackNormal */
#define	SPX_PACKET_TYPEB_TRACK_EXT		0x112	/* SPxPacketTrackExtended */
#define	SPX_PACKET_TYPEB_PLOT			0x113	/* SPxPacketPlot */
#define	SPX_PACKET_TYPEB_RST_PLOT		0x114	/* SPxRstPlot */
#define	SPX_PACKET_TYPEB_TRACKER_STATUS		0x115	/* SPxPacketTrackerStatus */
#define	SPX_PACKET_TYPEB_PLOT_STATUS		0x116	/* SPxPacketPlotStatus */
#define	SPX_PACKET_TYPEB_SR_MBR			0x120	/* SPxSRfileMasterBlock */
#define	SPX_PACKET_TYPEB_SR_PAUSE		0x121	/* Currently no payload */
#define	SPX_PACKET_TYPEB_SR_FRAME		0x122	/* SPxSRframeHeader */
#define	SPX_PACKET_TYPEB_NET			0x130	/* SPxPacketNet */
#define	SPX_PACKET_TYPEB_NMEA			0x131	/* NULL-terminated $ string */
#define	SPX_PACKET_TYPEB_SERIAL			0x132	/* Raw (binary?) data */
#define	SPX_PACKET_TYPEB_ADSB			0x133	/* Binary message */
#define	SPX_PACKET_TYPEB_AIS			0x134	/* Like NMEA, but !. */
#define	SPX_PACKET_TYPEB_HDLC_IFF		0x135	/* HDLC-IFF reports */
#define	SPX_PACKET_TYPEB_TOC			0x140	/* SPxPacketTOCHdr + Entry[] */
#define SPX_PACKET_TYPEB_IMG_SND_MNGR_STATUS	0x150	/* SPxPacketImgSndMngrStatus */
#define SPX_PACKET_TYPEB_IMAGE_CHUNK		0x151	/* SPxPacketImageChunk for video */
#define SPX_PACKET_TYPEB_METADATA		0x152	/* SPxPacketMetadata */
#define SPX_PACKET_TYPEB_AV_INFO		0x153	/* SPxPacketAVInfo */
#define SPX_PACKET_TYPEB_AUDIO_CHUNK		0x154	/* SPxPacketImageChunk for audio */
#define SPX_PACKET_TYPEB_METADATA_RAW		0x155	/* Raw binary metadata */
#define SPX_PACKET_TYPEB_AV_TRACK		0x156	/* SPxPacketAVTrack */
#define SPX_PACKET_TYPEB_SIM_TARGET_LIST	0x160	/* SPxPacketSimTargetList + targets. */
#define SPX_PACKET_TYPEB_LICENSE		0x170	/* Empty or SPxPacketLicenseInfo */
#define SPX_PACKET_TYPEB_LICENSE_ENC		0x171	/* Encrypted SPxPacketLicenseInfo */
#define SPX_PACKET_TYPEB_ALERT_ERROR		0x180	/* SPxPacketAlertError */
#define SPX_PACKET_TYPEB_ALERT_HPX_ALARM	0x181	/* SPxPacketAlertHPxAlarm */
#define SPX_PACKET_TYPEB_ALERT_LOGIC		0x182	/* SPxPacketAlertLogic */
#define SPX_PACKET_TYPEB_DROPOUT		0x190	/* SPxPacketDropout */
#define	SPX_PACKET_TYPEB_ASTERIX		0x1A0	/* Asterix msgs */
#define SPX_PACKET_TYPEB_ERROR			0x1B0	/* Error message. */
#define SPX_PACKET_TYPEB_ERROR_DESC		0x1B1	/* SPxPacketErrorDesc */
#define SPX_PACKET_TYPEB_FILE			0x1C0	/* File data. */
#define SPX_PACKET_TYPEB_CONFIG_FILE		0x1C1	/* SPxPacketConfigFile + Configuration file text. */
#define SPX_PACKET_TYPEB_FLIGHT_PLAN		0x1D0	/* SPxPacketFlightPlan. */
#define SPX_PACKET_TYPEB_CHAN_SELECT		0x1E0	/* SPxPacketChanSelect. */
#define SPX_PACKET_TYPEB_IMAGE_FILE		0x1F0	/* SPxPacketImageFile. */
#define SPX_PACKET_TYPEB_RECORD			0x200	/* SPxPacketRecord. */
#define SPX_PACKET_TYPEB_CHAN_DB_CONFIG		0x210	/* SPxPacketChanDbConfig. */
#define SPX_PACKET_TYPEB_JSON			0x220	/* SPxPacketJson + JSON data. */
#define SPX_PACKET_TYPEB_LINK			0x230	/* SPxPacketLink. */
#define SPX_PACKET_TYPEB_CAMERA_POS		0x240	/* SPxPacketCameraPos. */
#define SPX_PACKET_TYPEB_CAMERA_COMMAND		0x241	/* SPxPacketCameraCommand. */
#define SPX_PACKET_TYPEB_ALARM			0x250	/* SPxPacketAlarm. */
#define SPX_PACKET_TYPEB_KAL			0x260	/* KAL receiver packets */
#define SPX_PACKET_TYPEB_CHANGES		0x270	/* SPxPacketChanges. */
#define SPX_PACKET_TYPEB_STROBE			0x280	/* SPxPacketStrobe. */
#define SPX_PACKET_TYPEB_PROJECT_HB		0x290	/* Was SPxPacketProjectHB239 (now removed). */
#define	SPX_PACKET_TYPEB_EXTENDED_HB		0x2A0	/* Extended JSON format heartbeat. */
#define	SPX_PACKET_TYPEB_MONITOR_STATUS		0x2B0	/* Monitor JSON format status message. */
#define SPX_PACKET_TYPEB_PROJECT_GENERIC	0x300	/* SPxPacketProjectGeneric */
#define	SPX_PACKET_TYPEB_EXTENDED_HB_DEP	0x310	/* Deprecated extended heartbeat. */
#define SPX_PACKET_TYPEB_INFODB			0x320	/* SPxPacketJsonData + info DB JSON data. */
#define	SPX_PACKET_TYPEB_TEWA_STATUS		0x330	/* SPxPacketJsonData + TEWA status JSON message. */
#define SPX_PACKET_TYPEB_FR24_ADSB		0x340	/* Gzipped JSON string containing ADSB track data. */
#define SPX_PACKET_TYPEB_DOPPLER		0x350	/* SPxPacketDopplerHdr + payloads */
#define SPX_PACKET_TYPEB_AREA_EVENT		0x360	/* SPxPacketAreaEvent + numPoints SPxPacketAreaEventPos. */
#define SPX_PACKET_TYPEB_NAV_DATA		0x361	/* SPxPacketNavData */
#define SPX_PACKET_TYPEB_TEST			0x370	/* Generic test packet. */
#define SPX_PACKET_TYPEB_GPSA_STATUS		0x380	/* GPS Assist JSON format status message. */

/*
 * Define bitflags for SPxPacketTrackExtended optional fields.
 */
#define	SPX_PACKET_TRACK_EXT_RADIALSPEED	0x00000001
#define	SPX_PACKET_TRACK_EXT_AGE		0x00000002
#define	SPX_PACKET_TRACK_EXT_LATLONG		0x00000004
#define	SPX_PACKET_TRACK_EXT_MSGTIME		0x00000008
#define	SPX_PACKET_TRACK_EXT_SENSORDATA		0x00000010
#define	SPX_PACKET_TRACK_EXT_ALTITUDE		0x00000020
#define	SPX_PACKET_TRACK_EXT_FUSION		0x00000040
#define	SPX_PACKET_TRACK_EXT_SECONDARY		0x00000080
#define	SPX_PACKET_TRACK_EXT_LATLONG_MEAS	0x00000100
#define	SPX_PACKET_TRACK_EXT_GATE		0x00000200
#define	SPX_PACKET_TRACK_EXT_CPA		0x00000400
#define	SPX_PACKET_TRACK_EXT_THREAT		0x00000800
#define	SPX_PACKET_TRACK_EXT_DESCRIPTION	0x00001000
#define	SPX_PACKET_TRACK_EXT_ACCEL		0x00002000
#define	SPX_PACKET_TRACK_EXT_MOTION_STATUS	0x00004000
#define	SPX_PACKET_TRACK_EXT_EVENT_TIME		0x00008000
#define	SPX_PACKET_TRACK_EXT_VELOCITY_SD	0x00010000
#define	SPX_PACKET_TRACK_EXT_IJMS		0x00020000
#define	SPX_PACKET_TRACK_EXT_MIL2525D		0x00040000
#define	SPX_PACKET_TRACK_EXT_ALTITUDE_SD	0x00080000
#define SPX_PACKET_TRACK_EXT_RST		0x00100000
#define SPX_PACKET_TRACK_EXT_MHT		0x00200000
#define	SPX_PACKET_TRACK_EXT_CONTINUED		0x80000000

/*
 * Define values for SPxPacketTrackMinimal status field.
 */
#define	SPX_PACKET_TRACK_STATUS_DELETED		0
#define	SPX_PACKET_TRACK_STATUS_PROVISIONAL	1
#define	SPX_PACKET_TRACK_STATUS_ESTABLISHED	2
#define	SPX_PACKET_TRACK_STATUS_LOST		3
#define	SPX_PACKET_TRACK_STATUS_PLOT		4	/* One-shot plot */
#define	SPX_PACKET_TRACK_STATUS_UNKNOWN		9

/*
 * Define values for SPxPacketTrackMinimal validity field.
 */
#define	SPX_PACKET_TRACK_INVALID_NONE		0	/* Not invalid */
#define	SPX_PACKET_TRACK_INVALID_GENERAL	1	/* Invalid, no reason */
#define	SPX_PACKET_TRACK_INVALID_CONFIDENCE	2	/* Conf or M/N */
#define	SPX_PACKET_TRACK_INVALID_LOST		3	/* Not been updated */
#define	SPX_PACKET_TRACK_INVALID_SPEED_BAD	10	/* Speed invalid */
#define	SPX_PACKET_TRACK_INVALID_SPEED_UNSTABLE 11	/* Speed stability */
#define	SPX_PACKET_TRACK_INVALID_SPEED_LOW	12	/* Speed too low */
#define	SPX_PACKET_TRACK_INVALID_SPEED_HIGH	13	/* Speed too high */
#define	SPX_PACKET_TRACK_INVALID_SPEED_RADIAL	14	/* Radial speed err */
#define	SPX_PACKET_TRACK_INVALID_COURSE_BAD	20	/* Course invalid */
#define	SPX_PACKET_TRACK_INVALID_COURSE_UNSTABLE 21	/* Course stability */
#define	SPX_PACKET_TRACK_INVALID_POSN_LOCATION	30	/* Invalid position */
#define	SPX_PACKET_TRACK_INVALID_POSN_RANGE	31	/* Out of range */
#define	SPX_PACKET_TRACK_INVALID_POSN_TERRAIN	32	/* Hidden by terrain */


/*
 * Define values for SPxPacketTrackMinimal flags field.
 */
#define SPX_PACKET_TRACK_FLAG_SIMULATED		0x01
#define SPX_PACKET_TRACK_FLAG_TEST		0x02
#define SPX_PACKET_TRACK_FLAG_MANUAL_INIT	0x04
#define SPX_PACKET_TRACK_FLAG_ON_GROUND		0x08
#define SPX_PACKET_TRACK_FLAG_FIXED		0x10
#define SPX_PACKET_TRACK_FLAG_EXTRAPOLATED	0x20
#define SPX_PACKET_TRACK_FLAG_FILTERED		0x40

/*
 * Define similar (but different!) flags for SPxPacketPlot posnFlags field.
 */
#define SPX_PACKET_PLOT_POSNFLAG_ALT_RADAR		0x01
#define SPX_PACKET_PLOT_POSNFLAG_ALT_FL			0x02
#define SPX_PACKET_PLOT_POSNFLAG_ALT_GEOM		0x04
#define SPX_PACKET_PLOT_POSNFLAG_ALT_IS_RADARREL	0x08
#define SPX_PACKET_PLOT_POSNFLAG_ON_GROUND		0x10
#define SPX_PACKET_PLOT_POSNFLAG_FIXED			0x20

/*
 * Defines values for SPxPacketTrackNormal trackClassType field.
 */
#define SPX_PACKET_TRACK_CLASS_TYPE_MHT		0x00
#define SPX_PACKET_TRACK_CLASS_TYPE_ASTERIX	0x01
#define SPX_PACKET_TRACK_CLASS_TYPE_GEOJSON	0x02
#define SPX_PACKET_TRACK_CLASS_TYPE_AEROSCOPE	0x04

/*
 * Defines track class type-specific values for
 * SPxPacketTrackNormal trackClass field.
 */
#define SPX_PACKET_TRACK_CLASS_GEOJSON_LOW_CONF	    0x00
#define SPX_PACKET_TRACK_CLASS_GEOJSON_HIGH_CONF    0x01

/*
 * Define bitflags for SPxPacketTrackExtended sub-field sensor types.
 * Update SPxTrackGetStringForSensorType() if adding more types.
 */
#define	SPX_PACKET_TRACK_SENSOR_PRIMARY		0x00000001
#define	SPX_PACKET_TRACK_SENSOR_AIS		0x00000002
#define	SPX_PACKET_TRACK_SENSOR_ADSB		0x00000004
#define	SPX_PACKET_TRACK_SENSOR_IFF		0x00000008
#define	SPX_PACKET_TRACK_SENSOR_USER		0x00000010
#define	SPX_PACKET_TRACK_SENSOR_OTHER		0x80000000

/* Define bitflags for SPxPacketTrackSecondary iffFlags field. */
#define	SPX_PACKET_TRACK_SECONDARY_IFF_FLAGS_SPI	0x01
#define	SPX_PACKET_TRACK_SECONDARY_IFF_FLAGS_X		0x02
#define	SPX_PACKET_TRACK_SECONDARY_IFF_FLAGS_MODE_1	0x04 /* 6-bit */
#define	SPX_PACKET_TRACK_SECONDARY_IFF_FLAGS_MODE_1_12BIT 0x08 /* 12-bit */
#define	SPX_PACKET_TRACK_SECONDARY_IFF_FLAGS_MODE_2	0x10
#define	SPX_PACKET_TRACK_SECONDARY_IFF_FLAGS_MODE_A	0x20
#define	SPX_PACKET_TRACK_SECONDARY_IFF_FLAGS_MODE_C	0x40
#define	SPX_PACKET_TRACK_SECONDARY_IFF_FLAGS_UNCERTAIN	0x80

/* Define bitflags for SPxPacketTrackSecondary iffExtFlags field. */
#define	SPX_PACKET_TRACK_SECONDARY_IFF_EXT_FLAGS_ME	0x01	/* Mil Emerg */
#define	SPX_PACKET_TRACK_SECONDARY_IFF_EXT_FLAGS_SMOOTH	0x02	/* Smoothed/not extracted */
#define	SPX_PACKET_TRACK_SECONDARY_IFF_EXT_FLAGS_GARB	0x04	/* Garbled */
#define	SPX_PACKET_TRACK_SECONDARY_IFF_EXT_FLAGS_NVAL	0x08	/* Not validated */
#define	SPX_PACKET_TRACK_SECONDARY_IFF_EXT_FLAGS_MODE_6	0x10	/* Mode 6 */

/* Define bitflags for SPxPacketTrackSecondary iffConfidenceFlags field. */
#define	SPX_PACKET_TRACK_SECONDARY_IFF_CONF_FLAGS_MODE_1	0x01
#define	SPX_PACKET_TRACK_SECONDARY_IFF_CONF_FLAGS_MODE_2	0x02
#define	SPX_PACKET_TRACK_SECONDARY_IFF_CONF_FLAGS_MODE_3A	0x04
#define	SPX_PACKET_TRACK_SECONDARY_IFF_CONF_FLAGS_MODE_C	0x08

/* Define bitFlags for SPxPacketTrackSecondary fieldFlags. */
#define	SPX_PACKET_TRACK_SECONDARY_FIELD_FLAGS_ALT_RADAR	0x01
#define	SPX_PACKET_TRACK_SECONDARY_FIELD_FLAGS_ALT_FL		0x02
#define	SPX_PACKET_TRACK_SECONDARY_FIELD_FLAGS_ALT_GEOM		0x04
#define	SPX_PACKET_TRACK_SECONDARY_FIELD_FLAGS_VR_GEOM		0x08
#define	SPX_PACKET_TRACK_SECONDARY_FIELD_FLAGS_VR_BARO		0x10

/* Define bitFlags for SPxPacketTrackSecondary statusFlags. */
#define	SPX_PACKET_TRACK_SECONDARY_STATUS_FLAGS_CLIMB		0x01
#define	SPX_PACKET_TRACK_SECONDARY_STATUS_FLAGS_DESCEND		0x02
#define	SPX_PACKET_TRACK_SECONDARY_STATUS_FLAGS_VR_EXCEED	0x04

/* Define bitFlags for SPxPacketTrackFusion flags field. */
#define SPX_PACKET_TRACK_FUSE_FLAG_ALL_COASTED		0x00000001
#define SPX_PACKET_TRACK_FUSE_FLAG_PSR_TO_SSR_SET	0x00000002
#define SPX_PACKET_TRACK_FUSE_FLAG_PSR_TO_SSR_INV	0x00000004

/* Define modes for track reporting. */
#define	SPX_PACKET_TRACK_MODE_ALL		0	/* All tracks */
#define	SPX_PACKET_TRACK_MODE_DESIGNATED	1	/* Designated tracks */
#define	SPX_PACKET_TRACK_MODE_NEAREST		2	/* Nearest track */
#define	SPX_PACKET_TRACK_MODE_NEWEST		3	/* Newest track */

/*
 * Defines for threat reporting.
 */

/* Threat types. */
#define SPX_PACKET_THREAT_TYPE_UNKNOWN      0
#define SPX_PACKET_THREAT_TYPE_CAR          1
#define SPX_PACKET_THREAT_TYPE_TRUCK        2
#define SPX_PACKET_THREAT_TYPE_HELO         3
#define SPX_PACKET_THREAT_TYPE_PLANE        4
#define SPX_PACKET_THREAT_TYPE_MAN          5
#define SPX_PACKET_THREAT_TYPE_ARMVECH      6
#define SPX_PACKET_THREAT_TYPE_USER0        10
#define SPX_PACKET_THREAT_TYPE_USER1        11
#define SPX_PACKET_THREAT_TYPE_USER2        12
#define SPX_PACKET_THREAT_TYPE_USER3        13
#define SPX_PACKET_THREAT_TYPE_USER4        14
#define SPX_PACKET_THREAT_TYPE_USER5        15
#define SPX_PACKET_THREAT_TYPE_USER6        16
#define SPX_PACKET_THREAT_TYPE_USER7        17
#define SPX_PACKET_THREAT_TYPE_USER8        18
#define SPX_PACKET_THREAT_TYPE_USER9        19

/* Threat levels. */
#define SPX_PACKET_THREAT_LEVEL_UNCLASSIFIED    0
#define SPX_PACKET_THREAT_LEVEL_FRIENDLY        1
#define SPX_PACKET_THREAT_LEVEL_NEUTRAL         2
#define SPX_PACKET_THREAT_LEVEL_HOSTILE         3
#define SPX_PACKET_THREAT_LEVEL_CIVILIAN	4
#define SPX_PACKET_THREAT_LEVEL_JUNK		5
#define SPX_PACKET_THREAT_LEVEL_USER0		10
#define SPX_PACKET_THREAT_LEVEL_USER1		11
#define SPX_PACKET_THREAT_LEVEL_USER2		12
#define SPX_PACKET_THREAT_LEVEL_USER3		13
#define SPX_PACKET_THREAT_LEVEL_USER4		14
#define SPX_PACKET_THREAT_LEVEL_USER5		15
#define SPX_PACKET_THREAT_LEVEL_USER6		16
#define SPX_PACKET_THREAT_LEVEL_USER7		17
#define SPX_PACKET_THREAT_LEVEL_USER8		18
#define SPX_PACKET_THREAT_LEVEL_USER9		19

/* Threat flags. */
#define SPX_PACKET_THREAT_FLAG_TYPE_PROVISIONAL 0x01

/*
 * Define values for SPxPacketNet fields.
 */
#define	SPX_PACKET_NET_TYPE_TCP		0	/* For "packetType" */
#define	SPX_PACKET_NET_TYPE_UDP		1	/* For "packetType" */
#define	SPX_PACKET_NET_FMT_RAW		0	/* For "packetFormat" */
#define	SPX_PACKET_NET_FMT_ZLIB		1	/* For "packetFormat" */

/*
 * Defines for AV track reporting.
 */
#define SPX_PACKET_AV_TRACK_CAM_LATLONG		0x00000001
#define SPX_PACKET_AV_TRACK_CAM_ALTITUDE	0x00000002
#define SPX_PACKET_AV_TRACK_CAM_BEARING		0x00000004
#define SPX_PACKET_AV_TRACK_CAM_ELEVATION	0x00000008
#define SPX_PACKET_AV_TRACK_BEARING		0x00000010
#define SPX_PACKET_AV_TRACK_ELEVATION		0x00000020

/*
 * Define bitflags for SPxPacketAlertLogic optional fields.
 * (The region field flags match the values in the SPxLogicRuleRegionType
 * enumeration for convenience).
 */
#define SPX_PACKET_ALERT_LOGIC_NONE			0x00000000
#define SPX_PACKET_ALERT_LOGIC_REGION_ENTER		0x00000001
#define SPX_PACKET_ALERT_LOGIC_REGION_LEAVE		0x00000002
#define SPX_PACKET_ALERT_LOGIC_REGION_CREATED		0x00000004
#define SPX_PACKET_ALERT_LOGIC_REGION_LOST		0x00000008
#define SPX_PACKET_ALERT_LOGIC_REGION_IN		0x00000010
#define SPX_PACKET_ALERT_LOGIC_REGION_WAS_FIRST_IN	0x00000020
#define SPX_PACKET_ALERT_LOGIC_CPA			0x00001000

/*
 * Define bitflags for the SPxPacketFlightPlan flags.
 */
#define	SPX_PACKET_FLIGHT_PLAN_FLAGS_REPEATING		0x01
#define	SPX_PACKET_FLIGHT_PLAN_FLAGS_DELETED		0x80


/*
 * Defines for screen record and replay.
 */

/* Maximum number of screens supported */
#define SPX_MAX_NUM_SCREENS   3

/* For backward compatibility. */
#define MAX_NUM_SCREENS (SPX_MAX_NUM_SCREENS)

/* Bitfields for screen bitmasks. */
#define SPX_SR_SCREEN0  (1 << 0)
#define SPX_SR_SCREEN1 	(1 << 1)
#define SPX_SR_SCREEN2 	(1 << 2)

/* Protocol version of file structure */
#define SPX_SR_PROTOCOL_VER 1

/* The magic numbers for the different block types in the recording file. */
#define SPX_SR_MASTER_MAGIC_START 	0xDACE1234
#define SPX_SR_MASTER_MAGIC_END 	0xDACE6789
#define SPX_SR_SCREEN_MAGIC_START 	0xDACEABCD
#define SPX_SR_SCREEN_MAGIC_PAUSE 	0xDACEAB00

/*
 * Defines for SPxPacketImage packet.
 */

/* Image types. */
#define SPX_PACKET_IMAGE_FILE_TYPE_RAW       1   /* Raw image data. */
#define SPX_PACKET_IMAGE_FILE_TYPE_BMP       2   /* Windows bitmap image. */
#define SPX_PACKET_IMAGE_FILE_TYPE_JPEG      3   /* JPEG image. */
#define SPX_PACKET_IMAGE_FILE_TYPE_PNG       4   /* PNG image. */

/*
 * Defines for SPxPacketRecord packet.
 */

/* Record types. */
#define SPX_PACKET_RECORD_TYPE_NONE                  0x00000000 /* Invalid record. */
#define SPX_PACKET_RECORD_TYPE_CHANNEL_ADD           0x00000001 /* Channel added. */
#define SPX_PACKET_RECORD_TYPE_CHANNEL_REMOVE        0x00000002 /* Channel removed. */
#define SPX_PACKET_RECORD_TYPE_CHANNEL_STATUS        0x00000004 /* Channel status. */
#define SPX_PACKET_RECORD_TYPE_STAMP_USER            0x00000008 /* User logged a stamp */
#define SPX_PACKET_RECORD_TYPE_STAMP_SYSTEM          0x00000010 /* System logged a stamp */

/*
 * Define values for SPxPacketAlarm status field.
 */
#define SPX_PACKET_ALARM_STATUS_ENTER           1   /* Track entered alarm area. */
#define SPX_PACKET_ALARM_STATUS_INSIDE          2   /* Track updated inside alarm area. */
#define SPX_PACKET_ALARM_STATUS_EXIT            3   /* Track exited alarm area. */
#define SPX_PACKET_ALARM_STATUS_COASTED         4   /* Track coasted inside or outside alarm area. */
#define SPX_PACKET_ALARM_STATUS_DELETED         5   /* Track deleted inside or outside alarm area. */
#define SPX_PACKET_ALARM_STATUS_OUTSIDE         6   /* Track updated outside alarm area. */

/*
 * Define values for SPxPacketChanges changeFlags bitfield.
 */
#define SPX_PACKET_CHANGES_DISCONTINUITY	0x00000001

/*
 * Define values for SPxPacketNavData flags bitfield.
 */
#define SPX_PACKET_NAVDATA_FLAG_LAT_LONG_VALID			0x0001
#define SPX_PACKET_NAVDATA_FLAG_ALTITUDE_VALID			0x0002
#define SPX_PACKET_NAVDATA_FLAG_SPEED_VALID			0x0004
#define SPX_PACKET_NAVDATA_FLAG_COURSE_VALID			0x0008
#define SPX_PACKET_NAVDATA_FLAG_HEADING_VALID			0x0010
#define SPX_PACKET_NAVDATA_FLAG_HEADING_CHANGE_RATE_VALID	0x0020
#define SPX_PACKET_NAVDATA_FLAG_TERRAIN_HEIGHT_VALID		0x0040

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

/*
 * Define the type of the A and B packet headers.
 *
 * NB: A must be smaller than B, and the magic number must be in the
 *	same location in each header.
 */
typedef struct SPxPacketHeaderA_tag {
    UINT16 magic;		/* SPX_PACKET_MAGIC_A */
    UINT8 packetType;		/* One of SPX_PACKET_TYPEA_... */
    UINT8 totalSizeBytes;	/* Total size of header and payload */
} SPxPacketHeaderA;

typedef struct SPxPacketHeaderB_tag {
    UINT16 magic;		/* SPX_PACKET_MAGIC_B */
    UINT16 packetType;		/* One of SPX_PACKET_TYPEB_... */
    UINT32 totalSizeBytes;	/* Total size of header and payload */
    UINT32 timeSecs;		/* Whole seconds since unix epoch */
    UINT32 timeUsecs;		/* Microseconds to be added to timeSecs */
} SPxPacketHeaderB;

/* Define a union of the above headers. */
typedef union SPxPacketHeaderUnion_tag {
    SPxPacketHeaderA a;
    SPxPacketHeaderB b;
    UINT32 magic;
} SPxPacketHeaderUnion;


/*
 * Define the type of the payload contents.
 */

typedef struct SPxPacketAsterix240SPF1_tag {
    UINT8 numTriggers;		/* Number of triggers represented by this spoke. */
    UCHAR packing;		/* Radar packing type
				 * - see SPX_RIB_PACKING_... in SPxRib.h.
				 */
    UINT8 pimFlags;		/* Pim flags. */
    UINT8 sourceCode;		/* Source-specific code. */

} SPxPacketAsterix240SPF1;

/* Radar config. */
typedef struct SPxPacketRadarConfig_tag {
    /* First word. */
    UCHAR channelIndex;		/* Channel within file, if more than one */
    UCHAR pad1;			/* Padding */
    UINT16 nominalLength;	/* Nominal number of samples per return */

    /* Second word */
    REAL32 startRange;		/* Start range in metres of first sample */

    /* Third word */
    REAL32 endRange;		/* End range in metres of last sample */
} SPxPacketRadarConfig;

/* Radar returns. */
typedef struct SPxPacketRadarReturn_tag {
    /* First word */
    UINT16 azimuth;		/* Azimuth in range 0 to 65535 */
    UCHAR channelIndex;		/* Channel within file, if more than one */
    UCHAR encodeMode;		/* One of SPX_RIB_PACKING_... types. */

    /* Second word */
    UCHAR scanMode;		/* One of the SPX_SCAN_MODE_... modes */
    UCHAR dataFlags;		/* Bitwise combination of SPX_RIB_DATA_FLAG_... */
    UINT16 timeIntervalUsecs;	/* Time between this and previous sync */

    /* Third word */
    UINT32 totalLength;		/* Total payload size, including this struct */

    /* Fourth word */
    UINT16 rawLength;		/* Size of uncompressed radar data (bytes) */
    UINT16 encLength;		/* Size of compressed radar data (bytes) */

    /* Encoded data follows immediately here. */
} SPxPacketRadarReturn;

/* This structure can be added AFTER the radar return data, included
 * in the packet header B size, but NOT the totalLength size in the
 * radar return packet.
 * This allows backwards compatibility with old software and old files.
 */
typedef struct SPxPacketRadarReturnExtra_tag {
    UINT8 sizeBytes;		/* Size of this structure in bytes. */
    UCHAR numTriggers;		/* Number of triggers for this return. */
    UINT8 pimFlags;		/* Bitwise combination of SPX_PIM_FLAGS_... */
    UINT8 reserved1;

    UINT16 heading;		/* Platform heading (0..65536 = 0..360degs) */
    UINT16 reserved2;

} SPxPacketRadarReturnExtra;

/* Window configuration (not all fields may be applicable) */
typedef struct SPxPacketWindowConfig_tag {
    /* First word. */
    UINT16 protocolVersion;	/* Protocol version, currently 1 */
    UCHAR windowID;		/* ID of window this config belongs to */
    UCHAR pad1;			/* Padding, set to 0 */

    /* Second word. */
    INT16 screenX;		/* Position of window on screen */
    INT16 screenY;		/* Position of window on screen */

    /* Third word. */
    INT16 screenW;		/* Size of window on screen */
    INT16 screenH;		/* Size of window on screen */

    /* Fourth to ninth words, if applicable. */
    REAL32 viewX;		/* World coordinate at centre of window */
    REAL32 viewY;		/* World coordinate at centre of window */
    REAL32 viewW;		/* Width represented by window */
    REAL32 viewH;		/* Width represented by window */
    REAL32 sensorX;		/* Sensor position in world coords */
    REAL32 sensorY;		/* Sensor position in world coords */

    /* Tenth word, if applicable. */
    UCHAR state;		/* Window state (may be SPX_SC_STATE...) */
    UCHAR fadeType;		/* Fading type (SPX_RADAR_FADE_...) */
    UINT16 fadeRate;		/* Fading rate (revs, or 1/100ths of secs) */

    /* Colour information. */
    UCHAR red;			/* Red component of colour */
    UCHAR green;		/* Green component of colour */
    UCHAR blue;			/* Blue component of colour */
    UCHAR bright;		/* Brightness of colour */

    /* Source information. */
    UINT32 sourceID;
    UINT32 sourceArg1;
    UINT32 sourceArg2;

    /* Sweep line information. */
    UINT32 sweepLineRGB;	/* Sweep line colour/intensity */
    UCHAR sweepLineType;	/* SPxScSweepLineType_t */
    UCHAR sweepLineColMode;	/* SPxScSweepLineColMode_t */
    UINT16 reserved3;		/* Set to zero */

    /* Other information. */
    UINT32 reserved4;		/* Set to zero */
} SPxPacketWindowConfig;

/* Window content (one or more patches). */
typedef struct SPxPacketWindowPatches_tag {
    /* First word. */
    UINT16 protocolVersion;	/* Protocol version, currently 1 */
    UINT16 numPatches;		/* Number of patches in this message */

    /* Second word. */
    UCHAR windowID;		/* ID of window these patches belong to */
    UCHAR format;		/* Window format (0=1bpp, 1=2bpp, ...) */
    UINT16 sequenceNumber;	/* Incrementing and wrapping counter */

    /* Third word. */
    UCHAR encoding;		/* Encoding method (0=raw) */
    UCHAR pad1;			/* Padding, set to 0 */
    UCHAR pad2;			/* Padding, set to 0 */
    UCHAR pad3;			/* Padding, set to 0 */

    /* Patch data follows immediately here. */
} SPxPacketWindowPatches;

/* Parameter list (one or more generic parameters). */
typedef struct SPxPacketParamList_tag {
    /* First word. */
    UCHAR windowID;		/* ID of object these params belong to */
    UCHAR numParams;		/* Number of parameters that follow */
    UINT16 reserved1;		/* Padding, set to 0 */

    /* 'numParams' lots of SPxPacketParam structures follow here. */
} SPxPacketParamList;

typedef struct SPxPacketParam_tag {
     UINT16 paramID;
     UINT16 reserved;		/* Set to zero */
     UINT32 paramArg1;
     UINT32 paramArg2;
     UINT32 paramArg3;
} SPxPacketParam;

/* Acknowledgements. */
typedef struct SPxPacketAck_tag {
    UCHAR id;			/* ID of object being ack'ed, e.g. window */
    UCHAR reserved1;		/* Padding, set to zero */
    UINT16 seqNum;		/* Sequence number being acknowledged */
    UINT32 reserved2;		/* Reserved, set to zero */
} SPxPacketAck;

/* PIM data. */
typedef struct SPxPacketPimData_tag {
    UINT8 reserved00;		/* Reserved, set to 0 */
    UINT8 packingMode;		/* One of SPX_RIB_PACKING_... */
    UINT16 reserved02;		/* Reserved, set to 0 */

    UINT16 numSamples;		/* Number of range samples */
    UINT16 numAzimuths;		/* Number of azimuths */

    REAL32 startRange;		/* Start range in metres (first sample) */
    REAL32 endRange;		/* End range in metres (last sample) */

    UINT32 reserved16;		/* Reserved, set to 0 */
    UINT32 reserved20;		/* Reserved, set to 0 */
    UINT32 reserved24;		/* Reserved, set to 0 */
    UINT32 reserved28;		/* Reserved, set to 0 */
    UINT32 reserved32;		/* Reserved, set to 0 */
    UINT32 reserved36;		/* Reserved, set to 0 */
    UINT32 reserved40;		/* Reserved, set to 0 */
    UINT32 reserved44;		/* Reserved, set to 0 */
    UINT32 reserved48;		/* Reserved, set to 0 */
    UINT32 reserved52;		/* Reserved, set to 0 */
    UINT32 reserved56;		/* Reserved, set to 0 */
    UINT32 reserved60;		/* Reserved, set to 0 */

    /* NB: Radar data follows this, tightly packed one azimuth after another
     * with total size (assuming no compression) equal to:
     *	(numSamples * numAzimuths * SPxGetPackingBytesPerSample(packing))
     * In other words, the first sample is the shortest range of azimuth 0,
     * then the next sample in range and so on up to the last sample for
     * azimuth 0, then the same for the next azimuth and so on for a whole
     * revolution of data.
     */
} SPxPacketPimData;


/* Plot reports - Java and .NET may require update if this structure changed. */
typedef struct SPxPacketPlot_tag {
    UINT8 senderID;		/* Sender identification */
    UINT8 flags;		/* Quality flags, SPX_BLOB_FLAG_... */
    UINT8 posnFlags;		/* Position flags, SPX_PACKET_PLOT_POSNFLAG.. */
    UINT8 reserved03;		/* Reserved, set to zero */
    UINT32 reserved04;		/* Reserved, set to zero */

    /* Position */
    REAL32 rangeMetres;		/* Polar position */
    REAL32 azimuthDegrees;	/* Polar position */
    UINT32 reserved16;		/* Reserved, set to zero */
    UINT32 reserved20;		/* Reserved, set to zero */

    /* Bounding box. */
    REAL32 startRangeMetres;
    REAL32 endRangeMetres;
    REAL32 startAzimuthDegrees;
    REAL32 endAzimuthDegrees;

    /* Quality information. */
    UINT32 weight;		/* Typically number of samples used in plot */
    UINT32 strength;		/* Typically sum of values of samples used */
    UINT32 numComponents;	/* How many small plots merged to make this */
    UINT32 reserved52;		/* Reserved, set to zero */
    UINT32 reserved56;		/* Reserved, set to zero */
    UINT32 reserved60;		/* Reserved, set to zero */

    /* Enhanced information if available from sensor.
     * For the iff fields, see the comments in SPxPacketTrackSecondary
     * below for information about packing and octal printing etc.
     */
    UINT32 enhancedFlags;	/* Which of the following fields are valid */
    REAL32 radialSpeedMps;	/* Positive towards sensor, negative away */
    UINT16 iffMode3A;		/* Mode 3/A code, or zero if not known */
    UINT16 iffModeC;		/* Mode C code, or zero if not known */
    UINT16 iffMode2;		/* Mode 2 code, or zero if not known. */
    UINT16 iff12BitMode1;	/* 12-bit Mode 1 code, or zero if not known. */
    UINT8 iffMode1;		/* 6-bit Mode 1 code, or zero if not known. */
    UINT8 iffFlags;		/* SPX_PACKET_TRACK_SECONDARY_IFF_FLAGS_... */
    UINT8 iffExtFlags;		/* SPX_PACKET_TRACK_SECONDARY_IFF_EXT_FLAGS_.*/
    UINT8 reserved83;		/* Reserved, set to zero */
    REAL32 heightMetres;	/* Height in metres */
    UINT32 reserved88;		/* Reserved, set to zero */
    UINT32 reserved92;		/* Reserved, set to zero */
    UINT32 reserved96;		/* Reserved, set to zero */
} SPxPacketPlot;

/* Plot status reports. */
#define	SPX_PACKET_PLOT_STATUS_FLAGS_ALTITUDE	0x02
typedef struct SPxPacketPlotStatus_tag {

    UINT8 senderID;		/* Sender identification */
    UINT8 sector;		/* Sector number (0-31), 0=North crossing */
    UINT8 flags;		/* SPX_PACKET_PLOT_STATUS_FLAGS_... */
    UINT8 reserved03;		/* Reserved, set to zero */

    REAL32 updatePeriod;	/* Update period in use (seconds) */
    REAL32 sensorLatDegs;	/* Latitude of sensor, +ve north */
    REAL32 sensorLongDegs;	/* Longitude of sensor, +ve east */

    /* Sensor altitude, if flags & SPX_PACKET_PLOT_STATUS_FLAGS_ALTITUDE. */
    REAL32 sensorAltitudeMetres;/* Altitude of sensor (above MSL) */

    UINT32 reserved20;		/* Reserved, set to zero */
    UINT32 reserved24;		/* Reserved, set to zero */
    UINT32 reserved28;		/* Reserved, set to zero */

} SPxPacketPlotStatus;


/*
 * Fusion information, sometimes included in SPxPacketTrackExtended rpts.
 * Update SPxRemoteServer byte-swapping and un/packing if this changes.
 */

#define SPX_MAX_NUM_TRACK_IDS	(8)

typedef struct SPxPacketTrackFusion_tag {
    /* Bitmask of which sensor types are supporting the track. */
    UINT32 sensorTypes;		/* Combination of SPX_PACKET_TRACK_SENSOR_.. */

    /* Bitmap of which sensors are supporting the track. LSB is
     * sensor 0, etc.  Up to 32 sensors can be included.
     */
    UINT32 sensors;

    /* Array of track IDs used for this track/report, to allow a receiver to
     * interact with the originating server to get or set information for,
     * or delete, tracks. The number of entries in this array corresponds 
     * to the number of bits set in 'sensors'; trackID[0] corresponds to the 
     * least significant bit set, trackID[1] the next bit set and so on. 
     * Note that track IDs are only provided for the first SPX_MAX_NUM_TRACK_IDS 
     * bits that are set in sensors. It is important to note that the bit position 
     * in sensors is NOT the index into trackID[]. The index is the number of 
     * preceding less significant bits that are set.
     *
     * E.g. sensors = 0x00000014, tracksID = {123, 456, 0, 0, 0, 0, 0, 0}.
     * Sensor2 trackID is 123, Sensor4 trackID is 456.
     */
    UINT32 trackID[SPX_MAX_NUM_TRACK_IDS];

    /* The maximum separation in metres between any two observations
     * contributing to this track.
     */
    REAL32 maxRangeDiffMetres;

    /* Flags indicating properties - see SPX_PACKET_TRACK_FUSE_FLAG_.. */
    UINT32 flags;

    /* Differences from PSR to SSR positions. */
    REAL32 psrToSSRRangeMetres;	/* Metres from PSR range to SSR range */
    REAL32 psrToSSRAziDegs;	/* Degress from PSR azimuth to SSR azimuth */

    /* Differences from fused position to individual sensors, using the
     * same array mapping as the trackID field above.
     * Distances are in X/Y directions, positive north and east, in units
     * of tenths of metres initially, although this may change in future
     * releases with an appropriate flag to indicate the difference.
     */
    INT16 xOffsets[SPX_MAX_NUM_TRACK_IDS];
    INT16 yOffsets[SPX_MAX_NUM_TRACK_IDS];

    /* Reserved for future use. Set to zero. */
    UINT32 reserved[10];
} SPxPacketTrackFusion;

/*
 * Secondary track info, sometimes included in SPxPacketTrackExtended rpts.
 * Update SPxRemoteServer and SPxFuseNetReport byte-swapping if this changes.
 */
typedef struct SPxPacketTrackSecondary_tag {
    /* Indication of which sensor type is giving this information. */
    UINT32 sensorType;		/* One of SPX_PACKET_TRACK_SENSOR_... */

    /* Unique ID. */
    UINT32 uniqueID;		/* E.g. MMSI, AA, or 0 if not available */

    /* Name/label (up to 20 characters plus terminating NULL). */
    char name[21];		/* Callsign, vessel name etc. */

    /* Target information. */
    UINT8 targetType;		/* E.g. Ship cargo type, or ADS-B cat+value */
    UINT8 targetStatus;		/* E.g. AIS NavStatus, ADS-B capabilities */
    UINT8 targetFlags;		/* Misc flags, e.g. IFF validities */

    /* IFF codes, stored as octal values with three bits per digit, so the
     * 4-digit codes use the least significant 12-bits and the 2-digit code
     * uses the least significant 6-bits (even though bit 2 is never set).
     * They should be printed as octal values in order to look correct to
     * operators.
     */
    UINT16 iffMode3A;		/* Mode 3/A code, or zero if not known */
    UINT16 iffMode2;		/* Mode 2 code, or zero if not known. */
    UINT8 iffMode1;		/* 6-bit Mode 1 code, or zero if not known. */
    UINT8 iffFlags;		/* SPX_PACKET_TRACK_SECONDARY_IFF_FLAGS_... */
    UINT16 iff12BitMode1;	/* 12-bit Mode 1 code, or zero if not known. */

    /* Target dimensions, if available. */
    UINT16 targetWidth;		/* Target width in metres */
    UINT16 targetLength;	/* Target length in metres */
    REAL32 targetDraught;	/* Target draught in metres */
    REAL32 targetHeading;	/* Target heading in degrees */

    /* IFF confidences. */
    UINT16 iffMode3AConfidence;	/* Mode 3/A code confidence bits in bottom 12-bits. */
    UINT16 iffMode2Confidence;	/* Mode 2 code confidence bits in bottom 12-bits. */
    UINT8 iffMode1Confidence;	/* Mode 1 code confidence bits in bottom 6-bits. */
    UINT8 iffConfidenceFlags;   /* SPX_PACKET_TRACK_SECONDARY_IFF_CONF_FLAGS_... */

    /* Communications capability. */
    UINT8 commsCapability;	/* Communications capability. */

    /* Extended IFF flags. */
    UINT8 iffExtFlags;		/* SPX_PACKET_TRACK_SECONDARY_IFF_EXT_FLAGS_..*/

    UINT16 iffModeC;		/* 12-bit Mode C code, or zero if not known. */
    UINT16 iffModeCConfidence;	/* Mode-C code confidence bits int bottom 12-bits. */

    /* Flags to indicate which of the later fields are present. */
    UINT8 fieldFlags;		/* SPX_PACKET_TRACK_SECONDARY_FIELD_FLAGS_.. */
    UINT8 statusFlags;		/* SPX_PACKET_TRACK_SECONDARY_STATUS_FLAGS_..*/
    UINT16 reserved1b;

    /* Altitudes from various sources, if fieldFlags says they are set. */
    REAL32 altitudeRadarMetres;	/* Altitude derived from 3D Radar */
    REAL32 altitudeFLMetres;	/* from Flight Level (Mode C, Barometric) */
    REAL32 altitudeGeomMetres;	/* from Geometric source (e.g. GPS) */

    /* Vertical rates, if SPX_PACKET_TRACK_SECONDARY_FIELD_FLAGS_VR_... */
    REAL32 vertRateGeom;	/* Geometric vertical rate, m/s */
    REAL32 vertRateBaro;	/* Barometric vertical rate, m/s */

    /* Target dimensions (continued). */
    REAL32 targetHeight;	/* Target height extent (not altitude) in metres. */

    /* Reserved for future use. Set to zero. */
    UINT32 reserved2[9];
} SPxPacketTrackSecondary;

/*
 * RST track info, sometimes included in SPxPacketTrackExtended rpts.
 * Update SPxRemoteServer byte-swapping and spx_track_database.js if this
 * structure changes.
 */
typedef struct SPxPacketTrackRst_tag {
    /* General information */
    UINT8 model;		/* RST model */
    UINT8 isExtendedCoast;	/* Extended coasting? */
    UINT16 reserved2;

    /* Gains */
    REAL32 rangeGain;		/* Gain in range. */
    REAL32 azimuthGain;		/* Gain in azimuth. */
    REAL32 heightGain;		/* Gain in height. */
    REAL32 speedXGain;		/* Gain in speed X direction. */
    REAL32 speedYGain;		/* Gain in speed Y direction. */
    REAL32 speedHGain;		/* Gain in speed H direction. */

    /* Confidence. */
    REAL32 confidence;		/* Confidence score */

    /* Measured vertical rate. */
    REAL32 vertRate;		/* Vertical rate in m/s (+ve up). */

    /* Reserved for future use. Set to zero. */
    UINT32 reserved3[15];
} SPxPacketTrackRst;

/*
 * Extra MHT track info, sometimes included in SPxPacketTrackExtended rpts.
 * Update SPxRemoteServer byte-swapping and spx_track_database.js if this
 * structure changes.
 */
typedef struct SPxPacketTrackMht_tag {
    REAL32 rangeGain;
    REAL32 azimuthGain;
    REAL32 rangeRateGain;
    REAL32 azimuthRateGain;
    REAL32 meanWeight;
    UINT32 reserved2[11];

} SPxPacketTrackMht;

/*
 * Threat info, sometimes included in SPxPacketTrackExtended rpts.
 * Update SPxRemoteServer and SPxFuseNetReport byte-swapping if this changes.
 */
#define SPX_MAX_THREAT_NAME_LENGTH (31)
typedef struct SPxPacketTrackThreat_tag
{
    /* Threat name ending with '\0'.  32-bytes, so 8 words. */
    char name[SPX_MAX_THREAT_NAME_LENGTH + 1];

    /* Type and level. */
    UINT8 type;			/* One of SPX_PACKET_THREAT_TYPE... */
    UINT8 level;		/* One of SPX_PACKET_THREAT_LEVEL... */
    UINT8 flags;		/* Any of SPX_PACKET_THREAT_FLAG... */
    UINT8 reserved09;

    /* ID of app/operator that classified the threat. */
    UINT32 setterID;

    /* Timestamp of when app/operator changed the threat values. */
    UINT32 lastChangedSecs;	/* Seconds since unix epoch */

    /* Reserved fields for future use. */
    UINT32 reserved12;		/* Reserved, set to zero */
    UINT32 reserved13;		/* Reserved, set to zero */
    UINT32 reserved14;		/* Reserved, set to zero */
    UINT32 reserved15;		/* Reserved, set to zero */
    UINT32 reserved16;		/* Reserved, set to zero */
} SPxPacketTrackThreat;

/*
 * MIL-2525-D symbol info, sometimes included in SPxPacketTrackExtended rpts.
 * Update SPxRemoteServer and SPxFuseNetReport byte-swapping if this changes.
 */
typedef struct SPxPacketTrackMIL2525D_tag
{
    UINT8 identity;	/* MIL-2525-D identity. */
    UINT8 set;		/* MIL-2525-D symbol set. */
    UINT8 entity;	/* MIL-2525-D entity. */
    UINT8 entityType;	/* MIL-2525-D entity type. */
    UINT8 entitySubtype;/* MIL-2525-D entity sub-type. */
    UINT8 modifier1;	/* MIL-2525-D modifier 1. */
    UINT8 modifier2;	/* MIL-2525-D modifier 2. */
    UINT8 reserved;

} SPxPacketTrackMIL2525D;

/* Minimal track reports - Java and .NET may require update if this structure
 * is changed.
 */
typedef struct SPxPacketTrackMinimal_tag {
    UINT32 id;			/* Track ID (public) */
    UINT8 senderID;		/* Sender identification */
    UINT8 status;		/* Track status (SPX_PACKET_TRACK_STATUS_...) */
    UINT8 numCoasts;		/* Number of consecutive coasts */
    UINT8 id_ttm;		/* TTM ID */
    REAL32 rangeMetres;		/* Tracked Range */
    REAL32 azimuthDegrees;	/* Tracked Azimuth */
    REAL32 speedMps;		/* Speed */
    REAL32 courseDegrees;	/* Course */
    REAL32 sizeMetres;		/* Smoothed size in metres */
    REAL32 sizeDegrees;		/* Smoothed size in degrees */
    UINT32 weight;		/* Weight of target (number of samples) */
    UINT32 strength;		/* Strength of target (sum of values) */
    UINT8 flags;		/* Target flags (SPX_PACKET_TRACK_FLAG_...) */
    UINT8 validity;		/* One of SPX_PACKET_TRACK_INVALID_... */
    UINT8 uniTrackType;		/* Unified track type - see SPxUniTrack::TrackType_t */
    UINT8 sourceIndex;		/* Track source index */
    UINT32 reserved3;		/* Reserved, set to zero */
    UINT32 reserved4;		/* Reserved, set to zero */
    UINT32 reserved5;		/* Reserved, set to zero */
} SPxPacketTrackMinimal;

/* Normal track reports - Java and .NET may require update if this structure
 * is changed.
 */
typedef struct SPxPacketTrackNormal_tag {
    SPxPacketTrackMinimal min;	/* Everything in the minimal report first */
    REAL32 xMetres;		/* Tracked cartesian position */
    REAL32 yMetres;		/* Tracked cartesian position */
    REAL32 measRange;		/* Measured polar position */
    REAL32 measAzimuth;		/* Measured polar position */
    REAL32 measSizeMetres;	/* Measured size in range */
    REAL32 measSizeDegrees;	/* Measured size in azimuth */
    REAL32 sdRange;		/* SD of range measurements */
    REAL32 sdAzimuth;		/* SD of azimuth measurements */
    REAL32 sdRangeSize;		/* SD of size-in-range measurements */
    REAL32 sdAzimuthSize;	/* SD of size-in-azimuth measurements */
    UINT16 numDetectionsPQ;	/* Recent hit count, P (LSB) out of Q (MSB) */
    UINT16 trackClass;		/* Track class */
    UINT8 trackClassType;	/* Type of track class (SPX_PACKET_TRACK_CLASS_TYPE_...) */
    UINT8 reserved1;		/* Reserved, set to zero */
    UINT16 reserved2;		/* Reserved, set to zero */
    UINT32 reserved3;		/* Reserved, set to zero */
    UINT32 reserved4;		/* Reserved, set to zero */
} SPxPacketTrackNormal;

/* Extended track reports.
 *
 * - Java and .NET may require update if this structure changed.
 *
 * NB: This is a variable-sized structure on the network, since most of
 * the fields are optional.  The 'extMask' field contains bit flags to
 * indicate which of the subsequent fields are present on the network.
 * If present, they will be in the order given here.
 *
 * Therefore, only use sizeof() this structure with extreme care.
 *
 * Furthermore, note that this structure is subject to change without
 * notice, including the size of the sensorSpecificData[] array.  Use
 * the sensorDataBytes field to determine how many bytes are actually
 * valid in the array, with sizeof(sensorSpecificData) indicating the
 * size of the array itself.
 *
 * SPxRemoteServer::TrackPackNet() and TrackUnpackNet() should be maintained
 * in conjunction with this structure.
 */
typedef struct SPxPacketTrackExtended_tag {
    /* Everything in the normal report first */
    SPxPacketTrackNormal norm;

    /* Size (in bytes) of the entire message on the net (including the
     * 'norm' component, this size, the mask and the variable fields
     * that are present, but excluding those that are missing on the net
     * (and excluding the SPxPacketHeader).
     */
    UINT32 netSize;

    /* Bitmask of which of the following fields are present. */
    UINT32 extMask;

    /* Optional fields, presence on network and validity in client structure
     * controlled by bit flags in 'extMask' field above.
     */
    /* Radial velocity (extMask & SPX_PACKET_TRACK_EXT_RADIALSPEED). */
    REAL32 radialSpeedMps;	/* Positive towards sensor, negative away */
    REAL32 radialSpeedSD;	/* SD of radialSpeedMps */

    /* Age of track (extMask & SPX_PACKET_TRACK_EXT_AGE). */
    UINT32 age;			/* Age in scans (revolutions, or updates) */

    /* Lat/long (extMask & SPX_PACKET_TRACK_EXT_LATLONG). */
    REAL32 latDegs;		/* Tracked Latitude, positive north */
    REAL32 longDegs;		/* Tracked Longitude, positive east */

    /* Message time (extMask & SPX_PACKET_TRACK_EXT_MSGTIME).
     * This is the time the message was sent, not the time of the report.
     */
    UINT32 msgTimeSecs;		/* Whole seconds since unix epoch */
    UINT32 msgTimeUsecs;	/* Microseconds to be added to msgTimeSecs */

    /* Sensor-specific data (extMask & SPX_PACKET_TRACK_EXT_SENSORDATA). */
    UINT32 sensorDataBytes;	/* Number of bytes in sensorSpecificData */
    UINT8 sensorSpecificData[128]; /* Sensor-specific data */

    /* Altitude (extMask & SPX_PACKET_TRACK_EXT_ALTITUDE). */
    REAL32 altitudeMetres;	/* Altitude in metres */

    /* Fusion information (extMask & SPX_PACKET_TRACK_EXT_FUSION). */
    SPxPacketTrackFusion fusion;	/* Fusion information */

    /* Secondary information (extMask & SPX_PACKET_TRACK_EXT_SECONDARY). */
    SPxPacketTrackSecondary secondary;	/* IFF/AIS/ADSB information etc. */

    /* Lat/long (extMask & SPX_PACKET_TRACK_EXT_LATLONG_MEAS). */
    REAL32 latDegsMeas;		/* Measured latitude, positive north */
    REAL32 longDegsMeas;	/* Measured longitude, positive east */

    /* Search gate (extMask & SPX_PACKET_TRACK_EXT_GATE). */
    REAL32 gateStartRangeMetres;	/* Start range in metres */
    REAL32 gateEndRangeMetres;		/* End range in metres */
    REAL32 gateStartAziDegs;		/* Start azimuth in degrees */
    REAL32 gateEndAziDegs;		/* End azimuth in degrees */

    /* CPA/TCPA (extMask & SPX_PACKET_TRACK_EXT_CPA). */
    REAL32 cpaMetres;		/* Cloest point of approach. */
    REAL32 tcpaSecs;		/* Time to closest point of approach. */

    /* Threat info (extMask & SPX_PACKET_TRACK_EXT_THREAT). */
    SPxPacketTrackThreat threat;	/* Threat information. */

    /* Description (extMask & SPX_PACKET_TRACK_EXT_DESCRIPTION). */
    UINT16 descNetSize;			/* Byte length of description on net. */
    char description[62];		/* Track description. */

    /* Target accel and turn rate (extMask & SPX_PACKET_TRACK_EXT_ACCEL) */
    REAL32 accelMps;
    REAL32 turnRateDps;

    /* Target motion status (extMask & SPX_PACKET_TRACK_EXT_MOTION_STATUS) */
    UINT16 motionStatus;
    UINT16 padding; /* Not used. */

    /* Event time (extMask & SPX_PACKET_TRACK_EXT_EVENT_TIME).
     * Time of the event that this report applies to.
     */
    UINT32 eventTimeSecs;	/* Whole seconds since unix epoch */
    UINT32 eventTimeUsecs;	/* Microseconds to be added to eventTimeSecs */

    /* Velocity SD's (extMask & SPX_PACKET_TRACK_EXT_VELOCITY_SD). */
    REAL32 speedSD;		/* SD of speed */
    REAL32 courseSD;		/* SD of course */

    /* IJMS code (extMask & SPX_PACKET_TRACK_EXT_IJMS). */
    char IJMSCode[4];		/* IJMS code. */

    /* MIL-2525-D symbol (extMask & SPX_PACKET_TRACK_EXT_MIL2525D). */
    SPxPacketTrackMIL2525D mil2525D;

    /* Altitude SD's (extMask & SPX_PACKET_TRACK_EXT_ALTITUDE_SD). */
    REAL32 altitudeSD;		/* SD of altitude */

    /* RST specific fields (extMask & SPX_PACKET_TRACK_EXT_RST). */
    SPxPacketTrackRst rst;	/* RST information */

    /* Extra MHT fields (extMask & SPX_PACKET_TRACK_EXT_MHT)*/
    SPxPacketTrackMht mht;	/* MHT information */

    /* More fields may be added here in the future. */
} SPxPacketTrackExtended;


/* Tracker status - Java and .NET may require update if this structure changed. */
#define	SPX_PACKET_TRACKER_STATUS_FLAGS_LATLONG	0x01
#define SPX_PACKET_TRACKER_STATUS_FLAGS_ALTITUDE 0x02
typedef struct SPxPacketTrackerStatus_tag {
    /* Reporting control. */
    UINT32 reportType;		/* 0 or SPX_PACKET_TYPEB_TRACK_MIN/NORM/...*/
    UINT32 reportExtFlags;	/* Bitmask for content of extended reports */

    /* Statistics. */
    UINT16 numTracks;		/* Number of tracks */
    UINT16 numClusters;		/* Number of clusters */
    UINT16 numHypotheses;	/* Number of hypotheses */
    UINT16 numTrackLinks;	/* Number of track links */
    UINT32 numMeasAccepted;	/* Number of measurements accepted */
    UINT32 numMeasRejected;	/* Number of measurements rejected */

    /* Status fields. */
    REAL32 updatePeriod;	/* Update period in use (seconds) */
    UINT16 overloadFlags;	/* Bitmask of SPX_MHT_STATUS_OVERLOAD... */
    UINT8 senderID;		/* Sender identification */
    UINT8 flags;		/* SPX_PACKET_TRACKER_STATUS_FLAGS_... */

    /* Server version, encoded as 0x00xxyyzz for version Vxx.yy.zz */
    UINT32 serverVersion;	/* Server version, e.g. Vx.y = 0x00xxyy00 */

    /* Sensor lat/long, if flags & SPX_PACKET_TRACKER_STATUS_FLAGS_LATLONG. */
    REAL32 sensorLatDegs;	/* Latitude of sensor, +ve north */
    REAL32 sensorLongDegs;	/* Longitude of sensor, +ve east */

    /* Status of input sensors; currently used only by fusion engine. */
    UINT32 sourceStatus;

    /* Sensor altitude, if flags & SPX_PACKET_TRACKER_STATUS_FLAGS_ALTITUDE. */
    REAL32 sensorAltitudeMetres;/* Altitude of sensor (above MSL). */

    UINT8 uniTrackType;		/* Unified track type - see SPxUniTrack::TrackType_t */
    UINT8 sourceIndex;		/* Tracker source index */

    /* Reserved fields. */
    UINT16 reserved54;
    UINT32 reserved56;
    UINT32 reserved60;
} SPxPacketTrackerStatus_t;

/* Network snooped packet (e.g. recorded TCP or UDP stream). */
typedef struct SPxPacketNet_tag {
    /* First word. */
    UCHAR streamIndex;		/* Stream within file, if more than one */
    UCHAR packetType;		/* SPX_PACKET_NET_TYPE_... */
    UCHAR packetFormat;		/* SPX_PACKET_NET_FMT_... */
    UCHAR pad1;			/* Padding */

    /* Second word */
    INT32 payloadSize;		/* Size when uncompressed, 0=cfg, -1=err */

    /* Third word */
    UINT32 senderAddr;		/* Sender IP address, if known */

    /* Fourth word */
    UINT32 streamAddr;		/* Server or mcast IP address */

    /* Fifth word */
    UINT16 senderPort;		/* Sender port number, if known */
    UINT16 streamPort;		/* Server or mcast port number */

    /* Sixth to Eighth words */
    UINT32 pad6;		/* Padding */
    UINT32 pad7;		/* Padding */
    UINT32 pad8;		/* Padding */
} SPxPacketNet;

/*
 * Table-of-contents packet and entries (e.g. for recording files).
 * Use the sizeof() this structure with extreme care because it does
 * NOT include the actual table-of-contents entries which normally
 * follow it in a variable-sized array.
 */
typedef struct SPxPacketTOCEntry_tag {
    UINT32 secs;		/* Timestamp of entry */
    UINT32 reserved;
    UINT64 offset;		/* Offset to entry */
} SPxPacketTOCEntry;

typedef struct SPxPacketTOCHdr_tag {
    /* Times covered by file */
    UINT32 startTimeSecs;	/* Time of first packet */
    UINT32 endTimeSecs;		/* Time of last packet */

    /* Additional information. */
    UINT8 numChannels;		/* Number of channels in the file. */ 

    /* Other fields */
    UINT8 reserved3a;
    UINT16 reserved3b;

    UINT32 version;		/* Software version */

    /* Other fields */
    UINT32 reserved5;
    UINT32 reserved6;
    UINT32 reserved7;
    UINT32 reserved8;

    /* Offsets to particular fields, if present. */
    UINT64 offsetToNextName;	/* Next-name packet if present */
    UINT64 offsetReserved2;	/* Reserved */
    UINT64 offsetReserved3;	/* Reserved */
    UINT64 offsetReserved4;	/* Reserved */

    /* Table of contents. */
    UINT32 tocSize;		/* Number of spaces in toc array */
    UINT32 tocUsed;		/* Number of entries used in toc array */
    UINT32 tocReserved3;	/* Reserved */
    UINT32 tocReserved4;	/* Reserved */

    /* NB: array of SPxPacketTOCEntry[tocSize] follows this, not included
     * in sizeof(structure).
     */
} SPxPacketTOCHdr;	/* NB: Use sizeof() with extreme care */

/*
 * Image Sender Manager Status for describing the configuration of
 * the streams of image data that an image sender manager is 
 * currently distributing.
 */
#define SPX_IMG_SND_STREAM_NAME_LENGTH (31)

typedef struct SPxPacketImgSndStreamConfig_tag
{

    UINT16 streamID;	    /* ID of the stream within the manager */
    UINT16 port;	    /* Port this stream is distributing on. */
    UINT32 address;	    /* IP address this stream is distributing to. */
    UINT32 flags;	    /* Status flags. */

    UINT32 previewAddress;  /* IP address this stream is distributing a preview to - added in V2. */
    UINT16 previewPort;	    /* Port this stream is distributing a preview to - added in V2. */
    UINT16 reserved5;	    /* Reserved. */
    UINT32 reserved6;	    /* Reserved. */

    /* Size of image this stream is distributing. */
    UINT16 width;	    /* Width in pixels. */
    UINT16 height;	    /* Height in pixels. */

    REAL32 viewX;	    /* Window centre X co-ordinate in metres
			     * (relative to radar centre). */
    REAL32 viewY;	    /* Window centre Y co-ordinate in metres
			     * (relative to radar centre). */
    REAL32 viewW;	    /* Width of area distributed in metres. */
    REAL32 viewH;	    /* Height of area distributed in metres. */
    REAL32 rangeEnd;	    /* Maximum range of radar in metres. */

    UINT32 reserved13;	    /* Reserved. */
    UINT32 reserved14;	    /* Reserved. */
    UINT32 reserved15;	    /* Reserved. */

    /* The name will be NULL-terminated if less than the maximum name
     * size, but not otherwise. */
    char  name[SPX_IMG_SND_STREAM_NAME_LENGTH + 1]; /* Name of the stream. */

    UINT32 reserved24;	    /* Reserved. */
    UINT32 reserved25;	    /* Reserved. */
    UINT32 reserved26;	    /* Reserved. */
} SPxPacketImgSndStreamConfig;

typedef struct SPxPacketImgSndMngrStatus_tag
{
    UINT8 version;	    /* Version of this message - currently 1. */
    UINT8 numStreams;	    /* The number of image streams. */
    
    UINT16 managerID;	    /* ID of manager. */

    UINT32 reserved2;	    /* Reserved. */
    UINT32 reserved3;	    /* Reserved. */
    UINT32 reserved4;	    /* Reserved. */
    UINT32 reserved5;	    /* Reserved. */
    UINT32 reserved6;	    /* Reserved. */
    UINT32 reserved7;	    /* Reserved. */
    UINT32 reserved8;	    /* Reserved. */

    /* NB: array of SPxPacketImgSndStreamConfig[numStreams] follows
     * this, not included in sizeof(structure).
     */
} SPxPacketImgSndMngrStatus;	/* NB: Use sizeof() with extreme care */

/* SPxPacketImageChunk
 *
 * Any fields added should be handled by SPxAVSrcFile::ByteSwapImageChunkHdr().
 */
typedef struct SPxPacketImageChunk_tag
{
    UINT16 managerID;	    /* ID of sender manager. */
    UINT16 streamID;	    /* ID of stream. */

    UINT32 infoAddress;	    /* IP address to send information messages to. */
    UINT16 infoPort;	    /* Port to send information messages to. */
    UINT16 id;		    /* ID of data. */

    UINT16 frameRateNum;    /* Frame rate numerator - video/images only. */
    UINT16 frameRateDen;    /* Frame rate denominator - video/images only. */

    UINT8  compression;	    /* Type of compression used - See SPX_IMG_STREAM_COMPRESSION_... */
    UINT8  bitDepth;	    /* Bit depth - 8, 16, 32 etc (1 means 32) */
    UINT8  format;	    /* Video/Audio only - format eg, YUV, RGB, PCM - see SPX_AV_FORMAT_... */
    UINT8  flags;	    /* Flags for video/audio - see SPX_AV_FLAGS_... */

    UINT32 packetOffset;    /* Offset in bytes to place the data in this packet. */
    UINT32 packetBytes;	    /* Number of bytes of compressed data in this packet. */
    UINT32 totalBytes;	    /* Number of bytes of compressed data in image region. */

    UINT16 sequenceNum;	    /* Sequence number - increments for each region. */
    UINT16 azimuthNum;	    /* Last azimuth updated 0-65536 -> 0-360 degrees */

    UINT32 capTimeSecs;	    /* Video capture time - seconds part of UNIX timestamp. */
    UINT32 capTimeUsecs;    /* Video capture time - microseconds part of UNIX timestamp. */

    UINT32 chunkNum;	    /* Number of this packet within group. */
    UINT32 chunkTotal;	    /* Total chunks within this group. */

    UINT8  numAudioChannels;/* Audio only - Number of audio channels. */

    UINT8  screenNumber;    /* Screen number (for screen capture). */
    UINT16 reserved15;

    UINT32 samplesPerSec;   /* Audio only - Samples per second. */

    UINT16 width;	    /* Video only - Current total image width in pixels. */
    UINT16 height;	    /* Video only - Current total image height in pixels. */

    UINT16 x;		    /* Video only - X position of region to update. */
    UINT16 y;		    /* Video only - Y position of region to update. */
    UINT16 w;		    /* Video only - Width of region to update. */
    UINT16 h;		    /* Video only - Height of region to update. */

    REAL32 viewX;	    /* Images only - Window centre X co-ordinate in metres
			     * (relative to radar centre). */
    REAL32 viewY;	    /* Images only - Window centre Y co-ordinate in metres
			     * (relative to radar centre). */
    REAL32 viewW;	    /* Images only - Width of area distributed in metres. */
    REAL32 viewH;	    /* Images only - Height of area distributed in metres. */
    REAL32 rangeEnd;	    /* Images only - Maximum range of radar in metres. */

    UINT32 avgBytesPerSec;  /* Audio only - Average bytes per second. */
    UINT32 blockAlignment;  /* Audio only - Block alignment, in bytes. */

    UINT32 durationUsecs;   /* Duration of chunk in microseconds. */
    
    /* NB: image data follows this, of size packetBytes.
     */
} SPxPacketImageChunk;

/* SPxPacketMetadata */
typedef struct SPxPacketMetadata_tag
{
    UINT16 nameLength;	    /* Number of bytes in name string. */
    UINT16 valueLength;	    /* Number of bytes in value string or data. */
    UINT16 streamID;	    /* ID of stream (when saving metadata to a multi-stream file) */
    UINT16 type;	    /* Type of metadata - see SPX_METADATA_... */
    UINT16 options;	    /* Metadata options - see SPX_METADATA_OPT_... */
    
    UINT16 reserved3;
    UINT32 reserved4;

    /* NB: name string (nameLength bytes), then NULL terminator
     * then value string (or data) (valueLength bytes), then NULL terminator
     * follows this, not included in 
     * sizeof(structure).
     */
} SPxPacketMetadata;	/* NB: Use sizeof() with extreme care */

/* SPxPacketAVInfo */
typedef struct SPxPacketAVInfo_tag
{
    UINT32 address;	    /* IP address for replies/of client. */
    UINT16 streamPort;	    /* Port clients should send info messages to. */
    UINT16 clientPort;	    /* Port client is sending info messages with. */
    UINT32 reserved3;
    UINT32 reserved4;

    /* The following figures are all measured over the last 5 seconds. */
    UINT32 avgLatency;	    /* Average latency in milliseconds. */
    UINT32 minLatency;	    /* Minimum latency in milliseconds. */
    UINT32 maxLatency;	    /* Maximum latency in milliseconds. */
    UINT32 networkLoss;	    /* Percentage frames lost. */
    UINT32 droppedPackets;  /* Number of packets dropped by client. */

    UINT32 reserved10;
    UINT32 reserved11;
    UINT32 reserved12;

    UINT32 capTimeSecs;	    /* Capture time (seconds) of frame. */
    UINT32 capTimeUsecs;    /* Capture time (microseconds) of frame. */

    UINT32 reserved15;
    UINT32 reserved16;

    UINT16 dataId;	    /* ID of frame latency info refers to. */
    UINT16 numIntervals;    /* Number of SPxPacketAVProcInfo objects in this message. */

    UINT32 reserved19;
    UINT32 reserved20;

    /* NB: a series of SPxPacketAVProcInfo objects follows this. There are 
     * numIntervals objects. These are not included in sizeof(structure).
     */

} SPxPacketAVInfo; 	/* NB: Use sizeof() with extreme care */

/* SPxPacketAVProcInfo */
typedef struct SPxPacketAVProcInfo_tag
{
    UINT32 process;	    /* Process type - see SPX_AV_OP_... enumeration. */
    UINT32 intervalMsecs;   /* Interval in ms that this process took. */
    UINT32 droppedPackets;  /* Number of packets dropped by process in last 5 seconds. */

    UINT32 reserved4;
    UINT32 reserved5;
    UINT32 reserved6;
    UINT32 reserved7;
    UINT32 reserved8;

} SPxPacketAVProcInfo;

/* SPxPacketAVTrack */
typedef struct SPxPacketAVTrack_tag
{
    REAL32 cx;		    /* Target position in frame (0.0 - 1.0). */
    REAL32 cy;

    REAL32 width;	    /* Target size in frame (0.0 - 1.0). */
    REAL32 height;

    REAL32 gateCX;	    /* Gate centre position in frame (0.0 - 1.0). */
    REAL32 gateCY;
    REAL32 gateWidth;	    /* Gate size in frame (0.0 - 1.0). */
    REAL32 gateHeight;

    REAL32 extractCX;	    /* Extracted position in frame (0.0 - 1.0). */
    REAL32 extractCY;
    REAL32 extractWidth;    /* Extracted size in frame (0.0 - 1.0). */
    REAL32 extractHeight;

    REAL32 speedX;	    /* Speed in frame dimensions per second. */
    REAL32 speedY;

    REAL32 bearingDegs;	    /* Target position in world units. */
    REAL32 elevationDegs;

    REAL32 widthDegs;	    /* Target size in world units. */
    REAL32 heightDegs;

    REAL32 extractCXDegs;    /* Extracted position in world units. */
    REAL32 extractCYDegs;
    REAL32 extractWidthDegs; /* Extracted size in world units. */
    REAL32 extractHeightDegs;

    REAL32 gateCXDegs;	    /* Gate centre position in world units. */
    REAL32 gateCYDegs;
    REAL32 gateWidthDegs;   /* Gate size in world units. */
    REAL32 gateHeightDegs;

    REAL32 speedXDegsPerSec;/* Target speed in world units. */
    REAL32 speedYDegsPerSec;
    
    REAL32 rotationDegs;

    UINT8 coasting;
    UINT8 lost;
    UINT16 reserved1;
    UINT32 coastTimeSeconds;
    UINT32 coastTimeUseconds;
    UINT32 lastLostTimeSeconds;
    UINT32 lastLostTimeUseconds;


} SPxPacketAVTrack;

/* SPxPacketSimTargetList */
typedef struct SPxPacketSimTargetList_tag
{
    UINT32 numTargets; /* Number of targets. */

    REAL32 nearNoiseGain0;
    UINT32 nearNoiseRange;
    UINT8  enableNearNoise;

    UINT8  reserved13;
    UINT16 reserved14;

    UINT32 backgroundNoise;

    UINT32 reserved20;
    UINT32 reserved24;
    UINT32 reserved28;
    UINT32 reserved32;
    UINT32 reserved36;
    UINT32 reserved40;
    UINT32 reserved44;

    /* NB: a series of SPxPacketSimTarget objects follows this. There are 
     * numTargets objects. These are not included in sizeof(structure).
     */

} SPxPacketSimTargetList;

/* SPxPacketSimTarget */
typedef struct SPxPacketSimTarget_tag
{
    UINT32 id;		/* Target ID. */
    REAL32 x;		/* World x. */
    REAL32 y;		/* World y. */
    REAL32 breadth;	/* Breadth (in metres). */
    REAL32 length;	/* Length (in metres). */
    REAL32 heading;	/* Heading (in degrees). */
    REAL32 speed;	/* Speed (world units per second). */
    UINT8  state;	/* State (active, deleted). */

    UINT8  reserved29;
    UINT16 reserved30;
    UINT32 reserved32;
    UINT32 reserved36;
    UINT32 reserved40;
    UINT32 reserved44;

} SPxPacketSimTarget;

/* SPxPacketAlert */
typedef struct SPxPacketAlert_tag
{
    UINT32 senderID;	/* Sender ID of server. */
    UINT32 alertID;	/* Alert ID. */
    UINT32 alertType;	/* Type of alert structure. */

    UINT32 reserved12;

} SPxPacketAlert;

/* SPxPacketAlertError */
typedef struct SPxPacketAlertError_tag
{
    SPxPacketAlert alert; /* Common alert structure. */

    UINT32 type;	/* Error type. */
    UINT32 code;	/* Error code. */
    INT32 arg1;		/* Argument 1 passed to error handler. */
    INT32 arg2;		/* Argument 2 passed to error handler. */

    char arg3[32];	/* Argument 3 passed to error handler. */
    char arg4[32];	/* Argument 4 passed to error handler. */

} SPxPacketAlertError;

/* SPxPacketAlertHPxAlarm */
typedef struct SPxPacketAlertHPxAlarm_tag
{
    SPxPacketAlert alert; /* Common alert structure. */

    UINT32 boardID;	/* ID of HPx card in system. */
    UINT32 newAlarms;	/* New alarms. */

    UINT32 reserved8;
    UINT32 reserved12;

} SPxPacketAlertHPxAlarm;

/* SPxPacketAlertLogicRegion */
typedef struct SPxPacketAlertLogicRegion_tag
{
    char regionName[32];/* Name of region. */

    UINT32 reporterType; /* Type of reporter. */
    UINT32 reporterID;	/* Reporter ID. */
    UINT32 targetID;	/* Target ID. */

    UINT32 isRangeAziValid; /* Are the following range/azi fields valid? */
    REAL32 rangeMetres;	/* Range of target from radar in metres. */
    REAL32 aziDegs;	/* Bearing of target from radar in degrees. */

    UINT32 isLatLongValid;   /* Are the following lat/long fields valid? */
    REAL32 latDegs;	/* Target latitude in degrees. */
    REAL32 longDegs;	/* Target longitude in degrees. */

    REAL32 courseDegs;	/* Target course in degrees. */
    REAL32 speedMps;	/* Target speed in m/s. */

    UINT32 reserved76;
    UINT32 reserved80;
    UINT32 reserved84;

} SPxPacketAlertLogicRegion;

/* SPxPacketAlertLogicCpa */
typedef struct SPxPacketAlertLogicCpa_tag
{
    UINT32 reporterType; /* Type of reporter. */
    UINT32 reporterID;	/* Reporter ID. */
    UINT32 targetID;	/* Target ID. */

    UINT32 isRangeAziValid; /* Are the following range/azi fields valid? */
    REAL32 rangeMetres;	/* Range of target from radar in metres. */
    REAL32 aziDegs;	/* Bearing of target from radar in degrees. */

    UINT32 isLatLongValid;   /* Are the following lat/long fields valid? */
    REAL32 latDegs;	/* Target latitude in degrees. */
    REAL32 longDegs;	/* Target longitude in degrees. */

    REAL32 courseDegs;	/* Target course in degrees. */
    REAL32 speedMps;	/* Target speed in m/s. */

    REAL32 cpaMetres;	/* Closest point of approach distance in metres. */
    REAL32 tcpaSecs;	/* Time to closest point of approach in seconds. */

    UINT32 reserved52;
    UINT32 reserved56;
    UINT32 reserved60;

} SPxPacketAlertLogicCpa;

/*
 * Logic Alert
 *
 * NB: This is a variable-sized structure on the network, since the
 * logic rule specific structures are optional.
 * The 'mask' field contains bit flags to indicate which of the
 * subsequent structures are present on the network.
 * If present, they will be in the order given here.
 *
 * Therefore, only use sizeof() this structure with extreme care.
 */
typedef struct SPxPacketAlertLogic_tag
{
    SPxPacketAlert alert; /* Common alert structure. */

    UINT32 groupID;	/* ID of the logic group causing alert. */
    char groupName[32]; /* Name of the logic group causing alert. */

    UINT32 mask;	/* Bitmask of which structures are valid
			 * See SPX_PACKET_ALERT_LOGIC_... flags.
			 */

    /* Optional fields, presence on network and validity in client structure
     * controlled by bit flags in 'mask' field above.
     */
    SPxPacketAlertLogicRegion regionEnter;  /* Region Rules. */
    SPxPacketAlertLogicRegion regionLeave;
    SPxPacketAlertLogicRegion regionCreated;
    SPxPacketAlertLogicRegion regionLost;
    SPxPacketAlertLogicRegion regionIn;
    SPxPacketAlertLogicRegion regionWasFirstIn;
    SPxPacketAlertLogicCpa cpa;		    /* CPA Rule. */

} SPxPacketAlertLogic;


/*
 * Dropout reports.
 */
typedef struct SPxPacketDropout_tag {
    UINT32 seqNum;		/* Most recent sequence number received */
    INT32 jump;			/* Calculated jump in sequence */
    UINT32 problemCount;	/* Count of detected problems */
    UINT32 reserved4;		/* Reserved, set to zero */
    UINT32 reserved5;		/* Reserved, set to zero */
    UINT32 reserved6;		/* Reserved, set to zero */
    UINT32 reserved7;		/* Reserved, set to zero */
    UINT32 reserved8;		/* Reserved, set to zero */
} SPxPacketDropout;

/*
 * Error recording.
 */
typedef struct SPxPacketErrorDesc_tag
{
    UINT32 errorType;
    UINT32 errorCode;
    INT32 arg1;
    INT32 arg2;
    char arg3[64];
    char arg4[64];

} SPxPacketErrorDesc;

/*
 * File transfer.
 */
typedef struct SPxPacketFile_tag 
{
    char filename[256];         /* Null terminated filename. */
    UINT32 sizeBytes;           /* File size in bytes. */
    UINT32 reserved1;		/* Reserved, set to zero */
    UINT32 reserved2;		/* Reserved, set to zero */
    UINT32 reserved3;		/* Reserved, set to zero */
    UINT32 reserved4;		/* Reserved, set to zero */
    
} SPxPacketFile;

/*
 * Structures used within recording files (payloads of SPxPacket packets).
 */

/* The master block record appears at the start of the recording file. */
typedef struct SPxSRfileMasterBlock_tag
{
    /* General information. */
    UINT16 headerSizeBytes;	/* The size of this header in bytes */
    UINT8 protocolVersion;	/* The version of the protocol */
    UINT8 screenMask;		/* The screens recorded */

    /* Screen 0 information. */
    UINT8 xScreenID0;		/* X Windows screen number of this screen */
    UINT8 depthOfScreen0;	/* Depth of screen (expected to be 32) */
    UINT16 reservedScreen0;	/* Reserved, set to 0 */
    UINT16 widthOfScreen0;	/* Width of the screen grabs in pixels */
    UINT16 heightOfScreen0;	/* Height of the screen grabs in pixels */

    /* Screen 1 information. */
    UINT8 xScreenID1;		/* X Windows screen number of this screen */
    UINT8 depthOfScreen1;	/* Depth of screen (expected to be 32) */
    UINT16 reservedScreen1;	/* Reserved, set to 0 */
    UINT16 widthOfScreen1;	/* Width of the screen grabs in pixels */
    UINT16 heightOfScreen1;	/* Height of the screen grabs in pixels */

    /* Timestamps. */
    UINT32 epochStartTimeSeconds;	/* Recording start (seconds part) */
    UINT32 epochStartTimeUseconds;	/* Recording start (usecs part) */
    UINT32 epochEndTimeSeconds;		/* Recording end (seconds part) */
    UINT32 epochEndTimeUseconds;	/* Recording end (usecs part) */

    /* Length and frame rate. */
    UINT32 durationOfRecording;	/* Length of recording rounded up (seconds) */
    UINT32 framesPerSecond;	/* Nominal frames per second recorded */

    /* Screen 2 information. */
    UINT8 xScreenID2;		/* X Windows screen number of this screen */
    UINT8 depthOfScreen2;	/* Depth of screen (expected to be 32) */
    UINT16 reservedScreen2;	/* Reserved, set to 0 */
    UINT16 widthOfScreen2;	/* Width of the screen grabs in pixels */
    UINT16 heightOfScreen2;	/* Height of the screen grabs in pixels */

    /* Reserved fields, all set to 0. */
    UINT32 reserved14;
    UINT32 reserved15;
    UINT32 reserved16;

} SPxSRfileMasterBlock;

/* Header for each full or partial screen grab, followed by data. */
typedef struct SPxSRframeHeader_tag
{
    /* General information. */
    UINT16 headerSizeBytes;	/* The size of this header in bytes */
    UINT8 screenNumber;		/* The original screen number (0, 1 or 2). */
    UINT8 reserved1;		/* Reserved, set to 0 */

    /* Dimensions of this grab (AFTER scaling) or 0,0 for end of sequence. */
    UINT16 width;		/* Width in pixels. */
    UINT16 height;		/* Height in pixels. */

    /* Encoded size of frame in 32-bit words. */
    UINT32 numEncodedWords;	/* Amount of data to follow header */

    /* Offset to top-left of grab (AFTER scaling). */
    UINT16 x;			/* X coord in pixels */
    UINT16 y;			/* Y coord in pixels */

} SPxSRframeHeader;

/* Configuration file - NULL terminated text follows this structure. */
typedef struct SPxPacketConfigFile_tag {

    char name[256];
    UINT8 reserved[256];

} SPxPacketConfigFile;

/*
 * Flight plans.  Maintain SPxFlightPlan::NtohPacket() and HtonPacket() too.
 *
 * This structure is initially 1008 bytes long, which when added to the packet
 * B header (16 bytes) gives a total of 1024 bytes.  However, check the size
 * when reading packets from file/network in case a larger packet is needed
 * in the future.
 */
typedef struct SPxPacketFlightPlan_tag {
    /* Callsign and type of flight plan etc. */
    char callsign[21];		/* Null-terminated callsign/identifier*/
    UCHAR flightRules;		/* I=IFR, V=VFR etc. */
    UCHAR flightType;		/* S=Scheduled, M=Military etc. */
    UCHAR flags;		/* SPX_PACKET_FLIGHT_PLAN_FLAGS_... */

    /* Aircraft. */
    char acType[8];		/* Null-terminated type, e.g. "A319" */
    UINT16 iffCode;		/* Octal IFF mode-3/A code for flight */
    UCHAR numAC;		/* Number of aircraft in flight */
    UCHAR acWake;		/* Wake category (L/M/H/J) */

    /* Timestamp components. */
    UCHAR etdHours;		/* Estimated Time of Departure, 0 to 23 */
    UCHAR etdMins;		/* Estimated Time of Departure, 0 to 59 */
    UCHAR eetHours;		/* Estimated Elapsed Time, 0 to 23 */
    UCHAR eetMins;		/* Estimated Elapsed Time, 0 to 59 */

    /* Time this was last edited. */
    UINT32 lastEditSecs;	/* Seconds since 1970 epoch */

    /* Reserved space. */
    char reserved1[60];		/* Not yet used - set to 0 */

    /* Locations (NULL terminated strings). */
    char depAero[16];		/* Departure aerodrome, e.g. "EGLL" */
    char destAero[16];		/* Destination aerodrome, e.g. "Heathrow" */

    /* Requested information (NULL terminated strings). */
    char speed[8];		/* Human text, e.g. "0.9M", "400kts" */
    char altitude[8];		/* Human text e.g. "FL320", "32000ft" */

    /* Reserved space. */
    char reserved2[88];		/* Not yet used - set to 0 */

    /* Remarks (NULL terminated string). */
    char remarks[256];		/* Additional comments */

    /* Route (NULL terminated string). */
    char route[512];		/* White space separated waypoints etc. */
} SPxPacketFlightPlan;

/*
 * Channel switching.
 *
 * This packet may be included in a packet stream that contains
 * data that can apply to different channels. It is used to specify
 * the channel index for the next packet. Once the next packet has
 * been decoded, the channel index reverts to the default of 0.
 */
typedef struct SPxPacketChanSelect_tag 
{
    UCHAR nextChannelIndex;    /* Next channel index. */
    UINT8 reserved1[3];        /* Reserved space. */
    UINT32 reserved2;          /* Reserved space. */
    
} SPxPacketChanSelect;

/*
 * Strobe information (e.g. jamming).
 */
typedef struct SPxPacketStrobe_tag
{
    /* Timestamp of detection/receipt. */
    UINT32 timeSecs;		 /* Whole seconds since unix epoch */
    UINT32 timeUsecs;		 /* Microseconds to be added to timeSecs */

    /* Polar window defining the strobe. */
    REAL32 startRangeMetres;
    REAL32 endRangeMetres;
    REAL32 startAziDegs;
    REAL32 endAziDegs;
} SPxPacketStrobe;


/*
 * Image file.
 *
 * This packet is used to wrap an image file.
 */
typedef struct SPxPacketImageFile_tag 
{
    UINT32 imageOffset;         /* Offset in bytes to start of image data. */
    UINT32 imageBytes;	        /* Number of bytes of image data. */
    UINT8 type;                 /* Image type. */
    UINT8 reserved1[3];         /* Reserved space. */
    UINT32 widthPixels;         /* Image width in pixels. */              
    UINT32 heightPixels;        /* Image height in pixels. */
    UINT32 bpp;                 /* Image bits per pixel. */   
    UINT32 reserved3;           /* Reserved space. */
    UINT32 reserved4;           /* Reserved space. */
    UINT32 reserved5;           /* Reserved space. */
    char filename[256];         /* Image filename (no path). */

    /* NB: Image data follows this of size imageBytes. */
    
} SPxPacketImageFile;

/*
 * Record of events, etc.
 */
typedef struct SPxPacketRecord_tag
{
    UINT32 recID;               /* Unique record ID. */ 
    UINT32 type;                /* SPX_PACKET_RECORD_TYPE_ */     
    UINT32 timeSecs;            /* Whole seconds since unix epoch */
    UINT32 timeUsecs;           /* Microseconds to be added to timeSecs */
    UINT32 chanID;              /* Linked channel ID or 0 for none. */
    UINT32 rgb;                 /* Colour to use for rendering. */
    UINT8 isLatLongSet;         /* Is position set for this record? */
    UINT8 isIdle;               /* Is the channel currently idle? */
    UINT8 wasIdle;              /* Was the channel previously idle? (V1.85 and later). */
    UINT8 reserved1;            /* Reserved space. */
    REAL32 latDegs;             /* Record latitude in degrees. */
    REAL32 longDegs;            /* Record longitude in degrees. */
    UINT32 stampID;             /* Stamp ID - see SPxStamp.h (V1.83.1 and later). */
    char text[256];             /* Optional record text. */
    UINT32 status;              /* Associated status (SPxStatus_t). */
    UINT32 prevStatus;          /* Previous status if applicable. */
    char sessionName[256];      /* Name of associated session. */
    UINT32 version;             /* Current library version number. */
    UINT32 reserved4;           /* Reserved space. */
    UINT32 reserved5;           /* Reserved space. */
    UINT32 reserved6;           /* Reserved space. */
    UINT32 reserved7;           /* Reserved space. */
    UINT32 reserved8;           /* Reserved space. */
    UINT32 reserved9;           /* Reserved space. */
    UINT32 reserved10;          /* Reserved space. */

} SPxPacketRecord;

/*
 * Channel database configuration. 
 */
typedef struct SPxPacketChanDbConfig_tag
{
    UINT32 numChannels;          /* Total number of channels. */
    UINT32 reserved1;            /* Reserved space. */
    UINT32 reserved2;            /* Reserved space. */
    UINT32 reserved3;            /* Reserved space. */
    UINT32 reserved4;            /* Reserved space. */
    UINT32 reserved5;            /* Reserved space. */

} SPxPacketChanDbConfig;

/*
 * JSON file info.
 */
typedef struct SPxPacketJson_tag
{
    char file[256];		/* Filename */
    char reserved[256];		/* Reserved space */

} SPxPacketJson;

#define	SPX_PACKET_JSON_DATA_FLAGS_COMPRESSED	0x01
#define	SPX_PACKET_JSON_DATA_FLAGS_ENCRYPTED	0x02
typedef struct SPxPacketJsonData_tag
{
    UINT16 flags;		/* SPX_PACKET_JSON_DATA_FLAGS_... */
    UINT16 reserved1;
    UINT32 dataSizeBytes;	/* Size of uncompressed data. */
    UINT16 reserved4;
    UINT16 reserved5;
    UINT16 reserved6;
    UINT16 reserved7;

} SPxPacketJsonData;

/*
 * Packet linking for fast indexing of recording files.
 */
#define SPX_MAX_NUM_PACKET_LINKS 8
typedef struct SPxPacketLink_tag
{
    UINT8 channelIndex;          /* Channel index. */
    UINT8 numEntries;            /* Number of links. */
    UINT16 reserved1;            /* Reserved space. */
    UINT32 reserved2;            /* Reserved space. */
    UINT32 reserved3;            /* Reserved space. */
    UINT32 reserved4;            /* Reserved space. */
    UINT16 packetTypes[SPX_MAX_NUM_PACKET_LINKS]; /* Linked packet types. */
    UINT64 fileOffsets[SPX_MAX_NUM_PACKET_LINKS]; /* Offset to next packets. */

} SPxPacketLink;

/*
 * Camera position.
 */
typedef struct SPxPacketCameraPos_tag
{
    UINT8 channelIndex;		/* Channel index. */
    UINT8 direction;		/* Axis direction. */
    UINT8 valType;		/* Is this a position (0), a speed (1) or a limit (2)? */
    UINT8 isSecondary;		/* Is this for a secondary sensor? */
    REAL32 value;		/* Value in specified direction. */
    UINT32 reserved2;		/* Reserved space. */
    UINT32 reserved3;		/* Reserved space. */

} SPxPacketCameraPos;

/*
 * Camera command.
 */
typedef struct SPxPacketCameraCommand_tag
{
    UINT8 channelIndex;		/* Channel index. */
    UINT8 type;			/* See SPxCamera::RequestType. */
    UINT16 reserved1;		/* Reserved space. */
    UINT32 reserved2;		/* Reserved space. */
    REAL64 value1;		/* Values for command. */
    REAL64 value2;
    UINT32 mask;
    UINT32 reserved3;		/* Reserved space. */
    UINT32 reserved4;		/* Reserved space. */
    UINT32 reserved5;		/* Reserved space. */
    UINT32 reserved6;		/* Reserved space. */
    UINT32 reserved7;		/* Reserved space. */

} SPxPacketCameraCommand;

/*
 * Alarm.
 */
typedef struct SPxPacketAlarm_tag
{
    char alarmAreaName[32];     /* Null terminated name of alarm zone. */
    UINT8 alarmStatus;          /* SPX_PACKET_ALARM_STATUS_ */
    UINT8 reserved1;            /* Reserved for future use. */
    UINT16 reserved2;           /* Reserved for future use. */
    UINT32 trackID;             /* Track ID. */
    REAL32 rangeMetres;         /* Range in metres. */
    REAL32 azimuthDegs;         /* Azimuth in degrees. */
    REAL32 speedMps;            /* Speed in metres per second. */
    REAL32 courseDegs;          /* Course in degrees. */
    REAL32 latDegs;             /* Latitude in degrees, +ve north. */
    REAL32 longDegs;            /* Longitude in degrees, +ve east. */
    UINT32 weight;              /* Weight of track (number of samples). */
    UINT32 sensors;             /* Bit field of contributing sensors. */
    UINT32 reserved5;           /* Reserved for future use. */
    UINT32 reserved6;           /* Reserved for future use. */

} SPxPacketAlarm;

/*
 * Changes.
 */
typedef struct SPxPacketChanges_tag
{
    UINT32 changeFlags;
    UINT32 reserved1;
    UINT32 reserved2;
    UINT32 reserved3;

} SPxPacketChanges;

/*
 * License info.   Needs to be multiple of 8-bytes.
 */
typedef struct SPxPacketLicenseInfo_tag
{
    UINT16 headerSizeBytes;	/* The size of this header (=>version) */
    UINT16 infoType;		/* Type of info packet */
    UINT16 textOffset;		/* Offset from start to NULL-terminated text */
    UINT16 textSizeBytes;	/* Size of text including NULL terminator */
    UINT16 dataOffset;		/* Offset from start to binary data */
    UINT16 dataSizeBytes;	/* Size of binary data */
    UINT32 licNum;		/* License number */
    UINT32 mac;			/* MAC address */
    UINT32 errorCode;		/* SPx error code */
    UINT32 version;		/* SPx version */
    UINT32 timestamp;		/* Epoch seconds, or 0 if not set */
    UINT32 flags;		/* Flags being requested, or 0 if not set */
    UINT32 reserved10;		/* Not used, set to 0. */
    UINT32 reserved11;		/* Not used, set to 0. */
    UINT32 reserved12;		/* Not used, set to 0. */
    UINT32 reserved13;		/* Not used, set to 0. */
    UINT32 reserved14;		/* Not used, set to 0. */
    UINT32 reserved15;		/* Not used, set to 0. */
    UINT32 checksum;		/* XOR all the above fields then invert */
} SPxPacketLicenseInfo;

/*
 * Project specific messages.
 *
 * All project specific messages must begin with the 16-bit project code
 * and a 16-bit project specific packet type. This is used to determine the 
 * content of the rest of the structure.
 */
typedef struct SPxPacketProjectGeneric_tag
{
    UINT16 projectCode;		/* Project code. */
    UINT16 projectPacketType;	/* Project specific packet type. */
    /* Project specific fields follow. */

} SPxPacketProjectGeneric;


/*
 * Header for doppler data, for SPX_PACKET_TYPEB_DOPPLER.
 * This structure is followed by payload data depending on the dopplerFormat.
 * For P172, it is followed by an SPxReturnHeader and then data.
 */
typedef struct SPxPacketDopplerHdr_tag
{
    UINT16 headerSizeBytes;	/* The size of this header (=>version) */
    UINT16 dopplerFormat;	/* SPX_DOPPLER_FORMAT_... */
    UINT32 reserved02;		/* Not used, set to 0. */
    UINT32 reserved03;		/* Not used, set to 0. */
    UINT32 reserved04;		/* Not used, set to 0. */
    UINT32 reserved05;		/* Not used, set to 0. */
    UINT32 reserved06;		/* Not used, set to 0. */
    UINT32 reserved07;		/* Not used, set to 0. */
    UINT32 reserved08;		/* Not used, set to 0. */
} SPxPacketDopplerHdr;

/*
 * Header for SPxAreaObj event.
 */
typedef struct SPxPacketAreaEvent_tag
{
    UINT32 flags;		/* Bitmask of SPX_AREA_EVENT_... */
    char areaName[60];		/* Name of area event relates to. */
    char parentName[60];	/* Name of parent area. */

    UINT8 areaType;		/* Type of area. */
    UINT8 isColourSupported;	/* Is colour config supported? */
    UINT8 isLineWidthSupported;	/* Is line width config supported? */
    UINT8 isDashSupported;	/* Is dash config supported? */
    UINT8 isAutoCloseSupported;	/* Is auto close config supported? */
    UINT8 isEditable;		/* Is area editable? */
    UINT8 autoCloseEnabled;	/* Is auto close enabled? */
    UINT8 numDashes;		/* Number of dashes in dashes array. */

    UINT32 lineColour;		/* 0xAARRGGBB line colour. */
    UINT32 fillColour;		/* 0xAARRGGBB fill colour. */
    REAL32 lineWidth;		/* Line width in pixels. */
    REAL32 dashOffset;		/* Dash offset in pixels. */
    REAL32 dashes[8];		/* Dash spacing. */
    UINT8 isModeSupported;	/* Is mode config supported? */
    UINT8 mode;			/* Mode of the area. */
    UINT8 isMovable;		/* Is the area mobavle? */
    UINT8 reserved1;
    UINT32 heightModeMask;	/* Height mode mask. */
    REAL32 minHeightMetres;	/* Minimum height in metres. */
    REAL32 maxHeightMetres;	/* Maximum height in metres. */
    REAL32 posX;		/* X position. */
    REAL32 posY;		/* Y position. */
    UINT32 numPoints;		/* Number of points. */
    UINT32 reserved2;

} SPxPacketAreaEvent;

typedef struct SPxPacketAreaEventPos_tag
{
    REAL32 posX;		/* X position. */
    REAL32 posY;		/* Y position. */

} SPxPacketAreaEventPos;

/*
 * Nav Data state.
 */
typedef struct SPxPacketNavData_tag
{
    UINT16 flags;	/* Validity flags. */
    UINT16 expiredFlags;/* Expired flags. */
    REAL32 latDegs;
    REAL32 longDegs;
    REAL32 altitudeMetres;

    REAL32 speedMps;
    REAL32 courseDegs;
    REAL32 headingDegs;
    REAL32 terrainHeightMetres;

    REAL32 headingChangeRateHz;
    REAL32 headingMessageRateHz;
    REAL32 velocityMessageRateHz;
    UINT32 reserved3;

} SPxPacketNavData;

/*
 * Terrain data for radar area.
 */
typedef struct SPxPacketTerrainData_tag
{
    REAL32 centreLatDegs;
    REAL32 centreLongDegs;
    REAL32 widthDegs;
    REAL32 widthMetres;

    UINT16 widthPx;
    UINT16 heightPx;

    UINT32 reserved[3];

} SPxPacketTerrainData;

/*
 * AScan returns to be sent to a web interface.
 */
typedef struct SPxPacketWebAScanReturns_tag
{
    REAL32 startRangeMetres;
    REAL32 endRangeMetres;
    UINT32 numSamples;
    UINT32 reserved;

} SPxPacketWebAScanReturns;

/*
 * Bounds for radar video PNG sent to a web interface.
 */
typedef struct SPxPacketWebRadarVideoBounds_tag
{
    REAL32 centreLatDegs;
    REAL32 centreLongDegs;
    REAL32 widthDegs;
    REAL32 heightDegs;

} SPxPacketWebRadarVideoBounds;

/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/


#endif /* _SPX_PACKETS_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/

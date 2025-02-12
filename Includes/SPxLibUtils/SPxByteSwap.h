/*********************************************************************
*
* (c) Copyright 2007 - 2022, Cambridge Pixel Ltd.
*
* Purpose:
*   Header file for byte-swapping utility functions.
*
*
* Revision Control:
*   04/08/22 v1.40   SP 	Add SPxByteSwapInSitu() for SPxP539SensorData_t.
*
* Previous Changes:
*   26/07/22 1.39   BTB	Add SPxByteSwapInSitu() for SPxPacketWebRadarVideoBounds.
*   05/07/22 1.38   BTB	Add SPxByteSwapInSitu() for SPxPacketWebAScanReturns.
*   07/12/21 1.37   AGC	Add SPxByteSwapInSitu() for SPxPacketTerrain.
*   24/11/21 1.36   AGC	Add SPxByteSwapInSitu() for SPxPacketNavData.
*   19/08/21 1.35   AGC	Add SPxByteSwapInSitu() for SPxPacketAreaEventPos.
*   29/07/21 1.34   AGC	Add SPxByteSwapInSitu() for SPxPacketAreaEvent.
*   19/05/21 1.33   AGC	Add SPxByteSwapInSitu() for SPxRstPlot.
*   02/03/21 1.32   REW	Add SPxByteSwapInSitu() for SPxPacketDopplerHdr
*   10/11/20 1.31   AGC	Detect incorrect use of SPxNtohl/HtonlInSitu.
*   29/04/20 1.30   BTB	Add SPxByteSwapInSitu() for SPxPacketJsonData.
*   29/01/20 1.29   BTB	Add SPxByteSwapInSitu() for SPxPacketTrackerStatus_t.
*   22/08/19 1.28   REW	Add SPxCalcChecksum() for license message.
*   24/01/19 1.27   SP 	Remove SPxByteSwapInSitu() for SPxPacketProjectHB239.
*   11/01/19 1.26   AGC	Add SPxByteSwapInSitu() for SPxPacketPlotStatus.
*   16/07/18 1.25   AGC	Add SPxByteSwapInSitu() for P322 structures.
*   02/07/18 1.24   AGC	Add SPxByteSwapInSitu() for SPxPacketProjectHB239.
*   15/06/18 1.23   AGC	Add SPxByteSwapInSitu() for SPxPacketPlot.
*   23/01/18 1.22   AGC	Add SPxByteSwapInSitu() for SPxPacketAlertError.
*   23/05/17 1.21   REW	Add SPxByteSwapInSitu() for SPxPacketLicenseInfo.
*   20/04/17 1.20   SP 	Add SPxByteSwapInSitu() overload for SPxPacketAlarm.
*   23/08/16 1.19   AGC	Remove SPxByteSwapInSitu() overload for non-existent SPxPacketChanConfig.
*			Add SPxByteSwapInSitu() overload for SPxPacketConfigFile.
*   08/07/16 1.18   AGC	Add SPxByteSwapInSitu() overload for SPxPacketErrorDesc.
*   03/03/16 1.17   AGC	Add SPxByteSwapInSitu() overload for SPxPacketRadarReturnExtra.
*   16/11/15 1.16   AGC	Add SPxByteSwapInSitu() overload for SPxPacketAVTrack.
*   09/11/15 1.15   AGC	Add SPxByteSwapInSitu() overload for SPxPacketCameraCommand.
*   03/11/15 1.14   AGC	Add SPxByteSwapInSitu() overload for SPxPacketCameraPos.
*   15/05/15 1.13   SP 	Add SPxByteSwapInSitu() overload for SPxPacketLink.
*   29/04/15 1.12   AGC	Add byteswap for SPxPacketJson.
*   17/03/15 1.11   AGC	Add more SPxByteSwapInSitu overloads.
*   06/03/15 1.10   AGC	Add more SPxByteSwapInSitu overloads.
*			Add static assert for invalid byte-swap sizes.
*   13/03/14 1.9    AGC	Add template SPxByteSwapInSitu on T (as well as sizeof(T)).
*			Add various overloads of SPxByteSwapInSitu.
*   22/11/13 1.8    AGC	Move template SPxByteSwapInSitu here from SPxByteStream.
*			Include SPxTypes.h instead of SPxCommon.h.
*			Forward declare SPxReturnHeader instead of using extern "C".
*			Add SPxNtohInSitu/SPxHtonInSitu.
*   08/02/12 1.7    AGC	Add SPxByteSwap64().
*			Add SPxByteSwapFloat().
*			Add SPxByteSwapDouble().
*   19/08/11 1.6    SP 	Add SPxHostToBE32() and friends.
*   10/02/09 1.5    REW	Add SPxConverterHeaderToHost/Network() and
*			extern C around function declarations.
*   06/11/08 1.4    REW	Add SPxIsNetworkEndian().
*   04/11/07 1.3    REW	Add SPxByteSwapInSitu64()
*   16/07/07 1.2    REW	More swapping functions.
*   08/06/07 1.1    REW	Initial Version.
**********************************************************************/

#ifndef _SPX_BYTESWAP_H
#define _SPX_BYTESWAP_H

/* Make sure we have the common types. */
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

/*********************************************************************
*
*   Macros
*
**********************************************************************/

/*********************************************************************
*
*   Variable definitions
*
**********************************************************************/

/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

/* Forward declarations. */
struct SPxReturnHeader_tag;
struct SPxPacketHeaderA_tag;
struct SPxPacketHeaderB_tag;
struct SPxPacketRadarConfig_tag;
struct SPxPacketRadarReturn_tag;
struct SPxPacketNet_tag;
struct SPxPacketTOCHdr_tag;
struct SPxPacketTOCEntry_tag;
struct SPxPacketImageChunk_tag;
struct SPxPacketMetadata_tag;
struct SPxSRfileMasterBlock_tag;
struct SPxSRframeHeader_tag;
struct SPxNetChunkHeader_tag;
struct SPxScNetHeartbeatStruct_tag;
struct SPxPacketChanSelect_tag;
struct SPxPacketImageFile_tag;
struct SPxPacketRecord_tag;
struct SPxPacketChanDbConfig_tag;
struct SPxPacketJson_tag;
struct SPxPacketLink_tag;
struct SPxPacketCameraPos_tag;
struct SPxPacketCameraCommand_tag;
struct SPxPacketErrorDesc_tag;
struct SPxPacketConfigFile_tag;
struct SPxPacketLicenseInfo_tag;
struct SPxPacketAlert_tag;
struct SPxPacketAlertError_tag;
struct SPxPacketDopplerHdr_tag;
struct SPxPacketAreaEvent_tag;
struct SPxPacketAreaEventPos_tag;
struct SPxPacketNavData_tag;
struct SPxNetP322MsgDiscovery;
struct SPxNetP322MsgStatus;
struct SPxNetP322MsgHello;
struct SPxNetP322MsgHeartbeat;
struct SPxNetP322MsgTimingControl;
struct SPxNetP322MsgTransmitControl;
struct SPxNetP322MsgIPSetup;
struct SPxNetP322MsgSetup;
struct SPxNetP322MsgSave;
struct SPxRstPlot;
struct SPxP539SensorData_t;

/* Extern the functions we provide. */
extern int SPxIsHostBigEndian(void);
extern int SPxIsNetworkEndian(void);
extern void SPxNtohlInSituPtr(void *ptr);
extern void SPxHtonlInSituPtr(void *ptr);
extern void SPxByteSwapInSitu(struct SPxReturnHeader_tag *hdr);
extern void SPxByteSwapInSitu(struct SPxPacketHeaderA_tag *hdr);
extern void SPxByteSwapInSitu(struct SPxPacketHeaderB_tag *hdr);
extern void SPxByteSwapInSitu(struct SPxPacketRadarConfig_tag *hdr);
extern void SPxByteSwapInSitu(struct SPxPacketRadarReturn_tag *hdr);
extern void SPxByteSwapInSitu(struct SPxPacketRadarReturnExtra_tag *hdr);
extern void SPxByteSwapInSitu(struct SPxPacketPlot_tag *hdr);
extern void SPxByteSwapInSitu(struct SPxPacketPlotStatus_tag *hdr);
extern void SPxByteSwapInSitu(struct SPxPacketTrackerStatus_tag *hdr);
extern void SPxByteSwapInSitu(struct SPxPacketNet_tag *hdr);
extern void SPxByteSwapInSitu(struct SPxPacketTOCHdr_tag *hdr);
extern void SPxByteSwapInSitu(struct SPxPacketTOCEntry_tag *hdr);
extern void SPxByteSwapInSitu(struct SPxPacketImageChunk_tag *hdr);
extern void SPxByteSwapInSitu(struct SPxPacketMetadata_tag *hdr);
extern void SPxByteSwapInSitu(struct SPxPacketAVTrack_tag *hdr);
extern void SPxByteSwapInSitu(struct SPxSRfileMasterBlock_tag *hdr);
extern void SPxByteSwapInSitu(struct SPxSRframeHeader_tag *hdr);
extern void SPxByteSwapInSitu(struct SPxNetChunkHeader_tag *hdr);
extern void SPxByteSwapInSitu(struct SPxScNetHeartbeatStruct_tag *hdr);
extern void SPxByteSwapInSitu(struct SPxPacketChanSelect_tag *hdr);
extern void SPxByteSwapInSitu(struct SPxPacketImageFile_tag *hdr);
extern void SPxByteSwapInSitu(struct SPxPacketRecord_tag *hdr);
extern void SPxByteSwapInSitu(struct SPxPacketChanDbConfig_tag *hdr);
extern void SPxByteSwapInSitu(struct SPxPacketJson_tag *hdr);
extern void SPxByteSwapInSitu(struct SPxPacketJsonData_tag *hdr);
extern void SPxByteSwapInSitu(struct SPxPacketLink_tag *hdr);
extern void SPxByteSwapInSitu(struct SPxPacketCameraPos_tag *hdr);
extern void SPxByteSwapInSitu(struct SPxPacketCameraCommand_tag *hdr);
extern void SPxByteSwapInSitu(struct SPxPacketErrorDesc_tag *hdr);
extern void SPxByteSwapInSitu(struct SPxPacketConfigFile_tag *hdr);
extern void SPxByteSwapInSitu(struct SPxPacketAlarm_tag *hdr);
extern void SPxByteSwapInSitu(struct SPxPacketLicenseInfo_tag *hdr);
extern UINT32 SPxCalcChecksum(struct SPxPacketLicenseInfo_tag *hdr);
extern void SPxByteSwapInSitu(struct SPxPacketAlert_tag *hdr);
extern void SPxByteSwapInSitu(struct SPxPacketAlertError_tag *hdr);
extern void SPxByteSwapInSitu(struct SPxPacketDopplerHdr_tag *hdr);
extern void SPxByteSwapInSitu(struct SPxPacketAreaEvent_tag *hdr);
extern void SPxByteSwapInSitu(struct SPxPacketAreaEventPos_tag *hdr);
extern void SPxByteSwapInSitu(struct SPxPacketNavData_tag *hdr);
extern void SPxByteSwapInSitu(struct SPxPacketTerrainData_tag *hdr);
extern void SPxByteSwapInSitu(struct SPxPacketWebAScanReturns_tag *hdr);
extern void SPxByteSwapInSitu(struct SPxPacketWebRadarVideoBounds_tag *hdr);
extern void SPxByteSwapInSitu(struct SPxNetP322MsgDiscovery *msg);
extern void SPxByteSwapInSitu(struct SPxNetP322MsgStatus *msg);
extern void SPxByteSwapInSitu(struct SPxNetP322MsgHello *msg);
extern void SPxByteSwapInSitu(struct SPxNetP322MsgHeartbeat *msg);
extern void SPxByteSwapInSitu(struct SPxNetP322MsgTimingControl *msg);
extern void SPxByteSwapInSitu(struct SPxNetP322MsgTransmitControl *msg);
extern void SPxByteSwapInSitu(struct SPxNetP322MsgIPSetup *msg);
extern void SPxByteSwapInSitu(struct SPxNetP322MsgSetup *msg);
extern void SPxByteSwapInSitu(struct SPxNetP322MsgSave *msg);
extern void SPxByteSwapInSitu(struct SPxNetP322MsgFEStc *msg);
extern void SPxByteSwapInSitu(struct SPxNetP322MsgIFStc *msg);
extern void SPxByteSwapInSitu(struct SPxNetP322MsgStcCorrection *msg);
extern void SPxByteSwapInSitu(struct SPxNetP322MsgMaintenance *msg);
extern void SPxByteSwapInSitu(struct SPxRstPlot *hdr);
extern void SPxByteSwapInSitu(struct SPxP539SensorData_t *msg);
extern void SPxByteSwapInSitu64(void *ptr);
extern void SPxByteSwapInSitu32(void *ptr);
extern void SPxByteSwapInSitu16(void *ptr);
extern UINT64 SPxByteSwap64(UINT64 val);
extern UINT32 SPxByteSwap32(UINT32 val);
extern UINT16 SPxByteSwap16(UINT16 val);
extern float SPxByteSwapFloat(float val);
extern double SPxByteSwapDouble(double val);
extern void SPxConvertHeaderToHost(struct SPxReturnHeader_tag *header);
extern void SPxConvertHeaderToNetwork(struct SPxReturnHeader_tag *header);
extern UINT32 SPxHostToBE32(UINT32 val);
extern UINT32 SPxHostToLE32(UINT32 val);
extern UINT16 SPxHostToBE16(UINT16 val);
extern UINT16 SPxHostToLE16(UINT16 val);
extern UINT32 SPxBE32ToHost(UINT32 val);
extern UINT32 SPxLE32ToHost(UINT32 val);
extern UINT16 SPxBE16ToHost(UINT16 val);
extern UINT16 SPxLE16ToHost(UINT16 val);

/* Class used to allow static assert for unsupported byte swap size. */
namespace SPxByteSwapNS
{
    template<typename T=int>
    class Impl
    {
    public:
	Impl()
	{
	    SPX_STATIC_ASSERT_ALWAYS(T, Invalid_byte_swap_size);
	}
    };
}
template<typename T>
inline void SPxNtohlInSitu(T *ptr)
{
    SPX_STATIC_ASSERT((sizeof(T) == 4),
		      Type_is_wrong_size__Did_you_mean_SPxNtohInSitu);
    SPxNtohlInSituPtr(ptr);
}
template<typename T>
inline void SPxHtonlInSitu(T *ptr)
{
    SPX_STATIC_ASSERT((sizeof(T) == 4),
		      Type_is_wrong_size__Did_you_mean_SPxHtonInSitu);
    SPxHtonlInSituPtr(ptr);
}

/* Function to perform byte-swapping, templated on the sizeof
 * the type being extracted.
 * This will cause unsupported sizes to fail to compile.
 */
template<int> inline void SPxByteSwapInSitu(void *)
{
    SPxByteSwapNS::Impl<int> impl;
}
template<> inline void SPxByteSwapInSitu<1>(void *) {}
template<> inline void SPxByteSwapInSitu<2>(void *ptr) { SPxByteSwapInSitu16(ptr); }
template<> inline void SPxByteSwapInSitu<4>(void *ptr) { SPxByteSwapInSitu32(ptr); }
template<> inline void SPxByteSwapInSitu<8>(void *ptr) { SPxByteSwapInSitu64(ptr); }
template<typename T>
inline void SPxByteSwapInSitu(T *ptr)
{
    SPxByteSwapInSitu<sizeof(T)>(ptr);
}
template<typename T>
inline void SPxByteSwapInSitu(void *ptr)
{
    SPxByteSwapInSitu<sizeof(T)>(ptr);
}
template<typename T>
inline void SPxNtohInSitu(T *ptr)
{
    if( !SPxIsNetworkEndian() )
    {
	SPxByteSwapInSitu(ptr);
    }
}
template<typename T>
inline void SPxHtonInSitu(T *ptr)
{
    if( !SPxIsNetworkEndian() )
    {
	SPxByteSwapInSitu(ptr);
    }
}

#endif /* _SPX_BYTESWAP_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/

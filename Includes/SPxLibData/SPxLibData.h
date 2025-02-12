/*********************************************************************
*
* (c) Copyright 2007 - 2022, Cambridge Pixel Ltd.
*
* Purpose:
*   Top-level header for SPxLibData library.
*
*   This should be the only header that needs to be included in
*   code that uses this library.  It includes all the other public
*   headers for the library, specifying the full (relative) pathname
*   to the headers, i.e. it expects the headers to be in an SPxLibData
*   subdirectory.
*
* Revision Control:
*   24/06/22 v1.61   BTB	Add SPxAscanIface.h.
*
* Previous Changes:
*   21/06/22 1.60   AGC	Add SPxSimPlatNav.h.
*   18/05/22 1.59   AGC	Add SPxCamCtrlFlirE.h.
*   29/04/22 1.58   AGC	Add SPxCamCtrlSlx.h.
*   04/03/22 1.57   AGC	Add SPxCamCtrlSkyWall.h.
*			Add SPxSkyWallConn.h.
*   03/06/21 1.56   AGC	Add SPxRecord.h.
*   16/03/21 1.55   AGC	Add SPxCamCtrlCamMngr.h.
*   28/01/21 1.54   REW	Add SPxDopplerStore.h
*   17/11/20 1.53   SP 	Add SPxConvertFile.h
*   13/11/20 1.52   AGC	Add SPxCamCtrlIRay.h.
*   23/10/20 1.51   REW	Add SPxAeroRoute.h
*   13/10/20 1.50   REW	Add SPxAero headers.
*   17/09/20 1.49   AGC	Add SPxModeSCoverageMap.h.
*   02/04/20 1.48   SP 	Add SPxMIB.h.
*   25/06/19 1.47   AGC	Add SPxPlotReceiver...h
*			Add SPxPlotReporter...h
*   06/02/19 1.46   BTB	Add SPxCamCtrlLRAD.h.
*   17/10/18 1.45   AGC	Add SPxRadarPlatform.h and SPxRadarPlatformWeiss.h.
*   16/07/18 1.44   AJH	Add SPxDroneDecoder.
*   02/07/18 1.43   AGC	Add SPxCamCtrlOnvif.
*   12/02/18 1.42   BTB	Add SPxCANBus.h.
*   18/01/18 1.41   AGC	Add SPxSimAISInput.h.
*			Add SPxSimAISStation.h.
*   13/07/17 1.40   AJH	Add SPxCamCtrlNexus.h.
*   22/06/17 1.39   AGC	Add SPxCamCtrlZeoSys.h.
*			Add SPxCamCtrlVision4ce.h.
*   31/01/17 1.38   AGC	Add SPxSimPlatform.h.
*   18/01/17 1.37   AGC	Add SPxCameraManager.h.
*   20/04/16 1.36   SP 	Add SPxSQLDatabase.h.
*   11/01/16 1.35   AGC	Add SPxSimRadarNoise.h.
*   03/11/15 1.34   AGC	Add SPxCamCtrlReplay.h.
*   01/10/15 1.33   SP 	Use renamed classes.
*   26/03/15 1.32   SP 	Add SPxSerialPort.h and SPxSerial.h.
*   26/01/15 1.31   SP 	Add SPxChannelNavData.h.
*   14/10/14 1.30   SP 	Add SPxChannelRadar.h and SPxChannelHPxRadar.h.
*                       Move SPxChannel.h and SPxChannelDatabase.h here.
*   10/04/14 1.29   AGC	Add SPxEbml/SPxVpxEnc headers.
*   27/03/14 1.28   SP 	Add SPxCamCtrlTest.
*   15/01/14 1.27   REW	Add SPxFlightPlan headers.
*   04/11/13 1.26   AGC	Sort headers.
*   24/10/13 1.25   AGC	Add camera control headers.
*   04/10/13 1.24   AGC	Move functions to SPxDataInit.h.
*   13/06/13 1.23   REW	Add SPxHPxCommon.h and SPxROC.h
*   25/04/13 1.22   AGC	Add SPxSimTargetTemplate.h.
*   17/04/13 1.21   REW	SPxAsterixDecoder moved to SPxLibNet.
*   21/12/12 1.20   AGC	Add SPx Logic headers.
*   26/10/12 1.19   AGC	Add Radar Simulator headers.
*   29/05/12 1.18   AGC	Add SPxAsterixDecoder.h
*   06/04/11 1.17   AGC	Add SPxPacketDecoder.h and SPxPacketDecoderFile.h
*   23/02/09 1.16   REW	Add SPxNavData.h
*   29/10/08 1.15   REW	Add SPxDGRSource.h
*   10/10/08 1.14   REW	Add SPxExternalSource.h
*   06/03/08 1.13   REW	Add SPxHPx100Source.h
*   25/10/07 1.12   REW	Add SPxRadarStream.
*   19/10/07 1.11   REW	Add SPxSyncCombineSrc.
*   17/09/07 1.10   DGJ	Add SPxProcessRegion, LUT and AreaSelect.h
*   30/08/07 1.9    DGJ	Add SPxProcess.h
*   24/08/07 1.8    DGJ	Add SPxShowState.h.
*   31/07/07 1.7    REW	Add SPxRadarReplayRaw.h.
*   25/07/07 1.6    REW	Add SPxCompressORC.h.
*   09/07/07 1.5    REW	Args to SPxLibDataInit().
*   27/06/07 1.4    DGJ	Added SPxRadarReplay.h.
*   07/06/07 1.3    REW	Init function returns status value.
*			Headers are included from subdir.
*   24/05/07 1.2    REW	Added header, changed to SPx etc.
*   12/04/07 1.1    DGJ	Initial Version
**********************************************************************/

#ifndef _SPX_LIBDATA_H_
#define	_SPX_LIBDATA_H_

/* Include all the public headers from this library, assuming they
 * are in a subdirectory of one of the directories in the include
 * path (i.e. so you don't need to add each subdirectory to the include
 * path, just the parent of them all).
 */
#include "SPxLibData/SPxDataInit.h"
#include "SPxLibData/SPxAreaSelect.h"
#include "SPxLibData/SPxAeroDb.h"
#include "SPxLibData/SPxAeroNavAid.h"
#include "SPxLibData/SPxAeroRenderer.h"
#include "SPxLibData/SPxAeroRoute.h"
#include "SPxLibData/SPxAscanIface.h"
#include "SPxLibData/SPxCamCtrlChessAsp.h"
#include "SPxLibData/SPxCamCtrlCamMngr.h"
#include "SPxLibData/SPxCamCtrlChessPiranha36.h"
#include "SPxLibData/SPxCamCtrlCreate.h"
#include "SPxLibData/SPxCamCtrlFlirE.h"
#include "SPxLibData/SPxCamCtrlIRay.h"
#include "SPxLibData/SPxCamCtrlLRAD.h"
#include "SPxLibData/SPxCamCtrlNexus.h"
#include "SPxLibData/SPxCamCtrlOnvif.h"
#include "SPxLibData/SPxCamCtrlPelco.h"
#include "SPxLibData/SPxCamCtrlReplay.h"
#include "SPxLibData/SPxCamCtrlSkyWall.h"
#include "SPxLibData/SPxCamCtrlSlx.h"
#include "SPxLibData/SPxCamCtrlTest.h"
#include "SPxLibData/SPxCamCtrlVision4ce.h"
#include "SPxLibData/SPxCamCtrlZeoSys.h"
#include "SPxLibData/SPxCameraManager.h"
#include "SPxLibData/SPxChannel.h"
#include "SPxLibData/SPxChannelDatabase.h"
#include "SPxLibData/SPxChannelHPxRadar.h"
#include "SPxLibData/SPxChannelNavData.h"
#include "SPxLibData/SPxChannelRadar.h"
#include "SPxLibData/SPxCompressORC.h"
#include "SPxLibData/SPxConvertFile.h"
#include "SPxLibData/SPxDGRSource.h"
#include "SPxLibData/SPxDopplerStore.h"
#include "SPxLibData/SPxDroneDecoder.h"
#include "SPxLibData/SPxEbml.h"
#include "SPxLibData/SPxExternalSource.h"
#include "SPxLibData/SPxFlightPlan.h"
#include "SPxLibData/SPxFlightPlanDb.h"
#include "SPxLibData/SPxHPx100Source.h"
#include "SPxLibData/SPxHPxCommon.h"
#include "SPxLibData/SPxLUT.h"
#include "SPxLibData/SPxLogicDB.h"
#include "SPxLibData/SPxLogicGroup.h"
#include "SPxLibData/SPxLogicRule.h"
#include "SPxLibData/SPxLogicRuleCpa.h"
#include "SPxLibData/SPxLogicRuleRegion.h"
#include "SPxLibData/SPxMIB.h"
#include "SPxLibData/SPxModeSCoverageMap.h"
#include "SPxLibData/SPxNavData.h"
#include "SPxLibData/SPxPacketDecoder.h"
#include "SPxLibData/SPxPacketDecoderFile.h"
#include "SPxLibData/SPxPim.h"
#include "SPxLibData/SPxPlotReceiver.h"
#include "SPxLibData/SPxPlotReceiverFile.h"
#include "SPxLibData/SPxPlotReceiverSrc.h"
#include "SPxLibData/SPxPlotReporter.h"
#include "SPxLibData/SPxPlotReporterFile.h"
#include "SPxLibData/SPxProcMan.h"
#include "SPxLibData/SPxProcess.h"
#include "SPxLibData/SPxProcessRegion.h"
#include "SPxLibData/SPxROC.h"
#include "SPxLibData/SPxRadarDb.h"
#include "SPxLibData/SPxRadarPlatform.h"
#include "SPxLibData/SPxRadarPlatformWeiss.h"
#include "SPxLibData/SPxRadarReplay.h"
#include "SPxLibData/SPxRadarReplayRaw.h"
#include "SPxLibData/SPxRadarSimServer.h"
#include "SPxLibData/SPxRadarSimulator.h"
#include "SPxLibData/SPxRadarSource.h"
#include "SPxLibData/SPxRadarStream.h"
#include "SPxLibData/SPxRecord.h"
#include "SPxLibData/SPxRib.h"
#include "SPxLibData/SPxSerial.h"
#include "SPxLibData/SPxSerialPort.h"
#include "SPxLibData/SPxCANBus.h"
#include "SPxLibData/SPxScenGen.h"
#include "SPxLibData/SPxShowState.h"
#include "SPxLibData/SPxSimAISInput.h"
#include "SPxLibData/SPxSimAISStation.h"
#include "SPxLibData/SPxSimMotion.h"
#include "SPxLibData/SPxSimMotionRenderer.h"
#include "SPxLibData/SPxSimPlatform.h"
#include "SPxLibData/SPxSimPlatNav.h"
#include "SPxLibData/SPxSimRadar.h"
#include "SPxLibData/SPxSimRadarNoise.h"
#include "SPxLibData/SPxSimSegment.h"
#include "SPxLibData/SPxSimTarget.h"
#include "SPxLibData/SPxSimTargetTemplate.h"
#include "SPxLibData/SPxSimTerrain.h"
#include "SPxLibData/SPxSkyWallConn.h"
#include "SPxLibData/SPxSQLDatabase.h"
#include "SPxLibData/SPxSyncCombineSrc.h"
#include "SPxLibData/SPxTestGen.h"
#include "SPxLibData/SPxVpxEnc.h"

/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* _SPX_LIBDATA_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/

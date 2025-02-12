/*********************************************************************
*
* (c) Copyright 2007 - 2022, Cambridge Pixel Ltd.
*
* Purpose:
*   Top-level header for SPxLibWin library.
*
*   This should be the only header that needs to be included in
*   code that uses this library.  It includes all the other public
*   headers for the library.
*
* Revision Control:
*   26/04/22 v1.148  SP 	Add SPxMonitorAlertsDlgWin.h and SPxMonitorAlertsPopupDlgWin.h
*
* Previous Changes:
*   28/02/22 1.147  AJH	Add SPxROCAdvCtrlDlgWin and SPxROCAdvCtrlPopupDlgWin.
*   18/02/22 1.146  AGC	Add SPxCamRecordDlgWin.h.
*			Add SPxCamRecordPopupDlgWin.h.
*   21/10/21 1.145  AGC	Add SPxRangeRingsCfgDlgWin.h.
*   25/06/21 1.144  SP 	Add SPxDetectedObjRendererD2D.h.
*   26/04/21 1.143  REW	Add SPxSystemTrayApp.h.
*   01/03/21 1.142  AGC	Add SPxWeatherRendererD2D.h.
*   16/02/21 1.141  BTB	Add SPxWeatherConfigDlgWin.h.
*			Add SPxWeatherConfigPopupDlgWin.h.
*			Add SPxWeatherRendererWin.h.
*   28/01/21 1.140  JGB	Add SPxADSBMonDlgWin, SPxADSBMonPopupDlgWin.
*   05/01/21 1.139  AGC	Add SPxP489CtrlDlgWin.h.
*   05/01/21 1.138  AGC	Add SPxP488CtrlDlgWin.h.
*   27/11/20 1.137  BTB	Add SPxRemoteHPxCtrlDlgWin.h.
*			Add SPxRemoteHPxCtrlPopupDlgWin.h.
*   20/11/20 1.136  BTB	Use more standardised names for dialogs.
*   19/11/20 1.135  BTB	Add SPxRemoteRadarCtrlDlgWin.h.
*			Add SPxRemoteRadarCtrlPopupDlgWin.h.
*   12/11/20 1.134  SP 	Rename EScan to Rhi.
*   27/10/20 1.133  SP 	Add SPxEScanGraphicsRendererWin.h.
*			Add SPxEScanGraphicsRendererD2D.h.
*   23/10/20 1.132  AGC	Re-add SPxTimelineRendererWin.h.
*			Remove SPxEScanRendererWin.h.
*   22/10/20 1.131  SP 	Add SPxEScanWin.h.
*   13/10/20 1.130  REW	Add SPxAeroRenderer headers.
*   18/09/20 1.129  AJH	Add SPxAISMonDlgWin, SPxAISMonPopupDlgWin.
*   18/09/20 1.128  AGC	Add SPxTrackFiltersDlgWin.h.
*			Add SPxTrackFiltersPopupDlgWin.h.
*			Add SPxTrackGroupEditDlgWin.h.
*			Add SPxTrackGroupEditPopupDlgWin.h.
*   17/09/20 1.127  AGC Add SPxModeSCoverageRendererD2D.h.
*                       Add SPxModeSCoverageRendererD2D.h.
*   04/09/20 1.126  BTB	Add SPxInfoDBSequenceInputDlgWin.h.
*   26/08/20 1.125  AGC	Add SPxMIL2525DDisplayDlgWin.h.
*   04/08/20 1.124  AGC	Add SPxSimTerrainServerCtrlDlgWin.h.
*			Add SPxSimTerrainServerCtrlPopupDlgWin.h.
*   06/07/20 1.123  AGC	Add SPxCamTourDlgWin.h.
*			Add SPxCamTourPopupDlgWin.h.
*   11/06/20 1.122  BTB	Add SPxInfoDBDispDlgWin.h
*   15/05/20 1.121  REW	Add SPxDisclaimerDlgWin.h
*   29/04/20 1.120  BTB	Add SPxInfoDBFieldEditDlgWin.h.
*			Add SPxInfoDBInputDlgWin.h.
*			Add SPxInfoDBItemEditDlgWin.h.
*			Add SPxInfoDBItemEditPopupDlgWin.h.
*   29/04/20 1.119  AGC	Add SPxCoveragePopupDlgWin.h.
*   03/04/20 1.118  AGC	Add SPxMacroBtnDlgWin.h.
*			Add SPxMacroBtnPopupDlgWin.h.
*   03/03/20 1.117  BTB	Add SPxManualTrackerSensorInfoDlgWin.h.
*   21/02/20 1.116  BTB	Add SPxManualTrackerDlgWin.h.
*			Add SPxManualTrackerPopupDlgWin.h.
*   10/12/19 1.115  SP 	Add SPxSiteRendererWin.h.
*   01/11/19 1.114  AJH	Add SPxP437CtrlDlgWin.h.
*   27/08/19 1.113  BTB	Add SPxCamFOVCalPopupDlgWin.h.
*   07/08/19 1.112  BTB	Add SPxCamFOVCalDlgWin.h.
*			Add SPxCamFOVCalPrevDlgWin.h
*			Add SPxCamFOVCalSelDlgWin.h
*			Add SPxCamFOVCalStgsDlgWin.h
*   30/07/19 1.111  AGC	Add SPxAlertRendererD2D.h.
*   01/07/19 1.110  AGC	Add SPxAlarmRendererD2D.h.
*   25/06/19 1.109  AGC	Add SPxP435CtrlDlgWin.h.
*   19/06/19 1.108  AGC	Add more Direct2D renderers.
*   17/06/19 1.107  AJH	Add SPxP430CtrlDlgWin.h.
*   11/06/19 1.106  SP 	Add SPxP395CtrlDlgWin.h and 
*			SPxP395CtrlPopupDlgWin.h.
*   07/06/19 1.105  SP 	Add SPxWinListCtrl.h.
*   03/06/19 1.104  AGC	Add SPxSafetyNetRendererD2D.h.
*   17/04/19 1.103  BTB	Add SPxHelpBtnDlgWin.h.
*   13/03/19 1.102  SP 	Add SPxScrollDlgWin.h.
*   12/03/19 1.101  BTB	Remove SPxLRADAudioDlgWin.h.
*   01/03/19 1.100  BTB	Add SPxLRADAudioDlgWin.h.
*   14/02/19 1.99   BTB	Add SPxLRADSearchlightDlgWin.h.
*   05/12/18 1.98   REW	Add SPxNavMon headers.
*   30/11/18 1.97   AGC	Add various Direct2D renderers.
*   09/11/18 1.96   BTB	Add SPxSharedFeatureDlgWin.h.
*   19/10/18 1.95   AGC	Add SPxP408CtrlDlgWin.h.
*   17/10/18 1.94   BTB	Add SPxHtmlCtrlDlgWin.h.
*			Add SPxHtmlCtrlPopupDlgWin.h.
*			Add SPxTrackLogDlgWin.h.
*			Add SPxTrackLogPopupDlgWin.h.
*   12/10/18 1.93   BTB	Add SPxTrackNoteDlgWin.h.
*			Add SPxTrackNotePopupDlgWin.h.
*   29/08/18 1.92   AGC	Add SPxCamCorrPopupDlgWin.h.
*   17/08/18 1.91   BTB	Add SPxCamCorrAdjDlgWin.h.
*			Add SPxCamCorrDlgWin.h.
*			Add SPxCamCorrPointDlgWin.h
*			Add SPxCamCorrPrevDlgWin.h
*			Add SPxCamCorrRendererWin.h
*			Add SPxCamCorrSelDlgWin.h
*   20/07/18 1.90   AGC	Add SPxP322ClientDlgWin.h.
*			Add SPxP322ClientPopupDlgWin.h.
*   07/06/18 1.89   BTB	Add SPxRemoteP226CtrlPopupDlg.h.
*			Add SPxRemoteRDRPopupDlg.h.
*   04/06/18 1.88   AGC	Add SPxRFEffectorDlgWin.h.
*			Add SPxRFEffectorPopupDlgWin.h.
*   31/05/18 1.87   BTB	Add SPxUACConfigDlg.h.
*   16/05/18 1.86   BTB	Add SPxRemoteP226CtrlDlg.h.
*   03/05/18 1.85   AGC	Add SPxTerrainStatusRendererD2D.h.
*			Add SPxTerrainStatusRendererWin.h.
*   25/04/18 1.84   BTB	Add SPxRemoteRDRDlg.h.
*   22/02/18 1.83   REW	Add SPxDFRendererWin.h.
*   07/02/18 1.82   AGC	Add SPxProcessMapDlgWin.h and Popup.
*   05/02/18 1.81   AGC	Add SPxWinImageDlg.
*   02/02/18 1.80   AGC	Add SPxCoverageColourDlgWin.
*   09/01/18 1.79   SP 	Add SPxRFTargetRendererWin.h
*   18/10/17 1.78   SP 	Remove SPxRefPointRendererWin.h
*                       Add SPxAlarmRendererWin.h
*   18/10/17 1.77   AGC	Remove SPxWinRestartApp.h
*   22/08/17 1.76   SP	Add SPxRefPointRendererWin.h
*   03/08/17 1.75   SP 	Add SPxJoystickWin.h.
*   29/06/17 1.74   SP 	Add SPxWinCheckboxTreeCtrl.h.
*   12/04/17 1.73   AGC	Add SPxComInit.h.
*			    Add SPxComPtr.h.
*   17/11/16 1.72   AGC	Add SPxWinCustomDlg.h.
*   16/09/16 1.71   SP 	Add SPxSystemTrayDialog.h.
*   14/07/16 1.70   SP 	Add SPxSQLBuilderDlgWin.h.
*   14/07/16 1.69   AGC	Add SPxWinRestartApp.h.
*   15/06/16 1.68   AGC	Add SPxCoverageDlgWin.h.
*   29/04/16 1.67   SP 	Add SPxLanguageDlgWin.h.
*   08/04/16 1.66   SP 	Add SPxSQLQueryDlgWin.h.
*   30/11/15 1.65   REW	Add SPxTextEntryDlgWin.h and Popup.
*   26/11/15 1.64   AJH	Add SPxP172EmuDlgWin.h
*   13/11/15 1.63   REW	Add SPxPlotRendererWin.h
*   09/11/15 1.62   AGC	Add SPxTableDlgWin.h
*   06/11/15 1.61   REW	Add SPxPimCtrlDlgWin.h and Popup.
*   22/07/15 1.60   REW	Add SPxAboutDlgWin.h
*   14/04/15 1.59   AGC	Add SPxParamDlgWin.h
*   26/03/15 1.58   SP 	Add SPxSerialCtrlDlgWin.
*   06/03/15 1.57   AGC	Add SPxProcCtrlPopupDlgWin.h
*   06/03/15 1.56   SP 	Add SPxLevelIndicatorWin.h
*   05/02/15 1.55   AGC	Add SPxWinD2DUtils.h
*   09/01/15 1.54   REW	Add SPxSrcNetCtrlDlgWin.h for AGC.
*   07/01/15 1.53   SP 	Add SPxRecordRendererWin.h
*   21/11/14 1.52   SP 	Add SPxRhiRendererWin.h
*   22/09/14 1.51   REW	Add SPxSafetyNetRendererWin.h
*   20/06/14 1.50   AGC	Add SPxWinFileDialog.h.
*			Add SPxWinSplashDlg.h.
*   23/04/14 1.49   AGC	Add SPxWinMsgBox.h.
*			Add SPxWinTaskbarButton.h.
*   26/03/14 1.48   AGC	Add SPxProcCtrlDlgWin.h.
*   17/02/14 1.47   AGC	Add SPxCamCtrlDlgWin and SPxCamCtrlPopupDlgWin.
*			Add SPxCamCtrlAdvDlgWin.
*   21/01/14 1.46   SP 	Add SPxSeparatorWin.h.
*   15/01/14 1.45   SP 	Add SPxWinSliderCtrl.h.
*   15/01/14 1.44   REW	Add SPxFlightPlan dialog files.
*   22/11/13 1.43   AGC	Add SPxNetCtrlDlgWin.h.
*   04/11/13 1.42   SP 	Add SPxAlertRendererWin.h.
*                       Add SPxTiledMapDlgWin.h.
*                       Add SPxTiledMapPopupDlgWin.h.
*   21/10/13 1.41   AGC	Add SPxConfigObjDlgWin.h.
*			Add SPxHealthDlgWin.h.
*			Add SPxRendererDlgWin.h.
*			Add SPxServerDlgWin.h.
*			Add SPxTableLayoutWin.h.
*   07/10/13 1.40   AGC	Add SPxAreaEditorDlgWin.h.
*			Add SPxExtRendererWin.h.
*			Add SPx...RendererD2D.h.
*			Sort headers alphabetically.
*   04/10/13 1.39   AGC	Move functions to SPxWinInit.h.
*   05/09/13 1.38   SP 	Add SPxRasterMapRendererWin.h.
*   14/08/13 1.37   SP 	Add SPxThreatRendererWin.h.
*   01/08/13 1.36   SP 	Add SPxWinMenu.h.
*   25/07/13 1.35   AGC	Add SPxWinImageCtrl.h.
*   25/07/13 1.34   REW	Add SPxROCCtrlDlgWin and SPxROCCtrlPopupDlgWin.
*   12/12/12 1.33   REW	Add SPxP226CtrlDlgWin.h
*   23/11/12 1.32   AGC	Add SPxMouseControlWin.h.
*   09/11/12 1.31   AGC	Add SPxViewControlRendererWin.h.
*   26/10/12 1.30   AGC	Move MFC utilities to SPxWinMfcUtils.h.
*			Add SPxSim... re-usable dialogs.
*			Add SPxSrcFile... re-usable dialogs.
*			Add SPxTestGen... re-usable dialogs.
*   18/10/12 1.29   SP 	Add SPxP172Rc... headers.
*                       Add new combobox utility functions.
*                       Add SPxSetWindowText().
*   21/06/12 1.28   SP 	Add SPxTiledMapRendererWin.h.
*   15/05/12 1.27   REW	Move utils to SPxWin32Utils.h
*   04/04/12 1.26   AGC	Add SPxAutoDC.
*   29/03/12 1.25   AGC	Add SPxScenGenCtrlPopupDlgWin.
*			Add SPxScDestDisplayMixer.
*   21/02/12 1.24   REW	Re-include ENC headers.
*   13/02/12 1.23   REW	Comment out ENC headers until dependencies fixed.
*   08/02/12 1.22   SP 	Add SPxENCManagerWin and SPxENCRenderWin.
*			Add SPxOwnShipRendererWin.
*			Add SPxAreaRendererWin.
*			Add SPxWorldMapRendererWin.
*			Add SPxDDX_Slider.
*   13/01/12 1.21   AGC	Add SPxScDestDisplayWinRaw and SPxScFollowWinRaw.
*   23/12/11 1.20   REW	Add SPxADSBRendererWin.h
*   25/10/11 1.19   AGC	Add SPxScenGenCtrlDlgWin.h.
*   07/07/11 1.18   SP 	Add SPxGetMaxScreenSize().
*   26/05/11 1.17   SP 	Add SPX_OMIT_MFC protection around recent code.
*   25/05/11 1.16   SP 	Add SPxHPxAdvCtrlDlgWin.h and
*			SPxHPxAdvCtrlPopupDlgWin.h.
*			Add MFC utils prototypes.
*   12/05/11 1.15   SP 	Add SPxHPxCtrlDlgWin.h and
*                       SPxHPxCtrlPopupDlgWin.h.
*   26/04/11 1.14   SP 	Add SPxGraphicsRendererWin.h
*   11/03/11 1.13   SP 	Add SPxTrackRendererWin.h
*   02/03/11 1.12   REW	Add SPxAISRendererWin.h
*   17/12/10 1.11   AGC	Rename SPxWinToggleBtn to SPxWinButton.
*   13/12/10 1.10   AGC	Add SPxWinToggleBtn.
*   08/12/10 1.9    AGC	Add SPxWinSpinCtrl, SPxWinEditCtrl and 
*			SPxWinNumEditCtrl.
*   30/09/10 1.8    SP 	Include SPxBitmapWin if SPX_OMIT_MFC.
*   21/09/10 1.7    SP 	Add SPxFlushWinMsgQueue()
*   28/07/10 1.6    SP 	Add SPxBitmapWin.h, SPxGraphicsWindow.h 
*			and SPxRadarWindow.h
*   23/04/10 1.5    SP 	Add SPxIsWin64Bit global.
*   09/07/07 1.4    REW	Args to SPxLibWinInit().
*   19/06/07 1.3    DGJ	DestRender now DestDisplay.
*   13/06/07 1.2    DGJ	DestDisplay now DestRender, ScGeom now ScFollow
*   24/05/07 1.1    REW	Initial Version.
**********************************************************************/

#ifndef _SPX_LIBWIN_H_
#define	_SPX_LIBWIN_H_

/* Include the public headers from this library. */

/* MFC based classes */
#ifndef SPX_OMIT_MFC
#include "SPxAboutDlgWin.h"
#include "SPxAeroRendererD2D.h"
#include "SPxAeroRendererWin.h"
#include "SPxADSBMonDlgWin.h"
#include "SPxADSBRendererD2D.h"
#include "SPxADSBRendererWin.h"
#include "SPxAISMonDlgWin.h"
#include "SPxAISMonPopupDlgWin.h"
#include "SPxAISRendererD2D.h"
#include "SPxAISRendererWin.h"
#include "SPxAlarmRendererD2D.h"
#include "SPxAlarmRendererWin.h"
#include "SPxAlertRendererD2D.h"
#include "SPxAlertRendererWin.h"
#include "SPxAreaEditorDlgWin.h"
#include "SPxAreaRendererD2D.h"
#include "SPxAreaRendererWin.h"
#include "SPxCamCorrAdjDlgWin.h"
#include "SPxCamCorrDlgWin.h"
#include "SPxCamCorrPopupDlgWin.h"
#include "SPxCamCorrPointDlgWin.h"
#include "SPxCamCorrPrevDlgWin.h"
#include "SPxCamCorrRendererD2D.h"
#include "SPxCamCorrRendererWin.h"
#include "SPxCamCorrSelDlgWin.h"
#include "SPxCamCtrlAdvDlgWin.h"
#include "SPxCamCtrlDlgWin.h"
#include "SPxCamCtrlPopupDlgWin.h"
#include "SPxCamFOVCalDlgWin.h"
#include "SPxCamFOVCalPopupDlgWin.h"
#include "SPxCamFOVCalPrevDlgWin.h"
#include "SPxCamFOVCalSelDlgWin.h"
#include "SPxCamFOVCalStgsDlgWin.h"
#include "SPxCamRecordDlgWin.h"
#include "SPxCamRecordPopupDlgWin.h"
#include "SPxCamTourDlgWin.h"
#include "SPxCamTourPopupDlgWin.h"
#include "SPxConfigObjDlgWin.h"
#include "SPxCoverageDlgWin.h"
#include "SPxCoverageColourDlgWin.h"
#include "SPxCoveragePopupDlgWin.h"
#include "SPxDetectedObjRendererD2D.h"
#include "SPxDisclaimerDlgWin.h"
#include "SPxDFRendererD2D.h"
#include "SPxDFRendererWin.h"
#include "SPxENCManagerWin.h"
#include "SPxENCRendererWin.h"
#include "SPxExtRendererD2D.h"
#include "SPxExtRendererWin.h"
#include "SPxFlightPlanDbDlgWin.h"
#include "SPxFlightPlanDbPopupDlgWin.h"
#include "SPxFlightPlanDlgWin.h"
#include "SPxFlightPlanPopupDlgWin.h"
#include "SPxGraphicsRendererD2D.h"
#include "SPxGraphicsRendererWin.h"
#include "SPxGraphicsWindow.h"
#include "SPxHealthDlgWin.h"
#include "SPxHelpBtnDlgWin.h"
#include "SPxHistogramRendererD2D.h"
#include "SPxHPxAdvCtrlDlgWin.h"
#include "SPxHPxAdvCtrlPopupDlgWin.h"
#include "SPxHPxCtrlDlgWin.h"
#include "SPxHPxCtrlPopupDlgWin.h"
#include "SPxHtmlCtrlDlgWin.h"
#include "SPxHtmlCtrlPopupDlgWin.h"
#include "SPxImageD2D.h"
#include "SPxInfoDBDispDlgWin.h"
#include "SPxInfoDBFieldEditDlgWin.h"
#include "SPxInfoDBInputDlgWin.h"
#include "SPxInfoDBItemEditDlgWin.h"
#include "SPxInfoDBItemEditPopupDlgWin.h"
#include "SPxInfoDBSequenceInputDlgWin.h"
#include "SPxJoystickWin.h"
#include "SPxLanguageDlgWin.h"
#include "SPxLevelIndicatorWin.h"
#include "SPxLRADSearchlightDlgWin.h"
#include "SPxMacroButton.h"
#include "SPxMacroBtnDlgWin.h"
#include "SPxMacroBtnPopupDlgWin.h"
#include "SPxManualTrackerDlgWin.h"
#include "SPxManualTrackerPopupDlgWin.h"
#include "SPxManualTrackerSensorInfoDlgWin.h"
#include "SPxMIL2525DDisplayDlgWin.h"
#include "SPxModeSCoverageRendererD2D.h"
#include "SPxModeSCoverageRendererWin.h"
#include "SPxMonitorAlertsDlgWin.h"
#include "SPxMonitorAlertsPopupDlgWin.h"
#include "SPxMouseControlWin.h"
#include "SPxNavMonDlgWin.h"
#include "SPxNavMonPopupDlgWin.h"
#include "SPxNetCtrlDlgWin.h"
#include "SPxOwnShipRendererD2D.h"
#include "SPxOwnShipRendererWin.h"
#include "SPxParamDlgWin.h"
#include "SPxP172EmuDlgWin.h"
#include "SPxP172RcDlgWin.h"
#include "SPxP172RcInterfaceWin.h"
#include "SPxP172RcPopupDlgWin.h"
#include "SPxP172RcTransportWin.h"
#include "SPxP226CtrlDlgWin.h"
#include "SPxP322ClientDlgWin.h"
#include "SPxP322ClientPopupDlgWin.h"
#include "SPxP395CtrlDlgWin.h"
#include "SPxP395CtrlPopupDlgWin.h"
#include "SPxP408CtrlDlgWin.h"
#include "SPxP430CtrlDlgWin.h"
#include "SPxP435CtrlDlgWin.h"
#include "SPxP437CtrlDlgWin.h"
#include "SPxP488CtrlDlgWin.h"
#include "SPxP489CtrlDlgWin.h"
#include "SPxPimCtrlDlgWin.h"
#include "SPxPimCtrlPopupDlgWin.h"
#include "SPxPlotRendererD2D.h"
#include "SPxPlotRendererWin.h"
#include "SPxProcCtrlDlgWin.h"
#include "SPxProcCtrlPopupDlgWin.h"
#include "SPxProcessMapDlgWin.h"
#include "SPxProcessMapPopupDlgWin.h"
#include "SPxROCAdvCtrlDlgWin.h"
#include "SPxROCAdvCtrlPopupDlgWin.h"
#include "SPxROCCtrlDlgWin.h"
#include "SPxROCCtrlPopupDlgWin.h"
#include "SPxRadarWindow.h"
#include "SPxRadarWindowD2D.h"
#include "SPxRangeRingsCfgDlgWin.h"
#include "SPxRasterMapRendererD2D.h"
#include "SPxRasterMapRendererWin.h"
#include "SPxRemoteHPxCtrlDlgWin.h"
#include "SPxRemoteHPxCtrlPopupDlgWin.h"
#include "SPxRemoteP226CtrlDlgWin.h"
#include "SPxRemoteP226CtrlPopupDlgWin.h"
#include "SPxRemoteRadarCtrlDlgWin.h"
#include "SPxRemoteRadarCtrlPopupDlgWin.h"
#include "SPxRemoteRDRDlgWin.h"
#include "SPxRemoteRDRPopupDlgWin.h"
#include "SPxRecordRendererWin.h"
#include "SPxRendererDlgWin.h"
#include "SPxRFEffectorDlgWin.h"
#include "SPxRFEffectorPopupDlgWin.h"
#include "SPxRFTargetRendererWin.h"
#include "SPxRhiWin.h"
#include "SPxRhiGraphicsRendererD2D.h"
#include "SPxRhiGraphicsRendererWin.h"
#include "SPxRhiTrackRendererD2D.h"
#include "SPxRhiTrackRendererWin.h"
#include "SPxSafetyNetRendererD2D.h"
#include "SPxSafetyNetRendererWin.h"
#include "SPxScDestBitmapD2D.h"
#include "SPxScDestDisplayWin.h"
#include "SPxScFollowWin.h"
#include "SPxScenGenCtrlDlgWin.h"
#include "SPxScenGenCtrlPopupDlgWin.h"
#include "SPxScrollDlgWin.h"
#include "SPxSerialCtrlDlgWin.h"
#include "SPxServerDlgWin.h"
#include "SPxSeparatorWin.h"
#include "SPxSharedFeatureDlgWin.h"
#include "SPxSimMotionCtrlDlgWin.h"
#include "SPxSimMotionCtrlPopupDlgWin.h"
#include "SPxSimMotionNewDlgWin.h"
#include "SPxSimMotionRendererD2D.h"
#include "SPxSimMotionRendererWin.h"
#include "SPxSimRadarCtrlDlgWin.h"
#include "SPxSimRadarCtrlPopupDlgWin.h"
#include "SPxSimTargetCtrlDlgWin.h"
#include "SPxSimTargetCtrlPopupDlgWin.h"
#include "SPxSimTerrainCtrlDlgWin.h"
#include "SPxSimTerrainCtrlPopupDlgWin.h"
#include "SPxSimTerrainServerCtrlDlgWin.h"
#include "SPxSimTerrainServerCtrlPopupDlgWin.h"
#include "SPxSiteRendererWin.h"
#include "SPxSQLBuilderDlgWin.h"
#include "SPxSQLQueryDlgWin.h"
#include "SPxSrcFileCtrlDlgWin.h"
#include "SPxSrcFileCtrlPopupDlgWin.h"
#include "SPxSrcNetCtrlDlgWin.h"
#include "SPxSystemTrayApp.h"
#include "SPxSystemTrayDialog.h"
#include "SPxTableDlgWin.h"
#include "SPxTableLayoutWin.h"
#include "SPxTerrainStatusRendererD2D.h"
#include "SPxTerrainStatusRendererWin.h"
#include "SPxTestGenCtrlDlgWin.h"
#include "SPxTestGenCtrlPopupDlgWin.h"
#include "SPxTextEntryDlgWin.h"
#include "SPxTextEntryPopupDlgWin.h"
#include "SPxThreatRendererD2D.h"
#include "SPxThreatRendererWin.h"
#include "SPxTiledMapRendererD2D.h"
#include "SPxTiledMapRendererWin.h"
#include "SPxTiledMapDlgWin.h"
#include "SPxTiledMapPopupDlgWin.h"
#include "SPxTimelineRendererWin.h"
#include "SPxTrackFiltersDlgWin.h"
#include "SPxTrackFiltersPopupDlgWin.h"
#include "SPxTrackGroupEditDlgWin.h"
#include "SPxTrackGroupEditPopupDlgWin.h"
#include "SPxTrackLogDlgWin.h"
#include "SPxTrackLogPopupDlgWin.h"
#include "SPxTrackNoteDlgWin.h"
#include "SPxTrackNotePopupDlgWin.h"
#include "SPxTrackRendererD2D.h"
#include "SPxTrackRendererWin.h"
#include "SPxUACConfigDlgWin.h"
#include "SPxUpdateDlgWin.h"
#include "SPxViewControlRendererD2D.h"
#include "SPxViewControlRendererWin.h"
#include "SPxWeatherConfigDlgWin.h"
#include "SPxWeatherConfigPopupDlgWin.h"
#include "SPxWeatherRendererD2D.h"
#include "SPxWeatherRendererWin.h"
#include "SPxWinAscanWindow.h"
#include "SPxWinButton.h"
#include "SPxWinCheckboxTreeCtrl.h"
#include "SPxWinCustomDlg.h"
#include "SPxWinD2DUtils.h"
#include "SPxWinEditCtrl.h"
#include "SPxWinFileDialog.h"
#include "SPxWinImageCtrl.h"
#include "SPxWinImageDlg.h"
#include "SPxWinListCtrl.h"
#include "SPxWinMenu.h"
#include "SPxWinMsgBox.h"
#include "SPxWinNumEditCtrl.h"
#include "SPxWinSliderCtrl.h"
#include "SPxWinSpinCtrl.h"
#include "SPxWinSplashDlg.h"
#include "SPxWinTaskbarButton.h"
#include "SPxWorldMapRendererD2D.h"
#include "SPxWorldMapRendererWin.h"
#endif

/* Non-MFC classes */
#include "SPxAutoDC.h"
#include "SPxBitmapWin.h"
#include "SPxComInit.h"
#include "SPxComPtr.h"
#include "SPxScDestDisplayWinRaw.h"
#include "SPxScDestDisplayMixer.h"
#include "SPxScFollowWinRaw.h"
#include "SPxWin32Utils.h"
#include "SPxWinD2DUtils.h"
#include "SPxWinInit.h"

/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#ifndef SPX_OMIT_MFC

#include "SPxWinMfcUtils.h"

#endif /* NOT SPX_OMIT_MFC */

/*********************************************************************
*
*   Public Variables
*
**********************************************************************/

#endif /* _SPX_LIBWIN_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/

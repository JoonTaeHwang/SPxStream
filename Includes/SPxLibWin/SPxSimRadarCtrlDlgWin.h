/*********************************************************************
*
* (c) Copyright 2012 - 2022, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for the Simulated Radar control child dialog.
*
*
* Revision Control:
*   02/09/22 v1.34   AGC	Supress cppcheck duplicate inherited member warning.
*
* Previous Changes:
*   01/09/22 1.33   AGC	Support weather configuration.
*   19/02/21 1.32   AGC	Support doppler configuration.
*   24/04/19 1.31   AGC	Improve multi-channel support.
*   25/06/18 1.30   AGC	Correctly suppress cppcheck warning.
*   23/01/18 1.29   AGC	Support multiple objects.
*   31/01/17 1.28   AGC	Support units configuration.
*			Support multiple platforms.
*   29/11/16 1.27   AGC	Support muting radar.
*   14/07/16 1.26   AGC	Support more radar video data types.
*   03/03/16 1.25   AGC	Support various radar data types.
*   01/02/16 1.24   AGC	Show/hide MTI dialogs.
*   11/01/16 1.23   AGC	Add MTI dialogs.
*   01/12/15 1.22   AGC	Use project code to disable some controls.
*			Radar now automatically clears destination.
*   26/11/15 1.21   AGC	Move content to child dialogs.
*   22/10/15 1.20   AGC	Support sector scan.
*   11/12/14 1.19   AGC	Allow any SPxScDestDisplay.
*   14/10/14 1.18   AGC	Support encoding IFF mode in video.
*   18/07/14 1.17   AGC	Support IFF.
*   04/07/14 1.16   AGC	Add radar visibility map.
*   20/06/14 1.15   AGC	Clear display when radar disabled.
*   02/06/14 1.14   AGC	Support sender ID, radar name and enable checkbox.
*   16/05/14 1.13   AGC	Support hiding obscured targets independent of terrain settings.
*   08/05/14 1.12   AGC	Support start range.
*   21/01/14 1.11   AGC	Improved spin control handlers.
*   15/01/14 1.10   AGC	Position spin controls in resource file.
*   19/07/13 1.9    AGC	Rename upside down option to anticlockwise.
*   06/06/13 1.8    AGC	Support upside-down radars.
*   03/05/13 1.7    AGC	Support static radar height above ground.
*   28/03/13 1.6    AGC	Support sea clutter.
*			Use slider for sensitivity.
*   18/03/13 1.5    AGC	Support minimum beam height and sensitivity.
*   08/03/13 1.4    AGC	Support static radars.
*			Support use within tabbed dialog.
*   25/01/13 1.3    AGC	Add index to iteration.
*			Position now controlled by platform.
*			Add offsets from position.
*   09/11/12 1.2    AGC	Add azimuth/range noise controls.
*			Remove nav data config button.
*			Rename BeamwidthV to maxBeamAngle.
*   26/10/12 1.1    AGC	Initial version.
**********************************************************************/

#pragma once

/* Standard headers. */

/* Include the SPx resource header */
#include "SPxResource.h"

#include "SPxLibUtils/SPxError.h"
#include "SPxLibData/SPxSimRadar.h"
#include "SPxLibWin/SPxWinEditCtrl.h"
#include "SPxLibWin/SPxWinSpinCtrl.h"
#include "SPxLibWin/SPxWinButton.h"
#include "SPxLibWin/SPxTableDlgWin.h"

/* Forward declarations. */
class SPxSimRadar;
class SPxScSourceLocal;
class SPxScDestDisplay;
class SPxSimMotion;
class SPxRadarSimulator;
class SPxSimRadarVidTypeCtrlDlgWin;
class SPxSimRadarPositionCtrlDlgWin;
class SPxSimRadarPropCtrlDlgWin;
class SPxSimRadarNoiseCtrlDlgWin;
class SPxSimRadarVisMapCtrlDlgWin;
class SPxSimRadarMtiCtrlDlgWin;
class SPxSimRadarDopplerCtrlDlgWin;
class SPxSimRadarIffCtrlDlgWin;
class SPxSimRadarBlindSectorCtrlDlgWin;
class SPxSimRadarWeatherCtrlDlgWin;
class SPxGraphicsRenderer;

/*
 * SPxSimRadarCtrlDlgWin dialog
 */
class SPxSimRadarCtrlDlgWin : public SPxTableDlgWin
{
    DECLARE_DYNAMIC(SPxSimRadarCtrlDlgWin)

public:
    /* Constructor and destructor. */
    explicit SPxSimRadarCtrlDlgWin(CWnd* pParent,
				   unsigned int numObjects,
				   SPxSimRadar *obj[],
				   SPxScSourceLocal *sc[]=NULL,
				   SPxScDestDisplay *dest[]=NULL,
				   SPxGraphicsRenderer *gfxRenderer[]=NULL,
				   unsigned int projectCode=110);
    virtual ~SPxSimRadarCtrlDlgWin(void);
    virtual BOOL Create(void);
    virtual void SyncFromObj(void);

    void SetDisplayUnits(SPxUnitsDist_t unitsDist,
			 SPxUnitsSpeed_t unitsSpeed);

    void ShowHideItems(void);

    /* Dialog Data */
    enum { IDD = IDD_SPX_SIM_RADAR_CTRL_DIALOG };
    virtual int GetIDD(void) const { return IDD; }

    /* Configuration retrieval functions. */
    SPxSimRadar *GetObj(void) { return m_obj; }
    // cppcheck-suppress virtualCallInConstructor
    virtual void SetObjIndex(int index);

protected:
    virtual void DoDataExchange(CDataExchange* pDX); 
    virtual BOOL OnInitDialog(void);
    virtual void OnOK(void){ /* Don't call the CDialog handler */ };
    virtual void OnCancel(void){ /* Don't call the CDialog handler */ };
    void OnTimer(UINT_PTR nIDEvent);
    DECLARE_MESSAGE_MAP()

private:
    /* Private variables. */
    struct impl;
    // cppcheck-suppress duplInheritedMember
    SPxAutoPtr<impl> m_p;

    /* The parent window */
    CWnd *m_parentWin;

    /* The object we are controlling */
    SPxSimRadar *m_obj;
    SPxScSourceLocal *m_sc;
    SPxGraphicsRenderer *m_gfxRenderer;
    const unsigned int m_projectCode;
    UINT32 m_syncFlags;
    SPxUnitsDist_t m_unitsDist;
    SPxUnitsSpeed_t m_unitsSpeed;

    /* Controls. */
    CButton m_checkEnable;
    CButton m_checkMute;
    SPxWinEditCtrl m_editName;
    CComboBox m_comboDataType;
    CStatic m_lblVidTypes;

    SPxAutoPtr<SPxSimRadarVidTypeCtrlDlgWin> m_vidTypeDlg;
    SPxAutoPtr<SPxSimRadarPositionCtrlDlgWin> m_positionDlg;
    SPxAutoPtr<SPxSimRadarPropCtrlDlgWin> m_propertiesDlg;
    SPxAutoPtr<SPxSimRadarNoiseCtrlDlgWin> m_noiseDlg;
    SPxAutoPtr<SPxSimRadarVisMapCtrlDlgWin> m_visMapDlg;
    SPxAutoPtr<SPxSimRadarIffCtrlDlgWin> m_iffDlg;
    SPxAutoPtr<SPxSimRadarMtiCtrlDlgWin> m_mtiDlg;
    SPxAutoPtr<SPxSimRadarNoiseCtrlDlgWin> m_mtiNoiseDlg;
    SPxAutoPtr<SPxSimRadarDopplerCtrlDlgWin> m_dopplerDlg;
    SPxAutoPtr<SPxSimRadarBlindSectorCtrlDlgWin> m_blindSectorDlg;
    SPxAutoPtr<SPxSimRadarWeatherCtrlDlgWin> m_weatherDlg;

    /* Private functions */

    /*
     * Handler functions.
     */
    void OnCheckEnable(void);
    void OnCheckMute(void);
    void OnEditName(NMHDR *, LRESULT *);
    void OnChangeDataType(void);
    static SPxErrorCode changeFn(SPxSimRadar *radar,
				 UINT32 flags,
				 void *userArg);

}; /* class SPxSimRadarCtrlDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/

/*********************************************************************
*
* (c) Copyright 2015 - 2018, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for the Simulated Radar noise control child dialog.
*
*
* Revision Control:
*   25/06/18 v1.9    AGC	Correctly suppress cppcheck warning.
*
* Previous Changes:
*   23/01/18 1.8    AGC	Support multiple objects.
*   15/03/17 1.7    AGC	Support target size noise.
*   31/01/17 1.6    AGC	Support units configuration.
*   04/01/17 1.5    AGC	Support enhanced target noise.
*   03/03/16 1.4    AGC	Fix clang warnings.
*   11/01/16 1.3    AGC	Noise now controlled by SPxSimRadarNoise object.
*   01/12/15 1.2    AGC	Disable background noise in P172 mode.
*   26/11/15 1.1    AGC	Initial version.
**********************************************************************/

#pragma once

/* Standard headers. */

/* Include the SPx resource header */
#include "SPxResource.h"

/* For SPxWinSpinCtrl. */
#include "SPxLibWin/SPxWinSpinCtrl.h"

/* For SPxWinEditCtrl. */
#include "SPxLibWin/SPxWinEditCtrl.h"

/* For SPxTabbedItemDlg base class. */
#include "SPxLibWin/SPxTabbedDlgWin.h"

/* Forward declarations. */
class SPxSimRadar;
class SPxSimRadarNoise;

/*
 * SPxSimRadarNoiseCtrlDlgWin dialog
 */
class SPxSimRadarNoiseCtrlDlgWin : public SPxTabbedItemDlg
{
    DECLARE_DYNAMIC(SPxSimRadarNoiseCtrlDlgWin)

public:
    /* Constructor and destructor. */
    explicit SPxSimRadarNoiseCtrlDlgWin(CWnd* pParent,
					unsigned int numObjects,
					SPxSimRadar *obj[],
					SPxSimRadarNoise *noise[],
					unsigned int projectCode,
					const SPxSimRadarVideoType videoTypes[],
					const unsigned int numVideoTypes,
					int showTgtNoise);
    virtual ~SPxSimRadarNoiseCtrlDlgWin(void);
    BOOL Create(void);
    virtual void SyncFromObj(void) { SyncFromObj(NULL); }
    void SyncFromObj(const char *newTargetNoiseName);

    void SetDisplayUnits(SPxUnitsDist_t unitsDist);

    /* Dialog Data */
    enum { IDD = IDD_SPX_SIM_RADAR_NOISE_CTRL_DIALOG };
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
    void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar);
    DECLARE_MESSAGE_MAP()

private:
    /* Private variables. */
    struct impl;
    SPxAutoPtr<impl> m_p;
    CWnd *m_parentWin;
    SPxSimRadar *m_obj;
    SPxUnitsDist_t m_unitsDist;
    SPxSimRadarNoise *m_noise;
    const unsigned int m_projectCode;
    static const unsigned int MAX_NUM_VIDEO_TYPES = 8;
    SPxSimRadarVideoType m_videoTypes[MAX_NUM_VIDEO_TYPES];
    unsigned int m_numVideoTypes;
    const int m_showTgtNoise;
    CStringA m_currTgtNoiseName;
    int m_maxTgtNoiseItemWidth;

    CSliderCtrl m_sliderNoiseBackground;
    CStatic m_staticNoiseBackground;
    CButton m_checkNoiseNear;
    SPxWinSpinCtrl m_spinNoiseNearGain;
    CStatic m_lblNoiseNearRange;
    SPxWinSpinCtrl m_spinNoiseNearRange;
    CSliderCtrl m_sliderSeaClutter;
    CStatic m_lblSeaClutter;
    CSliderCtrl m_sliderNoiseNearVar;
    CStatic m_lblNoiseNearVar;
    CStatic m_groupTgtNoise;
    CButton m_btnTgtNoiseAdd;
    CButton m_btnTgtNoiseRemove;
    CListBox m_listTgtNoise;
    CStatic m_lblTgtNoiseName;
    SPxWinEditCtrl m_editTgtNoiseName;
    CStatic m_lblTgtNoiseDist;
    CComboBox m_comboTgtNoiseDist;
    CStatic m_lblAziNoiseMean;
    CStatic m_lblAziNoiseStdDev;
    CStatic m_lblRangeNoiseMean;
    CStatic m_lblRangeNoiseStdDev;
    SPxWinSpinCtrl m_spinAziNoiseMean;
    SPxWinSpinCtrl m_spinAziNoiseStdDev;
    SPxWinSpinCtrl m_spinRangeNoiseMean;
    SPxWinSpinCtrl m_spinRangeNoiseStdDev;
    CStatic m_lblNoiseTgtDegs;
    CStatic m_lblNoiseTgtDegs2;
    CStatic m_lblNoiseTgtM;
    CStatic m_lblNoiseTgtM2;
    CStatic m_groupTgtNoiseSize;
    CStatic m_lblLengthNoiseStdDev;
    CStatic m_lblBreadthNoiseStdDev;
    SPxWinSpinCtrl m_spinLengthNoiseStdDev;
    SPxWinSpinCtrl m_spinBreadthNoiseStdDev;
    CStatic m_lblNoiseTgtM3;
    CStatic m_lblNoiseTgtM4;

    /* Private functions */
    void OnCheckNoiseNear(void);
    void OnSpinChangeNoiseNearGain(NMHDR *, LRESULT *);
    void OnSpinChangeNoiseNearRange(NMHDR *, LRESULT *);
    void OnTargetNoiseAdd(void);
    void OnTargetNoiseRemove(void);
    void OnTargetNoiseListChange(void);
    void OnEditTargetNoiseName(NMHDR*, LRESULT*);
    void OnTargetNoiseDistChange(void);
    void OnSpinChangeAziNoiseMean(NMHDR *, LRESULT *);
    void OnSpinChangeAziNoiseStdDev(NMHDR *, LRESULT *);
    void OnSpinChangeRangeNoiseMean(NMHDR *, LRESULT *);
    void OnSpinChangeRangeNoiseStdDev(NMHDR *, LRESULT *);
    void OnSpinChangeLengthNoiseStdDev(NMHDR *, LRESULT *);
    void OnSpinChangeBreadthNoiseStdDev(NMHDR *, LRESULT *);
    static SPxErrorCode tgtNoiseFnWrapper(void *userArg, const char *name);
    SPxErrorCode tgtNoiseFn(const char *name);
    CString getSelNoiseName(void) const;

    using SPxTabbedItemDlg::Create;

}; /* class SPxSimRadarNoiseCtrlDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/

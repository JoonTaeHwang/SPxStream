/*********************************************************************
*
* (c) Copyright 2014 - 2022, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for the Simulated Target Template control child dialog.
*
*
* Revision Control:
*   06/07/22 v1.10   AGC	Support intensity above background noise.
*
* Previous Changes:
*   11/03/22 1.9    AGC	Use icons for buttons.
*   26/05/17 1.8    AGC	Add dialog retrieval functions.
*   31/01/17 1.7    AGC	Support units configuration.
*   23/08/16 1.6    AGC	Event function prototype updated.
*   28/07/16 1.5    AGC	Remove unused variable/function.
*   03/03/16 1.4    AGC	Fix clang warnings.
*   07/11/14 1.3    AGC	Update radar sim event handler.
*   02/06/14 1.2    AGC	Allow external object to add new template.
*   16/05/14 1.1    AGC	Initial version.
**********************************************************************/

#pragma once

/* Standard headers. */

/* Include the SPx resource header */
#include "SPxResource.h"

/* For SPxErrorCode. */
#include "SPxLibUtils/SPxError.h"

/* For SPxObjPtr. */
#include "SPxLibUtils/SPxObjPtr.h"

/* For SPxSimTargetTemplate. */
#include "SPxLibData/SPxSimTargetTemplate.h"

/* For SPxWinButton. */
#include "SPxLibWin/SPxWinButton.h"

/* For SPxWinEditCtrl. */
#include "SPxLibWin/SPxWinEditCtrl.h"

/* For SPxWinSpinCtrl. */
#include "SPxLibWin/SPxWinSpinCtrl.h"

/* For SPxSimTargetShapeCtrlPopupDlgWin dialog. */
#include "SPxSimTargetShapeCtrlPopupDlgWin.h"

/* Forward declarations. */
class SPxRadarSimulator;
class SPxSimTargetTemplate;
struct SPxRadarSimEventParams;

/*
 * SPxSimTgtTemplateCtrlDlgWin dialog
 */
class SPxSimTgtTemplateCtrlDlgWin : public CDialog
{
    DECLARE_DYNAMIC(SPxSimTgtTemplateCtrlDlgWin)

public:
    /* Constructor and destructor. */
    SPxSimTgtTemplateCtrlDlgWin(CWnd* pParent, 
				SPxRadarSimulator *obj);
    virtual ~SPxSimTgtTemplateCtrlDlgWin(void);
    BOOL Create(void);
    void SyncFromObj(int updateAll=TRUE);
    SPxErrorCode SetCurrTemplate(SPxSimTargetTemplate *tgtTemplate);
    void AddTemplate(void);

    void SetDisplayUnits(SPxUnitsDist_t unitsDist);

    CWnd *GetShapeDlg(void);

    /* Dialog Data */
    enum { IDD = IDD_SPX_SIM_TGT_TEMPLATE_CTRL_DIALOG };

    /* Configuration retrieval functions. */
    SPxRadarSimulator *GetObj(void) { return m_obj; }

protected:
    virtual void DoDataExchange(CDataExchange* pDX); 
    virtual BOOL OnInitDialog(void);
    void OnTimer(UINT_PTR nIDEvent);
    virtual void OnOK(void){ /* Don't call the CDialog handler */ };
    virtual void OnCancel(void);
    virtual INT_PTR OnToolHitTest(CPoint point, TOOLINFO* pTI) const;
    BOOL OnNeedToolTip(UINT id, NMHDR *pTTTStruct, LRESULT *pResult);
    DECLARE_MESSAGE_MAP()

private:
    /* Private variables. */

    /* The parent window */
    CWnd *m_parentWin;
    
    /* The object we are controlling */
    SPxRadarSimulator *m_obj;
    SPxObjPtr<SPxSimTargetTemplate> m_currTemplate;
    SPxUnitsDist_t m_unitsDist;

    /* Controls. */
    CButton m_btnAdd;
    CButton m_btnDelete;
    CButton m_btnDeleteAll;
    CComboBox m_comboTemplates;
    SPxWinEditCtrl m_editName;
    CStatic m_lblSet;
    CStatic m_lblProperty;
    CButton m_setPrimary;
    CButton m_setTrack;
    CButton m_setMultilat;
    CButton m_setAis;
    CButton m_setAdsb;
    CButton m_setLength;
    CButton m_setBreadth;
    CButton m_setIntensityType;
    CButton m_setShape;
    CButton m_checkPrimary;
    CButton m_checkTrack;
    CButton m_checkMultilat;
    CButton m_checkAis;
    CButton m_checkAdsb;
    CStatic m_lblLength;
    SPxWinSpinCtrl m_spinLength;
    CStatic m_lblBreadth;
    SPxWinSpinCtrl m_spinBreadth;
    CButton m_radioRcs;
    CButton m_radioAboveBackground;
    CStatic m_lblRcs;
    SPxWinSpinCtrl m_spinRcs;
    SPxWinSpinCtrl m_spinAboveBackground;
    SPxWinButton m_btnCfgShape;
    SPxSimTargetShapeCtrlPopupDlgWin<SPxSimTargetTemplate> m_shapeDlg;

    /* Other variables. */
    int m_syncRequired;
    CFont m_underlineFont;

    /* Private functions */
    static SPxErrorCode addTemplateToCombo(unsigned int index, SPxSimTargetTemplate *tgtTemplate, void *userArg);
    static SPxErrorCode eventFnWrapper(UINT32 flags, const SPxRadarSimEventParams *params, void *userArg);
    SPxErrorCode eventFn(UINT32 flags);
    void changeFlags(const CButton &btn, UINT32 flags);

    /* Handler functions. */
    void OnBnClickedDelete(void);
    void OnBnClickedDeleteAll(void);
    void OnChangeTemplate(void);
    void OnSetName(NMHDR*, LRESULT*);
    void OnBnClickedSetPrimary(void);
    void OnBnClickedSetTrack(void);
    void OnBnClickedSetMultilat(void);
    void OnBnClickedSetAis(void);
    void OnBnClickedSetAdsb(void);
    void OnBnClickedSetLength(void);
    void OnBnClickedSetBreadth(void);
    void OnBnClickedSetIntensityType(void);
    void OnBnClickedSetShape(void);
    void OnBnClickedCheckPrimary(void);
    void OnBnClickedCheckTrack(void);
    void OnBnClickedCheckMultilat(void);
    void OnBnClickedCheckAis(void);
    void OnBnClickedCheckAdsb(void);
    void OnBnClickedRadioRcs(void);
    void OnBnClickedRadioAboveBackground(void);
    void OnSpinChangeLength(NMHDR*, LRESULT*);
    void OnSpinChangeBreadth(NMHDR*, LRESULT*);
    void OnSpinChangeRcs(NMHDR*, LRESULT*);
    void OnSpinChangeIntensityAboveBackground(NMHDR*, LRESULT*);
    void OnBnClickedCfgShape(void);

    using CDialog::Create;

}; /* class SPxSimTgtTemplateCtrlDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/

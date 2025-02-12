/*********************************************************************
 *
 * (c) Copyright 2020 - 2022, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   Header for dialog for controlling and editing camera tours.
 *
 * Revision Control:
 *   14/06/22 v1.5   BTB	Various improvements to controls and behaviour.
 *
 * Previous Changes:
 *   22/03/21 1.4   BTB	Support outside updates to current tour.
 *   08/07/20 1.3   AGC	Add drag/drop support.
 *   07/07/20 1.2   AGC	Add options to support RadarWatch usage.
 *   02/07/20 1.1   AGC	Initial version.
 *
 *********************************************************************/

#pragma once

/* For SPxAutoPtr. */
#include "SPxLibUtils/SPxAutoPtr.h"

/* For SPxWinListCtrl enums. */
#include "SPxLibWin/SPxWinListCtrl.h"

/* For SPxWinSpinCtrl. */
#include "SPxLibWin/SPxWinSpinCtrl.h"

/* Include the base class header. */
#include "SPxTabbedDlgWin.h"

/* Forward declarations. */
class SPxCamera;

class SPxCamTourDlgWin : public SPxTabbedItemDlg
{
public:

    /* Dialog Data. */
    enum { IDD = IDD_SPX_CAM_TOUR_DIALOG };

    /* Construction and destruction. */
    SPxCamTourDlgWin(void);
    virtual ~SPxCamTourDlgWin(void);

    /* General. */
    virtual SPxErrorCode SetCamera(SPxCamera *camera);

    void EnableEdit(int enable);
    void SetAutoEnabled(int autoEnabled);
    void SetEditAccessControlled(int editAccessControlled);
    void SetUseBearingUserOffset(int useBearingUserOffset);
    void SetUseElevationUserOffset(int useElevationUserOffset);

    virtual void SyncFromObj(void);

    virtual int GetIDD(void) const { return IDD; }

protected:
    /*
     * Protected variables.
     */

    /*
     * Protected functions.
     */

    DECLARE_MESSAGE_MAP()
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog(void);
    virtual BOOL PreTranslateMessage(MSG *pMsg);

    /* GUI callback functions. */
    virtual void OnTimer(UINT_PTR nIDEvent);

private:
    /*
     * Private variables.
     */

    CToolTipCtrl m_toolTip;    /* Used to implement tool tips. */

    /* Child controls. */
    CButton m_tourCheck;
    CButton m_editCheck;
    CButton m_saveBn;
    CButton m_reloadBn;
    CButton m_appendBn;
    CButton m_deleteBn;
    CButton m_deleteAllBn;
    SPxWinListCtrl m_list;
    CStatic m_waitLabel;
    SPxWinSpinCtrl m_waitSpin;

    /* General. */
    SPxCamera *m_camera;    /* Camera to control. */
    int m_isAutoEnabled;
    int m_editAccessControlled;
    int m_useBearingUserOffset;
    int m_useElevationUserOffset;
    int m_stageUpdateRequired;
    int m_rowSelectOn;

    /*
     * Private functions.
     */

     /* General. */
    void updateStages(void);
    void syncGUIFromTimer(void);
    void onRowSelect(int row, SPxWinListCtrl::SelectState_t state);
    void onRowDragDrop(int row, int insertBeforeRow);
    void onPopupMenuCreate(int row, CMenu *menu);
    void onPopupMenuSelect(int row, int selection);
    void goToSelectedStage(void);
    void appendStage(void);
    void insertStage(unsigned int stageIndex);
    void deleteStage(unsigned int stageIndex);
    void deleteAllStages(void);
    void handleTourUpdate(void);

    /* GUI callback functions. */
    LRESULT OnSpinChange(WPARAM wParam, LPARAM lParam);
    void OnBnClickedTourCheck(void);
    void OnBnClickedEditCheck(void);
    void OnBnClickedSaveButton(void);
    void OnBnClickedReloadButton(void);
    void OnBnClickedAppendButton(void);
    void OnBnClickedDeleteButton(void);
    void OnBnClickedDeleteAllButton(void);

    /*
     * Private static functions.
     */

    static void rowSelectHandler(void *userArg,
				 int row,
				 SPxWinListCtrl::SelectState_t state);
    static void rowDragDropHandler(void *userArg, int row, int insertBeforeRow);
    static void popupMenuCreateHandler(void *userArg, int row, CMenu *menu);
    static void popupMenuSelectHandler(void *userArg, int row, int selection);
    static int tourUpdateHandler(void *invokingObj,
				 void *userArg,
				 void *arg);

};

/*********************************************************************
 *
 * End of file
 *
 **********************************************************************/

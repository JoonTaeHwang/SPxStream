/*********************************************************************
*
* (c) Copyright 2014 - 2020, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for the Terrain Database Server control child dialog,
*	this dialog is typically a child of SPxSimTerrainServerCtrlDlgWin.
*
*
* Revision Control:
*   06/10/20 v1.8    AGC	Support loading folders and files.
*
* Previous Changes:
*   11/08/20 1.7    AGC	Disable controls while updating custom URL.
*   04/08/20 1.6    AGC	Support custom terrain sources.
*   07/05/19 1.5    AGC	Support min/max latitude for server.
*   03/03/16 1.4    AGC	Fix clang warnings.
*   14/10/14 1.3    AGC	Undo some of last change to fix build issues.
*   14/10/14 1.2    AGC	Support multiple terrain servers.
*   27/01/14 1.1    AGC	Initial version.
**********************************************************************/

#pragma once

/* Standard headers. */

/* Include the SPx resource header */
#include "SPxResource.h"

/* For SPxAutoPtr. */
#include "SPxLibUtils/SPxAutoPtr.h"

/* For SPxErrorCode. */
#include "SPxLibUtils/SPxError.h"

/* For SPxWinButton. */
#include "SPxLibWin/SPxWinButton.h"

/* For SPxWinEditCtrl. */
#include "SPxLibWin/SPxWinEditCtrl.h"

/* For SPxWinSpinCtrl. */
#include "SPxLibWin/SPxWinSpinCtrl.h"

/* Forward declarations. */
class SPxTerrainDatabase;

/*
 * SPxSimTerrainServerCtrlDlgWin dialog
 */
class SPxSimTerrainServerCtrlDlgWin : public CDialog
{
    DECLARE_DYNAMIC(SPxSimTerrainServerCtrlDlgWin)

public:
    /* Constructor and destructor. */
    SPxSimTerrainServerCtrlDlgWin(CWnd* pParent, 
				  SPxTerrainDatabase *obj);
    virtual ~SPxSimTerrainServerCtrlDlgWin(void);
    BOOL Create(void);
    virtual void SyncFromObj(void);

    /* Dialog Data */
    enum { IDD = IDD_SPX_SIM_TERRAIN_SERVER_CTRL_DIALOG };

    /* Configuration retrieval functions. */
    SPxTerrainDatabase *GetObj(void) { return m_obj; }

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
    SPxAutoPtr<impl> m_p;

    /* The parent window */
    CWnd *m_parentWin;

    /* The objects we are controlling */
    SPxTerrainDatabase *m_obj;

    /* Controls. */
    CListBox m_listServers;
    CButton m_btnAdd;
    CButton m_btnRemove;
    SPxWinButton m_btnUp;
    SPxWinButton m_btnDown;
    CComboBox m_comboServer;
    CStatic m_lblServer;
    SPxWinEditCtrl m_editServer;
    CButton m_btnOpenServerFile;
    CButton m_btnOpenServerFolder;
    CStatic m_staticCacheSize;
    CStatic m_lblLatMin;
    CStatic m_lblLatMax;
    SPxWinSpinCtrl m_spinLatMin;
    SPxWinSpinCtrl m_spinLatMax;

    /* Other variables. */
    int m_recalcSize;

    /*
     * Handler functions.
     */
    void OnListServerSelChange(void);
    void OnBtnAdd(void);
    void OnBtnRemove(void);
    void OnBtnUp(void);
    void OnBtnDown(void);
    void OnComboServerSelChange(void);
    void OnBtnServerSet(NMHDR*, LRESULT*);
    void OnBtnOpenServerFile(void);
    void OnBtnOpenServerFolder(void);
    void OnSpinLatMinChange(NMHDR*, LRESULT*);
    void OnSpinLatMaxChange(NMHDR*, LRESULT*);
    void OnBtnDelCache(void);

    void openServer(int isFolder);
    SPxErrorCode setServerURL(void);
    SPxErrorCode completeServerURL(void);

    UINT64 calcDirSize(const char *path);
    static SPxErrorCode terrainEventFnWrapper(UINT32 flags, void *userArg);
    SPxErrorCode terrainEventFn(UINT32 flags);

    using CDialog::Create;

}; /* class SPxSimTerrainServerCtrlDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/

/*********************************************************************
*
* (c) Copyright 2020-2021, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for the AIS monitor track table child dialog.
*
*
* Revision Control:
*   11/02/21 v1.4    AJH	Add database to constructor.
*
* Previous Changes:
*   29/09/20 1.3    AJH	Hide range/bearing if no position available.
*   21/09/20 1.2    AJH	Support track filtering and standalone mode.
*   18/09/20 1.1    AJH	Initial Version.
**********************************************************************/

#pragma once

/* Standard headers. */

/* Include the SPx resource header */
#include "SPxResource.h"

#include "SPxLibUtils/SPxError.h"
#include "SPxLibUtils/SPxAutoPtr.h"
#include "SPxLibWin/SPxWinEditCtrl.h"

/* Forward declarations. */
class SPxAISDecoder;
class SPxAISTrack;
class SPxWorldMap;
class SPxUniTrack;
class SPxUniTrackDatabase;
class SPxTrackTableDlgWin;

/*
 * SPxAISTrackDlgWin dialog
 */
class SPxAISTrackDlgWin : public CDialog
{
    DECLARE_DYNAMIC(SPxAISTrackDlgWin)

public:
    /* Public types. */
    typedef SPxErrorCode(*TrackSelectCallback)( SPxAISTrack *track, void *userArg );
    typedef SPxErrorCode(*TrackShowCallback)( SPxAISTrack *track, void *userArg );
    typedef int(*TrackVisibleCallback)( SPxAISTrack *track, void *userArg );

    /* Constructor and destructor. */
    explicit SPxAISTrackDlgWin(CWnd* pParent, SPxAISDecoder *obj, SPxUniTrackDatabase *db=NULL);
    virtual ~SPxAISTrackDlgWin(void);
    virtual BOOL Create(void);
    virtual void SyncFromObj(void);

    void SetWorldMap(SPxWorldMap *worldMap) { m_worldMap = worldMap; }
    void SetStandalone(int enabled) { m_standalone = enabled; }
    SPxErrorCode SetTrackSelectCallback(TrackSelectCallback trackSelectFn, void *userArg);
    SPxErrorCode SetTrackShowCallback(TrackShowCallback trackShowFn, void *userArg);
    SPxErrorCode SetTrackVisibleCallback(TrackVisibleCallback trackVisibleFn, void *userArg);

    /* Dialog Data */
    enum { IDD = IDD_SPX_AIS_TRACK_DIALOG };
    virtual int GetIDD(void) const { return IDD; }

    /* Configuration retrieval functions. */
    SPxAISDecoder *GetObj(void) { return m_obj; }

protected:
    virtual void DoDataExchange(CDataExchange* pDX); 
    virtual BOOL OnInitDialog(void);
    virtual void OnOK(void){ /* Don't call the CDialog handler */ };
    virtual void OnCancel(void){ /* Don't call the CDialog handler */ };
    DECLARE_MESSAGE_MAP()

private:
    /*
     * Private variables.
     */
    /* The parent window */
    CWnd *m_parentWin;

    /* The object we are controlling */
    SPxAISDecoder *m_obj;

    /* Options. */
    SPxWorldMap *m_worldMap;
    TrackSelectCallback m_trackSelectFn;
    void *m_trackSelectUserArg;
    TrackShowCallback m_trackShowFn;
    void *m_trackShowUserArg;
    TrackVisibleCallback m_trackVisibleFn;
    void *m_trackVisibleUserArg;
    UINT32 m_trackTableMenuTrackID;
    BOOL m_standalone;
    unsigned int m_mmsiFilter;
    int m_colRange;
    int m_colBearing;

    /* The track database. */
    SPxUniTrackDatabase *m_uniTrackDB;
    SPxAutoPtr<SPxUniTrackDatabase> m_uniTrackDBAuto;

    /* MFC controls. */
    SPxAutoPtr<SPxTrackTableDlgWin> m_trackDlg;	    /* Track table */
    SPxWinEditCtrl m_editMMSI;

    /* GUI data exchange. */
    int m_visible;

    /*
     * Private functions
     */
    /* GUI functions. */
    void onVisible();
    void onEditMMSI(NMHDR *, LRESULT *);

    /* Callback handlers. */
    static SPxErrorCode getRefPointStatic(double *refLatDegs, double *refLongDegs, void *obj);
    SPxErrorCode getRefPoint(double *refLatDegs, double *refLongDegs);
    static int trackIncludeCallbackStatic(SPxUniTrack *track, void *userArg);
    int trackIncludeCallback(SPxUniTrack *track);
    static SPxErrorCode trackSelectCallbackStatic(UINT32 trackID,
	int doubleClick, void *userArg);
    SPxErrorCode trackSelectCallback(UINT32 trackID, int doubleClick);
    static void trackTableContextCreateStatic(SPxTrackTableDlgWin *dlg,
	int row, CMenu *menu, void *userArg);
    void trackTableContextCreate(SPxTrackTableDlgWin *dlg,
	int row, CMenu *menu);
    static void trackTableContextSelectStatic(SPxTrackTableDlgWin *dlg,
	int row, int selection, void *userArg);
    void trackTableContextSelect(SPxTrackTableDlgWin *dlg,
	int row, int selection);

    /* Avoid warnings about hiding overloaded virtual functions. */
    using CDialog::Create;

}; /* class SPxAISTrackDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/

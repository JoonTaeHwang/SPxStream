/*********************************************************************
*
* (c) Copyright 2020, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for the ADS-B monitor track table child dialog.
*
*
* Revision Control:
*   12/02/21 1.4    JGB	Pass Track DB through constructor.
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
class SPxADSBDecoder;
class SPxADSBTrack;
class SPxWorldMap;
class SPxUniTrack;
class SPxUniTrackDatabase;
class SPxTrackTableDlgWin;

/*
 * SPxADSBTrackDlgWin dialog
 */
class SPxADSBTrackDlgWin : public CDialog
{
    DECLARE_DYNAMIC(SPxADSBTrackDlgWin)

public:
    /* Public types. */
    typedef SPxErrorCode(*TrackSelectCallback)( SPxADSBTrack *track, void *userArg );
    typedef SPxErrorCode(*TrackShowCallback)( SPxADSBTrack *track, void *userArg );
    typedef int(*TrackVisibleCallback)( SPxADSBTrack *track, void *userArg );

    /* Constructor and destructor. */
    explicit SPxADSBTrackDlgWin(CWnd* pParent, SPxADSBDecoder *obj, SPxUniTrackDatabase* db = NULL);
    virtual ~SPxADSBTrackDlgWin(void);
    virtual BOOL Create(void);
    virtual void SyncFromObj(void);

    void SetWorldMap(SPxWorldMap *worldMap) { m_worldMap = worldMap; }
    void SetStandalone(int enabled) { m_standalone = enabled; }
    SPxErrorCode SetTrackSelectCallback(TrackSelectCallback trackSelectFn, void *userArg);
    SPxErrorCode SetTrackShowCallback(TrackShowCallback trackShowFn, void *userArg);
    SPxErrorCode SetTrackVisibleCallback(TrackVisibleCallback trackVisibleFn, void *userArg);

    /* Dialog Data */
    enum { IDD = IDD_SPX_ADSB_TRACK_DIALOG };
    virtual int GetIDD(void) const { return IDD; }

    /* Configuration retrieval functions. */
    SPxADSBDecoder *GetObj(void) { return m_obj; }

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
    SPxADSBDecoder *m_obj;

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
    unsigned int m_aaFilter;
    int m_colRange;
    int m_colBearing;

    /* The track database. */
    SPxUniTrackDatabase* m_uniTrackDB;
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
    void onEditAA(NMHDR *, LRESULT *);

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

}; /* class SPxADSBTrackDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/

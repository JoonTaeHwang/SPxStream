/*********************************************************************
 * (c) Copyright 2013 - 2020, 2022, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   Header file for SPxTiledMapDlgWin class.
 *
 * Revision Control:
 *  05/07/22 v1.14   AJH	Update server list when dialog opened.
 *
 * Previous Changes:
 *  29/10/20 1.13   REW	Add support for SkyVector Charts.
 *  21/10/20 1.12   SP 	Revert part of last change (N/C #802 still fixed).
 *  16/10/20 1.11   SP 	Fix N/C #802 - Don't recalc cache area unnecessarily.
 *  01/10/18 1.10   SP 	Add support for Google Maps.
 *  28/09/18 1.9    SP 	Add support for Bing Maps.
 *  11/05/16 1.8    SP 	Support multi-language.
 *  03/03/16 1.7    AGC Fix clang warnings.
 *  24/09/14 1.6    SP  Add additional Get...() functions.
 *  21/05/14 1.5    SP  Remove unused variables.
 *  16/05/14 1.4    SP  Support improvements to tiled map database
 *                      for custom and source object servers.
 *  01/05/14 1.3    SP  Add cache update button.
 *                      Add SetCacheArea().
 *  15/01/14 1.2    AGC Support any number of custom sources.
 *  04/11/13 1.1    SP  Initial version.
 *
 **********************************************************************/
#pragma once

/* Include the SPx resource header */
#include "SPxResource.h"

#include "SPxLibUtils/SPxAutoPtr.h"
#include "SPxLibUtils/SPxObj.h"

/* Forward declarations. */
class SPxTiledMapDatabase;
class SPxTiledMapSrc;
class SPxViewControl;

/*
 * SPxTiledMapDlgWin dialog.
 */
class SPxTiledMapDlgWin : public CDialog, public SPxObj
{
    DECLARE_DYNAMIC(SPxTiledMapDlgWin)

public:

    /*
     * Public functions.
     */

    SPxTiledMapDlgWin(CWnd *parentWin,
                      SPxTiledMapDatabase *tdb,
                      SPxViewControl *viewControl);
    virtual ~SPxTiledMapDlgWin(void);
    virtual BOOL Create(void);

    /* Access. */
    virtual SPxTiledMapDatabase *GetTiledMapDatabase(void) const;
    virtual SPxViewControl *GetViewControl(void) const;

    /* Set cache area. */
    virtual SPxErrorCode SetCacheArea(double latNDegs, double lonWDegs,
                                      double latSDegs, double lonEDegs,
                                      unsigned int widthPixels);

    /* Synchronise. */
    virtual void SyncFromObj(void);

    /* Standard parameter setting interface. */
    virtual int SetParameter(char *parameterName, char *parameterValue);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);

    /*
     * Public variables.
     */
    enum { IDD = IDD_SPX_TILED_MAP_DIALOG };

protected:

    /*
     * Protected functions.
     */

    void DoDataExchange(CDataExchange* pDX);
    BOOL OnInitDialog(void);
    void OnHScroll(UINT nSBCode, UINT nPos,
                   CScrollBar *pScrollBar);
    HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    void OnTimer(UINT_PTR nIDEvent);
    void OnShowWindow(BOOL bShow, UINT nStatus);
    void OnBnClickedCacheStartStopCheck(void);
    void OnBnClickedClearCacheButton(void);
    void OnBnClickedUpdateCacheButton(void);
    void OnCbnSelchangeServerCombo(void);
    void OnBnClickedBingSetKeyButton(void);
    void OnNMClickBingTermOfUseLink(NMHDR *pNMHDR, LRESULT *pResult);
    void OnBnClickedGoogleSetKeyButton(void);
    void OnNMClickGoogleTermOfUseLink(NMHDR *pNMHDR, LRESULT *pResult);
    void OnBnClickedSkyVectorSetKeyButton(void);
    DECLARE_MESSAGE_MAP()

private:

    /*
     * Public variables.
     */

    CWnd *m_parentWin;
    SPxTiledMapDatabase *m_tdb;
    SPxViewControl *m_viewCtrl;
    CStringA m_startLabel;
    CStringA m_stopLabel;
    BOOL m_updateAvailableServers;

    /* Current cache area. */
    BOOL m_isDownloadLimitExceeded;
    double m_cacheAreaLatNDegs;
    double m_cacheAreaLonWDegs;
    double m_cacheAreaLatSDegs;
    double m_cacheAreaLonEDegs;
    unsigned int m_cacheAreaWidthPixels;

    /* GUI data exchange variables. */
    double m_latNDegs;
    double m_lonWDegs;
    double m_latSDegs;
    double m_lonEDegs;
    int m_zoom;
    int m_minZoom;
    int m_maxZoom;
    UINT32 m_server;

    /*
     * Private functions.
     */
    SPxErrorCode UpdateCacheArea(void);
    void UpdateServer(void);

    using CDialog::Create;

}; /* SPxTiledMapDlgWin class */

/*********************************************************************
 *
 * End of file
 *
 **********************************************************************/

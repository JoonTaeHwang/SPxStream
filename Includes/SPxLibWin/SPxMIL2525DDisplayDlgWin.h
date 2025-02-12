/*********************************************************************
*
* (c) Copyright 2020, Cambridge Pixel Ltd.
*
* Purpose:
*	Dialog for MIL-2525-D display settings.
*
* Revision Control:
*   21/10/20 v1.4    AGC	Support managing multiple track renderers.
*
* Previous Changes:
*   06/10/20 1.3    AGC	Support info amplifier.
*   27/08/20 1.2    AGC	Update GetTabName().
*   26/08/20 1.1    AGC	Initial version.
**********************************************************************/

#pragma once

/*
 * Include the classes we need.
 */
#include "SPxTabbedItemDlg.h"

/*
 * Forward declare classes we use.
 */
class SPxTrackRenderer;

/*
 * SPxMIL2525DDisplayDlgWin dialog.
 */
class SPxMIL2525DDisplayDlgWin : public SPxTabbedItemDlg
{
    DECLARE_DYNAMIC(SPxMIL2525DDisplayDlgWin)

public:

    /* Dialog Data */
    enum { IDD = IDD_SPX_MIL2525D_DISP_DIALOG };

    /* Constructor and destructor. */
    explicit SPxMIL2525DDisplayDlgWin(void);
    virtual ~SPxMIL2525DDisplayDlgWin(void);
    virtual void SyncFromObj(void);
    virtual int GetIDD(void) const { return IDD; }
    virtual SPxErrorCode GetTabName(char *buf, int bufLen) const;

    void AddTrackRenderer(SPxTrackRenderer *trackRenderer);

protected:

    DECLARE_MESSAGE_MAP()
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog();

private:

    /* Misc. */
    HICON m_hIcon;                      /* Dialog icon. */

    /* Other objects. */
    struct impl;
    SPxAutoPtr<impl> m_p;

    /* GUI items. */
    CButton m_checkCompFrame;
    CButton m_checkCompFill;
    CButton m_checkCompIcon;
    CButton m_radioCompIconColour;
    CButton m_radioCompIconBW;
    CButton m_checkCompMod;
    CButton m_checkCompAmp;
    CButton m_checkAmpTN;
    CButton m_checkAmpIFF;
    CButton m_checkAmpSpeed;
    CButton m_checkAmpAltitude;
    CButton m_checkAmpInfo;
    CButton m_radioBrightLight;
    CButton m_radioBrightMedium;
    CButton m_radioBrightDark;
    CButton m_radioOpaque;
    CButton m_radioTransparent;
    CButton m_radioBackDark;
    CButton m_radioBackLight;
    CButton m_radioClutterNone;
    CButton m_radioClutterDots;

    void OnCheckCompFrame(void);
    void OnCheckCompFill(void);
    void OnCheckCompIcon(void);
    void OnClickCompIconColour(void);
    void OnClickCompIconBW(void);
    void OnCheckCompMod(void);
    void OnCheckCompAmp(void);
    void OnCheckAmpTN(void);
    void OnCheckAmpIFF(void);
    void OnCheckAmpSpeed(void);
    void OnCheckAmpAltitude(void);
    void OnCheckAmpInfo(void);
    void OnClickBrightness(void);
    void OnClickTransparency(void);
    void OnClickBackground(void);
    void OnClickClutter(void);

};

/*********************************************************************
*
*	End of file
*
**********************************************************************/

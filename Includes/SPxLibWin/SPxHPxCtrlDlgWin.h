/*********************************************************************
*
* (c) Copyright 2011 - 2018, 2022, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for the HPx control child dialog.
*
*
* Revision Control:
*   21/01/22 v1.16   AGC	Make range spin control behaviour consistent.
*
* Previous Changes:
*   25/06/18 1.15   AGC	Correctly suppress cppcheck warning.
*   23/01/18 1.14   AGC	Support multiple objects.
*   12/04/17 1.13   SP 	Major rework to support HPx-400 dual streams.
*   07/10/16 1.12   AGC	Support tabbed item dialog enable control.
*   03/03/16 1.11   AGC	Fix clang warnings.
*   05/06/15 1.10   SP 	Centralise enabling/disabling of controls.
*                       Handle OnEnable() to update child conrtols.
*   21/05/15 1.9    SP 	Add auto range control.
*   26/01/15 1.8    AGC	Remove unused HICON.
*   07/01/15 1.7    AGC	Derive from SPxTabbedItemDlg.
*   11/02/13 1.6    REW	Support auto gain/offset.
*   26/10/12 1.5    AGC	Add forward declarations.
*   07/07/11 1.4    AGC	Add SetIcon() function.
*   01/06/11 1.3    SP 	Allow -ve numbers in range correction CEdit.
*   25/05/11 1.2    SP 	Major rework to support HPx-200 card.
*   12/05/11 1.1    SP 	Initial version based on SPxWinCtrlSrcHPx.
*
**********************************************************************/

#pragma once

/* Include the SPx resource header */
#include "SPxResource.h"

/* For base class */
#include "SPxLibWin/SPxTabbedDlgWin.h"

/* For SPxWinSpinCtrl*/
#include "SPxLibWin/SPxWinSpinCtrl.h"

/* HPx card header. */
#include "SPxLibData/SPxHPx100Source.h"

/* Forward declarations. */
class SPxHPxAdvCtrlDlgWin;

/*
 * SPxWinCtrlSrcHPx dialog
 */
class SPxHPxCtrlDlgWin : public SPxTabbedItemDlg
{
    DECLARE_DYNAMIC(SPxHPxCtrlDlgWin)

public:
    /* Constructor and destructor. */
    SPxHPxCtrlDlgWin(CWnd* pParent, 
                     SPxHPx100Source *obj, 
                     BOOL showAdvCtrlBtn=TRUE);
    SPxHPxCtrlDlgWin(CWnd* pParent,
                     unsigned int numObjects,
                     SPxHPx100Source *obj[],
                     BOOL showAdvCtrlBtn=TRUE);
    virtual ~SPxHPxCtrlDlgWin();
    virtual BOOL Create(BOOL isDualModeFixed=FALSE, 
                        int fixedStreamIdx=-1);

    /* Dialog Data */
    enum { IDD = IDD_SPX_SRC_HPX_CTRL_DIALOG };
    virtual int GetIDD(void) const { return IDD; }

    /* Sync functions. */
    virtual void SyncFromObj(void);
    virtual void UpdateSignalStates(void);
    virtual void UpdateControlEnables(void);

    /* SPxTabbedItemDlg interface. */
    virtual int ShowCheck(void) { return TRUE; }
    virtual void Enable(int enable);
    virtual int GetEnable(void);
    virtual int HasObj(void) const { return (m_obj != NULL);  }

    /* Configuration retrieval functions. */
    virtual SPxHPx100Source *GetObj(void) { return m_obj; }
    // cppcheck-suppress virtualCallInConstructor
    virtual void SetObjIndex(int index);

    /* Misc functions. */
    virtual HICON SetIcon(HICON icon, BOOL bigIcon);

protected:
    virtual void DoDataExchange(CDataExchange* pDX); 
    virtual BOOL OnInitDialog();
    virtual void OnTimer(UINT_PTR nIDEvent);
    virtual void OnOK(void){ /* Don't call the CDialog handler */ };
    virtual void OnCancel(void){ /* Don't call the CDialog handler */ };
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    DECLARE_MESSAGE_MAP()

private:

    /* Private types */
    typedef struct
    {
        int lastEndRangeMetres;     /* Last configured end range. */

    } StreamContext_t;


    /* Private functions */
    void init(CWnd* pParent, BOOL showAdvCtrlBtn);

    /*
     * Handler functions.
     */

    afx_msg void OnEnable(BOOL bEnable);
    afx_msg void OnDualMode(void);
    afx_msg void OnStream(void);
    afx_msg void OnChannel(void);
    afx_msg void OnISMSrcTRG(void);
    afx_msg void OnISMSrcACP(void);
    afx_msg void OnISMSrcARP(void);
    afx_msg void OnAZI(void);
    afx_msg void OnARP(void);
    afx_msg void OnTRG(void);
    afx_msg void OnSIG1(void);
    afx_msg void OnSIG2(void);
    afx_msg void OnSIG3(void);
    afx_msg void OnERP(void);
    afx_msg void OnAziInterpolation(void);
    afx_msg void OnGainASpin(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnGainBSpin(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnOffASpin(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnOffBSpin(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnRangeSet(void);
    afx_msg void OnTPG(void);
    afx_msg void OnAlarmClear(void);
    afx_msg void OnAdvOptionsButton(void);
    afx_msg void OnAutoGainOffset(void);
    afx_msg void OnAutoRange(void);

    struct impl;
    SPxAutoPtr<impl> m_p;

    /* GUI control variables. */
    unsigned int m_dualMode;    /* Dual stream mode. */
    int m_streamIdx;            /* Current stream index (must be signed). */
    unsigned int m_chan;        /* Channel selection */
    unsigned int m_ismSrcTRG;   /* Trigger input signal mapping. */
    unsigned int m_ismSrcACP;   /* ACP input signal mapping. */
    unsigned int m_ismSrcARP;   /* ARP input signal mapping. */
    unsigned int m_trg;         /* Trigger selection */
    unsigned int m_azi;         /* Azimuth selection */
    unsigned int m_arp;         /* ARP selection */
    unsigned int m_sig1;        /* SIG1 selection. */
    unsigned int m_sig2;        /* SIG2 selection. */
    unsigned int m_sig3;        /* SIG3 selection. */
    BOOL m_trgInv;              /* Trigger inverted? */
    BOOL m_acpInv;              /* ACP inverted? */
    BOOL m_arpInv;              /* ARP inverted? */
    BOOL m_sig1Inv;             /* SIG1 inverted? */
    BOOL m_sig2Inv;             /* SIG2 inverted? */
    BOOL m_sig3Inv;             /* SIG3 inverted? */
    BOOL m_erpEnable;           /* End of Range Pulse enable */
    BOOL m_aziInterpEnable;     /* Azimuth interpolation enable. */
    unsigned int m_tpg;         /* TPG pattern */
    double m_gainA;             /* Video A gain */
    double m_gainB;             /* Video B gain */
    double m_offsetA;           /* Video A offset */
    double m_offsetB;           /* Video B offset */
    SPxWinSpinCtrl m_spinRangeStart; /* Start range in metres */
    SPxWinSpinCtrl m_spinRangeEnd;   /* End range in metres */
    SPxWinSpinCtrl m_spinRangeCorr;  /* Range correction in metres */
    int m_isAutoRange;          /* Auto range enabled? */

    /* The parent window */
    CWnd *m_parentWin;

    /* The object we are controlling */
    SPxHPx100Source *m_obj;

    /* Used to limit control of dual-stream functions. */
    BOOL m_isDualModeFixed;     /* Is dual mode control enabled. */
    int m_fixedStreamIdx;       /* Fixed stream index or -1 if not fixed. */

    /* The advanced HPx control dialog */
    SPxHPxAdvCtrlDlgWin *m_advCtrlDlg;
    BOOL m_showAdvCtrlBtn; /* True if available */

    /* Is an auto gain/offset in progress? */
    int m_autoGainOffsetInProgress;

    /* Stream specific variables. */
    StreamContext_t m_streams[SPX_HPX400_MAX_NUM_STREAMS];

    using CDialog::Create;

}; /* class SPxHPxCtrlDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/

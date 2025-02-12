/*********************************************************************
*
* (c) Copyright 2012 - 2022, Cambridge Pixel Ltd.
*
* Purpose:
*	A pop-up dialog used to set the start and stop 
*	positions of the four blanking sectors.
*
* Revision Control:
*   17/10/22 v1.10   SP 	Add m_statusLabel.
*
* Previous Changes:
*   12/10/22 1.9    SP 	Support sweep sectors on SPEXER 600.
*   08/06/20 1.8    SP 	Support syncing of controls.
*   11/05/16 1.7    SP 	Support multi-language.
*   26/11/15 1.6    SP 	Changes to support Mk11 radar.
*   26/01/15 1.5    AGC	Remove unused HICON.
*   26/11/13 1.4    AGC	Remove unused m_parentWin variable.
*   04/10/13 1.3    AGC	Simplify headers.
*   26/10/12 1.2    SP 	Further development.
*   18/10/12 1.1    SP 	Initial version.
*
**********************************************************************/

#pragma once

/* Include the SPx resource header */
#include "SPxResource.h"

/* Include the classes we need. */
#include "SPxLibWin/SPxP172RcBaseDlgWin.h"

/* Forward declarations. */
class SPxP172RcInterfaceWin;
class SPxBitmapWin;

/*
 * SPxP172RcSectorSizeDlgWin dialog.
 */
class SPxP172RcSectorSizeDlgWin : public SPxP172RcBaseDlgWin
{
    DECLARE_DYNAMIC(SPxP172RcSectorSizeDlgWin)

public:
    /* Constructor and destructor. */
    SPxP172RcSectorSizeDlgWin(SPxP172RcInterfaceWin *rdrIf, BOOL isXBand);
    virtual ~SPxP172RcSectorSizeDlgWin(void);
    virtual BOOL Create(CWnd* pParent);
    virtual BOOL Update(void);
    virtual void Sync(void);
    virtual void SendConfigSettings(void);
    SPxErrorCode SetStateFromConfig(void);
    SPxErrorCode SetConfigFromState(void);

    /* Dialog Data */
    enum { IDD = IDD_SPX_P172_RC_SECTOR_SIZE_DIALOG };

protected:
    DECLARE_MESSAGE_MAP()
    virtual void DoDataExchange(CDataExchange* pDX); 
    virtual BOOL OnInitDialog(void);
    virtual void OnOK(void){ OnBnClickedSectorSizeSetButton(); }
    virtual afx_msg void OnPaint(void);  
    virtual int SetParameter(char *name, char *value);
    virtual int GetParameter(char *name, char *valueBuf, int bufLen); 

private:

    /* 
     * Private types. 
     */

    /* Sector info. */
    typedef struct
    {
        CWnd *win;            /* Indicator window. */
        SPxBitmapWin *bitmap; /* Offscreen bitmap. */
        CDC *bitmapDC;        /* Offscreen graphics context. */
        Control_t startCtrl;  /* Start control. */
        Control_t stopCtrl;   /* Stop control. */
        const SPxP172RcInterfaceWin::SectorStatus_t *status; /* Sector status. */
        CStringA lastRange;   /* Last range mode for sweep sector. */

    } SectorInfo_t;

    /* 
     * Private functions. 
     */
    void DrawSectors(void);
    void DrawSector(unsigned int index);
    void SendBlankingTransmitSectorSetCommand(unsigned int index);
    void SendSweepSectorsSetCommand(void);
    void SendSweepSectorsResetCommand(void);
    const SPxP172RcInterfaceWin::SectorStatus_t *GetSectorStatus(unsigned int sectorNum);

    /* GUI callback functions. */
    afx_msg void OnBnClickedSectorSizeSetButton(void);
    afx_msg void OnBnClickedSectorSizeResetButton(void);

    /* 
     * Private variables. 
     */

    /* Brushes. */
    CBrush *m_bgBrush;
    CBrush *m_blackBrush;

    /* Pens. */
    CPen *m_bgPen;
    CPen *m_blackPen;
    CPen *m_statusOkPen;
    CPen *m_statusErrorPen;

    /* Fonts. */
    CFont *m_font;

    /* Sector information. */
    SectorInfo_t m_sectors[SPX_P172_NUM_SECTORS];

    /* Set/reset buttons. */
    CButton *m_setBn;
    CButton *m_resetBn;
    CStatic *m_statusLabel;

    /* Labels. */
    CStringA m_transmitLabel;
    CStringA m_blankingLabel;
    CStringA m_sweepLabel;
    CStringA m_sectorLabel;
    CStringA m_xBandLabel;
    CStringA m_sBandLabel;
    CStringA m_limitsLabel;
    CStringA m_sizeLabel;

}; /* class SPxP172RcSectorSizeDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/

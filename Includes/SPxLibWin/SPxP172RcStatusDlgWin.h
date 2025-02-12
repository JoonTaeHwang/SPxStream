/*********************************************************************
*
* (c) Copyright 2012 - 2020, Cambridge Pixel Ltd.
*
* Purpose:
*	A pop-up dialog used to show status information.
*
* Revision Control:
*   08/06/20 v1.7    SP 	Provide access to main dialog.
*
* Previous Changes:
*   22/03/19 1.6    SP 	Add support for SBS-900 status 13.
*                      	Automatically start new column when needed.
*   03/03/16 1.5    AGC	Fix clang warnings.
*   22/10/15 1.4    SP 	Support logging of status information.
*   26/01/15 1.3    AGC	Remove unused HICON and parent window.
*   26/10/12 1.2    SP 	Further development.
*   18/10/12 1.1    SP 	Initial version.
*
**********************************************************************/

#pragma once

/* Include the SPx resource header */
#include "SPxResource.h"

/* Include the classes we need. */
#include "SPxLibWin/SPxP172RcInterfaceWin.h"
#include "SPxLibWin/SPxP172RcStatusFieldDlgWin.h"
#include "SPxLibWin/SPxP172RcHelpDlgWin.h"

/* Forward declare classes. */
class SPxP172RcDlgWin;

/*
 * SPxP172RcStatusDlgWin dialog.
 */
class SPxP172RcStatusDlgWin : public CDialog
{
    DECLARE_DYNAMIC(SPxP172RcStatusDlgWin)

public:

    /* 
     * Public types. 
     */

    /* Unit to display status for. */
    typedef enum
    {
        /* RDU. */
        UNIT_TYPE_RDU,

        /* Active or X-band transceiver. */
        UNIT_TYPE_ACTIVE_X_TXRX,

        /* Reserve or S-band transceiver. */
        UNIT_TYPE_RESERVE_S_TXRX,

        /* Miscellanous - antenna, waveguide, etc. */
        UNIT_TYPE_MISC,

    } UnitType_t;

    /* 
     * Public functions. 
     */

    /* Constructor and destructor. */
    SPxP172RcStatusDlgWin(SPxP172RcDlgWin *mainDlg,
                          UnitType_t unitType,
                          SPxP172RcHelpDlgWin *helpDlg);
    virtual ~SPxP172RcStatusDlgWin();
    virtual BOOL Create(CWnd* pParent);

    /* Update and get status. */
    virtual BOOL Update(void);

    /* 
     * Public variables. 
     */

    /* Dialog Data */
    enum { IDD = IDD_SPX_P172_RC_STATUS_DIALOG };

protected:
    DECLARE_MESSAGE_MAP()
    virtual void DoDataExchange(CDataExchange* pDX); 
    virtual BOOL OnInitDialog();

private:

    /* 
     * Private types. 
     */

    /* Field information. */
    typedef struct _FieldInfo_t
    {
        /* Field dialog. */
        SPxP172RcStatusFieldDlgWin *dlg;

        /* Next entry in the list or NULL. */
        _FieldInfo_t *nextPtr;
    
    } FieldInfo_t;

    /* 
     * Private functions. 
     */

    void AddFieldDialog(const SPxP172RcInterfaceWin::StatusField_t *statusField,
                        const char *groupName,
                        const char *unitName);

    using CDialog::Create;

    /* 
     * Private variables. 
     */

    /* Dialog lables. */
    const char *m_txrxLabel;
    const char *m_miscLabel;
    const char *m_rduLabel;
    const char *m_xBandLabel;
    const char *m_sBandLabel;
    const char *m_activeLabel;
    const char *m_reserveLabel;
    const char *m_statusLabel;

    /* The unit associated with this dialog. */
    UnitType_t m_unitType;

    /* The main dialog. */
    SPxP172RcDlgWin *m_mainDlg;

    /* The radar interface. */
    SPxP172RcInterfaceWin *m_interface;

    /* Field info list. */
    FieldInfo_t *m_fieldListHead;

    /* Position of next field dialog. */
    int m_nextFieldDlgXIdx;
    int m_nextFieldDlgYIdx;
    int m_maxNumRows;

    /* Size of dialog client area to encompass all fields. */
    int m_clientWidth;
    int m_clientHeight;

    /* Dialog used to display help info. */
    SPxP172RcHelpDlgWin *m_helpDlg;

    /* Status fields that may be added at runtime. */
    BOOL m_isStatus13Added;

}; /* class SPxP172RcStatusDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/

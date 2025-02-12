/*********************************************************************
*
* (c) Copyright 2018 - 2019, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for the Nav Data Sentences child dialog.
*
*
* Revision Control:
*   21/01/19 v1.6    REW	Support only showing received messages.
*
* Previous Changes:
*   02/01/19 1.5    REW	Support free mode restrictions.
*   21/12/18 1.4    REW	Rename Used to Decode.
*   11/12/18 1.3    REW	Implement stats display etc.
*   06/12/18 1.2    REW	Avoid compiler warning.
*   05/12/18 1.1    REW	Initial Version.
**********************************************************************/

#pragma once

/* Standard headers. */

/* Include the SPx resource header */
#include "SPxResource.h"

#include "SPxLibUtils/SPxError.h"
#include "SPxLibUtils/SPxAutoPtr.h"
#include "SPxLibData/SPxNavData.h"

/* Forward declarations. */
class SPxNavData;
class SPxNetCtrlDlgWin;
class SPxSerialCtrlDlgWin;
class SPxTableDlgWin;

/*
 * SPxNavSentencesDlgWin dialog
 */
class SPxNavSentencesDlgWin : public CDialog
{
    DECLARE_DYNAMIC(SPxNavSentencesDlgWin)

public:
    /* Constructor and destructor. */
    explicit SPxNavSentencesDlgWin(CWnd* pParent, SPxNavData *obj);
    virtual ~SPxNavSentencesDlgWin(void);
    virtual BOOL Create(void);
    virtual void SyncFromObj(void);

    /* Link dialog to a parent table dialog (if appropriate). */
    virtual void SetParentTableDlg(SPxTableDlgWin *tableDlg)
    {
	m_tableDlg = tableDlg;
    }

    /* Options. */
    void SetAllowDecodeCtrl(int allow);

    /* Dialog Data */
    enum { IDD = IDD_SPX_NAV_SENTENCES_DIALOG };
    virtual int GetIDD(void) const { return IDD; }

    /* Configuration retrieval functions. */
    SPxNavData *GetObj(void) { return m_obj; }

protected:
    virtual void DoDataExchange(CDataExchange* pDX); 
    virtual BOOL OnInitDialog(void);
    virtual void OnOK(void){ /* Don't call the CDialog handler */ };
    virtual void OnCancel(void){ /* Don't call the CDialog handler */ };
    virtual void OnDecode(UINT id);
    DECLARE_MESSAGE_MAP()

private:
    /*
     * Private variables.
     */
    /* The parent window */
    CWnd *m_parentWin;			/* Parent dialog */
    SPxTableDlgWin *m_tableDlg;		/* Table we're in, if any */

    /* The object we are controlling */
    SPxNavData *m_obj;

    /* Options. */
    int m_allowDecodeCtrl;

    /* Internal per-sentence info. */
    struct SPxNavSentencesDlgWinRow_tag *m_rows;
    unsigned int m_numRows;	/* How many rows are there? */
    unsigned int m_numTypesReceived;	/* How many types have we received? */

    /* MFC controls. */
    unsigned int m_nextCtrlID;		/* Control IDs to use */
    CButton m_buttonOnlyShow;		/* Button for only show received */

    /*
     * Private functions
     */
    void onBnClickedOnlyShow(void);
    void layout(void);

    /* Avoid warnings about hiding overloaded virtual functions. */
    using CDialog::Create;
}; /* class SPxNavSentencesDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/

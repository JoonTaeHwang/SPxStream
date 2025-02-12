/*********************************************************************
*
* (c) Copyright 2020, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for the AIS History child dialog.
*
*
* Revision Control:
*   21/09/20 v1.3    AJH	Support display/filtering of decoded messages.
*
* Previous Changes:
*   18/09/20 1.2    AJH	Fix headers.
*   07/09/20 1.1    AJH	Initial Version.
**********************************************************************/

#pragma once

/* Standard headers. */

/* Include the SPx resource header */
#include "SPxResource.h"

#include "SPxLibUtils/SPxError.h"
#include "SPxLibUtils/SPxTime.h"
#include "SPxLibUtils/SPxAutoPtr.h"
#include "SPxLibWin/SPxWinMessageCtrl.h"
#include "SPxLibWin/SPxWinEditCtrl.h"

/* Forward declarations. */
class SPxAISDecoder;
class SPxAISTrack;
class SPxPacketDecoder;

/*
 * SPxAISHistoryDlgWin dialog
 */
class SPxAISHistoryDlgWin : public CDialog
{
    DECLARE_DYNAMIC(SPxAISHistoryDlgWin)

public:
    /* Constructor and destructor. */
    explicit SPxAISHistoryDlgWin(CWnd* pParent, SPxAISDecoder *obj,
				 int showErrors);
    virtual ~SPxAISHistoryDlgWin(void);
    virtual BOOL Create(void);
    virtual void SyncFromObj(void);

    /* Dialog Data */
    enum { IDD = IDD_SPX_AIS_HISTORY_DIALOG };
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

    /* MFC controls. */
    SPxWinMessageCtrl m_msgWnd;		/* Window for messages */
    SPxWinEditCtrl m_editFilter;

    /* GUI data exchange. */
    int m_freezeWin;
    int m_logToFile;
    int m_decode;
    char m_filter[256];

    /*
     * Private functions
     */
    /* GUI functions. */
    void onFreeze();
    void onLogToFile();
    void onClear();
    void onDecode();
    void onEditFilter(NMHDR *, LRESULT *);

    /* Callback handler. */
    static void msgHandlerStatic(SPxPacketDecoder *packetDecoder,
	void *userArg,
	UINT32 packetType,
	struct sockaddr_in *from,
	SPxTime_t *timestamp,
	const unsigned char *payload,
	unsigned int numBytes);
    void msgHandler(const unsigned char *payload,
	unsigned int numBytes);
    static void decodedHandlerStatic(const char *msg, void *userArg);
    void decodedHandler(const char *msg);

    /* Avoid warnings about hiding overloaded virtual functions. */
    using CDialog::Create;
}; /* class SPxAISHistoryDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/

/*********************************************************************
 *
 * (c) Copyright 2022, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   Header for dialog for record and replay of camera positions.
 *
 * Revision Control:
 *   18/02/22 v1.1   AGC	Initial version.
 *
 * Previous Changes:
 *
 *********************************************************************/

#pragma once

#include "SPxLibWin/SPxWinButton.h"
#include "SPxLibWin/SPxWinEditCtrl.h"

/* Include the base class headers. */
#include "SPxLibUtils/SPxObj.h"
#include "SPxTabbedDlgWin.h"

/* Forward declarations. */
class SPxRecord;
class SPxPacketDecoderFile;
class SPxThread;

class SPxCamRecordDlgWin : public SPxTabbedItemDlg, public SPxObj
{
public:

    /* Dialog Data. */
    enum { IDD = IDD_SPX_CAM_RECORD_DIALOG };

    /* Construction and destruction. */
    explicit SPxCamRecordDlgWin(SPxRecord *camRecorder,
				SPxPacketDecoderFile *camReplay);
    virtual ~SPxCamRecordDlgWin(void);

    virtual void SyncFromObj(void);
    void SyncLabels(void);

    virtual int GetIDD(void) const { return IDD; }

    void StartRecord(void);
    void StopRecord(void);
    void ToggleRecord(void);
    int IsRecording(void) const;

    void StartReplay(void);
    void StopReplay(void);
    void ToggleReplay(void);
    int CanReplay(void) const;
    int IsReplaying(void) const;

    /* Generic parameter assignment. */
    virtual int SetParameter(char *parameterName, char *parameterValue);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);

protected:
    /*
     * Protected variables.
     */

    /*
     * Protected functions.
     */

    DECLARE_MESSAGE_MAP()
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog(void);

    /* GUI callback functions. */
    virtual void OnTimer(UINT_PTR nIDEvent);

private:
    /*
     * Private variables.
     */
    SPxRecord *m_camRecorder;
    SPxPacketDecoderFile *m_camReplay;
    SPxAutoPtr<SPxThread> m_replayThread;

    /* GUI elements */
    SPxWinEditCtrl m_editPrefix;
    SPxWinButton m_btnRecStart;
    SPxWinButton m_btnRecStop;
    CStatic m_lblRecFile;
    CStatic m_lblRecLen;
    SPxWinButton m_btnPlayStart;
    SPxWinButton m_btnPlayStop;
    CStatic m_lblPlayFile;
    CStatic m_lblPlayPos;
    CStatic m_lblPlayLen;

    /* Bitmaps */
    HICON m_iconPlay;
    HICON m_iconRec;
    HICON m_iconStop;

    /*
     * Private functions.
     */
    void ChangePrefix(NMHDR*, LRESULT*);
    SPxErrorCode OpenRecentRecordFile(void);

    static void* threadFuncWrapper(SPxThread *thread);
    void* threadFunc(SPxThread* thread);

};

/*********************************************************************
 *
 * End of file
 *
 **********************************************************************/

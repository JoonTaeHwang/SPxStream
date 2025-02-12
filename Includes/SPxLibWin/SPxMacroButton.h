/*********************************************************************
*
* (c) Copyright 2019, 2020, 2022, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for a macro button with configurable button and tooltip
*	text as well as a configured parameter file to be loaded
*	on clicking the button.
*
* Revision Control:
*   02/09/22 v1.12   AGC	Supress cppcheck duplicate inherited member warning.
*
* Previous Changes:
*   21/05/20 1.11  REW	Add link to parent macro button dialog.
*   17/03/20 1.10  AGC	Add public CheckState().
*   04/03/20 1.9   AGC  Support images.
*                       Support new parameter control options.
*   03/07/19 1.8   BTB	Support setting remote server commands.
*   16/04/19 1.7   BTB	Add parameter for specifying system commands.
*   15/04/19 1.6   BTB	Derive from SPxWinButton rather than CButton.
*   12/04/19 1.5   BTB	Support radio buttons.
*   12/04/19 1.4   BTB	Support resizing to button text.
*   11/04/19 1.3   BTB	Add parameter for specifying keyboard shortcuts.
*   10/04/19 1.2   BTB	Support checkbox buttons.
*   08/04/19 1.1   BTB	Initial Version.
**********************************************************************/

#pragma once

/* Standard headers. */

/* Library headers. */
#include "SPxLibUtils/SPxAutoPtr.h"
#include "SPxLibUtils/SPxObj.h"
#include "SPxLibWin/SPxWinButton.h"

/* Forward declarations. */
class SPxRemoteServer;
class SPxMacroBtnDlgWin;

/*
 * SPxMacroButton control
 */
class SPxMacroButton : public SPxWinButton, public SPxObj
{
public:
    /* Construction and desctruction. */
    SPxMacroButton(int manageShowState = FALSE, int manageEnableState = FALSE);
    virtual ~SPxMacroButton(void);

    virtual int Configured(void) const;

    void SetParentMacroBtnDlg(SPxMacroBtnDlgWin *dlg);

    virtual void SetIsShowStateManaged(int manage);
    virtual int GetIsShowStateManaged(void) { return m_manageShowState; }

    virtual void SetIsEnableStateManaged(int manage);
    virtual int GetIsEnableStateManaged(void) { return m_manageEnableState; }

    void EnableResizingToText(int enable) { m_resizeButtonToText = enable; }

    void InstallRemoteServer(SPxRemoteServer *server);

    SPxErrorCode Sync(void);
    SPxErrorCode CheckState(void);

    static SPxErrorCode AddImageResource(const char *name, UINT id);

    /* Parameter handling. */
    virtual int SetParameter(char *name, char *value);
    virtual int GetParameter(char *name, char *valueBuf, int bufLen);
    virtual SPxErrorCode SetStateFromConfig(void);

protected:
    DECLARE_MESSAGE_MAP()

    virtual void PreSubclassWindow(void);
    virtual BOOL PreTranslateMessage(MSG *pMsg);

    /* GUI callback functions. */
    virtual afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    virtual afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    virtual afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    virtual afx_msg void OnMouseLeave(void);

private:
    /* Private variables. */
    struct impl;
    // cppcheck-suppress duplInheritedMember
    SPxAutoPtr<impl> m_p;

    CToolTipCtrl m_toolTip; /* Used to implement tool tip for the button. */
    int m_manageShowState;  /* Wether the button is managing show/hide itself. */
    int m_manageEnableState;/* Whether the button is managing enable/disable itself. */
    int m_checked;	    /* Whether or not the button is checked. */
    int m_resizeButtonToText; /* Whether or not to resize the button to fit the text. */

    /* Private functions. */
    SPxErrorCode syncButton(void);
    SPxErrorCode checkState(void);
    void translateKeyboardInputString(const char *inputString);
    void runSystemCommand(const char *systemCommand);
    SPxErrorCode sendRemoteCommand(const char *commandValue);

}; /* class SPxMacroButton */

/*********************************************************************
*
* End of file
*
**********************************************************************/

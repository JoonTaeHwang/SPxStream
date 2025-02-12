/*********************************************************************
*
* (c) Copyright 2018 - 2019, 2022, Cambridge Pixel Ltd.
*
* Purpose:
*	Header file for a wrapper object to configure a button box button
*	to run a configurable command.
*
* Revision Control:
*   24/05/22 v1.7    REW	Support loading config files.
*
* Previous Changes:
*   09/05/19 1.6    REW	Support right clicks to edit a parameter value.
*   12/10/18 1.5    REW	Support changing labels with state.
*   11/10/18 1.4    REW	Support 2 and 3-state toggle buttons.
*   31/05/18 1.3    REW	Support system commands.
*   02/05/18 1.2    REW	Support remote commands.
*   01/05/18 1.1    REW	Initial Version.
**********************************************************************/

#ifndef _SPX_BUTTON_BOX_CMD_BUTTON_H
#define _SPX_BUTTON_BOX_CMD_BUTTON_H

/* For common definitions and base class. */
#include "SPxLibUtils/SPxCriticalSection.h"
#include "SPxLibUtils/SPxCommon.h"
#include "SPxLibUtils/SPxError.h"
#include "SPxLibUtils/SPxObj.h"

/*********************************************************************
*
*   Constants
*
**********************************************************************/


/*********************************************************************
*
*   Type definitions
*
**********************************************************************/

/* Forward declarations. */
class SPxButtonBoxButton;
class SPxRemoteServer;

/* Button box command button class. */
class SPxButtonBoxCmdButton : public SPxObj
{
public:
    /* Creation etc. */
    SPxButtonBoxCmdButton(SPxButtonBoxButton *button);
    virtual ~SPxButtonBoxCmdButton(void);

    /* Configuration. */
    SPxErrorCode SetLabel(const char *label);
    const char *GetLabel(void) const		{ return(m_label); }
    SPxErrorCode SetLabel2(const char *label);
    const char *GetLabel2(void) const		{ return(m_label2); }
    SPxErrorCode SetLabel3(const char *label);
    const char *GetLabel3(void) const		{ return(m_label3); }
    SPxErrorCode SetLabelUnchecked(const char *label);
    const char *GetLabelUnchecked(void) const	{ return(m_labelUnchecked); }
    SPxErrorCode SetState(int state);
    int GetState(void) const			{ return(m_state); }
    SPxErrorCode SetType(int type);
    int GetType(void) const			{ return(m_type); }
    SPxErrorCode SetCheckedObject(const char *obj);
    const char *GetCheckedObject(void) const	{ return(m_checkedObject); }
    SPxErrorCode SetCheckedParam(const char *param);
    const char *GetCheckedParam(void) const	{ return(m_checkedParam); }
    SPxErrorCode SetCheckedValue(const char *val);
    const char *GetCheckedValue(void) const	{ return(m_checkedValue); }
    SPxErrorCode SetState2Object(const char *obj);
    const char *GetState2Object(void) const	{ return(m_state2Object); }
    SPxErrorCode SetState2Param(const char *param);
    const char *GetState2Param(void) const	{ return(m_state2Param); }
    SPxErrorCode SetState2Value(const char *val);
    const char *GetState2Value(void) const	{ return(m_state2Value); }
    SPxErrorCode SetState3Object(const char *obj);
    const char *GetState3Object(void) const	{ return(m_state3Object); }
    SPxErrorCode SetState3Param(const char *param);
    const char *GetState3Param(void) const	{ return(m_state3Param); }
    SPxErrorCode SetState3Value(const char *val);
    const char *GetState3Value(void) const	{ return(m_state3Value); }
    SPxErrorCode SetUncheckedObject(const char *obj);
    const char *GetUncheckedObject(void) const	{ return(m_uncheckedObject); }
    SPxErrorCode SetUncheckedParam(const char *param);
    const char *GetUncheckedParam(void) const	{ return(m_uncheckedParam); }
    SPxErrorCode SetUncheckedValue(const char *val);
    const char *GetUncheckedValue(void) const	{ return(m_uncheckedValue); }
    SPxErrorCode SetEditParam(const char *param);
    const char *GetEditParam(void) const	{ return(m_editParam); }
    SPxErrorCode SetRemoteAddr(const char *addr);
    const char *GetRemoteAddr(void) const	{ return(m_remoteAddr); }
    SPxErrorCode SetRemotePort(int port);
    int GetRemotePort(void) const		{ return(m_remotePort); }
    SPxErrorCode SetSystemCommand(const char *cmd);
    const char *GetSystemCommand(void) const	{ return(m_sysCmd); }
    SPxErrorCode SetCheckedFile(const char *filename);
    const char *GetCheckedFile(void) const	{ return(m_checkedFile); }
    SPxErrorCode SetUncheckedFile(const char *filename);
    const char *GetUncheckedFile(void) const	{ return(m_uncheckedFile); }

    /* Parameter setting. */
    int SetParameter(char *name, char *value);
    int GetParameter(char *name, char *valBuf, int bufLen);

private:
    /*
     * Private members.
     */
    /* Mutex protection. */
    mutable SPxCriticalSection m_mutex;

    /* Underlying button. */
    SPxButtonBoxButton *m_button;

    /* Button options. */
    char *m_label;		/* Label for button */
    char *m_label2;		/* Label when switching to state 2 */
    char *m_label3;		/* Label when switching to state 3 */
    char *m_labelUnchecked;	/* Label when switching to unchecked */
    int m_state;		/* 0 for not checked, 1 for checked */
    int m_type;			/* SPX_BB_BUTTON_TYPE_... */

    /* Information about what command to run when pressed. */
    char *m_checkedObject;	/* Object to control when button is set */
    char *m_checkedParam;	/* Param to control when button is set */
    char *m_checkedValue;	/* Value to set when button is set */
    char *m_state2Object;	/* Object to control when button in state 2 */
    char *m_state2Param;	/* Param to control when button in state 2 */
    char *m_state2Value;	/* Value to set when button in state 2 */
    char *m_state3Object;	/* Object to control when button in state 3 */
    char *m_state3Param;	/* Param to control when button in state 3 */
    char *m_state3Value;	/* Value to set when button in state 3 */
    char *m_uncheckedObject;	/* Object to control when button is cleared */
    char *m_uncheckedParam;	/* Param to control when button is cleared */
    char *m_uncheckedValue;	/* Value to set when button is cleared */
    char *m_editParam;		/* Param to edit on right-clicks */
    char *m_remoteAddr;		/* Address of remote server, or NULL */
    int m_remotePort;		/* Port for remote server */
    char *m_sysCmd;		/* System command to run */
    char *m_checkedFile;	/* File to load when button pressed */
    char *m_uncheckedFile;	/* File to load when button unpressed */

    /* Object for sending remote commands. */
    SPxRemoteServer *m_remoteServer;

    /* Internal setup function. */
    SPxErrorCode setupButton(void);

    /* Button callback functions. */
    static void staticOnLClicked(SPxButtonBoxButton *button, void *userData);
    void onLClicked(void);
    static void staticOnRClicked(SPxButtonBoxButton *button, void *userData);
    void onRClicked(void);
}; /* SPxButtonBoxCmdButton */

#endif /* _SPX_BUTTON_BOX_CMD_BUTTON_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/

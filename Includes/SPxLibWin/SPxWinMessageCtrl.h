/*********************************************************************
*
* (c) Copyright 2014 - 2020, 2022, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for a message window that can displays coloured
*	SPx errors and other arbitrary messages.
*
*
* Revision Control:
*   27/01/22 v1.12   SP 	Add Freeze().
*
* Previous Changes:
*   21/09/20 1.11   AJH	Add SetMessageFont().
*   16/09/20 1.10   AJH	Add SetMaxRepeatCount().
*   12/04/19 1.9    AGC	Include SPxError header.
*   01/02/19 1.8    AGC	Fix N/C #661 - add constructor option to show/hide errors.
*   11/12/18 1.7    REW	Add option to turn off adding line endings.
*   28/11/17 1.6    AGC	Respond to DPI changes.
*   22/06/17 1.5    SP 	Support log file.
*   04/02/16 1.4    AGC	Support manually forwarding SPxErrors.
*   10/03/14 1.3    AGC	Add option for including date.
*			Add option for scrolling to bottom.
*   05/03/14 1.2    AGC	Add Clear() function.
*   17/02/14 1.1    AGC	Initial Version.
**********************************************************************/
#pragma once

/* For std::va_list. */
#include <cstdarg>

/* For SPxAutoPtr. */
#include "SPxLibUtils/SPxAutoPtr.h"

/* For SPxErrorCode. */
#include "SPxLibUtils/SPxError.h"

/*
 * SPxWinMessageCtrl control
 */
class SPxWinMessageCtrl : public CRichEditCtrl
{
    DECLARE_DYNAMIC(SPxWinMessageCtrl)

public:
    /* Construction and desctruction. */
    explicit SPxWinMessageCtrl(int showErrors=TRUE);
    virtual ~SPxWinMessageCtrl(void);

    void SetShowErrors(int showErrors);
    void SetShowDate(int showDate);
    void SetScrollToBottom(int scrollToBottom);
    void SetAddLineEndings(int addLineEndings);
    void SetMaxLines(int maxLines);
    void SetMaxRepeatCount(unsigned int count);
    SPxErrorCode SetLogFilePrefix(const char *prefix);
    SPxErrorCode SetMessageFont(const char *font, unsigned int fontSize);

    void ReportMsgV(COLORREF colour, const char *msg, std::va_list ap);
    void ReportMsg(COLORREF colour, const char *msg, ...);
    void ReportMsg(const char *msg, ...);
    void ReportError(SPxErrorType errType, SPxErrorCode errorCode,
		     int arg1, int arg2, const char *arg3, const char *arg4);

    void Clear(void);
    void Freeze(int isFozen);
    int IsFrozen(void);

protected:
    /* Protected functions. */
    virtual void PreSubclassWindow(void);
    void OnTimer(UINT_PTR nIDEvent);
    void OnDpiChangedAfterParent(void);
    DECLARE_MESSAGE_MAP()

private:
    /* Private variables. */
    struct impl;
    struct msgDesc;
    SPxAutoPtr<impl> m_p;

    /* Private functions. */
    static void errorFnWrapper(SPxErrorType errType, SPxErrorCode errorCode,
			       int arg1, int arg2, const char *arg3, const char *arg4,
			       void *userArg);
    void updateMessages(int allMessages=FALSE);

}; /* class SPxWinMessageCtrl */

/*********************************************************************
*
* End of file
*
**********************************************************************/

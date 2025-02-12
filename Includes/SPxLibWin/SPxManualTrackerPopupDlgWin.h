/*********************************************************************
*
* (c) Copyright 2020 - 2022, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for the manual tracker control pop-up dialog.
*
*
* Revision Control:
*   02/09/22 v1.5    AGC	Supress cppcheck duplicate inherited member warning.
*
* Previous Changes:
*   26/05/22 1.4    BTB	Support specifying type of track report to send.
*   10/03/20 1.3    BTB	Support showing/hiding track lat/long.
*                      	Improve resizing of dialog.
*   02/03/20 1.2    BTB	Support configuration of speed/course calculation.
*   21/02/20 1.1    BTB	Initial Version.
**********************************************************************/

#pragma once

/* Include the base class header file */
#include "SPxManualTrackerDlgWin.h"

/*
 * SPxManualTrackerPopupDlgWin dialog
 */
class SPxManualTrackerPopupDlgWin : public SPxManualTrackerDlgWin
{
public:
    /* Constructor and destructor. */
    SPxManualTrackerPopupDlgWin(CWnd *pParent,
                                SPxAsterixEncoder *trackEncoder,
                                SPxPacketSender *trackSender,
                                int calcSpeedCourse,
                                int showLatLong,
                                int reportType,
                                const char *sensorName = NULL);
    virtual ~SPxManualTrackerPopupDlgWin();

protected:
    /* Protected functions */
    virtual void DoResizing(void);
    virtual BOOL OnInitDialog(void);
    virtual void PreSubclassWindow(void);
    virtual void OnCancel(void);

private:
    struct impl;
    // cppcheck-suppress duplInheritedMember
    SPxAutoPtr<impl> m_p;

}; /* class SPxManualTrackerPopupDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/

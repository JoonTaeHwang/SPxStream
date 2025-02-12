/*********************************************************************
 *
 * (c) Copyright 2020, 2021, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   Alarm config trigger conditions dialog.
 *
 * Revision Control:
 *   10/02/21 v1.2    AGC	Shift more logic to SPxCondDlgWin.
 *
 * Previous Changes:
 *   25/09/20 1.1   AGC	Initial version based on SPxRwAlarmConfigCondTabDlg.
 *
 *********************************************************************/
#pragma once

#include "SPxLibUtils/SPxAlarmCondition.h"
#include "SPxCondDlgWin.h"

/* Forward declare any classes required. */
class SPxUniTrackDatabase;
class SPxAlarmDatabase;
class SPxTrackClassMngr;
class SPxTrackGroupEditPopupDlgWin;
class SPxAlarmConfig;
class SPxAlarmCondition;

/* SPxAlarmConfigCondDlgWin dialog */
class SPxAlarmConfigCondDlgWin : public SPxCondDlgWin
{
public:
    /*
     * Public types.
     */

    /*
     * Public variables.
     */

    /*
     * Public functions.
     */

    /* Construction and destruction. */
    explicit SPxAlarmConfigCondDlgWin(SPxUniTrackDatabase *trackDB,
                                      SPxAlarmDatabase *alarmDB,
                                      SPxTrackClassMngr *trackClassMngr=NULL,
                                      SPxTrackGroupEditPopupDlgWin *trackGroupEditDlg=NULL);
    virtual ~SPxAlarmConfigCondDlgWin(void);

protected:
    /* 
     * Protected variables. 
     */

    /* 
     * Protected functions. 
     */

    DECLARE_MESSAGE_MAP()
    virtual void DoDataExchange(CDataExchange* pDX);

    /* Implement functions in the base class. */
    const char *getNameEditToolTip(void) const;
    virtual BOOL canTrackBeAddedToCombo(SPxUniTrack *track);

private:
    /*
     * Private variables.
     */

    /* General. */
    SPxAlarmDatabase *m_alarmDB;

    /*
     * Private functions.
     */

    /*
     * Private static functions.
     */
};

/*********************************************************************
 *
 * End of file
 *
 **********************************************************************/

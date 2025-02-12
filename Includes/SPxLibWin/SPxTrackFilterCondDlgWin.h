/*********************************************************************
 *
 * (c) Copyright 2020, 2021, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   Track filter conditions dialog.
 *
 * Revision Control:
 *  10/02/21 v1.5    AGC	Shift more logic to SPxCondDlgWin.
 *
 * Previous Changes:
 *  29/09/20 1.4    AGC	Exclude plots from track combo.
 *  24/09/20 1.3    AGC	Get picture database from track database.
 *  21/09/20 1.2    AGC	Fix build warnings.
 *  18/09/20 1.1    AGC	Initial version extracted from SPxRwTrackFilterCondTabDlg.
 *
 *********************************************************************/
#pragma once

#include "SPxLibNet/SPxUniTrackCondition.h"
#include "SPxCondDlgWin.h"

/* Forward declarations. */
class SPxUniTrack;
class SPxUniTrackDatabase;
class SPxTrackClassMngr;
class SPxTrackGroupEditPopupDlgWin;

/* SPxTrackFilterCondDlgWin dialog */
class SPxTrackFilterCondDlgWin : public SPxCondDlgWin
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
    explicit SPxTrackFilterCondDlgWin(SPxUniTrackDatabase *trackDB,
                                      SPxTrackClassMngr *trackClassMngr=NULL,
                                      SPxTrackGroupEditPopupDlgWin *trackGroupEditDlg=NULL);
    virtual ~SPxTrackFilterCondDlgWin(void);

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
    virtual const char *getNameEditToolTip(void) const;
    virtual BOOL canTrackBeAddedToCombo(SPxUniTrack *track);

private:
    /*
     * Private variables.
     */

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

/*********************************************************************
 *
 * (c) Copyright 2020, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   Track filters main panel child dialog.
 *
 * Revision Control:
 *   18/09/20 v1.1   AGC	Initial version extracted from SPxRwTrackFiltersPanelDlgWin.
 *
 * Previous Changes:
 *
 *********************************************************************/
#pragma once

/* Other headers. */
#include "SPxResource.h"
#include "SPxCondGroupsPanelDlgWin.h"
#include "SPxLibUtils/SPxEventHandler.h"
#include "SPxLibNet/SPxUniTrackCondition.h"
#include "SPxLibNet/SPxUniTrackFilter.h"

/* Forward declarations. */
class SPxConditionGroup;

class SPxTrackFiltersPanelDlgWin : public SPxCondGroupsPanelDlgWin
{
public:
    /*
     * Public types.
     */
    typedef void (*FilterEditFn)(SPxUniTrackFilter *filter);

    /*
     * Public variables.
     */

    /*
     * Public functions.
     */

    /* Construction and destruction. */
    explicit SPxTrackFiltersPanelDlgWin(SPxUniTrackDatabase *trackDB,
                                        int sharedMode=FALSE);
    virtual ~SPxTrackFiltersPanelDlgWin(void);

    /* General. */
    template<typename F, typename O>
    SPxErrorCode AddFilterEditCallback(F fn, O *obj)
    {
        return m_filterEditFns.Add(fn, obj);
    }
    template<typename F, typename O>
    SPxErrorCode RemoveFilterEditCallback(F fn, O *obj)
    {
        return m_filterEditFns.Remove(fn, obj);
    }

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
    virtual BOOL PreTranslateMessage(MSG* pMsg);

     /* Implement functions in the base class. */
    virtual void updateBuffer(void);
    virtual void reportGroupEdit(SPxConditionGroup *group);
    virtual void onPopupMenuCreate(int row, CMenu *menu);
    virtual void onPopupMenuSelect(int row, int selection);

    /* Event handling functions. */
    virtual void onTrackFilterCreate(SPxUniTrackFilter *filter);
    virtual void onTrackFilterChange(SPxUniTrackFilter *filter, 
                                     SPxUniTrackFilter::ChangeFlags_t flags);
    virtual void onTrackFilterDelete(SPxUniTrackFilter *filter);
    virtual void onTrackFilterConditionCreate(SPxUniTrackCondition *condition);
    virtual void onTrackFilterConditionChange(SPxUniTrackCondition *condition, 
                                              SPxUniTrackCondition::ChangeFlags_t flags);
    virtual void onTrackFilterConditionDelete(SPxUniTrackCondition *condition);
    virtual void onTrackGroupCreate(UINT32 trackGroupID);
    virtual void onTrackGroupDelete(UINT32 trackGroupID);

    /* GUI callback functions. */

private:
    /*
     * Private variables.
     */

    /* General. */
    SPxEventHandler<FilterEditFn> m_filterEditFns;
    CToolTipCtrl m_toolTip;             /* Used to implement tool tips. */
    SPxUniTrackDatabase *m_trackDB;     /* Track database. */
    SPxUniTrackFilter *m_pmTrackFilter;

    /*
     * Private functions.
     */

    /* General. */
    void addPopupMenuTrackFilterItems(CMenu *menu, SPxUniTrackFilter *filter);

    /*
     * Private static functions.
     */

};

/*********************************************************************
 *
 * End of file
 *
 **********************************************************************/

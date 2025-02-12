/*********************************************************************
 *
 * (c) Copyright 2017, Cambridge Pixel Ltd.
 *
 * File: $RCSfile: SPxWinCheckboxTreeCtrl.h,v $
 * ID: $Id: SPxWinCheckboxTreeCtrl.h,v 1.3 2017/09/26 15:54:31 rew Exp $
 *
 * Purpose:
 *   A class derived from CTreeCtrl, used to implement a tree
 *   with tri-state checkboxes.
 *
 * Revision Control:
 *   26/09/17 v1.3   SP 	Change setCheck() args.
 *                              Support expand all / collapse all.
 *
 * Previous Changes:
 *   05/07/17 1.2   SP 	Support restore of child items.
 *   29/06/17 1.1   SP 	Initial version.
 *
 **********************************************************************/

#pragma once

#define SPX_AFXWIN
#include "SPxLibUtils/SPxCommon.h"
#include "SPxLibUtils/SPxError.h"
#include "SPxLibUtils/SPxAutoPtr.h"

/* SPxWinCheckboxTreeCtrl class */
class SPxWinCheckboxTreeCtrl : public CTreeCtrl
{
public:

    /*
     * Public variables.
     */

    /*
     * Public functions.
     */

    /* Construction and destruction. */
    SPxWinCheckboxTreeCtrl(void);
    virtual ~SPxWinCheckboxTreeCtrl(void);

    /* General. */
     HTREEITEM InsertCheckboxItem(LPCTSTR text,  
                                  HTREEITEM parent=TVI_ROOT,  
                                  HTREEITEM insertAfter=TVI_LAST);
    SPxErrorCode SetTriStateCheck(HTREEITEM item, int check);
    SPxErrorCode SetTriStateSavedCheck(HTREEITEM item, int check);
    int GetTriStateCheck(HTREEITEM item);
    int GetTriStateSavedCheck(HTREEITEM item);
    SPxErrorCode ExpandAll(BOOL expand);
    SPxErrorCode ExpandBranch(HTREEITEM item, BOOL expand);

protected:

    /*
     * Protected variables.
     */

    /*
     * Protected functions.
     */

    virtual void PreSubclassWindow(void);
    virtual afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    virtual afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
    virtual afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
       
protected:

    /*
     * Private types.
     */

    /*
     * Private variables.
     */

    /* General. */
    CImageList m_imageList;  /* Images to display in tree control. */

    /*
     * Private functions.
     */
    SPxErrorCode setCheck(HTREEITEM item, int check, BOOL saveState);
    SPxErrorCode setSavedCheck(HTREEITEM item, int check);
    int getCheck(HTREEITEM item);
    int getSavedCheck(HTREEITEM item);
    int getAggregateCheck(HTREEITEM item);
    SPxErrorCode toggleCheck(HTREEITEM item);
    SPxErrorCode setDescendants(HTREEITEM item, int check, BOOL saveState);
    SPxErrorCode restoreDescendants(HTREEITEM item);
    SPxErrorCode updateAncestors(HTREEITEM item);

    DECLARE_MESSAGE_MAP()
};

/*********************************************************************
 *
 * End of file
 *
 **********************************************************************/


/*********************************************************************
*
* (c) Copyright 2020, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for a generic tabbed dialog item, used when displaying
*	multiple copies of the same dialog (for different objects)
*	in tabs or in a row (with optional scrollbars).
*
*
* Revision Control:
*   27/08/20 v1.3    AGC	Make GetTabName() consistent with SPxTabbedItemName.
*
* Previous Changes:
*   26/08/20 1.2    AGC	Add GetTabName() function.
*   03/04/20 1.1    AGC	Initial version.
**********************************************************************/

#pragma once

/* Standard headers. */

/* Include the SPx resource header */
#include "SPxResource.h"

#define SPX_AFXWIN
#include "SPxLibUtils/SPxCommon.h"
#include "SPxLibUtils/SPxError.h"

/* Base class for child dialogs for tabbed control. */
class SPxTabbedItemDlg : public CDialog
{
public:
    explicit SPxTabbedItemDlg(
        UINT nIDTemplate,
        CWnd* pParentWnd = NULL)
        : CDialog(nIDTemplate, pParentWnd) {}

    virtual void SyncFromObj(void) = 0;
    virtual int GetIDD(void) const = 0;
    virtual SPxErrorCode GetTabName(char * /*buf*/, int /*bufSizeBytes*/) const
    {
        return SPX_ERR_NOT_SUPPORTED;
    }
    virtual CWnd *GetGroupBox(void) { return NULL; }
    virtual int ShowCheck(void) { return FALSE; }
    virtual void Enable(int /*enable*/) {}
    virtual int GetEnable(void) { return FALSE; }
    virtual int HasObj(void) const { return TRUE; }
    virtual void SetObjIndex(int /*index*/) {}

protected:
    /* Function called to send message to parent dialog
     * indicating that a resync is required.
     */
    void SyncParent(void);
};

/*********************************************************************
*
* End of file
*
**********************************************************************/
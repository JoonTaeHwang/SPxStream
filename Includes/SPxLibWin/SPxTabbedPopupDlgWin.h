/*********************************************************************
*
* (c) Copyright 2013 - 2014, 2018, 2020, 2021, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for a generic tabbed popup dialog for displaying
*	multiple copies of the same dialog (for different objects)
*	in tabs or in a row (with optional scrollbars).
*
*
* Revision Control:
*   06/10/21 v1.9    AGC	Improve child to popup conversion.
*
* Previous Changes:
*   26/08/20 1.8    AGC	Support construction with no tabs.
*   23/01/18 1.7    AGC	Support toggling tab objects on same dialog.
*   27/01/14 1.6    AGC	Move content to SPxTabbedDlgWin.
*   21/10/13 1.5    AGC	Remove GetHeight() and GetMinWidth()
*			functions from SPxTabbedItemDlg.
*   15/10/13 1.4    AGC	Add GetHeight() and GetMinWidth()
*			functions to SPxTabbedItemDlg.
*   27/09/13 1.3    AGC	Include SPxCommon header.
*   30/07/13 1.2    AGC	Add HasObj() function to SPxTabbedItemDlg.
*   08/03/13 1.1    AGC	Initial Version.
**********************************************************************/
#pragma once

/* Include the base class header file */
#include "SPxTabbedDlgWin.h"

/*
 * SPxTabbedPopupDlgWin dialog
 */
class SPxTabbedPopupDlgWin : public SPxTabbedDlgWin
{
public:
    /* Constructor and destructor. */
    explicit SPxTabbedPopupDlgWin(CWnd* pParent,
	void (*syncFn)(void *)=NULL, void *syncArg=NULL);
    SPxTabbedPopupDlgWin(CWnd* pParent,
	SPxTabbedItemDlg *children[],
	unsigned int numChildren,
	void (*syncFn)(void *)=NULL, void *syncArg=NULL);
    SPxTabbedPopupDlgWin(CWnd* pParent,
	SPxTabbedItemDlg *tabDialog,
	unsigned int numTabs,
	void(*syncFn)(void *)=NULL, void *syncArg=NULL);
    virtual ~SPxTabbedPopupDlgWin();

protected:
    /* Protected functions */
    virtual BOOL OnInitDialog(void);

public:
    /* Nothing at present */
   
}; /* class SPxTabbedPopupDlgWin */

/*********************************************************************
*
* End of file
*
**********************************************************************/

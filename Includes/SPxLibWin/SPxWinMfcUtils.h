/*********************************************************************
 *
 * (c) Copyright 2012 - 2022, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   Windows MFC utility functions.
 *
 * Revision Control:
 *  04/05/22 v1.38   SP 	SPxDialogChildToPopupResize().
 *
 * Previous Changes:
 *  06/10/21 1.37   AGC	Add SPxDialogChildCreatePopup().
 *  11/12/20 1.36   SP 	Add SPxCleanUpMenu().
 *  11/09/20 1.35   AGC	Add new DateTimeCtrl functions.
 *  12/06/20 1.34   SP 	Add CTimeToSPxTime() and SPxTimeToCTime().
 *  13/06/19 1.33   BTB	Add SPxCheckWindowHasFocus().
 *  05/06/19 1.32   SP 	Add SPxComboBoxGetEntryFromVal().
 *  09/04/19 1.31   AGC	Per-monitor high DPI enhancements.
 *  14/03/19 1.30   AGC	Override SPxSetButtonIcon() for SPxWinButton.
 *  18/01/19 1.29   AGC	Add CButton variant of SPxCompactString().
 *  04/07/18 1.28   AGC	Support SPxSetWindowText() format warnings.
 *  03/05/18 1.27   AGC	Move SPxThreadDpiAwareness to SPxWin32Utils.
 *  26/04/18 1.26   SP 	Add SPxGetTopWindow() and SPxSetWindowOwner().
 *  14/12/17 1.25   AGC	Add SPxThreadDpiAwareness RAII class.
 *			Add SPxSetThreadDpiAwareness().
 *  31/07/17 1.24   SP 	Add optional size to SPxSetStatusIcon().
 *  05/07/17 1.23   SP 	Add SPxGetIconInfo().
 *  30/06/17 1.22   AGC	Add resizable option to SPxDialogChildToPopup().
 *  29/06/17 1.21   AGC	Add SPxDialogGetNearestMonitor().
 *  22/06/17 1.20   SP 	Add SPxGetStatusIconResourceID().
 *  22/06/17 1.19   AGC	Add SPxCompactString().
 *  04/01/17 1.18   AGC	Add SPxComboBoxSizeDropToContents().
 *  29/04/16 1.17   SP 	Add window arg to SPxGetXPixelsFromDu() and
 *			SPxGetYPixelsFromDu().
 *                      Remove SPxGetXDuFromPixels() and SPxGetYDuFromPixels()
 *			as they do not work correctly.
 *  11/03/16 1.16   SP 	Add pixels to/from dialog units conversion functions.
 *                      Add SPxLoadIcon().
 *  23/12/15 1.15   REW	Break SPxDialogMoveToPosition() out from
 *		        SPxDialogPopupShow().
 *  24/07/15 1.14   AGC	Add position options to SPxDialogPopupShow().
 *  10/07/15 1.13   AGC	Add SPxDialogMoveToVisible().
 *  11/11/14 1.12   SP 	Add SPxSetButtonIcon().
 *  18/08/14 1.11   SP 	Add SPxSetStatusIcon().
 *  08/05/14 1.10   AGC	Support popup of dialog in centre of parent.
 *  17/02/14 1.9    AGC	Add SPxDialogPopupShow().
 *  21/01/14 1.8    SP 	Add SPxSetWindowVisible.
 *  15/01/14 1.7    AGC	Add SPxDialogChildToPopup().
 *  08/11/13 1.6    AGC	Add variable argument list to SPxSetWindowText().
 *  04/11/13 1.5    AGC	Add SPxCompactPath().
 *  21/02/13 1.4    SP 	Add SPxDDX_Text with unsigned arg.
 *  12/02/13 1.3    SP 	Add prefix to SPxDDX_Text.
 *  09/11/12 1.2    AGC	Tidy line endings.
 *  26/10/12 1.1    AGC	Initial version.
 **********************************************************************/
#ifndef _SPX_WIN_MFC_UTILS_H
#define _SPX_WIN_MFC_UTILS_H

/* Other headers. */
#include "SPxLibUtils/SPxError.h"
#include "SPxLibUtils/SPxTypes.h"
#include "SPxLibUtils/SPxTime.h"

/*********************************************************************
*
*   Types
*
**********************************************************************/

/* Forward declarations. */
class SPxWinButton;

enum SPxDialogPosition
{
    SPX_DLG_POS_CENTRE_SCREEN = 0,
    SPX_DLG_POS_CENTRE_PARENT = 1,
    SPX_DLG_POS_CURSOR = 2,
    SPX_DLG_POS_EDGE_PARENT = 3
};

enum SPxEllipsisPos
{
    SPX_ELLIPSIS_POS_NONE = 0,
    SPX_ELLIPSIS_POS_END = 1,
    SPX_ELLIPSIS_POS_PRE_UNDERSCORE = 2
};

/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

extern SPxErrorCode SPxComboBoxAppendEntry(CComboBox *cb, 
                                           const char *str, 
                                           UINT32 val);

extern SPxErrorCode SPxComboBoxAppendEntryStr(CComboBox *cb, 
                                              const char *str, 
                                              const char *valStr);

extern SPxErrorCode SPxComboBoxSetFromVal(CComboBox *cb, 
                                          UINT32 val, 
                                          BOOL addIfMissing=TRUE);

extern SPxErrorCode SPxComboBoxSetFromValStr(CComboBox *cb, 
                                             const char *valStr,
                                             BOOL addIfMissing=TRUE);

extern UINT32 SPxComboBoxGetVal(CComboBox *cb);

extern SPxErrorCode SPxComboBoxGetValStr(CComboBox *cb, char *valStr, 
                                         unsigned int bufLen);

extern SPxErrorCode SPxComboBoxRemoveAll(CComboBox *cb);

extern SPxErrorCode SPxComboBoxRemoveAllStr(CComboBox *cb);

extern SPxErrorCode SPxComboBoxSizeDropToContents(CComboBox *cb);

extern SPxErrorCode SPxComboBoxGetEntryFromVal(CComboBox *cb, 
                                               UINT32 val,
                                               char *buf,
                                               unsigned int bufLen);

extern SPxErrorCode
#if defined(__GNUC__) || defined(__clang__)
__attribute__((format(printf, 2, 3)))
#endif
SPxSetWindowText(CWnd *win, const char *text, ...);

extern SPxErrorCode SPxSetWindowText(CWnd *win, int val);

extern SPxErrorCode SPxSetWindowVisible(CWnd *win, int isVisible);

extern HICON SPxLoadIcon(UINT id, unsigned int width=0, unsigned int height=0,
                         int isDpiScaled=FALSE);

extern SPxErrorCode SPxSetIcon(CStatic *ctrl, HICON icon);

extern SPxErrorCode SPxSetButtonIcon(CButton *button, HICON icon);
extern SPxErrorCode SPxSetButtonIcon(SPxWinButton *button, HICON icon);

extern UINT SPxGetStatusIconResourceID(SPxStatus_t status);

extern SPxErrorCode SPxSetStatusIcon(CStatic *ctrl, 
                                     SPxStatus_t status, 
                                     unsigned int size=32);

extern SPxErrorCode SPxGetIconInfo(HICON icon, 
                                   unsigned int *widthPixelsRtn=NULL,
                                   unsigned int *heightPixelsRtn=NULL,
                                   unsigned int *bppRtn=NULL);

extern void AFXAPI SPxDDX_CBIndexFromVal(CDataExchange *pDX, 
                                         int nIDC, 
                                         UINT32 &val);

extern void AFXAPI SPxDDX_CBIndexFromValStr(CDataExchange *pDX, 
                                            int nIDC, 
                                            char *valStr,
                                            unsigned int valStrLen);

extern void AFXAPI SPxDDX_Text(CDataExchange *pDX, int nIDC, 
                               double &val, int numDp,
                               const char *suffix=NULL,
                               const char *prefix=NULL);

extern void AFXAPI SPxDDX_Text(CDataExchange *pDX, int nIDC, 
                               int &val, 
                               const char *suffix=NULL,
                               const char *prefix=NULL);

extern void AFXAPI SPxDDX_Text(CDataExchange *pDX, int nIDC, 
                               unsigned int &val, 
                               const char *suffix=NULL,
                               const char *prefix=NULL);

extern void AFXAPI SPxDDX_Slider(CDataExchange *pDX, int nIDC, 
                                 double &val, int mult);

extern SPxErrorCode SPxCompactPath(CString *path, const CWnd *wnd);
extern SPxErrorCode SPxCompactString(CString *str, const CWnd *wnd,
                                     SPxEllipsisPos ellipsisPos);
extern SPxErrorCode SPxCompactString(CString *str, const CButton *button,
				     SPxEllipsisPos ellipsisPos);

extern BOOL SPxDialogChildToPopup(CDialog *dlg, int resizable=FALSE);
extern BOOL SPxDialogChildToPopupResize(CDialog *dlg, UINT resID);

extern CDialog *SPxDialogChildCreatePopup(CDialog *dlg, int resizable=FALSE);
extern CWnd *SPxDialogChildGetPopup(CWnd *wnd);

extern void SPxDialogPopupShow(CDialog *dlg, CWnd *parent=NULL);

extern void SPxDialogPopupShow(CDialog *dlg, SPxDialogPosition dlgPos, CWnd *parent=NULL);

extern void SPxDialogMoveToPosition(CDialog *dlg, SPxDialogPosition dlgPos,
                                    CWnd *parent=NULL);

extern void SPxDialogMoveToVisible(CDialog *dlg);

extern int SPxGetXPixelsFromDu(CWnd *dlg, int du);
extern int SPxGetYPixelsFromDu(CWnd *dlg, int du);

extern HMONITOR SPxDialogGetNearestMonitor(CWnd *dlg, HMONITOR excludeMon);

extern CWnd *SPxGetTopWindow(CWnd *win);

extern void SPxSetWindowOwner(CWnd *win, CWnd *owner);

extern int SPxCheckWindowHasFocus(CWnd *win);

extern void SPxTimeToCTime(const SPxTime_t *spxTime,
                           CTime *cTime);

extern void CTimeToSPxTime(const CTime *cTime,
                           SPxTime_t *spxTime);

extern SPxErrorCode SPxDateTimeCtrlSetDuration(CDateTimeCtrl *durationCtrl,
                                               const SPxTime_t *spxTime);

extern SPxErrorCode SPxDateTimeCtrlGetDuration(const CDateTimeCtrl *durationCtrl,
                                               SPxTime_t *spxTime);

extern SPxErrorCode SPxDateTimeCtrlSetDate(CDateTimeCtrl *dateCtrl,
                                           const SPxTime_t *spxTime);

extern SPxErrorCode SPxDateTimeCtrlGetDate(const CDateTimeCtrl *dateCtrl,
                                           SPxTime_t *spxTime);

extern SPxErrorCode SPxDateTimeCtrlSetTime(CDateTimeCtrl *timeCtrl,
                                           const SPxTime_t *spxTime);

extern SPxErrorCode SPxDateTimeCtrlGetTime(const CDateTimeCtrl *timeCtrl,
                                           SPxTime_t *spxTime);

extern SPxErrorCode SPxDateTimeCtrlSetDateTime(CDateTimeCtrl *dateCtrl,
                                               CDateTimeCtrl *timeCtrl,
                                               const SPxTime_t *spxTime);

extern SPxErrorCode SPxDateTimeCtrlGetDateTime(const CDateTimeCtrl *dateCtrl,
                                               const CDateTimeCtrl *timeCtrl,
                                               SPxTime_t *spxTime);

extern SPxErrorCode SPxCleanUpMenu(CMenu *menu);

#endif /* _SPX_WIN_MFC_UTILS_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/

/*********************************************************************
*
* (c) Copyright 2007 - 2022, Cambridge Pixel Ltd.
*
* Purpose:
*   Header for Windows utility functions.
*
*
* Revision Control:
*   06/09/22 v1.24   AGC	Suppress spurious cppcheck ODR warnings.
*
* Previous Changes:
 *  09/12/21 1.23   AGC	Add SPX_WMI_DPICHANGED_BEFOREPARENT.
 *  28/08/20 1.22   AGC	Use precompiled header.
 *  22/06/20 1.21   REW	Add SPxSetAUMID().
*   17/03/20 1.20   AGC	Support full-screening multiple windows.
*   10/03/20 1.19   AGC	Add SPxSetResourceModuleName().
*			Add SPxGetResourceModule().
*   29/11/19 1.18   AGC	Add SPxIsGuiThread().
*   13/06/19 1.17   BTB	Add SPxCheckWindowHasFocus().
*   21/06/18 1.16   AGC	Add SPxGetMonitorDpi().
*   15/06/18 1.15   AGC	Add SPxGetPhysicalDpi().
*   03/05/18 1.14   AGC	Move SPxThreadDpiAwareness here from SPxWinMfcUtils.
*			Add SPxWindowMoveToPosition().
*			Add SPxWindowMoveToVisible().
*   26/04/18 1.13   SP 	Add SPxGetTopWindow() and SPxSetWindowOwner().
*   24/01/18 1.12   AGC	Allow later load with SPxWinLibrary.
*   28/11/17 1.11   AGC	Add DPICHANGED message map macros.
*			Add optional HWND arg to SPxGetDpiScaledSize().
*   22/06/17 1.10   AGC	Add monitor option to SPxFullScreenToggle().
*   20/05/16 1.9    AGC	Add long overload for SPxGetDpiScaledSize().
*   11/03/16 1.8    SP 	Add some DPI scaling functions.
*   02/03/15 1.7    SP 	Add SPxBGRtoARGB().
*   04/10/13 1.6    SP 	Add SPxRotateImage().
*   30/07/13 1.5    AGC	Add SPxWinLibrary for loading/unloading DLL.
*   06/06/13 1.4    AGC	Improve full-screen efficiency.
*   24/05/13 1.3    AGC	Add SPxFullScreenToggle().
*   23/11/12 1.2    SP 	Add SPxCOLORREFtoARGB() and SPxRGBtoCOLORREF().
*   15/05/12 1.1    REW	Initial Version.
**********************************************************************/

#ifndef _SPX_WIN32UTILS_H_
#define	_SPX_WIN32UTILS_H_

/* For SPxErrorCode. */
#include "SPxLibUtils/SPxError.h"

/* For SPX_T16. */
#include "SPxLibUtils/SPxWideString.h"

/* For Windows types. */
#include "SPxLibUtils/SPxCommon.h"

/*********************************************************************
*
*   Constants
*
**********************************************************************/

DECLARE_HANDLE(SPxDpiAwareness);
#define SPX_DPI_AWARENESS_UNAWARE SPxDpiAwareness(-1)
#define SPX_DPI_AWARENESS_SYSTEM SPxDpiAwareness(-2)
#define SPX_DPI_AWARENESS_PER_MONITOR SPxDpiAwareness(-3)
#define SPX_DPI_AWARENESS_PER_MONITOR_V2 SPxDpiAwareness(-4)

#define SPX_WM_DPICHANGED 0x02E0
#define SPX_WM_DPICHANGED_BEFOREPARENT 0x02E2
#define SPX_WM_DPICHANGED_AFTERPARENT 0x02E3

#define SPX_ON_WM_DPICHANGED() \
    { SPX_WM_DPICHANGED, 0, 0, 0, AfxSig_v_w_l, \
	(AFX_PMSG)(AFX_PMSGW) \
	(static_cast< void (AFX_MSG_CALL CWnd::*)(WPARAM, LPARAM) > ( &ThisClass :: OnDpiChanged)) },
#define SPX_ON_WM_DPICHANGED_BEFOREPARENT() \
    { SPX_WM_DPICHANGED_BEFOREPARENT, 0, 0, 0, AfxSig_v_v_v, \
	(AFX_PMSG)(AFX_PMSGW) \
	(static_cast< void (AFX_MSG_CALL CWnd::*)() > ( &ThisClass :: OnDpiChangedBeforeParent)) },
#define SPX_ON_WM_DPICHANGED_AFTERPARENT() \
    { SPX_WM_DPICHANGED_AFTERPARENT, 0, 0, 0, AfxSig_v_v_v, \
	(AFX_PMSG)(AFX_PMSGW) \
	(static_cast< void (AFX_MSG_CALL CWnd::*)() > ( &ThisClass :: OnDpiChangedAfterParent)) },

/*********************************************************************
*
*   Type definitions
*
**********************************************************************/

/* Window position enumeration. */
enum SPxWindowPosition
{
    SPX_WIN_POS_CENTRE_SCREEN = 0,
    SPX_WIN_POS_CENTRE_PARENT = 1,
    SPX_WIN_POS_CURSOR = 2,
    SPX_WIN_POS_EDGE_PARENT = 3
};

/* Structure for storing fullscreen state. */
#define SPX_FULL_SCREEN_MAX_WINDOWS (3)
// cppcheck-suppress ctuOneDefinitionRuleViolation
struct SPxFullScreenState
{
    int fullscreen;
    unsigned int numWins;
    struct
    {
        HWND win;
        UINT showCmd;
        RECT size;
        RECT normSize;
        DWORD style;
        DWORD exStyle;
    } win[SPX_FULL_SCREEN_MAX_WINDOWS];
};

/* Automatically load/free a DLL. */
// cppcheck-suppress ctuOneDefinitionRuleViolation
struct SPxWinLibrary
{
    SPxWinLibrary()
	: m_hmodule(NULL) {}
    SPxWinLibrary(const char *lib)
	: m_hmodule(NULL) { Load(lib); }
    ~SPxWinLibrary(void)
    {
	Free();
    }
    HMODULE Load(const char *lib)
    {
	Free();
	m_hmodule = ::LoadLibrary(SPX_T16(lib));
	return m_hmodule;
    }
    void Free()
    {
	if(m_hmodule)
	{
	    ::FreeLibrary(m_hmodule);
	}
	m_hmodule = NULL;
    }
    operator HMODULE()
    {
	return m_hmodule;
    }
    HMODULE m_hmodule;
};

/* Automatically set/reset DPI thread awareness. */
class SPxThreadDpiAwareness
{
public:
    SPxThreadDpiAwareness(SPxDpiAwareness dpiAwareness);
    ~SPxThreadDpiAwareness(void);
    void Reset(void);

private:
    SPxThreadDpiAwareness(const SPxThreadDpiAwareness&) SPX_FUNCTION_DELETE;
    SPxThreadDpiAwareness& operator=(const SPxThreadDpiAwareness&) SPX_FUNCTION_DELETE;

    SPxDpiAwareness m_oldDpiAwareness;
};

/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

extern unsigned int SPxFlushWinMsgQueue(void);

extern void SPxGetMaxScreenSize(unsigned int *wRtn, 
                                unsigned int *hRtn);

extern UINT32 SPxCOLORREFtoARGB(COLORREF cr, UINT8 alpha=255);

extern COLORREF SPxRGBtoCOLORREF(UINT32 rgb);

extern SPxErrorCode SPxFullScreenToggle(HWND window,
                                        SPxFullScreenState *state,
                                        HMONITOR monitor=NULL,
                                        unsigned int numExtraWindows=0,
                                        HWND *extraWindows=NULL);

extern SPxErrorCode SPxRotateImage(HDC dstHdc,
                                   HDC srcHdc, 
                                   unsigned int srcWidth, 
                                   unsigned int srcHeight, 
                                   double rotationDegs,
                                   int centreX,
                                   int centreY,
                                   int translateX=0,
                                   int translateY=0);

extern UINT32 SPxBGRtoARGB(COLORREF bgr);

extern SPxDpiAwareness SPxSetThreadDpiAwareness(SPxDpiAwareness dpiAwareness);

extern double SPxGetDpiScale(HWND win=NULL);

extern int SPxGetDpiScaledSize(int size, HWND win=NULL);

extern long SPxGetDpiScaledSize(long size, HWND win=NULL);

extern unsigned int SPxGetDpiScaledSize(unsigned int size, HWND win=NULL);

extern float SPxGetDpiScaledSize(float size, HWND win=NULL);

extern double SPxGetDpiScaledSize(double size, HWND win=NULL);

extern SPxErrorCode SPxGetPhysicalDpi(HWND win, unsigned int *dpi);

SPxErrorCode SPxGetMonitorDpi(HWND win, unsigned int *dpi);

extern int SPxGetSystemMetrics(int index, HWND win=NULL);

extern BOOL SPxAdjustWindowRectEx(LPRECT lpRect, HWND hwnd);

extern void SPxWindowMoveToPosition(HWND win, SPxWindowPosition winPos,
				    HWND parent = NULL);

extern void SPxWindowMoveToVisible(HWND win);

extern HWND SPxGetTopWindow(HWND win);

extern void SPxSetWindowOwner(HWND win, HWND owner);

extern int SPxCheckWindowHasFocus(HWND win);

extern int SPxIsGuiThread(HWND win);

extern HMONITOR SPxWindowGetNearestMonitor(HWND wnd, unsigned int numExcludeMons,
                                           HMONITOR *excludeMons);

extern void SPxSetResourceModuleName(const char *resModule);
extern HMODULE SPxGetResourceModule(void);

extern SPxErrorCode SPxSetAUMID(const char *aumid);

#endif /* _SPX_WIN32UTILS_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/

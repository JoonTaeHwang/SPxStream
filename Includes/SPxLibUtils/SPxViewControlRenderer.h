/*********************************************************************
*
* (c) Copyright 2012 - 2021, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxViewControlRenderer base class which supports the
*	rendering of view control objects: view control soft buttons,
*	range scale, and cursor position.
*
* Revision Control:
*   12/11/21 v1.27   AGC	Support showing Georef position.
*
* Previous Changes:
*   02/02/21 1.26   REW	Support showing doppler at cursor position.
*   15/10/20 1.25   AGC	Save zoom mode configuration.
*   01/10/20 1.24   AGC	Support rubber-band zoom.
*   10/01/19 1.23   BTB	Use home view view control (if set) for centre button.
*   07/01/19 1.22   BTB	Support background box and border for cursor pos text.
*   02/01/19 1.21   BTB	Support rendering flashing text at the top of the screen.
*   13/12/18 1.20   BTB	Make sure range scale and cursor pos elements won't clash.
*   30/11/18 1.19   AGC	Add range in NM cursor option.
*   19/11/18 1.18   AGC	High DPI improvements.
*   06/07/18 1.17   BTB	Add multi-language support for stored buttons.
*   24/05/18 1.16   SP 	Add SetMenuItemsEnabled().
*   15/05/18 1.15   SP 	Add SetMenuItemsCaps().
*   11/09/17 1.14   AGC	Improve soft button image scaling.
*   13/01/17 1.13   REW	Add SPX_CURSOR_POS_OPT_UTM/MGRS etc.
*   09/09/16 1.12   AGC	Remove non-existent renderViewButtons function.
*   04/12/15 1.11   REW	Support stored views.
*   02/12/15 1.10   REW	Use long-click to set home view.
*   09/02/15 1.9    AGC	Use base class soft button support.
*   16/05/14 1.8    AGC	Add context menu item.
*   17/02/14 1.7    SP 	Support shaded range scale background.
*   07/10/13 1.6    AGC	Move rendering to renderAll().
*   25/01/13 1.5    AGC	Add height cursor position option.
*   11/01/13 1.4    AGC	Remove need for nav data object.
*   12/12/12 1.3    AGC	Support multiple cursor position options.
*			Add north marker support.
*			Support heading-up/moving platform.
*   23/11/12 1.2    AGC	Implement SPxMouseInterface functions.
*   09/11/12 1.1    AGC	Initial version.
**********************************************************************/
#ifndef _SPX_VIEW_CONTROL_RENDERER_H
#define _SPX_VIEW_CONTROL_RENDERER_H

/*
 * Other headers required.
 */

/* For SPxAutoPtr. */
#include "SPxLibUtils/SPxAutoPtr.h"

/* For SPxZoomMode_t. */
#include "SPxLibUtils/SPxMouseControl.h"

/* For SPxSoftButton callback type. */
#include "SPxLibUtils/SPxSoftButton.h"

/* For base class. */
#include "SPxLibUtils/SPxRenderer.h"

/*
 * Types
 */

enum SPxCursorPosOptions_t
{
    SPX_CURSOR_POS_OPT_NONE	    = 0x0000,
    SPX_CURSOR_POS_OPT_RANGE_AZI    = 0x0001,
    SPX_CURSOR_POS_OPT_X_Y	    = 0x0002,
    SPX_CURSOR_POS_OPT_LAT_LONG	    = 0x0004,	/* Changeable renderer units */
    SPX_CURSOR_POS_OPT_LAND_SEA	    = 0x0008,
    SPX_CURSOR_POS_OPT_HEIGHT	    = 0x0010,
    SPX_CURSOR_POS_OPT_UTM	    = 0x0020,	/* Fixed format */
    SPX_CURSOR_POS_OPT_MGRS	    = 0x0040,	/* Fixed format */
    SPX_CURSOR_POS_OPT_LAT_LONG_D   = 0x0080,	/* Fixed format */
    SPX_CURSOR_POS_OPT_LAT_LONG_DM  = 0x0100,	/* Fixed format */
    SPX_CURSOR_POS_OPT_LAT_LONG_DMS = 0x0200,	/* Fixed format */
    SPX_CURSOR_POS_OPT_RANGE_NM	    = 0x0400,	/* Range in Nautical Miles */
    SPX_CURSOR_POS_OPT_DOPPLER 	    = 0x0800,	/* Doppler if available */
    SPX_CURSOR_POS_OPT_GEOREF	    = 0x1000	/* Fixed format */
};

/* Forward declarations. */
class SPxWorldMap;
class SPxTerrainDatabase;
class SPxMouseControl;
class SPxSoftButton;
class SPxDopplerStore;

typedef SPxErrorCode (*SPxViewCtrlRendererFn_t)(SPxSoftButton *button, void *userArg);

/* Define our class */
class SPxViewControlRenderer : virtual public SPxRenderer
{
public:
    /* Constructor and destructor. */
    explicit SPxViewControlRenderer(SPxWorldMap *worldMap=NULL,
				    SPxTerrainDatabase *tdb=NULL,
				    SPxMouseControl *mouseCtrl=NULL);
    virtual ~SPxViewControlRenderer(void);
    virtual SPxErrorCode Create(void);
    SPxErrorCode SaveInitialView(void);
    SPxErrorCode SaveHomeView(void);
    
    /* Functions for handling mouse interaction. */
    virtual int HandleMousePress(UINT32 flags, int x, int y, SPxCursor_t *cursor);
    virtual int HandleMouseRelease(UINT32 flags, int x, int y, SPxCursor_t *cursor);
    virtual int HandleMouseMove(UINT32 flags, int x, int y, SPxCursor_t *cursor);
    virtual int HandleMouseLeave(void);

    /* View buttons configuration functions. */
    SPxErrorCode SetViewButtonsEnabled(int state);
    int GetViewButtonsEnabled(void) const;
    SPxErrorCode SetViewCentreOnPlatformEnabled(int state);
    int GetViewCentreOnPlatformEnabled(void) const;
    SPxErrorCode SetNumStoredViewButtons(unsigned int num);
    unsigned int GetNumStoredViewButtons(void) const;

    /* Range scale configuration functions. */
    SPxErrorCode SetRangeScaleEnabled(int state);
    int GetRangeScaleEnabled(void) const;
    SPxErrorCode SetRangeScaleColour(UINT32 col);
    UINT32 GetRangeScaleColour(void) const;
    SPxErrorCode SetRangeScaleFillColour(UINT32 col);
    UINT32 GetRangeScaleFillColour(void) const;
    SPxErrorCode SetRangeScaleBorderColour(UINT32 col);
    UINT32 GetRangeScaleBorderColour(void) const;

    /* Cursor position configuration functions. */
    SPxErrorCode SetCursorPosEnabled(int state);
    int GetCursorPosEnabled(void) const;
    SPxErrorCode SetCursorPosColour(UINT32 col);
    UINT32 GetCursorPosColour(void) const;
    SPxErrorCode SetCursorPosFillColour(UINT32 col);
    UINT32 GetCursorPosFillColour(void) const;
    SPxErrorCode SetCursorPosBorderColour(UINT32 col);
    UINT32 GetCursorPosBorderColour(void) const;
    SPxErrorCode SetCursorPosOptions(UINT32 options);
    UINT32 GetCursorPosOptions(void) const;

    /* North marker configuration functions. */
    SPxErrorCode SetNorthMarkerEnabled(int state);
    int GetNorthMarkerEnabled(void) const;
    SPxErrorCode SetNorthMarkerColour(UINT32 col);
    UINT32 GetNorthMarkerColour(void) const;

    /* Functions for specifying text to flash at the top of the screen. */
    SPxErrorCode SetTextToFlash(const char *text);
    SPxErrorCode SetTextFlashingEnabled(int state);

    /* Zoom mode configuration. */
    SPxErrorCode SetZoomMode(SPxZoomMode_t zoomMode);
    SPxZoomMode_t GetZoomMode(void) const;

    /* Rubber-band zoom configuration. */
    SPxErrorCode SetRubberBandZoomButton(UINT32 button);
    UINT32 GetRubberBandZoomButton(void) const;
    SPxErrorCode SetRubberBandZoomOneshot(int state);
    int GetRubberBandZoomOneshot(void) const;
    SPxErrorCode SetRubberBandZoomFillColour(UINT32 col);
    UINT32 GetRubberBandZoomFillColour(void) const;
    SPxErrorCode SetRubberBandZoomLineColour(UINT32 col);
    UINT32 GetRubberBandZoomLineColour(void) const;

    /* Misc. */
    SPxErrorCode SetMenuItemsEnabled(int state);
    SPxErrorCode SetMenuItemsCaps(int state);

    /* Doppler store link. */
    SPxErrorCode SetDopplerStore(SPxDopplerStore *ds);
    SPxDopplerStore *GetDopplerStore(void) const;

    /* Generic parameter assignment. */
    virtual int SetParameter(char *parameterName, char *parameterValue);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);

protected:
    SPxErrorCode CreateButtonsFromResource(void);
    SPxErrorCode CreateButtonsFromXPM(void);

private:
    /* Private variables. */
    struct impl;
    SPxAutoPtr<impl> m_p;

    /* Private functions. */
    virtual SPxErrorCode renderAll(void);
    SPxErrorCode renderRangeScale(void);
    SPxErrorCode attemptRangeScaleRendering(double minLength, double maxLength, int forceRendering,
					    int *isRenderedRtn, double *widthRtn);
    SPxErrorCode renderCursorPos(void);
    SPxErrorCode getCursorTextPosition(SPxRendererXY_t *pos, const char *text, double *widthRtn);
    SPxErrorCode renderNorthMarker(void);
    SPxErrorCode renderRubberBand(void);
    SPxErrorCode renderFlashingText(void);
    SPxSoftButton *createButton(int bitmap, int moBitmap, int x, int y,
				SPxSoftButtonClickedFn_t clickedFn,
				SPxSoftButtonClickedFn_t longClickedFn=NULL,
				int bnW=-2, int bnH=-2, int moBnW=-2, int moBnH=-2);
    SPxSoftButton *createButton(const char *bitmap, const char *moBitmap,
				int x, int y,
				SPxSoftButtonClickedFn_t clickedFn,
				SPxSoftButtonClickedFn_t longClickedFn=NULL,
				int bnW = -2, int bnH = -2, int moBnW = -2, int moBnH = -2);
    SPxErrorCode setStoredButtonText(void);
    SPxErrorCode updateStoredButtonVisibilities(void);

    /* Softbutton handlers. */
    static void upFnWrapper(SPxSoftButton *btn, void *userArg);
    void upFn(SPxSoftButton *btn);
    static void downFnWrapper(SPxSoftButton *btn, void *userArg);
    void downFn(SPxSoftButton *btn);
    static void leftFnWrapper(SPxSoftButton *btn, void *userArg);
    void leftFn(SPxSoftButton *btn);
    static void rightFnWrapper(SPxSoftButton *btn, void *userArg);
    void rightFn(SPxSoftButton *btn);
    static void centreFnWrapper(SPxSoftButton *btn, void *userArg);
    void centreFn(SPxSoftButton *btn);
    static void centreLongFnWrapper(SPxSoftButton *btn, void *userArg);
    void centreLongFn(SPxSoftButton *btn);
    static void zinFnWrapper(SPxSoftButton *btn, void *userArg);
    void zinFn(SPxSoftButton *btn);
    static void zoutFnWrapper(SPxSoftButton *btn, void *userArg);
    void zoutFn(SPxSoftButton *btn);
    static void zoomModeFnWrapper(SPxSoftButton *btn, void *userArg);
    void zoomModeFn(SPxSoftButton *btn);
    static void storedFn0Wrapper(SPxSoftButton *btn, void *userArg);
    static void storedFn1Wrapper(SPxSoftButton *btn, void *userArg);
    static void storedFn2Wrapper(SPxSoftButton *btn, void *userArg);
    static void storedFn3Wrapper(SPxSoftButton *btn, void *userArg);
    void storedFn(SPxSoftButton *btn, unsigned int idx);
    static void storedLongFn0Wrapper(SPxSoftButton *btn, void *userArg);
    static void storedLongFn1Wrapper(SPxSoftButton *btn, void *userArg);
    static void storedLongFn2Wrapper(SPxSoftButton *btn, void *userArg);
    static void storedLongFn3Wrapper(SPxSoftButton *btn, void *userArg);
    void storedLongFn(SPxSoftButton *btn, unsigned int idx);

    /* Context menu handlers. */
    static void recentreFnWrapper(SPxPopupMenu *popupMenu, int id, void *userArg);
    void recentreFn(void);

}; /* SPxViewControlRenderer */

#endif /* _SPX_VIEW_CONTROL_RENDERER_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/

/*********************************************************************
*
* (c) Copyright 2011 - 2022, Cambridge Pixel Ltd.
*
* Purpose:
*   Base class used for platform independent graphics renderer 
*   in the SPx library. See cpp header for extra info.
*
*
* Revision Control:
*   15/09/22 v1.24   AGC	Update offscreen bitmaps on DPI change.
*
* Previous Changes:
*   17/02/22 1.23   AGC	Add renderPoly() overload taking angle.
*   09/02/22 1.22   AGC	Support destroying soft buttons.
*   04/06/21 1.21   AGC	Add anti-alias option.
*   01/04/21 1.20   AGC	Add renderLine() overload taking length and angle.
*   05/03/21 1.19   AGC	Replace offscreen bitmap composite option with alpha option.
*   26/02/21 1.18   BTB	Support setting compositing mode on offscreen bitmaps.
*   12/02/21 1.17   SP 	Add alignment option to drawCaption().
*   17/09/20 1.16   AGC	Add requestRedraw() function.
*   27/08/20 1.15   AGC	Add IncludeLabelOffsetsInPositions().
*   26/08/20 1.14   AGC	Add GetRealDeviceScale().
*			Add GetPhysicalDpi().
*			Add getTextExtents() and renderText() overloads
*			for excluding overhangs.
*   06/12/19 1.13   AGC	Add IsRefPointLatLongSet().
*   31/10/19 1.12   AGC	Add renderText() overloads for rotated text.
*   11/07/19 1.11   AGC	Add double overload for drawCaption().
*   10/01/19 1.10   BTB	Support setting colour of caption text box border line.
*   09/01/19 1.70   BTB	Support setting colour of caption title and body text boxes.
*   13/12/18 1.69   BTB	Support adjusting text font size.
*			Add IsRadarLatLongSet().
*   07/12/18 1.68   AGC	Add double overload of renderPolyGradient().
*   06/12/18 1.67   BTB	Add renderSectorPartialGradient().
*			Add double overload of renderSectorPartialGradient().
*   30/11/18 1.66   AGC	Support off-screen bitmaps.
*   26/11/18 1.65   AGC	Add double overload of renderSectorGradient().
*   26/11/18 1.64   AGC	Support bold/italic text.
*   19/11/18 1.63   AGC	Match default args for double variant of renderArc().
*   18/10/18 1.62   REW	Add support for reference objects etc.
*   18/10/18 1.61   AGC	Improve handling of soft button double-clicks.
*   25/06/18 1.60   AGC	Correctly suppress cppcheck warnings.
*   16/05/18 1.59   BTB	Allow configurable caption length.
*   09/05/18 1.58   BTB	Fix line endings.
*   08/05/18 1.57   BTB	Move drawCaption from SPxSimMotionRenderer to allow wider use.
*   20/04/18 1.56   REW	Add GetWinXYFromRA().
*   27/02/18 1.55   AGC	Add double versions of renderPoint/renderImage().
*   02/02/18 1.54   AGC	Improve units support.
*   06/10/17 1.53   SP 	Add SPxUniTrackRenderer as a friend.
*                       Add renderDragDropLasso().
*   31/08/17 1.52   AGC	Add double versions of renderArc/renderSector().
*   03/08/17 1.51   AJH	Add user-supplied rotation offset.
*   01/08/17 1.50   AGC	Add double version of renderText().
*   26/06/17 1.49   SP 	Use renamed SPX_FORMAT_LL_MAX.
*   22/06/17 1.48   AGC	Add floating point overload for renderLine().
*   19/10/16 1.47   AGC	Add floating point overloads for renderPoly/renderRectangle().
*   23/09/16 1.46   REW	Use SPxUnitsDist_t, SPxUnitsSpeed_t and
*			SPxFormatLatLong_t.
*   09/09/16 1.45   AGC	Add GetMousePos().
*   08/07/16 1.44   AGC	Support feet for height units.
*   15/06/16 1.43   AGC	Make various functions const.
*   11/03/16 1.42   SP 	Add DPI scaling utility functions.
*   07/01/16 1.41   AGC	Add floating point overload for renderEllipse().
*   04/12/15 1.40   REW	Support mouseOverBitmap in SPxSoftButtons.
*			Tidy line endings.
*   09/11/15 1.39   AGC	Remove old style display units functions/types.
*			Support config of speed display units.
*			Add normalise flag to GetLatLongFromWinXY().
*   10/09/15 1.38   AGC	Change in SPxPopuMenuCreate() params.
*   12/06/15 1.37   AGC	Support user specified soft button sizes.
*   14/04/15 1.36   AGC	Add new perspective renderImage() overload.
*   09/02/15 1.35   AGC	Support automatic soft button management.
*   24/11/14 1.34   SP 	Add setClipRectangle().
*   20/10/14 1.33   REW	Support separate PointWidth.
*   18/07/14 1.32   AGC	Add GetDeviceScale() function.
*   29/01/14 1.31   AGC	Add double based overloads of Get...FromWinXY() functions.
*   15/01/14 1.30   AGC	Add double based overloads of GetWinXY...() functions.
*   06/12/13 1.29   AGC	Reduce memory allocations when rendering.
*   21/10/13 1.28   AGC	Support passing SPxWinMenu to SPxPopupMenuCreate under Windows.
*   15/10/13 1.27   AGC	Add GetViewControl() function.
*   08/10/13 1.26   AGC	Replace createPopupMenu() with SPxPopupMenuCreate().
*   07/10/13 1.25   AGC	Add more detailed form for SetDash().
*			Add clip rectangle functions.
*			Add background colour and clear functions.
*			Add named colour functions.
*			Move derived class rendering to renderAll().
*			Add more options to renderArc() and renderSector().
*   04/10/13 1.24   SP 	Add applyRotation() with integer args.
*   17/09/13 1.23   SP 	Support multiple view control platforms.
*   23/08/13 1.22   SP 	Add renderSector() and renderSectorGradient().
*   14/08/13 1.21   AGC Add renderImage().
*			Add renderEllipse().
*   12/02/13 1.20   AGC	Make applyRotation() protected.
*   25/01/13 1.19   AGC	Support polygons with gradients.
*   21/12/12 1.18   AGC	Add GetRAFromWinXY().
*			Add createPopupMenu().
*   12/12/12 1.17   AGC	Support configurable bearing units.
*   23/11/12 1.16   AGC	Add SPxRendererFlags_t enum.
*			Derive from SPxMouseInterface.
*   21/08/12 1.15   SP 	Move some utility functions to SPxTiledMap.
*   18/07/12 1.14   SP 	Rename DiffAngle() as CalculateRotation().
*   21/06/12 1.13   SP 	Add missing 'get' functions.
*   06/06/12 1.12   SP 	Add some utility functions.
*   08/05/12 1.11   SP	Use new lat/long conversion functions.
*                       Support both metres and lat/long view centre.
*   29/02/12 1.10   SP	Use new lat/long conversion functions.
*   20/02/12 1.9    AGC	More explicit lat/long, metres specification.
*   08/02/12 1.8    REW	Avoid compiler warnings.
*   08/02/12 1.7    SP 	Add display unit configuration.
*			Add SetRadarPositionLatLong().
*			Add dash configuration.
*			Support update from SPxViewControl.
*			Override SetActive().
*   13/01/12 1.6    SP 	Add IsLatLongSupported().
*   20/12/11 1.5    SP 	Add renderPoint().
*   27/07/11 1.4    SP 	Add SetRadarPositionMetres().
*                       Add SetRotationDegs().
*                       Make SetFont() and friends public.
*   27/06/11 1.3    REW	Add public coordinate conversion functions.
*   26/04/11 1.2    SP 	Add view control.  Add new rendering functions.
*   06/04/11 1.1    SP 	Initial Version.
**********************************************************************/

#ifndef _SPX_RENDERER_H
#define _SPX_RENDERER_H

#ifdef _WIN32
// Disable warning message 4250 (inheritance via dominance).
#pragma warning(disable : 4250)
#endif

/*
 * Other headers required.
 */

/* Include common types, callbacks, errors etc. */
#include "SPxLibUtils/SPxObj.h"
#include "SPxLibUtils/SPxError.h"
#include "SPxLibUtils/SPxLatLongUtils.h"
#include "SPxLibUtils/SPxStrings.h"
#include "SPxLibUtils/SPxViewControl.h"
#include "SPxLibUtils/SPxMouseControl.h"
#include "SPxLibUtils/SPxTypes.h"

#ifdef _WIN32
#include "SPxLibWin/SPxPopupMenuWin.h"
#else
#include "SPxLibX11/SPxPopupMenuGtk.h"
#endif

/*********************************************************************
*
*   Constants
*
**********************************************************************/


/*********************************************************************
*
*   Macros
*
**********************************************************************/


/*********************************************************************
*
*   Type definitions
*
**********************************************************************/

typedef enum
{
    SPX_RENDERER_FLAGS_LEFT	= 0x01,
    SPX_RENDERER_FLAGS_MIDDLE	= 0x02,
    SPX_RENDERER_FLAGS_RIGHT	= 0x04,
    SPX_RENDERER_FLAGS_CONTROL	= 0x08,
    SPX_RENDERER_FLAGS_SHIFT	= 0x10

} SPxRendererFlags_t;

/* Distance display units, for backwards compatibility. */
typedef SPxUnitsDist_t SPxRendererDistUnits_t;
#define SPX_RENDERER_UNITS_METRES		SPX_UNITS_METRES
#define SPX_RENDERER_UNITS_KILOMETRES		SPX_UNITS_KILOMETRES
#define SPX_RENDERER_UNITS_NAUTICAL_MILES	SPX_UNITS_NAUTICAL_MILES
#define SPX_RENDERER_UNITS_FEET			SPX_UNITS_FEET
#define SPX_RENDERER_UNITS_DIST_MAX		SPX_UNITS_DIST_MAX

/* Bearing display units, for backwards compatibility. */
typedef SPxFormatLatLong_t SPxRendererBearingUnits_t;
#define SPX_RENDERER_UNITS_DEGS			SPX_FORMAT_LL_DEGS
#define SPX_RENDERER_UNITS_DEGS_MINS		SPX_FORMAT_LL_DEGS_MINS
#define SPX_RENDERER_UNITS_DEGS_MINS_SECS	SPX_FORMAT_LL_DEGS_MINS_SECS
#define SPX_RENDERER_UNITS_BEARING_MAX		SPX_FORMAT_LL_MAX

/* Speed display units, for backwards compatibility. */
typedef SPxUnitsSpeed_t SPxRendererSpeedUnits_t;
#define SPX_RENDERER_UNITS_MPS			SPX_UNITS_MPS
#define SPX_RENDERER_UNITS_KMPH			SPX_UNITS_KMPH
#define SPX_RENDERER_UNITS_KNOTS		SPX_UNITS_KNOTS
#define SPX_RENDERER_UNITS_SPEED_MAX		SPX_UNITS_SPEED_MAX

/* Max characters in captions. */
#define SPX_RENDERER_MAX_CAPTION_LENGTH		18

/* Max number of reference points. */
#define	SPX_RENDERER_MAX_REF_POINTS		4

/* Coordinate pair. This must be scalar, so that "=" works for copying. */
typedef struct SPxRendererXY_tag 
{
    int x;
    int y;

} SPxRendererXY_t;

typedef struct SPxRendererXYD_tag
{
    double x;
    double y;

} SPxRendererXYD_t;

/* Text rendering modes */
typedef enum
{
    SPX_TEXT_RENDER_DEFAULT,    /* Default for platform */
    SPX_TEXT_RENDER_ANTI_ALIAS  /* Anti alias */

} SPxTextRenderingMode_t;

/* List of built-in dash types */
typedef enum
{
    SPX_RENDERER_DASH_SOLID	= 0,
    SPX_RENDERER_DASH_DASH	= 1,
    SPX_RENDERER_DASH_DOT	= 2,
    SPX_RENDERER_DASH_DASH_DOT	= 3,
    SPX_RENDERER_DASH_MAX /* MUST BE LAST ENTRY */

} SPxRendererDash_t;

/* List of font styles. */
typedef enum
{
    SPX_RENDERER_FONT_STYLE_NORMAL = 0,
    SPX_RENDERER_FONT_STYLE_BOLD = 1,
    SPX_RENDERER_FONT_STYLE_ITALIC = 2

} SPxRendererFontStyle_t;

/* Selection of which object to reference graphics to. */
typedef enum
{
    SPX_RENDERER_REFOBJ_DEFAULT	= 0,	/* Typically first radar/sensor */
    SPX_RENDERER_REFOBJ_REFPT = 1	/* Installed reference point */
} SPxRendererRefObj_t;

/* Alignment options. */
typedef enum 
{
    SPX_RENDERER_ALIGN_TL   = 1,    /* Top left. */
    SPX_RENDERER_ALIGN_BL   = 2,    /* Top right. */
    SPX_RENDERER_ALIGN_TR   = 3,    /* Bottom left. */
    SPX_RENDERER_ALIGN_BR   = 4     /* Bottom right. */

} SPxRendererAlignment_t;

/* Compositing modes. */
typedef enum
{
    SPX_RENDERER_COMP_MODE_SRC_OVER = 0,
    SPX_RENDERER_COMP_MODE_SRC_COPY = 1

} SPxRendererCompositingMode_t;

/* Forward declarations. */
class SPxPopupMenu;
class SPxImage;
class SPxRefPoint;
class SPxSoftButton;

/*
 * Define our class, derived from the SPx base object.
 */
class SPxRenderer : public SPxObj, public SPxMouseInterface
{
public:
    /* Add these classes as friends so that they can access the 
     * protected rendering functions in this class.
     */
    friend class SPxSymbol;
    friend class SPxSoftButton;
    friend class SPxUniTrackRenderer;

    typedef void (*SoftButtonFn_t)(SPxSoftButton *button, void *userArg);

    /*
     * Public functions.
     */

    /* Constructor and destructor */
    SPxRenderer(void);
    virtual ~SPxRenderer(void);
  
    /* View control. */
    virtual int IsLatLongInUse(void) const { return m_isLatLongInUse; }

    virtual int IsRadarLatLongSet(void) const { return m_radarLatLongSet; }

    int IsRefPointLatLongSet(void) const;

    virtual SPxErrorCode SetViewCentreLatLong(double latDegs, 
                                              double longDegs);

    virtual SPxErrorCode SetViewCentreMetres(double xMetres, 
                                             double yMetres);

    virtual SPxErrorCode SetViewSizePixels(unsigned int widthPixels,
                                           unsigned int heightPixels);

    virtual SPxErrorCode SetViewPixelsPerMetre(double pixelsPerMetre);

    virtual SPxErrorCode SetRadarPositionMetres(double xMetres, 
                                                double yMetres);

    virtual SPxErrorCode SetRadarPositionLatLong(double latDegs,
                                                 double longDegs);

    /* Configurable objects to reference graphics relative to. */
    virtual SPxErrorCode SetRefPoint(const SPxRefPoint *refPt,
					unsigned int idx);
    virtual const SPxRefPoint *GetRefPoint(unsigned int idx) const;

    /* Selection of which object to reference graphics relative to. */
    virtual SPxErrorCode SelectReferenceObject(SPxRendererRefObj_t objType,
						unsigned int idx=0);
    virtual SPxRendererRefObj_t GetReferenceObjectType(void) const
    {
	return(m_refObjectType);
    }
    virtual unsigned int GetReferenceObjectIdx(void) const
    {
	return(m_refObjectIdx);
    }

    /* User-selectable Rotation. */
    virtual SPxErrorCode SetRotationDegs(double degs);
    virtual double GetRotationDegs(void){ return(m_rotationDegs); }
    virtual SPxErrorCode SetRotationOffsetDegs(double degs);
    virtual double GetRotationOffsetDegs(void) { return( m_rotationOffsetDegs ); }
    
    virtual double GetViewCentreLatDegs(void) const { return(m_viewCentreLatDegs); }
    virtual double GetViewCentreLongDegs(void) const { return(m_viewCentreLongDegs); }
    virtual double GetViewCentreXMetres(void) const { return(m_viewCentreXMetres); }
    virtual double GetViewCentreYMetres(void) const { return(m_viewCentreYMetres); }
    virtual unsigned int GetViewWidthPixels(void) const { return(m_viewWidthPixels); }
    virtual unsigned int GetViewHeightPixels(void) const { return(m_viewHeightPixels); }
    virtual double GetViewPixelsPerMetre(void) const { return(m_viewPixelsPerMetre); }
    virtual double GetRadarPositionXMetres(void) const { return(m_radarXMetres); }
    virtual double GetRadarPositionYMetres(void) const { return(m_radarYMetres); }
    virtual double GetRadarPositionLatDegs(void) const { return(m_radarLatDegs); }
    virtual double GetRadarPositionLongDegs(void) const { return(m_radarLongDegs); }
    virtual double GetRefPositionXMetres(void) const;
    virtual double GetRefPositionYMetres(void) const;
    virtual double GetRefPositionLatDegs(void) const;
    virtual double GetRefPositionLongDegs(void) const;

    /* Coordinate conversion utility functions. */
    virtual SPxErrorCode GetWinXYFromLatLong(double latDegs,
						double longDegs,
						double *xPixelsRtn,
						double *yPixelsRtn,
						int *isInWindowRtn=0) const;
    virtual SPxErrorCode GetWinXYFromLatLong(double latDegs,
						double longDegs,
						int *xPixelsRtn,
						int *yPixelsRtn,
						int *isInWindowRtn=0) const;
    virtual SPxErrorCode GetWinXYFromMetres(double xMetres,
						double yMetres,
						double *xPixelsRtn,
						double *yPixelsRtn,
						int *isInWindowRtn=0) const;
    virtual SPxErrorCode GetWinXYFromMetres(double xMetres,
						double yMetres,
						int *xPixelsRtn,
						int *yPixelsRtn,
						int *isInWindowRtn=0) const;
    virtual SPxErrorCode GetWinXYFromRA(double rangeMetres,
						double angleDegs,
						int *xPixelsRtn,
						int *yPixelsRtn,
						int *isInWindowRtn=0) const;
    virtual SPxErrorCode GetWinXYFromRA(double rangeMetres,
						double angleDegs,
						double *xPixelsRtn,
						double *yPixelsRtn,
						int *isInWindowRtn=0) const;
    virtual SPxErrorCode GetLatLongFromWinXY(double xPixels,
						double yPixels,
						double *latDegsRtn,
						double *longDegsRtn,
						int normalise=TRUE) const;
    virtual SPxErrorCode GetLatLongFromWinXY(int xPixels,
						int yPixels,
						double *latDegsRtn,
						double *longDegsRtn,
						int normalise=TRUE) const;
    virtual SPxErrorCode GetMetresFromWinXY(double xPixels,
						double yPixels,
						double *xMetresRtn,
						double *yMetresRtn) const;
    virtual SPxErrorCode GetMetresFromWinXY(int xPixels,
						int yPixels,
						double *xMetresRtn,
						double *yMetresRtn) const;
    virtual SPxErrorCode GetRAFromWinXY(int xPixels,
						int yPixels,
						double *rangeMetresRtn,
						double *angleDegsRtn) const;

    /* Install a view control object. */
    SPxErrorCode SetViewControl(SPxViewControl *obj,
                                unsigned int radarIndex=0,
                                unsigned int platformIndex=0);
    SPxViewControl *GetViewControl(void) const;

    /* Functions to configure global rendering settings */
    // cppcheck-suppress virtualCallInConstructor
    virtual SPxErrorCode SetLineWidth(double width);
    virtual double GetLineWidth(void) { return m_lineWidth; }
    // cppcheck-suppress virtualCallInConstructor
    virtual SPxErrorCode SetPointWidth(double width);
    virtual double GetPointWidth(void) { return m_pointWidth; }
    virtual SPxErrorCode SetDash(SPxRendererDash_t dash);
    virtual SPxErrorCode SetDash(const double *dashes, unsigned int numDashes, double dashOffsetPixels);
    virtual SPxErrorCode GetDash(double *dashes, unsigned int *numDashes, double *dashOffsetPixels);
    // cppcheck-suppress virtualCallInConstructor
    virtual SPxErrorCode SetFont(const char *name, unsigned int size,
				 SPxRendererFontStyle_t style=SPX_RENDERER_FONT_STYLE_NORMAL);
    virtual const char *GetFontName(void) { return m_fontName; }
    virtual unsigned int GetFontSize(void) { return m_fontSize; }
    virtual SPxErrorCode SetFontSizeAdjustment(int adjustment);
    virtual int GetFontSizeAdjustment(void) { return m_fontSizeAdjustment; }
    virtual unsigned int GetAdjustedFontSize(void) { return m_adjustedFontSize; }
    virtual SPxRendererFontStyle_t GetFontStyle(void) { return m_fontStyle; }
    virtual SPxErrorCode SetTextRenderingMode(SPxTextRenderingMode_t mode);
    virtual SPxTextRenderingMode_t GetTextRenderingMode(void);
    SPxErrorCode SetAntiAliasEnabled(int state);
    int GetAntiAliasEnabled(void) const;
    virtual SPxErrorCode SetDistDisplayUnits(SPxUnitsDist_t units);
    virtual SPxUnitsDist_t GetDistDisplayUnits(void);
    virtual SPxErrorCode SetHeightDisplayUnits(SPxUnitsHeight_t units);
    virtual SPxUnitsHeight_t GetHeightDisplayUnits(void);
    virtual SPxErrorCode SetBearingDisplayUnits(SPxFormatLatLong_t units);
    virtual SPxFormatLatLong_t GetBearingDisplayUnits(void);
    virtual SPxErrorCode SetSpeedDisplayUnits(SPxUnitsSpeed_t units);
    virtual SPxUnitsSpeed_t GetSpeedDisplayUnits(void);
    SPxErrorCode SetSoftButtonsEnabled(int state);
    int GetSoftButtonsEnabled(void) const;

    virtual SPxErrorCode FillClipRect(UINT32 argb);
    virtual SPxErrorCode FillClipRect(UINT8 r, UINT8 g, UINT8 b, UINT8 a=255);
    virtual SPxErrorCode FillClipRect(const char *name);

    virtual SPxErrorCode SetBackgroundColour(UINT32 argb);
    virtual SPxErrorCode SetBackgroundColour(UINT8 r, UINT8 g, 
                                             UINT8 b, UINT8 a=0xFF);
    virtual SPxErrorCode SetBackgroundColour(const char *name);
    virtual UINT32 GetBackgroundColour(void);
    virtual UINT8 GetBackgroundColourA(void);
    virtual UINT8 GetBackgroundColourR(void);
    virtual UINT8 GetBackgroundColourG(void);
    virtual UINT8 GetBackgroundColourB(void);

    virtual SPxErrorCode SetClearBackgroundEnabled(int isEnabled);
    virtual int IsClearBackgroundEnabled(void) 
    { 
        return m_isClearBackgroundEnabled; 
    }

    /* The following function overrides a function in the base class. */
    virtual void SetActive(int active);

    /* Main render function. */
    SPxErrorCode Render(void);

    /* Optional functionality. */
    virtual int IsPerspectiveSupported(void) const { return FALSE; }

    /* Get current device scaling. */
    virtual float GetDeviceScale(void) const { return 1.0; }
    virtual float GetRealDeviceScale(void) const { return GetDeviceScale(); }
    virtual unsigned int GetPhysicalDpi(void) const { return 96; }

    virtual int IncludeLabelOffsetsInPositions(void) const { return FALSE; }

    /* Colour allocation. */
    static SPxErrorCode AddNamedColour(const char *name, UINT32 argb);
    static SPxErrorCode AddNamedColour(const char *name, 
                                       UINT8 r, UINT8 g, 
                                       UINT8 b, UINT8 a=255);
    static SPxErrorCode GetNamedColour(const char *name, UINT32 *argbRtn);

    /* Functions for handling mouse interaction. */
    virtual int HandleMousePress(UINT32 flags, int x, int y, SPxCursor_t *cursor);
    virtual int HandleMouseRelease(UINT32 flags, int x, int y, SPxCursor_t *cursor);
    virtual int HandleDoubleClick(UINT32 flags, int x, int y, SPxCursor_t *cursor);
    virtual int HandleMouseMove(UINT32 flags, int x, int y, SPxCursor_t *cursor);
    virtual int HandleMouseLeave(void);
    SPxErrorCode GetMousePos(int *x, int *y) const;

protected:
    /*
     * Protected functions.
     */

    /* Parmeter setting/getting */
    virtual int SetParameter(char *parameterName, char *parameterValue);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);
    
    /* View control. */
    virtual SPxErrorCode ViewControlHandler(SPxViewControl *viewCtrl,
                                   SPxViewControl::CallbackInfo_t *context, 
                                   UINT32 changeFlags);

    /* Display units. */
    double MetresToDisplayUnits(double metres, int isHeight=FALSE);
    double DisplayUnitsToMetres(double du, int isHeight=FALSE);
    const char *GetDisplayUnitDistText(int isHeight=FALSE);
    SPxErrorCode FormatBearingText(char *buf, int bufLen,
        double bearing, double resolution);
    double MpsToDisplayUnits(double mps);
    double DisplayUnitsToMps(double du);
    const char *GetDisplayUnitSpeedText(void);
    
    /* View to window conversion, protected for backwards compatibility. */
    SPxErrorCode getWinXYFromLatLong(double latDegs, double longDegs,
                                     int *xPixelsRtn, int *yPixelsRtn,
                                     int *isInWindowRtn=0)
    {
        return( GetWinXYFromLatLong(latDegs, longDegs, xPixelsRtn, yPixelsRtn,
                                    isInWindowRtn) );
    }
    SPxErrorCode getWinXYFromMetres(double xMetres, double yMetres,
                                    int *xPixelsRtn, int *yPixelsRtn,
                                    int *isInWindowRtn=0)
    {
        return( GetWinXYFromMetres(xMetres, yMetres, xPixelsRtn, yPixelsRtn,
                                   isInWindowRtn) );
    }

    /* The following rendering functions need to be provided by the derived
     * class according to the graphics library in use.
     */
    virtual SPxErrorCode startRendering(void) = 0;
    virtual SPxErrorCode renderAll(void) = 0;
    virtual SPxErrorCode renderPoly(UINT32 colour,
                                    unsigned int numPoints,
                                    SPxRendererXY_t *points,
                                    unsigned int filled) = 0;
    virtual SPxErrorCode renderPolyGradient(UINT32 colour[2],
                                            unsigned int numPoints,
                                            SPxRendererXY_t *points) = 0;
    virtual SPxErrorCode renderLine(UINT32 colour,
                                    SPxRendererXY_t *start,
                                    SPxRendererXY_t *end) = 0;
    virtual SPxErrorCode renderArc(UINT32 colour,
                                   SPxRendererXY_t *centre,
                                   unsigned int radiusX,
                                   unsigned int radiusY,
                                   double startDegs,
                                   double sweepDegs,
                                   unsigned int filled=FALSE) = 0;
    virtual SPxErrorCode renderEllipse(UINT32 colour,
                                       SPxRendererXY_t *topLeft,
                                       SPxRendererXY_t *bottomRight) = 0;
    virtual SPxErrorCode renderPoint(UINT32 colour,
                                     SPxRendererXY_t *point) = 0;
    virtual SPxErrorCode renderText(UINT32 colour,
                                    SPxRendererXY_t *point,
                                    const char *text,
                                    int adjustFontSize = TRUE) = 0;
    virtual SPxErrorCode renderText(UINT32 colour,
                                    SPxRendererXY_t *point,
                                    const char *text,
                                    double /*rotationDegs*/,
                                    SPxRendererXY_t * /*centre*/,
                                    int adjustFontSize = TRUE)
    {
        return renderText(colour, point, text, adjustFontSize);
    }
    virtual SPxErrorCode getTextExtents(const char *text, 
                                        double *widthRtn,
                                        double *heightRtn) = 0;
    virtual SPxErrorCode getTextExtents(const char *text,
                                        double *widthRtn,
                                        double *heightRtn,
                                        int /* includeOverhangs */)
    {
        return getTextExtents(text, widthRtn, heightRtn);
    }
    virtual SPxErrorCode renderImage(UINT32 colour,
                                     SPxImage *image,
                                     SPxRendererXY_t *topLeft,
                                     SPxRendererXY_t *bottomRight,
                                     double rotDegs=0.0) = 0;
    virtual SPxErrorCode renderImage(UINT32 /*colour*/,
                                     SPxImage * /*image*/,
                                     const SPxRendererXY_t * /*topLeft*/,
                                     const SPxRendererXY_t * /*bottomRight*/,
                                     const SPxRendererXYD_t /*bitmapPoints*/ [4])
    {
        return SPX_ERR_NOT_SUPPORTED;
    }
    virtual SPxErrorCode renderSector(UINT32 colour, 
                                      SPxRendererXY_t *centre,
                                      unsigned int innerRadius,
                                      unsigned int outerRadius,
                                      double startDegs, 
                                      double sweepDegs,
                                      unsigned int filled) = 0;
    virtual SPxErrorCode renderSectorGradient(UINT32 colour[2], 
                                              SPxRendererXY_t *centre,
                                              unsigned int innerRadius,
                                              unsigned int outerRadius,
                                              double startDegs, 
                                              double sweepDegs) = 0;
    virtual SPxErrorCode renderSectorPartialGradient(UINT32 colour[2], 
						     SPxRendererXY_t *centre,
						     unsigned int innerRadius,
						     unsigned int outerRadius,
						     unsigned int /*fillRadius*/,
						     double startDegs, 
						     double sweepDegs)
    {
	return renderSectorGradient(colour,
				    centre,
				    innerRadius,
				    outerRadius,
				    startDegs,
				    sweepDegs);
    }
    virtual SPxErrorCode setClipRectangle(SPxRendererXY_t *topLeft,
                                          SPxRendererXY_t *bottomRight) = 0;

    /* Floating point versions of rendering functions that by default just
     * call the integer versions, but may be overridden by platform renderers.
     */
    virtual SPxErrorCode renderLine(UINT32 colour,
                                    SPxRendererXYD_t *start,
                                    SPxRendererXYD_t *end);
    virtual SPxErrorCode renderLine(UINT32 colour,
                                    SPxRendererXYD_t *start,
                                    SPxRendererXYD_t *end,
                                    double /*lengthPx*/,
                                    double /*angleDegs*/)
    {
        return renderLine(colour, start, end);
    }
    virtual SPxErrorCode renderArc(UINT32 colour,
                                   SPxRendererXYD_t *centre,
                                   double radiusX,
                                   double radiusY,
                                   double startDegs,
                                   double sweepDegs,
                                   unsigned int filled=FALSE);
    virtual SPxErrorCode renderEllipse(UINT32 colour,
                                       SPxRendererXYD_t *topLeft,
                                       SPxRendererXYD_t *bottomRight);
    virtual SPxErrorCode renderPoly(UINT32 colour,
                                    unsigned int numPoints,
                                    SPxRendererXYD_t *points,
                                    unsigned int filled);
    virtual SPxErrorCode renderPoly(UINT32 colour,
                                    unsigned int numPoints,
                                    SPxRendererXYD_t *points,
                                    unsigned int filled,
                                    double /*angleDegs*/)
    {
        return renderPoly(colour, numPoints, points, filled);
    }
    virtual SPxErrorCode renderPolyGradient(UINT32 colour[2],
                                            unsigned int numPoints,
                                            SPxRendererXYD_t *points);
    virtual SPxErrorCode renderPoint(UINT32 colour,
                                     SPxRendererXYD_t *point);
    virtual SPxErrorCode renderText(UINT32 colour,
                                    SPxRendererXYD_t *point,
                                    const char *text,
                                    int adjustFontSize = TRUE);
    virtual SPxErrorCode renderText(UINT32 colour,
                                    SPxRendererXYD_t *point,
                                    const char *text,
                                    double /*rotationDegs*/,
                                    SPxRendererXYD_t * /*centre*/,
                                    int adjustFontSize = TRUE)
    {
        return renderText(colour, point, text, adjustFontSize);
    }
    virtual SPxErrorCode renderText(UINT32 colour,
                                    SPxRendererXYD_t *point,
                                    const char *text,
                                    int adjustFontSize,
                                    int /*includeOverhangs*/)
    {
        return renderText(colour, point, text, adjustFontSize);
    }
    virtual SPxErrorCode renderImage(UINT32 colour,
                                     SPxImage *image,
                                     SPxRendererXYD_t *topLeft,
                                     SPxRendererXYD_t *bottomRight,
                                     double rotDegs=0.0);
    virtual SPxErrorCode renderSector(UINT32 colour, 
                                      SPxRendererXYD_t *centre,
                                      double innerRadius,
                                      double outerRadius,
                                      double startDegs, 
                                      double sweepDegs,
                                      unsigned int filled);
    virtual SPxErrorCode renderSectorGradient(UINT32 colour[2],
                                              SPxRendererXYD_t *centre,
                                              double innerRadius,
                                              double outerRadius,
                                              double startDegs,
                                              double sweepDegs);
    virtual SPxErrorCode renderSectorPartialGradient(UINT32 colour[2], 
						     SPxRendererXYD_t *centre,
						     double innerRadius,
						     double outerRadius,
						     double fillRadius,
						     double startDegs, 
						     double sweepDegs);

    virtual SPxErrorCode endRendering(void) = 0;

    virtual SPxErrorCode requestRedraw(void);

    /* The following rendering functions are just wrappers around
     * the rendering primitives and are implemented in this class.
     */
    virtual SPxErrorCode renderRectangle(UINT32 colour,
                                         SPxRendererXY_t *topLeft,
                                         SPxRendererXY_t *bottomRight,
                                         unsigned int filled);
    virtual SPxErrorCode renderRectangle(UINT32 colour,
                                         SPxRendererXYD_t *topLeft,
                                         SPxRendererXYD_t *bottomRight,
                                         unsigned int filled);

    virtual SPxErrorCode renderDragDropLasso(UINT32 colour,
                                             SPxRendererXY_t *start,
                                             SPxRendererXY_t *end,
                                             int isEndOverObj);
    virtual SPxErrorCode renderDragDropLasso(UINT32 colour,
                                             SPxRendererXYD_t *start,
                                             SPxRendererXYD_t *end,
                                             int isEndOverObj);

    /* Functions for managing and using an off-screen bitmap. */
    virtual SPxErrorCode createOffscreenBitmap(unsigned int index);
    virtual SPxErrorCode setOffscreenBitmapTarget(unsigned int index);
    virtual SPxErrorCode renderOffscreenBitmap(unsigned int index);
    virtual SPxErrorCode updateOffscreenBitmaps(void);
    virtual SPxErrorCode clearOffscreenBitmaps(void);
    virtual SPxErrorCode setOffscreenBitmapAlpha(unsigned int index, double alpha);

    /* Apply the current rotation to a position. */
    void applyRotation(double *xPos, double *yPos, int subtract) const;
    void applyRotation(int *xPos, int *yPos, int subtract) const;

    SPxSoftButton *createSoftButton(int bitmap, int width=-1, int height=-1,
                                   int moBitmap=-1, int moW=-1, int moH=-1);
    SPxSoftButton *createSoftButton(const char *bitmap,
                                    int width=-1, int height=-1,
                                    const char *moBitmap=NULL,
                                    int moWidth=-1, int moHeight=-1);
    SPxErrorCode destroySoftButton(SPxSoftButton *btn);
    SPxErrorCode iterateButtons(SoftButtonFn_t fn, void *userArg);
    SPxErrorCode renderSoftButtons(void);

    /* Utility functions for DPI scaling. */
    int getDpiScaledSize(int size) const;
    unsigned int getDpiScaledSize(unsigned int size) const;
    float getDpiScaledSize(float size) const;
    double getDpiScaledSize(double size) const;

    int drawCaption(UINT32 titleColour, UINT32 bodyColour, UINT32 lineColour,
		    const SPxRendererXY_t *pos, const char *title, const char *txt,
		    unsigned int maxCaptionLen = SPX_RENDERER_MAX_CAPTION_LENGTH,
		    SPxRendererAlignment_t alignment = SPX_RENDERER_ALIGN_TL);
    int drawCaption(UINT32 titleColour, UINT32 bodyColour, UINT32 lineColour,
		    const SPxRendererXYD_t *pos, const char *title, const char *txt,
		    unsigned int maxCaptionLen = SPX_RENDERER_MAX_CAPTION_LENGTH,
		    SPxRendererAlignment_t alignment = SPX_RENDERER_ALIGN_TL);

    /*
     * Protected static functions.
     */

    /*
     * Protected variables.
     */ 

    /* View to be rendered. Lat/long is positive to the north/east. */
    double m_viewCentreLatDegs;		/* Latitude in centre of window */
    double m_viewCentreLongDegs;	/* Longitude in centre of window */
    double m_viewCentreXMetres;         /* X centre of window in metres */
    double m_viewCentreYMetres;         /* Y centre of window in metres */
    unsigned int m_viewWidthPixels;	/* Width of window */
    unsigned int m_viewHeightPixels;	/* Height of window */
    double m_viewPixelsPerMetre;	/* Scaling factor */
    double m_radarXMetres;              /* Radar X position in metres */ 
    double m_radarYMetres;              /* Radar Y position in metres */
    double m_radarLatDegs;		/* Radar latitude. */
    double m_radarLongDegs;		/* Radar longitude. */
    int m_radarLatLongSet;              /* Is radar lat/long defined? */
    double m_lineWidth;                 /* Line width */
    double m_pointWidth;		/* Point width (<=0 means lineWidth) */
    unsigned int m_dashArraySize;	/* Maximum number of dashes in dashes array. */
    unsigned int m_numDashes;		/* Number of dashes in dashes array. */
    SPxAutoPtr<double[]> m_dashes;	/* Dashes */
    double m_dashOffsetPixels;		/* Dash offset in pixels. */
    const char *m_fontName;             /* Font name */
    unsigned int m_fontSize;            /* Font size (pre-adjustment). */
    int m_fontSizeAdjustment;		/* Amount to increase to decrease font size by. */
    unsigned int m_adjustedFontSize;	/* Fond size (post-adjustment). */
    SPxRendererFontStyle_t m_fontStyle; /* Font style */
    SPxTextRenderingMode_t m_textRenderingMode; /* Get text rendering mode */
    int m_antiAliasEnabled;		/* Anti-aliasing enabled? */
    double m_rotationDegs;		/* Display rotation in degrees */
    double m_rotationOffsetDegs;	/* Further user-supplied rotation correction. */
    double m_cosRotation;		/* cos of m_rotationDegs+m_rotationOffsetDegs */
    double m_sinRotation;		/* sin of m_rotationDegs+m_rotationOffsetDegs */
    int m_isLatLongInUse;		/* View centre set as lat/long? */
    SPxUnitsDist_t m_displayUnitsDist;	/* Distance display units. */
    SPxUnitsHeight_t m_displayUnitsHeight; /* Height display units. */
    SPxFormatLatLong_t m_displayUnitsBearing; /* Bearing display units. */
    SPxUnitsSpeed_t m_displayUnitsSpeed; /* Speed display units. */
    SPxViewControl *m_viewCtrl;		/* Installed view control object. */
    unsigned int m_viewCtrlRadarIndex;	/* Index of radar in view control. */
    unsigned int m_viewCtrlPlatformIndex;/* Index of platform in view control. */
    UINT32 m_argbBackground;		/* Background colour. */
    int m_isClearBackgroundEnabled;	/* Clear background on render? */
    double m_lastDeviceScale;           /* Last using DPI scaling. */

    /* Which object are we referencing certain graphics from. */
    SPxRendererRefObj_t m_refObjectType; /* The type of object */
    unsigned int m_refObjectIdx;	/* Index of object (if appropriate) */

    /* Possible objects for referencing graphics from. */
    const SPxRefPoint *m_refPts[SPX_RENDERER_MAX_REF_POINTS]; /* Ref points */

private:
    /*
     * Private functions.
     */

    virtual SPxSoftButton *newSoftButton(const char *bitmap,
                                         int width, int height,
                                         const char *moBitmap,
                                         int moWidth, int moHeight) = 0;
    SPxErrorCode recalculateViewCentre(void);

    void recalculateRotation(void);

    void updateAdjustedFontSize(void);

    /*
     * Private static functions.
     */

    static int viewControlWrapper(void *invokingObj,
                                  void *contextPtr,
                                  void *changeFlagsPtr);

    /*
     * Private variables.
     */ 
    struct impl;
    SPxAutoPtr<impl> m_p;

}; /* SPxRenderer */

/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

/* Inline function for creating a popup menu that prevents use of a renderer
 * from having a dependency on GTK under Linux.
 */
#ifdef _WIN32
inline SPxPopupMenu *SPxPopupMenuCreate(HWND hwnd=NULL, SPxWinMenu *menu=NULL)
{
    return new SPxPopupMenuWin(hwnd, menu);
}
#else
inline SPxPopupMenu *SPxPopupMenuCreate(void)
{
    return new SPxPopupMenuGtk();
}
#endif

#endif /* _SPX_RENDERER_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/

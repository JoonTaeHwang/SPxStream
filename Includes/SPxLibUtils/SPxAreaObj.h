/*********************************************************************
 *
 * (c) Copyright 2013 - 2018, 2020 - 2022, Cambridge Pixel Ltd.
 *
 * Purpose:
 *	Area object interface definition.
 *
 * Revision Control:
 *  13/01/22 v1.25   SP 	Add GetDistanceToPoint().
 *
 * Previous Changes:
 *  15/11/21 1.24   BTB	Add interface for mode information.
 *  18/08/21 1.23   AGC	Add parameter support.
 *  03/08/21 1.22   AGC	Add SPX_AREA_EVENT_CHANGE_POS.
 *  29/07/21 1.21   AGC	Support min/max heights.
 *  10/02/21 1.20   AGC	Add SetItemActive() and IsItemNameSet().
 *			Add various new alarm functions.
 *			Support second area type for SPxAreaObjFind().
 *  22/12/20 1.19   AGC	Add SPxAreaObjFind() to find object at pixel position.
 *  08/10/20 1.18   SP 	Add area types.
 *  09/11/18 1.17   BTB	Add SetReadOnly().
 *  14/06/18 1.16   BTB	Add FlipCrossingDirection() for use by SPxGwPoly.
 *  11/05/18 1.15   BTB	Improvements suggested by AGC.
 *  10/05/18 1.14   BTB	Add SetAlarmCount() for use by SPxGwWidgetGroup.
 *			Make SPxGwWidgetGroup friend so it can call it safely.
 *  08/05/18 1.13   SP 	Add UseInAlarm() and RemoveFromAlarm().
 *  13/02/18 1.12   AGC	Add deselection event.
 *  03/11/17 1.11   AGC	Replace GetClassName() with GetAreaType().
 *			Extend IsPointWithin() with child support.
 *			Add SPxAreaObjFind().
 *  31/10/17 1.10   AGC	Add event callbacks.
 *			Add Add/RemoveArea() functions.
 *			Add point functions with renderer arg.
 *  23/10/17 1.9    AGC	Add GetParentArea() functions.
 *  18/07/14 1.8    AGC	Add AppendPoint() function.
 *  29/01/14 1.7    AGC	Add IsActive() function to interface.
 *  22/01/14 1.6    AGC	Support optional symbol scaling.
 *  04/12/13 1.5    AGC	Add inheritable options.
 *			Add delete areas function.
 *  26/11/13 1.4    AGC	Use SPxSymbol for symbol rendering.
 *  18/11/13 1.3    AGC	Fix clang warning.
 *  24/10/13 1.2    SP 	Add GetFullItemName().
 *  07/10/13 1.1    AGC	Initial version.
 *
 *********************************************************************/

#ifndef _SPX_AREA_OBJ_H
#define _SPX_AREA_OBJ_H

#include "SPxLibUtils/SPxTypes.h"
#include "SPxLibUtils/SPxError.h"
#include "SPxLibUtils/SPxSymbol.h"

/* Forward declarations. */
class SPxAreaObj;
class SPxRenderer;

#define SPX_AREA_NO_POINT (static_cast<unsigned int>(-1))

/* Function types. */
typedef SPxErrorCode (*SPxAreaObjFn)(SPxAreaObj *obj, void *userArg);
typedef void (*SPxAreaEventFn)(SPxAreaObj *obj, UINT32 flags, void *userArg);

enum SPxAreaType
{
    /* For backwards compatibility. */
    SPX_AREA_TYPE_PICTURE = 0,
    SPX_AREA_TYPE_GROUP_LAT_LONG = 1,
    SPX_AREA_TYPE_GROUP_METRES = 2,
    SPX_AREA_TYPE_GROUP_PIXELS = 3,
    SPX_AREA_TYPE_ARC = 4,
    SPX_AREA_TYPE_BITMAP = 5,
    SPX_AREA_TYPE_IMAGE = 6,
    SPX_AREA_TYPE_LINE = 7,
    SPX_AREA_TYPE_POLY = 8,
    SPX_AREA_TYPE_RECT = 9,
    SPX_AREA_TYPE_SECTOR = 10,
    SPX_AREA_TYPE_SYMBOL = 11,
    SPX_AREA_TYPE_TEXT = 12,
    SPX_AREA_TYPE_CIRCLE = 13,
    SPX_AREA_TYPE_MAX, /* Must be last entry. */

    /* For backwards compatibility. */
    SPX_GW_TYPE_PICTURE = SPX_AREA_TYPE_PICTURE,
    SPX_GW_TYPE_GROUP_LAT_LONG = SPX_AREA_TYPE_GROUP_LAT_LONG,
    SPX_GW_TYPE_GROUP_METRES = SPX_AREA_TYPE_GROUP_METRES,
    SPX_GW_TYPE_GROUP_PIXELS = SPX_AREA_TYPE_GROUP_PIXELS,
    SPX_GW_TYPE_ARC = SPX_AREA_TYPE_ARC,
    SPX_GW_TYPE_BITMAP = SPX_AREA_TYPE_BITMAP,
    SPX_GW_TYPE_IMAGE = SPX_AREA_TYPE_IMAGE,
    SPX_GW_TYPE_LINE = SPX_AREA_TYPE_LINE,
    SPX_GW_TYPE_POLY = SPX_AREA_TYPE_POLY,
    SPX_GW_TYPE_RECT = SPX_AREA_TYPE_RECT,
    SPX_GW_TYPE_SECTOR = SPX_AREA_TYPE_SECTOR,
    SPX_GW_TYPE_SYMBOL = SPX_AREA_TYPE_SYMBOL,
    SPX_GW_TYPE_TEXT = SPX_AREA_TYPE_TEXT,
    SPX_GW_TYPE_CIRCLE = SPX_AREA_TYPE_CIRCLE
};

/* For backwards compatibility. */
typedef SPxAreaType SPxGwAreaType;

enum SPxAreaUnits
{
    SPX_AREA_UNITS_NONE = 0,
    SPX_AREA_UNITS_PIXELS = 1,
    SPX_AREA_UNITS_METRES = 2,
    SPX_AREA_UNITS_LAT_LONG = 3
};

enum SPxAreaAlignment
{
    SPX_AREA_ALIGN_TOP_LEFT = 0,
    SPX_AREA_ALIGN_TOP_CENTRE,
    SPX_AREA_ALIGN_TOP_RIGHT,
    SPX_AREA_ALIGN_CENTRE_LEFT,
    SPX_AREA_ALIGN_CENTRE_CENTRE,
    SPX_AREA_ALIGN_CENTRE_RIGHT,
    SPX_AREA_ALIGN_BOTTOM_LEFT,
    SPX_AREA_ALIGN_BOTTOM_CENTRE,
    SPX_AREA_ALIGN_BOTTOM_RIGHT,
    SPX_AREA_ALIGN_MAX, /* Must be last entry. */

    /* For backwards compatibility. */
    SPX_GW_ALIGN_TOP_LEFT = SPX_AREA_ALIGN_TOP_LEFT,
    SPX_GW_ALIGN_TOP_CENTRE = SPX_AREA_ALIGN_TOP_CENTRE,
    SPX_GW_ALIGN_TOP_RIGHT = SPX_AREA_ALIGN_TOP_RIGHT,
    SPX_GW_ALIGN_CENTRE_LEFT = SPX_AREA_ALIGN_CENTRE_LEFT,
    SPX_GW_ALIGN_CENTRE_CENTRE = SPX_AREA_ALIGN_CENTRE_CENTRE,
    SPX_GW_ALIGN_CENTRE_RIGHT = SPX_AREA_ALIGN_CENTRE_RIGHT,
    SPX_GW_ALIGN_BOTTOM_LEFT = SPX_AREA_ALIGN_BOTTOM_LEFT,
    SPX_GW_ALIGN_BOTTOM_CENTRE = SPX_AREA_ALIGN_BOTTOM_CENTRE,
    SPX_GW_ALIGN_BOTTOM_RIGHT = SPX_AREA_ALIGN_BOTTOM_RIGHT,
    SPX_GW_ALIGN_MAX = SPX_AREA_ALIGN_MAX
};

enum SPxAreaInherit
{
    SPX_AREA_INHERIT_NONE 	    = 0x0000,
    SPX_AREA_INHERIT_LINE_COLOUR    = 0x0001,
    SPX_AREA_INHERIT_FILL_COLOUR    = 0x0002,
    SPX_AREA_INHERIT_LINE_WIDTH	    = 0x0004,
    SPX_AREA_INHERIT_DASH	    = 0x0008,
    SPX_AREA_INHERIT_ALL	    = 0xFFFF
};

enum SPxAreaEvent
{
    SPX_AREA_EVENT_NONE		    = 0x0000,
    SPX_AREA_EVENT_CREATE	    = 0x0001,
    SPX_AREA_EVENT_DELETE	    = 0x0002,
    SPX_AREA_EVENT_NAME		    = 0x0004,
    SPX_AREA_EVENT_SELECT	    = 0x0008,
    SPX_AREA_EVENT_LOAD		    = 0x0010,
    SPX_AREA_EVENT_DELETE_ALL	    = 0x0020,
    SPX_AREA_EVENT_CHANGE	    = 0x0040,
    SPX_AREA_EVENT_DESELECT	    = 0x0080,
    SPX_AREA_EVENT_CHANGE_POS	    = 0x0100
};

enum SPxAreaHeightMode
{
    SPX_AREA_HEIGHT_MODE_NONE = 0x0000,
    SPX_AREA_HEIGHT_MODE_MIN = 0x0001,
    SPX_AREA_HEIGHT_MODE_MAX = 0x00002,
    SPX_AREA_HEIGHT_MODE_MIN_MAX = SPX_AREA_HEIGHT_MODE_MIN | SPX_AREA_HEIGHT_MODE_MAX
};

class SPxAreaObj
{
public:
    /* Constructor and destructor. */
    SPxAreaObj(void) {}
    virtual ~SPxAreaObj(void) {}
    virtual void SetItemActive(int /*active*/) {}
    virtual int IsActive(void) const { return TRUE; }
    virtual int GetAreaType(void) const { return 0; }

    /* Naming. */
    virtual int IsNameSettable(void) const { return FALSE; }
    virtual int IsItemNameSet(void) const { return FALSE; }
    virtual SPxErrorCode SetItemName(const char * /*name*/) { return SPX_ERR_NOT_SUPPORTED; }
    virtual const char *GetItemName(void) const=0;
    virtual const char *GetFullItemName(void) const { return GetItemName(); }

    /* Selection. */
    virtual SPxErrorCode SetSelected(int /*selected*/) { return SPX_ERR_NOT_SUPPORTED; }
    virtual int IsSelected(void) const { return FALSE; }

    /* Read-only. */
    virtual SPxErrorCode SetReadOnly(int /*readOnly*/) { return SPX_ERR_NOT_SUPPORTED; }
    virtual int IsReadOnly(void) const { return FALSE; }

    /* Get parent. */
    virtual const SPxAreaObj *GetParentArea(void) const { return NULL; }
    virtual SPxAreaObj *GetParentArea(void) { return NULL; }

    /* Create/delete children. */
    virtual unsigned int GetNumTypes(void) const { return 0; }
    virtual const char *GetTypeName(unsigned int /*typeIndex*/) const { return NULL; }
    virtual SPxAreaObj *CreateArea(unsigned int /*typeIndex*/) { return NULL; }
    virtual SPxErrorCode DeleteAreas(void) { return SPX_NO_ERROR; }
    virtual SPxErrorCode AddArea(SPxAreaObj * /*childArea*/, SPxAreaObj * /*aboveArea*/ = NULL) { return SPX_ERR_NOT_SUPPORTED; }
    virtual SPxErrorCode RemoveArea(SPxAreaObj * /*childArea*/) { return SPX_ERR_NOT_SUPPORTED; }
    virtual SPxErrorCode AddEventCallback(SPxAreaEventFn /*fn*/, void * /*userArg*/) { return SPX_ERR_NOT_SUPPORTED; }
    virtual SPxErrorCode RemoveEventCallback(SPxAreaEventFn /*fn*/, void * /*userArg*/) { return SPX_ERR_NOT_SUPPORTED; }

    /* Iterate over children. */
    virtual SPxErrorCode IterateObjects(SPxAreaObjFn /*fn*/, void * /*userArg*/) const { return SPX_NO_ERROR; }

    /* Inherit properties. */
    virtual int IsInheritable(void) const { return FALSE; }
    virtual SPxErrorCode SetInheritMask(UINT32 /*mask*/) { return SPX_NO_ERROR; }
    virtual UINT32 GetInheritMask(void) const { return SPX_AREA_INHERIT_NONE; }

    /* Get units. */
    virtual SPxAreaUnits GetUnits(void) const { return SPX_AREA_UNITS_NONE; }

    /* Move object. */
    virtual int IsMovable(void) const { return FALSE; }
    virtual SPxErrorCode SetPos(double /*x*/, double /*y*/) { return SPX_ERR_NOT_SUPPORTED; }
    virtual SPxErrorCode GetPos(double * /*x*/, double * /*y*/) const { return SPX_ERR_NOT_SUPPORTED; }
    virtual int IsPointWithin(double /*x*/, double /*y*/, double /*margin*/) const { return FALSE; }
    virtual int IsPointWithin(double x, double y, double margin, int /*includeChildren*/) const { return IsPointWithin(x, y, margin); }
    virtual int IsPointWithin(double x, double y, double margin, int includeChildren, SPxRenderer * /*renderer*/) const { return IsPointWithin(x, y, margin, includeChildren); }
    virtual int IsPointWithin(double x, double y, double margin, int includeChildren, SPxRenderer *renderer, double /*heightMetres*/) const { return IsPointWithin(x, y, margin, includeChildren, renderer); }
    virtual int IsCrossing(double /*x1*/, double /*y1*/, double /*x2*/, double /*y2*/) const { return FALSE; }
    virtual SPxErrorCode Update(void) { return SPX_ERR_NOT_SUPPORTED; }

    /* Offset object. */
    virtual int IsOffsetable(void) const { return FALSE; }
    virtual SPxErrorCode SetPixelOffset(double /*x*/, double /*y*/) { return SPX_ERR_NOT_SUPPORTED; }
    virtual SPxErrorCode GetPixelOffset(double * /*x*/, double * /*y*/) const { return SPX_ERR_NOT_SUPPORTED; }

    /* Move points. */
    virtual unsigned int GetNumPoints(void) const { return 0; }
    virtual SPxErrorCode SetPointPos(unsigned int /*idx*/, double /*x*/, double /*y*/, int /*fixedRatio*/=FALSE) { return SPX_ERR_NOT_SUPPORTED; }
    virtual SPxErrorCode GetPointPos(unsigned int /*idx*/, double * /*x*/, double * /*y*/) const { return SPX_ERR_NOT_SUPPORTED; }
    virtual SPxErrorCode SetPointPos(unsigned int idx, double x, double y, int fixedRatio, SPxRenderer * /*renderer*/) { return SetPointPos(idx, x, y, fixedRatio); }
    virtual SPxErrorCode GetPointPos(unsigned int idx, double *x, double *y, SPxRenderer * /*renderer*/) const { return GetPointPos(idx, x, y); }
    virtual SPxErrorCode SetSelectedPoint(unsigned int /*idx*/) { return SPX_ERR_NOT_SUPPORTED; }
    virtual unsigned int GetSelectedPoint(void) const { return FALSE; }

    /* Add/remove points. */
    virtual int IsEditable(void) const { return FALSE; }
    virtual SPxErrorCode AddPoint(double /*x*/, double /*y*/) { return SPX_ERR_NOT_SUPPORTED; }
    virtual SPxErrorCode RemovePoint(unsigned int /*idx*/) { return SPX_ERR_NOT_SUPPORTED; }
    virtual SPxErrorCode AppendPoint(double /*x*/, double /*y*/) { return SPX_ERR_NOT_SUPPORTED; }

    /* Heights. */
    virtual int IsHeightSupported(void) const { return FALSE; }
    virtual SPxErrorCode SetHeightModeMask(UINT32 /*modeMask*/) { return SPX_ERR_NOT_SUPPORTED; }
    virtual UINT32 GetHeightModeMask(void) const { return 0; }
    virtual SPxErrorCode SetMinHeightMetres(double /*minHeightMetres*/) { return SPX_ERR_NOT_SUPPORTED; }
    virtual double GetMinHeightMetres(void) const { return 0.0; }
    virtual SPxErrorCode SetMaxHeightMetres(double /*maxHeightMetres*/) { return SPX_ERR_NOT_SUPPORTED; }
    virtual double GetMaxHeightMetres(void) const { return 0.0; }

    /* Poly auto close. */
    virtual int IsAutoCloseSupported(void) const { return FALSE; }
    virtual SPxErrorCode SetAutoCloseEnabled(int /*autoClose*/) { return SPX_ERR_NOT_SUPPORTED; }
    virtual int IsAutoCloseEnabled(void) const { return FALSE; }

    /* Set colours. */
    virtual int IsColourSupported(void) const { return FALSE; }
    virtual SPxErrorCode SetLineColour(UINT32 /*argb*/) { return SPX_ERR_NOT_SUPPORTED; }
    virtual UINT32 GetLineColour(void) const { return 0; }
    virtual SPxErrorCode SetFillColour(UINT32 /*argb*/) { return SPX_ERR_NOT_SUPPORTED; }
    virtual UINT32 GetFillColour(void) const { return 0; }

    /* Line width. */
    virtual int IsLineWidthSupported(void) const { return FALSE; }
    virtual SPxErrorCode SetLineWidthPixels(double /*lineWidthPixels*/) { return SPX_ERR_NOT_SUPPORTED; }
    virtual double GetLineWidthPixels(void) const { return SPX_ERR_NOT_SUPPORTED; }

    /* Dash style. */
    virtual int IsDashSupported(void) const { return FALSE; }
    virtual SPxErrorCode SetDash(const double /*dashes*/[], unsigned int /*numDashes*/, double /*offsetPixels*/) { return SPX_ERR_NOT_SUPPORTED; }
    virtual SPxErrorCode GetDash(double /*dashes*/[], unsigned int * /*numDashes*/, double * /*offsetPixels*/) const { return SPX_ERR_NOT_SUPPORTED; }

    /* Text. */
    virtual int IsTextSupported(void) const { return FALSE; }
    virtual SPxErrorCode SetText(const char * /*text*/) { return SPX_ERR_NOT_SUPPORTED; }
    virtual const char *GetText(void) const { return NULL; }
    virtual SPxErrorCode SetFontName(const char * /*font*/) { return SPX_ERR_NOT_SUPPORTED; }
    virtual const char *GetFontName(void) const { return NULL; }
    virtual SPxErrorCode SetFontSize(unsigned int /*fontSize*/) { return SPX_ERR_NOT_SUPPORTED; }
    virtual unsigned int GetFontSize(void) const { return SPX_ERR_NOT_SUPPORTED; }

    /* Alignment. */
    virtual int IsAlignmentSupported(void) const { return FALSE; }
    virtual SPxErrorCode SetAlignment(SPxAreaAlignment /*alignment*/) { return SPX_ERR_NOT_SUPPORTED; }
    virtual SPxAreaAlignment GetAlignment(void) const { return SPX_AREA_ALIGN_TOP_LEFT; }

    /* Symbol. */
    virtual int IsSymbolSupported(void) const { return FALSE; }
    virtual SPxErrorCode SetSymbol(SPxSymbolShape_t /*symbol*/) { return SPX_ERR_NOT_SUPPORTED; }
    virtual SPxSymbolShape_t GetSymbol(void) const { return SPX_SYMBOL_SHAPE_CROSS; }
    virtual SPxErrorCode EnableScaling(int /*enableScaling*/) { return SPX_ERR_NOT_SUPPORTED; }
    virtual int IsScalingEnabled(void) const { return FALSE; }

    /* Image. */
    virtual int IsImageSupported(void) const { return FALSE; }
    virtual SPxErrorCode SetImage(const char * /*path*/) { return SPX_ERR_NOT_SUPPORTED; }
    virtual const char *GetFullImagePath(void) const { return NULL; }
    virtual const char *GetShortImagePath(void) const { return NULL; }

    /* Load/save state. */
    virtual int IsSaveSupported(void) const { return FALSE; }
    virtual SPxErrorCode Load(const char * /*name*/) { return SPX_ERR_NOT_SUPPORTED; }
    virtual SPxErrorCode Save(const char * /*name*/) const { return SPX_ERR_NOT_SUPPORTED; }
    virtual const char *GetFullPath(void) const { return NULL; }
    virtual const char *GetShortPath(void) const { return NULL; }

    /* Use with alarms. */
    virtual int GetTotalAlarmCount(void) const { return 0; }
    virtual void UseInAlarm(UINT32 /*alarmType*/) { return; }
    virtual void RemoveFromAlarm(UINT32 /*alarmType*/) { return; }
    virtual void UseInDisarmedAlarm(UINT32 /*alarmType*/) { return; }
    virtual void RemoveFromDisarmedAlarm(UINT32 /*alarmType*/) { return; }
    virtual void HighlightAlarm(int /*enable*/, UINT32 /*endTime*/, void* /*key*/) { return; }
    virtual void FlipCrossingDirection(void) { return; }

    /* Mode. */
    virtual int IsModeSupported(void) {return FALSE;}
    virtual SPxErrorCode SetMode(int /*mode*/) { return SPX_ERR_NOT_SUPPORTED; }
    virtual int GetMode(void) {return 0;}

    /* Distance to point. */
    virtual int IsDistanceToPointSupported(void) const { return FALSE; }
    virtual SPxErrorCode GetDistanceToPoint(double /*x*/, double /*y*/, double* /*distMetres*/) const { return SPX_ERR_NOT_SUPPORTED; }

    /* Generic parameter assignment. */
    SPxErrorCode SetParameter(const char *name, const char *value,
			      char *replyBuf, unsigned int replyBufLen);
    SPxErrorCode GetParameter(const char *name, char *valueBuf, int bufLen);

}; /* SPxAreaObj */

/* Find a child object with the specified name. */
extern SPxAreaObj *SPxAreaObjFind(SPxAreaObj *obj, const char *name);

/* Find a child object at the specified location. */
extern SPxAreaObj *SPxAreaObjFind(SPxAreaObj *obj, int xPx, int yPx,
				  int marginPx,
				  SPxRenderer *renderer,
				  SPxAreaType areaType=SPX_AREA_TYPE_PICTURE,
				  SPxAreaType areaType2=SPX_AREA_TYPE_PICTURE,
				  SPxAreaObj *existingObj=NULL);

#endif /* _SPX_AREA_OBJ_H */

/*********************************************************************
 *
 *      End of file
 *
 *********************************************************************/


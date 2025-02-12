/*********************************************************************
 *
 * (c) Copyright 2013 - 2022, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   SPx graphics widget abstract base class.
 *
 * Revision Control:
 *  14/01/22 v1.29   SP 	Add overloads for UnitsToMetres() and MetresToUnits().
 *
 * Previous Changes:
 *  13/01/22 1.28   SP 	Add UnitsToMetres() and MetresToUnits().
 *  30/07/21 1.27   AGC	Support retrieving properties with parameters.
 *  29/07/21 1.26   AGC	Support min/max heights.
 *  10/02/21 1.25   AGC	Move gate alarm logic here from SPxGwPoly.
 *  02/05/19 1.24   BTB	Fix to positioning.
 *  09/11/18 1.23   BTB	SetReadOnly() is now part of SPxAreaObj interface.
 *  11/05/18 1.22   BTB	Improvements suggested by AGC.
 *			Add GetAlarmCount() for better count management.
 *  30/04/18 1.21   BTB	Allow showing labels without editing.
 *  27/02/18 1.20   AGC	Always use doubles for rendering.
 *  13/02/18 1.19   AGC	Make SetGroupSelected() virtual.
 *  08/11/17 1.18   AGC	Make render functions more generic.
 *  03/11/17 1.17   AGC	Remove GetClassName().
 *			Improve events.
 *  01/11/17 1.16   AGC	Fix compiler warnings.
 *  31/10/17 1.15   AGC	Improve widget group add/remove.
 *  23/10/17 1.14   AGC	Support name rendering.
 *  31/07/15 1.13   AGC	Highlight selected widget points.
 *  04/07/14 1.12   AGC	Add required stdio header.
 *  10/03/14 1.11   AGC	Add DeleteAllWidgets() function.
 *  29/01/14 1.10   AGC	Implement new SPxAreaObj IsActive() function.
 *  27/01/14 1.9    AGC	Add overloads of units/pixels conversion
 *			functions taking doubles.
 *  04/12/13 1.8    AGC	Move configuration functions here from SPxGwShape.
 *			Support inheritable options.
 *  26/11/13 1.7    AGC	Request redraw when active state is changed.
 *  22/11/13 1.6    AGC	Request redraw on modification.
 *  07/10/13 1.5    AGC	Move to SPxLibUtils.
 *			Make cross-platform.
 *			Add SPxAreaObj support.
 *  08/03/13 1.4    SP 	Make base class virtual.
 *  25/01/13 1.3    SP 	Allow group to be set in constructor.
 *                      Split this class into SPxGwShape.
 *                      Remove reference points.
 *  15/01/13 1.2    SP 	Remove namespace std from header.
 *  11/01/13 1.1    SP 	Initial version.
 *
 *********************************************************************/

#ifndef _SPX_GW_WIDGET_H
#define _SPX_GW_WIDGET_H

/* Other headers required. */
#include "stdio.h" /* For FILE. */
#include "SPxLibUtils/SPxAreaObj.h"
#include "SPxLibUtils/SPxGwObj.h"

/*********************************************************************
 *
 *  Constants
 *
 *********************************************************************/

/*********************************************************************
 *
 *   Macros
 *
 *********************************************************************/

/*********************************************************************
 *
 *   Type definitions
 *
 *********************************************************************/

/*********************************************************************
 *
 *   Class definitions
 *
 *********************************************************************/

/* Declare classes we need. */
class SPxGwWidgetGroup;
class SPxExtRenderer;

class SPxGwWidget : virtual public SPxGwObj, public SPxAreaObj
{
    /* Grant SPxGwWidgetGroup private access. */
    friend class SPxGwWidgetGroup;

public:

    /*
     * Public variables.
     */

    /*
     * Public functions.
     */

    /* Constructor and destructor. */
    SPxGwWidget(SPxGwWidgetGroup *group);
    virtual ~SPxGwWidget(void);
    virtual void SetActive(int active=TRUE);
    virtual void SetItemActive(int active) { SetActive(active); }
    virtual int IsActive(void) const;

    /* Configuration. */
    virtual const SPxGwWidgetGroup *GetWidgetGroup(void) const { return m_group; }
    virtual SPxGwWidgetGroup *GetWidgetGroup(void) { return m_group; }
    virtual SPxErrorCode DeleteAllWidgets(void) { return SPX_NO_ERROR; }
    virtual void RequestRedraw(void);

    virtual int IsChildSelected(void) const { return IsSelected(); }
    virtual int GetReadOnly(void) const { return m_readOnly; }
    virtual int IsPulseEnabled(void) const;

    /* Configuration. */
    virtual SPxErrorCode SetLineColour(UINT32 argb);
    virtual SPxErrorCode SetLineColour(UINT8 r, UINT8 g, 
                                       UINT8 b, UINT8 a=255);
    virtual SPxErrorCode SetLineColour(const char *name);
    virtual UINT32 GetLineColour(void) const;
    virtual UINT8 GetLineColourA(void) const;
    virtual UINT8 GetLineColourR(void) const;
    virtual UINT8 GetLineColourG(void) const;
    virtual UINT8 GetLineColourB(void) const;

    virtual SPxErrorCode SetFillColour(UINT32 argb);
    virtual SPxErrorCode SetFillColour(UINT8 r, UINT8 g, 
                                       UINT8 b, UINT8 a=255);
    virtual SPxErrorCode SetFillColour(const char *name);
    virtual UINT32 GetFillColour(void) const;
    virtual UINT8 GetFillColourA(void) const;
    virtual UINT8 GetFillColourR(void) const;
    virtual UINT8 GetFillColourG(void) const;
    virtual UINT8 GetFillColourB(void) const;

    virtual SPxErrorCode SetDash(const double dashes[], 
                                 unsigned int numDashes, 
                                 double offsetPixels);
    virtual SPxErrorCode GetDash(double dashes[],
                                 unsigned int *numDashes,
                                 double *offsetPixels) const;

    virtual SPxErrorCode SetLineWidthPixels(double lineWidth);
    virtual double GetLineWidthPixels(void) const;

    /* SPxAreaObj interface. */
    virtual SPxErrorCode SetReadOnly(int readOnly) { m_readOnly = readOnly; return SPX_NO_ERROR; }
    virtual int IsReadOnly(void) const;
    virtual int IsNameSettable(void) const { return TRUE; }
    virtual int IsItemNameSet(void) const;
    virtual SPxErrorCode SetItemName(const char *name);
    virtual const char *GetItemName(void) const;
    virtual SPxErrorCode SetSelected(int selected);
    virtual int IsSelected(void) const { return m_isSelected; }
    virtual const SPxAreaObj *GetParentArea(void) const;
    virtual SPxAreaObj *GetParentArea(void);
    virtual SPxErrorCode SetSelectedPoint(unsigned int idx);
    virtual unsigned int GetSelectedPoint(void) const { return m_currPoint; }
    virtual SPxAreaUnits GetUnits(void) const { return m_units; }
    virtual SPxErrorCode GetPos(double *x, double *y) const;

    virtual int IsColourSupported(void) const { return TRUE; }
    virtual int IsLineWidthSupported(void) const { return TRUE; }
    virtual int IsDashSupported(void) const { return TRUE; }
    virtual int IsInheritable(void) const { return (m_group != NULL); }

    virtual SPxErrorCode SetHeightModeMask(UINT32 modeMask);
    virtual UINT32 GetHeightModeMask(void) const { return m_heightModeMask; }
    virtual SPxErrorCode SetMinHeightMetres(double minHeightMetres);
    virtual double GetMinHeightMetres(void) const { return m_minHeightMetres; }
    virtual SPxErrorCode SetMaxHeightMetres(double maxHeightMetres);
    virtual double GetMaxHeightMetres(void) const { return m_maxHeightMetres; }

    virtual SPxErrorCode SetInheritMask(UINT32 mask);
    virtual UINT32 GetInheritMask(void) const;

    virtual void HighlightAlarm(int enable, UINT32 endTime, void *key);

    /* Implemented by derived class. */
    virtual SPxErrorCode LoadCfg(FILE *f)=0;
    SPxErrorCode LoadLine(const char *param, const char *value);
    virtual SPxErrorCode SaveCfg(FILE *f) const;
    SPxErrorCode SaveName(FILE *f) const;
    SPxErrorCode SaveCommon(FILE *f) const;

    virtual int CanUseInAlarm(void) const { return FALSE; }
    virtual int GetTotalAlarmCount(void) const;
    virtual void UseInAlarm(UINT32 alarmType);
    virtual void RemoveFromAlarm(UINT32 alarmType);
    virtual void UseInDisarmedAlarm(UINT32 alarmType);
    virtual void RemoveFromDisarmedAlarm(UINT32 alarmType);
    virtual void FlipCrossingDirection(void);
    virtual int IsCrossingDirectionInverted(void) const { return m_crossingDirectionInverted; }

    /* Generic parameter assignment. */
    virtual int SetParameter(char *name, char *value);
    virtual int GetParameter(char *name, char *valueBuf, int bufLen);

protected:

    /*
     * Protected variables.
     */

    /*
     * Protected functions.
     */

    /* Group setting selected state. */
    virtual SPxErrorCode SetGroupSelected(int selected);

    /* Configure units. */
    SPxErrorCode SetUnits(SPxAreaUnits units);

    int IsOffsetAbsolute(void) { return m_offsetIsAbsolute; }
    void SetIsOffsetAbsolute(int isAbsolute) { m_offsetIsAbsolute = isAbsolute; }

    /* Reference position. */
    virtual double GetRefX(void) const;
    virtual double GetRefY(void) const;

    SPxErrorCode UnitsToPixels(SPxRenderer *renderer,
	double x, double y, double *px, double *py) const;
    SPxErrorCode PixelsToUnits(SPxRenderer *renderer,
	double px, double py, double *x, double *y) const;

    SPxErrorCode UnitsToMetres(SPxRenderer *renderer,
	double x, double y, double *xMetres, double *yMetres) const;
    SPxErrorCode MetresToUnits(SPxRenderer *renderer,
	double xMetres, double yMetres, double *x, double *y) const;

    SPxErrorCode UnitsToMetres(SPxRenderer *renderer, double val, double *valMetres) const;
    SPxErrorCode MetresToUnits(SPxRenderer *renderer, double valMetres, double *val) const;

    /* Height check. */
    int IsHeightWithin(double heightMetres) const;

    UINT32 GetFillColourP(void) { return PulseColour(GetFillColour(), TRUE); }
    UINT32 GetLineColourP(void) { return PulseColour(GetLineColour(), FALSE); }
    UINT32 GetSelPointColour(void) const { return 0xFFFFFF00; }
    UINT32 PulseColour(UINT32 colour, int isFill=TRUE);

    virtual int IsShowItemNamesEnabled(void) const;
    virtual int IsShowItemName(void) const { return FALSE; }

    /* Widget group. */
    void AddToGroup(SPxGwWidgetGroup *group);
    void RemoveFromGroup(void);
    virtual void SetWidgetGroupLink(SPxGwWidgetGroup *group);

    /* Rendering. */
    virtual SPxErrorCode Render(SPxExtRenderer *renderer,
				int editEnabled=TRUE,
				int labelsEnabled=FALSE);

    /* Signal events to parent. */
    virtual void signalEvent(UINT32 flags);

    /* Used by other area objects to keep alarm count correct. */
    virtual void SetAlarmCount(UINT32 alarmType, int alarmCount);
    virtual int GetAlarmCount(UINT32 alarmType);
    virtual void ForceShowDirectionIndicators(int show);

private:

    /*
     * Private variables.
     */
    struct impl;
    SPxAutoPtr<impl> m_p;

    /* Group that this widget belongs to. */
    SPxGwWidgetGroup *m_group;

    /* Units. */
    SPxAreaUnits m_units;

    /* Offset is absolute position. */
    int m_offsetIsAbsolute;

    /* Selected? */
    int m_isSelected;

    /* Selected point. */
    unsigned int m_currPoint;

    /* Read-only? */
    int m_readOnly;

    /* Widget colours. */
    UINT32 m_argbLine;
    UINT32 m_argbFill;

    /* Widget line dashing. */
    double m_dashOffsetPixels;
    double m_oldDashOffsetPixels;

    /* Widget line width. */
    double m_lineWidthPixels;
    double m_oldLineWidthPixels;

    /* Heights. */
    UINT32 m_heightModeMask;
    double m_minHeightMetres;
    double m_maxHeightMetres;

    /* Mask of which properties to inherit from parent. */
    UINT32 m_inheritMask;

    /* Show gate crossing direction indicators? */
    int m_forceShowDirection;

    /* Number of alarms using the widget as a shape. */
    int m_alarmCount;
    int m_alarmCountFwd;
    int m_alarmCountRev;

    /* Number of disarmed alarms using the poly as a gate. */
    int m_disarmedAlarmCount;
    int m_disarmedAlarmCountFwd;
    int m_disarmedAlarmCountRev;

    /* Is crossing direction inverted? */
    int m_crossingDirectionInverted;

    /*
     * Private functions.
     */
    SPxErrorCode startRendering(SPxRenderer *renderer);
    virtual SPxErrorCode renderAll(SPxExtRenderer * /*renderer*/) { return SPX_ERR_NOT_SUPPORTED; }
    void renderPoints(SPxExtRenderer *renderer);
    void renderName(SPxExtRenderer *renderer);
    void renderAlarmArrows(SPxExtRenderer *renderer);
    SPxErrorCode endRendering(SPxRenderer *renderer);
    virtual void enableEventCallbacks(int /*enable*/) {}
    int isHighlighted(void) const;

    /*
     * Private static functions.
     */

}; /* SPxGwWidget */

#endif /* _SPX_GW_WIDGET_H */

/*********************************************************************
 *
 *      End of file
 *
 *********************************************************************/


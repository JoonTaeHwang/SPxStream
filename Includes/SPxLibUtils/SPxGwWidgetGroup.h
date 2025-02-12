/*********************************************************************
 *
 * (c) Copyright 2013 - 2022, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   SPx graphics widget group.
 *
 * Revision Control:
 *  24/02/22 v1.22   AGC 	Support SPxGwPictureBase use.
 *
 * Previous Changes:
 *  14/01/22 1.21   SP 	Implement GetDistanceToPoint().
 *  04/08/21 1.20   AGC	Support widget create/delete with parameter.
 *  29/07/21 1.19   AGC	Support min/max heights.
 *  10/02/21 1.18   AGC	Enhance alarm handling.
 *  25/06/18 1.17   AGC	Correctly suppress cppcheck warning.
 *  11/05/18 1.16   BTB	Add GetAlarmCount() for better count management.
 *  10/05/18 1.15   BTB	Manage alarm count in all child widgets.
 *  30/04/18 1.14   BTB	Allow showing labels without editing.
 *  13/02/18 1.13   AGC	Implement SetGroupSelected().
 *  03/11/17 1.12   AGC	Support GetAreaType().
 *			Support new IsPointWithin().
 *			Improve events.
 *  31/10/17 1.11   AGC	Support event callbacks.
 *			Support SPxAreaObj add/remove areas.
 *  25/10/17 1.10   SP 	Add IterateWidgets().
 *  23/10/17 1.9    AGC	Support name rendering.
 *  10/03/14 1.8    AGC	Add DeleteAllWidgets() function.
 *  04/12/13 1.7    AGC	Support inheritable options.
 *			Add DeleteAreas() function.
 *  22/11/13 1.6    AGC	Request redraw on modification.
 *  07/10/13 1.5    AGC	Move to SPxLibUtils.
 *			Make cross-platform.
 *			Add SPxAreaObj support.
 *  04/10/13 1.4    AGC	Simplify headers.
 *  25/01/13 1.3    SP 	Allow picture to be set in constructor.
 *			Derive class from SPxGwWidget.
 *  15/01/13 1.2    SP 	Prefix list with 'std::'.
 *  11/01/13 1.1    SP 	Initial version.
 *
 *********************************************************************/

#ifndef _SPX_GW_WIDGET_GROUP_H
#define _SPX_GW_WIDGET_GROUP_H

/* Other headers required. */
#include <stdio.h>
#include "SPxLibUtils/SPxAreaObj.h"
#include "SPxLibUtils/SPxGwWidget.h"

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

/* Function types. */
typedef SPxErrorCode (*SPxGwWidgetFn)(SPxGwWidget *widget, void *userArg);

/*********************************************************************
 *
 *   Class definitions
 *
 *********************************************************************/

/* Declare classes we need. */
class SPxGwPictureBase;
class SPxExtRenderer;
class SPxRenderer;

class SPxGwWidgetGroup : public SPxGwWidget
{
    /* Grant SPxGwPicture private access. */
    friend class SPxGwPictureBase;
    friend class SPxGwPicture;

public:

    /*
     * Public variables.
     */

    /*
     * Public functions.
     */

    /* Constructors and destructor. */
    SPxGwWidgetGroup(SPxGwPictureBase *picture, SPxAreaUnits units=SPX_AREA_UNITS_PIXELS);
    SPxGwWidgetGroup(SPxGwWidgetGroup *group=NULL);
    virtual ~SPxGwWidgetGroup(void);

    /* Configuration. */
    virtual SPxErrorCode SetOffset(double x, double y);
    virtual double GetOffsetX(void) const;
    virtual double GetOffsetY(void) const;
    virtual double GetPositionX(void) const;
    virtual double GetPositionY(void) const;

    virtual SPxGwPictureBase *GetPicture(void);
    virtual void RequestRedraw(void);
    virtual int IsPulseEnabled(void) const;
    virtual int IsShowItemNamesEnabled(void) const;

    virtual SPxErrorCode AddWidgetAbove(SPxGwWidget *widget,
                                        SPxGwWidget *above=NULL);

    virtual SPxErrorCode AddWidgetBelow(SPxGwWidget *widget,
                                        SPxGwWidget *below=NULL);

    // cppcheck-suppress virtualCallInConstructor
    virtual SPxErrorCode RemoveWidget(SPxGwWidget *widget);

    virtual SPxErrorCode DeleteAllWidgets(void);

    virtual SPxErrorCode AddEventCallback(SPxAreaEventFn fn, void *userArg);
    virtual SPxErrorCode RemoveEventCallback(SPxAreaEventFn fn, void *userArg);

    /* Old style pixels configuration. */
    virtual SPxErrorCode SetOffsetPixels(double x, double y);
    virtual double GetOffsetXPixels(void) const { return GetOffsetX(); }
    virtual double GetOffsetYPixels(void) const { return GetOffsetY(); }
    virtual double GetPositionXPixels(void) const { return GetPositionX(); }
    virtual double GetPositionYPixels(void) const { return GetPositionY(); }

    /* SPxAreaObj interface. */
    virtual int GetAreaType(void) const;
    virtual const SPxAreaObj *GetParentArea(void) const;
    virtual SPxAreaObj *GetParentArea(void);
    virtual unsigned int GetNumTypes(void) const;
    virtual const char *GetTypeName(unsigned int typeIndex) const;
    virtual SPxAreaObj *CreateArea(unsigned int typeIndex);
    virtual SPxErrorCode DeleteAreas(void);
    virtual SPxErrorCode AddArea(SPxAreaObj *childArea, SPxAreaObj *aboveArea=NULL);
    virtual SPxErrorCode RemoveArea(SPxAreaObj *childArea);

    virtual SPxErrorCode IterateObjects(SPxAreaObjFn fn, void *userArg) const;
    virtual SPxErrorCode IterateWidgets(SPxGwWidgetFn fn, void *userArg) const;
    virtual SPxErrorCode SetSelected(int selected);
    virtual int IsChildSelected(void) const;
    virtual int AreChildrenReadOnly(void) const;
    virtual int IsMovable(void) const { return TRUE; }
    virtual SPxErrorCode SetPos(double x, double y);
    virtual SPxErrorCode GetPos(double *x, double *y) const;
    virtual int IsPointWithin(double x, double y, double margin) const
    {
	return IsPointWithin(x, y, margin, FALSE);
    }
    virtual int IsPointWithin(double x, double y, double margin,
			      int includeChildren) const {
	return IsPointWithin(x, y, margin, includeChildren, NULL);
    }
    virtual int IsPointWithin(double x, double y, double margin,
			      int includeChildren,
			      SPxRenderer *renderer) const;
    virtual int IsPointWithin(double x, double y, double margin,
			      int includeChildren,
			      SPxRenderer *renderer,
			      double heightMetres) const;

    virtual int HasChanges(void) const;

    virtual int IsDistanceToPointSupported(void) const { return TRUE; }
    virtual SPxErrorCode GetDistanceToPoint(double x, double y, double *distMetres) const;

    virtual SPxErrorCode LoadCfg(FILE *f);
    SPxErrorCode LoadLine(const char *param, const char *value);
    virtual SPxErrorCode SaveCfg(FILE *f) const;

    virtual SPxErrorCode SignalEvent(UINT32 flags, SPxGwWidget *obj);

    /* Use with alarms. */
    virtual void UseInAlarm(UINT32 alarmType);
    virtual void RemoveFromAlarm(UINT32 alarmType);
    virtual void UseInDisarmedAlarm(UINT32 alarmType);
    virtual void RemoveFromDisarmedAlarm(UINT32 alarmType);
    virtual void HighlightAlarm(int enable, UINT32 endTime, void *key);

    /* Generic parameter assignment. */
    virtual int SetParameter(char *name, char *value);

protected:

    /*
     * Protected variables.
     */

    /*
     * Protected functions.
     */

    /* Picture. */
    virtual void SetPictureLink(SPxGwPictureBase *group);

    /* Rendering. */
    SPxErrorCode Render(SPxExtRenderer *renderer,
			int editEnabled=TRUE,
			int labelsEnabled=FALSE);

    virtual SPxErrorCode SetGroupSelected(int selected);

    virtual int IsShowItemName(void) const { return TRUE; }

    virtual void signalEvent(UINT32 flags);

    /* Used by other area objects to keep alarm count correct. */
    virtual void SetAlarmCount(UINT32 alarmType, int alarmCount);
    virtual void ForceShowDirectionIndicators(int show);

private:

    /*
     * Private variables.
     */
    struct impl;
    SPxAutoPtr<impl> m_p;

    /*
     * Private functions.
     */
    virtual SPxErrorCode renderAll(SPxExtRenderer * /*renderer*/) { return SPX_NO_ERROR; }
    SPxGwWidget *getNextWidget(void);
    virtual void enableEventCallbacks(int enable);
    void callEventCallbacks(SPxGwWidget *widget, UINT32 flags, int addCallbacks);

    /*
     * Private static functions.
     */

}; /* SPxGwWidgetGroup */

#endif /* _SPX_GW_WIDGET_GROUP_H */

/*********************************************************************
 *
 *      End of file
 *
 *********************************************************************/


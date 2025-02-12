/*********************************************************************
 *
 * (c) Copyright 2013 - 2022, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   SPx graphics widget polyline class.
 *
 * Revision Control:
 *  14/01/22 v1.16   SP 	Implement GetDistanceToPoint().
 *
 * Previous Changes:
 *  29/07/21 1.15   AGC	Support min/max heights.
 *  10/02/21 1.14   AGC	Move gate alarm logic to SPxGwWidget.
 *  14/06/18 1.13   BTB	Support flipping of forward crossing direction.
 *  15/05/18 1.12   BTB	Request redraw in external alarm count functions.
 *  11/05/18 1.11   BTB	Improvements suggested by AGC.
 *			Add GetAlarmCount() for better count management.
 *  10/05/18 1.10   BTB	Add SetAlarmCount() for use by SPxGwWidgetGroup.
 *			Make SPxGwWidgetGroup friend so it can call it safely.
 *  08/05/18 1.9    SP 	Add alarm functions to SPxAreaObj interface.
 *  03/05/18 1.8    BTB	Support rendering direction indicators.
 *  03/11/17 1.7    AGC	Support GetAreaType().
 *  31/10/17 1.6    AGC	Add IsCrossing() function.
 *  23/10/17 1.5    AGC	Improve mutex protection.
 *  18/07/14 1.4    AGC	Support AppendPoint() function.
 *  07/10/13 1.3    AGC	Move to SPxLibUtils.
 *			Make cross-platform.
 *			Add SPxAreaObj support.
 *  25/01/13 1.2    SP 	Allow group to be set in constructor.
 *  11/01/13 1.1    SP 	Initial version.
 *
 *********************************************************************/

#ifndef _SPX_GW_POLY_H
#define _SPX_GW_POLY_H

/* Other headers required. */
#include "SPxLibUtils/SPxAutoPtr.h"
#include "SPxLibUtils/SPxGwShape.h"

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

/* Forward declarations. */
class SPxExtRenderer;
class SPxGwWidgetGroup;

class SPxGwPoly : public SPxGwShape
{
public:

    /*
     * Public variables.
     */

    /*
     * Public functions.
     */

    /* Constructor and destructor. */
    SPxGwPoly(SPxGwWidgetGroup *group=NULL);
    virtual ~SPxGwPoly(void);

    /* Configuration. */
    SPxErrorCode SetPoints(const SPxGwPoint_t points[], 
                           unsigned int numPoints);

    SPxErrorCode GetPoints(unsigned int maxNumPoints,
                           SPxGwPoint_t pointsRtn[], 
                           unsigned int *numPointsRtn) const;

    virtual SPxErrorCode SetAutoCloseEnabled(int isEnabled);
    int IsAutoCloseEnabled(void) const;

    /* Old style pixels interface. */
    SPxErrorCode SetPointsPixels(const SPxGwPoint_t points[], 
                                 unsigned int numPoints);

    SPxErrorCode GetPointsPixels(unsigned int maxNumPoints,
                                 SPxGwPoint_t pointsRtn[], 
                                 unsigned int *numPointsRtn) const;

    /* SPxAreaObj interface. */
    virtual int GetAreaType(void) const;
    virtual int IsMovable(void) const { return TRUE; }
    virtual SPxErrorCode SetPos(double x, double y);
    virtual SPxErrorCode GetPos(double *x, double *y) const;
    virtual int IsPointWithin(double x, double y, double margin) const;
    virtual int IsPointWithin(double x, double y, double margin,
                              int /*includeChildren*/,
                              SPxRenderer * /*renderer*/,
                              double heightMetres) const;
    virtual int IsCrossing(double x1, double y1, double x2, double y2) const;

    virtual unsigned int GetNumPoints(void) const;
    virtual SPxErrorCode SetPointPos(unsigned int idx, double x, double y, int fixedRatio=FALSE);
    virtual SPxErrorCode GetPointPos(unsigned int idx, double *x, double *y) const;

    virtual int IsEditable(void) const { return TRUE; }
    virtual SPxErrorCode AddPoint(double x, double y);
    virtual SPxErrorCode RemovePoint(unsigned int idx);
    virtual SPxErrorCode AppendPoint(double x, double y);

    virtual int IsHeightSupported(void) const { return TRUE; }

    virtual int IsAutoCloseSupported(void) const { return TRUE; }

    virtual int IsDistanceToPointSupported(void) const { return TRUE; }
    virtual SPxErrorCode GetDistanceToPoint(double x, double y, double *distMetres) const;

    virtual SPxErrorCode LoadCfg(FILE *f);
    virtual SPxErrorCode SaveCfg(FILE *f) const;

    virtual int CanUseInAlarm(void) const { return TRUE; }

protected:
    /* Show item name? */
    virtual int IsShowItemName(void) const { return TRUE; }

private:

    /*
     * Private variables.
     */
    struct impl;
    SPxAutoPtr<impl> m_p;

    /* Auto close the poly? */
    int m_isAutoCloseEnabled;

    /*
     * Private functions.
     */
    virtual SPxErrorCode renderAll(SPxExtRenderer *renderer);
    int setPoints(const SPxGwPoint_t points[],
		  unsigned int numPoints);
    int isPointWithin(double x, double y, double margin,
                      int checkHeight,
                      double heightMetres) const;

}; /* SPxGwPoly */

#endif /* _SPX_GW_POLY_H */

/*********************************************************************
 *
 *      End of file
 *
 *********************************************************************/


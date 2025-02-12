/*********************************************************************
 *
 * (c) Copyright 2013, 2017, 2021 - 2022 Cambridge Pixel Ltd.
 *
 * Purpose:
 *   SPx graphics widget line class.
 *
 * Revision Control:
 *  17/01/22 v1.6    SP 	Implement GetDistanceToPoint().
 *
 * Previous Changes:
 *  10/02/21 1.5    AGC	Support use within gate alarms.
 *  03/11/17 1.4    AGC	Support GetAreaType().
 *  07/10/13 1.3    AGC	Move to SPxLibUtils.
 *			Make cross-platform.
 *			Add SPxAreaObj support.
 *  25/01/13 1.2    SP 	Allow group to be set in constructor.
 *  11/01/13 1.1    SP 	Initial version.
 *
 *********************************************************************/

#ifndef _SPX_GW_LINE_H
#define _SPX_GW_LINE_H

/* Other headers required. */
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

class SPxGwLine : public SPxGwShape
{
public:

    /*
     * Public variables.
     */

    /*
     * Public functions.
     */

    /* Constructor and destructor. */
    SPxGwLine(SPxGwWidgetGroup *group=NULL);
    virtual ~SPxGwLine(void);

    /* Configuration. */
    virtual SPxErrorCode SetStart(double x, double y);
    virtual double GetStartX(void) const { return m_startX; }
    virtual double GetStartY(void) const { return m_startY; }
    
    virtual SPxErrorCode SetEnd(double x, double y);
    virtual double GetEndX(void) const { return m_endX; }
    virtual double GetEndY(void) const { return m_endY; }

    /* Old style pixels configuration. */
    virtual SPxErrorCode SetStartPixels(double x, double y);
    virtual double GetStartXPixels(void) const { return m_startX; }
    virtual double GetStartYPixels(void) const { return m_startY; }
    
    virtual SPxErrorCode SetEndPixels(double x, double y);
    virtual double GetEndXPixels(void) const { return m_endX; }
    virtual double GetEndYPixels(void) const { return m_endY; }

    /* SPxAreaObj interface. */
    virtual int GetAreaType(void) const;
    virtual int IsMovable(void) const { return TRUE; }
    virtual SPxErrorCode SetPos(double x, double y);
    virtual SPxErrorCode GetPos(double *x, double *y) const;
    virtual int IsPointWithin(double x, double y, double margin) const;
    virtual int IsCrossing(double x1, double y1, double x2, double y2) const;

    virtual unsigned int GetNumPoints(void) const { return 2; }
    virtual SPxErrorCode SetPointPos(unsigned int idx, double x, double y, int fixedRatio=FALSE);
    virtual SPxErrorCode GetPointPos(unsigned int idx, double *x, double *y) const;

    virtual int IsDistanceToPointSupported(void) const { return TRUE; }
    virtual SPxErrorCode GetDistanceToPoint(double x, double y, double *distMetres) const;

    virtual SPxErrorCode LoadCfg(FILE *f);
    virtual SPxErrorCode SaveCfg(FILE *f) const;

    virtual int CanUseInAlarm(void) const { return TRUE; }
private:

    /*
     * Private variables.
     */

    /* Line start point. */
    double m_startX;
    double m_startY;

    /* Line end point. */
    double m_endX;
    double m_endY;

    /*
     * Private functions.
     */
    virtual SPxErrorCode renderAll(SPxExtRenderer *renderer);

}; /* SPxGwLine */

#endif /* _SPX_GW_LINE_H */

/*********************************************************************
 *
 *      End of file
 *
 *********************************************************************/


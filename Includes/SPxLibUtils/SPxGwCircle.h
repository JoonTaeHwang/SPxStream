/*********************************************************************
 *
 * (c) Copyright 2017, 2021 - 2022, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   SPx graphics widget circle class.
 *
 * Revision Control:
 *  14/01/22 v1.3    SP 	Implement GetDistanceToPoint().
 *
 * Previous Changes:
 *  03/08/21 1.3    AGC	Add required Set/GetPointPos() overloads.
 *  29/07/21 1.2    AGC	Support min/max heights.
 *  03/11/17 1.1    AGC	Initial version.
 *
 *********************************************************************/

#ifndef _SPX_GW_CIRCLE_H
#define _SPX_GW_CIRCLE_H

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
class SPxRenderer;
class SPxGwWidgetGroup;

class SPxGwCircle : public SPxGwShape
{
public:

    /*
     * Public variables.
     */

    /*
     * Public functions.
     */

    /* Constructor and destructor. */
    SPxGwCircle(SPxGwWidgetGroup *group=NULL);
    virtual ~SPxGwCircle(void);

    /* Configuration. */
    SPxErrorCode SetCentre(double x, double y);
    double GetCentreX(void) const { return m_centreX; }
    double GetCentreY(void) const { return m_centreY; }

    SPxErrorCode SetSize(double size);
    double GetSize(void) const { return m_size; }

    /* SPxAreaObj interface. */
    virtual int GetAreaType(void) const;
    virtual int IsMovable(void) const { return TRUE; }
    virtual SPxErrorCode SetPos(double x, double y);
    virtual SPxErrorCode GetPos(double *x, double *y) const;
    virtual int IsPointWithin(double x, double y, double margin) const
    {
	return IsPointWithin(x, y, margin, FALSE);
    }
    virtual int IsPointWithin(double x, double y, double margin,
			      int includeChildren) const
    {
	return IsPointWithin(x, y, margin, includeChildren, NULL);
    }
    virtual int IsPointWithin(double x, double y, double margin,
			      int includeChildren,
			      SPxRenderer *renderer) const
    {
	return isPointWithin(x, y, margin, includeChildren, renderer, FALSE, 0.0);
    }
    virtual int IsPointWithin(double x, double y, double margin,
			      int includeChildren,
			      SPxRenderer *renderer,
			      double heightMetres) const
    {
	return isPointWithin(x, y, margin, includeChildren, renderer, TRUE, heightMetres);
    }

    virtual unsigned int GetNumPoints(void) const { return 1; }
    virtual SPxErrorCode SetPointPos(unsigned int idx, double x, double y,
				     int fixedRatio=FALSE)
    {
	return SetPointPos(idx, x, y, fixedRatio, NULL);
    }
    virtual SPxErrorCode SetPointPos(unsigned int idx, double x, double y,
				     int fixedRatio, SPxRenderer *renderer);
    virtual SPxErrorCode GetPointPos(unsigned int idx, double *x, double *y) const
    {
	return GetPointPos(idx, x, y, NULL);
    }
    virtual SPxErrorCode GetPointPos(unsigned int idx, double *x, double *y,
				     SPxRenderer *renderer) const;

    virtual int IsHeightSupported(void) const { return TRUE; }

    virtual int IsDistanceToPointSupported(void) const { return TRUE; }
    virtual SPxErrorCode GetDistanceToPoint(double x, double y, double *distMetres) const;

    virtual SPxErrorCode LoadCfg(FILE *f);
    virtual SPxErrorCode SaveCfg(FILE *f) const;

protected:
    /* Show item name? */
    virtual int IsShowItemName(void) const { return TRUE; }

private:

    /*
     * Private variables.
     */
    double m_centreX;
    double m_centreY;
    double m_size;
    double m_sizeY;

    /*
     * Private functions.
     */
    double radius(double angleRads) const;
    virtual SPxErrorCode renderAll(SPxExtRenderer *renderer);
    int isPointWithin(double x, double y, double margin,
		      int includeChildren,
		      SPxRenderer *renderer,
		      int checkHeight,
		      double heightMetres) const;

}; /* SPxGwCircle */

#endif /* _SPX_GW_CIRCLE_H */

/*********************************************************************
 *
 *      End of file
 *
 *********************************************************************/


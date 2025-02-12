/*********************************************************************
 *
 * (c) Copyright 2013, 2017, 2021 - 2022, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   SPx graphics widget rectangle class.
 *
 * Revision Control:
 *  17/01/22 v1.6    SP 	Implement GetDistanceToPoint().
 *
 * Previous Changes:
 *  29/07/21 1.5    AGC	Support min/max heights.
 *  03/11/17 1.4    AGC	Support GetAreaType().
 *  07/10/13 1.3    AGC	Move to SPxLibUtils.
 *			Make cross-platform.
 *			Add SPxAreaObj support.
 *  25/01/13 1.2    SP 	Allow group to be set in constructor.
 *  11/01/13 1.1    SP 	Initial version.
 *
 *********************************************************************/

#ifndef _SPX_GW_RECT_H
#define _SPX_GW_RECT_H

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

class SPxGwRect : public SPxGwShape
{
public:

    /*
     * Public variables.
     */

    /*
     * Public functions.
     */

    /* Constructor and destructor. */
    SPxGwRect(SPxGwWidgetGroup *group=NULL);
    virtual ~SPxGwRect(void);

    /* Configuration. */
    virtual SPxErrorCode SetTopLeft(double x, double y);
    virtual double GetTopLeftX(void) const { return m_tlX; }
    virtual double GetTopLeftY(void) const { return m_tlY; }
    
    virtual SPxErrorCode SetSize(double width, double height);
    virtual double GetWidth(void) const { return m_width; }
    virtual double GetHeight(void) const { return m_height; }

    /* Old style pixels interface. */
    virtual SPxErrorCode SetTopLeftPixels(double x, double y);
    virtual double GetTopLeftXPixels(void) const { return GetTopLeftX(); }
    virtual double GetTopLeftYPixels(void) const { return GetTopLeftY(); }
    
    virtual SPxErrorCode SetSizePixels(double width, double height);
    virtual double GetWidthPixels(void) const { return GetWidth(); }
    virtual double GetHeightPixels(void) const { return GetHeight(); }

    /* SPxAreaObj interface. */
    virtual int GetAreaType(void) const;
    virtual int IsMovable(void) const { return TRUE; }
    virtual SPxErrorCode SetPos(double x, double y);
    virtual SPxErrorCode GetPos(double *x, double *y) const;
    virtual int IsPointWithin(double x, double y, double margin) const
    {
	return isPointWithin(x, y, margin, FALSE, 0.0);
    }
    virtual int IsPointWithin(double x, double y, double margin,
                              int /*includeChildren*/,
                              SPxRenderer * /*renderer*/,
                              double heightMetres) const
    {
        return isPointWithin(x, y, margin, TRUE, heightMetres);
    }

    virtual unsigned int GetNumPoints(void) const { return 4; }
    virtual SPxErrorCode SetPointPos(unsigned int idx, double x, double y, int fixedRatio=FALSE);
    virtual SPxErrorCode GetPointPos(unsigned int idx, double *x, double *y) const;

    virtual int IsHeightSupported(void) const { return TRUE; }

    virtual int IsDistanceToPointSupported(void) const { return TRUE; }
    virtual SPxErrorCode GetDistanceToPoint(double x, double y, double *distMetres) const;

    virtual SPxErrorCode LoadCfg(FILE *f);
    virtual SPxErrorCode SaveCfg(FILE *f) const;

private:

    /*
     * Private variables.
     */

    /* Rectangle top-left point. */
    double m_tlX;
    double m_tlY;

    /* Rectangle size. */
    double m_width;
    double m_height;

    /*
     * Private functions.
     */
    virtual SPxErrorCode renderAll(SPxExtRenderer *renderer);
    int isPointWithin(double x, double y, double margin,
                      int checkHeight,
                      double heightMetres) const;

}; /* SPxGwRect */

#endif /* _SPX_GW_RECT_H */

/*********************************************************************
 *
 *      End of file
 *
 *********************************************************************/


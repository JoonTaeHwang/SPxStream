/*********************************************************************
 *
 * (c) Copyright 2013, 2017, 2021 - 2022, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   SPx graphics widget sector class.
 *
 * Revision Control:
 *  14/01/22 v1.8    SP 	Implement GetDistanceToPoint().
 *
 * Previous Changes:
 *  29/07/21 1.7    AGC	Support min/max heights.
 *  03/11/17 1.6    AGC	Support GetAreaType().
 *  23/10/17 1.5    AGC	Support IsShowItemName().
 *  27/11/13 1.4    AGC	Fix lat/long sector point adjustment.
 *  07/10/13 1.3    AGC	Move to SPxLibUtils.
 *			Make cross-platform.
 *			Add SPxAreaObj support.
 *  25/01/13 1.2    SP 	Allow group to be set in constructor.
 *                      Support inner and outer radius.
 *                      File renamed from SPxGwSegment.
 *  11/01/13 1.1    SP 	Initial version.
 *
 *********************************************************************/

#ifndef _SPX_GW_SECTOR_H
#define _SPX_GW_SECTOR_H

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

class SPxGwSector : public SPxGwShape
{
public:

    /*
     * Public variables.
     */

    /*
     * Public functions.
     */

    /* Constructor and destructor. */
    SPxGwSector(SPxGwWidgetGroup *group=NULL);
    virtual ~SPxGwSector(void);

    /* Configuration. */
    SPxErrorCode SetCentre(double x, double y);
    double GetCentreX(void) const { return m_centreX; }
    double GetCentreY(void) const { return m_centreY; }
    
    SPxErrorCode SetInnerRadius(double len);
    double GetInnerRadius(void) const { return m_innerRadius; }

    SPxErrorCode SetOuterRadius(double len);
    double GetOuterRadius(void) const { return m_outerRadius; }

    SPxErrorCode SetStartDegs(double degs);
    double GetStartDegs(void) const { return m_startDegs; }

    SPxErrorCode SetEndDegs(double degs);
    double GetEndDegs(void)const { return m_endDegs; }

    /* Old style pixels interface. */
    SPxErrorCode SetCentrePixels(double x, double y);
    double GetCentreXPixels(void) const { return GetCentreX(); }
    double GetCentreYPixels(void) const { return GetCentreY(); }

    SPxErrorCode SetInnerRadiusPixels(double len);
    double GetInnerRadiusPixels(void) const { return GetInnerRadius(); }

    SPxErrorCode SetOuterRadiusPixels(double len);
    double GetOuterRadiusPixels(void) const { return GetOuterRadius(); }

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

protected:
    /* Show item name? */
    virtual int IsShowItemName(void) const { return TRUE; }

private:

    /*
     * Private variables.
     */
    double m_centreX;
    double m_centreY;
    double m_innerRadius;
    double m_outerRadius;
    double m_startDegs;
    double m_endDegs;
    double m_yOverX; /* Scaling between y and x units. */

    /*
     * Private functions.
     */
    virtual SPxErrorCode renderAll(SPxExtRenderer *renderer);
    int isPointWithin(double x, double y, double margin,
                      int checkHeight,
                      double heightMetres) const;

}; /* SPxGwSector */

#endif /* _SPX_GW_SECTOR_H */

/*********************************************************************
 *
 *      End of file
 *
 *********************************************************************/


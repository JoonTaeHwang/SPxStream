/*********************************************************************
 *
 * (c) Copyright 2013 - 2017, 2021 - 2022 Cambridge Pixel Ltd.
 *
 * Purpose:
 *   SPx graphics widget symbol class.
 *
 * Revision Control:
 *  13/01/22 v1.14   SP 	Implement GetDistanceToPoint().
 *
 * Previous Changes:
 *  15/06/21 1.13   AGC	Add Set/GetPointPos() overloads without renderer.
 *  08/11/17 1.12   AGC	Support set of point pos before first render.
 *  03/11/17 1.11   AGC	Support GetAreaType().
 *			Support new IsPointWithin().
 *  31/10/17 1.10   AGC	Support point functions with renderer arg.
 *  27/01/14 1.9    AGC	Fix gcc warning.
 *  22/01/14 1.8    AGC	Support optional symbol scaling.
 *  27/11/13 1.7    AGC	Fixed pixel size for symbols.
 *  26/11/13 1.6    AGC	Use SPxSymbol for symbol rendering.
 *  07/10/13 1.5    AGC	Move to SPxLibUtils.
 *			Make cross-platform.
 *			Add SPxAreaObj support.
 *  25/01/13 1.4    SP 	Allow group to be set in constructor.
 *  15/01/13 1.3    SP  Set centre position, not top-left.
 *  14/01/13 1.2    REW	Remove trailing comma in enum to avoid warnings.
 *  11/01/13 1.1    SP 	Initial version.
 *********************************************************************/

#ifndef _SPX_GW_SYMBOL_H
#define _SPX_GW_SYMBOL_H

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

/* Symbol types. */
typedef SPxSymbolShape_t SPxGwSymbol_t;

/*********************************************************************
 *
 *   Class definitions
 *
 *********************************************************************/

/* Forward declarations. */
class SPxExtRenderer;
class SPxGwWidgetGroup;
class SPxRenderer;

class SPxGwSymbol : public SPxGwShape
{
public:

    /*
     * Public variables.
     */

    /*
     * Public functions.
     */

    /* Constructor and destructor. */
    SPxGwSymbol(SPxGwWidgetGroup *group=NULL);
    virtual ~SPxGwSymbol(void);

    /* Configuration. */
    virtual SPxErrorCode SetSymbol(SPxSymbolShape_t sym);
    virtual SPxSymbolShape_t GetSymbol(void) const { return m_symbol.GetShape(0); }

    virtual SPxErrorCode EnableScaling(int enableScaling);
    virtual int IsScalingEnabled(void) const { return m_scalingEnabled; }

    virtual SPxErrorCode SetCentre(double x, double y);
    virtual double GetCentreX(void) const { return m_centreX; }
    virtual double GetCentreY(void) const { return m_centreY; }
    
    virtual SPxErrorCode SetSizePixels(double len);
    virtual double GetSizePixels(void) const { return m_length; }

    /* Old style pixels interface. */
    virtual SPxErrorCode SetCentrePixels(double x, double y);
    virtual double GetCentreXPixels(void) const { return GetCentreX(); }
    virtual double GetCentreYPixels(void) const { return GetCentreY(); }

    /* SPxAreaObj interface. */#
    virtual int GetAreaType(void) const;
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

    virtual unsigned int GetNumPoints(void) const { return 4; }
    virtual SPxErrorCode SetPointPos(unsigned int idx, double x, double y, int fixedRatio, SPxRenderer *renderer=NULL);
    virtual SPxErrorCode GetPointPos(unsigned int idx, double *x, double *y, SPxRenderer *renderer=NULL) const;
    virtual SPxErrorCode SetPointPos(unsigned int idx, double x, double y, int fixedRatio=FALSE) { return SetPointPos(idx, x, y, fixedRatio, NULL); }
    virtual SPxErrorCode GetPointPos(unsigned int idx, double *x, double *y) const { return GetPointPos(idx, x, y, NULL); }

    virtual int IsSymbolSupported(void) const { return TRUE; }

    virtual int IsDistanceToPointSupported(void) const { return TRUE; }
    virtual SPxErrorCode GetDistanceToPoint(double x, double y, double *distMetres) const;

    virtual SPxErrorCode LoadCfg(FILE *f);
    virtual SPxErrorCode SaveCfg(FILE *f) const;

private:

    /*
     * Private variables.
     */
    struct impl;
    SPxAutoPtr<impl> m_p;

    /* Symbol to draw. */
    SPxSymbol m_symbol;

    /* Symbol centre point. */
    double m_centreX;
    double m_centreY;

    /* Symbol size. */
    double m_length;

    /* Is symbol scaling enabled? */
    int m_scalingEnabled;

    /* Is a one-off rescaling required? */
    int m_rescaleRequired;

    /*
     * Private functions.
     */
    virtual SPxErrorCode renderAll(SPxExtRenderer *renderer);
    void updateRendererInfo(SPxRenderer *renderer);

}; /* SPxGwSymbol */

#endif /* _SPX_GW_SYMBOL_H */

/*********************************************************************
 *
 *      End of file
 *
 *********************************************************************/


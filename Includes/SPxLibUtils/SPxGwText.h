/*********************************************************************
 *
 * (c) Copyright 2013, 2017, 2018, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   SPx graphics widget text class.
 *
 * Revision Control:
 *  25/06/18 v1.8    AGC	Correctly suppress cppcheck warnings.
 *
 * Previous Changes:
 *  03/11/17 1.7    AGC	Support GetAreaType().
 *			Support new IsPointWithin().
 *  31/10/17 1.6    AGC	Support point functions with renderer arg.
 *  07/10/13 1.5    AGC	Move to SPxLibUtils.
 *			Make cross-platform.
 *			Add SPxAreaObj support.
 *  20/03/13 1.4    SP 	Support text alignment.
 *  25/01/13 1.3    SP 	Allow group to be set in constructor.
 *  15/01/13 1.2    SP 	Add GetExtents().
 *  11/01/13 1.1    SP 	Initial version.
 *
 *********************************************************************/

#ifndef _SPX_GW_TEXT_H
#define _SPX_GW_TEXT_H

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

/* Forward declare any classes we need. */
class SPxExtRenderer;

class SPxGwText : public SPxGwShape
{
public:

    /*
     * Public variables.
     */

    /*
     * Public functions.
     */

    /* Constructor and destructor. */
    SPxGwText(SPxGwWidgetGroup *group=NULL);
    virtual ~SPxGwText(void);

    /* Configuration. */
    // cppcheck-suppress virtualCallInConstructor
    virtual SPxErrorCode SetText(const char *str);
    virtual SPxErrorCode GetText(char *buf, unsigned int bufLen) const;
    // cppcheck-suppress virtualCallInConstructor
    virtual const char *GetText(void) const;

    // cppcheck-suppress virtualCallInConstructor
    virtual SPxErrorCode SetFontName(const char *name);
    virtual SPxErrorCode GetFontName(char *buf, unsigned int bufLen) const;
    // cppcheck-suppress virtualCallInConstructor
    virtual const char *GetFontName(void) const;

    virtual SPxErrorCode SetFontSize(unsigned int size);
    virtual unsigned int GetFontSize(void) const { return m_fontSize; }

    virtual SPxErrorCode SetOffset(double x, double y);
    virtual double GetOffsetX(void) const { return m_x; }
    virtual double GetOffsetY(void) const { return m_y; }

    virtual SPxErrorCode SetAlignment(SPxGwAlignment_t alignment);
    virtual SPxGwAlignment_t GetAlignment(void) const { return m_alignment; }

    SPxErrorCode GetSizePixels(double *widthRtn, double *heighRtn,
	SPxExtRenderer *renderer);

    /* Old style pixels interface. */
    virtual SPxErrorCode SetOffsetPixels(double x, double y);
    virtual double GetOffsetXPixels(void) const { return GetOffsetX(); }
    virtual double GetOffsetYPixels(void) const { return GetOffsetY(); }

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
			      int includeChildren) const {
	return IsPointWithin(x, y, margin, includeChildren, NULL);
    }
    virtual int IsPointWithin(double x, double y, double margin,
			      int includeChildren,
			      SPxRenderer *renderer) const;

    virtual int IsOffsetable(void) const { return TRUE; }
    virtual SPxErrorCode SetPixelOffset(double x, double y);
    virtual SPxErrorCode GetPixelOffset(double *x, double *y) const;

    virtual int IsTextSupported(void) const { return TRUE; }
    virtual int IsAlignmentSupported(void) const { return TRUE; }

    virtual SPxErrorCode LoadCfg(FILE *f);
    virtual SPxErrorCode SaveCfg(FILE *f) const;

private:

    /*
     * Private variables.
     */
    struct impl;
    SPxAutoPtr<impl> m_p;

    /* Text position. */
    double m_x;
    double m_y;
    double m_offX;
    double m_offY;
    SPxGwAlignment_t m_alignment;

    /* Font. */
    unsigned int m_fontSize;

    /*
     * Private functions.
     */
    virtual SPxErrorCode renderAll(SPxExtRenderer *renderer);

}; /* SPxGwText */

#endif /* _SPX_GW_TEXT_H */

/*********************************************************************
 *
 *      End of file
 *
 *********************************************************************/


/*********************************************************************
 *
 * (c) Copyright 2013 - 2018,, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   SPx graphics widget image class.
 *
 * Revision Control:
 *  25/06/18 v1.9    AGC	Correctly suppress cppcheck warning.
 *
 * Previous Changes:
 *  03/11/17 1.8    AGC	Support GetAreaType().
 *  08/07/16 1.7    AGC	Support loading image from resource under Windows.
 *  22/11/13 1.6    AGC	Request redraw on modification.
 *  07/10/13 1.5    AGC	Move to SPxLibUtils.
 *			Make cross-platform.
 *			Add SPxAreaObj support.
 *  08/03/13 1.4    SP 	Derive from SPxGwImageBuffer.
 *  25/01/13 1.3    SP 	Allow group to be set in constructor.
 *  15/01/13 1.2    SP 	Set position as centre, not top-left.
 *  11/01/13 1.1    SP 	Initial version.
 *
 *********************************************************************/

#ifndef _SPX_GW_IMAGE_H
#define _SPX_GW_IMAGE_H

/* Other headers required. */
#include "SPxLibUtils/SPxGwImageBuffer.h"
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

class SPxGwImage : public SPxGwImageBuffer, public SPxGwShape 
{
public:

    /*
     * Public variables.
     */

    /*
     * Public functions.
     */

    /* Constructor and destructor. */
    SPxGwImage(SPxGwWidgetGroup *group=NULL);
    virtual ~SPxGwImage(void);

    virtual SPxErrorCode LoadFile(const char *filename);
#ifdef _WIN32
    virtual SPxErrorCode LoadResource(int resourceID);
#endif
    virtual SPxErrorCode LoadFromBuffer(SPxGwImageBuffer *imgBuf);

    virtual SPxErrorCode SetCentre(double x, double y);
    virtual double GetCentreX(void) const { return m_centreX; }
    virtual double GetCentreY(void) const { return m_centreY; }
    virtual SPxErrorCode SetSize(double width, double height, int set=TRUE);
    virtual double GetWidth(void) const;
    virtual double GetHeight(void) const;

    /* Old style pixels interface. */
    virtual SPxErrorCode SetCentrePixels(double x, double y);
    virtual double GetCentreXPixels(void) const { return GetCentreX(); }
    virtual double GetCentreYPixels(void) const { return GetCentreY(); }

    /* SPxAreaObj interface. */
    virtual int GetAreaType(void) const;
    virtual int IsMovable(void) const { return TRUE; }
    virtual SPxErrorCode SetPos(double x, double y);
    virtual SPxErrorCode GetPos(double *x, double *y) const;
    virtual int IsPointWithin(double x, double y, double margin) const;

    virtual unsigned int GetNumPoints(void) const { return 4; }
    virtual SPxErrorCode SetPointPos(unsigned int idx, double x, double y, int fixedRatio=FALSE);
    virtual SPxErrorCode GetPointPos(unsigned int idx, double *x, double *y) const;

    virtual int IsColourSupported(void) const { return FALSE; }

    virtual int IsImageSupported(void) const { return TRUE; }
    virtual SPxErrorCode SetImage(const char *path) { return LoadFile(path); }
    virtual const char *GetFullImagePath(void) const;
    // cppcheck-suppress virtualCallInConstructor
    virtual const char *GetShortImagePath(void) const;

    virtual SPxErrorCode LoadCfg(FILE *f);
    virtual SPxErrorCode SaveCfg(FILE *f) const;

private:

    /*
     * Private variables.
     */

    /* Image centre point. */
    double m_centreX;
    double m_centreY;

    /* Scaled image size. */
    int m_sizeSet;
    double m_width;
    double m_height;

    /*
     * Private functions.
     */
    virtual SPxErrorCode renderAll(SPxExtRenderer *renderer);

}; /* SPxGwImage */

#endif /* _SPX_GW_IMAGE_H */

/*********************************************************************
 *
 *      End of file
 *
 *********************************************************************/


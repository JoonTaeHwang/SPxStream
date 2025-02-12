/*********************************************************************
 *
 * (c) Copyright 2013 - 2022, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   SPx Video Simulator SPxVidSimMoveable class header.
 *
 * Revision Control:
 *   30/08/22 v1.10  SP 	Move ZRefMode_t here.
 *
 * Previous Changes:
 *   29/07/22 1.9   SP 	Major rework to correctly apply parent's pitch and roll.
 *   22/07/22 1.8   SP 	Changes to simplify creation and deletion.
 *   17/08/18 1.7   AGC	Add pitch/yaw error support.
 *   13/08/18 1.6   AGC	Add lean support.
 *   10/08/18 1.5   AGC	Handle parent object deletion.
 *   02/07/18 1.4   AGC	Make various functions const.
 *   25/06/18 1.3   SP 	Major refactor to support parent
 *                     	relative offsets.
 *   07/10/16 1.2   SP 	Support lat/long position.
 *   15/07/13 1.1   SP 	Initial version.
 *
 *********************************************************************/

#ifndef _SPX_VID_SIM_MOVABLE_H
#define _SPX_VID_SIM_MOVABLE_H

/* Other headers required. */
#include "SPxLibUtils/SPxVidSimObj.h"

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

class SPxVidSimMovable : public SPxVidSimObj
{
public:

    /*
     * Public types.
     */

    /* Z reference modes. */
    enum ZRefMode_t
    {
        Z_REF_MODE_SEA_LEVEL    = 0,   /* Sea level. */
        Z_REF_MODE_TERRAIN      = 1    /* Terrain. */
    };

    /*
     * Public variables.
     */

    /*
     * Public functions.
     */

    /* Constructor and destructor. */
    SPxVidSimMovable(ObjectType_t objectType, SPxVidSimObj *parent);
    virtual ~SPxVidSimMovable(void);

    /* Absolute lat/long position. */
    virtual SPxErrorCode SetLatLong(double latDegs, double lonDegs);
    virtual double GetLatDegs(void) const;
    virtual double GetLongDegs(void) const;

    /* Absolute position in metres, relative to top-level ancestor. */
    virtual double GetXMetres(void) const;
    virtual double GetYMetres(void) const;
    virtual double GetZMetres(void) const;

    /* Positional offset from parent in metres. */
    virtual SPxErrorCode SetXOffsetMetres(double degs);
    virtual double GetXOffsetMetres(void) const;
    virtual SPxErrorCode SetYOffsetMetres(double degs);
    virtual double GetYOffsetMetres(void) const;
    virtual SPxErrorCode SetZOffsetMetres(double degs);
    virtual double GetZOffsetMetres(void) const;

    /* Absolute rotation in degrees, relative to top-level ancestor. */
    virtual double GetPitchDegs(void) const;
    virtual double GetRollDegs(void) const;
    virtual double GetYawDegs(void) const;

    /* Rotational offset from parent in degrees. */
    virtual SPxErrorCode SetPitchOffsetDegs(double degs);
    virtual double GetPitchOffsetDegs(void) const;
    virtual SPxErrorCode SetRollOffsetDegs(double degs);
    virtual double GetRollOffsetDegs(void) const;
    virtual SPxErrorCode SetYawOffsetDegs(double degs);
    virtual double GetYawOffsetDegs(void) const;

protected:

    /*
     * Protected variables.
     */

    /*
     * Protected functions.
     */

    /* Parameter handling. */
    virtual int SetParameter(char *name, char *value);
    virtual int GetParameter(char *name, char *valueBuf, int bufLen);

    /* Event handling. */
    virtual void HandleEvent(const EventInfo_t *eventInfo);

    /* Recalculation. */
    SPxErrorCode Update(void);

private:

    /*
     * Private variables.
     */

    /* World lat/long position. */
    int m_isPositionAbsolute;
    double m_latDegs;           /* Degrees +ve north, -ve south. */
    double m_lonDegs;           /* Degrees +ve east, -ve west. */ 

    /* Asolute X/Y/Z position, relative to top-level ancestor. */
    double m_xMetres;           /* Metres +ve east, -ve west. */ 
    double m_yMetres;           /* Metres +ve north, -ve south. */
    double m_zMetres;           /* Metres +ve up from ground surface, -ve down. */

    /* X/Y/Z offset, relative to parent. */
    double m_xOffsetMetres;     /* Metres +ve right, -ve left. */ 
    double m_yOffsetMetres;     /* Metres +ve forward, -ve backward. */ 
    double m_zOffsetMetres;     /* Metres +ve up from parent surface, -ve down. */

    /* Absolute pitch/roll/yaw, relative to top-level ancestor. */
    double m_pitchDegs;         /* Rotation on X axis, +ve up from ground surface. */
    double m_rollDegs;          /* Rotation on Y axis, +ve up from ground surface. */
    double m_yawDegs;           /* Rotation on Z axis, +ve clockwise from north. */

    /* Pitch/roll/yaw offset relative to parent. */
    double m_pitchOffsetDegs;   /* Rotation on X axis, +ve up from platform surface. */
    double m_rollOffsetDegs;    /* Rotation on Y axis, +ve up from platform surface. */
    double m_yawOffsetDegs;     /* Rotation on Z axis, +ve clockwise from platform front. */

    /*
     * Private functions.
     */

    /* General. */
    void rotateXYZ(double xMetres,
                   double yMetres,
                   double zMetres,
                   double pitchDegs, 
                   double rollDegs, 
                   double yawDegs,
                   double *xMetresRtn,
                   double *yMetresRtn,
                   double *zMetresRtn) const;

    void combineRotations(double pitch1Degs, 
                          double roll1Degs, 
                          double yaw1Degs,
                          double pitch2Degs, 
                          double roll2Degs, 
                          double yaw2Degs,
                          double *pitchDegs, 
                          double *rollDegs, 
                          double *yawDegs)  const;

    /* Matrix manipulation. */
    void pitch3x3(double degs, double m[][3])  const;
    void roll3x3(double degs, double m[][3])  const;
    void yaw3x3(double degs, double m[][3])  const;
    void copy3x1(const double a[][1], double m[][1])  const;
    void copy3x3(const double a[][3], double m[][3])  const;
    void mult3x3_3x1(const double a[][3], const double b[][1], double m[][1])  const;
    void mult3x3_3x3(const double a[][3], const double b[][3], double m[][3])  const;

    /* Test functions. */
    SPxErrorCode testRotateXYZ(void) const;
    SPxErrorCode testCombineRotations(void) const;

    /*
     * Private static functions.
     */

}; /* SPxVidSimMovable */

#endif /* _SPX_VID_SIM_MOVEABLE_H */

/*********************************************************************
 *
 *      End of file
 *
 *********************************************************************/

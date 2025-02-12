/*********************************************************************
*
* (c) Copyright 2018, Cambridge Pixel Ltd.
*
* Purpose:
*   Types and definitions used by various camera correction related
*   classes.
*
* Revision Control:
*   26/10/18 v1.4    AGC	Support calculating camera height.
*
* Previous Changes:
*   24/08/18 1.3   BTB	Change namespace name to avoid future conflicts.
*   23/08/18 1.2   BTB	Add enumeration for point selection mode.
*   17/08/18 1.1   BTB	Initial Version.
* 
**********************************************************************/

#ifndef _SPX_CAM_CORR_DEFS_H
#define _SPX_CAM_CORR_DEFS_H

namespace SPxCamCorrectionNS
{
    #define SPX_MAX_CAM_CORRECTION_POINTS 4

    enum CorrectionPointMode_t
    {
	CORR_POINT_MODE_MANUAL = 0,
	CORR_POINT_MODE_HORIZON = 1
    };

    struct CorrectionPoint
    {
	CorrectionPoint()
	{
	    mode = CORR_POINT_MODE_MANUAL;
	    pointLat = 0.0;
	    pointLong = 0.0;
	    pointHeight = 0.0;
	    pointRange = 0.0;
	    pointBearing = 0.0;
	    pointTilt = 0.0;
	    correctionTilt = 0.0;
	}

	CorrectionPointMode_t mode;

	double pointLat;
	double pointLong;
	double pointHeight;

	double pointRange;
	double pointBearing;

	double pointTilt;
	double correctionTilt;
    };
}

#endif /* SPX_CAM_CORR_DEFS_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/

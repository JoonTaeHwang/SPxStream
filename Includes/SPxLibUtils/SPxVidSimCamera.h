/*********************************************************************
 *
 * (c) Copyright 2013 - 2022, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   SPx Video Simulator SPxVidSimCamera class header.
 *
 * Revision Control:
 *   27/09/22 v1.16   AGC	Remove unused functions.
 *
 * Previous Changes:
 *   27/09/22 1.15  AGC	Rename SPxVidSimCameraMount to SPxVidSimMount.
 *   04/08/22 1.14  SP 	Derive from SPxVidSimMovable.
 *   22/07/22 1.13  SP 	Changes to simplify creation and deletion.
 *   13/08/18 1.12  AGC	Enhance relative focus support.
 *   10/08/18 1.11  AGC	Use SPxCamCtrlRecv to process camera commands.
 *                     	Use SPxCamSim... to simulate camera behaviour.
 *   02/07/18 1.10  AGC	Derive from new interface.
 *                     	Make various functions const.
 *   18/10/17 1.9   AGC	Add SetAbsZoomSpeed().
 *   10/10/17 1.8   AGC	Add GetZoomSpeed().
 *   07/10/16 1.7   SP 	Move events to base class.
 *   15/10/15 1.6   SP 	Remove unused functions and types.
 *   14/10/15 1.5   SP 	Support colour mode and brightness.
 *   04/09/15 1.4   SP 	Support FOV LUT.
 *   26/08/15 1.3   SP 	Improve FOV speed control.
 *   06/08/15 1.2   SP 	Support remote control.
 *   15/07/13 1.1   SP 	Initial version.
 *
 *********************************************************************/

#ifndef _SPX_VID_SIM_CAMERA_H
#define _SPX_VID_SIM_CAMERA_H

/* Other headers required. */
#include "SPxLibUtils/SPxVidSimMovable.h"
#include "SPxLibData/SPxCamCtrlBase.h"
#include "SPxLibData/SPxCamZoomIface.h"

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

/* Forward declare any classes required. */
class SPxVidSimMount;
class SPxCamCtrlRecv;
class SPxCamZoomIface;
class SPxCamFocusIface;
class SPxCamPresetIface;

class SPxVidSimCamera : public SPxVidSimMovable
{
public:

    /*
     * Public types.
     */

    /*
     * Public variables.
     */

    /*
     * Public functions.
     */

    /* Constructor and destructor. */
    explicit SPxVidSimCamera(SPxVidSimMount *mount, int index=0);
    virtual ~SPxVidSimCamera(void);
    SPxErrorCode Initialise(void);

    /* Configuration. */
    SPxErrorCode SetFrameSizePixels(unsigned int width, unsigned int height);
    unsigned int GetFrameWidthPixels(void) const;
    unsigned int GetFrameHeightPixels(void) const;
    SPxErrorCode SetFramesPerSec(unsigned int fps);
    unsigned int GetFramesPerSec(void) { return m_fps; }
    SPxErrorCode SetFieldOfViewDegs(double degs);
    double GetFieldOfViewDegs(void) const;
    double GetMinFieldOfViewDegs(void) const;
    double GetMaxFieldOfViewDegs(void) const;
    int IsRelFocusSupported(void) const;
    int IsAbsFocusSupported(void) const;
    int IsAutoFocusEnabled(void) const;
    UINT16 GetFocusPos(void) const;
    double GetFocusRangeMetres(void) const;
    SPxErrorCode SetColourMode(ColourMode_t mode);
    ColourMode_t GetColourMode(void) const;
    SPxErrorCode SetBackgroundBrightness(double brightness);
    double GetBackgroundBrightness(void) const;
    SPxErrorCode SetTargetBrightness(double brightness);
    double GetTargetBrightness(void) const;

    SPxCamZoomIface *GetZoomIface(void);
    SPxCamFocusIface *GetFocusIface(void);

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

private:

    /*
     * Private variables.
     */
    class SimZoomFocus;

    struct impl;
    SPxAutoPtr<impl> m_p;

    /* Camera settings. */
    unsigned int m_frameWidthPixels;    /* Frame width in pixels. */
    unsigned int m_frameHeightPixels;   /* Frame height in pixels. */
    unsigned int m_fps;                 /* Frames per second. */

    ColourMode_t m_colourMode;          /* Colour, greyscale, etc. */
    double m_backgroundBrightness;      /* Background brightness. */
    double m_targetBrightness;          /* Target brightness. */


    /*
     * Private static variables.
     */

    static unsigned int m_nextCameraNum; /* Used to name cameras. */

    /*
     * Private functions.
     */

    /*
     * Private static functions.
     */

}; /* SPxVidSimCamera */

#endif /* _SPX_VID_SIM_CAMERA_H */

/*********************************************************************
 *
 *      End of file
 *
 *********************************************************************/

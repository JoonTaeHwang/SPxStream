/*********************************************************************
*
* (c) Copyright 2018, 2019, 2022 Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxCamCtrlRecv class - a base class
*	for receiving and responding to camera control messages.
*
* Revision Control:
*   27/09/22 v1.6    AGC	Support measuring range.
*
* Previous Changes:
*   21/01/19 1.5    AGC	Support pan/tilt/zoom timeouts.
*   10/10/18 1.4    AGC	Improve auto-focus support reporting.
*   20/08/18 1.3    AGC	Support connecting track database.
*			Support setting lat/long/height.
*   17/08/18 1.2    AGC	Move SetSrcType() to derived classes.
*			Add vid size support.
*			Add primary sensor support.
*   10/08/18 1.1    AGC	Initial version.
**********************************************************************/
#ifndef _SPX_CAM_CTRL_RECV_H
#define _SPX_CAM_CTRL_RECV_H

/* For base class. */
#include "SPxLibNet/SPxDecoderRepeater.h"

/* Forward declarations. */
class SPxNetTCP;
class SPxCamPanTiltIface;
class SPxCamZoomIface;
class SPxCamFocusIface;
class SPxCamPresetIface;
class SPxCamMeasureRangeIface;
class SPxTrackDatabase;
struct SPxLatLong_tag;

class SPxCamCtrlRecv : public SPxDecoderRepeater
{
public:
    SPxCamCtrlRecv(void);
    virtual ~SPxCamCtrlRecv(void);

    SPxErrorCode SetPanTiltIface(SPxCamPanTiltIface *panTiltIface);
    SPxErrorCode SetZoomIface(SPxCamZoomIface *zoomIface, int isPrimary = TRUE, int isSlave = FALSE);
    SPxErrorCode SetFocusIface(SPxCamFocusIface *focusIface, int isPrimary = TRUE, int isSlave = FALSE);
    SPxErrorCode SetPresetIface(SPxCamPresetIface *presetIface, int isPrimary = TRUE);
    SPxErrorCode SetMeasureRangeIface(SPxCamMeasureRangeIface *measureRangeIface);

    SPxErrorCode SetMaxZoomPosition(UINT16 pos, int isPrimary = TRUE);
    SPxErrorCode GetMaxZoomPosition(UINT16 *pos, int isPrimary = TRUE) const;
    SPxErrorCode SetZoomLUTFilename(const char *filename, int isPrimary = TRUE);
    SPxErrorCode GetZoomLUTFilename(char *buffer, int bufLen, int isPrimary = TRUE) const;
    SPxErrorCode SetFocusLUTFilename(const char *filename, int isPrimary = TRUE);
    SPxErrorCode GetFocusLUTFilename(char *buffer, int bufLen, int isPrimary = TRUE) const;

    SPxErrorCode SetVidSize(unsigned int width, unsigned int height, int isPrimary = TRUE);
    SPxErrorCode GetVidSize(unsigned int *width, unsigned int *height, int isPrimary = TRUE) const;

    virtual SPxErrorCode SetTrackDatabase(SPxTrackDatabase *trackDB);
    SPxTrackDatabase *GetTrackDatabase(void);

    SPxErrorCode SetLatLong(const SPxLatLong_tag *latLong);
    SPxErrorCode GetLatLong(SPxLatLong_tag *latLong) const;

    SPxErrorCode SetHeightMetres(double heightMetres);
    SPxErrorCode GetHeightMetres(double *heightMetres) const;

    virtual SPxErrorCode GetPrimarySensor(unsigned int * /*primarySensor*/) const { return SPX_ERR_NOT_SUPPORTED; }

protected:
    explicit SPxCamCtrlRecv(SPxNetTCP *tcp);

    /* Call these functions from derived class if required. */
    SPxErrorCode Pan(float panSpeed, float *panSpeedSet = NULL,
		     int continueZoomFocus = FALSE, double timeoutSecs = 0.0);
    SPxErrorCode Tilt(float tiltSpeed, float *tiltSpeedSet = NULL,
		     int continueZoomFocus = FALSE, double timeoutSecs = 0.0);
    SPxErrorCode PanTilt(float panSpeed, float tiltSpeed,
			 float *panSpeedSet = NULL,
			 float *tiltSpeedSet = NULL,
			 int continueZoomFocus = FALSE,
			 double timeoutSecs = 0.0);
    SPxErrorCode SetPan(float panAngleDegs);
    SPxErrorCode GetPan(float *panAngleDegs) const;
    SPxErrorCode SetTilt(float tiltAngleDegs);
    SPxErrorCode GetTilt(float *tiltAngleDegs) const;
    SPxErrorCode GetMaxPanSpeedDegsPerSec(double *maxPanSpeedDegsPerSec) const;
    SPxErrorCode GetMaxTiltSpeedDegsPerSec(double *maxTiltSpeedDegsPerSec) const;

    SPxErrorCode Zoom(float zoomSpeed, int continuePanTilt = FALSE,
		      double timeoutSecs = 0.0, int isPrimary = TRUE);
    SPxErrorCode SetZoomDegs(float fovDegs, int isPrimary = TRUE);
    SPxErrorCode GetZoomDegs(float *fovDegs, int isPrimary = TRUE) const;
    SPxErrorCode GetZoomRangeDegs(float *minFovDegs, float *maxFovDegs, int isPrimary = TRUE) const;
    SPxErrorCode SetZoomPos(UINT16 zoomPos, int isPrimary = TRUE);
    SPxErrorCode GetZoomPos(UINT16 *zoomPos, int isPrimary = TRUE) const;

    SPxErrorCode Focus(float focusSpeed, int continuePanTilt = FALSE, int isPrimary = TRUE);
    SPxErrorCode SetFocusMetres(float metres, int isPrimary = TRUE);
    SPxErrorCode GetFocusMetres(float *metres, int isPrimary = TRUE) const;
    SPxErrorCode SetFocusPos(UINT16 focusPos, int isPrimary = TRUE);
    SPxErrorCode GetFocusPos(UINT16 *focusPos, int isPrimary = TRUE) const;
    int IsAutoFocusSupported(int isPrimary = TRUE);
    SPxErrorCode AutoFocus(int autoFocus, int isPrimary = TRUE);
    int IsAutoFocusEnabled(int isPrimary = TRUE) const;

    SPxErrorCode GoToPreset(UINT8 preset, int isPrimary = TRUE);

    SPxErrorCode MeasureRange(int numMeasurements);
    SPxErrorCode GetMeasuredRange(double *rangesMetres,
				  unsigned int *numRanges) const;

private:
    struct impl;
    SPxAutoPtr<impl> m_p;

    float getFOVForZoom(UINT16 zoomPos, int isPrimary, int isSlave) const;
    UINT16 getZoomForFOV(float fovDegs, int isPrimary, int isSlave) const;

}; /* SPxCamCtrlRecv */

#endif /* _SPX_CAM_CTRL_RECV_H */

/*********************************************************************
*
*   End of File
*
**********************************************************************/

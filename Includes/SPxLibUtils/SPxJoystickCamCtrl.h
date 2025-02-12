/*********************************************************************
 *
 * (c) Copyright 2015 - 2020, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   Joystick class for controlling a camera.
 *
 * Revision Control:
 *  18/11/20 v1.7    AGC	Run joystick button actions on GUI thread.
 *
 * Previous Changes:
 *  26/10/20 1.6    AGC	Access camera through SPxCameraRelIface.
 *  02/07/18 1.5    AGC	Support named presets.
 *  11/09/17 1.4    SP 	Match base class change to SetOnScreenControl().
 *  29/08/17 1.3    AGC	Match base class change to SetOnScreenControl().
 *  19/10/16 1.2    AGC	Support presets.
 *			Support on-screen joystick interaction.
 *  29/06/15 1.1    AGC	Initial version.
 *
 **********************************************************************/
#ifndef SPX_JOYSTICK_CAM_CTRL_H_
#define SPX_JOYSTICK_CAM_CTRL_H_

/* Library headers. */
#include "SPxLibUtils/SPxAutoPtr.h"
#include "SPxLibUtils/SPxJoystickInterface.h"

/* Forward declarations. */
class SPxCameraRelIface;
#ifdef _WIN32
class SPxJoystickWin;
#endif
class SPxThread;

/* SPxJoystickInterface dialog */
class SPxJoystickCamCtrl : public SPxJoystickInterface
{
public:
    SPxJoystickCamCtrl(void);
    virtual ~SPxJoystickCamCtrl(void);

    SPxErrorCode SetCamera(SPxCameraRelIface *camera, SPxObj *obj=NULL,
			   int isPrimary=TRUE);

#ifdef _WIN32
    virtual SPxErrorCode SetOnScreenControl(SPxJoystickWin *ctrl,
					    int ctrlWhenVisible=TRUE,
					    int ctrlAxesEnables=TRUE);
#endif

    SPxErrorCode SetSavePresets(int savePresets);
    SPxErrorCode SetPreset(SPxJoystickInterface::Button_t button,
			   const char *preset);
    SPxErrorCode GetPreset(SPxJoystickInterface::Button_t button,
			   char *buf, unsigned int bufLen) const;
    SPxErrorCode ClearPreset(SPxJoystickInterface::Button_t button);
    SPxErrorCode SetPresetController(SPxJoystickInterface::Button_t button,
				     int controller);
    SPxErrorCode GetPresetController(SPxJoystickInterface::Button_t button,
				     int *controller) const;

    virtual int SetParameter(char *name, char *value);
    virtual int GetParameter(char *name, char *valueBuf, int bufLen);
    SPxErrorCode SetConfigFromState(void);

private:
    struct impl;
    SPxAutoPtr<impl> m_p;

    static void *threadFnWrapper(SPxThread *thread);
    void *threadFn(SPxThread *thread);
    void updateCamera(void);
    static void buttonReleasedWrapper(void *userArg, SPxJoystickInterface::Button_t button);
    SPxErrorCode buttonReleased(void);
    static int deleteCameraWrapper(void *invokingObject, void *userObject, void *arg);
    int deleteCamera(SPxObj *camera);
#ifdef _WIN32
    static void osBtnHandler(unsigned int index, void *userArg);
#endif
};

#endif /* SPX_JOYSTICK_CAM_CTRL_H_ */
/*********************************************************************
 *
 * End of file
 *
 **********************************************************************/

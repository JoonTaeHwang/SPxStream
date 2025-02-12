/*********************************************************************
 *
 * (c) Copyright 2015 - 2019, Cambridge Pixel Ltd.
 *
 *
 * Purpose:
 *   USB joystick interface.
 *
 * Revision Control:
 *  26/09/19 v1.12   AGC	Fix non-linearity types.
 *
 * Previous Changes:
 *  26/09/19 1.11   AGC	Add non-linearity options.
 *  12/07/19 1.10   BTB	Add handler for joystick selection. 
 *  03/08/18 1.9    BTB	Add handler for joystick movement.
 *  07/09/17 1.8    SP 	Optionally allow on-screen joystick axes 
 *			to be enabled/disabled.
 *  23/08/17 1.7    SP 	Optionally allow on-screen joystick to 
 *                      be controlled when hidden.
 *  19/10/16 1.6    AGC	SetOnScreenControl() now virtual.
 *  13/10/16 1.5    AGC	Support Windows joystick dialog interaction.
 *  01/10/15 1.4    AGC	Support choosing joystick device.
 *  31/07/15 1.3    AGC	Support axis inversion.
 *  24/07/15 1.2    AGC	Add support for dead zones.
 *  23/06/15 1.1    AGC	Initial version (based on CxEye implementation).
 *
 **********************************************************************/
#ifndef SPX_JOYSTICK_INTERFACE_H_
#define SPX_JOYSTICK_INTERFACE_H_

/*
 * Headers.
 */

/* Library headers. */
#include "SPxLibUtils/SPxObj.h"
#include "SPxLibUtils/SPxCriticalSection.h"

/*
 * Constants.
 */

/* Forward declarations. */
#ifdef _WIN32
class SPxJoystickWin;
#endif

/* SPxJoystickInterface dialog */
class SPxJoystickInterface : public SPxObj
{
public:

    /*
     * Public types.
     */

    /* Buttons. */
    typedef enum
    {
        BUTTON_0 = 0,
        BUTTON_1 = 1,
        BUTTON_2 = 2,
        BUTTON_3 = 3,
        BUTTON_4 = 4,
        BUTTON_5 = 5,
        BUTTON_6 = 6,
        BUTTON_7 = 7,
        BUTTON_8 = 8,
        BUTTON_9 = 9,
        MAX_NUM_BUTTONS

    } Button_t;

    /* Button handlers. */
    typedef void (*ButtonFn_t)(void *userArg,           /* User arg. */
                               Button_t button);	/* Button number. */

    typedef SPxErrorCode (*IterateFn_t)(void *userArg,		/* User arg. */
					const char *name,	/* Joystick name. */
					const void *dev);	/* Platform specific device info. */

    /* Handler for joystick movement. */
    typedef void (*MovementFn_t)(void *userArg);	/* User arg. */

    /* Handler for joystick selection. */
    typedef void (*SelectionFn_t)(void *userArg);	/* User arg. */

    /*
     * Public variables.
     */

    static const char AUTO[];

    /*
     * Public functions.
     */

    /* Construction and destruction. */
    SPxJoystickInterface(void);
    virtual ~SPxJoystickInterface(void);
#ifdef _WIN32
    SPxErrorCode Create(HWND hwnd, int selectFirst=TRUE);
#endif

    /* Hardware device control. */
    SPxErrorCode IterateDevices(IterateFn_t fn, void *userArg) const;
    SPxErrorCode SelectDevice(const char *name);
    SPxErrorCode GetDeviceName(char *buf, unsigned int bufLen) const;

    /* On-screen control. */
#ifdef _WIN32
    virtual SPxErrorCode SetOnScreenControl(SPxJoystickWin *ctrl, 
                                            int ctrlWhenVisible=TRUE,
                                            int ctrlAxesEnables=TRUE);
    virtual SPxJoystickWin *GetOnScreenControl(void);
#endif

    /* Configuration. */
    SPxErrorCode PollState(void);
    int IsSupported(void) const;
    virtual int IsXAxisSupported(void) const { return m_isXAxisSupported; }
    SPxErrorCode GetXAxis(double *val);
    virtual int IsYAxisSupported(void) const { return m_isYAxisSupported; }
    SPxErrorCode GetYAxis(double *val);
    virtual int IsZAxisSupported(void) const { return m_isZAxisSupported; }
    SPxErrorCode GetZAxis(double *val);
    int IsRotXAxisSupported(void) const { return m_isRotXAxisSupported; }
    SPxErrorCode GetRotXAxis(double *val);
    int IsRotYAxisSupported(void) const { return m_isRotYAxisSupported; }
    SPxErrorCode GetRotYAxis(double *val);
    int IsRotZAxisSupported(void) const { return m_isRotZAxisSupported; }
    SPxErrorCode GetRotZAxis(double *val);
    int IsButtonPressed(Button_t button);

    /* Dead zones. */
    void SetDeadZoneX(double percentage) { m_deadZoneX = percentage; }
    double GetDeadZoneX(void) const { return m_deadZoneX; }
    void SetDeadZoneY(double percentage) { m_deadZoneY = percentage; }
    double GetDeadZoneY(void) const { return m_deadZoneY; }
    void SetDeadZoneZ(double percentage) { m_deadZoneZ = percentage; }
    double GetDeadZoneZ(void) const { return m_deadZoneZ; }

    void SetInvertX(int invertX) { m_invertX = invertX; }
    int GetInvertX(void) const { return m_invertX; }
    void SetInvertY(int invertY) { m_invertY = invertY; }
    int GetInvertY(void) const { return m_invertY; }
    void SetInvertZ(int invertZ) { m_invertZ = invertZ; }
    int GetInvertZ(void) const { return m_invertZ; }

    void SetNonLinearityX(double nonLinearityX) { m_nonLinearityX = nonLinearityX; }
    double GetNonLinearityX(void) const { return m_nonLinearityX; }
    void SetNonLinearityY(double nonLinearityY) { m_nonLinearityY = nonLinearityY; }
    double GetNonLinearityY(void) const { return m_nonLinearityY; }
    void SetNonLinearityZ(double nonLinearityZ) { m_nonLinearityZ = nonLinearityZ; }
    double GetNonLinearityZ(void) const { return m_nonLinearityZ; }

    /* Button handlers. */
    SPxErrorCode SetButtonReleasedHandler(ButtonFn_t fn, void *userArg);

    /* Joystick movement handler. */
    SPxErrorCode SetJoystickMovementHandler(MovementFn_t fn, void *userArg);

    /* Joystick selection handler. */
    SPxErrorCode SetJoystickSelectionHandler(SelectionFn_t fn, void *userArg);

protected:

    /*
     * Protected variables.
     */

    /*
     * Protected functions.
     */

    /* Parameter settings. */
    virtual int SetParameter(char *name, char *value);
    virtual int GetParameter(char *name, char *valueBuf, int bufLen);

    void SetHandleMovement(int handleMovement) { m_handleMovement = handleMovement; }
    int GetHandleMovement(void) { return m_handleMovement; }

private:

    /*
     * Private types.
     */

    /*
     * Private variables.
     */

    struct impl;
    SPxAutoPtr<impl> m_p;

    mutable SPxCriticalSection m_mutex;

    /* Supported features. */
    int m_isXAxisSupported;
    int m_isYAxisSupported;
    int m_isZAxisSupported;
    int m_isRotXAxisSupported;
    int m_isRotYAxisSupported;
    int m_isRotZAxisSupported;

    /* Axis values. */
    double m_xAxis;
    double m_yAxis;
    double m_zAxis;
    double m_rotXAxis;
    double m_rotYAxis;
    double m_rotZAxis;

    /* Dead zones. */
    double m_deadZoneX;
    double m_deadZoneY;
    double m_deadZoneZ;

    /* Axis inversion. */
    int m_invertX;
    int m_invertY;
    int m_invertZ;

    /* Non-linearity. */
    double m_nonLinearityX;
    double m_nonLinearityY;
    double m_nonLinearityZ;

    /* Allow joystick movement handler to be called. */
    int m_handleMovement;

    /* Button state. */
    int m_isButtonPressed[MAX_NUM_BUTTONS];

    /* Button handler functions. */
    ButtonFn_t m_buttonReleasedFn;
    void *m_buttonReleasedFnArg;

    /* Iteration handler function. */
    mutable IterateFn_t m_iterateFn;
    mutable void *m_iterateUserArg;

    /* Joystick movement function. */
    MovementFn_t m_movementFn;
    void *m_movementUserArg;

    /* Joystick selection function. */
    SelectionFn_t m_selectionFn;
    void *m_selectionUserArg;

    /*
     * Private functions.
     */
    SPxErrorCode iterateDevices(IterateFn_t fn, void *userArg) const;
    SPxErrorCode selectDevice(const char *name);

#ifdef _WIN32
    void releaseJoystick(void);
    void releaseInput(void);

    /*
     * Private static functions.
     */
    static BOOL CALLBACK EnumJoysticksCallback(const void *pDevInst,
					       void *pContext);

    static BOOL CALLBACK EnumJoystickObjectsCallback(const void *pObjInst,
						     void *pContext);

    static SPxErrorCode selectReqDevice(void *userArg, const char *name, 
					const void *dev);
#endif

}; /* Class SPxJoystickInterface. */

#endif /* SPX_JOYSTICK_INTERFACE_H_ */
/*********************************************************************
 *
 * End of file
 *
 **********************************************************************/

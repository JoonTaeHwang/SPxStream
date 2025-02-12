/*********************************************************************
 *
 * (c) Copyright 2015, 2016, Cambridge Pixel Ltd.
 *
 * File: $RCSfile: SPxJoystickSim.h,v $
 * ID: $Id: SPxJoystickSim.h,v 1.4 2016/10/13 08:59:19 rew Exp $
 *
 * Purpose:
 *   Joystick class for controlling a Radar Simulator.
 *
 * Revision Control:
 *  13/10/16 v1.4    AGC	Overload IsZAxisSupported().
 *
 * Previous Changes:
 *  07/10/16 1.3    AGC	Support 2D/3D control.
 *  22/10/15 1.2    AGC	Use interface for object control.
 *			Support control of pitch.
 *  01/10/15 1.1    AGC	Initial version.
 *
 **********************************************************************/
#ifndef SPX_JOYSTICK_SIM_H_
#define SPX_JOYSTICK_SIM_H_

/* Library headers. */
#include "SPxLibUtils/SPxAutoPtr.h"
#include "SPxLibUtils/SPxJoystickInterface.h"

/* Forward declarations. */
struct SPxSimState;
class SPxRadarSimulator;
class SPxThread;

/* Interface for classes this class can control. */
class SPxJoystickSimIface
{
public:
    SPxJoystickSimIface(void) {}
    virtual ~SPxJoystickSimIface(void) {}

    virtual int IsSourceJoystick(void) const=0;
    virtual SPxErrorCode SetCurrentState(const SPxSimState *)=0;
    virtual SPxErrorCode GetCurrentState(SPxSimState *) const=0;
};

class SPxJoystickSim : public SPxJoystickInterface
{
public:
    enum AxisType
    {
	AXIS_2D = 0,
	AXIS_3D = 1
    };
    enum ControlType
    {
	CONTROL_SPEED = 0,
	CONTROL_ACCEL = 1
    };

    SPxJoystickSim(void);
    virtual ~SPxJoystickSim(void);

    SPxErrorCode AddObj(SPxJoystickSimIface *obj);
    SPxErrorCode RemoveObj(SPxJoystickSimIface *obj);

    SPxErrorCode SetAxisType(AxisType axisType);
    AxisType GetAxisType(void) const;
    SPxErrorCode SetControlType(ControlType controlType);
    ControlType GetControlType(void) const;
    SPxErrorCode SetMinSpeedMps(double minSpeedMps);
    double GetMinSpeedMps(void) const;
    SPxErrorCode SetMaxSpeedMps(double maxSpeedMps);
    double GetMaxSpeedMps(void) const;
    SPxErrorCode SetMinAccelLinearMps2(double minAccelLinearMps2);
    double GetMinAccelLinearMps2(void) const;
    SPxErrorCode SetMaxAccelLinearMps2(double maxAccelLinearMps2);
    double GetMaxAccelLinearMps2(void) const;
    SPxErrorCode SetMinAccelTurnDegsPerSec(double minAccelTurnDegsPerSec);
    double GetMinAccelTurnDegsPerSec(void) const;
    SPxErrorCode SetMaxAccelTurnDegsPerSec(double maxAccelTurnDegsPerSec);
    double GetMaxAccelTurnDegsPerSec(void) const;
    SPxErrorCode SetMinVrateMps(double minVrateMps);
    double GetMinVrateMps(void) const;
    SPxErrorCode SetMaxVrateMps(double maxVrateMps);
    double GetMaxVrateMps(void) const;

    double GetSpeedMps(void) const;
    double GetAccelLinearMps2(void) const;
    double GetAccelTurnDegsPerSec(void) const;
    double GetVrateMps(void) const;

    virtual int IsZAxisSupported(void) const;

    virtual int SetParameter(char *name, char *value);
    virtual int GetParameter(char *name, char *valueBuf, int bufLen);

private:
    struct impl;
    SPxAutoPtr<impl> m_p;

    static void *threadFnWrapper(SPxThread *thread);
    void *threadFn(SPxThread *thread);
    void updateObj(UINT32 msecs);
};

#endif /* SPX_JOYSTICK_SIM_H_ */
/*********************************************************************
 *
 * End of file
 *
 **********************************************************************/

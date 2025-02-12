/*********************************************************************
 *
 * (c) Copyright 2017 - 2021, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   Header for SPxAlarmAction class, which is used by the 
 *   SPxAlarmConfig to store individual actions.  Note that 
 *   processing of alarm actions is performed in SPxAlarm.
 *
 * Revision Control:
 *   28/04/21 v1.18  SP 	Add TYPE_LOG_TO_FILE alarm action.
 *
 * Previous Changes:
 *   23/04/21 1.17  SP 	Correct typo in TYPE_AUTO_ACKNOWLEDGE.
 *   21/04/21 1.16  SP 	Change timeouts from OPTION_TYPE_LIST to OPTION_TYPE_VALUE.
 *                     	Support option value units.
 *   08/04/21 1.15  SP 	Support unique ID.
 *   10/02/21 1.14  AGC	Support auto-reset, auto-delete and highlight shape.
 *   09/05/19 1.13  SP 	Support different option types.
 *   21/03/19 1.12  BTB	Add alarm once action.
 *   04/01/19 1.11  BTB	Support action values which are not pre-added.
 *   17/12/18 1.10  BTB	Add auto acknowledge and auto clear actions.
 *			Add concept of stop-on-clear actions.
 *   25/06/18 1.9   AGC	Suppress cppcheck warning.
 *   24/05/18 1.8   SP 	Move action processing to SPxAlarm.
 *   22/05/18 1.7   SP 	Support action options.
 *   18/05/18 1.6   SP 	Support more built-in actions.
 *   17/05/18 1.5   SP 	Add GetConfig().
 *   03/05/18 1.4   SP 	Pass CPA track to startStopActions().
 *   17/11/17 1.3   SP 	Support file load and save.
 *   13/11/17 1.2   SP 	Further development.
 *   08/11/17 1.1   SP 	Initial version.
 *
 *********************************************************************/

#ifndef _SPX_ALARM_ACTION_H
#define _SPX_ALARM_ACTION_H

/*
 * Other headers required.
 */

#include "SPxLibUtils/SPxAutoPtr.h"
#include "SPxLibUtils/SPxCriticalSection.h"
#include "SPxLibUtils/SPxObj.h"
#include "SPxLibUtils/SPxTime.h"

/*********************************************************************
 *
 *   Constants
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

/* Forward-declare other classes we use. */
class SPxAlarmDatabase;
class SPxAlarmConfig;
class SPxUniTrack;
class SPxAreaObj;

/*
 * Define our class.
 */
class SPxAlarmAction : public SPxObj
{
    /* These classes can access our internals directly. */
    friend class SPxAlarmConfig;

public:
    /*
     * Public types.
     */

    /* Action type. */
    typedef UINT32 Type_t;
    static const UINT32 TYPE_UNKNOWN            = 0;
    static const UINT32 TYPE_HIGHLIGHT_TRACK    = 1;   /* Start of built-in actions (1 to 99). */
    static const UINT32 TYPE_SOUND_ALARM        = 2;
    static const UINT32 TYPE_SEND_NET_MESG      = 3;
    static const UINT32 TYPE_AUTO_ACKNOWLEDGE   = 4;
    static const UINT32 TYPE_AUTO_CLEAR         = 5;
    static const UINT32 TYPE_SOUND_ALARM_ONCE   = 6;
    static const UINT32 TYPE_AUTO_RESET         = 7;
    static const UINT32 TYPE_AUTO_DELETE        = 8;
    static const UINT32 TYPE_HIGHLIGHT_SHAPES   = 9;
    static const UINT32 TYPE_LOG_TO_FILE        = 10;
    static const UINT32 TYPE_RESERVED_1         = 100; /* Was sound alarm. */
    static const UINT32 TYPE_RESERVED_2         = 101; /* Was send net mesg. */
    static const UINT32 TYPE_USER_START         = 102; /* Start of user-defined actions. */

    /* Change flags. */
    typedef UINT32 ChangeFlags_t;
    static const UINT32 CHANGE_STATE           = 0x00000001;
    static const UINT32 CHANGE_OPTION_VALUE    = 0x00000002;

    /* Option type. */
    typedef enum
    {
        OPTION_TYPE_NONE        = 0,    /* No options. */
        OPTION_TYPE_VALUE       = 1,    /* Single value. */
        OPTION_TYPE_LIST        = 2     /* List of names and values. */

    } OptionType_t;

    /* Option value type. */
    typedef enum
    {
        OPTION_VALUE_TYPE_NONE      = 0,    /* Not set or generic value. */
        OPTION_VALUE_TYPE_TIME_SECS = 1     /* Time value in seconds. */

    } OptionValueType_t;

    /*
     * Public variables.
     */

    /*
     * Public functions.
     */

    /* Note that the constructor and destructor are not public, so the
     * class can only be instantiated by the SPxAlarmConfig.
     */

    virtual UINT32 GetID(void) { return m_id; }

    virtual SPxTime_t GetResetTime(void) const { return m_resetTime; }

    virtual UINT32 GetType(void) const { return m_type; }
    virtual const char *GetTypeString(void);

    virtual SPxAlarmConfig *GetConfig(void) { return m_config; }

    virtual SPxErrorCode SetEnabled(int isEnabled);
    virtual int IsEnabled(void) const{ return m_isEnabled; }

    virtual int IsStopOnClearAction(void) const;
    virtual int IsStopOnEmptyAction(void) const;

    virtual SPxErrorCode SetOptionType(OptionType_t optType, 
                                       OptionValueType_t valType=OPTION_VALUE_TYPE_NONE,
                                       UINT32 valMin=0,
                                       UINT32 valMax=0);
    virtual OptionType_t GetOptionType(void) { return m_optionType; }
    virtual OptionValueType_t GetOptionValueType(void) { return m_optionValueType; }
    virtual SPxErrorCode AddOption(const char *name, UINT32 value);

    virtual unsigned int GetNumOptions(void);
    virtual const char *GetOptionName(void);
    virtual SPxErrorCode SetOptionValue(UINT32 value);
    virtual UINT32 GetOptionValue(void) { return m_optionValue; }
    virtual const char *GetOptionNameFromValue(UINT32 value);
    virtual const char *GetOptionNameFromIndex(unsigned int index);
    virtual UINT32 GetOptionValueFromIndex(unsigned int index);

    /*
     * Public static functions.
     */

    static int IsTypeBuiltIn(Type_t type);
    static int IsTypeUserDefined(Type_t type);
    static int IsTypeValid(Type_t type);
    static const char *GetTypeString(Type_t type);

protected:

    /*
     * Protected variables.
     */

    /*
     * Protected functions.
     */

    /* Protected functions may be accessed by the SPxAlarmConfig. */

    /* Connameuctor and denameuctor. */
    SPxAlarmAction(SPxAlarmConfig *config);
    virtual ~SPxAlarmAction(void);

    /* General. */
    // cppcheck-suppress virtualCallInConstructor
    virtual SPxErrorCode reset(void);
    virtual SPxErrorCode initialise(Type_t type);

    /* Config load/save. */
    virtual SPxErrorCode load(FILE *fPtr);
    virtual SPxErrorCode save(FILE *fPtr);

private:

    /*
     * Private types.
     */

    /*
     * Private variables.
     */

    /* Private member variables. */
    struct impl;
    SPxAutoPtr<impl> m_p;
    mutable SPxCriticalSection m_mutex;     /* Mutex. */
    UINT32 m_id;                            /* Action ID. */
    SPxAlarmDatabase *m_alarmDB;            /* Alarm database. */
    SPxAlarmConfig *m_config;               /* Alarm config . */
    SPxTime_t m_resetTime;                  /* Last reset time. */
    Type_t m_type;                          /* Action type. */
    OptionType_t m_optionType;              /* Option type. */
    OptionValueType_t m_optionValueType;    /* Option value type. */
    UINT32 m_optionValueMin;                /* Option value min limit. */
    UINT32 m_optionValueMax;                /* Option value max limit. */
    UINT32 m_optionValue;                   /* Selected option value. */
    int m_isEnabled;                        /* Is action enabled? */

    /*
     * Private functions.
     */

    /* General. */
    SPxErrorCode handleChange(ChangeFlags_t flags);
    SPxErrorCode reportChangeEvent(ChangeFlags_t flags);
    SPxErrorCode setOptionType(OptionType_t optType, 
                               OptionValueType_t valType=OPTION_VALUE_TYPE_NONE,
                               UINT32 valMin=0,
                               UINT32 valMax=0);
    SPxErrorCode addOption(const char *name, UINT32 value);
    SPxErrorCode setOptionValue(UINT32 value);

    /*
     * Private static functions.
     */

}; /* SPxAlarmAction */

#endif /* SPX_ALARM_ACTION_H */

/*********************************************************************
 *
 * End of file
 *
 *********************************************************************/

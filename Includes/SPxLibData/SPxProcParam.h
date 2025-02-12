/*********************************************************************
*
* (c) Copyright 2007 - 2022, Cambridge Pixel Ltd.
*
* Purpose:
*   Header for Process Parameters classes.
*
* Revision Control:
*   02/09/22 v1.29   AGC	Delete SPxProcessParameter copy constructor.
*
* Previous Changes:
*   19/06/20 1.28   AJH	Add SPX_PROC_PARAM_CTRL_TYPE_PASSWORD.
*   06/04/20 1.27   AGC	Suppress spurious cppcheck warning.
*   03/04/20 1.26   AGC Mark setters/getters as virtual.
*   22/06/18 1.25   AGC	Fix new cppcheck warnings.
*   27/03/18 1.24   SP 	Use strings for menu entry values.
*   05/05/16 1.23   AJH	Add long description to radio buttons.
*   14/12/15 1.22   AGC	Increase short description length.
*   04/09/15 1.21   AGC	Increase max menu/radio counts to 16.
*   17/03/15 1.20   SP 	Add mutex protection to run parameter set/get.
*   06/06/14 1.19   AGC	Prevent undefined behaviour in double to int cast.
*   01/05/14 1.18   AGC	Fix warnings in AV library.
*   25/04/14 1.17   REW	Add Region functions to SPxProcParamIface.
*   24/04/14 1.16   REW	Add IsAreaDependent() support.
*   26/03/14 1.15   AGC	Add SPxProcParamIface.
*			Add support for push buttons.
*			Add support for labels.
*   17/02/14 1.14   AGC	Use const for name in constructors.
*   15/06/11 1.13   REW	Add SetCtrlHidden() etc.
*   03/12/10 1.12   SP 	Support extra radio and menu args.
*   26/11/10 1.11   SP 	Support parameter exclusion.
*   26/11/10 1.10   REW	Add tag SPxProcParamCtrlInfo_t.
*   10/11/10 1.9    SP 	Changes to GUI control info functions.
*   08/11/10 1.8    SP 	Add GUI control info functions.
*   15/09/10 1.7    REW	Make destructor virtual.
*   21/10/08 1.6    DGJ	Add destructors.
*   01/07/08 1.5    REW	Add IsStatus() support.
*   21/01/08 1.4    REW	Handle string memory ourselves.
*   17/01/08 1.3    REW	Avoid icc compiler warnings.
*   11/12/07 1.2    DGJ	Support string parameter type for Process
*   12/06/07 1.1    REW	Initial Version, breakout from SPxProcMan.h.
**********************************************************************/

#ifndef _SPX_PROCPARAM_H
#define _SPX_PROCPARAM_H

/*
 * Other headers required.
 */

#include "SPxLibUtils/SPxCommon.h" /* Needed for UINT32, etc */
#include "SPxLibUtils/SPxError.h"  /* Needed for SPxErrorCode, etc */
#include "SPxLibUtils/SPxCriticalSection.h"

/*********************************************************************
*
*   Constants
*
**********************************************************************/


/*********************************************************************
*
*   Macros
*
**********************************************************************/


/*********************************************************************
*
*   Type definitions
*
**********************************************************************/

/* Early declaration of class as used in structures below */
class SPxProcessParameter;
class SPxProcessRunParameter;

/*
 * Types of parameters for a process construction.
 */
typedef enum {
    SPX_PROC_PARAM_TYPE_INT,
    SPX_PROC_PARAM_TYPE_REAL,
    SPX_PROC_PARAM_TYPE_STRING
} SPxProcParamType;

/*
 * Information used to display a parameter on a GUI.
 *
 * NOTE that these definitions are also used by the SPxWizard classes
 * so please bear that in mind if changing any of the following.
 */

/* Display control type */
typedef enum 
{
    SPX_PROC_PARAM_CTRL_TYPE_LABEL,  /* Label (for status) */
    SPX_PROC_PARAM_CTRL_TYPE_ENTRY,  /* Text entry */
    SPX_PROC_PARAM_CTRL_TYPE_SPIN,   /* Spin button */
    SPX_PROC_PARAM_CTRL_TYPE_CHECK,  /* Check / toggle button */
    SPX_PROC_PARAM_CTRL_TYPE_BUTTON, /* Push button */
    SPX_PROC_PARAM_CTRL_TYPE_RADIO,  /* Radio button */
    SPX_PROC_PARAM_CTRL_TYPE_MENU,   /* Drop-down menu */
    SPX_PROC_PARAM_CTRL_TYPE_SLIDER, /* Slider */
    SPX_PROC_PARAM_CTRL_TYPE_HIDDEN,	/* Excluded from display */
    SPX_PROC_PARAM_CTRL_TYPE_PASSWORD	/* Encrypted text entry */

} SPxProcParamCtrlType_t;

/* Text entry configuration data */
typedef struct 
{
    /* Nothing at present */

} SPxProcParamCtrlLabel_t;

typedef struct 
{
    /* Nothing at present */

} SPxProcParamCtrlEntry_t;

/* Spin button configuration data */
typedef struct 
{
    double minVal;      /* Minimum value */
    double maxVal;      /* Maximum value */
    int numDigits;      /* Number of decimal places */

} SPxProcParamCtrlSpin_t;

/* Check button configuration data */
typedef struct 
{
    char label[128];    /* Button label */

} SPxProcParamCtrlCheck_t;

/* Push button configuration data */
typedef struct 
{
    char label[128];    /* Button label */

} SPxProcParamCtrlButton_t;

/* Radio button configuration data */
#define SPX_PROC_PARAM_CTRL_RADIO_MAX_BTNS 16
typedef struct 
{
    char labels[SPX_PROC_PARAM_CTRL_RADIO_MAX_BTNS][128]; /* Button labels */
    int values[SPX_PROC_PARAM_CTRL_RADIO_MAX_BTNS];       /* Button values */
    char longDesc[SPX_PROC_PARAM_CTRL_RADIO_MAX_BTNS][128];

} SPxProcParamCtrlRadio_t;

/* Menu configuration data */
#define SPX_PROC_PARAM_CTRL_MENU_MAX_ENTRIES 16
typedef struct 
{
    char labels[SPX_PROC_PARAM_CTRL_MENU_MAX_ENTRIES][128]; /* Entry labels */
    char values[SPX_PROC_PARAM_CTRL_MENU_MAX_ENTRIES][128]; /* Entry values */

} SPxProcParamCtrlMenu_t;

/* Slider configuration data */
typedef struct {
    double minVal;      /* Minimum value */
    double maxVal;      /* Maximum value */
    double stepVal;     /* Step size */
    int showValue;      /* Show value next to the slider? */
    int numDigits;      /* Number of decimal places to display */

} SPxProcParamCtrlSlider_t;

/* A union of all control types */
typedef union 
{
    SPxProcParamCtrlLabel_t label;   /* Label */
    SPxProcParamCtrlEntry_t entry;   /* Text entry */
    SPxProcParamCtrlSpin_t spin;     /* Spin button */
    SPxProcParamCtrlCheck_t check;   /* Check button */
    SPxProcParamCtrlButton_t button; /* Push button */
    SPxProcParamCtrlRadio_t radio;   /* Radio buttons */
    SPxProcParamCtrlMenu_t menu;     /* Drop-down menu */
    SPxProcParamCtrlSlider_t slider; /* Slider */

} SPxProcParamCtrl_t;

/* Top-level control information */
typedef struct SPxProcParamCtrlInfo_tag
{
    SPxProcParamCtrlType_t type;    /* Control type */
    SPxProcParamCtrl_t ctrl;        /* Control specific information */
    char shortDesc[64];             /* Short description (title) */
    char longDesc[128];             /* Long description (help text) */
    int hasEnableRadio;             /* Control has an enable radio button? */
    SPxProcessParameter *paramGroup;/* Parameter group for this control */

} SPxProcParamCtrlInfo_t;

/* Interface required for object to be controllable. */
class SPxProcParamIface
{
public:
    SPxProcParamIface(void) {}
    virtual ~SPxProcParamIface(void) {}
    virtual void SetParamValueI(const char *name, int value)=0;
    virtual void SetParamValueR(const char *name, double value)=0;
    virtual void SetParamValueS(const char *name, const char *value)=0;
    virtual SPxProcessRunParameter *GetFirstRunParameter(void)=0;
    virtual char *GetProcessName(void)=0;
    virtual char *GetName(void)=0;
    virtual int SetRegionName(unsigned int /*idx*/, const char * /*name*/)
					{ return(SPX_ERR_NOT_SUPPORTED); }
    virtual int SetRegionValue(unsigned int /*idx*/, const char * /*value*/)
					{ return(SPX_ERR_NOT_SUPPORTED); }
    virtual const char *GetRegionName(unsigned int /*idx*/) { return(NULL); }
    virtual const char *GetRegionValue(unsigned int /*idx*/) { return(NULL); }
};

/*
 * Class that holds a general description of a process parameter, but not a
 * specific instance of one of those parameters (see SPxProcessParameter
 * instead).
 */
class SPxProcessParameter
{
public:
    /* Public fields. */

    /* Static public functions */
    static SPxErrorCode LoadExcludeFile(const char *filename, int isSilent=0);
    static void SPxProcessParameterShutdown(void);

    /*
     * Public functions.
     */
    /* Constructors, depending on type. */
    SPxProcessParameter(const char *name, SPxProcParamType type, int defaultVal,
					int isStatus);
    SPxProcessParameter(const char *name, SPxProcParamType type, double defaultVal,
					int isStatus);
    SPxProcessParameter(const char *name, SPxProcParamType type, const char *defVal,
					int isStatus);
    virtual ~SPxProcessParameter(void);

    /* Information retrieval. */
    char *GetName(void)				{ return m_name; }
    SPxProcParamType GetType(void)		{ return m_type; }
    int GetDefaultI(void)			{ return m_defaultI; }
    double GetDefaultR(void)			{ return m_defaultR; }
    const char *GetDefaultS(void)		{ return m_defaultS; }
    int IsStatus(void)				{ return m_isStatus; }
    int IsAreaDependent(void)			{ return m_isAreaDependent; }
    int IsBold(void)				{ return m_isBold; }
    int IsExcluded(const char *objName);

    /* Linked list handling. */
    SPxProcessParameter *GetNext(void)		{ return m_next; }
    void SetNext(SPxProcessParameter *next)	{ m_next = next; }

    /* Read-only access to GUI control info. */
    const SPxProcParamCtrlInfo_t *GetCtrlInfo(void) { return &m_ctrlInfo; }

    /* Functions used to set GUI control info */
    void SetCtrlDesc(const char *longDesc, 
                     const char *shortDesc=0);

    void SetCtrlBold(void);

    void SetCtrlHidden(void);

    void SetCtrlLabel(void);

    void SetCtrlAreaDependent(int areaDependent)
    {
	m_isAreaDependent = areaDependent ? TRUE : FALSE;
    }

    void SetCtrlEntry(void);

    void SetCtrlSpin(double minVal=-1E9, 
                     double maxVal=1E9, 
                     int numDigits=3);

    void SetCtrlSpinEnableRadio(int hasEnableRadio,
                                SPxProcessParameter *group=0);

    void SetCtrlCheck(const char *label="Enable");

    void SetCtrlButton(const char *label);

    void SetCtrlRadio(const char *names[], 
                      const int values[],
                      unsigned int numButtons);

    void SetCtrlRadio(const char *name0, int val0,
                      const char *name1, int val1,
                      const char *name2=0, int val2=0,
                      const char *name3=0, int val3=0,
                      const char *name4=0, int val4=0,
                      const char *name5=0, int val5=0,
                      const char *name6=0, int val6=0,
                      const char *name7=0, int val7=0);

    void SetCtrlMenu(const char *names[], 
                     const int values[],
                     unsigned int numEntries);

    void SetCtrlMenu(const char *name0, int val0,
                     const char *name1, int val1,
                     const char *name2=0, int val2=0,
                     const char *name3=0, int val3=0,
                     const char *name4=0, int val4=0,
                     const char *name5=0, int val5=0,
                     const char *name6=0, int val6=0,
                     const char *name7=0, int val7=0);

    void SetCtrlSlider(double minVal, 
                       double maxVal,
                       double stepVal=0.1);

    void SetCtrlSliderEnableRadio(int hasEnableRadio,
                                  SPxProcessParameter *group=0);

    void SetCtrlSliderShowValue(int showValue, int numDigits=2);

private:
    /*
     * Private fields.
     */
    char *m_name;

    /* Type and default values. */
    SPxProcParamType m_type;
    int m_defaultI;
    double m_defaultR;
    char  *m_defaultS;

    /* Flag to say if this parameter is just for status-only, not usually
     * set by users, just the process itself.
     */
    int m_isStatus;

    /* Flag to say the parameter may be area-dependent. */
    int m_isAreaDependent;

    /* Flag to say if the parameter should be displayed with bold text. */
    int m_isBold;

    /* GUI control information */
    SPxProcParamCtrlInfo_t m_ctrlInfo;

    /* Linked list information. */
    SPxProcessParameter *m_next;

    /*
     * Private functions.
     */
    void SetCtrlDefaults(void);

    /* Prevent use of copy-constructor and assignment operator on SPxProcessParameter. */
    SPxProcessParameter(const SPxProcessParameter&) SPX_FUNCTION_DELETE;
    SPxProcessParameter& operator=(const SPxProcessParameter&) SPX_FUNCTION_DELETE;

}; /* SPxProcessParameter */

/*
 * A class that represents the invocation of a parameter for a particular
 * running-process.
 */
class SPxProcessRunParameter
{
public:
    /*
     * Public fields.
     */

    /*
     * Public functions.
     */
    /* Constructor and Destructor */
    SPxProcessRunParameter(SPxProcessParameter *parameter);
    virtual ~SPxProcessRunParameter(void);

    /* Value get/set functions. */
    virtual int GetValueI(void);
    virtual void SetValueI(int val);
    virtual double GetValueR(void);
    virtual void SetValueR(double val);
    virtual const char *GetValueS(void);
    // cppcheck-suppress virtualCallInConstructor
    virtual void SetValueS(const char *newVal);

    /* Access to underlying objects. */
    SPxProcessParameter *GetParameter(void)	{ return m_parameter;}

    /* Linked list handling. */
    SPxProcessRunParameter *GetNext(void)	{ return m_next; }
    void SetNext(SPxProcessRunParameter *next)	{ m_next = next; }

private:
    /*
     * Private fields.
     */
    /* Pointer to underlying parameter object. */
    SPxProcessParameter *m_parameter;

    /* Current value of this parameter. */
    int m_valueI;
    double m_valueR;
    char *m_valueS;	/* This is allocated on the heap on an assignment. */

    /* Linked list information. */
    SPxProcessRunParameter *m_next;

    /* Parameter mutex. */
    SPxCriticalSection m_mutex;

    /*
     * Private functions.
     */
}; /* SPxProcessRunParameter */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* _SPX_PROCPARAM_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/

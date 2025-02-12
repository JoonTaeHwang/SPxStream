/*********************************************************************
*
* (c) Copyright 2007 - 2022, Cambridge Pixel Ltd.
*
* Purpose:
*   Define SPxObj as the root SPx Object
*
* Revision Control:
*   25/07/22 v1.65   AGC	Pass client ID to SPxObj::Set/Get().
*
* Previous Changes:
*   29/03/22 1.64   AGC	Pass connection ID to SPxObj::Set/Get().
*   14/09/21 1.63   AGC	Add CopyParameterListsTo() function.
*   14/09/21 1.62   AGC	Support silencing parameter errors.
*   28/06/21 1.61   AGC	Support passing from address to SPxObj::Set/Get().
*   18/06/21 1.60   AGC	Support multiple message reporters.
*   06/04/21 1.59   SP 	Support parameter set with optional reply.
*   22/03/21 1.58   AGC	Allow SetParamSocket() override.
*   16/03/21 1.57   AGC	Support passing socket to SPxObj::Set/Get().
*   02/03/21 1.56   JGB	Add GetObjectList()
*   29/04/20 1.55   AGC	Add IsInRead/WriteParameterList().
*   19/08/19 1.54   SP 	Add Set/IsStandby().
*   07/11/18 1.53   BTB	Add SetIsMaster() and GetIsMaster().
*   26/10/18 1.52   BTB	Add SetUseSharedDirectory() and UsingSharedDirectory().
*   19/10/18 1.51   AJH	Remove redundant argument from SetParameterLists().
*   18/10/18 1.50   AJH	Add RemoveEntryFromParameterLists().
*   16/07/18 1.49   AGC	Make ApplyParamToObj/ObjToParam() protected.
*   30/04/18 1.48   AGC	Add PrepareForRestart().
*   16/03/18 1.47   AGC	Remove max deletion callbacks variable.
*   06/10/17 1.46   SP 	Add IsClassName().
*   26/09/17 1.45   SP 	Add AppendToParameterLists().
*                       Add IterateParameterLists().
*   26/06/17 1.44   SP 	Make SetSaveUpdateOnly() virtual.
*   09/05/17 1.43   AGC	Fix cppcheck warning.
*   07/11/16 1.42   AGC	Always use STL container for parameters.
*   16/09/16 1.41   AGC	Improve deletion callback mutex protection.
*   22/01/16 1.40   AGC	Add silent flag to AddDeletionCallback().
*   07/01/16 1.39   AGC	Add RemoveAllParameterLists().
*   09/11/15 1.38   AGC	Support override of update only save option.
*   15/07/15 1.37   AGC	Support update only on save option.
*   16/02/15 1.36   AGC	Add overloads of Add/RemoveDeletionCallback taking
*			SPxCallbackListFn_t.
*   07/01/15 1.35   AGC	Make GetId() const.
*   07/11/14 1.34   AGC	Generate error if deletion callback added after callbacks invoked.
*   24/09/14 1.33   SP 	Add Lock/UnlockObjList().
*   10/04/14 1.32   AGC	Make Get() functions take const command.
*   17/02/14 1.31   AGC	Add overloads of Set/GetParameter taking const
*			parameters to avoid warnings from callers.
*   18/11/13 1.30   REW	Rename GetData() to GetBinaryData().
*   18/11/13 1.29   AGC	Support non-static lists pasts to SetParameterLists().
*   15/11/13 1.28   SP 	Add GetData().
*   04/10/13 1.27   AGC	Simplify headers.
*   21/12/12 1.26   AGC	Support alerts.
*   31/08/12 1.25   AGC	Make Set() parameter const.
*   21/08/12 1.24   AGC	Use private structure for read/write lists.
*			Do not specify a body for copy-ctor/assignment operator.
*   29/05/12 1.23   AGC	Add GetNextReadParam() and GetNextWriteParam().
*			Make IsNameSet() const.
*   17/11/11 1.22   AGC	Add private copy-constructor and assignment operator.
*   07/10/11 1.21   AGC	Add GetConstClassName() function.
*   31/08/11 1.20   AGC	Add GetConstName() function.
*			Create objListMutex and deletionCallbackMutex statically.
*   25/05/11 1.19   AGC	Add RemoveFromParameterLists().
*			Avoid duplicates in parameter lists.
*   11/05/11 1.18   AGC	Make GetActive() const.
*			SetClassName() now takes const parameter.
*   28/10/10 1.17   AGC	Support setting objects to not be saved.
*			Support appending to parameter lists.
*			Support setting state or config for a subset
*			of all objects.
*   04/10/10 1.16   AGC	Implement SetConfigFromState() functions.
*   18/06/10 1.15   SP 	Add m_systemObj flag.
*			Add SetSystemObj() and IsSystemObj().
*   27/05/10 1.14   SP 	Add SetStateFromConfig(),
*			SetConfigFromState(), SetStateFromConfigAll()
*			and SetConfigFromStateAll()
*   26/04/10 1.13   REW	Support m_maxDeletionCallbacks.
*   23/04/10 1.12   REW	Fix #085 with separate mutex for callbacks.
*   15/04/10 1.11   DGJ	Support magic number at object start.
*   30/09/09 1.10   REW	Add IsNameSet().
*   29/09/09 1.9    REW	Support deletion callbacks and global mutex.
*   13/11/08 1.8    REW	Add string version of Get() and GetParameter().
*   22/10/08 1.7    DGJ	Add SPxDeleteAll(), SPxDeleteAllSystem()
*   25/10/07 1.6    REW	SetName() takes a const char * now.
*   17/09/07 1.5    DGJ	Set/Get Active added
*   07/09/07 1.4    REW	Add Get() and GetParameter().
*   03/09/07 1.3    REW	Support message reporters.
*   24/08/07 1.2    DGJ	Renamed GetNext() to GetNextObject()
*   29/06/07 1.1    DGJ	Initial Version
**********************************************************************/

#ifndef _SPX_OBJ_H
#define _SPX_OBJ_H

/* Standard headers */
#include <stdio.h>

/* Include SPxCommon.h for SOCKET_TYPE */
#include "SPxCommon.h"

/* Include for SPxErrorCode */
#include "SPxError.h"

/* Include for SPxAutoPtr. */
#include "SPxAutoPtr.h"

/* Include for SPxAtomic. */
#include "SPxAtomic.h"

/*********************************************************************
*
*   Constants
*
**********************************************************************/

/* Magic number to allow us to sanity-check the base class objects. */
#define SPX_OBJ_MAGIC	0x5AF0A40F


/*********************************************************************
*
*   Type definitions
*
**********************************************************************/

/* Forward-declare the classes we need. */
class SPxMessageReporter;
class SPxAlertReporter;
class SPxCallbackList;

/* Define the type of the callback function. */
typedef int (*SPxCallbackListFn_t)(void *invokingObject,
					void *userObject,
					void *arg);

/* Use in IterateParameterLists(). */
typedef SPxErrorCode (*SPxParamFn_t)(void *invokingObject,
                                     void *userArg,
                                     const char *paramName);

/*
 * Base class of all other SPx classes.
 */
class SPxObj
{
public:
    /*
     * Public functions.
     */
    /* Constructor/destructor. */
    SPxObj(void);
    virtual ~SPxObj(void);

    /* Class-name functions.  The Set() is protected. */
    char *GetClassName(void);		/* Get the class name. */
    const char *GetConstClassName(void) const;
    int IsClassName(const char *className) const;

    /* Determine if this is system-created (TRUE) or user-created. 
     * The associated Set() is protected.
     */
    int IsSystemObj(void) { return m_systemObj; }

    /* Verify that the magic number is correct */
    int Verify(FILE *fileLog=0);

    /* Object-name functions. */
    char *SetName(const char *newName);	/* Set the instance name. */
    char *GetName(void);		/* Return the name, or a default. */
    const char *GetConstName(void) const; /* Return the name which could be NULL. */
    int IsNameSet(void) const { return(m_nameSet); }

    /* Shutdown object ready for application restart. */
    virtual SPxErrorCode PrepareForRestart(void) { return SPX_NO_ERROR; }

    /* Save inclusion/exclusion. Normally all named objects are saved.
     * Use SetSave(FALSE) to prevent a named object from saving its state,
     * when SetConfigFromState() or SetConfigFromStateAll() is called.
     */
    void SetSave(int save) { m_save = save; }
    int IsSave(void) { return m_save; }

    virtual void SetSaveUpdateOnly(int updateOnly) { m_saveUpdateOnly = updateOnly; }
    int IsSaveUpdateOnly(void) const { return m_saveUpdateOnly && !m_overrideSaveUpdateOnly; }

    static void SetOverrideSaveUpdateOnly(int overrideSaveUpdateOnly)
    { m_overrideSaveUpdateOnly = overrideSaveUpdateOnly; }

    virtual void SetUseSharedDirectory(int useSharedDirectory) { m_useSharedDirectory = useSharedDirectory; }
    int UsingSharedDirectory(void) { return m_useSharedDirectory; }

    virtual void SetIsMaster(int isMaster) { m_isMaster = isMaster; }
    int GetIsMaster(void) { return m_isMaster; }

    /* Parameter setting/getting. */
    int SetParameter(const char *parameterName, const char *parameterValue);
    int SetParameter(const char *parameterName, char *parameterValue);
    virtual int SetParameter(char *parameterName, char *parameterValue);

    /* Parameter getting.  If you only override one of these, make
     * sure it is the variant that returns the value in a string, since the
     * base class integer variant invokes the other one if not implemented.
     */
    virtual int GetParameter(char *parameterName, int *valuePtr);
    int GetParameter(const char *parameterName, char *valueBuf, int bufLen);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);

    /* Binary data getting. */
    virtual int GetBinaryData(char *command, char *dataBuf, 
                        unsigned int dataBufLen,
                        unsigned int *dataLen);

    /* Get read/write parameter lists. */
    const char *GetNextReadParam(const char *prevParam) const;
    const char *GetNextWriteParam(const char *prevParam) const;

    /* Used to initialise an object from the configuration parameters
     * or update the configuration parameters based on the state of 
     * an object.
     */
    virtual SPxErrorCode SetStateFromConfig(void);
    virtual SPxErrorCode SetConfigFromState(void);

    /* ID retrieval. */
    int GetId(void) const;

    /* Registration of functions to be invoked on certain events.
     * NB: The "fn" arguments are really of type "SPxCallbackListFn_t".
     *
     * As of 16/02/15, overloads with the correct function type are
     * now included alongside those taking void*.
     */
    int AddDeletionCallback(SPxCallbackListFn_t fn, void *userPtr, int silent=FALSE);
    int RemoveDeletionCallback(SPxCallbackListFn_t fn,void *userPtr);
    int AddDeletionCallback(void *fn, void *userPtr, int silent=FALSE)
    {
	return AddDeletionCallback((SPxCallbackListFn_t)fn, userPtr, silent);
    }
    int RemoveDeletionCallback(void *fn,void *userPtr)
    {
	return RemoveDeletionCallback((SPxCallbackListFn_t)fn, userPtr);
    }

    /* Linked list manipulation. */
    SPxObj *GetNextObject(void) {return m_next;}	/* Next in chain */

    /*
     * Public variables.
     */

    /*
     * Static functions for class.
     */
    /* Initialisation function, called by SPxLibUtilsInit() */
    static int SPxObjInit(void);
    static int SPxObjShutdown(void);

    /* Parameter setting/getting. */
    static int Set(const char *command, char *valueBuf=NULL, int valueBufLen=0,
		   SOCKET_TYPE sock=SPX_SOCK_INVALID, UINT32 fromAddr=0,
		   UINT32 id=0, const char *clientID=NULL);
    static int Get(const char *command, int *valuePtr);
    static int Get(const char *command, char *valueBuf, int valueBufLen,
		   SOCKET_TYPE sock=SPX_SOCK_INVALID, UINT32 fromAddr=0,
		   UINT32 id=0, const char *clientID = NULL);
    static int Get(const char *command, char *dataBuf, 
                   unsigned int dataBufLen, unsigned int *dataLen);

    /* Get Object List */
    static int GetObjectList(char* valueBuf, int valueBufLen);

    /* Message reporting. */
    static void Report(const char *msg);

    /* Alert reporting. */
    static void ReportAlert(UINT32 packetType, void *data, unsigned int numBytes);

    /* Object searching/retrieval. */
    static int GetNumInstances(void);			/* Number in list */
    static SPxObj *GetFirstObject(void);		/* First in list */
    static SPxObj *FindObjectByName(const char *name);	/* Get by name */
    static SPxObj *FindObjectByID(int id);		/* Get by ID */

    /* Read and write configuration settings to/from all objects */
    static SPxErrorCode SetStateFromConfigAll(const char *incObjList[]=NULL, 
					      const char *excObjList[]=NULL);
    static SPxErrorCode SetConfigFromStateAll(const char *incObjList[]=NULL,
					      const char *excObjList[]=NULL);

    /* Message reporting installation. */
    static int InstallReporter(SPxMessageReporter *msgReporter);
    static int RemoveReporter(SPxMessageReporter *msgReporter);

    /* Alert reporting installation. */
    static int InstallAlerter(SPxAlertReporter *alertReporter);
    static int RemoveAlerter(SPxAlertReporter *alertReporter);

    /* Activity state */
    virtual void SetActive(int active) {m_active = active;}
    virtual int GetActive(void) const {return m_active;}
    virtual SPxErrorCode SetStandby(int standby) {m_standby = standby; return SPX_NO_ERROR;}
    virtual int IsStandby(void) const {return m_standby;}

    /* Delete all SPx objects of a particular type */
    static void DeleteAllObjectsOfType(const char *className, 
				       int systemObj=FALSE);

    /* Lock list during access. */
    static SPxErrorCode LockObjList(void);
    static SPxErrorCode UnlockObjList(void);

    /* Assign/query socket being used for parameter change. */
    virtual void SetParamSocket(SOCKET_TYPE sock, UINT32 id=0, const char *clientID=NULL)
    { m_paramSock = sock; m_paramID = id; m_paramClientID = clientID; }
    SOCKET_TYPE GetParamSocket(void) const { return m_paramSock; }
    virtual void SetParamFromAddr(UINT32 fromAddr) { m_paramFromAddr = fromAddr; }
    UINT32 GetParamFromAddr(void) const { return m_paramFromAddr; }
    UINT32 GetParamID(void) const { return m_paramID; }
    const char *GetParamClientID(void) const { return m_paramClientID; }
    virtual void SetParamSilent(int silent) { m_paramSilent = silent; }
    int GetParamSilent(void) const { return m_paramSilent; }

    /* Assign/query buffer used to return a value for "set" commands. */
    virtual void SetParamReplyBuffer(char *buf, unsigned int bufLen);
    char *GetParamReplyBuffer(unsigned int *bufLen);

    /* Copy parameters lists to another object. */
    SPxErrorCode CopyParameterListsTo(SPxObj *otherObj) const;

protected:
    /*
     * Protected functions (for use by derived classes).
     */
    /* Class-name functions.  The Get() is public.  */
    void SetClassName(const char *className);	/* Set the class name. */

    /* Set an object as either user-created (FALSE) or
     * system-created (TRUE). The companion IsSystemObj() is public. 
     */
    void SetSystemObj(int state) { m_systemObj = state; }

    /* Function to invoke deletion callbacks.  This will be called
     * automatically by the base class destructor, but may also be
     * called by the derived class destructors to give earlier warning
     * to registered functions that the object is being deleted (e.g.
     * before the derived class starts free'ing memory etc.).
     */
    void InvokeDeletionCallbacks(void);

    /* Used to install parameter list used by SetStateFromConfig(), etc */
    void SetParameterLists(const char *readlist[], const char *writelist[]);

    /* Used to append new parameters to the lists used by SetStateFromConfig(). */
    void AppendToParameterLists(const char *readParam, const char *writeParam);

    /* Used to uninstall some or all of the parameter list used by SetStateFromConfig(). */
    void RemoveFromParameterLists(const char *readlist[], const char *writelist[]);
    void RemoveEntryFromParameterLists(const char *readParam, const char *writeParam);
    void RemoveAllParameterLists(void);

    /* Used to iterate through parameter lists. */
    SPxErrorCode IterateParameterLists(SPxParamFn_t readParamCallback, 
                                       SPxParamFn_t writeParamCallback,
                                       void *userArg);
    int IsInReadParameterList(const char *readParam) const;
    int IsInWriteParameterList(const char *writeParam) const;

    /* Use to apply a parameter to the object */
    SPxErrorCode ApplyParamToObj(const char *name, int *numMatchesRtn);
    /* Use to retrieve parameter from object and store in config. */
    SPxErrorCode ApplyObjToParam(const char *name, int *numMatchesRtn);

private:
    /*
     * Private functions.
     */

    /*
     * Private variables.
     */
    
    /* Magic number of verification, to denote start of object. Value is
     * set as SPX_OBJ_MAGIC on object construction.
     */
    UINT32 m_magicVerify;

    /* Names and ID. */
    int m_nameSet;		/* Has name been explicitly set? */
    int m_id;			/* ID of this object instance. */
    char *m_className;		/* Name of the class of this object. */

    int m_save;			/* Should the object be saved. */
    int m_saveUpdateOnly;	/* Should only existing parameters be updated on save? */

    int m_useSharedDirectory;	/* Whether the object should use a shared directory for all files. */

    int m_isMaster;		/* Whether this instance is the master (only used with shared directories). */

    /* Flag to show if this is a user or system object */
    int m_systemObj;		/* TRUE if system-created else user-created */

    SOCKET_TYPE m_paramSock;	/* Socket for parameter control. */
    UINT32 m_paramFromAddr;	/* From address for parameter control. */
    UINT32 m_paramID;		/* Connection ID for parameter control. */
    const char *m_paramClientID; /* Client UUID for parameter control. */
    int m_paramSilent;		/* Silence bad parameter errors? */

    /* Control information. */
    SPxAtomic<int> m_active;				/* Activity state */
    SPxAtomic<int> m_standby;				/* Is in standby? */

    /* Support for registered callbacks for certain events. */
    SPxAtomic<int> m_deletionCallbacksCreating;
    SPxAtomic<int> m_deletionCallbacksCreated;
    SPxCallbackList *m_deletionCallbacks;	/* May be NULL */
    SPxAtomic<int> m_deletionCallbacksInvoked;	/* Have the deletion callbacks been invoked? */

    /* Linked list. */
    SPxObj *m_prev;		/* Previous object in chain before this one */
    SPxObj *m_next;		/* Next object in chain after this one */

    /* Parameter reply buffer. */
    char *m_paramReplyBuf;
    unsigned int m_paramReplyBufLen;

    /*
     * Static variables for class.
     */
    static SPxObj *m_firstInChain;	/* First object allocated */
    static SPxObj *m_lastInChain;	/* Last object allocated */
    static SPxAtomic<int> m_numberInstances;	/* Number of instances */
    static SPxAtomic<int> m_nextID;		/* Next ID to allocate */

    /* Override for only saving existing parameters. */
    static int m_overrideSaveUpdateOnly;

    struct impl;
    SPxAutoPtr<impl> m_p; /* Internal private object. */

    SPxErrorCode SetStateFromConfigLoop(const char *p, SPxErrorCode err);
    SPxErrorCode SetConfigFromStateLoop(const char *p, SPxErrorCode err);

    /* Prevent use of copy-constructor and assignment operator on SPxObj. */
    SPxObj(const SPxObj&) SPX_FUNCTION_DELETE;
    SPxObj& operator=(const SPxObj&) SPX_FUNCTION_DELETE;
    

}; /* SPxObj */


/*********************************************************************
*
*   Macros
*
**********************************************************************/


/*********************************************************************
*
*   Variable definitions
*
**********************************************************************/

/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

/* Deletion functions.  Do NOT call any other SPx function after these. */
void SPxDeleteAll(void);
void SPxDeleteAllSystem(void);

#endif /* _SPX_OBJ_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/

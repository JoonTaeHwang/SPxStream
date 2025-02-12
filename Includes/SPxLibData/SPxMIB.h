/*********************************************************************
 *
 * (c) Copyright 2020, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   Used to create a Management Information Database (MIB) 
 *   for use with an SNMP agent.
 *
 * Revision Control:
 *   14/12/20 v1.9   SP 	Reorder functions.
 *
 * Previous Changes:
 *   23/04/20 1.8   SP 	Support product traps.
 *   23/04/20 1.7   SP 	Add BuildOID().
 *   21/04/20 1.6   SP 	Fix build warnings.
 *   21/04/20 1.5   SP 	Support built-in objects.
 *   08/04/20 1.4   SP 	Add StringToEntOID().
 *   07/04/20 1.3   SP 	Add SetObjectValues().
 *   06/04/20 1.2   SP 	Various bug fixes.
 *   20/04/20 1.1   SP 	Initial version.
 *
 *********************************************************************/

#ifndef _SPX_MIB_H
#define _SPX_MIB_H

/* Library headers. */
#include "SPxLibUtils/SPxObj.h"
#include "SPxLibUtils/SPxCriticalSection.h"
#include "SPxLibUtils/SPxError.h"
#include "SPxLibUtils/SPxTimer.h"
#include "SPxLibUtils/SPxSdbApplication.h"
#include "SPxLibNet/SPxNetInterfaces.h"

/* Forward declare any classes required. */
class SPxThread;

/* SPxMIB dialog */
class SPxMIB : public SPxObj
{
public:
    /*
     * Public variables.
     */

    /* Cambridge Pixel's Private Enterprise Number. */
    static const UINT32 CAMBRIDGE_PIXEL_PEN = 55531;

    /* Max number of nodes in an OID. */
    static const unsigned int MAX_NUM_NODES = 20;

    /* Exception codes (also see BERTag_t in SPxSNMPAgent.h). */
    static const UINT16 NULL_EXCEPTION              = 0x05;
    static const UINT16 NO_SUCH_OBJECT_EXCEPTION    = 0x80;
    static const UINT16 NO_SUCH_INSTANCE_EXCEPTION  = 0x81;
    static const UINT16 END_OF_MIB_VIEW_EXCEPTION   = 0x82;

    /*
     * Public types.
     */

    /* Object ID. */
    struct OID_t
    {
        UINT32 nodes[MAX_NUM_NODES];    /* Array of nodes. */
        unsigned int numNodes;          /* Number of nodes in use. */
    };

    /* Object value type. */
    enum ValueType_t
    {
        VALUE_TYPE_NONE         = 0,    /* Value not set or NULL. */
        VALUE_TYPE_EXCEPTION    = 1,    /* Value is an exception code. */
        VALUE_TYPE_STRING       = 2,    /* Value is a string. */
        VALUE_TYPE_INT32        = 3,    /* Value is a 32-bit signed integer. */
        VALUE_TYPE_UINT32       = 4,    /* Value is a 32-bit unsigned integer. */
        VALUE_TYPE_IP_ADDRESS   = 5,    /* Value is a 32-bit IP address. */
        VALUE_TYPE_TIMER32      = 6     /* Value is a 32-bit timer in 100s second. */
    };

    /* Object value. */
    struct Value_t
    {
        ValueType_t type;   /* Value type. */
        char str[256];      /* Value encoded in a string. */
    };

    /* Object callback function. */
    struct Object_t;
    typedef void (*CallbackFn_t)(SPxMIB *mib, const OID_t *oid, void *userArg);

    /* Object in MIB. */
    struct Object_t
    {
        OID_t oid;          /* Object ID. */
        Value_t value;      /* Object value. */
        int isReadOnly;     /* Is read-only? */
        CallbackFn_t fn;    /* Callback function. */
        void *fnUserArg;    /* Callback user argument. */
    };

    /*
     * Public functions.
     */

    /* Construction and destruction. */
    SPxMIB(UINT32 pen=0);
    virtual ~SPxMIB(void);

    /* Private Enterprise Number. */
    unsigned int GetPEN(void) { return m_pen; }

    /* Generic application settings. */
    SPxErrorCode SetAppType(SPxSdbApplication::Type_t type);
    SPxSdbApplication::Type_t GetAppType(void) { return m_appType; }
    SPxErrorCode SetAppName(const char *name);
    const char *GetAppName(void);
    SPxErrorCode SetAppStatus(SPxStatus_t status);
    SPxStatus_t GetAppStatus(void) { return m_appStatus; }

    /* Utility functions. */
    SPxErrorCode StrToOID(const char *oidStr, OID_t *oid, int col=-1, int row=-1);
    SPxErrorCode StrToEntOID(const char *oidStr, OID_t *oid, int col=-1, int row=-1);
    SPxErrorCode StrToProductObjectOID(const char *oidStr, OID_t *oid, int col=-1, int row=-1);
    SPxErrorCode StrToProductTrapOID(const char *oidStr, OID_t *oid, int col=-1, int row=-1);

    /* Static Utility functions. */
    static SPxErrorCode StaticStrToOID(const char *oid1Str, 
                                       const char *oid2Str, 
                                       OID_t *oid,
                                       int col=-1, 
                                       int row=-1);
    static SPxErrorCode OIDToStr(const OID_t *oid, char *buf, unsigned int bufLen);
    static SPxErrorCode BuildOID(const OID_t *oid,
                                 OID_t *oidRtn,
                                 int col=-1, 
                                 int row=-1);

    /* User access. */
    SPxErrorCode Lock(void);
    SPxErrorCode Unlock(void);
    SPxCriticalSection *GetMutex(void) { return &m_mutex; }
    int IsObjectInMIB(const OID_t *oid);
    SPxErrorCode AddObject(const OID_t *oid, CallbackFn_t fn=NULL, void *fnUserArg=NULL);
    SPxErrorCode RemoveObject(const OID_t *oid, int removeDescendants);
    SPxErrorCode SetObjectValue(const OID_t *oid, const Value_t *value);
    SPxErrorCode SetObjectValue(const OID_t *oid, const char *value);
    SPxErrorCode SetObjectValue(const OID_t *oid, INT32 value);
    SPxErrorCode SetObjectValue(const OID_t *oid, UINT32 value);
    SPxErrorCode SetObjectValueAddr(const OID_t *oid, UINT32 addr);
    SPxErrorCode SetObjectValueTimer(const OID_t *oid, UINT32 ticks);
    SPxErrorCode SetObjectReadOnly(const OID_t *oid, int isReadOnly);
    int IsObjectReadOnly(const OID_t *oid);

    /* SNMP agent access. */
    SPxErrorCode AgentGetObject(const OID_t *oid, Object_t *objectRtn);
    SPxErrorCode AgentGetNextObject(const OID_t *oid, Object_t *objectRtn);
    SPxErrorCode AgentSetObjectValues(const Object_t objects[], 
                                      unsigned int numObjects, 
                                      unsigned int *errorIndexRtn);



protected:
    /*
     * Protected variables.
     */

    /*
     * Protected functions.
     */

    /* Parameter set/get. */
    virtual int SetParameter(char *name, char *value);
    virtual int GetParameter(char *name, char *valueBuf, int bufLen);

private:

    /*
     * Private constants.
     */

    /*
     * Private types.
     */

    /*
     * Private variables.
     */

    /* General. */
    struct impl;
    SPxAutoPtr<impl> m_p;
    mutable SPxCriticalSection m_mutex;     /* Object mutex. */
    unsigned int m_pen;                     /* Private Enterprise Number. */
    SPxTimer m_timers;                      /* Thread timer. */
    SPxNetInterfaces m_netIfs;              /* Used to get info on network interface. */
    SPxSdbApplication::Type_t m_appType;    /* Application type. */
    SPxStatus_t m_appStatus;                /* Application status. */
    SPxTime_t m_appStartTime;               /* Application start time. */

    /*
     * Private functions.
     */

    const char *oidToStr(const OID_t *oid);
    SPxErrorCode setAppName(const char *name);
    void updateNetObjects(void);

    /*
     * Private static functions.
     */

    /* Timers. */
    static void slowTimer(void *userArg);

    /* Object callbacks. */
    static void appLicNumHandler(SPxMIB *mib, const OID_t *oid, void *userArg);
    static void appLicStatusHandler(SPxMIB *mib, const OID_t *oid, void *userArg);
    static void appLicExpDateHandler(SPxMIB *mib, const OID_t *oid, void *userArg);
    static void appPIDHandler(SPxMIB *mib, const OID_t *oid, void *userArg);
    static void appUptimeHandler(SPxMIB *mib, const OID_t *oid, void *userArg);
    static void appConfigHandler(SPxMIB *mib, const OID_t *oid, void *userArg);
};

#endif /* _SPX_MIB_H */

/*********************************************************************
 *
 * End of file
 *
 **********************************************************************/

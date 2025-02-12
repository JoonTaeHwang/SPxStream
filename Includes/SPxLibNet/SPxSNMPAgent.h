/*********************************************************************
 *
 * (c) Copyright 2020, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   Used to add an SNMP agent to an application.
 *
 * Revision Control:
 *   21/04/20 v1.7   SP 	Various enhancements.
 *
 * Previous Changes:
 *   08/04/20 1.6   SP 	Support sending of trap messages.
 *   07/04/20 1.5   SP 	Support set operation.
 *   06/04/20 1.4   SP 	Various bug fixes.
 *   03/04/20 1.3   SP 	Support long-form BER length.
 *   02/04/20 1.2   SP 	Fix build warning.
 *   20/04/20 1.1   SP 	Initial version.
 *
 *********************************************************************/

#ifndef _SPX_SNMP_AGENT_H
#define _SPX_SNMP_AGENT_H

/* Library headers. */
#include "SPxLibUtils/SPxObj.h"
#include "SPxLibUtils/SPxCriticalSection.h"
#include "SPxLibUtils/SPxError.h"
#include "SPxLibData/SPxMIB.h"
#include "SPxLibNet/SPxPacketSend.h"
#include "SPxLibNet/SPxPacketDecoderNet.h"

/* Forward declare any classes required. */
class SPxThread;

/* SPxSNMPAgent dialog */
class SPxSNMPAgent : public SPxObj
{
public:

    /*
     * Public types.
     */

    /* Generic trap types. */
    enum GenTrap_t
    {
        GEN_TRAP_COLD_START             = 0,
        GEN_TRAP_WARM_START             = 1,
        GEN_TRAP_LINK_DOWN              = 2,
        GEN_TRAP_LINK_UP                = 3,
        GEN_TRAP_AUTHORIZATION_FAILURE  = 4,
        GEN_TRAP_EGP_NEIGHBOUR_LOSS     = 5,
        GEN_TRAP_ENTERPRISE_SPECIFIC    = 6
    };

    /*
     * Public variables.
     */

    /*
     * Public functions.
     */

    /* Construction and destruction. */
    SPxSNMPAgent(void);
    virtual ~SPxSNMPAgent(void);
    SPxErrorCode Create(void);

    // cppcheck-suppress virtualCallInConstructor
    virtual void Shutdown(void);

    /* Debugging. */
    SPxErrorCode SetLogFile(FILE *fPtr);
    FILE *GetLogFile(void) { return m_logFile; }
    SPxErrorCode SetVerbosity(unsigned int level);
    unsigned int GetVerbosity(void) { return m_verbosity; }

    /* MIB access. */
    SPxMIB *GetMIB(void) { return &m_mib; }

    /* Port and interface for receiving requests. */
    SPxErrorCode SetPortNumber(unsigned int port);
    unsigned int GetPortNumber(void) const;
    SPxErrorCode SetInterfaceAddr(const char *ifAddr);
    const char *GetInterfaceAddr(void);

    /* Address, port and interface for sending traps. */
    SPxErrorCode SetTrapAddress(const char *address, int port=0, const char *ifAddr=NULL);
    const char *GetTrapAddress(void);
    SPxErrorCode SetTrapPort(int port);
    int GetTrapPort(void) const;
    const char *GetTrapIfAddress(void) const;

    /* Community string. */
    SPxErrorCode SetCommunity(const char *str);
    const char *GetCommunity(void);

    /* Trap sending. */
    SPxErrorCode SendTrap(GenTrap_t genType,
                          UINT32 specType, 
                          const SPxMIB::Object_t objects[], 
                          unsigned int numObjects);

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

    /* Max number of objects in a request or response. */
    static const UINT32 MAX_NUM_OBJECTS = 256;

    /* SNMP versions. */
    static const UINT8 SNMP_VERSION_1 = 0;
    static const UINT8 SNMP_VERSION_2 = 1;
    static const UINT8 SNMP_VERSION_3 = 2;

    /*
     * Private tags.
     */

    /* ASN.1 Basic Encoding Rules (BER) tags. */
    enum BERTag_t
    {
        BER_TAG_UNKNOWN             = 0x00,
        BER_TAG_BOOLEAN             = 0x01,
        BER_TAG_INTEGER             = 0x02,
        BER_TAG_BIT_STRING          = 0x03,
        BER_TAG_OCTET_STRING        = 0x04,
        BER_TAG_NULL                = 0x05,
        BER_TAG_OID                 = 0x06,
        BER_TAG_SEQUENCE            = 0x30,
        BER_TAG_IP_ADDRESS          = 0x40,
        BER_TAG_COUNTER             = 0x41,
        BER_TAG_GAUGE               = 0x42,
        BER_TAG_TIME_TICKS          = 0x43,
        BER_TAG_COUNTER64           = 0x46,
        /* noSuchObject             = 0x80 (see SPxMIB.h). */
        /* noSuchInstance           = 0x81 (see SPxMIB.h). */
        /* endOfMibView             = 0x82 (see SPxMIB.h). */
        BER_TAG_SNMP_GET            = 0xA0,
        BER_TAG_SNMP_GETNEXT        = 0xA1,
        BER_TAG_SNMP_RESPONSE       = 0xA2,
        BER_TAG_SNMP_SET            = 0xA3,
        BER_TAG_SNMP_TRAP           = 0xA4,
        BER_TAG_SNMP_GETBULK        = 0xA5,
        BER_TAG_SNMP_INFORM         = 0xA6,
        BER_TAG_SNMP_V2_TRAP        = 0xA7,
        BER_TAG_SNMP_V2_REPORT      = 0xA8
    };

    /* SNMP status. */
    enum Error_t
    {
        /* SNMPv1 and later. */
        ERROR_OK                    = 0,
        ERROR_TOO_BIG               = 1,
        ERROR_NO_SUCH_NAME          = 2,
        ERROR_BAD_VALUE             = 3,
        ERROR_READ_ONLY             = 4,
        ERROR_GENERAL               = 5,

        /* SNMPv2 and later. */
        ERROR_NO_ACCESS             = 6,
        ERROR_WRONG_TYPE            = 7,
        ERROR_WRONG_LENGTH          = 8,
        ERROR_WRONG_ENCODING        = 9,
        ERROR_WRONG_VALUE           = 10,
        ERROR_NO_CREATION           = 11,
        ERROR_INCONSISTENT_VALUE    = 12,
        ERROR_RESOURCE_UNAVAILABLE  = 13,
        ERROR_COMMIT_FAILED         = 14,
        ERROR_UNDO_FAILED           = 15,
        ERROR_AUTHORIZATION_ERROR   = 16,
        ERROR_NOT_WRITABLE          = 17,
        ERROR_INCONSISTENT_NAME     = 18
    };

    /* Request. */
    struct Request_t
    {
        UINT32 senderAddr;                          /* Sender host address. */
        UINT16 senderPort;                          /* Sender UDP port. */
        INT32 version;                              /* SNMP_VERSION_x */
        char community[256];                        /* Community string. */
        BERTag_t type;                              /* Request type. */
        INT32 id;                                   /* Request ID. */
        int isIDSet;                                /* TRUE if request ID is set. */
        INT32 nonRepeaters;                         /* Used in a GETBULK request. */
        INT32 maxRepititions;                       /* Used in a GETBULK request. */
        SPxMIB::Object_t objects[MAX_NUM_OBJECTS];  /* Objects read from request. */
        UINT32 numObjects;                          /* Length of objects[]. */
    };

    /* Response. */
    struct Response_t
    {
        Error_t errorStatus;                        /* Error status to report. */
        UINT32 errorIndex;                          /* One-based error index to report. */
        SPxMIB::Object_t objects[MAX_NUM_OBJECTS];  /* Objects to write to response. */
        UINT32 numObjects;                          /* Length of objects[]. */
    };

    /*
     * Private variables.
     */

    /* General. */
    struct impl;
    SPxAutoPtr<impl> m_p;
    SPxCriticalSection m_logMutex;          /* Mutex for message logging. */
    unsigned int m_verbosity;               /* Debug verbosity. */
    FILE *m_logFile;                        /* Debug log file. */
    mutable SPxCriticalSection m_mutex;     /* Object mutex. */
    SPxPacketDecoderNet m_receiver;         /* Message receiver. */
    SPxPacketSender m_sender;               /* Message sender (except traps). */
    SPxPacketSender m_trapSender;           /* Trap sender. */
    SPxMIB m_mib;                           /* Managment Information Database. */

    /*
     * Private functions.
     */

    /* Debug. */
    void logMesg(UINT32 verbosity, const char *format, ...);

    /* Request handling. */
    void receiveRequest(UINT32 senderAddr,
                        UINT16 senderPort,
                        const UINT8 *payload,
                        UINT32 numBytes);

    SPxErrorCode decodeRequest(const UINT8 *payload,
                               UINT32 numBytes,
                               Request_t *request);

    void handleRequest(const Request_t *request, Response_t *response);
    void handleGet(const Request_t *request, Response_t *response);
    void handleGetNext(const Request_t *request, Response_t *response);
    void handleGetBulk(const Request_t *request, Response_t *response);
    void handleSet(const Request_t *request, Response_t *response);

    void copyObjects(const Request_t *request, 
                     Response_t *response);

    SPxMIB::Object_t *getObject(const SPxMIB::OID_t *oid,
                                UINT32 index,
                                int getNext,
                                Response_t *response);

    /* Response sending. */
    void sendResponse(const Request_t *request, const Response_t *response);

    /* Read BER data. */
    SPxErrorCode readBERStart(const char *berName,
                              const UINT8 **mesgPtrRtn,
                              UINT32 *mesgLenRtn,
                              BERTag_t *tagRtn,
                              UINT32 *lenRtn);

    SPxErrorCode readBERInt32(const char *berName,
                              const UINT8 **mesgPtr,
                              UINT32 *mesgLen,
                              UINT32 valueLen,
                              INT32 *valueRtn);

    SPxErrorCode readBERString(const char *berName,
                               const UINT8 **mesgPtr,
                               UINT32 *mesgLen,
                               UINT32 valueLen,
                               char *buf,
                               UINT32 bufLen);

    SPxErrorCode readBEROID(const char *berName,
                            const UINT8 **mesgPtr,
                            UINT32 *mesgLen,
                            UINT32 valueLen,
                            SPxMIB::OID_t *oidRtn);

    SPxErrorCode skipBERValue(const char *berName,
                              const UINT8 **mesgPtr,
                              UINT32 *mesgLen,
                              UINT32 valueLen);

    /* Write BER data. */
    SPxErrorCode writeBERStart(const char *berName,
                               UINT8 **bufPtr,
                               UINT32 *bufLen,
                               BERTag_t tag,
                               UINT8 **lenPtr);

    SPxErrorCode insertBERLen(const char *berName,
                             UINT8 **bufPtr,
                             UINT32 *bufLen,
                             UINT8 *lenPtr,
                             UINT32 len);

    SPxErrorCode writeBERInt32(const char *berName,
                               UINT8 **bufPtr,
                               UINT32 *bufLen,
                               INT32 value);

    SPxErrorCode writeBERUInt32(const char *berName,
                                UINT8 **bufPtr,
                                UINT32 *bufLen,
                                BERTag_t tag,
                                UINT32 value);

    SPxErrorCode writeBERString(const char *berName,
                                UINT8 **bufPtr,
                                UINT32 *bufLen,
                                const char *value);

    SPxErrorCode writeBEROID(const char *berName,
                             UINT8 **bufPtr,
                             UINT32 *bufLen,
                             const SPxMIB::OID_t *oid);

    SPxErrorCode writeBERVarBindings(UINT8 **bufPtr,
                                     UINT32 *bufLen,
                                     const SPxMIB::Object_t objects[], 
                                     unsigned int numObjects);

    /*
     * Private static functions.
     */

    static void requestHandler(SPxPacketDecoder *decoder,
                              void *userArg,
                              UINT32 packetType,
                              struct sockaddr_in *from,
                              SPxTime_t *timestamp,
                              const unsigned char *payload,
                              unsigned int numBytes);
};

#endif /* _SPX_SNMP_AGENT_H */

/*********************************************************************
 *
 * End of file
 *
 **********************************************************************/

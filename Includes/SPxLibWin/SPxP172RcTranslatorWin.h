/*********************************************************************
 *
 * (c) Copyright 2014 - 2022, Cambridge Pixel Ltd.
 *
 *
 * Purpose:
 *   This class is used to translate PKSER messages (as used by the 
 *   SBS 700, 800 and 900 series radars) to PCAN message format 
 *   (as used by SxV/SCV, Mk5, Mk11TU and Upmast).
 *
 * Revision Control:
 *   12/10/22 v1.11  SP 	Support Tx sweep sector commands and status.
 *
 * Previous Changes:
 *   27/09/22 1.10  SP 	Add initial SPEXER 600 support.
 *   04/11/21 1.9   SP 	Support Mk5 X-Band.
 *   09/08/18 1.8   SP 	Add mutex protection to SendCommand().
 *   12/02/18 1.7   BTB	Support Upmast radars.
 *                     	Move CalcChecksum() to parent class.
 *   11/01/16 1.6   SP 	Add m_pkserPartialSentence.
 *   27/11/15 1.5   SP 	Add IsProtocolSBand().
 *   26/11/15 1.4   SP 	Support MK11 Turning Unit (Mk11TU).
 *                     	Rename SxV specific functions and types.
 *   01/09/14 1.3   AGC	VS2015 format string changes.
 *   26/03/14 1.2   SP 	Support additional commands.
 *   17/02/14 1.1   SP 	Initial version (not complete).
 *
 *********************************************************************/

#pragma once

/*
 * Other headers required.
 */

/* The various classes that we use. */
#include "SPxLibWin/SPxP172RcTransportWin.h"

/*********************************************************************
 *
 *   Constants
 *
 *********************************************************************/

/* Maximum field length used for parsing message fields.
 * If this is ever changed be sure to change the maximum
 * fields lengths set in sscanf() format strings. These are
 * set to SPX_P172_MAX_FIELD_LEN-1 to allow space for the
 * string terminator.
 */
#define SPX_P172_MAX_FIELD_LEN 32

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

/*
 * Define our class.
 */
class SPxP172RcTranslatorWin : public SPxP172RcTransportWin
{
public:

    /*
     * Public types.
     */

    /* Supported protocols (and model type for PCAN radars). */
    typedef enum
    {
        PROTOCOL_PKSER = 0,             /* PKSER - SBS 700/800/900 series. */
        PROTOCOL_PCAN_SXV = 1,          /* PCAN  - SxV and SCV. */
        PROTOCOL_PCAN_MK11_XBAND = 2,   /* PCAN  - Mk11 Turning Unit X-Band. */
        PROTOCOL_PCAN_MK11_SBAND = 3,   /* PCAN  - Mk11 Turning Unit S-Band. */
        PROTOCOL_PCAN_UPMAST_XBAND = 4, /* PCAN  - Upmast X-Band. */
        PROTOCOL_PCAN_UPMAST_SBAND = 5, /* PCAN  - Upmast S-Band. */
        PROTOCOL_PCAN_MK5_XBAND = 6,    /* PCAN  - Mk5 X-Band. */
     /* PROTOCOL_PCAN_MK5_SBAND = 7,       Reserved for Mk5 S-Band or variant. */
        PROTOCOL_PCAN_S600_XBAND = 8    /* PCAN  - SPEXER 600 X-Band. */
     /* PROTOCOL_PCAN_S600_SBAND = 9       Reserved for SPEXER 600 S-Band or variant. */

    } Protocol_t;

    /* Message classification. */
    typedef enum
    {
        MESG_TYPE_UNKNOWN = 0,
        MESG_TYPE_HEARTBEAT,
        MESG_TYPE_STATUS,
        MESG_TYPE_COMMAND,

    } MesgType_t;

    /* Message reporting. */
    typedef struct
    {
        MesgType_t type;    /* Message classification. */
        const char *text;   /* Message text in native format. */

    } MesgInfo_t;
    
    /*
     * Public functions.
     */

    /* Create and destroy */
    SPxP172RcTranslatorWin(void);
    virtual ~SPxP172RcTranslatorWin(void);

    /* Configuration. */
    virtual SPxErrorCode SetProtocol(Protocol_t protocol);
    virtual Protocol_t GetProtocol(void) { return m_protocol; }
    virtual SPxErrorCode SetPcanLocalNode(unsigned int id);
    virtual unsigned int GetPcanLocalNode(void) { return m_pcanLocalNode; }
    virtual SPxErrorCode SetPcanRadarNode(unsigned int id);
    virtual unsigned int GetPcanRadarNode(void) { return m_pcanRadarNode; }
    virtual void NotifyHeartbeatTimeout(void);

    /* Message handler callbacks (in native format, PKSER or PCAN). */
    virtual SPxErrorCode AddTxMesgTextHandler(void *userArg, 
                                              SPxCallbackListFn_t fn);
    virtual SPxErrorCode AddRxMesgTextHandler(void *userArg, 
                                              SPxCallbackListFn_t fn);

    /* Message handler callbacks (in PKSER format). */
    virtual SPxErrorCode AddPkserRxMesgTextHandler(void *userArg, 
                                                   SPxCallbackListFn_t fn);

    /* Message sending and receiving. */
    virtual SPxErrorCode SendCommand(MesgDest_t dest,
                                     const char *part1,
                                     const char *part2,
                                     const char *part3=NULL,
                                     const char *part4=NULL,
                                     const char *part5=NULL,
                                     const char *part6=NULL);
    virtual SPxErrorCode SendCommand(MesgDest_t dest, const char *body);
    SPxErrorCode PollForMesgs(void);

protected:

    /*
     * Protected variables.
     */

    /*
     * Protected functions.
     */

    /* Message handling. */
    virtual const char *HandleMesg(const char *sentence);

    /* Parameters set/get */
    virtual int SetParameter(char *name, char *value);
    virtual int GetParameter(char *name, char *valueBuf, int bufLen);

private:

    /*
     * Private types.
     */

    /* PCAN radar connection states. */
    typedef enum
    {
        PCAN_STATE_NOT_AVAILABLE = 0, /* Not available, e.g. controlled by someone else. */
        PCAN_STATE_AVAILABLE = 1,     /* Available for control. */
        PCAN_STATE_CONTROLLED = 2,    /* Controlled by us. */
        PCAN_STATE_CONFIGURED = 3    /* Configured by us. */

    } PcanState_t;

    /* PCAN data message types. */
    typedef enum
    {
        PCAN_DATA_MESG_TYPE_NONE = 0,                   /* No message. */
        PCAN_DATA_MESG_TYPE_STATUS = 1,                 /* General status message. */
        PCAN_DATA_MESG_TYPE_TRUE_SECTOR_TX_STATUS = 2,  /* True sector transmit status message. */
        PCAN_DATA_MESG_TYPE_REL_SECTOR_BLANK_STATUS = 3,/* Relative sector blanking status message. */
        PCAN_DATA_MESG_TYPE_SECTOR_PROC_STATUS = 4,     /* Sector processing status message. */
        PCAN_DATA_MESG_TYPE_VIDEO_STATUS = 5,           /* Video status message. */
        PCAN_DATA_MESG_TYPE_TX_SWEEP_STATUS = 6         /* Tx sweep sectors status message. */

    } PcanDataMesgType_t;

    /*
     * Private variables.
     */

    /* Protocol to use. */
    Protocol_t m_protocol;

    /* Translated sentence buffer. */
    char m_pkserSentence[SPX_P172_MAX_SENTENCE_LEN];

    /* Partially translated sentence buffer. */
    CStringA *m_pkserPartialSentence;

    /* Message handlers (native format). */
    SPxCallbackList *m_txMesgTextCbList;
    SPxCallbackList *m_rxMesgTextCbList;

    /* Message handlers (PKSER format). */
    SPxCallbackList *m_pkserRxMesgTextCbList;

    /* PCAN radar connection state. */
    PcanState_t m_pcanState;  

    /* Nodes IDs. */
    UINT8 m_pcanLocalNode;   /* This controller's node ID. */
    UINT8 m_pcanRadarNode;   /* Remote radar's node ID. */

    /* Last time we sent a 'set node address' message. */
    UINT32 m_pcanLastSetNodeAddrMesgTime;

    /* Next expected data message type. */
    PcanDataMesgType_t m_pcanNextDataMesgType;

    /* Next expected data size in bytes. */
    int m_pcanNextDataSizeBytes;

    /* Misc. */
    SPxCriticalSection m_sendMutex;

    /*
     * Private functions.
     */

    SPxErrorCode SendPkserSentence(MesgDest_t dest, 
                                   const char *pkserBody);

    SPxErrorCode SendPcanSentence(MesgDest_t dest, 
                                  const char *pkserBody);

    SPxErrorCode HandlePcanSentence(const char *pcanSentenceIn,
                                    char *pkserSentenceOut,
                                    unsigned int outBufLen,
                                    MesgType_t *mesgType);
    
    SPxErrorCode HandlePcanE(const char *pcanSentenceIn,
                             CStringA *sentence,
                             MesgType_t *mesgType);

    SPxErrorCode HandlePcanM(const char *pcanSentenceIn,
                             CStringA *sentence,
                             MesgType_t *mesgType);

    SPxErrorCode HandlePcanT(const char *pcanSentenceIn,
                             CStringA *sentence,
                             MesgType_t *mesgType);
    
    SPxErrorCode HandlePcanD(const char *pcanSentenceIn,
                             CStringA *sentence,
                             MesgType_t *mesgType);

    SPxErrorCode HandlePcanGeneralStatus(const char *pcanSentenceIn,
                                         CStringA *sentence);

    SPxErrorCode HandlePcanSectorStatus(const char *pcanSentenceIn,
                                        CStringA *sentence,
                                        BOOL isBlankingSector);

    SPxErrorCode HandlePcanProcSectorStatus(const char *pcanSentenceIn,
                                            CStringA *sentence);

    SPxErrorCode HandlePcanTxSweepSectorsStatus(const char *pcanSentenceIn,
                                                CStringA *sentence);

    MesgType_t GetPkserMesgType(const char *sentence);

    BOOL IsProtocolPcan(void);

    BOOL IsProtocolSBand(void);

    SPxErrorCode GetRangeVal(const char *rangeStr, UINT8 *rangeValRtn);
    SPxErrorCode GetRangeStr(UINT8 rangeVal, char *buf, unsigned int bufLen);

    /*
     * Private static functions.
     */

}; /* SPxP172RcTranslatorWin */

/*********************************************************************
 *
 * End of file
 *
 *********************************************************************/

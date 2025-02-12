/*********************************************************************
 *
 * (c) Copyright 2015 - 2021, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   SPxChannelNetRaw class header.
 *
 * Revision Control:
 *   28/09/21 v1.14  SP 	Support recording of uncompressed data.
 *
 * Previous Changes:
 *   02/11/20 1.13  AGC	Use thread event to speed-up shutdown.
 *   26/10/20 1.12  AJH	Add m_changePending.
 *   07/08/20 1.11  SP 	Add Set/GetDataTimeout().
 *   03/01/19 1.10  SP 	Add IsFilteringAvailable().
 *   02/01/19 1.9   SP 	Support include and exclude filters.
 *   21/12/18 1.8   SP 	Support input/replay filtering.
 *   03/08/17 1.7   SP 	Override SetSaveUpdateOnly().
 *   12/04/17 1.6   AGC	Correct SPxThread forward declaration.
 *   22/01/16 1.5   SP 	Add fileID constructor arg.
 *   03/11/15 1.4   AGC	Packet decoder callback data now const.
 *   15/06/15 1.3   SP 	Don't accept connections when disabled.
 *                     	Release port for replay channels if no data.
 *   26/03/15 1.2   SP 	Support output selection.
 *   06/03/15 1.1   SP 	Initial version.
 *
 *********************************************************************/

#ifndef _SPX_CHANNEL_NET_RAW_H
#define _SPX_CHANNEL_NET_RAW_H

/* Other headers required. */
#include "SPxLibData/SPxChannel.h"
#include "SPxLibData/SPxCompress.h"

/*********************************************************************
 *
 *  Constants
 *
 *********************************************************************/

/* Max number of filters. */
#define SPX_CHANNEL_NET_RAW_MAX_NUM_FILTERS 4

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

/*********************************************************************
 *
 *   Class definitions
 *
 *********************************************************************/

/* Forward declare any classes required. */
class SPxThread;

class SPxChannelNetRaw : public SPxChannel
{
public:

    /*
     * Public types.
     */

    /* Filter mode. */
    typedef enum
    {
        FILTERING_MODE_NONE    = 0,    /* Filtering off. */
        FILTERING_MODE_INCLUDE = 1,    /* Filters define packets to include. */
        FILTERING_MODE_EXCLUDE = 2     /* Filters define packets to exclude. */

    } FilteringMode_t;

    /* Filter type. */
    typedef enum
    {
        FILTER_TYPE_NONE    = 0,    /* Filter off. */
        FILTER_TYPE_SPX     = 1,    /* Filter on SPx packet type. */
        FILTER_TYPE_ASTERIX = 2     /* Filter on ASTERIX category. */

    } FilterType_t;

    /*
     * Public variables.
     */

    /*
     * Public functions.
     */

    /* Constructor and destructor. */
    SPxChannelNetRaw(SPxChannelDatabase *database, UINT8 id, UINT8 fileID);
    SPxErrorCode Create(Media_t inputMedia, Media_t outputMedias);
    virtual ~SPxChannelNetRaw(void);

    /* Live input config functions. */
    virtual void GetLiveInputInfo(char *buf, 
                                  unsigned int bufSizeBytes) const;
    virtual int IsLiveInputAvail(void) const;

    /* Output config functions. */
    virtual int IsOutputMediaAvail(Media_t media) const;
    virtual SPxErrorCode SetOutputEnabled(unsigned int index, int isEnabled);
    virtual int IsOutputEnabled(unsigned int index) const;
    virtual void GetOutputInfo(unsigned int index,
                               char *buf, 
                               unsigned int bufSizeBytes) const;
    virtual SPxNetAddr *GetOutputNetCtrlIf(unsigned int index);

    /* SPxNetAddr interface functions. */
    virtual SPxErrorCode SetAddress(const char *address, int port=0, 
                                    const char *ifAddr=NULL);
    virtual SPxErrorCode SetAddress(UINT32 address, int port=0, 
                                    const char *ifAddr=NULL);
    virtual UINT32 GetAddress(void) const;
    virtual const char *GetAddressString(void) const;
    virtual int GetPort(void) const;
    virtual UINT32 GetIfAddress(void) const;
    virtual const char *GetIfAddressString(void) const;
    
    /* Get packet sender. */
    virtual SPxPacketSender *GetPacketSender(void) { return m_packetSender; }

    /* Config load/save. */
    virtual SPxErrorCode SetStateFromConfig(void);
    virtual SPxErrorCode SetConfigFromState(void);
    virtual void SetSaveUpdateOnly(int updateOnly);

    /* Filtering. */
    virtual int IsFilteringAvailable(void) { return !m_isTCP; }
    virtual SPxErrorCode SetFilteringMode(FilteringMode_t mode);
    virtual FilteringMode_t GetFilteringMode(void) { return m_filteringMode; }
    virtual int IsFilteringEnabled(void) { return m_filteringMode != FILTERING_MODE_NONE; }
    virtual SPxErrorCode SetFilter(unsigned int index, FilterType_t type, UINT32 value);
    virtual FilterType_t GetFilterType(unsigned int index);
    virtual unsigned int GetFilterValue(unsigned int index);

    /* Compression. */
    virtual SPxErrorCode SetEncodeType(int type);
    virtual int GetEncodeType(void) { return m_encodeType; }

    /* Misc. */
    virtual SPxErrorCode SetDataTimeout(unsigned int msecs);
    virtual unsigned int GetDataTimeout(void) { return m_dataTimeoutMsecs; }

protected:

    /*
     * Protected variables.
     */

    /*
     * Protected functions.
     */

    /* Live input config functions. */
    virtual SPxErrorCode SetLiveInputEnabled(int isEnabled);
    virtual int IsLiveInputEnabled(void) const;

    /* Check and get channel status. */
    virtual SPxStatus_t CheckStatus(char *mesgBuf, unsigned int mesgBufLen);

    /* Replay packet handler. */
    virtual void HandleReplayPacket(UINT16 packetType,
                                    const unsigned char *data,
                                    unsigned int dataSizeBytes);

    /* Background processing function called from thread. */
    virtual UINT32 doBackgroundProcessing(void);

    /* Parameter handling. */
    virtual int SetParameter(char *name, char *value);
    virtual int GetParameter(char *name, char *valueBuf, int bufLen);

private:

    /*
     * Private types.
     */

    /* Filter. */
    typedef struct
    {
        FilterType_t type;
        UINT32 value;

    } Filter_t;

    /*
     * Private variables.
     */

    /* General. */
    int m_isLiveInputEnabled;            /* Is input enabled? */
    int m_isTCP;                         /* TRUE if TCP, FALSE if UDP. */
    FilteringMode_t m_filteringMode;     /* Filtering mode. */
    Filter_t m_filters[SPX_CHANNEL_NET_RAW_MAX_NUM_FILTERS]; /* Filters. */
    unsigned int m_dataTimeoutMsecs;     /* Data not received timeout in msecs. */
    int m_changePending;                 /* Setup change pending. */

    /* TCP/UDP Input. */
    SPxCriticalSection m_srcMutex;       /* Source socket mutex. */
    mutable char m_srcAddrString[16];    /* Current input address. */
    mutable char m_srcIfAddrString[16];  /* Current input ifaddrs. */
    mutable int m_srcPort;               /* Current input port. */
    SOCKET_TYPE m_srcSock;		 /* Handle of socket */
    struct sockaddr_in m_srcSockAddr;	 /* Socket address structure */
    struct sockaddr_in m_srcFromAddr;	 /* Remote address structure */
    struct sockaddr_in m_srcIfAddr;	 /* Interface address structure */
    int m_isSrcBoundOrConnected;         /* Is socket bound or connected? */
    int m_isSrcMcastJoined;              /* Is UDP multicast group joined. */
    UINT8 *m_srcBuf;                     /* Buffer to read into. */
    unsigned int m_srcBufSizeBytes;      /* Buffer size. */
    SPxThread *m_srcThread;              /* Thread used to receive data. */
    int m_isUserOutputEnabled;           /* Has user enabled the output? */
   
    /* TCP/UDP output. */
    SPxPacketSender *m_packetSender;     /* Packet sender.*/

    /* Compression. */
    UINT8 *m_compBuf;                    /* Buffer to use for compression. */
    unsigned int m_compBufSizeBytes;     /* Buffer size. */
    SPxCompress m_compress;              /* Compression object. */
    int m_encodeType;                    /* Compression type. */
    
    /*
     * Private functions.
     */

    void doInputProcessing(SPxThread *thread);
    void checkInputSocket(void);
    void joinOrLeaveMcast(int join);
    SPxErrorCode updateInputSocketAddress(const char *addr,
                                          int port,
                                          const char *ifAddr);
    int readInputSocket(void);
    void recordData(const UINT8 *data, unsigned int numBytes);
    void outputData(const UINT8 *data, unsigned int numBytes);
    int isAllowedByFilters(const UINT8 *data, unsigned int numBytes);
    int isAllowedByFilter(const Filter_t *filter,
                          const UINT8 *data, 
                          unsigned int numBytes);
    int isSPxPacketAllowed(UINT32 packetType, 
                           const UINT8 *data, 
                           unsigned int numBytes);
    int isAsterixPacketAllowed(UINT32 category, 
                               const UINT8 *data, 
                               unsigned int numBytes);

    /*
     * Private static functions.
     */

    static void *inputThreadHandler(SPxThread *thread);

}; /* SPxChannelNetRaw */

#endif /* _SPX_CHANNEL_NET_RAW_H */

/*********************************************************************
 *
 *      End of file
 *
 *********************************************************************/

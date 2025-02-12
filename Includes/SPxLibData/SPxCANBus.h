/*********************************************************************
*
* (c) Copyright 2011 - 2018, Cambridge Pixel Ltd.
*
*
* Purpose:
*    Header for SPxCANBus class.
*
* Revision Control:
*   20/02/18 v1.4   BTB	Track connection state to improve error handling.
*
* Previous Changes:
*   16/02/18 1.3   BTB	Give member variables correct prefix.
*			Remove checking of status flags, which was too slow.
*			Base whether to write on pending message responses.
*   15/02/18 1.2   BTB	Schedule own function to do PollSerialPort.
*			This keeps all polls on one timer thread.
*   12/02/18 1.1   BTB	Initial Version.
*
**********************************************************************/

#ifndef _SPX_CANBUS_H
#define _SPX_CANBUS_H

/* For SPxAutoPtr. */
#include "SPxLibUtils/SPxAutoPtr.h"

/* For SPxCriticalSection. */
#include "SPxLibUtils/SPxCriticalSection.h"

/* For SPxTimer */
#include "SPxLibUtils/SPxTimer.h"

/* We need our base class. */
#include "SPxLibData/SPxSerialPort.h"

/* Supported serial modes */
enum SPxCANBusAdapter_t
{
    SPX_CANBUS_ADAPTER_LAWICEL = 0,	/* Lawicel adapter (default). */
    SPX_CANBUS_ADAPTER_GRIDCONNECT = 1	/* grid connect adapter. */

};

/* This state (Lawicel only) identifies what we are currently doing
 * with the CANBus connection.
 * This will change what we report on receiving an error.
 */
enum SPXCANBusConnectionState_t
{
    SPX_CANBUS_CONNECTION_CLEARING_QUEUES = 0,
    SPX_CANBUS_CONNECTION_SETTING_BAUD = 1,
    SPX_CANBUS_CONNECTION_OPENING = 2,
    SPX_CANBUS_CONNECTION_OPEN = 3,
    SPX_CANBUS_CONNECTION_CLOSING = 4

};

/*
* Define our class.
*/
class SPxCANBus : public SPxSerialPort
{
public:

    /*
    * Public functions.
    */

    explicit SPxCANBus(SPxCANBusAdapter_t adapter = SPX_CANBUS_ADAPTER_GRIDCONNECT);
    virtual ~SPxCANBus(void);

    SPxErrorCode OpenConnection(const char *name = NULL,
				unsigned int baud = 0,
				int openPort = TRUE,
				SPxSerialAccess_t access = SPX_SERIAL_ACCESS_READ_WRITE,
				SPxSerialParity_t parity = SPX_SERIAL_PARITY_NONE);

    void BufferData(const unsigned char *data, unsigned int dataLenBytes);
    int ReadData(const unsigned char *data, unsigned int dataLenBytes,
	unsigned char *buf, unsigned int bufSize, unsigned int *bufBytes);

private:

    /*
    * Private variables.
    */
    struct impl;
    SPxAutoPtr<impl> m_p;

    /* Mutex */
    SPxCriticalSection m_mutex;

    SPxAutoPtr<SPxTimer> m_timer;
    
    /* Current CANBus adapter. */
    SPxCANBusAdapter_t m_adapter;

    /* Current connection state.
     * Only used for Lawicel adapter.
     */
    SPXCANBusConnectionState_t m_connState;

    /* Only used for Lawicel adapter. */
    unsigned int m_pendingMesgResponses;

    /*
     * Private functions.
     */

    void Init(void);

    /* Schedulable function used to call PollSerialPort. */
    void PollForMesgs(void) { SPxSerialPort::PollSerialPort(); }

    /* Data <-> gridconnect adapter message conversion. */
    void OpenGridConnectConnection(void);
    void BufferGridConnectMessage(const unsigned char *data, unsigned int dataLenBytes);
    int ReadGridConnectMessage(const unsigned char *data, unsigned int dataLenBytes,
	unsigned char *buf, unsigned int bufSize, unsigned int *bufBytes);

    /* Data <-> Lawicel adapter message conversion. */
    void OpenLawicelConnection(void);
    void CloseLawicelConnection(void);
    void BufferLawicelMessage(const unsigned char *data, unsigned int dataLenBytes);
    int ReadLawicelMessage(const unsigned char *data, unsigned int dataLenBytes,
	unsigned char *buf, unsigned int bufSize, unsigned int *bufBytes);

    /*
    * Private static functions.
    */

    /* Functions for writing queued CANBus messages. */
    static void WriteGridConnectMessagesWrapper(void *userArg);
    void WriteGridConnectMessages(void);

    static void WriteLawicelMessagesWrapper(void *userArg);
    void WriteLawicelMessages(void);

}; /* SPxCANBus */

#endif /* _SPX_CANBUS_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/

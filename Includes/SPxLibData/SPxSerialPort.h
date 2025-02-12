/*********************************************************************
 *
 * (c) Copyright 2011 - 2018, 2020, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   Header for SPxSerialPort class.
 *
 * Revision Control:
 *  07/01/20 v1.22   AJH	Support raw mode for Linux.
 *
 * Previous Changes:
 *  22/06/18 1.21   AGC	Fix cppcheck warnings.
 *  30/04/18 1.20   AGC	Add PrepareForRestart().
 *  14/12/15 1.19   AJH	Added parity argument to SetSerialPort().
 *  03/11/15 1.18   AGC	Packet decoder callback data now const.
 *  23/06/15 1.17   AGC	Derive from SPxPacketDecoder.
 *  12/06/15 1.16   REW	Add SPX_SERIAL_MANUFACTURER_... options and DLE.
 *  26/03/15 1.15   SP 	Support SPxSerial interface.
 *  07/01/15 1.14   AGC	Add IsOpen() function.
 *			Support parameters.
 *  06/11/14 1.13   REW	Add SPX_SERIAL_MODE_TPN24.
 *  03/09/14 1.12   REW	Rename BTE2000 to HDLC_IFF.
 *  07/03/14 1.11   REW	Add SPX_SERIAL_MODE_ASTERIX.
 *  14/08/13 1.10   AGC	Improve raw data support.
 *  12/12/12 1.9    AGC	Allow AIS and NMEA to start with ! or $.
 *  12/10/12 1.8    REW	Add SPX_SERIAL_MODE_OUMP.
 *  01/10/12 1.7    AGC	Allow WriteData() to be called from
 *  20/06/12 1.6    REW	Add SPX_SERIAL_MODE_BTE2000.
 *  08/05/12 1.5    AGC	Add support for setting access mode.
 *  07/10/11 1.4    SP 	Add optional openPort arg to SetSerialPort().
 *			Add OpenSerialPort().
 *			Make CloseSerialPort() public.
 *  22/09/11 1.3    AGC	Make GetSerialName() and GetSerialBaud() const.
 *  13/09/11 1.2    AGC	Rename from SPxSerialRecv to SPxSerialPort.
 *			Add WriteData() function.
 *			Add SetSessionStrings() function.
 *  09/08/11 1.1    SP	Initial Version.
 *
 **********************************************************************/

#ifndef _SPX_SERIAL_PORT_H
#define _SPX_SERIAL_PORT_H

/*
 * Other headers required.
 */

/* For SPxAutoPtr. */
#include "SPxLibUtils/SPxAutoPtr.h"

/* For SPxCriticalSection. */
#include "SPxLibUtils/SPxCriticalSection.h"

/* For SPxErrorCode. */
#include "SPxLibUtils/SPxError.h"

/* We need SPxPacketSend for network output. */
#include "SPxLibNet/SPxPacketSend.h"

/* We need our base classes. */
#include "SPxLibData/SPxPacketDecoder.h"
#include "SPxLibData/SPxSerial.h"

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

/* Forward declare our class so it can be used below */
class SPxSerialPort;
struct SPxTime_tag;
class SPxThread;

/* Supported serial modes */
enum SPxSerialMode_t
{
    SPX_SERIAL_MODE_RAW = 0,	/* Raw serial data */
    SPX_SERIAL_MODE_NMEA = 1,	/* NMEA-0183 sentences (beginning with '$') */
    SPX_SERIAL_MODE_ADSB = 2,	/* ADS-B messages from RadarGadget receiver */
    SPX_SERIAL_MODE_AIS = 3,	/* AIS mesgs, like NMEA but start with '!' */
    SPX_SERIAL_MODE_HDLC_IFF =4,/* HDLC-IFF target report messages */
    SPX_SERIAL_MODE_OUMP = 5,	/* Own Unit Management Protocol */
    SPX_SERIAL_MODE_ASTERIX = 6,/* Asterix packets */
    SPX_SERIAL_MODE_TPN24 = 7	/* Specific sub-type of HDLC_IFF */

};

/* Supported access modes */
enum SPxSerialAccess_t
{
    SPX_SERIAL_ACCESS_NONE = 0x0,
    SPX_SERIAL_ACCESS_READ = 0x1,
    SPX_SERIAL_ACCESS_WRITE = 0x2,
    SPX_SERIAL_ACCESS_READ_WRITE = SPX_SERIAL_ACCESS_READ | SPX_SERIAL_ACCESS_WRITE
};

/* Supported parity modes */
enum SPxSerialParity_t
{
    SPX_SERIAL_PARITY_NONE = 0x0,
    SPX_SERIAL_PARITY_EVEN = 0x1,
    SPX_SERIAL_PARITY_ODD = 0x2
};

/* Define the type of the callback handler functions. */
typedef void (*SPxSerialPortFn_t)(SPxSerialPort *,	/* this */
				  void *,		/* userArg */
				  SPxTime_tag *,	/* time/NULL */
				  const unsigned char *,/* payload */
				  unsigned int);	/* numBytes */
typedef unsigned int (*SPxSerialPortRawFn_t)(SPxSerialPort *,	/* this */
					     void *,		/* userArg */
					     SPxTime_tag *,	/* time/NULL */
					     const unsigned char *,/* payload */
					     unsigned int);	/* numBytes */

/*
 * Define our class.
 */
class SPxSerialPort : public SPxPacketDecoder, public SPxSerial
{
public:

    /*
     * Public functions.
     */

    explicit SPxSerialPort(SPxSerialMode_t mode=SPX_SERIAL_MODE_RAW);
    explicit SPxSerialPort(UINT32 packetType);
    virtual ~SPxSerialPort(void);

    /* Session strings. */
    virtual SPxErrorCode SetSessionStrings(const char *openString, 
                                           const char *closeString);

    /* Configure, open and close serial port input. */
    SPxErrorCode SetSerialPort(const char *name=NULL,
                               unsigned int baud=0,
                               int openPort=TRUE,
                               SPxSerialAccess_t access=SPX_SERIAL_ACCESS_READ_WRITE,
                               SPxSerialParity_t parity=SPX_SERIAL_PARITY_NONE,
                               unsigned int rawMode=0);
    SPxErrorCode OpenSerialPort(SPxSerialAccess_t access=SPX_SERIAL_ACCESS_READ_WRITE);
    SPxErrorCode CloseSerialPort(void);
    int IsOpen(void) const { return m_serialPortOpen; }
    virtual const char *GetSerialName(void) const { return m_serialPortName; }
    virtual unsigned int GetSerialBaud(void) const {return m_serialBaud; }
    virtual SPxSerialParity_t GetSerialParity(void) const { return m_parity; }
    virtual SPxSerialAccess_t GetSerialAccess(void) const { return m_access; }

    /* Shutdown ready for application restart. */
    virtual SPxErrorCode PrepareForRestart(void);

    /* Receive and handle */
    virtual SPxErrorCode StartHandlerThread(void);
    virtual SPxErrorCode StopHandlerThread(void);
    virtual SPxErrorCode PollSerialPort(void);
    virtual SPxErrorCode SetHandler(SPxSerialPortFn_t fn, void *userArg);
    virtual SPxErrorCode SetRawHandler(SPxSerialPortRawFn_t fn, void *userArg);

    /* Send. */
    int WriteData(const unsigned char *data, unsigned int dataLenBytes,
	int mutexAlreadyLocked=FALSE);
    int ReadData(unsigned char *buf, unsigned int bufSize);

    /* Generic parameter assignment. */
    virtual int SetParameter(char *parameterName, char *parameterValue);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);

    /*
     * Functions used to implement the SPxSerial interface.
     */
    virtual SPxErrorCode SetSerialName(const char *name);
    virtual SPxErrorCode SetSerialBaud(unsigned int baud);
    virtual SPxErrorCode SetSerialParity(SPxSerialParity_t parity);

protected:
    /* These functions are protected so they can be overridden for testing. */
    virtual SPxErrorCode OpenSerialPortInternal(const char *name, SPxSerialAccess_t access);
    virtual SPxErrorCode SetupSerialPortInternal(void);
    virtual SPxErrorCode CloseSerialPortInternal(void);
    virtual int WriteDataInternal(const unsigned char *data, unsigned int dataLenBytes);
    virtual int ReadDataInternal(unsigned char *buf, unsigned int bufSize);

private:

    /*
     * Private variables.
     */
    struct impl;
    SPxAutoPtr<impl> m_p;

    SPxCriticalSection m_mutex;         /* Mutex */
    UINT32 m_packetType;

    /* Serial port input. */
    char *m_serialPortName;             /* Name of serial port */
    int m_serialPortOpen;               /* Is port open? */
#ifdef _WIN32
    HANDLE m_serialPort;                /* Handle of serial port */
#else
    int m_serialPort;                   /* FD of serial port */
#endif
    unsigned int m_serialBaud;		/* Serial connection rate. */
    SPxSerialAccess_t m_access;		/* Current serial access mode. */
    SPxSerialParity_t m_parity;		/* Current parity mode. */
    unsigned int m_rawMode;		/* Set serial port to raw mode. */

    /* Worker thread. */
    SPxThread *m_thread;                /* Thread object handle */

    /* Buffer. */
    unsigned char *m_buf;               /* Buffer for reading data */
    unsigned int m_bufSize;             /* Size of buffer in bytes */

    /* Session strings. */
    char *m_openString;			/* String to send after opening serial port. */
    char *m_closeString;		/* String to send before closign serial port. */

    /* Callback function */
    SPxSerialPortFn_t m_fn;             /* Callback function */
    void *m_fnUserArg;                  /* Callback function arg */
    SPxSerialPortRawFn_t m_rawFn;	/* Raw callback function */
    void *m_rawFnUserArg;		/* Raw callback function arg */

    /*
     * Private functions.
     */
    void Init(UINT32 packetType);
    void *HandlerThreadFn(SPxThread *thread);

    /*
     * Private static functions.
     */

    static void *HandlerThreadWrapper(SPxThread *thread);
    static void HandleData(SPxPacketDecoder *decoder,
			   void *userArg,
			   UINT32 packetType,
			   struct sockaddr_in *fromAddr,
			   struct SPxTime_tag *timePtr,
			   const unsigned char *payload,
			   unsigned int numBytes);
    static void HandleDataRawWrapper(SPxPacketDecoder *decoder,
				     void *userArg,
				     UINT32 packetType,
				     struct sockaddr_in *fromAddr,
				     struct SPxTime_tag *timePtr,
				     const unsigned char *payload,
				     unsigned int numBytes);
    void HandleDataRaw(struct sockaddr_in *fromAddr,
		       struct SPxTime_tag *timePtr,
		       const unsigned char *payload,
		       unsigned int numBytes);

}; /* SPxSerialPort */

#endif /* _SPX_SERIAL_PORT_H */

/*********************************************************************
 *
 * End of file
 *
 **********************************************************************/

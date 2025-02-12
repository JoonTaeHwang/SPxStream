/*********************************************************************
 *
 * (c) Copyright 2018, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   Class for decoding location information from an SA Flyer drone.
 *
 *
 * Revision Control:
 *  14/07/18 v1.1    AJH     Initial version.
 *
 * Previous Changes:
  **********************************************************************/

/* SPx headers. */
#include "SPxLibData/SPxSerialPort.h"
#include "SPxLibNet/SPxADSBDecoder.h"
#include "SPxLibNet/SPxADSBTrack.h"

class SPxDroneDecoder : public SPxObj
{
public:
    /* Constructor and destructor. */
    SPxDroneDecoder(SPxADSBDecoder *adsbDecoder);
    ~SPxDroneDecoder(void);

    /* Public methods. */

    /* Generic parameter assignment. */
    int SetParameter(char *parameterName, char *parameterValue);
    int GetParameter(char *parameterName, char *valueBuf, int bufLen);

    /* Static public functions for log file support. */
    static void SetLogFile(FILE *logFile) { m_logFile = logFile; }
    static FILE *GetLogFile(void) { return( m_logFile ); }

private:
    /* The serial port used for receiving reports. */
    SPxSerialPort *m_serialPort;		

    /* ADS-B decoder for injecting new track reports. */
    SPxADSBDecoder *m_adsbDecoder;

    /* Static log file debug support. */
    static FILE *m_logFile;			/* Destination for msgs */

    /* Private functions. */
    unsigned int handleInput(const unsigned char *payload, unsigned int numBytes);
    static unsigned int handleInputWrapper(SPxSerialPort *port, void *userArg,
	SPxTime_tag *timePtr, const unsigned char *payload, unsigned int numBytes);
};

/*********************************************************************
 *
 * End of file
 *
 **********************************************************************/

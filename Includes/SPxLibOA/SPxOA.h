/*********************************************************************
*
* (c) Copyright 2022, Cambridge Pixel Ltd.
*
* Purpose:
*   Header for SPx Open Access functions.
*
* Revision Control:
*   13/09/22 v1.11   BTB	Add SPxOARadarHeaderToNetworkOrder().
*
* Previous Changes:
*   12/09/22 1.10   BTB	Tidy up and tweak radar data header.
*   05/09/22 1.9    BTB	Fix Windows API for all library variants.
*   02/09/22 1.8    BTB	Fixes to dll linking for functions.
*   26/08/22 1.7    BTB	Make sure interface is presented as C.
*   24/08/22 1.6    BTB	Rework handling of channel index.
*   24/08/22 1.5    BTB	Further expand info in radar data header.
*   18/08/22 1.4    BTB	Use proper radar data header with expanded info.
*   17/08/22 1.3    BTB	Add callback for NMEA data.
*   16/08/22 1.2    BTB	Improve/expand function arguments.
*   10/08/22 1.1    BTB	Initial Version.
**********************************************************************/

#ifndef _SPX_OPEN_ACCESS_H
#define _SPX_OPEN_ACCESS_H

/*
 * Other headers required.
 */
#include "SPxLibUtils/SPxTypes.h"

/*********************************************************************
*
*   Constants
*
**********************************************************************/

#ifdef _WIN32
#ifdef SPXOA_SHARED_LIBRARY
#ifdef SPXOA_LIBRARY_EXPORTS
#define SPXOA_LIBRARY_API __declspec(dllexport)
#else
#define SPXOA_LIBRARY_API __declspec(dllimport)
#endif
#else 
#define SPXOA_LIBRARY_API 
#endif
#else 
#define SPXOA_LIBRARY_API 
#endif

/*********************************************************************
*
*   Type definitions
*
**********************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

    /* Header describing a set of radar data. */
    typedef struct {
        REAL32 azimuthDegs;             /* Azimuth of the radar data. */
        REAL32 platformHeadingDegs;     /* Heading of the platform. */
        REAL32 startRangeMetres;        /* Start range of the data. */
        REAL32 endRangeMetres;          /* End range of the data. */
        UINT32 sampleDataLengthBytes;   /* Length in bytes of the data. */
        UINT16 timeIntervalUsecs;       /* Time interval since last data. */
        UINT16 reserved;
        UINT8 bytesPerSample;           /* Number of bytes per sample of data. */
        UINT8 packing;                  /* Flag indicating the exact packing of the data. */
        UINT8 refMode;                  /* 0/1/2 for North/Heading/Course referenced respectively. */
        UINT8 channelIndex;             /* Index of the channel of the radar data. */
        UINT32 reserved2[5];
    } SPxOARadarDataHeader;

    /* Handler for radar data. */
    typedef void (*SPxOARadarFunc)(void *userArg,
                                   SPxOARadarDataHeader *dataHeader,
                                   const unsigned char *sampleData);

    /* Handler for NMEA data. */
    typedef void (*SPxOANmeaFunc)(void *userArg,
                                  const char *sentence);

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

    /* Install data handlers. */
    extern SPXOA_LIBRARY_API void SPxOASetRadarHandler(SPxOARadarFunc fn, void *userArg);
    extern SPXOA_LIBRARY_API void SPxOASetNmeaHandler(SPxOANmeaFunc fn, void *userArg);
    /* Read a CPR file. */
    extern SPXOA_LIBRARY_API int SPxOAReadFile(const char *filename,
                                               int startTimeSecs = 0,
                                               int durationSecs = -1);
    /* Convert a radar header to network byte order. */
    extern SPXOA_LIBRARY_API void SPxOARadarHeaderToNetworkOrder(SPxOARadarDataHeader *header);

#ifdef __cplusplus
}
#endif

#endif /* _SPX_OPEN_ACCESS_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/

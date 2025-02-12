/*********************************************************************
*
* (c) Copyright 2021 - 2022, Cambridge Pixel Ltd.
*
* Purpose:
*	Used to implement radar and track distribution over a
*	web interface.
*
* Revision Control:
*   14/10/22 v1.12   AGC	Add errCode to getHttpExt().
*
* Previous Changes:
*   22/07/22 1.11   BTB	Send radar video asynchronously.
*   05/04/22 1.10   BTB	Add asynchronous track reporting.
*   09/02/22 1.9    BTB	Throttle scan conversion when not required.
*   24/11/21 1.8    SP 	Add support for multiple platforms.
*   01/09/21 1.7    AGC	Use SPxWebAreaEventReporter.
*   18/08/21 1.6    AGC	Initial area support.
*   12/07/21 1.5    BTB	Add proper Linux support.
*   21/06/21 1.4    AGC	Add encoding to getHttpExt().
*   15/06/21 1.3    BTB	Support distribution of radar video.
*   09/06/21 1.2    AGC	Overload getHttpExt() instead of getHttp().
*   27/05/21 1.1    BTB	Initial Version.
**********************************************************************/

#ifndef _SPX_WEB_INTERFACE_RADAR_H
#define _SPX_WEB_INTERFACE_RADAR_H

/* Includes. */
#include "SPxLibNet/SPxWebInterface.h"

/* Forward declarations. */
class SPxImage;
class SPxNavData;
class SPxAreaObj;
class SPxScDestBitmap;
class SPxViewControl;
class SPxUniTrackDatabase;
class SPxWebAreaEventReporter;
class SPxWebTrackReporter;
class SPxTimer;

class SPxWebInterfaceRadar : public SPxWebInterface
{
public:
    /*
     * Public functions.
     */

    /* Construction and destruction. */
    SPxWebInterfaceRadar(unsigned int numRadarVideos,
                         unsigned int widthPixels,
                         unsigned int heightPixels);
    virtual ~SPxWebInterfaceRadar(void);

    /* Setup. */
    SPxErrorCode SetTrackDB(SPxUniTrackDatabase *trackDB);
    SPxErrorCode SetNavData(SPxNavData *navData, unsigned int platformIndex=0);
    SPxErrorCode SetAreaDB(SPxAreaObj *areaDB);
    SPxErrorCode SetPlatformLatLong(double latDegs, 
                                    double longDegs, 
                                    unsigned int platformIndex=0);
    SPxErrorCode GetPlatformLatLong(double *latDegs, 
                                    double *longDegs, 
                                    unsigned int platformIndex=0);
    SPxErrorCode AddRadarVideo(SPxRunProcess *prevProc,
                               double offsetXMetres,
                               double offsetYMetres,
                               unsigned int *indexRtn=NULL,
                               unsigned int radarIndex=0,
                               unsigned int platformIndex=0);
    SPxErrorCode RequestVideo(const char *url);

    /* Radar Video Configuration. */
    unsigned int GetNumVideos(void);
    SPxErrorCode SetVideoName(unsigned int index, const char *name);
    const char *GetVideoName(unsigned int index);
    SPxErrorCode SetVideoEnabled(unsigned int index, int enable);
    int GetVideoEnabled(unsigned int index) const;
    SPxErrorCode SetRGB(unsigned int index, unsigned int rgb);
    unsigned int GetRGB(unsigned int index) const;
    SPxErrorCode SetFadeType(unsigned int index, unsigned int type);
    unsigned int GetFadeType(unsigned int index) const;
    SPxErrorCode SetFadeRate(unsigned int index, unsigned int rate);
    unsigned int GetFadeRate(unsigned int index) const;
    SPxErrorCode SetBrightness(unsigned int index, unsigned int bright);
    unsigned int GetBrightness(unsigned int index) const;

    /* Generic parameter assignment. */
    virtual int SetParameter(char *name, char *value);
    virtual int GetParameter(char *name, char *valueBuf, int bufLen);

protected:
    /*
     * Protected functions.
     */

    /* Override base class functions. */
    virtual int getHttpExt(const char *urlExt,
			   const char *urlExtWithArgs,
			   const char *sessionCookie,
			   char *buf,
			   unsigned int bufSize,
			   char *mime,
			   unsigned int mimeSize,
			   char *encoding,
			   unsigned int encodingSize,
			   void *sessionPtr,
			   unsigned int *errCode);

private:
    /*
     * Private types.
     */

    struct impl;

    /*
     * Private variables.
     */

    SPxAutoPtr<impl> m_p;
    SPxUniTrackDatabase *m_uniTrackDB;
    SPxViewControl *m_view;
    unsigned int m_numRadarVideos;
    unsigned int m_widthPixels;
    unsigned int m_heightPixels;
    unsigned int m_disableSecs;
    int m_radarDisabled;
    int m_haveRequest;
    UINT32 m_radarLastRequested;
    unsigned int radarReporterID;
    SPxTimer m_timer;
    SPxAutoPtr<SPxWebAreaEventReporter> m_areaEventRptr;
    SPxAutoPtr<SPxWebTrackReporter> m_webTrackReporter;

    /*
     * Private functions.
     */
    SPxErrorCode parseOptions(const char *urlExt, const char *optName,
                              unsigned int numVals,
                              double *val1Ptr, double *val2Ptr=NULL);
    SPxErrorCode parseOptions(const char *urlExt, const char *optName,
                              unsigned int numVals,
                              unsigned int *val1Ptr, unsigned int *val2Ptr=NULL);
    SPxNavData *getNavData(unsigned int platformIndex);
    void sendRadarVideoImage(void);
    void handleTimer(void);
};

#endif /* _SPX_WEB_INTERFACE_RADAR_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/

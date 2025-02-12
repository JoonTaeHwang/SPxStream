/*********************************************************************
*
* (c) Copyright 2022, Cambridge Pixel Ltd.
*
* Purpose:
*	Used to report A-Scan data asynchronously through a web interface.
*
* Revision Control:
*   05/07/22 v1.1    BTB	Initial Version.
*
* Previous Changes:
**********************************************************************/

#ifndef _SPX_WEB_ASCAN_REPORTER_H
#define _SPX_WEB_ASCAN_REPORTER_H

/* Includes. */
#include "SPxLibData/SPxAscanIface.h"
#include "SPxLibUtils/SPxObj.h"

/* Forward declarations. */
class SPxPIM;
class SPxRunProcess;
class SPxWebInterface;

class SPxWebAscanReporter: public SPxObj, public SPxAscanIface
{
public:
    /*
     * Public functions.
     */

    /* Construction and destruction. */
    explicit SPxWebAscanReporter(SPxRunProcess *prevProc,
                                 SPxPIM *pim,
                                 SPxWebInterface *webIf);
    virtual ~SPxWebAscanReporter(void);

    void SetThresholdProcess(SPxRunProcess *rp);

    SPxErrorCode KeepAlive(void);

    SPxErrorCode SetReportThreshold(int report);
    int GetReportThreshold(void) { return m_reportThreshold; }

    SPxErrorCode SetSampleRateMs(unsigned int rateMs);
    unsigned int GetSampleRateMs(void) { return m_sampleRateMs; }

    SPxErrorCode SetNumSamplesRange(unsigned int samples);
    unsigned int GetNumSamplesRange(void) { return m_numSamplesRange; }

    /* Inherited from SPxAscanIface. */
    void NewReturn(SPxReturn *newReturn);
    void NewAux(SPxReturn *newReturn, unsigned int index=0);

    void NewThresholdData(unsigned char *data,
                          unsigned int sizeBytes,
                          unsigned int azi,
                          int multiByte);

    /* Generic parameter assignment. */
    virtual int SetParameter(char *name, char *value);
    virtual int GetParameter(char *name, char *valueBuf, int bufLen);

protected:
    /*
     * Protected functions.
     */

private:
    /*
     * Private variables.
     */
    struct impl;
    SPxAutoPtr<impl> m_p;

    SPxPIM *m_pim;

    SPxWebInterface *m_webIf;
    unsigned int m_sampleReporterID;
    unsigned int m_threshReporterID;

    SPxRunProcess *m_ascanProc;
    SPxRunProcess *m_proThresh;

    int m_reportThreshold;

    unsigned int m_sampleRateMs;
    unsigned int m_numSamplesRange;

    double m_startRangeMetres;
    double m_endRangeMetres;
    unsigned int m_nominalLengthSamples;

    /*
     * Private functions.
     */
    void checkKeepAlive(void);

    SPxErrorCode sendSampleData(void);
    SPxErrorCode sendThresholdData(void);

    static void thresholdDataHandler(void *userArg,
                                     SPxRunProcess *rp,
                                     unsigned int azi,
                                     SPxProcDataType_t type,
                                     UCHAR packing,
                                     void *data,
                                     unsigned int sizeBytes);

};

#endif /* _SPX_WEB_ASCAN_REPORTER_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/

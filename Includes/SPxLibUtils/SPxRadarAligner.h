/*********************************************************************
 *
 * (c) Copyright 2021 - 2022, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   Header for SPxRadarAligner class which is used to calculate
 *   the offset required to align two streams of radar video.
 *
 * Revision Control:
 *   02/02/22 v1.10  SP 	Change to how points are reported to webIf.
 *
 * Previous Changes:
 *   28/01/22 1.9   SP 	Changes to statistics.
 *   27/01/22 1.8   SP 	Report statistics.
 *   19/11/21 1.7   SP 	Further improvements to confidence calculation.
 *   18/11/21 1.6   SP 	Use standard deviation to calculate confidence.
 *   11/11/21 1.5   SP 	Support confidence value.
 *   03/11/21 1.4   SP 	Support web interface.
 *   01/11/21 1.3   SP 	Revert last change.
 *   01/11/21 1.2   SP 	Fix build issue with older compilers.
 *   29/10/21 1.1   SP 	Initial version.
 *
 *********************************************************************/

#ifndef _SPX_RADAR_ALIGNER_H
#define _SPX_RADAR_ALIGNER_H

/* Other classes that we need. */
#include "SPxLibUtils/SPxObj.h"
#include "SPxLibUtils/SPxAutoPtr.h"
#include "SPxLibUtils/SPxTimer.h"
#include "SPxLibUtils/SPxViewControl.h" /* For SPxLatLong_t. */

/*********************************************************************
 *
 *   Constants
 *
 *********************************************************************/

/* Parameters. */

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

/* Forward declare any classes required. */
class SPxPIM;
class SPxWebInterface;

/*
 * Define our class.
 */
class SPxRadarAligner : public SPxObj
{
public:

    /*
     * Public variables.
     */
    static const unsigned int MAX_NUM_POINTS = 360;

    /*
     * Public types.
     */

    /* Feature point. */
    struct Point_t
    {
        double rangeMetres;     /* Point range in metres. */
        double aziDegs;         /* Point azimuth in degrees. */
        double xMetres;         /* X position in metres (+ve east). */
        double yMetres;         /* Y position in metres (+ve north). */
        int isPaired;           /* Is this point paired? */
        double pairXMetres;     /* X position in metres (+ve east) of paired point. */
        double pairYMetres;     /* Y position in metres (+ve north) of paired point. */
    };

    /* Feature set. */
    struct FeatureSet_t
    {
        SPxPIM *pim;                    /* PIM for this feature set. */
        Point_t points[MAX_NUM_POINTS]; /* Feature points. */
        unsigned int numPoints;         /* Number of feature points. */
    };

    /* Feature set index. */
    enum FeatureSetIndex_t
    {
        FEATURE_SET_1 = 1,
        FEATURE_SET_2 = 2
    };

    /* Statistics. */
    struct Stats_t
    {
        unsigned int fs1NumPoints;          /* Number of points in FS1. */
        unsigned int fs2NumPoints;          /* Number of points in FS2. */
        unsigned int numPairsAvail;         /* Number of pairs available. */
        unsigned int numPairsUsed;          /* Number of pairs used. */
        double stdDevDeltaXMetres;          /* SD of X delta between points used. */
        double stdDevDeltaYMetres;          /* SD of Y delta between points used. */
        double stdDevConfidence;            /* Confidence based on SD. */
        double numPairsUsedConfidence;      /* Confidence based on number of pairs used. */
        double minNumPairsConfidence;       /* Confidence based on min pairs required. */
        unsigned int numIterations;         /* Number of iterations to refine offset. */
    };

    /*
     * Public functions.
     */

    /* Constructor and destructor. */
    explicit SPxRadarAligner(SPxPIM *pim1, SPxPIM *pim2);
    virtual ~SPxRadarAligner(void);

    /* General. */
    SPxErrorCode SetRadarError(double metres);
    double GetRadarError(void) { return m_radarErrorMetres; }
    SPxErrorCode SetNumFeaturePoints(unsigned int numPoints);
    unsigned int GetNumFeaturePoints(void) { return m_numFeaturePoints; }
    SPxErrorCode SetMinFeatureEntent(double metres);
    double GetMinFeatureEntent(void) { return m_minFeatureExtentMetres; }
    SPxErrorCode IterateFeaturePoints(FeatureSetIndex_t fsi, 
                                      SPxCallbackListFn_t fn, 
                                      void *userArg);
    SPxErrorCode CalculateOffset(double *xMetresRtn, 
                                 double *yMetresRtn, 
                                 double *confidenceRtn,
                                 Stats_t *statsRtn=NULL);

    /* Web interface support. */
    SPxErrorCode SetWebInterface(SPxWebInterface *webIf);
    SPxErrorCode UpdateWebInterface(const SPxLatLong_t *radarLL, int includePairs);

    /*
     * Public variables.
     */

protected:

    /*
     * Private types.
     */

    /*
     * Protected variables.
     */

    /*
     * Protected functions.
     */

    /* Generic parameter assignment. */
    virtual int SetParameter(char *parameterName, char *parameterValue);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);

private:

    /*
     * Private types.
     */

    /* Pairs of points. */
    struct PointPair_t
    {
        double distMetres;              /* Distance between points. */
        double deltaXMetres;            /* X delta between points. */
        double deltaYMetres;            /* Y delta between points. */
        unsigned int fs1PointIndex;     /* Index of FS1 point. */
        unsigned int fs2PointIndex;     /* Index of FS2 point. */
    };

    /*
     * Private variables.
     */

    /* Private member variables. */
    struct impl;
    SPxAutoPtr<impl> m_p;
    SPxCriticalSection m_mutex;             /* Mutex for object. */
    double m_radarErrorMetres;              /* Expected radar error in metres. */
    unsigned int m_numFeaturePoints;        /* Number of points to harvest. */
    double m_minFeatureExtentMetres;        /* Minimum feature extent in metres. */
    FeatureSet_t m_fs1;                     /* PIM1 feature set. */
    FeatureSet_t m_fs2;                     /* PIM2 feature set. */
    SPxWebInterface *m_webIf;               /* Web interface. */
    unsigned int m_webFsKey;                /* Key used to send to web interface. */

    /*
     * Private functions.
     */

    FeatureSet_t *getFeatureSet(FeatureSetIndex_t fsi, SPxErrorCode *err);
    FeatureSet_t *getFeatureSet(SPxPIM *pim, SPxErrorCode *err);

    SPxErrorCode updateFeaturePoints(FeatureSet_t *fs);

    SPxErrorCode calcDelta(double xMetres, 
                           double yMetres,
                           double *deltaXMetresRtn, 
                           double *deltaYMetresRtn,
                           double *confidenceRtn,
                           Stats_t *statsRtn);

    SPxErrorCode calcOffset(double *xMetresRtn,  
                            double *yMetresRtn,
                            double *confidenceRtn,
                            Stats_t *statsRtn);

    SPxErrorCode reportFeaturePoints(FeatureSet_t *fs, 
                                     const SPxLatLong_t *radarLL,
                                     int includePairs);

    /*
     * Private static functions.
     */

    static bool comparePairs(const PointPair_t &p1, const PointPair_t &p2);
    static bool comparePairPtrs(const PointPair_t *p1, const PointPair_t *p2);

    static int webIfDeleted(void *invokingObject,
                            void *userObject,
                            void *arg);

}; /* SPxRadarAligner */

/*********************************************************************
 *
 *   Function prototypes
 *
 *********************************************************************/

#endif /* SPX_RADAR_ALIGNER_H */

/*********************************************************************
 *
 * End of file
 *
 *********************************************************************/

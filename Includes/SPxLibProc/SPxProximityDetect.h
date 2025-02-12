/*********************************************************************
*
* (c) Copyright 2012, 2013, 2018, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for the SPxProximityDetector class.
*
* Revision Control:
*   15/06/18 v1.5    AGC	Support sector callbacks.
*
* Previous Changes:
*   04/06/18 1.4    AGC	Support plot callbacks.
*   04/10/13 1.3    AGC	Simplify headers.
*   31/05/12 1.2    REW	Make base class functions protected.
*			Change callback type and report structure.
*   29/05/12 1.1    REW	Initial Version.
**********************************************************************/

#ifndef _SPX_PROXIMITYDETECT_H
#define _SPX_PROXIMITYDETECT_H

/*
 * Other headers required.
 */
/* We need our base class header. */
#include "SPxLibProc/SPxProcSpanDetect.h"

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

/* Forward-declare the classes we use. */
class SPxCallbackList;
class SPxProximityDetector;
struct SPxReturnHeader_tag;
class SPxRunProcess;

/*
 * Public type for proximity reports (not the same as that used internally).
 */
typedef struct SPxProximityReport_tag {
    /* Position. */
    double rangeMetres;

    /* Quality information. */
    double sizeMetres;
    double intensityPercent;
} SPxProximityReport_t;


/* Type of callback function for proximity detections. */
typedef SPxErrorCode (*SPxProximityDetectorFn_t)(
					SPxProximityDetector *detector,
					void *userObject,
					double azimuthDegrees,
					const SPxProximityReport_t *reports,
					unsigned int numReports);


/*
 * Proximity-detector class, derived from a span detector.
 */
class SPxProximityDetector :public SPxProSpanDetector
{
public:
    /*
     * Public functions.
     */
    /* Constructor/destructor. */
    SPxProximityDetector(void);
    virtual ~SPxProximityDetector();

    /* Callback support. */
    SPxErrorCode SetCallback(SPxProximityDetectorFn_t fn, void *userObject)
    {
	m_callbackObj = userObject;
	m_callbackFn = fn;
	return(SPX_NO_ERROR);
    }
    SPxErrorCode AddPlotCallback(SPxCallbackListFn_t fn, void *userObject);
    SPxErrorCode RemovePlotCallback(SPxCallbackListFn_t fn, void *userObject);
    SPxErrorCode AddSectorCallback(SPxCallbackListFn_t fn, void *userObject);
    SPxErrorCode RemoveSectorCallback(SPxCallbackListFn_t fn, void *userObject);

protected:
    /* Override the base class functions we want. */
    void ReturnStarted(unsigned int azi16Bit,
		       SPxReturnHeader_tag *header,
		       SPxRunProcess *rp,
		       double aziWidthDegs);
    void NewSpan(const SPxSpan_t *span);
    void ReturnFinished(unsigned int azi16Bit);

private:
    /*
     * Private functions.
     */

    /*
     * Private variables.
     */
    /* Current return header and run-process etc. */
    SPxReturnHeader_tag *m_currentHeader;
    SPxRunProcess *m_currentRP;

    /* Callback support. */
    SPxProximityDetectorFn_t m_callbackFn;
    void *m_callbackObj;
    SPxAutoPtr<SPxCallbackList> m_plotCallbackList;
    SPxAutoPtr<SPxCallbackList> m_sectorCallbackList;

    /* Detections. */
    SPxProximityReport_t *m_reports;
    unsigned int m_reportsAllocated;
    unsigned int m_reportsPresent;

    /* Convenience values. */
    double m_metresPerSample;
    double m_aziDegs;
    double m_minSizeMetres;
    double m_maxSizeMetres;
    unsigned int m_lastSector;
};

/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* _SPX_PROXIMITYDETECT_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/

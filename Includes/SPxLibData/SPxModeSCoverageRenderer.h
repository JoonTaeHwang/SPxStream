/*********************************************************************
*
* (c) Copyright 2020, Cambridge Pixel Ltd.
*
*
* Purpose:
*	Header for SPxModeSCoverageRenderer - base class which
*	supports the rendering of coverage maps for Mode S stations.
*
* Revision Control:
*   15/10/20 v1.2    AGC	Restrict coverage to single node.
*
* Previous Changes:
*   17/09/20 1.1    AGC	Initial version.
**********************************************************************/
#ifndef _SPX_MODE_S_COVERAGE_RENDERER_H
#define _SPX_MODE_S_COVERAGE_RENDERER_H

/* For SPxAutoPtr. */
#include "SPxLibUtils/SPxAutoPtr.h"

/* For base class. */
#include "SPxLibUtils/SPxRenderer.h"

/* Forward declarations. */
class SPxModeSCoverageMap;

/* Define our class */
class SPxModeSCoverageRenderer : virtual public SPxRenderer
{
public:
    /* Constructor and destructor. */
    explicit SPxModeSCoverageRenderer(SPxModeSCoverageMap *map);
    virtual ~SPxModeSCoverageRenderer(void);

    SPxErrorCode SetLocalNode(UINT8 sac, UINT8 sic);
    SPxErrorCode GetLocalNode(UINT8 *sac, UINT8 *sic) const;

    SPxErrorCode SetAltitudeMetres(double altitudeMetres);
    SPxErrorCode GetAltitudeMetres(double *altitudeMetres) const;

    SPxErrorCode SetClusterState(UINT8 clusterState);
    SPxErrorCode GetClusterState(UINT8 *clusterState) const;

    SPxErrorCode SetShowSurveillance(int show);
    SPxErrorCode SetShowDatalink(int show);
    SPxErrorCode SetShowLockout(int show);
    int IsShowingSurveillance(void) const;
    int IsShowingDatalink(void) const;
    int IsShowingLockout(void) const;

    SPxErrorCode SetSurveillanceColour(UINT32 colour);
    SPxErrorCode GetSurveillanceColour(UINT32 *colour) const;
    SPxErrorCode SetDatalinkColour(UINT32 colour);
    SPxErrorCode GetDatalinkColour(UINT32 *colour) const;
    SPxErrorCode SetLockoutColour(UINT32 colour);
    SPxErrorCode GetLockoutColour(UINT32 *colour) const;

    int IsRedrawRequired(void) const;

    virtual int SetParameter(char *parameterName, char *parameterValue);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);

private:
    /* Private variables. */
    struct impl;
    SPxAutoPtr<impl> m_p;

    /* Private functions. */
    virtual SPxErrorCode renderAll(void);
    static SPxErrorCode drawRectSurveillance(const SPxLatLong_tag *topLeft,
                                             const SPxLatLong_tag *bottomRight,
                                             UINT8 stateMask,
                                             void *userArg);
    static SPxErrorCode drawRectDatalink(const SPxLatLong_tag *topLeft,
                                         const SPxLatLong_tag *bottomRight,
                                         UINT8 stateMask,
                                         void *userArg);
    static SPxErrorCode drawRectLockout(const SPxLatLong_tag *topLeft,
                                        const SPxLatLong_tag *bottomRight,
                                        UINT8 stateMask,
                                        void *userArg);
    SPxErrorCode drawRect(const SPxLatLong_tag *topLeft,
                          const SPxLatLong_tag *bottomRight,
                          UINT32 col);
    static int mapDeletedFn(void *invokingObject,
			    void *userObject,
			    void *arg);

}; /* SPxModeSCoverageRenderer */

#endif /* _SPX_MODE_S_COVERAGE_RENDERER_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/

/*********************************************************************
*
* (c) Copyright 2022, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxSimPlatNav object which is a simulated source
*	of navigation data for a simulated platform.
*
*
* Revision Control:
*   28/06/22 v1.2    AGC	Support date/time nav data output.
*
* Previous Changes:
*   21/06/22 1.1    AGC	Initial version.
**********************************************************************/
#ifndef SPX_SIM_PLAT_NAV_H_
#define SPX_SIM_PLAT_NAV_H_

/* For SPxAutoPtr. */
#include "SPxLibUtils/SPxAutoPtr.h"

/* For base class. */
#include "SPxLibUtils/SPxObj.h"

/* Forward declarations. */
class SPxSimPlatform;
class SPxNavData;

/* Define our class. */
class SPxSimPlatNav : public SPxObj
{
public:
    /* Public functions. */
    explicit SPxSimPlatNav(SPxSimPlatform *platform,
			   unsigned int index);
    virtual ~SPxSimPlatNav(void);

    /* Configuration. */
    SPxErrorCode SetNavDataOutputIntervalMS(unsigned int ms);
    SPxErrorCode GetNavDataOutputIntervalMS(unsigned int *ms) const;
    SPxErrorCode SetHeadingOutputIntervalMS(unsigned int ms);
    SPxErrorCode GetHeadingOutputIntervalMS(unsigned int *ms) const;
    SPxErrorCode SetRateOfTurnOutputIntervalMS(unsigned int ms);
    SPxErrorCode GetRateOfTurnOutputIntervalMS(unsigned int *ms) const;
    SPxErrorCode SetPitchRollOutputIntervalMS(unsigned int ms);
    SPxErrorCode GetPitchRollOutputIntervalMS(unsigned int *ms) const;
    SPxErrorCode SetDateTimeOutputIntervalMS(unsigned int ms);
    SPxErrorCode GetDateTimeOutputIntervalMS(unsigned int *ms) const;
    SPxErrorCode SetGenNavDataFixChar(int genNavDataFixChar);
    int IsGenNavDataFixChar(void) const;
    SPxErrorCode SetGenNavDataSternSpeeds(int genNavDataSternSpeeds);
    int IsGenNavDataSternSpeeds(void) const;
    SPxErrorCode SetNavDataMessages(UINT32 navDataMessages);
    SPxErrorCode GetNavDataMessages(UINT32 *navDataMessages) const;
    SPxErrorCode SetNavDataDelayOutputMS(UINT32 navDataDelayOutputMS);
    SPxErrorCode GetNavDataDelayOutputMS(UINT32 *navDataDelayOutputMS) const;
    SPxErrorCode SetSFI(const char *sfi);
    SPxErrorCode GetSFI(char *buf, unsigned int bufLen) const;

    /* Nav data output functions (called by SPxSimPlatform). */
    SPxErrorCode SetPlatformSource(int isFromNavData, int enableDest);
    SPxErrorCode OutputNavData(double latDegs, double longDegs,
			       double altitudeWithNoise,
			       double speedWithNoise,
			       double course,
			       double courseWithNoise,
			       double headingWithNoise,
			       double headingWithCourseNoise,
			       double magVarDegs,
			       double driftMps,
			       double setDegs,
			       double lengthMetres,
			       double accDegsPerSec);
    SPxErrorCode OutputHeading(double headingWithNoiseDegs,
			       double magVarDegs);
    SPxErrorCode OutputRateOfTurn(double rateOfTurnDegsPerSec);
    SPxErrorCode OutputPitchRoll(double pitchDegs, double rollDegs,
				 double headingWithNoiseDegs);
    SPxErrorCode OutputDateTime(void);
    UINT32 GetReqNavData(UINT32 now) const;
    UINT32 GetReqHeading(UINT32 now) const;
    UINT32 GetReqRateOfTurn(UINT32 now) const;
    UINT32 GetReqPitchRoll(UINT32 now) const;
    UINT32 GetReqDateTime(UINT32 now) const;

    /* Get internal objects. */
    SPxErrorCode GetPlatform(const SPxSimPlatform **platform) const;
    SPxErrorCode GetPlatform(SPxSimPlatform **platform);
    SPxErrorCode GetNavData(const SPxNavData **navData) const;
    SPxErrorCode GetNavData(SPxNavData **navData);
    
    /* Set/get parameter interface. */
    virtual int SetParameter(char *parameterName, char *parameterValue);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);

protected:
    /* Constructor for testing. */
    explicit SPxSimPlatNav(SPxSimPlatform *platform,
			   unsigned int index,
			   SPxNavData *navData);

private:
    /* Private variables. */
    struct impl;
    SPxAutoPtr<impl> m_p;
 
    /* Private functions. */
    void init(SPxSimPlatform *platform);
    SPxErrorCode queueNavDataSentence(const char *sentence);
    SPxErrorCode outputNavDataSentence(const char *sentence);
    void checkDelayedNavData(void);
    static int navDataFnWrapper(void *invokingObject,
				void *userObject,
				void *arg);
    int navDataFn(void);
    static int platformDeleted(void *invokingObject, void *userObject, void *arg);

}; /* SPxSimPlatNav. */

#endif /* SPX_SIM_PLAT_NAV_H_ */

/*********************************************************************
*
* End of file
*
**********************************************************************/

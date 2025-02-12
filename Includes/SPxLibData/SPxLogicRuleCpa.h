/*********************************************************************
 *
 * (c) Copyright 2012, Cambridge Pixel Ltd.
 *
 * File: $RCSfile: SPxLogicRuleCpa.h,v $
 * ID: $Id: SPxLogicRuleCpa.h,v 1.1 2012/12/21 16:05:39 rew Exp $
 *
 * Purpose:
 *	Header for SPxLogicRuleCpa class.
 *
 * Revision Control:
 *  21/12/12 v1.1    AGC	Initial Version.
 *
 * Previous Changes:
 **********************************************************************/

#ifndef _SPX_LOGIC_RULE_CPA_H
#define _SPX_LOGIC_RULE_CPA_H

/*
 * Other headers required.
 */

#include "SPxLibUtils/SPxAutoPtr.h"
#include "SPxLibUtils/SPxError.h"
#include "SPxLibData/SPxLogicRule.h"

/*********************************************************************
 *
 *   Type definitions
 *
 **********************************************************************/

/* Forward declarations. */
class SPxLogicGroup;
class SPxNavData;
struct SPxPacketAlertLogic_tag;
struct SPxLogicRuleCpaPriv;
struct SPxLogicKey;
struct SPxLogicTarget;

/*
 * Define our class.
 */
class SPxLogicRuleCpa : public SPxLogicRule
{
public:
    /* Public functions. */
    explicit SPxLogicRuleCpa(SPxLogicGroup *group, int active=TRUE);
    virtual ~SPxLogicRuleCpa(void);

    SPxErrorCode SetMinDistanceMetres(double minDistMetres);
    double GetMinDistanceMetres(void) const;
    SPxErrorCode SetMinTimeSecs(double minTimeSecs);
    double GetMinTimeSecs(void) const;

    /* Parameters set/get */
    virtual int SetParameter(char *parameterName, char *parameterValue);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);

private:
    /* Private variables. */
    SPxAutoPtr<SPxLogicRuleCpaPriv> m_p;

    /* Private functions. */
    virtual int Check(SPxPacketAlertLogic_tag *alert, 
	const SPxLogicKey *key, const SPxLogicTarget *target,
	UINT32 flags);
    virtual SPxErrorCode Load(FILE *f);
    virtual SPxErrorCode Save(FILE *f);

}; /* SPxLogicRuleCpa */

#endif /* _SPX_LOGIC_RULE_CPA_H */

/*********************************************************************
 *
 * End of file
 *
 **********************************************************************/

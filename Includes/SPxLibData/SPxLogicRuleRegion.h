/*********************************************************************
 *
 * (c) Copyright 2012, Cambridge Pixel Ltd.
 *
 * File: $RCSfile: SPxLogicRuleRegion.h,v $
 * ID: $Id: SPxLogicRuleRegion.h,v 1.1 2012/12/21 16:05:39 rew Exp $
 *
 * Purpose:
 *	Header for SPxLogicRuleRegion class.
 *
 * Revision Control:
 *  21/12/12 v1.1    AGC	Initial Version.
 *
 * Previous Changes:
 **********************************************************************/

#ifndef _SPX_LOGIC_RULE_REGION_H
#define _SPX_LOGIC_RULE_REGION_H

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
class SPxActiveRegion;
struct SPxPacketAlertLogic_tag;
struct SPxPacketAlertLogicRegion_tag;
struct SPxLogicRuleRegionPriv;
struct SPxLogicTargetProp;

/* Enumeration of region triggers. */
enum SPxLogicRuleRegionType
{
    SPX_LOGIC_RULE_REGION_NONE		= 0x00,
    SPX_LOGIC_RULE_REGION_ENTER		= 0x01,
    SPX_LOGIC_RULE_REGION_LEAVE		= 0x02,
    SPX_LOGIC_RULE_REGION_CREATE	= 0x04,
    SPX_LOGIC_RULE_REGION_LOST		= 0x08,
    SPX_LOGIC_RULE_REGION_IN		= 0x10,
    SPX_LOGIC_RULE_REGION_WAS_FIRST_IN	= 0x20
};

/*
 * Define our class.
 */
class SPxLogicRuleRegion : public SPxLogicRule
{
public:
    /* Public functions. */
    explicit SPxLogicRuleRegion(SPxLogicGroup *group, int active=TRUE);
    virtual ~SPxLogicRuleRegion(void);

    SPxErrorCode SetType(UINT32 type);
    UINT32 GetType(void) const;
    SPxErrorCode SetUserRegion(SPxActiveRegion *region);
    SPxActiveRegion *GetRegion(void);

    /* Parameters set/get */
    virtual int SetParameter(char *parameterName, char *parameterValue);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);

private:
    /* Private variables. */
    SPxAutoPtr<SPxLogicRuleRegionPriv> m_p;

    /* Private functions. */
    virtual int Check(SPxPacketAlertLogic_tag *alert, 
	const SPxLogicKey *key, const SPxLogicTarget *target,
	UINT32 flags);
    virtual SPxErrorCode Load(FILE *f);
    virtual SPxErrorCode Save(FILE *f);
    int getRangeAzi(const SPxLogicTargetProp *prop,
	double *rangeMetres, double *aziDegs);
    void fillRegion(SPxPacketAlertLogicRegion_tag *alertRegion,
	const SPxLogicKey *key,
	const SPxLogicTargetProp *target,
	SPxActiveRegion *region,
	SPxLogicRuleRegionType type);

}; /* SPxLogicRuleRegion */

#endif /* _SPX_LOGIC_RULE_REGION_H */

/*********************************************************************
 *
 * End of file
 *
 **********************************************************************/

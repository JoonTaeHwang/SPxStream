/*********************************************************************
 *
 * (c) Copyright 2012, Cambridge Pixel Ltd.
 *
 * File: $RCSfile: SPxLogicRule.h,v $
 * ID: $Id: SPxLogicRule.h,v 1.2 2013/01/14 12:06:59 rew Exp $
 *
 * Purpose:
 *	Header for SPxLogicRule class.
 *
 * Revision Control:
 *  11/01/13 v1.2    AGC	Simplify Load/Save of common rule commands.
 *
 * Previous Changes:
 *  21/12/12 1.1    AGC	Initial Version.
 **********************************************************************/

#ifndef _SPX_LOGIC_RULE_H
#define _SPX_LOGIC_RULE_H

/*
 * Other headers required.
 */

#include "SPxLibUtils/SPxAutoPtr.h"
#include "SPxLibUtils/SPxError.h"
#include "SPxLibUtils/SPxObj.h"

/*********************************************************************
 *
 *   Type definitions
 *
 **********************************************************************/

/* Forward declarations. */
class SPxLogicGroup;
struct SPxLogicRulePriv;
struct SPxPacketAlertLogic_tag;
struct SPxLogicRuleCpaPriv;
struct SPxLogicKey;
struct SPxLogicTarget;

/*
 * Define our classes.
 */
class SPxLogicRule : public SPxObj
{
public:
    /* Public functions. */
    explicit SPxLogicRule(SPxLogicGroup *group, int active=TRUE);
    virtual ~SPxLogicRule(void);

    SPxErrorCode SetUseAllTargets(int useAllTargets);
    int GetUseAllTargets(void) const;

    /* Parameters set/get */
    virtual int SetParameter(char *parameterName, char *parameterValue);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);

protected:
    /* Protected functions. */
    SPxErrorCode Load(const char *buf);
    virtual SPxErrorCode Load(FILE *f)=0;
    virtual SPxErrorCode Save(FILE *f);
    int GetParam(const char *buf, const char *param);
    int GetParam(const char *buf, const char *param, char *var, unsigned int varLen);
    int GetParam(const char *buf, const char *param, int *var);
    int GetParam(const char *buf, const char *param, unsigned int *var);
    int GetParam(const char *buf, const char *param, float *var);
    int GetParam(const char *buf, const char *param, double *var);

private:
    /* Private variables. */
    SPxAutoPtr<SPxLogicRulePriv> m_p;

    /* Functions called by SPxLogicGroup. */
    virtual int Check(SPxPacketAlertLogic_tag *alert, 
	const SPxLogicKey *key, const SPxLogicTarget *target,
	UINT32 flags)=0;

    friend class SPxLogicGroup;

}; /* SPxLogicRule */

#endif /* _SPX_LOGIC_RULE_H */

/*********************************************************************
 *
 * End of file
 *
 **********************************************************************/

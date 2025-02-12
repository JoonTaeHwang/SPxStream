/*********************************************************************
 *
 * (c) Copyright 2012, Cambridge Pixel Ltd.
 *
 * File: $RCSfile: SPxLogicGroup.h,v $
 * ID: $Id: SPxLogicGroup.h,v 1.1 2012/12/21 16:05:39 rew Exp $
 *
 * Purpose:
 *   Header for SPxLogicGroup class.
 *
 * Revision Control:
 *  21/12/12 v1.1    AGC	Initial Version.
 *
 * Previous Changes:
 **********************************************************************/

#ifndef _SPX_LOGIC_GROUP_H
#define _SPX_LOGIC_GROUP_H

/*
 * Other headers required.
 */

#include "SPxLibUtils/SPxAutoPtr.h"
#include "SPxLibUtils/SPxError.h"
#include "SPxLibData/SPxLogicDB.h"
#include "SPxLibUtils/SPxObj.h"

/*********************************************************************
 *
 *   Type definitions
 *
 **********************************************************************/

/* Forward declarations. */
class SPxLogicDB;
struct SPxLogicTarget;
class SPxLogicRule;
struct SPxLogicGroupPriv;
struct SPxPacketAlertLogic_tag;

/*
 * Define our class.
 */
class SPxLogicGroup : public SPxObj
{
public:
    /* Public functions. */
    explicit SPxLogicGroup(SPxLogicDB *logicDB);
    virtual ~SPxLogicGroup(void);

    /* Parameters set/get */
    virtual int SetParameter(char *parameterName, char *parameterValue);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);

private:
    /* Private variables. */
    SPxAutoPtr<SPxLogicGroupPriv> m_p;

    /* Functions called by SPxLogicDB. */
    int Check(void);
    SPxErrorCode Load(FILE *f);
    SPxErrorCode Save(FILE *f);

    /* Functions called by SPxLogicRule. */
    SPxErrorCode AddLogicRule(SPxLogicRule *rule);
    SPxErrorCode RemoveLogicRule(SPxLogicRule *rule);

    /* Other private functions. */
    SPxErrorCode report(SPxPacketAlertLogic_tag *alertLogic);
    static int targetFnWrapper(const SPxLogicKey *key, 
	const SPxLogicTarget *target, UINT32 flags, void *userArg);
    int targetFn(const SPxLogicKey *key, const SPxLogicTarget *target,
	UINT32 flags);

    friend class SPxLogicRule;
    friend class SPxLogicDB;

}; /* SPxLogicGroup */

#endif /* _SPX_LOGIC_GROUP_H */

/*********************************************************************
 *
 * End of file
 *
 **********************************************************************/

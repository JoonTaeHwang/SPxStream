/*********************************************************************
*
* (c) Copyright 2014, 2020, 2022 Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxFlightPlan class.
*
* Revision Control:
*   26/01/22 v1.6    BTB	Add string version of clearance code.
*
* Previous Changes:
*   19/01/22 1.5    BTB	Add clearance code.
*   22/10/20 1.4    REW	Support updates from info database items.
*   06/06/14 1.3    REW	Support timestamp for last edit time.
*   21/01/14 1.2    REW	Keep record of if plan has ever been active.
*   15/01/14 1.1    REW	Initial Version.
**********************************************************************/

#ifndef _SPX_FLIGHT_PLAN_H
#define _SPX_FLIGHT_PLAN_H

/*
 * Other headers required.
 */
#include "SPxLibUtils/SPxObj.h"
#include "SPxLibUtils/SPxTime.h"


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

/* Forward-declare other classes we use. */
class SPxInfoDBItem;

/*
 * Define our class.
 */
class SPxFlightPlan : public SPxObj
{
/* The database class can access our internals directly. */
friend class SPxFlightPlanDb;

public:
    /*
     * Public variables.
     */

    /*
     * Public functions.
     */

    /* Note that the constructor and destructor are not public, so the
     * class can only be instantiated by the SPxFlightPlanDb.
     */

    /* Override the base class SetActive() function. */
    virtual void SetActive(int active);

    /* See also the SPxObj::GetActive() function in the base class. */

    /* Has the object ever been active? */
    int GetHasBeenActive(void) const	{ return(m_hasBeenActive); }

    /* Selection. */
    SPxErrorCode SetIsSelected(int selected)
    {
	m_isSelected = (selected ? TRUE : FALSE);
	return(SPX_NO_ERROR);
    }
    int GetIsSelected(void) const	{ return(m_isSelected); }

    /* Raw field access functions. */
    const char *GetCallsign(void) const	{ return(m_callsign); }
    char GetFlightRules(void) const	{ return(m_flightRules); }
    char GetFlightType(void) const	{ return(m_flightType); }
    unsigned int GetIFFCode(void) const	{ return(m_iffCode); }
    unsigned char GetNumAC(void) const	{ return(m_numAC); }
    const char *GetACType(void) const	{ return(m_acType); }
    char GetACWake(void) const		{ return(m_acWake); }
    int GetIsRepeating(void) const	{ return(m_isRepeating); }
    const char *GetDepAero(void) const	{ return(m_depAero); }
    unsigned int GetETDHours(void) const { return(m_etdHours); }
    unsigned int GetETDMins(void) const	{ return(m_etdMins); }
    const char *GetRoute(void) const	{ return(m_route); }
    const char *GetSpeed(void) const	{ return(m_speed); }
    const char *GetAltitude(void) const	{ return(m_altitude); }
    const char *GetDestAero(void) const	{ return(m_destAero); }
    unsigned int GetEETHours(void) const { return(m_eetHours); }
    unsigned int GetEETMins(void) const	{ return(m_eetMins); }
    const char *GetRemarks(void) const	{ return(m_remarks); }
    unsigned int GetClearanceCode(void) const { return(m_clCode); }
    const char *GetClearanceCodeString(void) const { return(m_clString); }

    /* Raw field setting functions. */
    SPxErrorCode SetCallsign(const char *callsign);
    SPxErrorCode SetFlightRules(char flightRules);
    SPxErrorCode SetFlightType(char flightType);
    SPxErrorCode SetIFFCode(unsigned int iffCode);
    SPxErrorCode SetNumAC(unsigned char num);
    SPxErrorCode SetACType(const char *acType);
    SPxErrorCode SetACWake(char acWake);
    SPxErrorCode SetIsRepeating(int isRepeating);
    SPxErrorCode SetDepAero(const char *aero);
    SPxErrorCode SetETDHours(unsigned int hours);
    SPxErrorCode SetETDMins(unsigned int mins);
    SPxErrorCode SetRoute(const char *route);
    SPxErrorCode SetSpeed(const char *speed);
    SPxErrorCode SetAltitude(const char *altitude);
    SPxErrorCode SetDestAero(const char *aero);
    SPxErrorCode SetEETHours(unsigned int hours);
    SPxErrorCode SetEETMins(unsigned int mins);
    SPxErrorCode SetRemarks(const char *remarks);
    SPxErrorCode SetClearanceCode(unsigned int clCode);

    /* Set all fields from a single packet structure and vice versa. */
    SPxErrorCode SetFromPacket(const struct SPxPacketFlightPlan_tag *packet);
    SPxErrorCode BuildPacket(struct SPxPacketFlightPlan_tag *packet) const;

    /* Set fields from an info database item. */
    SPxErrorCode SetFromItem(SPxInfoDBItem *item);

    /* Convenience functions to get fields in different units etc. */
    /* TODO: GetSpeedMps(), GetSpeedKnots(), altitude etc. */

    /* Flight strip generation. */
    SPxErrorCode GetFlightStrip(char *str, size_t count, unsigned int numLines);

    /* Timestamps. */
    SPxErrorCode SetLastEditTime(const SPxTime_t *timestamp=NULL);
    const SPxTime_t *GetLastEditTime(void) const
    {
	return(&m_lastEditTime);
    }
    const SPxTime_t *GetLastStatusChangeTime(void) const
    {
	return(&m_lastStatusChangeTime);
    }

    /*
     * Public static functions.
     */
    static SPxErrorCode NtohPacket(struct SPxPacketFlightPlan_tag *packet);
    static SPxErrorCode HtonPacket(struct SPxPacketFlightPlan_tag *packet);

    /* Functions that can be given to the SPxFlightPlanDb::IteratePlans()
     * function to do certain actions on all plans.
     */
    static int IteratorSelect(void * /*database*/, void * /*arg*/, void *plan)
    {
	if (plan != NULL) { ((SPxFlightPlan *)plan)->SetIsSelected(TRUE); }
	return(SPX_NO_ERROR);
    }
    static int IteratorDeselect(void * /*database*/, void * /*arg*/, void *plan)
    {
	if (plan != NULL) { ((SPxFlightPlan *)plan)->SetIsSelected(FALSE); }
	return(SPX_NO_ERROR);
    }

protected:

    /*
     * Protected variables.
     */
    /* Object linked list pointers (maintained by database). */
    SPxFlightPlan *m_next;
    SPxFlightPlan *m_prev;

    /*
     * Protected functions.
     */

    /* Constructor and destructor. */
    SPxFlightPlan(void);
    virtual ~SPxFlightPlan(void);

    /* Reset internals */
    SPxErrorCode Reset(void);


private:

    /*
     * Private variables.
     */
    /* Flight plan fields. */
    char m_callsign[21];
    char m_flightRules;
    char m_flightType;
    unsigned int m_iffCode;
    unsigned char m_numAC;
    char m_acType[8];
    char m_acWake;
    int m_isRepeating;
    char m_depAero[16];
    unsigned int m_etdHours;
    unsigned int m_etdMins;
    char m_route[512];
    char m_speed[8];
    char m_altitude[8];
    char m_destAero[16];
    unsigned int m_eetHours;
    unsigned int m_eetMins;
    char m_remarks[256];
    unsigned int m_clCode;
    char m_clString[16];

    /* Timestamps. */
    SPxTime_t m_lastStatusChangeTime;	/* Time of last status change. */
    SPxTime_t m_lastEditTime;		/* Time of last user edit */

    /* Set to TRUE if the plan is selected */
    int m_isSelected;

    /* Set to TRUE if the plan has ever been active. */
    int m_hasBeenActive;

    /*
     * Private functions.
     */

    
}; /* SPxFlightPlan*/

/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/


#endif /* SPX_FLIGHT_PLAN_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/

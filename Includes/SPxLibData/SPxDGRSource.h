/*********************************************************************
*
* (c) Copyright 2008, 2009, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxDGRSource.h,v $
* ID: $Id: SPxDGRSource.h,v 1.5 2009/10/30 16:11:08 rew Exp $
*
* Purpose:
*	Header for SPxDGRSource class, which supports the PMC_DGR
*	radar input cards.
*
*	Note that much of the radar input card functionality is provided
*	by the base class from which this class is derived.  The base
*	class auto-detects the type of hardware present and changes its
*	behaviour accordingly, handling the differences in card types
*	and controlling access to type-specific functionality where
*	appropriate.
*
*	Only functions and variables that are purely specific to this
*	particular radar input card are defined here.  The base class
*	header should be referred to for further definitions.
*
*	Be careful to preserve commonality where appropriate if modifying
*	this in the future.
*
*
* Revision Control:
*   30/10/09 v1.5    REW	Move GetCardSubType() to base class.
*
* Previous Changes:
*   25/06/09 1.4    REW	Add SPX_DGR_VIDEO_NONE and DEBUG_PRF.
*   13/11/08 1.3    REW	GetParameter() returns value in a string.
*   04/11/08 1.2    REW	Update DEBUG flags to match base class.
*   29/10/08 1.1    REW	Initial checked-in version.
**********************************************************************/

#ifndef _SPX_DGR_SOURCE_H
#define _SPX_DGR_SOURCE_H

/*
 * Other headers required.
 */
/* We need the base class header (which also includes others). */
#include "SPxLibData/SPxHPx100Source.h"


/*********************************************************************
*
*   Constants
*
**********************************************************************/

/* Local-bus addresses, sizes etc. */
#define	SPX_DGR_MEM_BASE_ADDR		0x00000000
#define	SPX_DGR_MEM_SIZE_BYTES		0x00200000
#define	SPX_DGR_LUT_BASE_ADDR		0x00200000
#define	SPX_DGR_REG_BASE_ADDR		0x00204000

/* Register offsets from the base of the register location. */
#define	SPX_DGR_REG_PERTRG		0x00
#define	SPX_DGR_REG_DLYTRG		0x04
#define	SPX_DGR_REG_NUMSAMPLE		0x08
#define	SPX_DGR_REG_CLKADDIV		0x0c
#define	SPX_DGR_REG_CONFIG		0x10
#define	SPX_DGR_REG_STATUS2		0x14
#define	SPX_DGR_REG_STATUS		0x18
#define	SPX_DGR_REG_FRACFREQ		0x1c
#define	SPX_DGR_REG_MASK		0x20
#define	SPX_DGR_REG_VERSION		0x24
#define	SPX_DGR_REG_DAC			0x28
#define	SPX_DGR_REG_MAGBANK		0x2c
#define	SPX_DGR_REG_OFFSET		0x30
#define	SPX_DGR_REG_TRGTEST		0x34
#define	SPX_DGR_REG_ACPTEST		0x38
#define	SPX_DGR_REG_ARPTEST		0x3c
#define	SPX_DGR_REG_TIMALTRG		0x40
#define	SPX_DGR_REG_TIMALACP		0x44
#define	SPX_DGR_REG_TIMALARP		0x48
#define	SPX_DGR_REG_SAMPLEK		0x4c
#define	SPX_DGR_REG_LASTREG		0x4c	/* Keep maintained! */

/*
 * Debug flags.
 */
#define	SPX_DGR_DEBUG_STATE		0x00000001
#define	SPX_DGR_DEBUG_DECODE		0x00000002
#define	SPX_DGR_DEBUG_INTERRUPT		0x00000004
#define	SPX_DGR_DEBUG_BANK		0x00000008
#define	SPX_DGR_DEBUG_AZIMUTHS		0x00000010
#define	SPX_DGR_DEBUG_RANGE		0x00000020
#define	SPX_DGR_DEBUG_INT_RATE		0x00000040
#define	SPX_DGR_DEBUG_ALARM		0x00000080
#define	SPX_DGR_DEBUG_AZI_INTERP	0x00000100
#define	SPX_DGR_DEBUG_PRF		0x00000200
#define	SPX_DGR_DEBUG_READ_REG		0x00010000
#define	SPX_DGR_DEBUG_READ_MEM		0x00020000
#define	SPX_DGR_DEBUG_READ_LUT		0x00040000
#define	SPX_DGR_DEBUG_WRITE_REG		0x00100000
#define	SPX_DGR_DEBUG_WRITE_MEM		0x00200000
#define	SPX_DGR_DEBUG_WRITE_LUT		0x00400000
#define	SPX_DGR_DEBUG_VERBOSE		0x80000000

/*
 * Input channel options.
 */
#define	SPX_DGR_VIDEO_INVALID		0	/* Same as none */
#define	SPX_DGR_VIDEO_NONE		0	/* None */
#define	SPX_DGR_VIDEO_A			1	/* Analogue A */
#define	SPX_DGR_VIDEO_B			2	/* Analogue B */
#define	SPX_DGR_VIDEO_DIG_NOT_SUPPORTED	3	/* Not supported */
#define	SPX_DGR_VIDEO_COMBI_HW		4	/* Combination, HighestWins */
#define	SPX_DGR_VIDEO_COMBI_OR		5	/* Combination, OR */
#define	SPX_DGR_VIDEO_COMBI_ADD		6	/* Combination, ADD */
#define	SPX_DGR_VIDEO_COMBI_LUT		7	/* Combination, LUT_D */

/*
 * Test Pattern Generator options.
 */
#define	SPX_DGR_TPG_DISABLE		0	/* Off */
#define	SPX_DGR_TPG_DIG_PULS_PULS	1	/* Digital, A=Pulse B=Pulse */
#define	SPX_DGR_TPG_DIG_PULS_RAMP	2	/* Digital, A=Pulse B=Ramp */
#define	SPX_DGR_TPG_DIG_RAMP_PULS	3	/* Digital, A=Ramp B=Pulse */
#define	SPX_DGR_TPG_DIG_RAMP_RAMP	4	/* Digital, A=Ramp B=Ramp */
#define	SPX_DGR_TPG_ANALOGUE_A		5	/* Analogue ramp into A */
#define	SPX_DGR_TPG_ANALOGUE_B		6	/* Analogue ramp into B */
#define	SPX_DGR_TPG_DAC			7	/* Direct value from DAC */

/*
 * Azimuth options.
 */
#define	SPX_DGR_AZI_ACP			0	/* ACP-in */
#define	SPX_DGR_AZI_422			1	/* ACP-422 */

/*
 * ARP options.
 */
#define	SPX_DGR_ARP_ARP			0	/* ARP-in */
#define	SPX_DGR_ARP_422			1	/* ARP-422 */

/*
 * Trigger options.
 */
#define	SPX_DGR_TRG_TRG			0	/* TRG-in */
#define	SPX_DGR_TRG_422			1	/* TRG-422 */

/*
 * LUT definitions.
 */
#define	SPX_DGR_LUT_A_SIZEBYTES		1024
#define	SPX_DGR_LUT_B_SIZEBYTES		1024
#define	SPX_DGR_LUT_D_SIZEBYTES		256

/*
 * DAC definitions.
 */
/* Channel numbers */
#define	SPX_DGR_DAC_OFFSET_A		0
#define	SPX_DGR_DAC_VREF_A		1	/* Not currently used */
#define	SPX_DGR_DAC_OFFSET_B		2
#define	SPX_DGR_DAC_VREF_B		3	/* Not currently used */

/* Min/max offsets.  See comments in SetDAC() function for justication. */
#define	SPX_DGR_DAC_OFFSET_MIN		(-4.75)
#define	SPX_DGR_DAC_OFFSET_MAX		(6.77)


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

/*
 * Define our class, derived from the base hardware-input class.
 */
class SPxDGRSource :public SPxHPx100Source
{
public:
    /*
     * Public fields.
     */

    /*
     * Public functions.
     */
    /* Constructor, destructor etc. */
    SPxDGRSource(SPxRIB *buffer);
    virtual ~SPxDGRSource();

    /* NB: Also see the public functions in the base class. */

    /* Status retrieval. */
    unsigned int GetTBI(void);

    /* Generic parameter assignment. */
    virtual int SetParameter(char *parameterName, char *parameterValue);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);

private:
    /*
     * Private fields.
     */

    /*
     * Private functions.
     */
}; /* SPxDGRSource class */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* _SPX_DGR_SOURCE_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/

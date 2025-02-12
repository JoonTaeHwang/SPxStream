/*********************************************************************
*
* (c) Copyright 2013 - 2019, 2021 - 2022, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxROC class, which supports radar output cards.
*
*
* Revision Control:
*   13/10/22 v1.40   SP 	Change OpenBoard() noClear default to TRUE.
*
* Previous Changes:
*   11/07/22 1.39   AGC	Add NoClear option to OpenBoard().
*   20/06/22 1.38   AJH	Add m_regParAziWidth.
*   14/03/22 1.37   AJH	Support open drain and inverted outputs.
*   28/02/22 1.36   AJH	Add XADC functions and 12V detection.
*   23/02/22 1.35   AJH	Support DAC for HPx-310 signal/power outputs.
*   30/11/21 1.34   AJH	Add initial support for HPx-310.
*   14/10/19 1.33   AGC	Suppress cppcheck warning.
*   21/11/18 1.32   AJH	Support serial azimuth 2 sync bits.
*   02/11/18 1.31   AJH	Support serial azimuth 2 baud rate.
*   19/10/18 1.30   AJH	Add m_useDrv.
*   26/07/18 1.29   AJH	Support inversion of signal-in-video logic.
*   25/07/18 1.28   AJH	More P383 setup.
*   24/07/18 1.27   AJH	Support P383 azimuth setup.
*   18/07/18 1.26   AJH	Support signal-in-video levels.
*   19/06/18 1.25   AJH	Support setting of P383 data.
*   29/05/18 1.24   AJH	Support P383 serial azimuth mode.
*   30/04/18 1.23   AGC	Support changing nav data object.
*   30/04/18 1.22   AGC	Add PrepareForRestart().
*   26/04/18 1.21   AJH	outputMoreAziEvents() takes headingUp argument.
*   10/01/18 1.20   AJH	Support P379 serial trigger/azimuth mode.
*   05/12/17 1.19   REW	Support Get/SetTrgToRangeZeroUsecs.
*   31/01/17 1.18   AGC	Support user supplied nav data.
*   02/11/16 1.17   REW	DumpState() supports second stream.
*   09/12/15 1.16   REW	Support new P313 composite serial ACP/ARP mode.
*			Make more Get functions const.
*			Add VIDEO_FIFO debug registers.
*   14/10/14 1.15   REW	Remove GetBoardIdx() (now in base class).
*   02/07/14 1.14   REW	Remove m_boardIdx (also in base class).
*   04/10/13 1.13   AGC	Simplify headers.
*   17/09/13 1.12   REW	Support constant test pattern with arg.
*   04/09/13 1.11   REW	Drive parallel azi in test mode too.
*   23/08/13 1.10   REW	Add more test patterns.
*   15/08/13 1.9    REW	Add SHM support.
*   01/08/13 1.8    REW	Add LUT utility functions.
*   26/07/13 1.7    REW	Add auto-recovery for full situations.
*   25/07/13 1.6    REW	Add more status and PRF monitoring.
*   19/07/13 1.5    REW	Add time adjustment fine tuning.
*   15/07/13 1.4    REW	Support output streaming etc.
*   26/06/13 1.3    REW	Avoid compiler warning. More definitions etc.
*   19/06/13 1.2    REW	Add more registers, functions etc.
*   14/06/13 1.1    REW	Initial Version.
**********************************************************************/

#ifndef _SPX_ROC_H
#define _SPX_ROC_H

/*
 * Other headers required.
 */
/* We need SPxLibUtils for common types, events, errors etc. */
#include "SPxLibUtils/SPxError.h"

/* We need the radar source base class header. */
#include "SPxLibData/SPxRadarSource.h"

/* We need the HPx common header. */
#include "SPxLibData/SPxHPxCommon.h"

/* We also need SPxRIB definitions. */
#include "SPxLibData/SPxRib.h"

/* For base class. */
#include "SPxLibUtils/SPxObj.h"

/*********************************************************************
*
*   Constants
*
**********************************************************************/

/*
 * Azimuth Modes.
 */
#define	SPX_ROC_AZI_MODE_NORMAL		0	/* Normal pulses for ACP/ARP */
#define	SPX_ROC_AZI_MODE_P313		313	/* Composite serial ACP/ARP */
#define SPX_ROC_AZI_MODE_P379		379	/* Composite serial trigger/azimuth */
#define SPX_ROC_AZI_MODE_P383		383	/* Composite serial trigger/azimuth */
 /*
 * Test Patterns.
 */
#define	SPX_ROC_TEST_PATTERN_DISABLED		0	/* Disabled */
#define	SPX_ROC_TEST_PATTERN_BLACK		1	/* Zero video */
#define	SPX_ROC_TEST_PATTERN_WHITE		2	/* Max video */
#define	SPX_ROC_TEST_PATTERN_RANGE_RAMPS	3	/* Sawtooth */
#define	SPX_ROC_TEST_PATTERN_RING_ALT		4	/* Alternate rings */
#define	SPX_ROC_TEST_PATTERN_RING_MINMAX	5	/* Min/max range rings*/
#define	SPX_ROC_TEST_PATTERN_CONST		6	/* Constant from Arg */


/*
 * Debug flags. Bottom 16-bits are here, top 16-bits in SPxHPxCommon.
 */
#define	SPX_ROC_DEBUG_STATE		0x00000001	/* Start/stop */
#define	SPX_ROC_DEBUG_RANGE		0x00000002	/* Frac freq etc. */
#define	SPX_ROC_DEBUG_DATA		0x00000010	/* Radar returns */
#define	SPX_ROC_DEBUG_AZIS		0x00000020	/* ACP info */
#define	SPX_ROC_DEBUG_TIMESTAMPS	0x00000040	/* Queued events */
#define	SPX_ROC_DEBUG_BUFFER		0x00000100	/* Video buffer */
#define	SPX_ROC_DEBUG_EVENT_FIFO	0x00000200	/* Event buffer */
#define	SPX_ROC_DEBUG_PRF		0x00000400	/* PRF estimates */
#define	SPX_ROC_DEBUG_PRI		0x00000800	/* PRI estimates */
#define	SPX_ROC_DEBUG_LATENCY		0x00001000	/* Latency info */
#define	SPX_ROC_DEBUG_TUNING		0x00002000	/* Clock adjustments */
/* Top 16-bits are in common base class. */

/*
 * Local-bus addresses, sizes etc.
 */
#define	SPX_ROC_REG_BASE_ADDR		0x00000000
#define	SPX_ROC_REG_SIZE_BYTES		0x00000080	/* 128 bytes */
#define	SPX_ROC_LUT_BASE_ADDR		0x00100000
#define	SPX_ROC_LUT_SIZE_BYTES		0x00000400	/* 1024 bytes */
#define	SPX_ROC_MEM_BASE_ADDR		0x00200000
#define	SPX_ROC_MEM_SIZE_PAGE_BYTES	0x00200000	/* 2MB, power of 2 */
#define	SPX_ROC_MEM_SIZE_TOTAL_BYTES	0x02000000	/* 32MB */

/* The HPx-310 has different memory and LUT base addresses. */
#define	SPX_ROC_MEM_BASE_ADDR_HPX310	0x00000000
#define	SPX_ROC_LUT_BASE_ADDR_HPX310	0x00008000
#define	SPX_ROC_MEM_SIZE_TOTAL_BYTES_HPX310	0x20000000	/* 512MB */

/*
 * Register offsets from the base of the register location.
 */
#define	SPX_ROC_REG_VERSION		0x00
#define	SPX_ROC_REG_DATE		0x04
#define	SPX_ROC_REG_CONTROL		0x08
#define	SPX_ROC_REG_MEM_PAGE		0x0C
#define	SPX_ROC_REG_MEM_ADDR		0x10
#define	SPX_ROC_REG_MEM_DATA		0x14
#define	SPX_ROC_REG_VIDEO_FIFO_W	0x18
#define	SPX_ROC_REG_VIDEO_FIFO_R	0x1c
#define	SPX_ROC_REG_EVENT_FIFO_CONTROL	0x20
#define	SPX_ROC_REG_EVENT_FIFO_TIME	0x24
#define	SPX_ROC_REG_EVENT_FIFO_ACTION	0x28
#define	SPX_ROC_REG_TIME		0x2c
#define	SPX_ROC_REG_TIME_ADJUST		0x30
#define	SPX_ROC_REG_FRACFREQ		0x34
#define	SPX_ROC_REG_TRG_TO_VIDEO	0x38
#define	SPX_ROC_REG_TRG_WIDTH		0x3c
#define	SPX_ROC_REG_ARP_WIDTH		0x40
#define	SPX_ROC_REG_ACP_WIDTH		0x44
#define	SPX_ROC_REG_SIG1_WIDTH		0x48
#define	SPX_ROC_REG_SIG2_WIDTH		0x4c
#define	SPX_ROC_REG_SHM_WIDTH		0x50
#define	SPX_ROC_REG_GAIN_OFFSET_A	0x54
#define	SPX_ROC_REG_GAIN_OFFSET_B	0x58
#define	SPX_ROC_REG_SPIFLASH		0x5c
#define	SPX_ROC_REG_VIDEO_DEBUG		0x60
#define	SPX_ROC_REG_CONTROL_DEBUG	0x64
#define	SPX_ROC_REG_PARALLEL_AZI_WIDTH	0x68
#define	SPX_ROC_REG_VIDEO_FIFO_LENGTH	0x6C
#define	SPX_ROC_REG_VIDEO_FIFO_MAGIC0	0x70
#define	SPX_ROC_REG_VIDEO_FIFO_MAGIC1	0x74
#define SPX_ROC_REG_SER_AZI2_BAUD_DIV	0x78
#define SPX_ROC_REG_SER_AZI2_STATUS	0x7C
#define SPX_ROC_REG_SER_AZI3_DELAY	0x80
#define SPX_ROC_REG_SER_AZI3_BAUD_DIV	0x84
#define SPX_ROC_REG_SER_AZI3_MESSAGE	0x88
#define SPX_ROC_REG_VIDEO_SIGNALS_1	0x8C
#define SPX_ROC_REG_VIDEO_SIGNALS_2	0x90
#define	SPX_HPX310_REG_DAC		0x100
#define	SPX_HPX310_REG_POWER_ENA	0x104
#define	SPX_HPX310_REG_SIG2_RS422	0x108
#define	SPX_HPX310_REG_XADC		0x11C
#define	SPX_ROC_REG_LASTREG		0x90	/* Keep maintained! */
#define	SPX_HPX310_REG_LASTREG		0x11C	/* Keep maintained! */

/*
 * Bit definitions for various registers.
 */
/* Control register. */
#define	SPX_ROC_REG_CONTROL_RESET_FIFO_VID	0x00000001
#define	SPX_ROC_REG_CONTROL_RESET_FIFO_EVENT	0x00000002
#define	SPX_ROC_REG_CONTROL_RESET_TIME		0x00000004
#define	SPX_ROC_REG_CONTROL_RESET_CONTROL	0x00000008
#define	SPX_ROC_REG_CONTROL_RESET_VIDEO		0x00000010
#define	SPX_ROC_REG_CONTROL_ENABLE_FIFO_VID	0x00000100
#define	SPX_ROC_REG_CONTROL_ENABLE_FIFO_EVENT	0x00000200
#define	SPX_ROC_REG_CONTROL_ENABLE_TIME		0x00000400
#define	SPX_ROC_REG_CONTROL_ENABLE_ACP_CMPARP	0x00001000
#define	SPX_ROC_REG_CONTROL_ENABLE_ACP_SER	0x00002000
#define	SPX_ROC_REG_CONTROL_ENABLE_ACP_SER2	0x00004000
#define	SPX_ROC_REG_CONTROL_ENABLE_ACP_SER3	0x00008000
#define	SPX_ROC_REG_CONTROL_STATUS_12V_AUX	0x00040000
#define	SPX_ROC_REG_CONTROL_STATUS_BAD_MAGIC	0x00080000
#define	SPX_ROC_REG_CONTROL_STATUS_VID_UFLOW	0x00100000
#define	SPX_ROC_REG_CONTROL_STATUS_VID_OFLOW	0x00200000
#define	SPX_ROC_REG_CONTROL_STATUS_EVENT_OFLOW	0x00400000
#define	SPX_ROC_REG_CONTROL_STATUS_LBE_MASK	0xFF000000

/* Gain/offset. */
#define	SPX_ROC_REG_GAIN_OFFSET_MASK_GAIN	0x00003FFF
#define	SPX_ROC_REG_GAIN_OFFSET_MASK_OFFSET	0x00003FFF /* NB: +shift */
#define	SPX_ROC_REG_GAIN_OFFSET_SHIFT_OFFSET	16

/* Timing limits. */
#define SPX_ROC_REG_TIME_LIMIT			0x0FFFFFF
#define SPX_ROC_REG_TIME_LIMIT_HPX310		0x3FFFFFF

/* Event FIFO control. */
#define	SPX_ROC_REG_EVENT_CONTROL_FULL		0x00000001
#define	SPX_ROC_REG_EVENT_CONTROL_COUNT_SHIFT	8
#define	SPX_ROC_REG_EVENT_CONTROL_COUNT_MASK	0x000003ff /* NB: +shift */
#define	SPX_ROC_REG_EVENT_CONTROL_COUNT_MASK_HPX310	0x00001fff /* NB: +shift */

/* Event FIFO actions. */
#define	SPX_ROC_REG_EVENT_TRG_RISE		0x00000001
#define	SPX_ROC_REG_EVENT_TRG_FALL		0x00000002
#define	SPX_ROC_REG_EVENT_ARP_RISE		0x00000004
#define	SPX_ROC_REG_EVENT_ARP_FALL		0x00000008
#define	SPX_ROC_REG_EVENT_ACP_RISE		0x00000010
#define	SPX_ROC_REG_EVENT_ACP_FALL		0x00000020
#define	SPX_ROC_REG_EVENT_SIG1_RISE		0x00000040
#define	SPX_ROC_REG_EVENT_SIG1_FALL		0x00000080
#define	SPX_ROC_REG_EVENT_SIG2_RISE		0x00000100
#define	SPX_ROC_REG_EVENT_SIG2_FALL		0x00000200
#define	SPX_ROC_REG_EVENT_SHM_RISE		0x00000400
#define	SPX_ROC_REG_EVENT_SHM_FALL		0x00000800

/* Video signal level enables. */
#define	SPX_ROC_REG_VIDSIG1_TRG_ENABLE		0x0000C000
#define	SPX_ROC_REG_VIDSIG1_ACP_ENABLE		0xC0000000
#define	SPX_ROC_REG_VIDSIG2_ARP_ENABLE		0x0000C000

/* Power enables. */
#define SPX_ROC_REG_POWER_ENA_TRG_SIG1		0x00000001
#define SPX_ROC_REG_POWER_ENA_ACP_SIG2		0x00000002
#define SPX_ROC_REG_POWER_ENA_ARP_SHM		0x00000004
#define SPX_ROC_REG_POWER_ENA_GPIO		0x00000008
#define SPX_ROC_REG_POWER_ENA_TRG_OD		0x00000100
#define SPX_ROC_REG_POWER_ENA_ACP_OD		0x00000200
#define SPX_ROC_REG_POWER_ENA_ARP_OD		0x00000400
#define SPX_ROC_REG_POWER_ENA_SIG1_OD		0x00000800
#define SPX_ROC_REG_POWER_ENA_SIG2_OD		0x00001000
#define SPX_ROC_REG_POWER_ENA_SHM_OD		0x00002000
#define SPX_ROC_REG_POWER_ENA_TRG_INV		0x00010000
#define SPX_ROC_REG_POWER_ENA_ACP_INV		0x00020000
#define SPX_ROC_REG_POWER_ENA_ARP_INV		0x00040000
#define SPX_ROC_REG_POWER_ENA_SIG1_INV		0x00080000
#define SPX_ROC_REG_POWER_ENA_SIG2_INV		0x00100000
#define SPX_ROC_REG_POWER_ENA_SHM_INV		0x00200000

/* DAC definitions. */
#define	SPX_HPX310_DAC_TRG_SIG1		0	/* Trigger/SIG1 3-30V output */
#define	SPX_HPX310_DAC_ACP_SIG2		1	/* ACP/SIG2 3-30V output */
#define	SPX_HPX310_DAC_ARP_SHM		2	/* ARP/SHM 3-30V output */
#define	SPX_HPX310_DAC_GPIO		3	/* GPIO 5-8V output */

/* Outputs. */
#define SPX_HPX310_OUTPUT_TRG		0
#define SPX_HPX310_OUTPUT_ACP		1
#define SPX_HPX310_OUTPUT_ARP		2
#define SPX_HPX310_OUTPUT_SHM		3

/* Output types. */
#define	SPX_HPX310_TYPE_PUSH_PULL	0
#define	SPX_HPX310_TYPE_OPEN_DRAIN	1

/* XADC register definitions. */
#define SPX_HPX310_XADC_VCCINT		0x01
#define SPX_HPX310_XADC_VCCAUX		0x02
#define SPX_HPX310_XADC_VCCBRAM		0x06
#define SPX_HPX310_XADC_30V_TRG_SIG1	0x10
#define SPX_HPX310_XADC_30V_ARP_SHM	0x11
#define SPX_HPX310_XADC_3V3A		0x14
#define SPX_HPX310_XADC_15VA		0x15
#define SPX_HPX310_XADC_1V5		0x16
#define SPX_HPX310_XADC_15VA_NEG	0x17
#define SPX_HPX310_XADC_30V_ACP_SIG2	0x18
#define SPX_HPX310_XADC_1VA		0x1C
#define SPX_HPX310_XADC_2V5		0x1D
#define SPX_HPX310_XADC_1V2		0x1E
#define SPX_HPX310_XADC_5V		0x1F
#define SPX_HPX310_XADC_VCCINT_MAX	0x21
#define SPX_HPX310_XADC_VCCAUX_MAX	0x22
#define SPX_HPX310_XADC_VCCBRAM_MAX	0x23
#define SPX_HPX310_XADC_VCCINT_MIN	0x25
#define SPX_HPX310_XADC_VCCAUX_MIN	0x26
#define SPX_HPX310_XADC_VCCBRAM_MIN	0x27

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

/* Forward declare classes in case headers are in wrong order. */
class SPxThread;
class SPxEvent;
class SPxNavData;

/*
 * Define our class, derived from SPx base object and HPx common.
 */
class SPxROC :public SPxObj, public SPxHPxCommon
{
public:
    /*
     * Public fields.
     */

    /*
     * Public functions.
     */
    /* Constructor, destructor etc. */
    explicit SPxROC(SPxNavData *navData=NULL);
    virtual ~SPxROC();

    /* Choice of board to open if OpenBoard() called with zero index. */
    SPxErrorCode SetBoardIdx(unsigned int idx);
    /* NB: GetBoardIdx() is now in SPxHPxCommon base class. */

    /* Board open/close functions.  (ProbeBoard() is in SPxHPxCommon) */
    SPxErrorCode OpenBoard(unsigned int idx=0, int noInit=FALSE, int noClear=TRUE);
    // cppcheck-suppress virtualCallInConstructor
    SPxErrorCode CloseBoard(int noStop=FALSE);

    /* Shutdown ready for application restart. */
    virtual SPxErrorCode PrepareForRestart(void);

    /* Data output. */
    SPxErrorCode NewData(const struct SPxReturnHeader_tag *hdr,
				const unsigned char *data);

    /* Stop and restart output. */
    SPxErrorCode Restart(void);

    /* Change Nav Data object. */
    SPxErrorCode SetNavData(SPxNavData *navData);

    /*
     * Configuration functions.
     */
    SPxErrorCode SetNumACPs(unsigned int numACPs);
    unsigned int GetNumACPs(void) const;
    SPxErrorCode SetPreBufferSecs(double secs);
    double GetPreBufferSecs(void) const;
    SPxErrorCode SetTrgToRangeZeroUsecs(double secs);
    double GetTrgToRangeZeroUsecs(void) const;
    SPxErrorCode SetAziMode(unsigned int mode);
    unsigned int GetAziMode(void) const;

    SPxErrorCode SetOutputType(unsigned int output, unsigned int type);
    SPxErrorCode SetTRGType(unsigned int type) { return SetOutputType(SPX_HPX310_OUTPUT_TRG, type); }
    SPxErrorCode SetACPType(unsigned int type) { return SetOutputType(SPX_HPX310_OUTPUT_ACP, type); }
    SPxErrorCode SetARPType(unsigned int type) { return SetOutputType(SPX_HPX310_OUTPUT_ARP, type); }
    SPxErrorCode SetSHMType(unsigned int type) { return SetOutputType(SPX_HPX310_OUTPUT_SHM, type); }
    unsigned int GetTRGType(void) const { return(m_trgType); }
    unsigned int GetACPType(void) const { return(m_acpType); }
    unsigned int GetARPType(void) const { return(m_arpType); }
    unsigned int GetSHMType(void) const { return(m_shmType); }
    SPxErrorCode SetOutputInverted(unsigned int output, unsigned int inverted);
    SPxErrorCode SetTRGInverted(unsigned int inverted) { return SetOutputInverted(SPX_HPX310_OUTPUT_TRG, inverted); }
    SPxErrorCode SetACPInverted(unsigned int inverted) { return SetOutputInverted(SPX_HPX310_OUTPUT_ACP, inverted); }
    SPxErrorCode SetARPInverted(unsigned int inverted) { return SetOutputInverted(SPX_HPX310_OUTPUT_ARP, inverted); }
    SPxErrorCode SetSHMInverted(unsigned int inverted) { return SetOutputInverted(SPX_HPX310_OUTPUT_SHM, inverted); }
    unsigned int GetTRGInverted(void) const { return(m_trgInv); }
    unsigned int GetACPInverted(void) const { return(m_acpInv); }
    unsigned int GetARPInverted(void) const { return(m_arpInv); }
    unsigned int GetSHMInverted(void) const { return(m_shmInv); }

    /* Signal widths. */
    SPxErrorCode SetTRGWidth(double usecs);
    double GetTRGWidth(void) const;
    SPxErrorCode SetARPWidth(double usecs);
    double GetARPWidth(void) const;
    SPxErrorCode SetACPWidth(double usecs);
    double GetACPWidth(void) const;
    SPxErrorCode SetSIG1Width(double usecs);
    double GetSIG1Width(void) const;
    SPxErrorCode SetSIG2Width(double usecs);
    double GetSIG2Width(void) const;
    SPxErrorCode SetSHMWidth(double usecs);
    double GetSHMWidth(void) const;

    /* Gain/offset. */
    double GetMinGain(void) const;
    double GetMaxGain(void) const;
    double GetMinOffset(void) const;
    double GetMaxOffset(void) const;
    SPxErrorCode SetGainA(double gain);
    SPxErrorCode SetGainB(double gain);
    SPxErrorCode SetOffsetA(double offset);
    SPxErrorCode SetOffsetB(double offset);
    double GetGainA(void) const;
    double GetGainB(void) const;
    double GetOffsetA(void) const;
    double GetOffsetB(void) const;

    /* Video signal levels. */
    SPxErrorCode SetVideoSignalLevel(unsigned int signal, unsigned int level);
    unsigned int GetVideoSignalLevel(unsigned int signal) {
	switch( signal )
	{
	case 0:
	default:    return m_vidSig0Level; break;
	case 1:	    return m_vidSig1Level; break;
	case 2:	    return m_vidSig2Level; break;
	}
    }
    SPxErrorCode SetVideoSignalInverted(unsigned int signal, unsigned int inverted);
    unsigned int GetVideoSignalInverted(unsigned int signal) {
	switch( signal )
	{
	case 0:
	default:    return m_vidSig0Inv; break;
	case 1:	    return m_vidSig1Inv; break;
	case 2:	    return m_vidSig2Inv; break;
	}
    }

    /* Serial azimuth control. */
    SPxErrorCode SetSerialAziP379Mode(unsigned int baudRate,
	unsigned int syncBits);
    SPxErrorCode GetSerialAziP379Mode(unsigned int *baudRate,
	unsigned int *syncBits);
    SPxErrorCode SetSerialAziP383Mode(unsigned int baudRate,
	unsigned int delayUsec, unsigned int controlBits);
    SPxErrorCode GetSerialAziP383Mode(unsigned int *baudRate,
	unsigned int *delayUsec, unsigned int *controlBits);

    /* Test pattern control. */
    SPxErrorCode SetTestPattern(unsigned int pattern);
    unsigned int GetTestPattern(void) const	{ return(m_testPattern); }
    SPxErrorCode SetTestArg(unsigned int arg);
    unsigned int GetTestArg(void) const		{ return(m_testArg); }
    SPxErrorCode SetTestPRF(double hz);
    double GetTestPRF(void) const		{ return(m_testPRF); }
    SPxErrorCode SetTestPeriod(double secs);
    double GetTestPeriod(void) const		{ return(m_testPeriod); }
    SPxErrorCode SetTestRangeMetres(double metres);
    double GetTestRangeMetres(void) const	{ return(m_testRangeMetres); }
    SPxErrorCode SetTestRangeSamples(unsigned int samples);
    unsigned int GetTestRangeSamples(void) const { return(m_testRangeSamples);}

    /* LUT control. */
    SPxErrorCode SetLUTAuto(int autoEnabled);
    int GetLUTAuto(void) const			{ return(m_lutAuto); }
    SPxErrorCode BuildLUT(unsigned char *buf,
				unsigned int bufSizeBytes,
				unsigned int packing);

    /* Basic testing functions. */
    SPxErrorCode RegTest(void);
    SPxErrorCode MemTest(void);
    SPxErrorCode LUTTest(unsigned int lutFlags=0xF);
    /* NB: FlashTest() is in common base class */

    /* Status functions. */
    SPxErrorCode GetStatus12VAuxGood(void);
    unsigned int GetBufferedDataNumBytes(void);
    double GetBufferedDataPercentFull(void);
    double GetBufferedDataLatencySecs(void);
    unsigned int GetBufferedEvents(void);
    double GetBufferedEventsPercentFull(void);
    double GetBufferedEventsLatencySecs(void);
    double GetSampleClockHz(void) const;
    double GetSampleSizeMetres(void) const;
    double GetStartRangeMetres(void) const { return((double)m_lastStartRange);}
    double GetEndRangeMetres(void) const   { return((double)m_lastEndRange); }

    /* Utility functions to check if certain features are available.
     * They require the card to have already been opened.
     * NB: These return SPX_NO_ERROR if the feature is supported.
     */
    SPxErrorCode IsAziModeSupported(unsigned int mode) const;

    /* Low-level hardware access functions (not for normal use). */
    /* NB: ReadReg() and WriteReg() are in common base class. */
    SPxErrorCode ReadMem(void *dst, unsigned int src, unsigned int nBytes,
							int useDrv=0);
    SPxErrorCode WriteMem(unsigned int dst, void *src, unsigned int nBytes,
							int useDrv=0);
    SPxErrorCode ReadLUT(unsigned int lutIdx, unsigned char *buf,
				unsigned int bufSizeBytes);
    SPxErrorCode WriteLUT(unsigned int lutIdx, const unsigned char *buf,
				unsigned int bufSizeBytes);
    unsigned int GetLUTSize(unsigned int /*lutIdx*/)
    {
	return(SPX_ROC_LUT_SIZE_BYTES);
    }
    unsigned char *GetLUTBuffer(void)	{ return(m_lutBuf); }
    /* NB: ReadFlash() and associated functions are in common base class. */

    /* DAC setup, but see convenience functions SetOffset() etc.
     * below.  The SetDAC() function takes a voltage whereas the
     * SetDACDirect() function takes a register value and is for
     * internal use only.  The SetVoltage() functions are the ones
     * that will be most useful to client applications.
     */
    SPxErrorCode SetDAC(unsigned int dac, double voltage);
    double GetDAC(unsigned int dac);
    SPxErrorCode SetDACDirect(unsigned int dac, int regLevel);
    int GetDACDirect(unsigned int dac);

    SPxErrorCode SetVoltageTRGSIG1(double voltage)
    {
	return(SetDAC(SPX_HPX310_DAC_TRG_SIG1, voltage));
    }
    double GetVoltageTRGSIG1(void) { return(GetDAC(SPX_HPX310_DAC_TRG_SIG1)); }
    SPxErrorCode SetVoltageACPSIG2(double voltage)
    {
	return(SetDAC(SPX_HPX310_DAC_ACP_SIG2, voltage));
    }
    double GetVoltageACPSIG2(void) { return(GetDAC(SPX_HPX310_DAC_ACP_SIG2)); }
    SPxErrorCode SetVoltageARPSHM(double voltage)
    {
	return(SetDAC(SPX_HPX310_DAC_ARP_SHM, voltage));
    }
    double GetVoltageARPSHM(void) { return(GetDAC(SPX_HPX310_DAC_ARP_SHM)); }
    SPxErrorCode SetVoltageGPIO(double voltage)
    {
	return(SetDAC(SPX_HPX310_DAC_GPIO, voltage));
    }
    double GetVoltageGPIO(void) { return(GetDAC(SPX_HPX310_DAC_GPIO)); }

    /* Generic parameter assignment. */
    virtual int SetParameter(char *parameterName, char *parameterValue);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);

    /* Debug. */
    void DumpState(const char *reason = NULL, unsigned int streamIdx = 0);
    const char *GetRegName(unsigned int reg);
    void DumpVoltages(void);

    /* Control. */
    void SetUseDrv(int useDrv) { m_useDrv = useDrv; }
    int GetUseDrv(void) const { return m_useDrv; }

protected:
    /*
     * Protected fields (used by derived classes).
     */

private:
    /*
     * Private fields.
     */
    /* Nav data for SHM. */
    SPxNavData *m_navData;

    /* Control options. */
    int m_noInit;			/* Were we opened in noInit mode? */
    int m_useDrv;			/* Should we use driver SDK access? */

    /* Worker-thread. */
    SPxThread *m_thread;		/* Thread handle for object */
    SPxEvent *m_event;			/* Event object for signalling */

    /* Register values. */
    UINT32 m_regControl;		/* Value of CONTROL register */
    UINT32 m_regVideoFifoW;		/* Value of VIDEO_FIFO_W register */
    UINT32 m_regTimeAdjust;		/* Value of TIME_ADJUST register */
    UINT32 m_regFracFreq;		/* Value of FRACFREQ register */
    UINT32 m_regTrgToVideo;		/* Value of TRG_TO_VIDEO register */
    UINT32 m_regTrgWidth;		/* Value of TRG_WIDTH register */
    UINT32 m_regArpWidth;		/* Value of ARP_WIDTH register */
    UINT32 m_regAcpWidth;		/* Value of ACP_WIDTH register */
    UINT32 m_regSig1Width;		/* Value of SIG1_WIDTH register */
    UINT32 m_regSig2Width;		/* Value of SIG2_WIDTH register */
    UINT32 m_regShmWidth;		/* Value of SHM_WIDTH register */
    UINT32 m_regGainOffsetA;		/* Value of GAIN_OFFSET_A register */
    UINT32 m_regGainOffsetB;		/* Value of GAIN_OFFSET_B register */
    UINT32 m_regVideoSignals1;		/* Value of VIDEO_SIGNALS_1 register */
    UINT32 m_regVideoSignals2;		/* Value of VIDEO_SIGNALS_2 register */
    UINT32 m_regSerAzi2BaudDiv;		/* Value of SER_AZI2_BAUD_DIV register */
    UINT32 m_regSerAzi3Delay;		/* Value of SER_AZI3_DELAY register */
    UINT32 m_regSerAzi3BaudDiv;		/* Value of SER_AZI3_BAUD_DIV register */
    UINT32 m_regPowerEna;		/* Value of POWER_ENA register */
    UINT32 m_regParAziWidth;		/* Value of PARALLEL_AZI_WIDTH register */

    /* Clock divisors etc. */
    double m_mainClkHz;			/* Main clock frequency in Hz */
    unsigned int m_nsPerMainClk;	/* Nanoseconds per main clock */

    /* Card-dependent sizes, offsets and limits. */
    unsigned int m_memSizeTotalBytes;	/* SPX_ROC_MEM_SIZE_TOTAL_BYTES(_HPX310) */
    unsigned int m_memBaseAddr;		/* SPX_ROC_MEM_BASE_ADDR(_HPX310) */
    unsigned int m_lutBaseAddr;		/* SPX_ROC_LUT_BASE_ADDR(_HPX310) */
    unsigned int m_eventControlCountMask;   /* SPX_ROC_REG_EVENT_CONTROL_COUNT_MASK(_HPX310) */
    unsigned int m_timeRegLimit;	/* SPX_ROC_REG_TIME_LIMIT(_HPX310) */

    /* Configuration options. */
    int m_canUseDmaNotification;	/* Flag */

    /* Buffer for building LUT values. */
    unsigned char *m_lutBuf;		/* May be accessed by user too */
    int m_lutAuto;			/* Do we automatically set the LUT? */
    int m_lutLastPacking;		/* Last packing type for auto LUT */

    /* Linked lists of returns to output. */
    struct SPxROCReturn_tag *m_freeReturns;	/* List of free buffers */
    struct SPxROCReturn_tag *m_firstPendingReturn; /* Start of pending list */
    struct SPxROCReturn_tag *m_lastPendingReturn;  /* End of pending list */
    unsigned int m_numReturnsAllocated;		/* Count */

    /* Status info. */
    UINT32 m_lastErrorEventFull;	/* Time of last error message */
    UINT32 m_possibleFullTime;		/* Time when thought full */

    /* Timing information. */
    double m_preBufferSecs;		/* How much data to pre-buffer */
    double m_trgToRangeZeroUsecs;	/* Time from trigger to range zero */
    UINT32 m_nextTriggerTime;		/* Time for next trigger to write */
    REAL32 m_lastStartRange;		/* Start range of last spoke */
    REAL32 m_lastEndRange;		/* End range of last spoke */
    UINT16 m_lastNominalLength;		/* Nominal length of last spoke */
    double m_lastTrgToRangeZeroUsecs;	/* Time from trigger to range zero */

    /* PRF estimation and checks */
    unsigned int m_prfNumReturns;	/* Number of rtns since last check */
    UINT32 m_prfLastTicker;		/* Ticker for last PRF check */
    double m_prfHz;			/* Estimate of PRF in hz */

    /* PRI monitoring. */
    unsigned int m_priTotalNS;		/* Total nsecs of PRI's since check */
    UINT32 m_priLastTicker;		/* Ticker for last PRI check */
    UINT32 m_priLastWarning;		/* Ticker for last warning */
    int m_priMismatch;			/* Is there a mismatch? */

    /* Clock adjustments. */
    int m_tuningState;			/* Tuning state. */
    unsigned int m_tuningNumReturns;	/* Number of rtns since last check */
    UINT32 m_tuningLastTicker;		/* Ticker for last tuning check */
    UINT32 m_tuningReportLastTicker;	/* Ticker for last debug report */
    unsigned int m_tuningLastNumReturns;/* Number of returns at last check */
    double m_tuningAvgLatencySecs;	/* Average latency */
    double m_tuningLastAvgLatencySecs;	/* Average latency at last check */
    int m_tuningCorrectionCycles;	/* Correction to apply */

    /* Azimuth information. */
    unsigned int m_aziMode;		/* Normally 0, unless special mode */
    unsigned int m_acpsPerRev;		/* Number of ACPs to output per rev */
    UINT32 m_lastACPtime;		/* Timestamp of last ACP  */
    unsigned int m_lastACPcount;	/* Count for last ACP */
    UINT32 m_avgACPinterval;		/* Time interval between ACPs */
    UINT32 m_avgACPlastMsecs;		/* Time of last avg ACP check */
    unsigned int m_avgACPlastCount;	/* ACP count at last check */

    /* Configuration options. */
    unsigned int m_trgType;		/* TRG mode, SPX_HPX310_TRG_... */
    unsigned int m_trgInv;		/* TRG inverted, TRUE or FALSE */
    unsigned int m_acpType;		/* ACP mode, SPX_HPX310_ACP_... */
    unsigned int m_acpInv;		/* ACP inverted, TRUE or FALSE */
    unsigned int m_arpType;		/* ARP mode, SPX_HPX310_ARP_... */
    unsigned int m_arpInv;		/* ARP inverted, TRUE or FALSE */
    unsigned int m_shmType;		/* SHM mode, SPX_HPX310_SHM_... */
    unsigned int m_shmInv;		/* SHM inverted, TRUE or FALSE */

    /* SHM. */
    unsigned int m_shmAziIdx;		/* Azimuth index for SHM */

    /* Test pattern. */
    unsigned int m_testPattern;		/* Pattern if non-zero */
    unsigned int m_testArg;		/* Argument for some test patterns */
    double m_testPRF;			/* PRF in Hz */
    double m_testPeriod;		/* Period in seconds */
    double m_testRangeMetres;		/* End range in metres */
    unsigned int m_testRangeSamples;	/* Number of samples */
    unsigned char *m_testBuf;		/* Buffer for test header and data */
    unsigned int m_testBufSizeBytes;	/* Size of buffer */
    unsigned int m_testAzi;		/* Test pattern parallel azimuth */

    /* Useful values. */
    const int m_max14BitSigned;
    const int m_min14BitSigned;

    /* P379 values. */
    unsigned int m_serialAzi2Baud;
    unsigned int m_serialAzi2SyncBits;

    /* P383 values. */
    UINT32 m_p383Data;
    unsigned int m_vidSig0Level;
    unsigned int m_vidSig1Level;
    unsigned int m_vidSig2Level;
    unsigned int m_vidSig0Inv;
    unsigned int m_vidSig1Inv;
    unsigned int m_vidSig2Inv;
    unsigned int m_serialAzi3DelayUsec;
    unsigned int m_serialAzi3Baud;
    unsigned int m_serialAzi3CtrlBits;

    /* DAC levels. */
    double m_dacVoltageTRGSIG1;		/* Voltage for TRG/SIG1. */
    double m_dacVoltageACPSIG2;		/* Voltage for ACP/SIG2. */
    double m_dacVoltageARPSHM;		/* Voltage for ARP/SHM. */
    double m_dacVoltageGPIO;		/* Voltage for GPIO. */

    /* Internal DAC registers and values. */
    double m_dacVref;			/* DAC reference voltage. */
    int m_dacRegLevelTRGSIG1;		/* Register level for TRG/SIG1. */
    int m_dacRegLevelACPSIG2;		/* Register level for ACP/SIG2. */
    int m_dacRegLevelARPSHM;		/* Register level for ARP/SHM. */
    int m_dacRegLevelGPIO;		/* Register level for GPIO. */
    double m_dacVFB;			/* Positive feedback voltage. */
    double m_dacIFB;			/* Bias current. */
    double m_dacRFB;			/* Feedback resistor. */
    double m_dacRDAC;			/* DAC output resistor. */
    double m_dacRVOUT;			/* PS output resistor. */

    /*
     * Private functions.
     */
    /* Worker thread functions. */
    static void *threadWrapper(SPxThread *thread);
    void *threadFunc(SPxThread *thread);
    SPxErrorCode writeReturnToCard(const struct SPxROCReturn_tag *rtn);
    SPxErrorCode checkTimingForReturn(const struct SPxROCReturn_tag *rtn);
    SPxErrorCode outputMoreAziEvents(UINT32 nextTrgTime, UINT16 azi16, int headingUp);
    SPxErrorCode outputEvent(UINT32 eventTime, UINT32 eventMask);
    SPxErrorCode outputMoreTestPattern(void);
    SPxErrorCode adjustTuning(UINT32 now);

    /* Utility functions. */
    SPxErrorCode clearMemory(void);
    int getNumBytesInVideoFifo(void);
    SPxErrorCode stopAndResetOutput(void);
    SPxErrorCode discardPendingReturns(void);
    int getFpgaTimeDiff(UINT32 firstTime, UINT32 secondTime);
    double getLatencyTrg(void);
    double getLatencyAcp(void);

    /* Conversion functions. */
    int signed14BitToInt(int bit14) const;
    int intToSigned14Bit(int value) const;
    double getGainFor14Bit(int bit14) const;
    int get14BitForGain(double gain) const;
    double getOffsetFor14Bit(int bit14) const;
    int get14BitForOffset(double offset) const;

    /* Control functions. */
    SPxErrorCode waitForDAC(void);	/* Wait until okay to write to DAC */
    void calcDACValues(unsigned int dac);

    /* Page control. */
    SPxErrorCode setPageForAddress(unsigned int memAddress);
    SPxErrorCode setPage(unsigned int page);

    /* P383 data. */
    SPxErrorCode setP383Data(unsigned int data)
	{ m_p383Data = data; return SPX_NO_ERROR; }
    unsigned int getP383Data(void) { return m_p383Data; }

    /* Internal functions. */
    SPxErrorCode writeLUTlocked(unsigned int lutIdx,
				const unsigned char *buf,
				unsigned int bufSizeBytes);
}; /* SPxROC class */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* _SPX_ROC_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/

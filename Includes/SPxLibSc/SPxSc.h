/*********************************************************************
*
* (c) Copyright 2007 - 2019, Cambridge Pixel Ltd.
*
* Purpose:
*   Header for base class of all scan conversion classes.
*
*
* Revision Control:
*   28/03/19 v1.30   AGC	Rename SetRadarLUT lutId parameter to rid.
*
* Previous Changes:
*   20/03/19 1.29   AGC	Support live/trails video with single scan converter.
*   20/03/19 1.28   AGC	Support sharing scan converter destinations.
*   17/05/18 1.27   AGC	Support scan conversion scaling.
*   29/06/16 1.26   AJH	Fix prototype for GetViewControl().
*   24/06/16 1.25   AJH	Add virtual functions for view control.
*   18/05/16 1.24   AGC	Rename moving platform reference mode.
*   11/05/16 1.23   AGC	Add rotation and moving platform reference mode.
*   03/11/15 1.22   AGC	PatchSet() patch data now const.
*   04/10/13 1.21   AGC	Simplify headers.
*   22/11/12 1.20   REW	Add PanView().
*   09/06/11 1.19   AGC	Avoid warnings when header compiled
*			with warning level 4 in Visual Studio.
*   04/02/11 1.18   REW	Avoid warning in last change.
*   04/02/11 1.17   AGC	Add Set/Get SweepLine Type/ColMode/Colour functions.
*			Remove SPX_SC_SWEEPLINE_TYPE_DEST_ONLY.
*   28/01/11 1.16   AGC	Add SPX_SC_SWEEPLINE_TYPE_DEST_ONLY.
*   27/05/10 1.15   SP 	Add SPX_SC_TYPE_UNKNOWN and GetScType().
*   21/08/09 1.14   REW	Add SetSweepLine().
*   20/08/08 1.13   REW	Add MCAST equivalents of sc type definitions.
*   13/08/08 1.12   REW	Add SPX_SC_TYPE_PPI/BSCAN definitions.
*   31/01/08 1.11   REW	Add SPX_SC_STATE_HIDDEN/VISIBLE.
*   17/01/08 1.10   REW	Avoid icc compiler warnings.
*   14/11/07 1.9    REW	Add SPX_SC_STATE_... constants.
*   25/10/07 1.8    REW	Change SetSource() to use UINT32 args.
*   29/06/07 1.7    DGJ	Derive from SPxObj.
*   28/06/07 1.6    REW	Undo last change.
*   28/06/07 1.5    DGJ	Added SPxScRemote.h
*   18/06/07 1.4    REW	Make FADE definitions a typedef.
*   13/06/07 1.3    DGJ	Made window positions signed.
*   08/06/07 1.2    DGJ	Added fade types. Tweaked PatchSet().
*   07/06/07 1.1    DGJ	Initial Version.
**********************************************************************/

#ifndef _SPX_SC_H
#define _SPX_SC_H

/*
 * Other headers required.
 */
/* For base class. */
#include "SPxLibUtils/SPxObj.h"

/*********************************************************************
*
*   Constants
*
**********************************************************************/

/*
 * Define the possible states for ShowRadar().
 *
 * The first three control the real state of the scan conversion and
 * display.  These may be seen by the destination objects as well (such
 * as the bitmap class and the X11/Windows display classes/servers).
 * The last two (HIDDEN and VISIBLE) are used by the window-following
 * classes to inform the scan converter that although the window may
 * be active (i.e. scan conversion is enabled), it is not actually
 * visible at the moment (e.g. minimised, offscreen etc.) and hence
 * there is no point doing the scan conversion.  When made visible
 * again, the scan converter will revert to the previous activity
 * state.
 */
#define	SPX_SC_STATE_STOP_AND_CLEAR	0	/* Clear immediately */
#define	SPX_SC_STATE_RUN		1	/* Normal updates */
#define	SPX_SC_STATE_STOP_AND_FADE	2	/* Fade away (if real-time) */
#define	SPX_SC_STATE_HIDDEN		0xfe	/* Window minimised etc. */
#define	SPX_SC_STATE_VISIBLE		0xff	/* Window restored */


/*
 * Scan conversion window types.
 */
#define SPX_SC_TYPE_UNKNOWN             (-1)    
#define	SPX_SC_TYPE_PPI			0
#define	SPX_SC_TYPE_BSCAN		1
#define	SPX_SC_TYPE_PPI_MCAST		10	/* Only for SPxScRemote */
#define	SPX_SC_TYPE_BSCAN_MCAST		11	/* Only for SPxScRemote */
#define	SPX_SC_TYPE_PPI_MASTER		20	/* Master PPI when sharing dest */
#define	SPX_SC_TYPE_PPI_SLAVE		30	/* Slave PPI when sharing dest */


/* Define the mask of the LSBs used for the sweep line display in
 * real-time fade mode.
 */
#define	SPX_SC_SWEEPLINE_RT_BITMASK	7	/* 3 LSBs */


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

/* Fade types. */
enum SPxRadarFadeMode_t
{
    SPX_RADAR_FADE_REAL_TIME = 0,               // Real-time fading
    SPX_RADAR_FADE_SWEEP = 1,                   // Sweep-based fading.
    SPX_RADAR_FADE_REPLACE = 2                  // Replacement mode.
};

/* Sweep line types. */
enum SPxScSweepLineType_t
{
    SPX_SC_SWEEPLINE_TYPE_OFF = 0,	/* Off - No sweep line */
    SPX_SC_SWEEPLINE_TYPE_AUTO = 1	/* On - Automatic type of line */
};

/* Sweep line modes. */
enum SPxScSweepLineColMode_t
{
    SPX_SC_SWEEPLINE_COLMODE_OFFSET = 0,	/* Offset added to radar */
    SPX_SC_SWEEPLINE_COLMODE_HIGHEST = 1,	/* Highest-wins with radar */
    SPX_SC_SWEEPLINE_COLMODE_REPLACE = 2	/* Overwrite the radar */
};

enum SPxScMpRefMode_t
{
    SPX_SC_MP_REF_MODE_NORMAL = 0,
    SPX_SC_MP_REF_MODE_ADJUST_FROM_NORTH = 1
};

/* Forward declare any classes we need in our own class in case
 * headers are included in the wrong order.
 */
class SPxViewControl;

/*
 * The base class for all scan-conversion classes.
 */
class SPxSc :public SPxObj
{
public:
    /*
     * Public fields.
     */

    /*
     * Public functions.
     */
    /* Constructor/destructor, but note this is an abstract class
     * and cannot be instantiated.
     */
    SPxSc(void) {;}
    virtual ~SPxSc() {;}

    /* Set functions, all pure virtual. */
    virtual int SetWinPos(INT16 screenX, INT16 screenY)=0;
    virtual int SetWinGeom(INT16 screenX, INT16 screenY,
				UINT16 screenW, UINT16 screenH)=0;
    virtual int SetWinParam(UINT16 param, UINT32 arg1,
				UINT32 arg2=0, UINT32 arg3=0)=0;
    virtual int SetFade(UCHAR fadeType, UINT16 fadeRate)=0;
    virtual int SetFade(UCHAR /*rid*/, UCHAR fadeType, UINT16 fadeRate) { return SetFade(fadeType, fadeRate); }
    virtual int SetView(REAL32 viewX, REAL32 viewY,
				REAL32 viewW, REAL32 viewH)=0;
    virtual int PanView(REAL32 viewX, REAL32 viewY, UCHAR clearTrails=0)=0;
    virtual int SetRadarColour(UCHAR rid,
				UCHAR red, UCHAR green, UCHAR blue)=0;
    virtual int SetRadarBright(UCHAR rid, UCHAR bright)=0;
    virtual int SetRadarLUT(UCHAR rid, UCHAR * lutData)=0;
    virtual int ShowRadar(UCHAR rid, UCHAR state)=0;
    virtual int PatchSet(UCHAR format, UINT16 numPatches,
				const UINT16 *patchData, UINT16 patchLen)=0;
    virtual int SetSource(UINT32 sourceID, UINT32 arg1=0, UINT32 arg2=0)=0;
    virtual int SetRadarPosition(REAL32 radarX, REAL32 radarY,
				UCHAR clearTrails=0)=0;
    virtual int SetRotationDegs(double /*rotationDegs*/) { return SPX_ERR_NOT_SUPPORTED; }
    virtual double GetRotationDegs(void) { return 0.0; }
    virtual SPxErrorCode SetScaleFactor(int /*scaleFactor*/) { return SPX_ERR_NOT_SUPPORTED; }
    virtual int GetScaleFactor(void) const { return 1; }
    virtual int SetMovingPlatformRefMode(SPxScMpRefMode_t) { return SPX_ERR_NOT_SUPPORTED; }
    virtual SPxScMpRefMode_t GetMovingPlatformRefMode(void) const { return SPX_SC_MP_REF_MODE_NORMAL; }
    virtual int SetSweepLine(SPxScSweepLineType_t type,
				SPxScSweepLineColMode_t colourMode,
				UINT32 rgb,
				UINT32 reserved1, UINT32 reserved2)=0;

    /* These functions should be overridden where applicable. */
    virtual int SetSweepLineType(SPxScSweepLineType_t /*type*/) { return 0; }
    virtual int SetSweepLineColMode(SPxScSweepLineColMode_t /*colMode*/) { return 0; }
    virtual int SetSweepLineColour(UINT32 /*rgb*/) { return 0; }
    virtual int GetSweepLineType(SPxScSweepLineType_t * /*type*/) { return SPX_ERR_NOT_SUPPORTED; }
    virtual int GetSweepLineColMode(SPxScSweepLineColMode_t * /*colMode*/) { return SPX_ERR_NOT_SUPPORTED; }
    virtual int GetSweepLineColour(UINT32 * /*rgb*/) { return SPX_ERR_NOT_SUPPORTED; }
    virtual SPxErrorCode SetViewControl(SPxViewControl * /*obj*/,
	unsigned int /*radarIndex*/=0,
	unsigned int /*platformIndex*/=0) { return SPX_ERR_NOT_SUPPORTED; }
    virtual SPxViewControl *GetViewControl(void) const { return 0; }

    /* This function should be overridden where applicable */
    virtual int GetScType(void) { return SPX_SC_TYPE_UNKNOWN; }

private:
    /*
     * Private fields.
     */

    /*
     * Private functions.
     */
}; /* SPxSc */
   
       
/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/



#endif /* _SPX_SC_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/

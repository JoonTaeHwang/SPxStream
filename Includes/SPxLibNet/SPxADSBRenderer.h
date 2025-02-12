/*********************************************************************
*
* (c) Copyright 2011 - 2020, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxADSBRenderer abstract class which supports the display
*	of ADSB messages in an application, but only via a derived class.
*
*
* Revision Control:
*   20/11/20 v1.9    REW	Support flashing SPI and Emergency tracks.
*
* Previous Changes:
*   18/09/20 1.8    AGC	Support showing excluded tracks.
*   07/12/18 1.7    REW	Support optional fading of trail points.
*   06/03/18 1.6    AGC	Support configurable labels.
*   20/11/17 1.5    AGC	Support flashing of targets.
*   13/07/17 1.4    SP 	Add GetDecoder().
*   07/10/13 1.3    AGC	Move rendering to renderAll().
*   04/10/13 1.2    AGC	Simplify headers.
*   23/12/11 1.1    REW	Initial Version.
**********************************************************************/

#ifndef _SPX_ADSB_RENDERER_H
#define _SPX_ADSB_RENDERER_H

/*
 * Other headers required.
 */
/* We need SPxLibUtils for common types, callbacks, errors etc. */
#include "SPxLibUtils/SPxRenderer.h"

/* We need the SPxADSBDecoder class. */
#include "SPxLibNet/SPxADSBDecoder.h"


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

/* Need to forward-declare other classes in case headers are in wrong order. */
class SPxADSBDecoder;
class SPxADSBTrack;
class SPxRenderer;

/*
 * Define our class, derived from the abstract renderer base class.
 */
class SPxADSBRenderer : virtual public SPxRenderer
{
public:
    /*
     * Public variables.
     */

    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    SPxADSBRenderer(SPxADSBDecoder *decoder);
    virtual ~SPxADSBRenderer(void);

    /* Get decoder. */
    SPxADSBDecoder *GetDecoder(void) { return m_decoder; }

    /* Symbol control. */
    void SetSymbolLength(UINT32 pixels)		{ m_symbolLength = pixels; }
    UINT32 GetSymbolLength(void)		{ return(m_symbolLength); }

    /* Colour control. */
    void SetSymbolColour(UINT32 argb)		{ m_symbolColour = argb; }
    UINT32 GetSymbolColour(void)		{ return(m_symbolColour); }

    /* Vector control. */
    void SetVectorSeconds(UINT32 secs)		{ m_vectorSeconds = secs; }
    UINT32 GetVectorSeconds(void)		{ return(m_vectorSeconds); }

    /* Label control. */
    void SetShowAllLabels(int show)		{m_showAllLabels = show;}
    int GetShowAllLabels(void)			{return(m_showAllLabels);}
    void SetShowActivatedLabels(int show)	{m_showActivatedLabels = show;}
    int GetShowActivatedLabels(void)		{return(m_showActivatedLabels);}
    void SetShowSelectedLabels(int show)	{m_showSelectedLabels = show;}
    int GetShowSelectedLabels(void)		{return(m_showSelectedLabels);}
    void SetLabel(const char *txt);
    const char *GetLabel(void)			{return m_label;}
    void SetLabelMaxRows(unsigned int maxRows)	{m_labelMaxRows=maxRows;}
    unsigned int GetLabelMaxRows(void) const	{return(m_labelMaxRows);}

    /* History trail control. */
    void SetHistorySeconds(UINT32 secs)		{ m_historySeconds = secs; }
    UINT32 GetHistorySeconds(void)		{ return(m_historySeconds); }
    void SetShowAllTrails(int show)		{m_showAllTrails = show;}
    int GetShowAllTrails(void)			{return(m_showAllTrails);}
    void SetShowActivatedTrails(int show)	{m_showActivatedTrails = show;}
    int GetShowActivatedTrails(void)		{return(m_showActivatedTrails);}
    void SetShowSelectedTrails(int show)	{m_showSelectedTrails = show;}
    int GetShowSelectedTrails(void)		{return(m_showSelectedTrails);}
    void SetFadeTrailPoints(int enabled)	{m_fadeTrailPoints = enabled;}
    int GetFadeTrailPoints(void) const		{return(m_fadeTrailPoints);}

    /* Flashing tracks. */
    SPxErrorCode SetEmergencyFlashHz(unsigned int hz);	/* 0 for no flash */
    unsigned int GetEmergencyFlashHz(void) const { return(m_emergencyFlashHz);}
    SPxErrorCode SetSPIFlashHz(unsigned int hz);	/* 0 for no flash */
    unsigned int GetSPIFlashHz(void) const { return(m_spiFlashHz); }
    SPxErrorCode SetHighlightFlashHz(unsigned int hz);	/* 0 for no flash */
    unsigned int GetHighlightFlashHz(void) const { return(m_highlightFlashHz); }

    /* Excluded tracks. */
    SPxErrorCode SetShowExcludedTracks(int state);
    int GetShowExcludedTracks(void) const { return m_showExcludedTracks; }

    /* Generic parameter assignment. */
    virtual int SetParameter(char *parameterName, char *parameterValue);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);

protected:
    /*
     * Protected functions.
     */

private:
    /*
     * Private variables.
     */
    /* Handle of associated decoder object. */
    SPxADSBDecoder *m_decoder;		/* Decoder */

    /* Rendering options. */
    UINT32 m_symbolLength;		/* Pixels */
    UINT32 m_vectorSeconds;		/* Length of vector */
    UINT32 m_historySeconds;		/* Length of trail history */
    int m_showAllLabels;		/* Show all vessel names */
    int m_showActivatedLabels;		/* Show activated vessel names */
    int m_showSelectedLabels;		/* Show selected vessel names */
    int m_showAllTrails;		/* Show all history trails */
    int m_showActivatedTrails;		/* Show activated history trails */
    int m_showSelectedTrails;		/* Show selected history trails */
    int m_fadeTrailPoints;		/* Fade trail points with age */
    unsigned int m_labelMaxRows;	/* Max label rows to show */
    const char *m_label;		/* Label string to render */

    /* Rendering colours */
    UINT32 m_symbolColour;		/* Colour, 0xAARRGGBB */

    /* Target flash support. */
    unsigned int m_emergencyFlashHz;	/* Zero for no flash */
    unsigned int m_spiFlashHz;		/* Zero for no flash */
    unsigned int m_highlightFlashHz;	/* Zero for no flash */

    /* Excluded targets. */
    int m_showExcludedTracks;

    /*
     * Private functions.
     */
    virtual SPxErrorCode renderAll(void);
    SPxErrorCode renderTrack(SPxADSBTrack *track);
    static int staticRenderTrack(void *decoder, void *arg, void *track);

}; /* SPxADSBRenderer */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/


#endif /* SPX_ADSB_RENDERER_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/

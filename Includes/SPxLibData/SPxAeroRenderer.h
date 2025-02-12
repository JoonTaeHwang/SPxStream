/*********************************************************************
*
* (c) Copyright 2020, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxAeroRenderer abstract class which supports the display
*	of aeronautical nav aids etc. but only via a derived class.
*
*
* Revision Control:
*   27/10/20 v1.5    REW	Support route highlighting.
*
* Previous Changes:
*   16/10/20 1.4    REW	Render frequencies and long names for VOR/NDB.
*   15/10/20 1.3    REW	Support VORs and flags to Show item groups.
*   14/10/20 1.2    REW	Add support for corridors.
*   13/10/20 1.1    REW	Initial Version.
**********************************************************************/

#ifndef _SPX_AERO_RENDERER_H
#define _SPX_AERO_RENDERER_H

/*
 * Other headers required.
 */
#include "SPxLibUtils/SPxRenderer.h"
#include "SPxLibUtils/SPxSymbol.h"


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
class SPxAeroDb;
class SPxAeroNavAid;
class SPxAeroRoute;
class SPxCriticalSection;

/*
 * Define our class, derived from the abstract renderer base class.
 */
class SPxAeroRenderer : virtual public SPxRenderer
{
public:
    /*
     * Public variables.
     */

    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    SPxAeroRenderer(SPxAeroDb *db);
    virtual ~SPxAeroRenderer(void);

    /* Mutex protection. */
    SPxCriticalSection *m_mutex;

    /* Get database. */
    SPxAeroDb *GetDb(void) { return m_db; }

    /* Route to highlight. */
    SPxErrorCode SetRoute(const SPxAeroRoute *route);
    const SPxAeroRoute *GetRoute(void) const	{ return(m_route); }

    /* Display control. */
    void SetShowCorridors(int show)		{ m_showCorridors = show; }
    int GetShowCorridors(void) const		{ return(m_showCorridors); }
    void SetShowNDBs(int show)			{ m_showNDBs = show; }
    int GetShowNDBs(void) const			{ return(m_showNDBs); }
    void SetShowRoutes(int show)		{ m_showRoutes = show; }
    int GetShowRoutes(void) const		{ return(m_showRoutes); }
    void SetShowVORs(int show)			{ m_showVORs = show; }
    int GetShowVORs(void) const			{ return(m_showVORs); }
    void SetShowWaypoints(int show)		{ m_showWaypoints = show; }
    int GetShowWaypoints(void) const		{ return(m_showWaypoints); }

    /* Symbol selection. */
    void SetSymbolNDBs(SPxSymbolShape_t sym)	{ m_symbolNDBs=sym; }
    SPxSymbolShape_t GetSymbolNDBs(void) const	{ return(m_symbolNDBs); }
    void SetSymbolVORs(SPxSymbolShape_t sym)	{ m_symbolVORs=sym; }
    SPxSymbolShape_t GetSymbolVORs(void) const	{ return(m_symbolVORs); }
    void SetSymbolWaypoints(SPxSymbolShape_t sym) { m_symbolWaypoints=sym; }
    SPxSymbolShape_t GetSymbolWaypoints(void) const
						{ return(m_symbolWaypoints); }

    /* Colour control. */
    void SetColourCorridors(UINT32 argb)	{ m_colourCorridors = argb; }
    UINT32 GetColourCorridors(void) const	{ return(m_colourCorridors); }
    void SetColourNDBs(UINT32 argb)		{ m_colourNDBs = argb; }
    UINT32 GetColourNDBs(void) const		{ return(m_colourNDBs); }
    void SetColourRoutes(UINT32 argb)		{ m_colourRoutes = argb; }
    UINT32 GetColourRoutes(void) const		{ return(m_colourRoutes); }
    void SetColourVORs(UINT32 argb)		{ m_colourVORs = argb; }
    UINT32 GetColourVORs(void) const		{ return(m_colourVORs); }
    void SetColourWaypoints(UINT32 argb)	{ m_colourWaypoints = argb; }
    UINT32 GetColourWaypoints(void) const	{ return(m_colourWaypoints); }

    /* Size control. */
    void SetSizeNDBs(UINT32 pixels)		{ m_sizeNDBs = pixels; }
    UINT32 GetSizeNDBs(void) const		{ return(m_sizeNDBs); }
    void SetSizeRoutes(UINT32 pixels)		{ m_sizeRoutes = pixels; }
    UINT32 GetSizeRoutes(void) const		{ return(m_sizeRoutes); }
    void SetSizeVORs(UINT32 pixels)		{ m_sizeVORs = pixels; }
    UINT32 GetSizeVORs(void) const		{ return(m_sizeVORs); }
    void SetSizeWaypoints(UINT32 pixels)	{ m_sizeWaypoints = pixels; }
    UINT32 GetSizeWaypoints(void) const		{ return(m_sizeWaypoints); }

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
    /* Handle of associated database object. */
    SPxAeroDb *m_db;			/* Database */

    /* Flags. */
    int m_showCorridors;		/* TRUE or FALSE */
    int m_showNDBs;			/* TRUE or FALSE */
    int m_showRoutes;			/* TRUE or FALSE */
    int m_showVORs;			/* TRUE or FALSE */
    int m_showWaypoints;		/* TRUE or FALSE */

    /* Colours */
    UINT32 m_colourCorridors;		/* Colour, 0xAARRGGBB */
    UINT32 m_colourNDBs;		/* Colour, 0xAARRGGBB */
    UINT32 m_colourRoutes;		/* Colour, 0xAARRGGBB */
    UINT32 m_colourVORs;		/* Colour, 0xAARRGGBB */
    UINT32 m_colourWaypoints;		/* Colour, 0xAARRGGBB */

    /* Sizes. */
    UINT32 m_sizeNDBs;			/* Pixels */
    UINT32 m_sizeRoutes;		/* Pixels */
    UINT32 m_sizeVORs;			/* Pixels */
    UINT32 m_sizeWaypoints;		/* Pixels */

    /* Symbol choices. */
    SPxSymbolShape_t m_symbolNDBs;	/* Choice of symbol */
    SPxSymbolShape_t m_symbolVORs;	/* Choice of symbol */
    SPxSymbolShape_t m_symbolWaypoints;	/* Choice of symbol */

    /* Route to highlight, if any. */
    const SPxAeroRoute *m_route;	/* NULL if none */

    /*
     * Private functions.
     */
    virtual SPxErrorCode renderAll(void);
    static int staticRenderItem(void *db, void *arg, void *item);
    SPxErrorCode renderItem(SPxAeroNavAid *item);
    SPxErrorCode renderCorridor(SPxAeroNavAid *item);
    SPxErrorCode renderPosition(const SPxAeroNavAid *item);
    SPxErrorCode renderRoute(const SPxAeroRoute *route);

}; /* SPxAeroRenderer */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/


#endif /* SPX_AERO_RENDERER_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/

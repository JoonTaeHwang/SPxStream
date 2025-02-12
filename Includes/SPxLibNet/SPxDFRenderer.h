/*********************************************************************
*
* (c) Copyright 2018, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for SPxDFRenderer abstract class which supports the display
*	of Direction Finder messages in an application, but only via a
*	derived class.
*
*
* Revision Control:
*   22/02/18 v1.1    REW	Initial Version.
*
* Previous Changes:
**********************************************************************/

#ifndef _SPX_DF_RENDERER_H
#define _SPX_DF_RENDERER_H

/*
 * Other headers required.
 */
/* We need SPxLibUtils for common types, callbacks, errors etc. */
#include "SPxLibUtils/SPxRenderer.h"

/* We need the SPxDFReceiver class. */
#include "SPxLibNet/SPxDFReceiver.h"


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
class SPxDFReceiver;
class SPxRenderer;

/*
 * Define our class, derived from the abstract renderer base class.
 */
class SPxDFRenderer : virtual public SPxRenderer
{
public:
    /*
     * Public variables.
     */

    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    SPxDFRenderer(SPxDFReceiver *receiver);
    virtual ~SPxDFRenderer(void);

    /* Get receiver. */
    SPxDFReceiver *GetReceiver(void) { return m_receiver; }

    /* Vector control. */
    void SetVectorLength(double m)		{ m_vectorLengthMetres = m; }
    double GetVectorLength(void)		{ return(m_vectorLengthMetres);}

    /* Colour control. */
    void SetVectorColour(UINT32 argb)		{ m_vectorColour = argb; }
    UINT32 GetVectorColour(void)		{ return(m_vectorColour); }

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
    /* Handle of associated receiver object. */
    SPxDFReceiver *m_receiver;		/* Receiver */

    /* Rendering options. */
    double m_vectorLengthMetres;	/* Length of vector */
    UINT32 m_vectorColour;		/* Colour, 0xAARRGGBB */

    /*
     * Private functions.
     */
    virtual SPxErrorCode renderAll(void);
}; /* SPxDFRenderer */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/


#endif /* SPX_DF_RENDERER_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/

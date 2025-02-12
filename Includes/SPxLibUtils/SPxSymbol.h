/*********************************************************************
*
* (c) Copyright 2011 - 2021, Cambridge Pixel Ltd.
*
* Purpose:
*   Header for SPxSymbol class.
*
* Revision Control:
*   20/05/21 v1.21   AGC	Add STAR.
*
* Previous Changes:
*   12/11/20 1.20   SP 	Add Copy().
*   16/10/20 1.19   REW	Add CROSS_8 and CONCENTRIC_RINGS.
*   26/08/20 1.18   AGC	Add IJMS symbol shapes.
*			Make various render functions public and static.
*   27/06/19 1.17   AGC	Add SPX_SYMBOL_SHAPE_NOT_SET.
*   21/02/19 1.16   BTB	Add camera and speaker symbols.
*   30/11/18 1.15   AGC	Use floating point positions.
*   19/11/18 1.14   AGC	Fix warnings.
*   19/11/18 1.13   AGC	High DPI improvements.
*   07/12/16 1.12   REW	Add dash symbol.
*   19/09/14 1.11   REW	Add radar symbol.
*   09/04/14 1.10   REW	Add explicit assignments of shape numbers.
*   08/04/14 1.9    REW	Add more symbols.
*   06/12/13 1.8    AGC	Add compass and gear symbols.
*   26/11/13 1.7    AGC	Support using SPxSymbol in SPxGwSymbol.
*   04/10/13 1.6    AGC	Simplify headers.
*   14/08/13 1.5    AGC	Add shape base on SPxImage.
*			Add vehicle shapes.
*			Support rotation.
*   14/11/12 1.4    SP 	Keep SPxSymbolShape_t enum values
*                       as documented for RDC and add comment.
*   08/02/12 1.3    AGC	Configurable line width and dash style.
*   07/07/11 1.2    SP	Support symbol selection.
*   26/04/11 1.1    SP 	Initial Version.
* 
**********************************************************************/

#ifndef _SPX_SYMBOL_H
#define _SPX_SYMBOL_H

/*
 * Other headers required.
 */

/* We need SPxLibUtils for common types, callbacks, errors etc. */
#include "SPxLibUtils/SPxError.h"
#include "SPxLibUtils/SPxRenderer.h"

/* For base class. */
#include "SPxLibUtils/SPxObj.h"

/*********************************************************************
*
*   Constants
*
**********************************************************************/

/* Number of symbol modes */
#define SPX_NUM_SYMBOL_MODES 10

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

/* Forward declarations. */
class SPxImage;

/* List of built-in symbol shapes. Only append items
 * so that enum values are not changed, as this is
 * used when object is controlled remotely (e.g. in RDC).
 */
typedef enum
{
    SPX_SYMBOL_SHAPE_NOT_SET = -1,
    SPX_SYMBOL_SHAPE_CROSS = 0,
    SPX_SYMBOL_SHAPE_DIAG_CROSS = 1,
    SPX_SYMBOL_SHAPE_TRIANGLE = 2,
    SPX_SYMBOL_SHAPE_INV_TRIANGLE = 3,
    SPX_SYMBOL_SHAPE_CIRCLE = 4,
    SPX_SYMBOL_SHAPE_CIRCLE_CROSS = 5,
    SPX_SYMBOL_SHAPE_CIRCLE_DIAG_CROSS = 6,
    SPX_SYMBOL_SHAPE_SQUARE = 7,
    SPX_SYMBOL_SHAPE_SQUARE_CROSS = 8,
    SPX_SYMBOL_SHAPE_SQUARE_DIAG_CROSS = 9,
    SPX_SYMBOL_SHAPE_DIAMOND = 10,
    SPX_SYMBOL_SHAPE_DIAMOND_CROSS = 11,
    SPX_SYMBOL_SHAPE_DIAMOND_DIAG_CROSS = 12,
    SPX_SYMBOL_SHAPE_CIRCLE_LARGE_DIAG_CROSS = 13,
    SPX_SYMBOL_SHAPE_IMAGE = 14,
    SPX_SYMBOL_SHAPE_CIRCLE_FILLED = 15,
    SPX_SYMBOL_SHAPE_HELICOPTER = 16,
    SPX_SYMBOL_SHAPE_AEROPLANE = 17,
    SPX_SYMBOL_SHAPE_CAR = 18,
    SPX_SYMBOL_SHAPE_TRUCK = 19,
    SPX_SYMBOL_SHAPE_TANK = 20,
    SPX_SYMBOL_SHAPE_PERSON = 21,
    SPX_SYMBOL_SHAPE_COMPASS = 22,
    SPX_SYMBOL_SHAPE_GEAR = 23,
    SPX_SYMBOL_SHAPE_L = 24,
    SPX_SYMBOL_SHAPE_M = 25,
    SPX_SYMBOL_SHAPE_Z = 26,
    SPX_SYMBOL_SHAPE_HORSESHOE = 27,
    SPX_SYMBOL_SHAPE_OMEGA = 28,
    SPX_SYMBOL_SHAPE_SIGMA = 29,
    SPX_SYMBOL_SHAPE_KILO = 30,
    SPX_SYMBOL_SHAPE_ARCH_UP = 31,
    SPX_SYMBOL_SHAPE_ARCH_DOWN = 32,
    SPX_SYMBOL_SHAPE_ARCH_LEFT = 33,
    SPX_SYMBOL_SHAPE_ARCH_RIGHT = 34,
    SPX_SYMBOL_SHAPE_RADAR = 35,
    SPX_SYMBOL_SHAPE_DASH = 36,
    SPX_SYMBOL_SHAPE_CAMERA = 37,
    SPX_SYMBOL_SHAPE_SPEAKER = 38,
    SPX_SYMBOL_SHAPE_IJMS_PENDING = 39,
    SPX_SYMBOL_SHAPE_IJMS_PENDING_PAIRED = 40,
    SPX_SYMBOL_SHAPE_IJMS_UNKNOWN = 41,
    SPX_SYMBOL_SHAPE_IJMS_UNKNOWN_PAIRED = 42,
    SPX_SYMBOL_SHAPE_IJMS_ASSUMED_FRIEND = 43,
    SPX_SYMBOL_SHAPE_IJMS_ASSUMED_FRIEND_PAIRED = 44,
    SPX_SYMBOL_SHAPE_IJMS_ASSUMED_HOSTILE = 45,
    SPX_SYMBOL_SHAPE_IJMS_ASSUMED_HOSTILE_PAIRED = 46,
    SPX_SYMBOL_SHAPE_IJMS_HOSTILE = 47,
    SPX_SYMBOL_SHAPE_IJMS_HOSTILE_PAIRED = 48,
    SPX_SYMBOL_SHAPE_IJMS_FRIEND = 49,
    SPX_SYMBOL_SHAPE_IJMS_FRIEND_PAIRED = 50,
    SPX_SYMBOL_SHAPE_IJMS_FRIEND_HELO = 51,
    SPX_SYMBOL_SHAPE_IJMS_FRIEND_HELO_PAIRED = 52,
    SPX_SYMBOL_SHAPE_IJMS_SPECIAL_MISSION = 53,
    SPX_SYMBOL_SHAPE_IJMS_SPECIAL_MISSION_PAIRED = 54,
    SPX_SYMBOL_SHAPE_IJMS_SPECIAL_STRIKE = 55,
    SPX_SYMBOL_SHAPE_IJMS_SPECIAL_STRIKE_PAIRED = 56,
    SPX_SYMBOL_SHAPE_IJMS_INTERCEPTOR = 57,
    SPX_SYMBOL_SHAPE_IJMS_INTERCEPTOR_PAIRED = 58,
    SPX_SYMBOL_SHAPE_CROSS_8 = 59,
    SPX_SYMBOL_SHAPE_CONCENTRIC_RINGS = 60,
    SPX_SYMBOL_SHAPE_STAR = 61,
    SPX_SYMBOL_SHAPE_MAX, /* MUST BE LAST NORMAL ENTRY */

    /* For backwards compatibility. */
    SPX_GW_SYM_CROSS = SPX_SYMBOL_SHAPE_CROSS,
    SPX_GW_SYM_DIAG_CROSS = SPX_SYMBOL_SHAPE_DIAG_CROSS,
    SPX_GW_NUM_SYMBOLS = SPX_SYMBOL_SHAPE_MAX

} SPxSymbolShape_t;

/* Symbol attributes for a specific mode */
typedef struct
{
    SPxSymbolShape_t shape;	/* Shape to use */
    SPxRendererDash_t dash;	/* Type of line to use */
    double lineWidth;		/* Line width */
    double sizePixels;		/* Size in pixels */
    UINT32 colour;		/* ARGB colour */
    double rotDegs;		/* Clockwise rotation in degrees. */
    SPxImage *image;		/* Image (only valid if shape is IMAGE) */

} SPxSymbolAttr_t;

/* Need to forward-declare other classes in case headers are in wrong order.*/
class SPxRenderer;

/*
 * Define our class.
 */
class SPxSymbol : public SPxObj
{
public:
    /*
     * Public variables.
     */

    /*
     * Public functions.
     */
    /* Constructor and destructor. */
    SPxSymbol(void);
    virtual ~SPxSymbol(void);

    /* Copy from another symbol. */
    SPxErrorCode Copy(SPxSymbol *symbol);

    /* Configure the symbol for a specific mode */
    SPxErrorCode SetShape(unsigned int mode, SPxSymbolShape_t shape, SPxImage *image=NULL);
    SPxErrorCode SetDash(unsigned int mode, SPxRendererDash_t dash);
    SPxErrorCode SetLineWidth(unsigned int mode, double width);
    SPxErrorCode SetSizePixels(unsigned int mode, unsigned int size);
    SPxErrorCode SetSizePixels(unsigned int mode, double size);
    SPxErrorCode SetColour(unsigned int mode, UINT32 col);
    SPxErrorCode SetRotation(unsigned int mode, double rotDegs);
    SPxSymbolShape_t GetShape(unsigned int mode) const;
    SPxImage *GetImage(unsigned int mode);
    SPxRendererDash_t GetDash(unsigned int mode);
    double GetLineWidth(unsigned int mode);
    double GetSizePixels(unsigned int mode);
    UINT32 GetColour(unsigned int mode);
    double GetRotation(unsigned int mode);

    /* Configure the symbol for all modes */
    SPxErrorCode SetShapeAllModes(SPxSymbolShape_t shape, SPxImage *image=NULL);
    SPxErrorCode SetDashAllModes(SPxRendererDash_t dash);
    SPxErrorCode SetLineWidthAllModes(double lineWidth);
    SPxErrorCode SetSizePixelsAllModes(unsigned int size);
    SPxErrorCode SetSizePixelsAllModes(double size);
    SPxErrorCode SetColourAllModes(UINT32 col);
    SPxErrorCode SetRotationAllModes(double rotDegs);
    void SetIsSelected(int isSelected) { m_isSelected = isSelected; }
    int GetIsSelected(void) { return m_isSelected; }

    /* Set the symbol mode to use during rendering */
    SPxErrorCode SetMode(unsigned int mode);
    unsigned int GetMode(void);

    /* Render the symbol */
    SPxErrorCode Render(SPxRenderer *renderer,
                        unsigned int xPixels,
                        unsigned int yPixels);
    SPxErrorCode Render(SPxRenderer *renderer,
                        double xPixels,
                        double yPixels);

    /* Rendering functions. */
    static SPxErrorCode RenderSymbol(SPxRenderer *r,
                                     const SPxSymbolAttr_t *attr,
                                     double xPixels,
                                     double yPixels);

    static SPxErrorCode RenderSelectionMarkers(SPxRenderer *r,
                                               const SPxSymbolAttr_t *attr,
                                               double xPixels,
                                               double yPixels);

    static void RotatePoints(SPxRendererXYD_t *points,
                             double rotDegs,
                             double xPixels,
                             double yPixels,
                             unsigned int numPoints = 1);

    /* Generic parameter assignment. */
    virtual int SetParameter(char *parameterName, char *parameterValue);
    virtual int GetParameter(char *parameterName, char *valueBuf, int bufLen);

    /* Symbol configuration */
    

protected:
    /*
     * Protected functions.
     */

private:
    /*
     * Private variables.
     */

    /* The current mode number */
    unsigned int m_mode;

    /* Symbol attributes for each different mode */
    SPxSymbolAttr_t m_modeAttr[SPX_NUM_SYMBOL_MODES];

    /* Show symbol as selected */
    int m_isSelected;

    /*
     * Private functions.
     */

}; /* SPxSymbol */


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* SPX_SYMBOL_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/

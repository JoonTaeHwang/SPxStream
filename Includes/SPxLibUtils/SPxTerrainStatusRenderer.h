/*********************************************************************
*
* (c) Copyright 2018, Cambridge Pixel Ltd.
*
* Purpose:
*   Header for SPxTerrainStatusRenderer base class which supports
*   the display of loaded/available terrain areas, but only via a
*   derived class.
*
* Revision Control:
*   03/05/18 v1.1    AGC	Initial Version.
*
* Previous Changes:
**********************************************************************/

#ifndef SPX_TERRAIN_STATUS_RENDERER_H_
#define SPX_TERRAIN_STATUS_RENDERER_H_

/* We need SPxAutoPtr class. */
#include "SPxLibUtils/SPxAutoPtr.h"

/* We need SPxError for error types. */
#include "SPxLibUtils/SPxError.h"

/* We need SPxRenderer for our base class. */
#include "SPxLibUtils/SPxRenderer.h"

class SPxTerrainDatabase;

class SPxTerrainStatusRenderer : virtual public SPxRenderer
{
public:
    /* Constructor and destructor. */
    explicit SPxTerrainStatusRenderer(SPxTerrainDatabase *terrainDB);
    virtual ~SPxTerrainStatusRenderer(void);

private:
    SPxTerrainDatabase *m_terrainDB;

    virtual SPxErrorCode renderAll(void);

}; /* SPxTerrainStatusRenderer */

/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* SPX_TERRAIN_STATUS_RENDERER_H_ */

/*********************************************************************
*
* End of file
*
**********************************************************************/

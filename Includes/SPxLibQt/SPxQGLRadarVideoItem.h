/*********************************************************************
 *
 * (c) Copyright 2013, Cambridge Pixel Ltd.
 *
 * File: $RCSfile: SPxQGLRadarVideoItem.h,v $
 * ID: $Id: SPxQGLRadarVideoItem.h,v 1.2 2013/10/07 13:29:02 rew Exp $
 *
 * Purpose:
 *   SPxQGLRadarVideoItem class header file.
 *
 * Revision Control:
 *   07/10/13 v1.2    AGC	Simplify headers.
 *
 * Previous Changes:
 *   06/06/13 1.1    SP         Initial version.
 *
 *********************************************************************/

/* Avoid multiple inclusion. */
#ifndef SPX_Q_GL_RADAR_VIDEO_ITEM_H
#define SPX_Q_GL_RADAR_VIDEO_ITEM_H

/* Qt headers. */
#include <QGLContext>

/* SPx headers. */
#include "SPxLibQt/SPxQRadarVideoItem.h"

/* Forward declarations. */
class SPxScSourceLocal;
class SPxScDestTexture;

/* SPxQGLRadarVideoItem class. */
class SPxQGLRadarVideoItem : public SPxQRadarVideoItem
{
public:

    /*
     * Public functions.
     */

    /* Constructor and destructor. */
    SPxQGLRadarVideoItem(QGraphicsItem *parent,
                         SPxScSourceLocal *spxSc,
                         SPxScDestTexture *spxTexture);
    virtual ~SPxQGLRadarVideoItem(void);

    /*
     * Public variables.
     */

protected:

    /*
     * Protected functions.
     */

    /* Base class functions. */
    virtual void render(QPainter *painter, QWidget *widget);

private:

    /*
     * Private functions.
     */

    /*
     * Private variables.
     */

    /* OpenGL. */
    QGLContext *m_glContext;
    SPxScDestTexture *m_spxTexture;
};

#endif /* SPX_Q_GL_RADAR_VIDEO_ITEM_H */

/*********************************************************************
 *
 *      End of file
 *
 *********************************************************************/

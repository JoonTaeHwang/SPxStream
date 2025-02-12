/*********************************************************************
 *
 * (c) Copyright 2013, 2016, 2021, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   SPxQGLRadarWidget class header file.
 *
 * Revision Control:
 *   27/05/21 v1.5    AGC       Fix deprecated WindowFlags warnings.
 *
 * Previous Changes:
 *   11/05/16 1.4    AGC    Support FBOs as alternative to pixmaps.
 *			
 *   07/10/13 1.3    AGC    Simplify headers.
 *   05/09/13 1.2    AGC    Fix clang warning.
 *   06/06/13 1.1    SP     Initial version.
 *
 *********************************************************************/

/* Avoid multiple inclusion. */
#ifndef SPX_Q_GL_RADAR_WIDGET_H
#define SPX_Q_GL_RADAR_WIDGET_H

/* Qt headers. */
#include <QGLWidget>

/* SPx headers. */
#include "SPxLibQt/SPxQRadarWidgetBase.h"
#include "SPxLibUtils/SPxAutoPtr.h"

/* Forward declarations. */
class SPxScSourceLocal;
class SPxScDestTexture;
class QGLFramebufferObject;

/* SPxQGLRadarWidget class. */
class SPxQGLRadarWidget : public SPxQRadarWidgetBase, public QGLWidget
{
public:

    /*
     * Public functions.
     */

    /* Constructors and destructor. */
    SPxQGLRadarWidget(QWidget *parent=0,
                      const QGLWidget *shareWidget=0,
                      Qt::WindowFlags f=Qt::WindowFlags());

    SPxQGLRadarWidget(QGLContext * context,
                      QWidget *parent=0,
                      const QGLWidget *shareWidget=0,
                      Qt::WindowFlags f=Qt::WindowFlags());

    SPxQGLRadarWidget(const QGLFormat &format,
                      QWidget *parent=0,
                      const QGLWidget *shareWidget=0,
                      Qt::WindowFlags f=Qt::WindowFlags());

    virtual ~SPxQGLRadarWidget(void);

    /* Add radar scan converter and destination. */
    SPxErrorCode addRadar(SPxScSourceLocal *sc, 
                          SPxScDestTexture *texture);

    /* Set underlay and overlay FBOs. */
    virtual SPxErrorCode setUlayFBO(QGLFramebufferObject *fbo);
    virtual SPxErrorCode setOlayFBO(QGLFramebufferObject *fbo);

    /* Composite the display. */
    virtual SPxErrorCode compositeDisplay(void);

protected:

    /*
     * Protected functions.
     */

    virtual void paintEvent(QPaintEvent *event);
    virtual void resizeEvent(QResizeEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void wheelEvent(QWheelEvent *event);

    /*
     * Protected variables.
     */

private:

    /*
     * Private types.
     */

    /*
     * Private functions.
     */

    void initCommon(void);

    /*
     * Private variables.
     */

    /* Private structure. */
    struct impl;
    SPxAutoPtr<impl> m_p;

};

#endif /* SPX_Q_GL_RADAR_WIDGET_H */

/*********************************************************************
 *
 *      End of file
 *
 *********************************************************************/

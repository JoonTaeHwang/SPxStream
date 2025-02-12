/*********************************************************************
 *
 * (c) Copyright 2021, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   SPxQOpenGLRadarWidget class header file.
 *
 * Revision Control:
 *  06/07/21 v1.3    AGC	Remove non-existent checkGL() function.
 *
 * Previous Changes:
 *  27/05/21 1.2    AGC	Fix deprecated WindowFlags warnings.
 *  28/04/21 1.1    AGC	Initial version.
 *
 *********************************************************************/

/* Avoid multiple inclusion. */
#ifndef SPX_Q_OPENGL_RADAR_WIDGET_H
#define SPX_Q_OPENGL_RADAR_WIDGET_H

/* Qt headers. */
#include <QOpenGLWidget>

/* SPx headers. */
#include "SPxLibQt/SPxQRadarWidgetBase.h"
#include "SPxLibUtils/SPxAutoPtr.h"

/* Forward declarations. */
class SPxScSourceLocal;
class SPxScDestTexture;
class QOpenGLFramebufferObject;

/* SPxQOpenGLRadarWidget class. */
class SPxQOpenGLRadarWidget : public SPxQRadarWidgetBase, public QOpenGLWidget
{
public:

    /*
     * Public functions.
     */

    /* Constructors and destructor. */
    SPxQOpenGLRadarWidget(QWidget *parent=0,
                          Qt::WindowFlags f=Qt::WindowFlags());

    virtual ~SPxQOpenGLRadarWidget(void);

    /* Add radar scan converter and destination. */
    SPxErrorCode addRadar(SPxScSourceLocal *sc, 
                          SPxScDestTexture *texture);

    /* Set underlay and overlay FBOs. */
    virtual SPxErrorCode setUlayFBO(QOpenGLFramebufferObject *fbo);
    virtual SPxErrorCode setOlayFBO(QOpenGLFramebufferObject *fbo);

    /* Composite the display. */
    virtual SPxErrorCode compositeDisplay(void);

protected:

    /*
     * Protected functions.
     */

    virtual void initializeGL(void);
    virtual void paintGL(void);
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

    void createShaderProgram(void);
    void createGeometry(void);

    /*
     * Private variables.
     */

    /* Private structure. */
    struct impl;
    SPxAutoPtr<impl> m_p;

};

#endif /* SPX_Q_OPENGL_RADAR_WIDGET_H */

/*********************************************************************
 *
 *      End of file
 *
 *********************************************************************/

/*********************************************************************
 *
 * (c) Copyright 2013, 2021, Cambridge Pixel Ltd.
 *
 * Purpose:
 *   SPxQRadarWidget class header file.
 *
 *   This class results in very high CPU load and intended for 
 *   internal use only.
 *
 * Revision Control:
 *   27/05/21 v1.7    AGC       Fix deprecated WindowFlags warnings.
 *
 * Previous Changes:
 *   07/10/13 1.6    AGC    Simplify headers.
 *   05/09/13 1.5    AGC    Fix clang warning.
 *   06/06/13 1.4    SP     Move OpenGL code to SPxQGLRadarWidget.
 *                          Move common code to SPxQRadarWidgetBase.
 *   24/05/13 1.3    SP     Fix comment cut/paste error.
 *   09/05/13 1.2    SP     Minor changes.
 *   03/05/13 1.1    SP     Initial version.
 *
 *********************************************************************/

/* Avoid multiple inclusion. */
#ifndef SPX_Q_RADAR_WIDGET_H
#define SPX_Q_RADAR_WIDGET_H

/* Qt headers. */
#include <QWidget>

/* SPx headers. */
#include "SPxLibQt/SPxQRadarWidgetBase.h"
#include "SPxLibUtils/SPxAutoPtr.h"

/* Forward declarations. */
class SPxScSourceLocal;
class SPxScDestBitmap;
struct SPxQRadarWidgetPriv;

/* SPxQRadarWidget class. */
class SPxQRadarWidget : public SPxQRadarWidgetBase, public QWidget
{
public:

    /*
     * Public functions.
     */

    /* Constructors and destructor. */
    SPxQRadarWidget(QWidget *parent=0,
                    Qt::WindowFlags f=Qt::WindowFlags());

    virtual ~SPxQRadarWidget(void);

    /* Add radar scan converter and destination. */
    SPxErrorCode addRadar(SPxScSourceLocal *sc, 
                          SPxScDestBitmap *bitmap);

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
    SPxAutoPtr<SPxQRadarWidgetPriv> m_p;

};

#endif /* SPX_Q_RADAR_WIDGET_H */

/*********************************************************************
 *
 *      End of file
 *
 *********************************************************************/

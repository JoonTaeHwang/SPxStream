/*********************************************************************
 *
 * (c) Copyright 2013, Cambridge Pixel Ltd.
 *
 * File: $RCSfile: SPxQRadarVideoItem.h,v $
 * ID: $Id: SPxQRadarVideoItem.h,v 1.6 2013/12/02 16:38:43 rew Exp $
 *
 * Purpose:
 *   SPxQRadarVideoItem class header file.
 *
 * Revision Control:
 *   02/12/13 v1.6    SP 	 Add set/getMetresPerSceneUnit().
 *
 * Previous Changes:
 *   07/10/13 1.5    AGC	Simplify headers.
 *   06/06/13 1.4    SP         Move OpenGL code to SPxQGLRadarVideoItem.
 *   09/05/13 1.3    SP         Add update() with discrete args.
 *   03/05/13 1.2    SP 	Remove unused includes.
 *   25/04/13 1.1    SP 	Initial version.
 *
 *********************************************************************/

/* Avoid multiple inclusion. */
#ifndef SPX_Q_RADAR_VIDEO_ITEM_H
#define SPX_Q_RADAR_VIDEO_ITEM_H

/* Qt headers. */
#include <QGraphicsItem>

/* SPx headers. */
#include "SPxLibUtils/SPxError.h"

/* Forward declarations. */
class SPxScSourceLocal;
class SPxScDestBitmap;

/* SPxQGLRadarVideoItem class. */
class SPxQRadarVideoItem : public QGraphicsItem
{
public:

    /*
     * Public functions.
     */

    /* Constructor and destructor. */
    SPxQRadarVideoItem(QGraphicsItem *parent,
                       SPxScSourceLocal *spxSc,
                       SPxScDestBitmap *spxBitmap);
    virtual ~SPxQRadarVideoItem(void);

    /* Configuration. */
    virtual SPxErrorCode setMetresPerSceneUnit(double numMetres);
    virtual double getMetresPerSceneUnit(void) { return m_metresPerSceneUnit; }
    virtual void syncToGraphicsView(int isEnabled);
    virtual void setBoundingRect(QRectF *rect);
    virtual void update(const QRectF & rect = QRectF());
    virtual void update(qreal x, qreal y, qreal width, qreal height);

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

    /* Base class functions. */
    virtual QRectF boundingRect() const;
    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget);

    void updateBoundingRect(QRectF *userRect=NULL);
    void updateWindowSize(QPainter *painter);
    void updateView(QPainter *painter);

    /*
     * Private variables.
     */

    /* Radar video rendering. */
    SPxScDestBitmap *m_spxBitmap;
    SPxScSourceLocal *m_spxSc;

    /* QImage mapped to the bitmap. */
    QImage *m_image;

    /* Scaling between scene units and metres. */
    double m_metresPerSceneUnit;

    /* Current scan converter view. */
    double m_viewWidthMetres;
    double m_viewCentreXMetres;
    double m_viewCentreYMetres;

    /* Update scan converter view from graphics view? */
    int m_isSyncToGraphicsView;

    /* Auto calculate the bounding rect? */
    int m_isAutoBoundingRect;

    /* Radar coverage bounding rect in scene units. */
    QRectF m_boundingRect;
};

#endif /* SPX_Q_RADAR_VIDEO_ITEM_H */

/*********************************************************************
 *
 *      End of file
 *
 *********************************************************************/

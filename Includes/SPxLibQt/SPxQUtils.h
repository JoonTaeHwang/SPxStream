/*********************************************************************
*
* (c) Copyright 2021, Cambridge Pixel Ltd.
*
* Purpose:
*   Header file for utility functions.
*
* Revision Control:
*   13/09/21 v1.1    SP 	Initial version.
*
* Previous Changes:
*
**********************************************************************/

#ifndef _SPX_Q_UTILS_H
#define _SPX_Q_UTILS_H

/*
 * Other headers required.
 */

/* Qt headers. */
#include <QtGlobal>

/* Forward declare other classes required. */
class QMouseEvent;
class QWheelEvent;

/*********************************************************************
*
*   Class definitions
*
**********************************************************************/


/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

qreal SPxQGetEventX(QMouseEvent *evt);
qreal SPxQGetEventX(QWheelEvent *evt);
qreal SPxQGetEventY(QMouseEvent *evt);
qreal SPxQGetEventY(QWheelEvent *evt);
int SPxQGetWheelEventDelta(QWheelEvent *evt);

#endif /* _SPX_Q_UTILS_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/

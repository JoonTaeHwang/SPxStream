/*********************************************************************
*
* (c) Copyright 2015, 2017, Cambridge Pixel Ltd.
*
* File: $RCSfile: SPxPopupMenuQt.h,v $
* ID: $Id: SPxPopupMenuQt.h,v 1.2 2017/01/13 15:21:44 rew Exp $
*
* Purpose:
*   Header file for SPxPopupMenuQt class.
*
* Revision Control:
*   13/01/17 v1.2    AGC	Implement base class virtual function changes.
*
* Previous Changes:
*   05/02/15 1.1    JP Initial Version.
*
**********************************************************************/

#ifndef _SPX_POPUP_MENU_QT_H
#define _SPX_POPUP_MENU_QT_H

/* Library headers. */
#include "SPxLibUtils/SPxError.h"
#include "SPxLibUtils/SPxPopupMenu.h"

/* Qt headers. */
#include <QObject>

/* Forward declarations. */
class QMenu;
class QAction;

template <class Key, class T>
class QMap;

/* Define our class. */
class SPxPopupMenuQt : private QObject, public SPxPopupMenu
{
    Q_OBJECT
public:
    /* Public functions. */
    SPxPopupMenuQt(void);
    virtual ~SPxPopupMenuQt(void);

    virtual int AppendItem(int enabled, int id, const char *label,
			   SPxPopupMenuFn fn, void *userArg,
			   SPxMouseInterface *iface=NULL,
			   const SPxPopupMenu *subMenu=NULL);
    virtual void AppendSeparator(SPxMouseInterface *iface=NULL);
    virtual SPxErrorCode ShowMenu(void);
    virtual SPxPopupMenu *CreateMenu(void);

private:

    /* Private functions. */
    /* Avoid objs copies of objs of this class. */
    SPxPopupMenuQt(const SPxPopupMenuQt& popupMenu);
    SPxPopupMenuQt& operator=( const SPxPopupMenuQt& popupMenu);

    /* Private variables. */
    QMenu *m_menu;
    QMap<int, QAction*>* m_map;

private slots:
    void onMenuItemClicked(QAction* action);

}; /* SPxPopupMenuGtk */

#endif /* _SPX_POPUP_MENU_QT_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/

/*********************************************************************
*
* (c) Copyright 2012 - 2020, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for re-usable Network Control GTK dialog.
*
*
* Revision Control:
*   22/01/20 v1.7    AGC	Use new SPxGtkIncludes header.
*
* Previous Changes:
*   07/01/20 1.6    AGC	Silence deprecated warnings.
*   25/06/19 1.5    AGC	Make Sync() function public.
*   06/08/18 1.4    AGC	Suppress gcc 8 warnings from GTK headers.
*   18/07/14 1.3    AGC	Avoid warnings in glib headers.
*   07/09/12 1.2    AGC	Remove combobox popdown handler.
*   31/08/12 1.1    AGC	Initial Version.
**********************************************************************/

#ifndef _SPX_GTK_GUI_NET_CTRL_H
#define _SPX_GTK_GUI_NET_CTRL_H

/*
 * Other headers required.
 */

/* For GTK+ enumerations/types. */
#include "SPxLibX11/SPxGtkIncludes.h"

/* For SPxAutoPtr. */
#include "SPxLibUtils/SPxAutoPtr.h"

/* For base class. */
#include "SPxGtkGuiCtrl.h"

/*********************************************************************
*
*   Type definitions
*
**********************************************************************/

/* Forward declarations. */
class SPxNetAddr;

class SPxGtkGuiNetCtrl : public SPxGtkGuiCtrl
{
public:
    /* Public functions. */
    explicit SPxGtkGuiNetCtrl(SPxObj *obj, SPxNetAddr *netObj, 
	int receipt=TRUE);
    virtual ~SPxGtkGuiNetCtrl(void);

    void SetTitle(const char *title);
    void SetStandardAddress(const char *addr, int port);

    /* GTK callbacks. */
    void setAddrButtonClicked(GtkButton *button);
    void interfaceComboChanged(GtkComboBox *combo);

    virtual SPxErrorCode Sync(void);

private:
    /* Private variables. */
    struct impl;
    SPxAutoPtr<impl> m_p;

    /* Private functions. */
    static int onTimerWrapper(void *data);
    int onTimer(void);
    void updateInterfaceCombo(void);
    void updateWarnLabel(void);
    static int netDeletedWrapper(void *invokingObject,
				 void *userObject, void *arg);
    int netDeleted(SPxObj *netObj);
    static int getComboBoxCount(GtkComboBox *combo);
    
}; /* SPxGtkGuiCtrl */

/* Scenario Generator re-usable dialog callbacks. */
#ifdef __cplusplus
extern "C"
{
#if 0 /* Defeat emacs auto indent */
}
#endif
#endif /* __cplusplus */

void on_net_set_addr_button_clicked(GtkButton *button,
                                    gpointer user_data);

void on_net_interface_combo_changed(GtkComboBox *combo,
				    gpointer user_data);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _SPX_GTK_GUI_NET_CTRL_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/

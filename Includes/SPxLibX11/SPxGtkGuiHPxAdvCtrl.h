/*********************************************************************
*
* (c) Copyright 2013, 2018, 2020, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for re-usable advanced HPx source GTK dialog.
*
*
* Revision Control:
*   22/01/20 v1.3    AGC	Use new SPxGtkIncludes header.
*
* Previous Changes:
*   14/09/18 1.2    AGC	Updates to support HPx-400 features.
*   08/03/13 1.1    AGC	Initial Version.
**********************************************************************/

#ifndef _SPX_GTK_GUI_HPX_ADV_CTRL_H
#define _SPX_GTK_GUI_HPX_ADV_CTRL_H

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
class SPxGtkGuiHPxCtrl;
class SPxHPx100Source;

class SPxGtkGuiHPxAdvCtrl : public SPxGtkGuiCtrl
{
public:
    /* Public functions. */
    explicit SPxGtkGuiHPxAdvCtrl(SPxGtkGuiHPxCtrl *parent,
				 SPxHPx100Source *srcHw,
				 int fixedStreamIdx=-1);
    virtual ~SPxGtkGuiHPxAdvCtrl(void);

    void Show(GtkWindow *parent);

    /* GTK callbacks. */
    void interruptsSpinbuttonValueChanged(GtkSpinButton *spinbutton);
    void acpVrefSpinbuttonValueChanged(GtkSpinButton *spinbutton);
    void arpVrefSpinbuttonValueChanged(GtkSpinButton *spinbutton);
    void trgVrefSpinbuttonValueChanged(GtkSpinButton *spinbutton);
    void sig1VrefSpinbuttonValueChanged(GtkSpinButton *spinbutton);
    void sig2VrefSpinbuttonValueChanged(GtkSpinButton *spinbutton);
    void sig3VrefSpinbuttonValueChanged(GtkSpinButton *spinbutton);
    void streamRadiobuttonToggled(GtkRadioButton *button);
    void statsEnableCheckbuttonToggled(GtkToggleButton *button);
    void digEnableCheckbuttonToggled(GtkToggleButton *button);
    void advCtrlWindowHide(GtkWidget *widget);

private:
    /* Private variables. */
    struct impl;
    SPxAutoPtr<impl> m_p;

    /* Private functions. */
    virtual SPxErrorCode Sync(void);
    static int onTimerWrapper(void *data);
    int onTimer(void);
    void updateStatusDisplay(void);
    void updateStatisticsDisplay(void);
    static int hpxDeletedWrapper(void *invokingObject,
				 void *userObject, void *arg);
    int hpxDeleted(SPxHPx100Source *srcHw);

}; /* SPxGtkGuiHPxCtrl */

/* HPx source re-usable dialog callbacks. */
#ifdef __cplusplus
extern "C"
{
#if 0 /* Defeat emacs auto indent */
}
#endif
#endif /* __cplusplus */

/*
* These prototypes for GTK callback functions are not strictly
* necessary. The callbacks are linked at runtime by LibGlade
* (or GtkBuilder) and so are never actually referenced in the code.
* However the intel compiler will generate a warning if a prototype
* is missing (although gcc will not).
*/

/* HPx source advanced callbacks. */
extern void on_src_hw_interrupts_spinbutton_value_changed(GtkSpinButton *spinbutton,
							  gpointer user_data);
extern void on_src_hw_acp_vref_spinbutton_value_changed(GtkSpinButton *spinbutton,
							gpointer user_data);
extern void on_src_hw_arp_vref_spinbutton_value_changed(GtkSpinButton *spinbutton,
							gpointer user_data);
extern void on_src_hw_trg_vref_spinbutton_value_changed(GtkSpinButton *spinbutton,
							gpointer user_data);
extern void on_src_hw_sig1_vref_spinbutton_value_changed(GtkSpinButton *spinbutton,
							 gpointer user_data);
extern void on_src_hw_sig2_vref_spinbutton_value_changed(GtkSpinButton *spinbutton,
							 gpointer user_data);
extern void on_src_hw_sig3_vref_spinbutton_value_changed(GtkSpinButton *spinbutton,
							 gpointer user_data);
extern void on_src_hw_adv_stream_radiobutton_toggled(GtkRadioButton *button,
						     gpointer user_data);
extern void on_src_hw_stats_enable_checkbutton_toggled(GtkToggleButton *button,
						       gpointer user_data);
extern void on_src_hw_adv_dig_enable_checkbutton_toggled(GtkToggleButton *button,
							 gpointer user_data);
extern void on_src_hw_adv_ctrl_window_hide(GtkWidget *widget,
					   gpointer user_data);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _SPX_GTK_GUI_HPX_ADV_CTRL_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/

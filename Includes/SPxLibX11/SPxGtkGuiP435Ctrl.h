/*********************************************************************
*
* (c) Copyright 2019, 2020, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for re-usable P435 Source Control GTK dialog.
*
*
* Revision Control:
*   22/01/20 v1.4    AGC	Use new SPxGtkIncludes header.
*
* Previous Changes:
*   16/07/19 1.3    AGC	Use consistent new line characters.
*   15/07/19 1.2    AGC	Simplify address configuration.
*			Add frequency band control.
*   25/06/19 1.1    AGC	Initial Version.
**********************************************************************/

#ifndef _SPX_GTK_GUI_P435_CTRL_H
#define _SPX_GTK_GUI_P435_CTRL_H

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
class SPxNetworkReceiveP435;
class SPxGtkGuiNetCtrl;

class SPxGtkGuiP435Ctrl : public SPxGtkGuiCtrl
{
public:
    /* Public functions. */
    explicit SPxGtkGuiP435Ctrl(SPxNetworkReceiveP435 *obj);
    virtual ~SPxGtkGuiP435Ctrl(void);

    /* GTK callbacks. */
    void waveformComboChanged(GtkComboBox *combo);
    void narrowFreqSpinValueChanged(GtkSpinButton *spinbutton);
    void wideFreqSpinValueChanged(GtkSpinButton *spinbutton);

private:
    /* Private variables. */
    SPxNetworkReceiveP435 *m_obj;   /* The object we are controlling. */
    SPxAutoPtr<SPxGtkGuiNetCtrl> m_controlDlg;

    /* Private functions. */
    virtual SPxErrorCode Sync(void);
    static int objDeletedWrapper(void *invokingObject,
				 void *userObject, void *arg);
    int objDeleted(SPxObj *Obj);

}; /* SPxGtkGuiCtrl */

/* P435 re-usable dialog callbacks. */
#ifdef __cplusplus
extern "C"
{
#if 0 /* Defeat emacs auto indent */
}
#endif
#endif /* __cplusplus */

extern void on_p435_waveform_combo_changed(GtkComboBox *combo,
					   gpointer user_data);
extern void on_p435_narrow_freq_spin_value_changed(GtkSpinButton *spinbutton,
						   gpointer user_data);
extern void on_p435_wide_freq_spin_value_changed(GtkSpinButton *spinbutton,
						 gpointer user_data);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _SPX_GTK_GUI_P435_CTRL_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/

/*********************************************************************
*
* (c) Copyright 2013, 2020, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for re-usable HPx-300 Radar Output Card Control GTK dialog.
*
*
* Revision Control:
*   22/01/20 v1.2    AGC	Use new SPxGtkIncludes header.
*
* Previous Changes:
*   05/09/13 1.1    AGC	Initial Version.
**********************************************************************/

#ifndef _SPX_GTK_GUI_ROC_CTRL_H
#define _SPX_GTK_GUI_ROC_CTRL_H

/*
* Other headers required.
*/

/* For GTK+ enumerations/types. */
#include "SPxLibX11/SPxGtkIncludes.h"

/* For base class. */
#include "SPxGtkGuiCtrl.h"

/*********************************************************************
*
*   Type definitions
*
**********************************************************************/

/* Forward declarations. */
class SPxROC;

class SPxGtkGuiROCCtrl : public SPxGtkGuiCtrl
{
public:
    /* Public functions. */
    explicit SPxGtkGuiROCCtrl(SPxROC *obj);
    virtual ~SPxGtkGuiROCCtrl(void);
    void UpdateStatusVals(void);

    /* GTK callbacks. */
    void restartBtnClicked(GtkButton *btn);
    void testPatternComboChanged(GtkComboBox *combo);
    void gainASpinChanged(GtkSpinButton *btn);
    void gainBSpinChanged(GtkSpinButton *btn);
    void offsetASpinChanged(GtkSpinButton *btn);
    void offsetBSpinChanged(GtkSpinButton *btn);
    void trgSpinChanged(GtkSpinButton *btn);
    void acpSpinChanged(GtkSpinButton *btn);
    void arpSpinChanged(GtkSpinButton *btn);
    void sig1SpinChanged(GtkSpinButton *btn);
    void sig2SpinChanged(GtkSpinButton *btn);
    void shmSpinChanged(GtkSpinButton *btn);

private:
    /* Private variables. */
    SPxROC *m_obj;   		/* The object we are controlling. */
    int m_timer;		/* Timer for updating GUI. */

    /* Private functions. */
    virtual SPxErrorCode Sync(void);
    static int onTimerWrapper(void *data);
    int onTimer(void);
    static int objDeletedWrapper(void *invokingObject,
				 void *userObject, void *arg);
    int objDeleted(SPxObj *Obj);

}; /* SPxGtkGuiROCCtrl */

/* ROC re-usable dialog callbacks. */
#ifdef __cplusplus
extern "C"
{
#if 0 /* Defeat emacs auto indent */
}
#endif
#endif /* __cplusplus */

void on_roc_restart_btn_clicked(GtkButton *button,
				gpointer user_data);

void on_roc_test_pattern_combo_changed(GtkComboBox *combo,
				       gpointer user_data);

void on_roc_gain_a_spin_value_changed(GtkSpinButton *button,
				      gpointer user_data);

void on_roc_gain_b_spin_value_changed(GtkSpinButton *button,
				      gpointer user_data);

void on_roc_offset_a_spin_value_changed(GtkSpinButton *button,
					gpointer user_data);

void on_roc_offset_b_spin_value_changed(GtkSpinButton *button,
					gpointer user_data);

void on_roc_trg_spin_value_changed(GtkSpinButton *button,
				   gpointer user_data);

void on_roc_acp_spin_value_changed(GtkSpinButton *button,
				   gpointer user_data);

void on_roc_arp_spin_value_changed(GtkSpinButton *button,
				   gpointer user_data);

void on_roc_sig1_spin_value_changed(GtkSpinButton *button,
				    gpointer user_data);

void on_roc_sig2_spin_value_changed(GtkSpinButton *button,
				    gpointer user_data);

void on_roc_shm_spin_value_changed(GtkSpinButton *button,
				   gpointer user_data);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _SPX_GTK_GUI_ROC_CTRL_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/

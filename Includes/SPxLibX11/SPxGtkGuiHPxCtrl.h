/*********************************************************************
*
* (c) Copyright 2013, 2018, 2020, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for re-usable HPx source GTK dialog.
*
*
* Revision Control:
*   22/01/20 v1.3    AGC	Use new SPxGtkIncludes header.
*
* Previous Changes:
*   14/09/18 1.2    AGC	Updates to support HPx-400 features.
*   08/03/13 1.1    AGC	Initial Version.
**********************************************************************/

#ifndef _SPX_GTK_GUI_HPX_CTRL_H
#define _SPX_GTK_GUI_HPX_CTRL_H

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
class SPxHPx100Source;

class SPxGtkGuiHPxCtrl : public SPxGtkGuiCtrl
{
public:
    /* Public functions. */
    explicit SPxGtkGuiHPxCtrl(SPxHPx100Source *srcHw,
			      int isDualModeFixed=FALSE,
			      int fixedStreamIdx=-1);
    virtual ~SPxGtkGuiHPxCtrl(void);

    void ShowAdvCtrls(int show);

    /* GTK callbacks. */
    void streamModeComboboxChanged(GtkComboBox *combobox);
    void mapChanComboboxChanged(GtkComboBox *combobox);
    void mapTrgComboboxChanged(GtkComboBox *combobox);
    void mapAcpComboboxChanged(GtkComboBox *combobox);
    void mapArpComboboxChanged(GtkComboBox *combobox);
    void typesTrgComboboxChanged(GtkComboBox *combobox);
    void typesAziComboboxChanged(GtkComboBox *combobox);
    void typesArpComboboxChanged(GtkComboBox *combobox);
    void typesSig1ComboboxChanged(GtkComboBox *combobox);
    void typesSig2ComboboxChanged(GtkComboBox *combobox);
    void typesSig3ComboboxChanged(GtkComboBox *combobox);
    void tpgComboboxChanged(GtkComboBox *combobox);
    void streamRadiobuttonToggled(GtkToggleButton *button);
    void typesTrgInvCheckbuttonToggled(GtkToggleButton *button);
    void typesAziInvCheckbuttonToggled(GtkToggleButton *button);
    void typesArpInvCheckbuttonToggled(GtkToggleButton *button);
    void typesSig1InvCheckbuttonToggled(GtkToggleButton *button);
    void typesSig2InvCheckbuttonToggled(GtkToggleButton *button);
    void typesSig3InvCheckbuttonToggled(GtkToggleButton *button);
    void erpEnableCheckbuttonToggled(GtkToggleButton *button);
    void aziIntEnableCheckbuttonToggled(GtkToggleButton *button);
    void autoRangeCheckbuttonToggled(GtkToggleButton *button);
    void aGainSpinbuttonValueChanged(GtkSpinButton *spinbutton);
    void bGainSpinbuttonValueChanged(GtkSpinButton *spinbutton);
    void aOffsetSpinbuttonValueChanged(GtkSpinButton *spinbutton);
    void bOffsetSpinbuttonValueChanged(GtkSpinButton *spinbutton);
    void rangeSetButtonClicked(GtkButton *button);
    void alarmsClearButtonClicked(GtkButton *button);
    void advOptionsButtonClicked(GtkButton *button);
    void autoGainOffsetButtonClicked(GtkButton *button);

private:
    /* Private variables. */
    struct impl;
    SPxAutoPtr<impl> m_p;

    /* Private functions. */
    virtual SPxErrorCode Sync(void);
    void updateAlarmStates(void);
    static int onTimerWrapper(void *data);
    int onTimer(void);
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

/* HPx source callbacks. */
extern void on_src_hw_stream_mode_combobox_changed(GtkComboBox *combobox,
						   gpointer user_data);
extern void on_src_hw_map_chan_combobox_changed(GtkComboBox *combobox,
						gpointer user_data);
extern void on_src_hw_map_trg_combobox_changed(GtkComboBox *combobox,
					       gpointer user_data);
extern void on_src_hw_map_acp_combobox_changed(GtkComboBox *combobox,
					       gpointer user_data);
extern void on_src_hw_map_arp_combobox_changed(GtkComboBox *combobox,
					       gpointer user_data);
extern void on_src_hw_types_trg_combobox_changed(GtkComboBox *combobox,
						 gpointer user_data);
extern void on_src_hw_types_azi_combobox_changed(GtkComboBox *combobox,
						 gpointer user_data);
extern void on_src_hw_types_arp_combobox_changed(GtkComboBox *combobox,
						 gpointer user_data);
extern void on_src_hw_types_sig1_combobox_changed(GtkComboBox *combobox,
						  gpointer user_data);
extern void on_src_hw_types_sig2_combobox_changed(GtkComboBox *combobox,
						  gpointer user_data);
extern void on_src_hw_types_sig3_combobox_changed(GtkComboBox *combobox,
						  gpointer user_data);
extern void on_src_hw_tpg_combobox_changed(GtkComboBox *combobox,
					   gpointer user_data);
extern void on_src_hw_stream_radiobutton_toggled(GtkToggleButton *button,
						 gpointer user_data);
extern void on_src_hw_types_trg_inv_checkbutton_toggled(GtkToggleButton *button,
						        gpointer user_data);
extern void on_src_hw_types_azi_inv_checkbutton_toggled(GtkToggleButton *button,
						        gpointer user_data);
extern void on_src_hw_types_arp_inv_checkbutton_toggled(GtkToggleButton *button,
							gpointer user_data);
extern void on_src_hw_types_sig1_inv_checkbutton_toggled(GtkToggleButton *button,
							 gpointer user_data);
extern void on_src_hw_types_sig2_inv_checkbutton_toggled(GtkToggleButton *button,
							 gpointer user_data);
extern void on_src_hw_types_sig3_inv_checkbutton_toggled(GtkToggleButton *button,
							 gpointer user_data);
extern void on_src_hw_erp_enable_checkbutton_toggled(GtkToggleButton *button,
						     gpointer user_data);
extern void on_src_hw_azi_int_enable_checkbutton_toggled(GtkToggleButton *button,
							 gpointer user_data);
extern void on_src_hw_auto_range_checkbutton_toggled(GtkToggleButton *button,
						     gpointer user_data);
extern void on_src_hw_a_gain_spinbutton_value_changed(GtkSpinButton *spinbutton,
						      gpointer user_data);
extern void on_src_hw_b_gain_spinbutton_value_changed(GtkSpinButton *spinbutton,
						      gpointer user_data);
extern void on_src_hw_a_offset_spinbutton_value_changed(GtkSpinButton *spinbutton,
							gpointer user_data);
extern void on_src_hw_b_offset_spinbutton_value_changed(GtkSpinButton *spinbutton,
							gpointer user_data);
extern void on_src_hw_range_set_button_clicked(GtkButton *button,
					       gpointer user_data);
extern void on_src_hw_alarms_clear_button_clicked(GtkButton *button,
						  gpointer user_data);
extern void on_src_hw_adv_options_button_clicked(GtkButton *button,
						 gpointer user_data);
extern void on_src_hw_auto_gain_offset_button_clicked(GtkButton *button,
						      gpointer user_data);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _SPX_GTK_GUI_HPX_CTRL_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/

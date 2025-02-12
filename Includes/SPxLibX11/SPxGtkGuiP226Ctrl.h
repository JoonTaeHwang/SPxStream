/*********************************************************************
*
* (c) Copyright 2013, 2016, 2018, 2020, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for re-usable P226 Source Control GTK dialog.
*
*
* Revision Control:
*   22/01/20 v1.4    AGC	Use new SPxGtkIncludes header.
*
* Previous Changes:
*   14/09/18 1.3    AGC	Add radar type display.
*			Support Local IR control.
*			Support sector blanking control.
*   23/09/16 1.2    AGC	Add channel control.
*   15/05/13 1.1    AGC	Initial Version.
**********************************************************************/

#ifndef _SPX_GTK_GUI_P226_CTRL_H
#define _SPX_GTK_GUI_P226_CTRL_H

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
class SPxNetworkReceiveP226;

class SPxGtkGuiP226Ctrl : public SPxGtkGuiCtrl
{
public:
    /* Public functions. */
    explicit SPxGtkGuiP226Ctrl(SPxNetworkReceiveP226 *obj);
    virtual ~SPxGtkGuiP226Ctrl(void);
    void UpdateStatusVals(void);

    /* GTK callbacks. */
    void queryBtnClicked(GtkButton *button);
    void radarChannelComboChanged(GtkComboBox *combo);
    void transmittingCheckToggled(GtkToggleButton *button);
    void rangeSpinValueChanged(GtkSpinButton *button);
    void scanRateSliderValueChanged(GtkRange *range);
    void gainSliderValueChanged(GtkRange *range);
    void seaClutterSliderValueChanged(GtkRange *range);
    void rainSliderValueChanged(GtkRange *range);
    void ftcSliderValueChanged(GtkRange *range);
    void interferenceSliderValueChanged(GtkRange *range);
    void localIRSliderValueChanged(GtkRange *range);
    void gainAutoCheckToggled(GtkToggleButton *button);
    void seaClutterAutoComboChanged(GtkComboBox *combo);
    void sectorIDComboChanged(GtkComboBox *combo);
    void sectorBlankingCheckToggled(GtkToggleButton *button);
    void sectorStartAngleValueChanged(GtkSpinButton *button);
    void sectorEndAngleValueChanged(GtkSpinButton *button);

private:
    /* Private variables. */
    SPxNetworkReceiveP226 *m_obj;   /* The object we are controlling. */
    int m_timer;		    /* Timer for updating GUI. */

    /* Private functions. */
    virtual SPxErrorCode Sync(void);
    static int onTimerWrapper(void *data);
    int onTimer(void);
    static int objDeletedWrapper(void *invokingObject,
				 void *userObject, void *arg);
    int objDeleted(SPxObj *Obj);
    unsigned int getRangeValUint(GtkRange *range);

}; /* SPxGtkGuiCtrl */

/* P226 re-usable dialog callbacks. */
#ifdef __cplusplus
extern "C"
{
#if 0 /* Defeat emacs auto indent */
}
#endif
#endif /* __cplusplus */

void on_p226_radar_query_btn_clicked(GtkButton *button,
				     gpointer user_data);

void on_p226_radar_channel_combo_changed(GtkComboBox *combo,
					 gpointer user_data);

void on_p226_radar_transmitting_check_toggled(GtkToggleButton *button,
					      gpointer user_data);

void on_p226_radar_range_spin_value_changed(GtkSpinButton *button,
					    gpointer user_data);

void on_p226_radar_scan_rate_slider_value_changed(GtkRange *range,
						  gpointer user_data);

void on_p226_radar_gain_slider_value_changed(GtkRange *range,
					     gpointer user_data);

void on_p226_radar_sea_clutter_slider_value_changed(GtkRange *range,
						    gpointer user_data);

void on_p226_radar_rain_slider_value_changed(GtkRange *range,
					     gpointer user_data);

void on_p226_radar_ftc_slider_value_changed(GtkRange *range,
					    gpointer user_data);

void on_p226_radar_interference_slider_value_changed(GtkRange *range,
						     gpointer user_data);

void on_p226_radar_local_ir_slider_value_changed(GtkRange *range,
						 gpointer user_data);

void on_p226_radar_gain_auto_check_toggled(GtkToggleButton *button,
					   gpointer user_data);

void on_p226_radar_sea_clutter_auto_combo_changed(GtkComboBox *combo,
						  gpointer user_data);

void on_p226_sector_id_combo_changed(GtkComboBox *combo,
				     gpointer user_data);

void on_p226_sector_blanking_check_toggled(GtkToggleButton *button,
					   gpointer user_data);

void on_p226_sector_start_angle_spin_value_changed(GtkSpinButton *button,
						   gpointer user_data);

void on_p226_sector_end_angle_spin_value_changed(GtkSpinButton *button,
						 gpointer user_data);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _SPX_GTK_GUI_P226_CTRL_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/

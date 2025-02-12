/*********************************************************************
*
* (c) Copyright 2019, 2020, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for re-usable P430 Source Control GTK dialog.
*
*
* Revision Control:
*   22/01/20 v1.2    AGC	Use new SPxGtkIncludes header.
*
* Previous Changes:
*   16/06/19 1.1    AJH	Initial Version.
**********************************************************************/

#ifndef _SPX_GTK_GUI_P430_CTRL_H
#define _SPX_GTK_GUI_P430_CTRL_H

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
class SPxNetworkReceiveP430;

class SPxGtkGuiP430Ctrl : public SPxGtkGuiCtrl
{
public:
    /* Public functions. */
    explicit SPxGtkGuiP430Ctrl(SPxNetworkReceiveP430 *obj);
    virtual ~SPxGtkGuiP430Ctrl(void);
    void UpdateStatusVals(void);

    /* GTK callbacks. */
    void transmittingCheckToggled(GtkToggleButton *button);
    void rangeSpinValueChanged(GtkSpinButton *button);
    void gainSliderValueChanged(GtkRange *range);
    void seaClutterSliderValueChanged(GtkRange *range);
    void rainSliderValueChanged(GtkRange *range);
    void gainAutoCheckToggled(GtkToggleButton *button);
    void seaClutterAutoComboChanged(GtkComboBox *combo);
    void rainAutoCheckToggled(GtkToggleButton *button);
    void sectorIDComboChanged(GtkComboBox *combo);
    void sectorStartAngleValueChanged(GtkSpinButton *button);
    void sectorWidthValueChanged(GtkSpinButton *button);

private:
    /* Private variables. */
    SPxNetworkReceiveP430 *m_obj;   /* The object we are controlling. */
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

/* P430 re-usable dialog callbacks. */
#ifdef __cplusplus
extern "C"
{
#if 0 /* Defeat emacs auto indent */
}
#endif
#endif /* __cplusplus */

void on_p430_radar_transmitting_check_toggled(GtkToggleButton *button,
					      gpointer user_data);

void on_p430_radar_range_spin_value_changed(GtkSpinButton *button,
					    gpointer user_data);

void on_p430_radar_gain_slider_value_changed(GtkRange *range,
					     gpointer user_data);

void on_p430_radar_sea_clutter_slider_value_changed(GtkRange *range,
						    gpointer user_data);

void on_p430_radar_rain_slider_value_changed(GtkRange *range,
					     gpointer user_data);

void on_p430_radar_gain_auto_check_toggled(GtkToggleButton *button,
					   gpointer user_data);

void on_p430_radar_sea_clutter_auto_combo_changed(GtkComboBox *combo,
						  gpointer user_data);

void on_p430_radar_rain_auto_check_toggled(GtkToggleButton *button,
					   gpointer user_data);

void on_p430_sector_id_combo_changed(GtkComboBox *combo,
				     gpointer user_data);

void on_p430_sector_start_angle_spin_value_changed(GtkSpinButton *button,
						   gpointer user_data);

void on_p430_sector_width_spin_value_changed(GtkSpinButton *button,
						 gpointer user_data);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _SPX_GTK_GUI_P430_CTRL_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/

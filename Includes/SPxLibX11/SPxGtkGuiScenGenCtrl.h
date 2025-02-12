/*********************************************************************
*
* (c) Copyright 2012 - 2020, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for re-usable Scenario Generator GTK dialog.
*
*
* Revision Control:
*   22/01/20 v1.6    AGC	Use new SPxGtkIncludes header.
*
* Previous Changes:
*   07/01/20 1.5    AGC	Silence deprecated warnings.
*   06/08/18 1.4    AGC	Suppress gcc 8 warnings from GTK headers.
*   18/07/14 1.3    AGC	Avoid warnings in glib headers.
*   10/03/14 1.2    AGC	Move private struct inside class.
*   31/08/12 1.1    AGC	Initial Version.
**********************************************************************/

#ifndef _SPX_GTK_GUI_SCEN_GEN_CTRL_H
#define _SPX_GTK_GUI_SCEN_GEN_CTRL_H

/*
 * Other headers required.
 */

/* For GTK+ enumerations/types. */
#include "SPxLibX11/SPxGtkIncludes.h"

/* For SPxAutoPtr. */
#include "SPxLibUtils/SPxAutoPtr.h"

/* For SPxSGtargetState enumeration. */
#include "SPxLibData/SPxScenGen.h"

/* For base class. */
#include "SPxGtkGuiCtrl.h"

/*********************************************************************
*
*   Type definitions
*
**********************************************************************/

/* Forward declarations. */
class SPxScenarioGenerator;
struct SPxGtkGuiScenGenCtrlPriv;

class SPxGtkGuiScenGenCtrl : public SPxGtkGuiCtrl
{
public:
    /* Public functions. */
    explicit SPxGtkGuiScenGenCtrl(SPxScenarioGenerator *srcScen);
    virtual ~SPxGtkGuiScenGenCtrl(void);

    /* GTK callbacks. */
    void readFileButtonClicked(GtkButton *btn);
    void saveButtonClicked(GtkButton *btn);
    void deleteAllButtonClicked(GtkButton *btn);
    void addTargetButtonClicked(GtkButton *btn);
    void noiseBackgroundChanged(GtkRange *range);
    void noiseNearToggled(GtkToggleButton *btn);
    void noiseGainChanged(GtkSpinButton *btn);
    void noiseRangeChanged(GtkSpinButton *btn);
    void idComboChanged(GtkComboBox *combo);
    void autoUpdateToggled(GtkCheckButton *btn);
    void xChanged(GtkSpinButton *btn);
    void yChanged(GtkSpinButton *btn);
    void breadthChanged(GtkSpinButton *btn);
    void lengthChanged(GtkSpinButton *btn);
    void speedChanged(GtkSpinButton *btn);
    void headingChanged(GtkSpinButton *btn);
    void deleteTargetButtonClicked(GtkButton *btn);
    void distRecvComboChanged(GtkComboBox *combo);
    void netSetButtonClicked(GtkButton *btn);

private:
    /* Private types. */
    struct SPxTargetData;

    /* Private variables. */
    SPxAutoPtr<SPxGtkGuiScenGenCtrlPriv> m_p;

    /* Private functions. */
    virtual SPxErrorCode Sync(void);
    void updateAddressCtrls(void);
    static int onTimerWrapper(void *data);
    int onTimer(void);
    static void targetFnWrapper(SPxScenarioGenerator *obj,
			  int id,
			  SPxSGtargetState state,
			  void *userArg);
    void targetFn(SPxScenarioGenerator *obj,
		   int id,
		   SPxSGtargetState state);
    static int mainLoopTargetFnWrapper(void *data);
    int mainLoopTargetFn(SPxTargetData *targetData);
    static int scenGenDeletedWrapper(void *invokingObject,
				      void *userObject, void *arg);
    int scenGenDeleted(SPxScenarioGenerator *srcScen);

}; /* SPxGtkGuiCtrl */

/* Scenario Generator re-usable dialog callbacks. */
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

/* Scenario Generator callbacks. */
extern void on_src_scen_read_file_button_clicked(GtkButton *button,
						 void *user_data);
extern void on_src_scen_save_button_clicked(GtkButton *button,
					    void *user_data);
extern void on_src_scen_delete_all_button_clicked(GtkButton *button,
						  void *user_data);
extern void on_src_scen_add_target_button_clicked(GtkButton *button,
						  void *user_data);
extern void on_src_scen_noise_background_changed(GtkRange *range,
						 void *user_data);
extern void on_src_scen_noise_near_toggled(GtkToggleButton *btn,
					   void *user_data);
extern void on_src_scen_noise_gain_changed(GtkSpinButton *btn,
					   void *user_data);
extern void on_src_scen_noise_range_changed(GtkSpinButton *btn,
					    void *user_data);
extern void on_src_scen_id_combo_changed(GtkComboBox *combo,
					 void *user_data);
extern void on_src_scen_auto_update_toggled(GtkCheckButton *btn,
					    void *user_data);
extern void on_src_scen_x_changed(GtkSpinButton *btn,
				  void *user_data);
extern void on_src_scen_y_changed(GtkSpinButton *btn,
				  void *user_data);
extern void on_src_scen_breadth_changed(GtkSpinButton *btn,
					void *user_data);
extern void on_src_scen_length_changed(GtkSpinButton *btn,
				       void *user_data);
extern void on_src_scen_speed_changed(GtkSpinButton *btn,
				      void *user_data);
extern void on_src_scen_heading_changed(GtkSpinButton *btn,
					void *user_data);
extern void on_src_scen_delete_target_button_clicked(GtkButton *btn,
						     void *user_data);
extern void on_src_scen_dist_recv_combo_changed(GtkComboBox *combo,
						void *user_data);
extern void on_src_scen_net_set_button_clicked(GtkButton *btn,
					       void *user_data);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _SPX_GTK_GUI_SCEN_GEN_CTRL_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/

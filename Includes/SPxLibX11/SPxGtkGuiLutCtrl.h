/*********************************************************************
*
* (c) Copyright 2013, 2020, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for re-usable LUT Control GTK dialog.
*
*
* Revision Control:
*   22/01/20 v1.3    AGC	Use new SPxGtkIncludes header.
*
* Previous Changes:
*   14/11/13 1.2    REW	Support arg3 and arg4.
*   08/03/13 1.1    AGC	Initial Version.
**********************************************************************/
#ifndef _SPX_GTK_GUI_LUT_CTRL_H
#define _SPX_GTK_GUI_LUT_CTRL_H

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
class SPxLUT;
class SPxRunProcess;

class SPxGtkGuiLutCtrl : public SPxGtkGuiCtrl
{
public:
    /* Public functions. */
    explicit SPxGtkGuiLutCtrl(SPxLUT *lut, SPxRunProcess *proLut);
    virtual ~SPxGtkGuiLutCtrl(void);

    void SetTitle(const char *title);
    void Show(GtkWindow *parent);

    /* GTK callbacks. */
    void readFileButtonClicked(GtkButton *button);
    void typeComboboxChanged(GtkComboBox *combobox);
    void arg1SpinbuttonValueChanged(GtkSpinButton *spinbutton);
    void arg2SpinbuttonValueChanged(GtkSpinButton *spinbutton);
    void arg3SpinbuttonValueChanged(GtkSpinButton *spinbutton);
    void arg4SpinbuttonValueChanged(GtkSpinButton *spinbutton);

private:
    /* Private variables. */
    SPxLUT *m_lut;
    SPxRunProcess *m_proLut;
    int m_syncing;

    /* Private functions. */
    virtual SPxErrorCode Sync(void);
    void setLut(void);

}; /* SPxGtkGuiLutCtrl */

#ifdef __cplusplus
extern "C"
{
#if 0 /* Defeat emacs auto indent */
}
#endif
#endif /* __cplusplus */

extern void on_pro_lut_type_combobox_changed(GtkComboBox *combobox,
                                             gpointer user_data);
extern void on_pro_lut_arg1_spinbutton_value_changed(GtkSpinButton *spinbutton,
                                                     gpointer user_data);
extern void on_pro_lut_arg2_spinbutton_value_changed(GtkSpinButton *spinbutton,
                                                     gpointer user_data);
extern void on_pro_lut_arg3_spinbutton_value_changed(GtkSpinButton *spinbutton,
                                                     gpointer user_data);
extern void on_pro_lut_arg4_spinbutton_value_changed(GtkSpinButton *spinbutton,
                                                     gpointer user_data);
extern void on_pro_lut_read_file_button_clicked(GtkButton *button,
                                          gpointer user_data);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _SPX_GTK_GUI_LUT_CTRL_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/

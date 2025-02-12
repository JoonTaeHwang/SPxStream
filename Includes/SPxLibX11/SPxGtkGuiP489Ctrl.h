/*********************************************************************
*
* (c) Copyright 2021, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for re-usable P489 Source Control GTK dialog.
*
*
* Revision Control:
*   05/01/21 v1.1    AGC	Initial Version.
*
* Previous Changes:
**********************************************************************/

#ifndef _SPX_GTK_GUI_P489_CTRL_H
#define _SPX_GTK_GUI_P489_CTRL_H

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
class SPxNetworkReceiveP489;
class SPxRunProcess;
class SPxGtkGuiNetCtrl;

class SPxGtkGuiP489Ctrl : public SPxGtkGuiCtrl
{
public:
    /* Public functions. */
    explicit SPxGtkGuiP489Ctrl(SPxNetworkReceiveP489 *obj);
    virtual ~SPxGtkGuiP489Ctrl(void);

    void SetProToRibRaw(SPxRunProcess *proToRibRaw);
    void SetProToRibProc(SPxRunProcess *proToRibProc);

    /* GTK callbacks. */
    void endRangeSpinValueChanged(GtkSpinButton *spinbutton);
    void gainSpinValueChanged(GtkSpinButton *spinbutton);
    void periodMultSpinValueChanged(GtkSpinButton *spinbutton);

private:
    /* Private variables. */
    SPxNetworkReceiveP489 *m_obj;   /* The object we are controlling. */
    SPxRunProcess *m_proToRibRaw;
    SPxRunProcess *m_proToRibProc;
    SPxAutoPtr<SPxGtkGuiNetCtrl> m_controlDlg;

    /* Private functions. */
    virtual SPxErrorCode Sync(void);
    static int objDeletedWrapper(void *invokingObject,
				 void *userObject, void *arg);
    int objDeleted(SPxObj *Obj);

}; /* SPxGtkGuiP489Ctrl */

/* P435 re-usable dialog callbacks. */
#ifdef __cplusplus
extern "C"
{
#if 0 /* Defeat emacs auto indent */
}
#endif
#endif /* __cplusplus */

extern void on_p489_end_range_spin_value_changed(GtkSpinButton *spinbutton,
					         gpointer user_data);
extern void on_p489_gain_spin_value_changed(GtkSpinButton *spinbutton,
					    gpointer user_data);
extern void on_p489_period_mult_spin_value_changed(GtkSpinButton *spinbutton,
						   gpointer user_data);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _SPX_GTK_GUI_P489_CTRL_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/

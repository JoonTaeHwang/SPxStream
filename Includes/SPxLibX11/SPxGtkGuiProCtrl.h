/*********************************************************************
 *
 * (c) Copyright 2009 - 2014, 2018, 2020, 2022, Cambridge Pixel Ltd.
 *
 * Purpose:
 *  SPx Server generic process control dialog header file.
 *
 * Revision Control:
*   02/09/22 v1.14   AGC	Rename ParamContext_t to avoid cppcheck ODR warning.
 *
 * Previous Changes:
 *  22/01/20 1.13   AGC	Use new SPxGtkIncludes header.
 *  06/08/18 1.12   AGC	Suppress gcc 8 warnings from GTK headers.
 *  18/07/14 1.11   AGC	Avoid warnings in glib headers.
 *  29/04/14 1.10   REW	Add indicator for region name validity.
 *  25/04/14 1.9    REW	Support area-dependent params.
 *  10/04/14 1.8    AGC	Move to SPxLibX11.
 *  26/03/14 1.7    AGC	Support SPxProcParamIface.
 *  01/12/10 1.6    SP  Callback onWindowDelete must return TRUE 
 *                      to stop event being propagated.
 *  26/11/10 1.5    SP 	Support parameter hiding.
 *  10/11/10 1.4    SP 	Further enhancements.
 *  08/11/10 1.3    SP 	Use new param GUI display info.
 *  15/09/10 1.2    REW	Make destructor virtual.
 *  12/05/09 1.1    SP 	Initial version
 **********************************************************************/

#ifndef _SPX_GTK_GUI_PRO_CTRL_H
#define _SPX_GTK_GUI_PRO_CTRL_H

/* For GTK+ enumerations/types. */
#include "SPxLibX11/SPxGtkIncludes.h"

/* Forward declaration. */
class SPxGtkGuiProCtrl;
class SPxProcessRunParameter;
class SPxProcessParameter;
struct SPxProcParamCtrlInfo_tag;
class SPxProcParamIface;

/*
 * Types
 */

/* Parameter context */
typedef struct
{
    SPxGtkGuiProCtrl *obj;                    /* Pointer to 'this' object */
    int paramNum;                           /* Parameter number */
    SPxProcessRunParameter *runParam;       /* Process run parameter */
    SPxProcessParameter *param;             /* Underlying parameter */
    const struct SPxProcParamCtrlInfo_tag *ctrlInfo; /* GUI control info */
    GtkWidget *ctrlWidget;                  /* Main control widget */
    GtkRadioButton *enableRadio;            /* Aux enable/disable widget */

} SPxGtkGuiProCtrlParamContext_t;

/* Region-based parameters. */
typedef struct SPxGtkGuiProCtrlAreaContext_tag
{
    unsigned int regionIdx;		/* Index for this region */
    GtkWidget *regionIndicator;		/* Indicator for region validity */
    GtkWidget *regionWidget;		/* Entry box for region name */
    SPxGtkGuiProCtrlParamContext_t **baseParams;	/* List of ptrs to base params */
    GtkWidget **ctrlWidgets;		/* List of ptrs to controls */
} SPxGtkGuiProCtrlAreaContext_t;

class SPxGtkGuiProCtrl
{

public:

    /* Functions */
    SPxGtkGuiProCtrl(GtkWindow *parent, 
                     SPxProcParamIface *runPro, 
                     const char *title);
    virtual ~SPxGtkGuiProCtrl();
    void ShowWindow();

private:

    /* Functions */
    void InitGUI(GtkWindow *parent, const char *title);
    void InitAreaDependentGUI(GtkWidget *vbox);
    void SetParamValue(int paramNum, double val);
    void SetParamString(int paramNum, const char *str);
    void AddCtrl(SPxGtkGuiProCtrlParamContext_t *ctxPtr);
    void AddLabelCtrl(SPxGtkGuiProCtrlParamContext_t *ctxPtr);
    void AddSpinCtrl(SPxGtkGuiProCtrlParamContext_t *ctxPtr);
    void AddEntryCtrl(SPxGtkGuiProCtrlParamContext_t *ctxPtr);
    void AddSliderCtrl(SPxGtkGuiProCtrlParamContext_t *ctxPtr);
    void AddRadioCtrl(SPxGtkGuiProCtrlParamContext_t *ctxPtr);
    void AddMenuCtrl(SPxGtkGuiProCtrlParamContext_t *ctxPtr);
    void AddCheckCtrl(SPxGtkGuiProCtrlParamContext_t *ctxPtr);
    void AddButtonCtrl(SPxGtkGuiProCtrlParamContext_t *ctxPtr);
    void AddEnableRadio(SPxGtkGuiProCtrlParamContext_t *ctxPtr);
    void AddCtrlLabels(SPxGtkGuiProCtrlParamContext_t *ctxPtr);
    void SyncFromObj(int syncAll=TRUE);

    /* Event callback functions */
    static gboolean onWindowDelete(GtkWidget *widget,
                                   GdkEvent *event,
                                   gpointer userData);

    static void onValueChanged(GtkWidget *widget, gpointer user_data);

    static void onTextChanged(GtkWidget *widget, gpointer user_data);

    static void onButtonClicked(GtkWidget *widget, gpointer user_data);

    static void onRadioToggled(GtkToggleButton *button,
                               gpointer userData);

    static void onEnableRadioToggled(GtkToggleButton *button,
                                     gpointer userData);

    static void onShowAreaDependencies(GtkWidget *widget, gpointer userData);
    static void staticOnRegionNameChanged(GtkWidget *widget, gpointer userData);
    void onRegionNameChanged(GtkWidget *widget);
    static gboolean staticOnRegionNameLostFocus(GtkWidget *widget,
						GdkEventFocus *event,
						gpointer userData);
    gboolean onRegionNameLostFocus(GtkWidget *widget, GdkEventFocus *event);
    static void staticOnRegionValueChanged(GtkWidget *widget,
						gpointer userData);
    void onRegionValueChanged(GtkWidget *widget);

    /* Utility functions. */
    SPxGtkGuiProCtrlAreaContext_t *findAreaContextForWidget(GtkWidget *widget);
    void updateRegionIndicator(SPxGtkGuiProCtrlAreaContext_t *area);

    /* Run process associated with this dialog */
    SPxProcParamIface *m_runPro;

    /* Number of parameters */
    int m_numParams;
    int m_numAreaDependentParams;

    /* Additional context for each parameter */
    SPxGtkGuiProCtrlParamContext_t *m_paramCtx;

    /* Additional context for each area-based parameter. */
    SPxGtkGuiProCtrlAreaContext_t *m_areaContexts;

    /* Dialog window */
    GtkWidget *m_windowWidget;

    /* Parameter container table */
    GtkContainer *m_paramTable;
 
    /* Area-dependent parameter container table */
    GtkContainer *m_areaTable;
 
}; /* SPxGtkGuiProCtrl */

#endif /* _SPX_GTK_GUI_PRO_CTRL_H */

/*********************************************************************
 *
 * End of file
 *
 **********************************************************************/

/*********************************************************************
*
* (c) Copyright 2010 - 2018, 2020, Cambridge Pixel Ltd.
*
* Purpose:
*	Header file for SPxWizard functionality on Linux with GTK.
*
* Revision Control:
*   19/06/20 v1.9    AJH 	Support password entry.
*
* Previous Changes:
*   27/03/18 1.8    SP 	Use strings for menu entry values.
*   21/10/13 1.7    REW	Support multiple instances of same page.
*   04/10/13 1.6    AGC	Simplify headers.
*   10/05/12 1.5    AGC	Improve positioning of pages.
*   22/12/10 1.4    SP 	Remember window position between steps.
*   03/12/10 1.3    SP 	Support hidden top-level radio button.
*   01/12/10 1.2    SP 	Populate class functions to create working version.
*   26/11/10 1.1    REW	Initial Version.
**********************************************************************/

#ifndef _SPX_WIZARDGTK_H
#define _SPX_WIZARDGTK_H

/*
 * Other headers required.
 */

#include "SPxLibUtils/SPxWizard.h"      /* For SPxWizard base class */
#include "SPxLibX11/SPxGtk.h"           /* For GTK headers, etc */


/*********************************************************************
*
*   Constants
*
**********************************************************************/


/*********************************************************************
*
*   Type definitions
*
**********************************************************************/

/* Need to define the class so that we can use it below */
class SPxWizardGtk;

/* Control context information */
typedef struct _ControlContext
{
    SPxWizardGtk *obj;              /* Pointer back to this object */
    SPxProcParamCtrlInfo_t info;    /* Control info struct */
    void *callbackArg;              /* Arg to pass to change function */
    _ControlContext *nextPtr;       /* Linked list pointer */

} ControlContext_t;

/*
 * Define the class.
 */
class SPxWizardGtk :public SPxWizard
{
public:
    /*
     * Public functions.
     */
    /* Constructor/destructor. */
    SPxWizardGtk(GtkWindow *parent=NULL);
    virtual ~SPxWizardGtk(void);

protected:
    /*
     * Protected functions.
     */
    /* Functions to build the wizard, which we need to overwrite from
     * the base class to actually implement the wizard functionality
     * on different platforms.
     */
    virtual SPxErrorCode startPage(const char *label,
				    unsigned int pageIdx,
				    unsigned int numPages,
				    unsigned int pageInstance,
				    unsigned int numInstances);

    virtual SPxErrorCode addNote(unsigned int indent,
				    const char *note);

    virtual SPxErrorCode addButton(unsigned int indent, void *callbackArg,
				    int isRadio,
				    const char *label,
				    int initialState);

    virtual SPxErrorCode addControl(unsigned int indent, void *callbackArg,
				    struct SPxProcParamCtrlInfo_tag *info,
				    const char *initialValue);

    virtual SPxWizardPageResult_t runPage(void);

private:
    /*
     * Private functions.
     */
    void resetPage(void);

    GtkWidget *createSpinCtrl(ControlContext_t *ctxPtr, 
                              double initialValue);

    GtkWidget *createEntryCtrl(ControlContext_t *ctxPtr, 
                               const char *initialValue);

    GtkWidget *createPasswordCtrl(ControlContext_t *ctxPtr, 
                               const char *initialValue);

    GtkWidget *createSliderCtrl(ControlContext_t *ctxPtr, 
                                double initialValue);

    GtkWidget *createRadioCtrl(ControlContext_t *ctxPtr, 
                               int initialValue);

    GtkWidget *createMenuCtrl(ControlContext_t *ctxPtr, 
                              const char *initialValue);

    GtkWidget *createCheckCtrl(ControlContext_t *ctxPtr, 
                               int initialValuel);

    GtkWidget *createRadioTopLevel(ControlContext_t *ctxPtr,
                                   const char *label, 
                                   int isActive);

    GtkWidget *createCheckTopLevel(ControlContext_t *ctxPtr,
                                   const char *label, 
                                   int isActive);

    void addCtrlToGui(GtkWidget *ctrl, 
                      unsigned int indent, 
                      const char *desc,
                      const char *help);

    GtkWidget *gtkLabelNewWithMarkup(const char *text,
                                     const char *markup);

    /*
     * Static GTK callback functions.
     */
    static gboolean onWindowDelete(GtkWidget *widget, 
                                   GdkEvent *event,
                                   gpointer userData);

    static void onPrevButtonClicked(GtkButton *button, 
                                    gpointer userData);

    static void onNextButtonClicked(GtkButton *button, 
                                    gpointer userData);

    static void onFinishButtonClicked(GtkButton *button, 
                                      gpointer userData);

    static void onCancelButtonClicked(GtkButton *button, 
                                      gpointer userData);

    static void onCtrlValueChanged(GtkWidget *widget, 
                                   gpointer userData);

    static void onCtrlTextChanged(GtkWidget *widget, 
                                  gpointer userData);

    static void onCtrlRadioToggled(GtkToggleButton *button, 
                                   gpointer userData);

    static void onTopLevelButtonToggled(GtkToggleButton *button,
                                        gpointer userData);

    /*
     * Private variables.
     */
    
    /* GTK widgets */
    GtkWidget *m_window;        /* The main wizard window */
    GtkWidget *m_ctrlVBox;      /* Box used to hold controls */
    GtkWidget *m_ctrlLabel;     /* Label on control frame */
    GtkWidget *m_progressBar;   /* Progress bar */

    /* Navigation buttons */
    GtkWidget *m_nextBn;
    GtkWidget *m_prevBn;
    GtkWidget *m_finishBn;
    GtkWidget *m_cancelBn;

    /* Status value to return from runPage() */
    SPxWizardPageResult_t m_result;

    /* Default top-level radio that is not displayed on the GUI
     * but will be selected by default is no other radio buttons 
     * in the group are selected.
     */
    GtkRadioButton *m_defaultTopLevelRadio;

    /* A linked list containing context data for all controls */
    ControlContext_t *m_controlCtxList;
    
    /*
     * Static variables for class.
     */

}; /* SPxWizardGtk */


/*********************************************************************
*
*   Macros
*
**********************************************************************/

/*********************************************************************
*
*   Variable definitions
*
**********************************************************************/

/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

#endif /* _SPX_WIZARDGTK_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/

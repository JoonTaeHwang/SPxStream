/*********************************************************************
*
* (c) Copyright 2008 - 2020, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for applications wishing to use the GTK+ helper/utility
*	functions in the SPx library.
*
*
* Revision Control:
*   22/01/20 v1.18   AGC	Move GTK header includes to SPxGtkIncludes header.
*
* Previous Changes:
*   07/01/20 1.17   AGC	Silence deprecated warnings.
*   14/09/18 1.16   AGC	Add missing changes.
*   14/09/18 1.15   AGC	Support attributes for combo-box items.
*			Add SPxGtkComboBoxRemoveEntry().
*   14/09/18 1.14   AGC	Add SPxGtkGetIconList().
*   06/08/18 1.13   AGC	Suppress gcc 8 warnings from GTK headers.
*   13/04/16 1.12   AGC	Add SPxGtkPixbufLoadFromFile().
*   18/07/14 1.11   AGC	Avoid warnings in glib headers.
*   04/10/13 1.10   AGC	Simplify headers.
*   15/05/13 1.9    AGC	Make attr argument to SPxGtkSetLabelText() optional.
*   08/03/13 1.8    AGC	HPx dialog now a separate class.
*			Add second form of SPxGtkSetLabelText() function.
*   31/08/12 1.7    AGC	Scen gen dialog now a separate class.
*			Forward declare HPx source.
*   19/04/12 1.6    AGC	Add noise controls to scen gen dialog.
*   08/02/12 1.5    AGC	Add re-usable HPx control dialog callbacks.
*			Add GTK+ utilities.
*   14/11/11 1.4    AGC	Add scen gen net set button callback.
*   02/11/11 1.3    AGC	Add GTK callback prototypes for scen gen.
*   25/10/11 1.2    AGC	Add glade file loading.
*   21/11/08 1.1    REW	Initial Version.
**********************************************************************/

#ifndef _SPX_GTK_H
#define _SPX_GTK_H

/*
* If defined then LibGlade will be used to create the GUI.
* If not defined then GtkBuilder will be used to create the GUI.
* GtkBuilder is the replacement for LibGlade and is supported in
* GTK V2.12 and onwards. At present we support CentOS5 which
* uses GTK V2.10, therefore we must use LibGlade.
*/
#ifndef USE_GTK_BUILDER
#define USE_LIB_GLADE 1
#endif

/*
 * Other headers required.
 */
/* We need SPxLibUtils for common types, events, errors etc. */
#include "SPxLibUtils/SPxError.h"
#include "SPxLibData/SPxScenGen.h"

/* Include header that includes GTK headers with
 * unnecessary warnings disabled.
 */
#include "SPxLibX11/SPxGtkIncludes.h"

/*********************************************************************
*
*   Constants
*
**********************************************************************/


/*********************************************************************
*
*   Macros
*
**********************************************************************/


/*********************************************************************
*
*   Type definitions
*
**********************************************************************/

/* Foward declarations. */
class SPxHPx100Source;

/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

/*
* GTK+ helper/utility functions.
*/
/* Initialisation */
extern GtkWidget *SPxGtkInit(int *argcPtr, char ***argvPtr, const char *title);

/* Logo and icon retrieval. */
extern GdkPixbuf *SPxGtkGetIcon(void);
extern GList *SPxGtkGetIconList(void);
extern GdkPixbuf *SPxGtkGetLogo(int width=0);
extern GdkPixbuf *SPxGtkPixbufLoadFromFile(const char *name, GError **error);

/* Glade file loading */
extern SPxErrorCode SPxGtkGuiLoadFromFile(const char *name);
extern SPxErrorCode SPxGtkGuiLoadFromString(const char *content);

/* Utilities */
extern SPxErrorCode SPxGtkComboBoxAppendEntry(GtkComboBox *cb, 
                                              const char *str, 
                                              UINT32 val,
                                              const char *attr=NULL);
extern SPxErrorCode SPxGtkComboBoxRemoveEntry(GtkComboBox *cb,
                                              UINT32 val);
extern SPxErrorCode SPxGtkComboBoxSetFromVal(GtkComboBox *cb,
                                             UINT32 val,
                                             const char *attr=NULL);
extern UINT32 SPxGtkComboBoxGetVal(GtkComboBox *cb);
extern void SPxGtkSetLabelText(const char *label, const char *text, const char *attr=NULL);
extern void SPxGtkSetLabelText(GtkLabel *label, const char *text, const char *attr=NULL);

/* Get a named GTK object from any of the loaded re-usable dialogs. */
extern GObject *SPxGtkGuiGetObj(const char *name);

#endif /* _SPX_GTK_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/

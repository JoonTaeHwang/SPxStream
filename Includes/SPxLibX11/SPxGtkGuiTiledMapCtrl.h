/*********************************************************************
*
* (c) Copyright 2014, 2020, Cambridge Pixel Ltd.
*
* Purpose:
*	Header for re-usable Tiled Map control GTK dialog.
*
*
* Revision Control:
*   22/01/20 v1.2    AGC	Use new SPxGtkIncludes header.
*
* Previous Changes:
*   02/06/14 1.1    SP 	Initial version.
*
**********************************************************************/

#ifndef _SPX_GTK_GUI_TILED_MAP_CTRL_H
#define _SPX_GTK_GUI_TILED_MAP_CTRL_H

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
class SPxTiledMapDatabase;
class SPxViewControl;
struct SPxGtkGuiTiledMapCtrlPriv;

class SPxGtkGuiTiledMapCtrl : public SPxGtkGuiCtrl
{
public:
    /* Public functions. */
    explicit SPxGtkGuiTiledMapCtrl(SPxTiledMapDatabase *tdb,
                                   SPxViewControl *viewCtrl);
    virtual ~SPxGtkGuiTiledMapCtrl(void);

    SPxErrorCode SetCacheArea(double latNDegs, double lonWDegs,
                              double latSDegs, double lonEDegs,
                              unsigned int widthPixels);

    /* GTK callbacks. */
    void serverComboboxChanged(GtkComboBox *combobox);
    void startStopCachingAreaClicked(GtkButton *button);
    void updateCacheClicked(GtkButton *button);
    void deleteCacheClicked(GtkButton *button);
    void maxZoomChanged(GtkRange *range);

private:
    /* Private variables. */
    SPxAutoPtr<SPxGtkGuiTiledMapCtrlPriv> m_p;
    
    /* Private functions. */
    virtual SPxErrorCode Sync(void);
    SPxErrorCode UpdateCacheArea(void);
    void UpdateServer(void);
    static int onTimerWrapper(void *data);
    int onTimer(void);
    static int tdbDeletedWrapper(void *invokingObject,
				 void *userObject, 
                                 void *arg);
    int tdbDeleted(SPxTiledMapDatabase *tdb); 
  
}; /* SPxGtkGuiCtrl */

/* Re-usable dialog callbacks. */
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

void on_tiled_map_server_combobox_changed(GtkComboBox *combobox,
                                          gpointer user_data);

void on_tiled_map_start_stop_caching_area_button_clicked(GtkButton *button,
                                                         gpointer user_data);

void on_tiled_map_update_cache_button_clicked(GtkButton *button,
                                              gpointer user_data);

void on_tiled_map_delete_cache_button_clicked(GtkButton *button,
                                              gpointer user_data);

void on_tiled_map_zoom_hscale_value_changed(GtkRange *range,
                                            gpointer user_data);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _SPX_GTK_GUI_TILED_MAP_CTRL_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/

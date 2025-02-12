/*********************************************************************
*
* (c) Copyright 2007 - 2022, Cambridge Pixel Ltd.
*
* Purpose:
*   Header file for SPx error handling.
*
* Revision Control:
*   10/08/22 v1.299  SP 	Add SPX_ERR_FUSION_OBS_UNUSABLE.
*
* Previous Changes:
*   04/08/22 1.298  SP 	Add SPX_ERR_FUSION_NAV_DATA_NOT_SET.
*   22/07/22 1.297  SP 	Remove unused SPX_ERR_VID_SIM_xxx errors.
*   28/06/22 1.296  AGC	Add SPX_ERR_REAL_PATH.
*   22/06/22 1.295  REW	Add SPX_ERR_LICENSE_SUPPORT.
*   14/06/22 1.294  AGC	Add SPX_ERR_WEB_IF_BIND.
*   13/06/22 1.293  SP 	Add SPX_ERR_BAD_PROJECT_CODE.
*   10/03/22 1.292  SP 	Add SPX_ERR_AEROSCOPE_REQUEST_FAILED.
*   09/03/22 1.291  SP 	Add SPX_ERR_AEROSCOPE_ID_NOT_FOUND.
*   08/03/22 1.290  SP 	Rename SPX_ERR_AEROSCOPE_TOKEN_NOT_SET.
*   02/02/22 1.289  SP 	Add SPX_ERR_AEROSCOPE_xxx errors.
*   20/01/22 1.288  SP 	Add SPX_ERR_TRACK_INCORRECT_TYPE.
*   17/12/21 1.287  AGC	Add SPX_ERR_TERRAIN_xxx errors.
*   29/10/21 1.286  SP 	SPX_ERR_RADAR_ALIGNER_xxx errors.
*   14/09/21 1.285  BTB	Fix IDs of new track filter errors.
*   14/09/21 1.284  BTB	Add SPX_ERR_TRACK_FILTER_xxx errors.
*   10/08/21 1.283  SP 	Add SPX_ERR_AV_CAIRO_ERROR.
*   04/08/21 1.282  AGC	Add SPX_ERR_WEB_IF_INVALID_UTF8.
*   02/08/21 1.281  SP 	Add SPX_ERR_NET_NO_SECTOR_UPDATE.
*   18/06/21 1.280  AGC	Support auto reporting of errors to clients.
*   03/06/21 1.279  AGC	Increase error recorder flexibility.
*   05/05/21 1.278  AGC	Add SPX_ERR_COMM_INCOMPLETE.
*   15/04/21 1.277  SP 	Add SPX_ERR_ALARM_BAD_TRACK_ID.
*   07/04/21 1.276  SP 	Add SPX_ERR_ALARM_xxx errors.
*   11/02/21 1.275  AGC	Add SPX_ERR_WEB_IF_HTTPS_NOT_SUPPORTED.
*   12/11/20 1.274  AGC	Add SPX_ERR_AV_GIGE_VISION.
*   08/10/20 1.273  AGC	Add SPX_ERR_PARAM_NOT_SET.
*   13/08/20 1.272  SP 	Fix last change.
*   13/08/20 1.271  SP 	Add SPX_ERR_DATA_EXPIRED.
*   04/08/20 1.270  AGC	Add SPX_ERR_TIFF_xxx errors.
*   15/07/20 1.269  SP 	Add SPX_ERR_ZLIB_COMPRESS.
*   08/07/20 1.268  REW	Rename SPX_ERR_AV_BLACKBODY.
*   24/06/20 1.267  REW	Add SPX_ERR_HW_BIST_TEMP.
*   28/05/20 1.266  AGC	Add SPX_ERR_AV_OPENCV_ERROR.
*   22/05/20 1.265  AJH Add SPX_ERR_AV_BLACK_BODY.
*   14/04/20 1.264  AGC	Add SPX_ERR_AV_SYS_TIMER_RES.
*   08/04/20 1.263  SP 	Fix typos in recent changes.
*   07/04/20 1.262  SP 	Add SPX_ERR_MIB_OBJECT_READ_ONLY.
*   03/04/20 1.261  SP 	Changes to SPX_ERR_SNMP_xxx.
*   02/04/20 1.260  SP 	Add SPX_ERR_SNMP_xxx and SPX_ERR_MIB_xxx.
*   20/03/20 1.259  SP 	Add SPX_ERR_MON_STREAM_ASTERIX_NOGO.
*   18/03/20 1.258  SP 	Add SPX_ERR_MON_NET_PACKET_TIMEOUT.
*   12/03/20 1.257  SP 	Add SPX_ERR_JSON_DECODE_FIELD.
*   09/03/20 1.256  SP 	Add SPX_ERR_COMM_NOT_PERMITTED.
*   03/02/20 1.255  SP 	Add more SPX_ERR_MON_xxx errors.
*   30/01/20 1.254  SP 	Add more SPX_ERR_JSON_xxx errors.
*   20/01/20 1.253  SP 	Add more SPX_ERR_MON_xxx errors.
*   20/01/20 1.252  SP 	Add more SPX_ERR_MON_xxx errors.
*   25/10/19 1.251  SP 	SPX_ERR_JSON_xxx errors.
*   11/09/19 1.250  SP 	Add more monitoring and redundancy errors.
*   10/09/19 1.249  SP 	Add more monitoring and redundancy errors.
*   09/09/19 1.248  SP 	Add monitoring and redundancy errors.
*   23/08/19 1.247  REW	Add SPX_ERR_LICENSE_LM.
*   19/08/19 1.246  SP 	Add SPX_ERR_NOT_FOUND.
*   26/06/19 1.245  AGC	Add SPX_ERR_AV_RTP_PORT_OVERRIDE.
*   07/06/19 1.244  SP 	Add SPX_ERR_STATUS_MISMATCH.
*   01/05/19 1.243  REW	Add SPX_ERR_FUSION_SENSOR_RESTART.
*   24/04/19 1.242  REW	Add SPX_ERR_FUSION_SLAVE_ENTER.
*   21/02/19 1.241  AGC	Add SPX_ERR_COL_CONV_FAILED.
*   06/02/19 1.240  AGC	Add ONVIF errors.
*   07/11/18 1.239  BTB	Add Environment Variable error.
*   19/10/18 1.238  DGJ	Additional MBT errors
*   28/09/18 1.237  SP 	Add SPX_ERR_TILED_MAP_KEY_NOT_SET and others.
*   06/07/18 1.236  REW	Add SPX_ERR_FUSION_SENSOR_TIMES.
*   04/06/18 1.235  AGC	Add SPX_ERR_LICENSE_EXPIRES_SOON.
*   14/05/18 1.234  REW	Correct SPX_ERR_MHT_CONFIG_BAD_FILE for DGJ.
*   01/05/18 1.233  REW	Add SPX_ERR_BB_RADIO_INTERSECT.
*   21/03/18 1.232  DGJ	Add SPX_ERR_MBT_BAD_MODEL_INSTANCE.
*   12/02/18 1.231  BTB	Add CANBus errors.
*   02/02/18 1.230  REW	Add SPX_ERR_AIS_NO_IMO, NO_IMAGE and NO_KEY.
*   23/01/18 1.229  AGC	Fix N/C #514 - Add SPxErrorWithRecord().
*   05/12/17 1.228  REW	Add SPX_ERR_LICENSE_NUM_APPS.
*   26/09/17 1.227  SP 	Add SPX_ERR_BAD_GROUP_ID.
*   26/07/17 1.226  AGC	Add SPX_ERR_LICENSE_NET_DISABLED.
*   26/06/17 1.225  SP 	Add SPX_ERR_RW.
*                       Change value of SPX_ERR_SQL.
*   22/06/17 1.224  AGC	Add SPX_ERR_NOT_READY.
*   20/04/17 1.223  REW	Add SPX_ERR_HW_INT_COUNT.
*   12/04/17 1.222  AGC	Add SPX_ERR_NET_FIREWALL_CONFIG.
*   08/03/17 1.221  AGC	Add SPX_ERR_HW_FASTSTARTUP_DETECTED.
*   01/03/17 1.220  REW	Add more specific fusion sensor codes.
*   23/02/17 1.219  DGJ Changed error details for MBT
*   23/01/17 1.218  DGJ Errors for track class in MBT
*   31/10/16 1.217  SP 	Add SPX_ERR_VID_SIM_RADAR_xxx.
*   31/10/16 1.216  REW	Add SPX_ERR_HW_BAD_STREAM.
*   13/10/16 1.215  AGC	Add SPX_ERR_AV_OPEN_H264_ERROR.
*   14/09/16 1.214  DGJ Additional errors for MBT.
*   02/09/16 1.213  DGJ	Additional errors for MBT.
*   23/08/16 1.212  DGJ	Additional errors for MBT
*   08/07/16 1.211  AGC Record error descriptions to file.
*   24/06/16 1.210  DGJ	Additional errors for MBT
*   29/04/16 1.209  SP 	Add SPX_ERR_FILE_SYNTAX.
*   20/04/16 1.208  AGC	Add SPX_ERR_SOCKET_CONNECT_RETRY.
*   24/03/16 1.207  SP 	Add SPX_ERR_SQL.
*   03/03/16 1.206  DGJ New errors for MBT work.
*   22/02/16 1.205  SP 	Add SPX_ERR_USER_CANCELLED.
*   12/02/16 1.204  AGC	Add more DirectShow errors.
*   04/02/16 1.203  AGC	Add V3 error handler with user arg.
*   22/12/15 1.202  REW	Add SPX_ERR_PASSWORD.
*   09/12/15 1.201  REW	Add SPX_ERR_HW_FIFO_... and SPX_ERR_P313 codes.
*   16/11/15 1.200  SP 	Add new file and directory errors.
*   22/10/15 1.199  AGC	Add SPX_ERR_RANDOM.
*   06/08/15 1.198  AGC	Add SPX_ERR_LICENSE_NUM_CHANNELS.
*			Rename SPX_ERR_LICENSE_NUM_CLIENTS.
*   29/07/15 1.197  SP 	Add SPX_ERR_WEB_IF errors.
*   05/06/15 1.196  SP 	Add SPX_ERR_VIEW_CTRL_VIEW_NOT_SET.
*                       Add SPX_ERR_RECORD_BAD_PACKET_TYPE.
*   11/05/15 1.195  SP 	Add SPX_ERR_RECORD_READ.
*                       Add SPX_ERR_RECORD_LINK_FAILED.
*   22/04/15 1.194  AGC	Add SPX_ERR_HW_UPDATE_DRIVER.
*   16/02/15 1.193  SP 	Add SPX_ERR_VIEW_CTRL_FAILED.
*   11/12/14 1.192  SP 	Add SPX_ERR_TIMELINE_NO_IMAGE.
*   11/12/14 1.191  REW	Add SPX_ERR_LICENSE_BLOCKED.
*   04/12/14 1.190  REW	Add SPX_ERR_HW_MEM_OVERFLOW.
*   14/10/14 1.189  AGC	Add SPX_ERR_TERRAIN_NO_INTERSECT.
*   15/09/14 1.188  AGC	Add SPX_ERR_RAD_SIM_OVERLOAD.
*   04/07/14 1.187  AGC	Include narrower standard header.
*   16/05/14 1.186  AGC	Add motion/template name mismatch error codes.
*   01/05/14 1.185  SP 	Add new SPX_ERR_TILED_MAP_ error codes.
*			Rename SPX_ERR_TILED_MAP_NO_FILE to NO_TILE.
*   10/04/14 1.184  AGC	Add SPX_ERR_VPX_NO_MORE_DATA.
*   26/03/14 1.183  AGC	Add SPX_ERR_RAD_SIM.
*			Add SPX_ERR_RAD_SIM_TARGET_NAME_MISMATCH.
*   05/03/14 1.182  AGC	Add SPX_ERR_SEEK_FILE.
*			Add SPX_ERR_VPX.
*   17/02/14 1.181  AGC	Add SPxSetErrorHandler() taking integer/long arg.
*   13/12/13 1.180  SP 	Add SPX_ERR_ACTIVE_REGION_WEIGHTING_FIXED.
*   02/12/13 1.179  SP 	Add SPX_ERR_JOYSTICK.
*   04/11/13 1.178  AGC	Add SPX_ERR_ORC_WINDOW_SIZE.
*   21/10/13 1.177  SP 	SPX_ERR_ACTIVE_REGION_POLY_NOT_LL.
*   17/09/13 1.176  AGC	Add SPX_ERR_AV_D2D_ERROR.
*   26/07/13 1.175  REW	Add SPX_ERR_HW_AUTO_RECOVER.
*   25/07/13 1.174  REW	More HW errors for HPx-300.
*   19/07/13 1.173  SP 	More video simulator errors.
*   15/07/13 1.172  SP 	Add video simulator errors.
*   15/07/13 1.171  REW	Add SPX_ERR_HW_SYNC_ERROR.
*   18/06/13 1.170  REW	Add SPX_ERR_HW_BIST_POWER.
*   06/06/13 1.169  SP 	Add SPX_ERR_GL_BAD_CONTEXT error.
*   09/05/13 1.168  AGC	Fix name of SPX_ERR_BAD_PIM_CONFIGURATION.
*   26/04/13 1.167  SP 	Add additional OpenGL errors.
*   25/04/13 1.166  SP 	Add SPX_ERR_GL_BAD_WINDOW error.
*   21/02/13 1.165  AGC	Add SPX_ERR_AV_WIN_SIZE error.
*   25/01/13 1.164  SP 	Add more SPX_ERR_GW_xxx errors.
*   25/01/13 1.163  AGC	Add SPX_ERR_AV_DVP_ERROR.
*   25/01/13 1.162  REW	Add SPX_ERR_NET_MSG_LOSS.
*   15/01/13 1.161  SP 	Add SPX_ERR_GW_BAD_COLOUR.
*   11/01/13 1.160  SP 	Add SPX_ERR_GW_xxx.
*   12/12/12 1.159  AGC	Add SPX_ERR_NET_PLOT_MOVE.
*   11/12/12 1.158  REW	Add SPX_ERR_P226.
*   09/11/12 1.157  AGC	Add SPX_ERR_TILED_MAP_OUT_OF_BOUNDS.
*   24/10/12 1.156  REW	Add SPX_ERR_LICENSE_NUMCLIENTS.
*   18/10/12 1.155  REW	Add SPX_ERR_LICENSE_HPX.
*   18/10/12 1.154  AGC	Print errors to file by default.
*   21/08/12 1.153  SP 	Add new SPX_ERR_TILED_MAP_xxx errors.
*   02/08/12 1.152  SP 	Add SPX_ERR_PPI_DRAWABLE_DEPTH.
*   02/08/12 1.151  AGC	Add SPX_ERR_AV_RTSP_ERROR.
*   13/07/12 1.150  AGC	Add SPX_ERR_ACTIVE_REGION_LL_ALREADY_SET.
*   06/06/12 1.149  SP 	Add SPX_ERR_TILED_MAP_xxx.
*			Add SPX_ERR_LL_CONV_FAILED.
*   30/04/12 1.148  SP 	Add SPX_ERR_ECDIS_COPROCESS_EXIT.
*   26/04/12 1.147  REW	Add SPX_ERR_WORLDMAP_FILE_LOAD.
*   19/04/12 1.146  SP 	Add SPX_ERR_ECDIS_BAD_SYS_CONFIG.
*   19/04/12 1.145  AGC	Add SPX_ERR_AV_GL_ERROR.
*   04/04/12 1.144  SP 	Add SPX_ERR_ECDIS_NO_SYS_CONFIG.
*   29/03/12 1.143  SP 	Add SPX_ERR_ENC_BAD_SLAVE.
*   29/03/12 1.142  AGC	Add SPX_ERR_WIN_DISPLAY_GPU.
*   29/03/12 1.141  DGJ	Add SPX_ERR_MHT_SWAP_TRACK_ID.
*   15/03/12 1.140  REW	Add SPX_ERR_NAV_NMEA_CHECKSUM.
*   08/03/12 1.139  DGJ	Add SPX_ERR_MHT_REPAIR_TRACK_ID
*   07/03/12 1.138  REW	Add SPX_ERR_HW_ACCESS_DENIED.
*   23/02/12 1.137  DGJ	Add SPX_ERR_MHT_VERIFY
*   20/02/12 1.136  AGC	Add SPX_ERR_AV_VDPAU_ERROR.
*   18/01/12 1.135  AGC	Add SPX_ERR_AV_RSVG_ERROR.
*   13/01/12 1.134  AGC	Add SPX_ERR_AV_V4L_ERROR.
*   13/01/12 1.133  SP 	Add SPX_ERR_ENC_xxx errors.
*			Add SPX_ERR_ECDIS_xxx errors.
*   21/12/11 1.132  REW	Add ADSB errors.
*   21/12/11 1.131  DGJ	Add SPX_ERR_ACTIVE_REGION_NO_LL_RADAR.
*			Add SPX_ERR_ACTIVE_REGION_FILE_SYNTAX.
*   16/12/11 1.130  AGC	Add SPX_ERR_AV_CL_ERROR.
*   02/11/12 1.129  AGC	Add SPxGetLastError(), SPxGetErrorString(),
*			and SPxGetErrorStringDetail().
*			Fix some MHT error comments.
*   18/11/11 1.128  AGC	Add SPX_ERR_AV_DSOUND_NO_DEVICE.
*   17/11/11 1.127  REW	Add SPX_ERR_MHT_PARAM_NOT_AREA_DEPEND for DGJ.
*   17/11/11 1.126  AGC	Add SPX_ERR_CLOSE_FILE.
*   14/11/11 1.125  SP 	Add ASTERIX error codes.
*   14/11/11 1.124  AGC	Add SPX_ERR_AV_MATROX_CHANNEL_IN_USE.
*			Add SPX_ERR_AV_DEVICE_LOST.
*			Add SPX_ERR_SOCKET_KEEPALIVE.
*   25/10/11 1.123  AGC	Add SPX_ERR_SG_BAD_TARGET_ID.
*   07/10/11 1.122  SP 	Add SPX_ERR_SERIAL_CLOSE.
*   04/10/11 1.121  REW	Add SPX_ERR_READ_FILE/WRITE_FILE.
*   27/09/11 1.120  AGC	Add SPX_ERR_AV_PROC_CHAIN_ACTIVE.
*   22/09/11 1.119  AGC	Add SPX_ERR_AV_WRONG_FRAME_RATE.
*   13/09/11 1.118  AGC	Add SPX_ERR_SERIAL_CFG_WRITE.
*			Add SPX_ERR_AV_NO_MORE_ITEMS.
*   07/09/11 1.117  AGC	Add SPX_ERR_AV_NVIDIA_ERROR.
*   31/08/11 1.116  AGC	Add V2 error handler.
*   19/08/11 1.115  SP 	Add SPX_ERR_BAD_PACKING_TYPE and 
*			SPX_ERR_BAD_SAMPLE_SIZE.
*   19/08/11 1.114  AGC	Add SPX_ERR_AV_DSOUND_ERROR.
*   03/08/11 1.113  SP 	Add generic serial port errors.
*   21/07/11 1.112  REW	Add FLASH read/write errors.
*   13/07/11 1.111  AGC	SPX_ERR_TIMER_TOO_MANY_FUNCTIONS.
*   07/07/11 1.110  SP 	Add track renderer errors.
*   27/06/11 1.109  SP 	Add SPX_ERR_START_PROC_FAILED.
*   27/06/11 1.108  REW	Add AIS errors.
*   09/06/11 1.107  AGC	Add AV server errors.
*   24/05/11 1.106  AGC	Add SPX_ERR_ASCAN_BAD_WINDOW, SPX_ERR_SOCKET_CONNECT,
*			and SPX_ERR_SOCKET_GETSOCKNAME.
*   24/05/11 1.105  DGJ	Add SPX_ERR_SG_BAD_PATH_TERM.
*   06/04/11 1.104  REW	Add SPX_ERR_HW_BIST_FLASH.
*   18/03/11 1.103  REW	Add SPX_ERR_HW_BAD_SIG.
*   04/02/11 1.102  AGC	Add SPX_ERR_GLX_DIFF_THREAD and SPX_ERR_GLX_NOT_CREATED.
*   28/01/11 1.101  AGC	Add GL/GLX error codes.
*   27/01/11 1.100  REW	Add SPX_ERR_HW_DAC_NOT_READY.
*   17/01/11 1.99   REW	Ensure space before = to assist with parsing.
*   22/12/10 1.98   AGC	Add file errors.
*   15/12/10 1.97   SP 	Add SPX_ERR_ACTIVE_REGION_NAME_FIXED etc.
*   05/11/10 1.96   REW	Add SPX_ERR_WORLDMAP_RESOLUTION etc.
*   28/10/10 1.95   AGC	Add SPX_ERR_LEGACY_FILE.
*   28/10/10 1.94   DGJ	Add SPX_ERR_MHT_EXTRA_TRACK_IN_H
*   29/09/10 1.93   REW	Add SPxGetErrorSeverityString().
*   29/09/10 1.92   REW	Add SPX_ERR_WARNING and SPX_ERR_INFO types.
*   20/08/10 1.91   SP 	Add SPX_ERR_NET_PLOT_DISCARD.
*   28/07/10 1.90   SP 	Add SPX_ERR_NOT_INITIALISED and
*			SPX_ERR_NOT_SUPPORTED.
*   27/05/10 1.89   DGJ	Add MHT DRC errors.
*   24/05/10 1.88   REW	Add SPX_ERR_HW_ACTIVE.
*   06/05/10 1.87   REW	Add SPX_ERR_HW_FAILED_RESET.
*   26/04/10 1.86   REW	Add generic SPX_ERR_ALREADY_DONE.
*   15/04/10 1.85   DGJ	Add SPX_ERR_PPI_BAD_WINDOW_DISPLAY.
*   22/03/10 1.84   SP 	Add GLX mixer errors.
*   08/03/10 1.83   REW	Add more fusion errors.
*   02/03/10 1.82   REW	Add more fusion errors.
*   24/02/10 1.81   REW	Add first fusion errors.
*   26/11/09 1.80   REW	Add SPX_ERR_P100.
*   23/11/09 1.79   REW	Add SPX_ERR_SOCKET_SETOPT.
*   05/10/09 1.78   REW	Add SPX_ERR_WIN_DISPLAY_NO_CREATE.
*   14/09/09 1.77   DGJ	Errors for button box
*   31/07/09 1.76   SP 	Add error handler function pointer type.
*   24/07/09 1.75   REW	Add SPX_ERR_HW_BUSY.
*   21/07/09 1.74   SP 	Add plugin and cuda errors.
*   08/07/09 1.73   SP  Add SPX_ERR_XDISPLAY_DAMAGE_EXTENSION,
*			SPX_ERR_XDISPLAY_FIXES_EXTENSION and
*			SPX_ERR_XDISPLAY_SHM_PIXMAPS.
*   19/06/09 1.72   REW	Add SPX_ERR_REPLAY_NO_TOC.
*   08/06/09 1.71   SP 	Add SPX_ERR_SG_BAD_ALIGNMENT.
*   24/04/09 1.70   DGJ	Add SPX_ERR_PIM_NO_AZI_REF.
*   19/03/09 1.69   DGJ	Add SPX_ERR_SG_BAD_RADAR.
*   27/02/09 1.68   REW	More navigation data errors.
*   26/02/09 1.67   REW	Add navigation data errors.
*   20/02/09 1.66   DGJ	Add A-Scan Errors. REW changed MHT ASSOC_MATRIX
*   08/02/09 1.65   REW	Add SPX_ERR_RIB_FULL.
*   23/01/09 1.64   DGJ	Add SPX_ERR_MHT_BUFFER_SIZE etc. for MHT
*   28/11/08 1.63   REW	Add SPX_ERR_SR_LARGE_FILE.
*   26/11/08 1.62   REW	Add SPX_ERR_SR_... screen recorder errors.
*   18/11/08 1.61   REW	Add SPX_ERR_DYNAMIC_LOAD.
*   21/10/08 1.60   REW	Add SPX_ERR_HW_TPG_... errors.
*   19/10/08 1.59   DGJ	Add error for ORC destination buffer sz.
*   17/10/08 1.58   REW	Add SPX_ERR_PROCESS_TYPE.
*   13/10/08 1.57   REW	Add SPX_ERR_LICENSE_CRYPT error.
*   13/10/08 1.56   DGJ	Add more PPI errors.
*   09/10/08 1.55   REW	Add SPX_ERR_LICENSE_MAC.
*   25/09/08 1.54   DGJ	Add ASCAN errors.
*   25/09/08 1.53   REW	Add SPX_ERR_SOCKET_LISTEN/SELECT and
*			SPX_ERR_ZLIB_UNCOMRPESS.
*   15/09/08 1.52   DGJ	Add SPX_ERR_LUT_INDEX.
*   16/09/08 1.51   REW	Add SPX_ERR_SOCKET_INIT.
*   27/08/08 1.50   REW	Add SPX_ERR_SOCKET_CONNECTED and
*			SPX_ERR_HW_NOT_SUPPORTED.
*   05/08/08 1.49   REW	Add SPX_ERR_HW_ATEXIT.
*   05/08/08 1.48   REW	Remove trailing comma.
*   25/07/08 1.47   DGJ	Add Scenario generator errors
*   22/07/08 1.46   REW	Add SPX_ERR_P126 and other projects.
*   17/07/08 1.45   REW	Add SPX_ERR_HW_INT_KICKSTART.
*   23/06/08 1.44   DGJ	MHT Errors start
*   29/05/08 1.43   REW	Add SPX_ERR_INIT.
*   08/04/08 1.42   REW	Add hardware DAC-related errors.
*   04/04/08 1.41   REW	Add hardware LUT-related errors.
*   19/03/08 1.40   REW	Add hardware-related errors.
*   13/02/08 1.39   REW	Add SPX_ERR_NETSEND_CREATE.
*   13/02/08 1.38   DGJ	Error messages for PPI class
*   01/02/08 1.37   DGJ	Expanded text for some error messages
*   30/01/08 1.36   REW	Add more licensing errors.
*   24/01/08 1.35   REW	Add more licensing errors.
*   23/01/08 1.34   DGJ	Add SPX_ERR_BAD_INDEX_FOR_FOLLOW.
*   18/01/08 1.33   DGJ	Add SPX_ERR_STRING_LENGTH.
*   15/01/08 1.32   REW	Remove trailing comma from SPxErrorType.
*   07/01/08 1.31   DGJ	Additional comments for error messages.
*   02/01/08 1.30   REW	Add SPX_ERR_SYSCALL. Correct 1.22 to 1.30.
*   11/12/07 1.29   DGJ	New error code for process parameter
*   11/12/07 1.28   REW	Add SPX_ERR_REPLAY_READ.
*   10/12/07 1.27   REW	Error codes for recording process.
*			Add SPxDefaultErrorHandler().
*   12/10/07 1.26   DGJ	Error code for Process Manager
*   08/10/07 1.25   DGJ	Error codes for target database
*   28/09/07 1.24   DGJ	SPX_ERR_PROCESS_PIMS ecode.
*   17/09/07 1.23   DGJ	Region processing error codes
*   13/09/07 1.22   DGJ	Add SPX_ERR_BAD_MODE_COMBINE.
*   10/09/07 1.21   REW	Add SPX_ERR_COMM_FAILED.
*   10/09/07 1.20   REW	Add SPX_ERR_BAD_COMM_REPLY.
*   06/09/07 1.19   DGJ	Add SPX_ERR_NET_RESTACK_ATOM
*   26/08/07 1.18   DGJ	SPX_ERR_TOO_MANY_SC_WINS, 
*			SPX_ERR_SG_RIB_CONNECT error code
*   25/07/07 1.17   REW	More SOCKET errors, NET and ORC errors.
*   16/07/07 1.16   REW	More REPLAY errors.
*   06/07/07 1.15   DGJ	Add SPX_ERR_BAD_PARAMETER_VALUE.
*   04/07/07 1.14   DGJ Add SPX_ERR_XDISPLAY_BAD_DISPLAY.
*   04/07/07 1.13   REW	Add SPX_ERR_PATCH_PROTOCOL.
*   02/06/07 1.12   REW	Add SPX_ERR_TIMEOUT.
*   29/06/07 1.11   DGJ	BAD_OBJECT error
*   28/06/07 1.10   REW	Fix error I introduced in last merge.
*   27/06/07 1.9    DGJ	Added SPX_ERR_XDISPLAY_NO_DISPLAY_SET and
*			SPX_ERR_BITMAP_... errors.
*   21/06/07 1.8    DGJ	Added SPX_ERR_STUB_CREATE_DISPLAY.
*   19/06/07 1.7    DGJ	Added file replay errors.
*   15/06/07 1.6    DGJ	Added SPX_ERR_XDISPLAY_WM_HINTS.
*   13/06/07 1.5    REW	Add SPX_ERR_BAD_PROCESS_SOURCE.  Remove
*			extra S from PROCESSS.  DGJ added Win errors.
*   08/06/07 1.4    DGJ	Added more errors and reorganised.
*   07/06/07 1.3    REW	Add SPX_ERR_READ_FAILED.
*   04/06/07 1.2    DGJ	Added many more error codes. (REW reformatted).
*   24/05/07 1.1    REW	Initial Version, from SPxCommon.h.
**********************************************************************/

#ifndef _SPX_ERROR_H
#define _SPX_ERROR_H

/* Other includes */
#include <cstddef>		/* For NULL */

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

/*
 * Type for an error type.
 */
typedef enum
{
    SPX_ERR_SYSTEM = 10,		/* System error */
    SPX_ERR_SYSTEM_CANT_RECOVER = 11,	/* Serious system error */
    SPX_ERR_USER = 12,			/* User error, eg param val wrong. */
    SPX_ERR_WARNING = 13,		/* Minor issue, but carrying on */
    SPX_ERR_INFO = 14			/* Information only */
} SPxErrorType;

/*
 * List of error codes.
 */

// IMPORTANT
// NOTE: The list of error messages below is read by a program to create the
// table that appears in the user-manual. So please ensure that each error 
//  conforms to the following form:

//  ErrorName = ErrorValue /* Error message */

//  The Error message may be multiline, but the comment must start on the same
//  line as the error name and value.
//
//  After adding errors the generate-error-strings.py script in Scripts should
//  be run to update the error strings within the SPxError.cpp file.
//
typedef enum
{
// DO NOT EDIT OR CHANGE THE COMMENT LINE BELOW AS THIS IS USED FOR ERROR PROCESSING */
/* STARTOFERRORS */
    SPX_NO_ERROR = 0,			    /* No Error */
    SPX_ERR_ASSERT = 1,			    /* Internal logic failure */
    SPX_ERR_UNKNOWN = 2,		    /* Unknown error */
    SPX_ERR_SYSCALL = 3,		    /* System call error */
    SPX_ERR_INIT = 4,			    /* Library not initialised */
    SPX_ERR_DYNAMIC_LOAD = 5,		    /* Failed a dynamic load */

    SPX_ERR_LICENSE = 10,		    /* Feature unlicensed */
    SPX_ERR_LICENSE_FILE = 11,		    /* Problem with a license file */
    SPX_ERR_LICENSE_DONGLE = 12,	    /* We didn't find a dongle */
    SPX_ERR_LICENSE_EXPIRED = 13,	    /* License has expired */
    SPX_ERR_LICENSE_VERSION = 14,	    /* License is for the wrong version */
    SPX_ERR_LICENSE_INVALID = 15,	    /* License is corrupt/invalid */
    SPX_ERR_LICENSE_DONGLE_INIT = 16,	    /* Problem initialising dongle lib */
    SPX_ERR_LICENSE_DONGLE_READ = 17,	    /* Problem reading from dongle */
    SPX_ERR_LICENSE_DONGLE_INVALID = 18,    /* Dongle contents not valid */
    SPX_ERR_LICENSE_DONGLE_REMOVED = 19,    /* Dongle removed/changed */
    SPX_ERR_LICENSE_MAC = 20,		    /* MAC address mismatch */
    SPX_ERR_LICENSE_CRYPT = 21,		    /* Encryption/decryption problem */
    SPX_ERR_LICENSE_HPX = 22,		    /* HPx not found for license */
    SPX_ERR_LICENSE_NUM_CLIENTS = 23,	    /* Too many clients */
    SPX_ERR_LICENSE_BLOCKED = 24,	    /* This license is blocked */
    SPX_ERR_LICENSE_NUM_CHANNELS = 25,	    /* Too many channels */
    SPX_ERR_LICENSE_NET_DISABLED = 26,	    /* Network interface disabled */
    SPX_ERR_LICENSE_NUM_APPS = 27,	    /* Too many applications */
    SPX_ERR_LICENSE_EXPIRES_SOON = 28,	    /* License will expire soon */
    SPX_ERR_LICENSE_LM = 29,		    /* License Manager error */
    SPX_ERR_LICENSE_SUPPORT = 30,	    /* Out of license support period */

    SPX_ERR_NO_FILE = 100,		    /* Missing file */
    SPX_ERR_SYNTAX_IN_FILE = 101,	    /* Syntax error in file */
    SPX_ERR_BAD_PARAMETER = 102,	    /* The parameter is not valid */
    SPX_ERR_WOULD_BLOCK = 103,		    /* The call would cause a block */
    SPX_ERR_CANT_ADD_RUN_PROCESS = 104,	    /* Failed to add a RunProcess object */
    SPX_ERR_BAD_BUFFER_SIZE = 105,	    /* Bad size of buffer */
    SPX_ERR_BAD_MALLOC = 106,		    /* A call to malloc failed */
    SPX_ERR_BUFFER_TOO_SMALL = 107,	    /* A buffer is too small */
    SPX_ERR_BAD_ARGUMENT = 108,		    /* An argument is out of range */
    SPX_ERR_STUB_CREATE_DISPLAY = 109,	    /* Called stub SPxScCreateDisplay */
    SPX_ERR_BAD_OBJECT_FOR_COMMAND = 110,   /* Bad object specified by command */
    SPX_ERR_BAD_PARAMETER_FOR_COMMAND = 111,/* Bad parameter for command */
    SPX_ERR_NOSUPPORT_COMMAND = 112,	    /* Object doesn't support command */
    SPX_ERR_TIMEOUT = 113,		    /* Something timed out */
    SPX_ERR_BAD_PARAMETER_VALUE = 114,	    /* Bad value for parameter in command */
    SPX_ERR_RIB_INACTIVE = 115,		    /* Writing to inactive RIB */
    SPX_ERR_RIB_FULL = 116,		    /* The RIB is full */
    SPX_ERR_ALREADY_DONE = 117,		    /* Something already done/used */
    SPX_ERR_NOT_INITIALISED = 118,	    /* Object not initialised/created */
    SPX_ERR_NOT_SUPPORTED = 119,	    /* Operation not supported */

// Utility functions.
    SPX_ERR_THREAD_CREATE_FAILED = 120,	    /* Failed to create thread */
    SPX_ERR_THREAD_ACTIVE = 121,	    /* Thread already active */
    SPX_ERR_THREAD_INACTIVE = 122,	    /* No current thread */
    SPX_ERR_EVENT_CREATE_FAILED = 123,	    /* Failed to create event object */
    SPX_ERR_EVENT_EXISTS = 124,		    /* Event object already exists */
    SPX_ERR_EVENT_NOEXIST = 125,	    /* Event object doesn't exist */
    SPX_ERR_MUTEX_CREATE_FAILED = 126,	    /* Failed to create mutex/crit-sect */
    SPX_ERR_MUTEX_EXISTS = 127,		    /* Mutex already exists */
    SPX_ERR_MUTEX_NOEXIST = 128,	    /* Mutex doesn't exist */
    SPX_ERR_MUTEX_LOCK = 129,		    /* Failed to lock mutex */
    SPX_ERR_MUTEX_UNLOCK = 130,		    /* Failed to unlock mutex */
    SPX_ERR_START_PROC_FAILED = 131,	    /* Failed to start a process */
    SPX_ERR_TIMER_TOO_MANY_FUNCTIONS = 132, /* Too many callback functions for a timer */
    SPX_ERR_LL_CONV_FAILED = 133,	    /* Lat/long conversion error. */
    SPX_ERR_RANDOM = 134,		    /* Random number generation error. */
    SPX_ERR_PASSWORD = 135,		    /* Password error. */
    SPX_ERR_USER_CANCELLED = 136,           /* Operation was cancelled by user. */
    SPX_ERR_NOT_READY = 137,		    /* Operation not ready. */
    SPX_ERR_COL_CONV_FAILED = 138,	    /* Colour conversion failed. */
    SPX_ERR_STATUS_MISMATCH = 139,          /* Status value does not match */
    SPX_ERR_NOT_FOUND = 140,                /* Object not found. */
    SPX_ERR_STOP_PROC_FAILED = 141,	    /* Failed to stop a process */
    SPX_ERR_DATA_EXPIRED = 142,		    /* Data has expired. */
    SPX_ERR_PARAM_NOT_SET = 143,	    /* Parameter is not set, and should not be saved. */
    SPX_ERR_ENCRYPTION_FAILED = 144,        /* Encryption failed */
    SPX_ERR_DECRYPTION_FAILED = 145,        /* Decryption failed */
    SPX_ERR_BAD_PROJECT_CODE = 146,         /* Bad project code. */

// PIM Errors
    SPX_ERR_PIM_MATCH = 150,		    /* PIM specified doesn't match pim of previous process. */
    SPX_ERR_PIM_ALREADY_CONNECTED = 151,    /* The PIM is already connected */
    SPX_ERR_BAD_PIM_CONFIGURATION = 152,    /* The PIM configuration is inconsistent */
    SPX_ERR_BAD_RADAR = 153,		    /* Bad Radar specified */
    SPX_ERR_BAD_PIM_HEADER = 154,	    /* Error in pim header */
    SPX_ERR_SC_SCALE = 155,		    /* Scan conv scale was out of range */
    SPX_ERR_BAD_PIM_TYPE = 156,		    /* Bad type of PIM encountered */
    SPX_ERR_BAD_RAN_COMBINE = 157,	    /* Bad range combination */
    SPX_ERR_BAD_AZI_COMBINE = 159,	    /* Bad azimuth combine mode */
    SPX_ERR_RIB_LOGIC = 160,		    /* RIB buffer logic is broken */
    SPX_ERR_NO_CONFIG_FILE = 161,	    /* Could not open the config file */
    SPX_ERR_DUMP_FILE_ACCESS = 162,	    /* Couldn't open dump file for write */
    SPX_ERR_BAD_PIM_ARGUMENT = 163,	    /* Bad arg in attempt to create pim */
    SPX_ERR_PIM_NO_AZI_REF = 164,	    /* PIM azimuth adjustment is
					     * configured to use flag in input
					     * video to define north/heading
					     * but the flag isn't set.
					     */
    SPX_ERR_BAD_PACKING_TYPE = 165,         /* Bad radar header packing type */
    SPX_ERR_BAD_SAMPLE_SIZE = 166,          /* Sample size is not supported */

// File related errors.
    SPX_ERR_CREATE_FILE = 180,		    /* Could not create file */
    SPX_ERR_DELETE_FILE = 181,		    /* Could not delete file */
    SPX_ERR_RENAME_FILE = 182,		    /* Could not rename file */
    SPX_ERR_LEGACY_FILE = 183,		    /* File contains legacy parameters */
    SPX_ERR_OPEN_FILE = 184,		    /* Could not open file. */
    SPX_ERR_READ_FILE = 185,		    /* Could not read file. */
    SPX_ERR_WRITE_FILE = 186,		    /* Could not write file. */
    SPX_ERR_CLOSE_FILE = 187,		    /* Could not close file. */
    SPX_ERR_FILE_ALREADY_EXISTS = 188,      /* File already exists. */
    SPX_ERR_FILE_DOES_NOT_EXIST = 189,      /* File does not exist. */
    SPX_ERR_SEEK_FILE = 190,		    /* Could not seek file. */
    SPX_ERR_CREATE_DIR = 191,		    /* Could not create directory. */
    SPX_ERR_DELETE_DIR = 192,		    /* Could not delete directory. */
    SPX_ERR_RENAME_DIR = 193,		    /* Could not rename directory. */
    SPX_ERR_DIR_ALREADY_EXISTS = 194,       /* Directory already exists. */
    SPX_ERR_DIR_DOES_NOT_EXIST = 195,       /* Directory does not exist. */
    SPX_ERR_FILE_SYNTAX = 196,              /* Error in file syntax. */ 
    SPX_ERR_REAL_PATH = 197,		    /* Failed to resolve real path. */

// Command Processing
    SPX_ERR_BAD_COMM_ARGS = 200,	    /* Bad argument count. */
    SPX_ERR_BAD_COMM_COMMAND = 201,	    /* Bad command */
    SPX_ERR_BAD_COMM_WID = 202,		    /* Bad window id */
    SPX_ERR_PATCH_MAGIC = 203,		    /* Bad magic number in patch */
    SPX_ERR_PATCH_PROTOCOL = 204,	    /* Bad patch length */
    SPX_ERR_PATCH_FORMAT = 205,		    /* Bad patch format */
    SPX_ERR_PATCH_SIZE = 206,		    /* Bad patch size */
    SPX_ERR_PATCH_COUNT = 207,		    /* Bad patch count */
    SPX_ERR_PATCH_LEN = 208,		    /* Bad patch length */
    SPX_ERR_CANT_CREATE_WIN = 209,	    /* Couldn't create window. Too many? */
    SPX_ERR_BAD_COMM_REPLY = 210,	    /* Unknown response */
    SPX_ERR_COMM_FAILED = 211,		    /* Command failed, unknown reason. */
    SPX_ERR_COMM_PASSWORD_INCORRECT = 212,  /* Password is incorrect. */
    SPX_ERR_COMM_NOT_PERMITTED = 213,	    /* Command not permitted. */
    SPX_ERR_COMM_NOT_AVAILABLE = 214,	    /* Command or function not available. */
    SPX_ERR_COMM_INCOMPLETE = 215,	    /* Command or function incomplete. */

// Networking
    SPX_ERR_NETWORK_NOT_CREATED = 260,		/* Network not configured */
    SPX_ERR_DEST_NET_WIN_CONSTRUCT = 261,	/* Window not constructed. */
    SPX_ERR_NETWORK_NOT_CONNECTED = 262,	/* Network not connected */
    SPX_ERR_NETWORK_CONNECT_FAIL = 263,		/* Connect failed on network. */
    SPX_ERR_SOCKET_OPEN = 264,			/* A socket open failed. */
    SPX_ERR_SOCKET_BIND = 265,			/* A socket bind failed */
    SPX_ERR_SOCKET_NON_BLOCKING = 266,		/* Could not set non-blocking */
    SPX_ERR_BAD_NET_HEARTBEAT = 267,		/* Bad heartbeat format detected */
    SPX_ERR_READ_FAILED = 268,			/* A read failed */
    SPX_ERR_SEND_FAILED = 269,			/* A write failed */
    SPX_ERR_SOCKET_REUSEADDR = 270,		/* Couldn't set SO_REUSEADDR */
    SPX_ERR_SOCKET_RCVBUF = 271,		/* Couldn't set SO_RCVBUF */
    SPX_ERR_SOCKET_SNDBUF = 272,		/* Couldn't set SO_SNDBUF */
    SPX_ERR_NET_BAD_MSG = 273,			/* Unknown/bad message */
    SPX_ERR_NET_BAD_ORDER = 274,		/* Cannot handle ordering */
    SPX_ERR_NET_BAD_FMT = 275,			/* Unknown/bad encode format */
    SPX_ERR_NETSEND_CREATE = 276,		/* Network distribution
						 * object Create() failed or
						 * not called.
						 */
    SPX_ERR_SOCKET_CONNECTED = 277,		/* Already connected */
    SPX_ERR_SOCKET_INIT = 278,			/* Socket startup failed */
    SPX_ERR_SOCKET_LISTEN = 279,		/* Socket listen() failed */
    SPX_ERR_SOCKET_SELECT = 280,		/* Socket select() failed */
    SPX_ERR_SOCKET_IOCTL = 281,			/* Socket ioctl() failed */
    SPX_ERR_SOCKET_SETOPT = 282,		/* setsockopt() failed */
    SPX_ERR_NET_PLOT_DISCARD = 283,		/* Plot discarded from message */
    SPX_ERR_SOCKET_CONNECT = 284,		/* Socket connect() failed */
    SPX_ERR_SOCKET_GETSOCKNAME = 285,		/* Socket getsockname() failed */
    SPX_ERR_SOCKET_KEEPALIVE = 286,		/* Couldn't set SO_KEEPALIVE */
    SPX_ERR_NET_PLOT_MOVE = 287,		/* Plot moved from position in message */
    SPX_ERR_NET_MSG_LOSS = 288,			/* Possible network dropout */
    SPX_ERR_SOCKET_CONNECT_RETRY = 289,		/* Socket connect() failed, but retrying */
    SPX_ERR_NET_FIREWALL_CONFIG = 290,		/* Possible firewall configuration */
    SPX_ERR_NET_NO_SECTOR_UPDATE = 291,		/* Sector crossing messages not received. */

// X Display Errors    
    SPX_ERR_XDISPLAY_CONNECT = 300,             /* Could not connect to the X Display */
    SPX_ERR_XDISPLAY_ARGB_VISUAL = 301,         /* No ARGB visual found. */
    SPX_ERR_XDISPLAY_SHM_EXTENSION = 302,       /* No SHM extension available. */
    SPX_ERR_XDISPLAY_SHM_VERSION = 303,         /* SHM extension version is too early.*/
    SPX_ERR_XDISPLAY_SHM_CREATE = 304,          /* Can't create the SHM extension */
    SPX_ERR_XDISPLAY_SHM_ATTACH = 305,          /* SHM attach problem.*/
    SPX_ERR_XDISPLAY_SHM_ATTACH_SERVER = 306,   /* SHM attach problem.*/
    SPX_ERR_XDISPLAY_IMAGE_CREATE = 307,        /* Failed to create the X Image */
    SPX_ERR_DISPLAY_SERVER_PATCH_FORMAT = 308,  /* The patch format isn't supported.*/
    SPX_ERR_DISPLAY_SERVER_PATCH_SIZE = 309,    /* Unexpected patch size.*/
    SPX_ERR_XDISPLAY_BAD_ULAY = 310,            /* Bad underlay window specified */
    SPX_ERR_XDISPLAY_BAD_OLAY = 311,            /* Bad overlay window specified */
    SPX_ERR_XDISPLAY_BAD_WIN_PARAM = 312,       /* Bad window parameter specified.*/
    SPX_ERR_XDISPLAY_TOO_MANY_WINDOWS = 313,    /* Too many window created. */ 
    SPX_ERR_XDISPLAY_WM_HINTS = 314,            /* Could not set the decoration hint */
    SPX_ERR_XDISPLAY_NO_DISPLAY_SET = 315,	/* No X Display set */
    SPX_ERR_XDISPLAY_BAD_DISPLAY = 316,         /* Could not open the X Display */
    SPX_ERR_NET_RESTACK_ATOM = 317,             /* Restack atom doesn't exist */
    SPX_ERR_XDISPLAY_DAMAGE_EXTENSION = 318,    /* No XDamage extension available. */
    SPX_ERR_XDISPLAY_FIXES_EXTENSION = 319,     /* No XFixes extension available. */
    SPX_ERR_XDISPLAY_SHM_PIXMAPS = 320,         /* No XShm pixmaps available. */
    SPX_ERR_PPI_BAD_RADAR = 330,		/* Bad radar index for PPI function */
    SPX_ERR_PPI_BAD_FADE = 331,			/* Bad fade type specified for PPI function */
    SPX_ERR_PPI_SET = 332,			/* Call SetSPx only for first video in PPI */
    SPX_ERR_PPI_BAD_BITMAP = 333,		/* Bad bitmap referenced in PPI class */
    SPX_ERR_PPI_BAD_SC = 334,			/* Bad scan conversion object referenced in PPI class */
    SPX_ERR_PPI_RADAR_COUNT = 335,		/* Bad radar count for PPI class */
    SPX_ERR_PPI_COMPOSITE_EXTENSION = 336,	/* The X Server is not
						 * configured to support the
						 * COMPOSITE extension.
						 */
    SPX_ERR_PPI_NUM_VIDEOS = 337,		/* Requested number of radar videos is out of range */
    SPX_ERR_PPI_MEMORY = 338,			/* Memory allocation failed. */
    SPX_ERR_PPI_IMAGE_FAIL = 339,		/* Failed to create the Image for radar video. */
    SPX_ERR_PPI_SM_EXTENSION = 340,		/* The X Server does not
						 * support the shared-memory
						 * extension.
						 */
    SPX_ERR_PPI_SM_GET = 341,			/* Could not create a shared-memory segment */
    SPX_ERR_PPI_SM_ATTACH = 342,		/* Could not attach to the shared-memory segment */	
    SPX_ERR_PPI_NO_BITMAP = 343,		/* Bitmap was null in CompositeDIsplay (Called SetSPx?) */
    SPX_ERR_PPI_NULL_WINDOW = 344,		/* Null Windows id specified for Redirect */
    SPX_ERR_PPI_WINDOW_REDIRECT = 345,		/* Window is already specified for redirection */
    SPX_ERR_PPI_WINDOW_DEPTH = 346,		/* Window depth isn't 24 or 32 */
    SPX_ERR_PPI_PIXMAP_DEPTH = 347,		/* Pixmap depth isn't 24 or 32 */
    SPX_ERR_PPI_WIN_SIZE = 348,			/* Bad window size requested */	
    SPX_ERR_PPI_DRAWABLE_EXISTS = 349,		/* A display drawable is
						 * already set and cannot be
						 * changed.
						 */
    SPX_ERR_PPI_BAD_DRAWABLE = 350,		/* A bad drawable was specified */
    SPX_ERR_PPI_NULL_DISPLAY = 351,		/* A null X Display was provided */
    SPX_ERR_PPI_BAD_WINDOW_DISPLAY = 352,	/* Null X Display or X Windows */
    SPX_ERR_PPI_DRAWABLE_DEPTH = 353,		/* Drawable depth isn't 24 or 32 */

// Processing
    SPX_ERR_COMBINE_INDEX = 400,	    /* Bad index for combine table */
    SPX_ERR_BAD_EXEC_PROCESS = 401,	    /* Bad process provided to SPxRunProcess. */
    SPX_ERR_BAD_PROCESS_PARAMETER = 402,    /* Bad process parameter */
    SPX_ERR_REAL_PARAM_AS_INT = 403,	    /* Getting a real param as an int */
    SPX_ERR_BAD_PROCESS_SOURCE = 404,	    /* Bad process source (proc/PIM) */
    SPX_ERR_BAD_MODE_COMBINE = 405,	    /* Bad mode for combine process. */
    SPX_ERR_POLY_POINTS = 420,		    /* Poly must have at least 4 pnts */
    SPX_ERR_POLY_OPEN = 421,		    /* Poly must be closed. */
    SPX_ERR_POLY_COLINEAR = 422,	    /* Poly is colinear */
    SPX_ERR_ACTIVE_REGION_ARG = 423,	    /* Bad argument for active region */
    SPX_ERR_NO_AREA_FILE = 424,		    /* Failed to open area file */
    SPX_ERR_ACTIVE_REGION_MODE = 425,	    /* Bad region mode specification */
    SPX_ERR_ACTIVE_REGION_POINTS = 426,	    /* Error in format of polygon
					     * points being read from file.
					     */
    SPX_ERR_ACTIVE_REGION_AZIMUTH = 427,    /* Bad azi for region interval */
    SPX_ERR_AREA_SELECT_MISSING = 428,	    /* SPxAreaSelect object missing in
					     * SPxProAreaSelect process
					     */
    SPX_ERR_SSI_LUT_MISSING = 429,	    /* LUT missing for SSI Process. */
    SPX_ERR_SSI_PIM_SIZE_MISMATCH = 430,    /* PIM size mismatch */
    SPX_ERR_PROCESS_PIMS = 431,		    /* Incorrect PIMs for process. */
    SPX_ERR_ASCANWIN_PROCESS = 432,	    /* Missing AScanWindow object in
					     * AScan Process.
					     */
    SPX_ERR_PROC_BAD_TARGET_SECTOR = 433,   /* Bad sector number for target
					     * database.
					     */
    SPX_ERR_PROCESS_MANAGER = 434,	    /* Process manager for process isn't
					     * same as process manager for input
					     * process. Could indicate
					     * cross-thread problem.
					     */
    SPX_ERR_PARAM_STRING_LEN = 435,	    /* The length of the string
					     * parameter being assigned to a
					     * process parameter is too large.
					     */
    SPX_ERR_STRING_LENGTH = 436,	    /* The string length is too small
					     * for the return of a parameters
					     * value.
					     */
    SPX_ERR_LUT_INDEX = 437,		    /* Bad LUT index specified in file 
					     * that defines LUT contents.
					     */
    SPX_ERR_PROCESS_TYPE = 438,		    /* Wrong/bad process type found */
    SPX_ERR_ACTIVE_REGION_NAME_FIXED = 439, /* Active region name cannot be changed. */
    SPX_ERR_ACTIVE_REGION_APP_FIXED = 440,  /* Active region application type cannot 
                                             * be changed.
                                             */
    SPX_ERR_ACTIVE_REGION_NO_LL_RADAR = 441, /* The lat/long of the radar has
					      * not been set, but a lat/long
					      * polygon is being used.
					      */

    SPX_ERR_ACTIVE_REGION_FILE_SYNTAX = 442, /* Syntax error reading active
					      * region file.
					      */
    SPX_ERR_ACTIVE_REGION_LL_ALREADY_SET = 443, /* The lat/long of the radar has
						 * already been set, but a file
						 * contains a different radar
						 * lat/long setting.
						 */
    SPX_ERR_ACTIVE_REGION_POLY_NOT_LL = 444, /* A master active region contains 
                                              * a polygon defined in metres not 
                                              * lat/long.
                                              */
    SPX_ERR_ACTIVE_REGION_WEIGHTING_FIXED = 445,/* Active region weighting cannot 
                                                 * be changed.
                                                 */

// Scenario Generation
    SPX_ERR_SCENARIO_FILE = 500,	/* Could not open the scenario file */
    SPX_ERR_TOO_MANY_SG_TARGETS = 501,	/* Too many scenario targets */
    SPX_ERR_NO_RADAR = 502,		/* No such radar found in database */
    SPX_ERR_SC_SECTOR = 503,		/* Bad sector for scan conversion */
    SPX_ERR_SG_RIB_CONNECT = 504,	/* Could not add a RIB - too many? */
    SPX_ERR_SG_PATH = 505,		/* Found end of file whilst reading
					 * path command (missing endpath)
					 */
    SPX_ERR_SG_BAD_COMMAND = 506,	/* Bad command found in scenario file */
    SPX_ERR_SG_PATH_SIZE = 507,		/* The path has too few points. */
    SPX_ERR_SG_BAD_PATH_ID = 508,	/* A bad path id was specified */
    SPX_ERR_SG_BAD_PATH_CREATE = 509,	/* Failed to create a new path */
    SPX_ERR_SG_BAD_RADAR = 510,		/* A bad radar ID or name was used */
    SPX_ERR_SG_BAD_ALIGNMENT = 511,	/* A bad target alignment arg was used */
    SPX_ERR_SG_BAD_PATH_TERM = 512,	/* There was an error in a line
					 * defining a point for a path.
					 */
    SPX_ERR_SG_BAD_TARGET_ID = 513,	/* A bad target id was specified */

// Windows Display Module
    SPX_ERR_WIN_DISPLAY_BAD_WIN_PARAM = 600,	/* Bad window parameter */
    SPX_ERR_WIN_DISPLAY_BITMAP = 601,	/* Couldn't create bitmap */
    SPX_ERR_WIN_DISPLAY_LAYWIN = 602,	/* Couldn't create layered window */
    SPX_ERR_TOO_MANY_SC_WINS = 603,	/* Too many SC windows being tracked */
    SPX_ERR_BAD_INDEX_FOR_FOLLOW = 604, /* Bad index in adding a follow win. */
    SPX_ERR_WIN_DISPLAY_NO_CREATE = 605, /* Window hasn't been created */
    SPX_ERR_WIN_DISPLAY_GPU = 606,	/* Failed to use GPU resource. */

// Recording process.
    SPX_ERR_RECORD_TIME = 700,		/* Failed to get time */
    SPX_ERR_RECORD_FILE_OPEN = 701,	/* Failed to open the file */
    SPX_ERR_RECORD_WRITE = 702,		/* Failed to write to file */
    SPX_ERR_RECORD_READ = 703,          /* Failed to read from file. */
    SPX_ERR_RECORD_LINK_FAILED = 704,   /* Failed to link packets in the file. */
    SPX_ERR_RECORD_BAD_PACKET_TYPE = 705, /* Packet type cannot be recorded. */

// Radar Replay Module
    SPX_ERR_REPLAY_FILE_OPEN = 800,		/* Could not open the file */
    SPX_ERR_REPLAY_READ = 801,			/* Failed a read from file */
    SPX_ERR_REPLAY_BAD_RANGE_INFO = 802,	/* Bad samples per return */
    SPX_ERR_REPLAY_BAD_AZIMUTH_INFO = 803,	/* Bad azimuth count */
    SPX_ERR_REPLAY_BAD_TIME_INFO = 804,		/* Bad prf */
    SPX_ERR_REPLAY_BAD_SAMPLE_INFO = 805,	/* Bad sampling interval */
    SPX_ERR_REPLAY_BAD_HEADER = 806,		/* Bad file header */
    SPX_ERR_REPLAY_BAD_FORMAT = 807,		/* Unknown format */
    SPX_ERR_REPLAY_NO_TOC = 808,		/* No table of contents */
    SPX_ERR_REPLAYED_ERROR = 850,		/* Replayed error */

// Bitmap destination
    SPX_ERR_BITMAP_BAD_TYPE = 900,	/* Bad bitmap type */
    SPX_ERR_BITMAP_NOFADE = 901,	/* Cannot fade this type of bitmap */

// Compression/decompression
    SPX_ERR_ORC_MAGIC_HEADER = 1000,	/* Bad magic number in header */
    SPX_ERR_ORC_DECODE_ERROR = 1001,	/* General decode error */
    SPX_ERR_ORC_SELF_TEST_FAIL = 1002,	/* Compression self test failed */
    SPX_ERR_ORC_MEMORY_SIZE = 1003,	/* Not enough memory */
    SPX_ERR_ORC_DEST_BUFFER_SZ = 1004,	/* Buffer for decompressed data is
					 * too small
					 */
    SPX_ERR_ORC_WINDOW_SIZE = 1005,	/* Window size too large */
    SPX_ERR_ZLIB_UNCOMPRESS = 1010,	/* ZLib uncompress() failed */
    SPX_ERR_ZLIB_COMPRESS = 1011,	/* ZLib compress() failed */

// Tracking etc.
    SPX_ERR_TRACKING_CREATE = 1101,	  /* Failed to create new track */
    SPX_ERR_TRACKING_BAD_HANDLE = 1102,	  /* Bad track handle */
    SPX_ERR_MHT_BAD_PARAM_TYPE = 1103,	  /* Bad parameter type */
    SPX_ERR_MHT_NO_ACTIVE_REGION = 1104,  /* No Active Region of given name
					   * was found.
					   */
    SPX_ERR_MHT_BUFFER_SIZE = 1105,	  /* The supplied buffer for a
					   * parameter read was too small
					   */
    SPX_ERR_MHT_NO_PARAM = 1106,	 /* No parameter of specified name
					  * for set/get function
					  */
    SPX_ERR_MHT_BAD_ASSOC_MATRIX = 1107, /* Could not calculate the
					  * association matrix
					  */
    SPX_ERR_MHT_BAD_DRC_FILE = 1108,	/* The specified DRC file could
					 * not be opened.
					 */
    SPX_ERR_MHT_EMPTY_DRC = 1109,	/* The DRC file was read but contained
					 * no segments (warning only).
					 */
    SPX_ERR_MHT_EXTRA_TRACK_IN_H = 1110, /* A hypothesis contains an unexpected
					  * occurrence of a track ID.
					  */
    SPX_ERR_MHT_PARAM_NOT_AREA_DEPEND = 1111, /* The parameter is not area-dependent. */

    SPX_ERR_MHT_VERIFY = 1112,		/* Verify check failed */
    SPX_ERR_MHT_REPAIR_TRACK_ID = 1113,	/* Attempt to repair a track specified
					 * a bad track ID.
					 */
    SPX_ERR_MHT_SWAP_TRACK_ID = 1114,	/* A bad public ID was specified for a track swap */
    SPX_ERR_MHT_CONFIG_BAD_FILE = 1115,	/* Could not open MHT's track class file. */

    /* MBT errors follow */
    SPX_ERR_MBT_MODEL_TYPE = 1150,	/* Unknown model type requested. */
    SPX_ERR_MBT_NO_MODEL_INSTANCE = 1151,   /* Could not create a new model of
					     * the requested type - no more
					     * instances available.
					     */
    SPX_ERR_MBT_BAD_PERIOD_METHOD = 1152,   /* The method of estimating the
					     * period is recognised.
					     */
    SPX_ERR_MBT_MEM_ALLOC = 1153,	/* Memory could not be allocated */
    SPX_ERR_MBT_NO_NEW_TRACK = 1154,	/* Could not allocate a new track from
					 * the free store.
					 */
    SPX_ERR_MBT_NO_NEW_PLOT = 1155,	/* Could not allocate a new plot from
					 * the free store.
					 */
    SPX_ERR_MBT_BAD_MHT_PARAM = 1156,	/* The assignment of an MHT parameter
					 * for a model specified a bad MHT
					 * parameter.
					 */
    SPX_ERR_MBT_BAD_ACTIVE_AREA = 1157,	    /* A requested active area could
						* not be found.
						*/

    SPX_ERR_MBT_PARAMETER_TYPE = 1158,	/* The type of an MBT parameter
					 * is not recognised.
					 */
    SPX_ERR_MBT_BAD_MODEL_CLASS = 1159,	/* The class of an MBT model
					 * is not recognised.
					 */

    SPX_ERR_MBT_MODEL_MISSING_NAME = 1160,	/* The specialisation model
						 * does not have a model name
						 * defined.
						 */

    SPX_ERR_MBT_MODEL_MISSING_MODEL = 1161,	/* The specialisation model
						 * does not have a model type
						 * defined.
						 */
    SPX_ERR_MBT_MODEL_WRITE_ERR = 1162,	/* The specialisation model template
					 * file could not be opened for writing.
					 */
    SPX_ERR_MBT_MODEL_READ_ERR = 1163,	/* The specialisation model file could
					 * not be opened for reading.
					 */

    SPX_ERR_MBT_MODEL_BASED_ON_SELF = 1164,	/* A specialisation model
						 * cannot be based on itself.
						 */

    SPX_ERR_MBT_NO_CONFIG_PATH = 1165,	/* The config path for the MBT models
					 * could not be found.
					 */

    SPX_ERR_MBT_CONFIG_BAD_END_CLASS = 1166, /* Bad ENDCLASS in config file */

    SPX_ERR_MBT_CONFIG_DEF_CLASS = 1167,	/* Read a CLASS definition when
						 * already inside a class
						 * definition (missing
						 * ENDCLASS).
						 */

    SPX_ERR_MBT_CONFIG_NO_CODE = 1168,		/* Code not set inside a track
						 * CLASS definition.
						 */
    SPX_ERR_MBT_CONFIG_MISSING_END_CLASS = 1169, /* Missing ENDCLASS in
						  * config file.
						  */
    SPX_ERR_MBT_CONFIG_UNKNOWN = 1170,	/* There was an unrecognised line 
					 * in the track class file.
					 */
    SPX_ERR_MBT_BAD_MODEL_INSTANCE = 1171,	/* Bad instance name for new class */
    SPX_ERR_MBT_BAD_MODEL = 1172,		/* Unreconginised model name */
    SPX_ERR_MBT_BAD_PARAMETER_FOR_MODEL = 1173,	/* Unrecognised parameter name for model */

// Hardware sources
    SPX_ERR_HW_NO_DRIVER = 1201,	/* No device driver found */
    SPX_ERR_HW_WRONG_DRIVER = 1202,	/* Wrong device driver found */
    SPX_ERR_HW_DRIVER_FAILED = 1203,	/* Device driver operation failed */
    SPX_ERR_HW_ATEXIT = 1204,		/* Failed to install tidy-up fn. */
    SPX_ERR_HW_NOT_SUPPORTED = 1205,	/* Feature not supported by card */
    SPX_ERR_HW_ACCESS_DENIED = 1206,	/* Driver access denied */
    SPX_ERR_HW_MEM_OVERFLOW = 1207,	/* Memory overflow - not reading fast
					 * enough.
					 */
    SPX_ERR_HW_UPDATE_DRIVER = 1208,	/* Device driver update required. */
    SPX_ERR_HW_BAD_STREAM = 1209,	/* Invalid stream ID specified. */

    SPX_ERR_HW_EEPROM_ERROR = 1210,	/* Problem with EEPROM */
    SPX_ERR_HW_SANITY_CHECK = 1211,	/* Basic assumption wrong */
    SPX_ERR_HW_BIST_REG = 1212,		/* Register test failed */
    SPX_ERR_HW_BIST_MEM = 1213,		/* Memory test failed */
    SPX_ERR_HW_BIST_LUT = 1214,		/* LUT test failed */
    SPX_ERR_HW_BIST_FLASH = 1215,	/* Flash test failed */
    SPX_ERR_HW_BIST_POWER = 1216,	/* Power test failed */
    SPX_ERR_HW_BIST_TEMP = 1217,	/* Temperature warning exceeded */

    SPX_ERR_HW_NOT_FOUND = 1220,	/* Card not found */
    SPX_ERR_HW_ALREADY_OPEN = 1221,	/* A card has already been opened */
    SPX_ERR_HW_FAILED_OPEN = 1222,	/* Failed to open card */
    SPX_ERR_HW_NOT_OPEN = 1223,		/* Card is not open but should be */
    SPX_ERR_HW_FAILED_CLOSE = 1224,	/* Failed to close card */
    SPX_ERR_HW_UNKNOWN = 1225,		/* Unknown card type */
    SPX_ERR_HW_BUSY = 1226,		/* Requested card is already open */
    SPX_ERR_HW_FAILED_RESET = 1227,	/* Failed to reset card */
    SPX_ERR_HW_ACTIVE = 1228,		/* Card is already digitising */

    SPX_ERR_HW_BAR_FAILURE = 1230,	/* Failed to get PCI BAR info */
    SPX_ERR_HW_BAR_TYPE = 1231,		/* PCI BAR I/O rather than MEM */
    SPX_ERR_HW_BAR_PREFETCH = 1232,	/* PCI BAR marked as prefetchable */
    SPX_ERR_HW_BAR_SIZE = 1233,		/* PCI BAR size wrong */
    SPX_ERR_HW_BAR_MMAP = 1234,		/* PCI BAR mapping failed */
    SPX_ERR_HW_BAR_UNMAP = 1235,	/* PCI BAR un-mapping failed */

    SPX_ERR_HW_REG_READ = 1240,		/* Register read failed */
    SPX_ERR_HW_REG_WRITE = 1241,	/* Register write failed */
    SPX_ERR_HW_MEM_READ = 1242,		/* Memory read failed */
    SPX_ERR_HW_MEM_WRITE = 1243,	/* Memory write failed */
    SPX_ERR_HW_LUT_READ = 1244,		/* LUT read failed */
    SPX_ERR_HW_LUT_WRITE = 1245,	/* LUT write failed */
    SPX_ERR_HW_FLASH_READ = 1246,	/* FLASH read failed */
    SPX_ERR_HW_FLASH_WRITE = 1247,	/* FLASH write failed */
    SPX_ERR_HW_LBE_ACCESS = 1248,	/* Local Bus access error */

    SPX_ERR_HW_DMA_OPEN = 1250,		/* DMA channel open failed */
    SPX_ERR_HW_DMA_CLOSE = 1251,	/* DMA channel close failed */
    SPX_ERR_HW_DMA_FAILED = 1252,	/* DMA operation failed */

    SPX_ERR_HW_DAC_CHANNEL = 1255,	/* DAC channel invalid */
    SPX_ERR_HW_DAC_LEVEL = 1256,	/* DAC voltage unachievable */
    SPX_ERR_HW_DAC_NOT_READY = 1257,	/* DAC not ready for next write */

    SPX_ERR_HW_INT_INSTALL = 1260,	/* Interrupt setup failed */
    SPX_ERR_HW_INT_REMOVE = 1261,	/* Interrupt shutdown failed */
    SPX_ERR_HW_INT_ENABLE = 1262,	/* Interrupt enable failed */
    SPX_ERR_HW_INT_WAIT = 1263,		/* Interrupt wait failed */
    SPX_ERR_HW_INT_KICKSTART = 1264,	/* Interrupts kick started */
    SPX_ERR_HW_INT_COUNT = 1265,	/* Interrupt count mismatch */

    SPX_ERR_HW_DECODE_BANK_CHANGED = 1270, /* Bank changed during reading */
    SPX_ERR_HW_DECODE_SHORT_HDR = 1271,	/* Not enough bytes for header */
    SPX_ERR_HW_DECODE_BAD_MAGIC = 1272,	/* Wrong magic value */
    SPX_ERR_HW_DECODE_BAD_BANK = 1273,	/* Wrong bank value */
    SPX_ERR_HW_DECODE_BAD_NUMSAMPLES = 1274,	/* Invalid numsamples */
    SPX_ERR_HW_DECODE_BAD_AZIMUTH = 1275,	/* Invalid azimuth */
    SPX_ERR_HW_DECODE_SHORT_DATA = 1276, /* Not enough bytes for data */

    SPX_ERR_HW_BAD_CHANNEL = 1280,	/* Invalid Video Channel */
    SPX_ERR_HW_BAD_TPG = 1281,		/* Invalid TPG mode */
    SPX_ERR_HW_BAD_ACP = 1282,		/* Invalid ACP/AZI mode */
    SPX_ERR_HW_BAD_ARP = 1283,		/* Invalid ARP mode */
    SPX_ERR_HW_BAD_TRG = 1284,		/* Invalid TRG mode */
    SPX_ERR_HW_BAD_TPG_TRG_FREQ = 1285,	/* Invalid Test Pattern PRF */
    SPX_ERR_HW_BAD_TPG_ACP_FREQ = 1286,	/* Invalid Test Pattern ACP freq */
    SPX_ERR_HW_BAD_TPG_ARP_FREQ = 1287,	/* Invalid Test Pattern ARP freq */
    SPX_ERR_HW_BAD_SIG = 1288,		/* Invalid SIG1 or 2 option */

    SPX_ERR_HW_SYNC_ERROR = 1290,	/* HPx synchronisation error */
    SPX_ERR_HW_TIMING_MISMATCH = 1291,	/* HPx timing mismatch */
    SPX_ERR_HW_AUTO_RECOVER = 1292,	/* HPx auto-recovery triggered */

    SPX_ERR_HW_FIFO_UNDERFLOW = 1295,	/* HPx FIFO underflow has occurred */
    SPX_ERR_HW_FIFO_OVERFLOW = 1296,	/* HPx FIFO overflow has occurred */

    SPX_ERR_HW_FASTSTARTUP_ENABLED = 1297, /* HPx driver incompatible with Windows Fast Start-Up */

// Tracker. Deprecated range - use the 1100 range instead.
    SPX_ERR_MHT_BAD_ASSOC_MATRIX_OLD = 1500, /* Deprecated, see 1107 */

// A-Scan Display.
    SPX_ERR_ASCAN_DESTINATION = 1600,	/* Destination for A-Scan is empty */
    SPX_ERR_ASCAN_FONT = 1601,		/* Could not load A-Scan font */
    SPX_ERR_ASCAN_HIGHLIGHT = 1602,	/* Cannot set highlight when viewing
					 * samples
					 */
    SPX_ERR_ASCAN_HIGHLIGHT_WIDTH = 1603, /* Highlight width out of range */
    SPX_ERR_ASCAN_NO_CREATE = 1604,	/* A-Scan window not been Created */
    SPX_ERR_ASCAN_BAD_SLAVE = 1605,	/* Bad A-Scan highlight slave */
    SPX_ERR_ASCAN_NO_UPDATE_INTERVAL = 1606,	/* Don't call SetUpdateInterval
						 * for X11
						 */

    SPX_ERR_ASCAN_MAX_DIM_ZERO = 1607,	/* Maximum size for A-Scan window
					 * unexpectedly zero
					 */
    SPX_ERR_ASCAN_MAX_DIM_HIGH = 1608,	/* Maximum size for A-Scan window is
					 * too large.
					 */
    SPX_ERR_ASCAN_PARENT = 1609,	/* A relative size specification
					 * implies the need for a parent
					 * window, which was not provided.
					*/
    SPX_ERR_ASCAN_BAD_WINDOW = 1610,	/* X Window invalid for A-Scan */

// Screen Recorder
    SPX_ERR_SR_FILENAME = 1700,		/* Missing filename */
    SPX_ERR_SR_DISPLAY = 1701,		/* Invalid Display */
    SPX_ERR_SR_VISUAL = 1702,		/* Could not create the visual */
    SPX_ERR_SR_XIMAGE_CREATE = 1703,	/* Could not create the X Image */
    SPX_ERR_SR_SM_CREATE = 1704,	/* Could not create shared memory */
    SPX_ERR_SR_SM_ATTACH = 1705,	/* Could not attach shared memory */
    SPX_ERR_SR_FILE_OPEN = 1706,	/* Could not open the output file */
    SPX_ERR_SR_SCREEN_GRAB_FAILED = 1707, /* Screen grab failed */
    SPX_ERR_SR_BAD_SCREEN_INDEX = 1708,	/* Bad screen index specified */
    SPX_ERR_SR_BAD_MAPPING_VALUE = 1709, /* Bad screen mapping value */
    SPX_ERR_SR_BAD_MAPPING_SCREEN = 1710, /* Bad screen mapping destination */
    SPX_ERR_SR_NO_SCREEN = 1711,	/* No requested screen available. */
    SPX_ERR_SR_DISK_WRITE = 1712,	/* Failed write to the disk (full?) */
    SPX_ERR_SR_DISK_READ = 1713,	/* Failed read from disk (file end?) */
    SPX_ERR_SR_OVERLAY_FONT = 1714,	/* Font for overlay */
    SPX_ERR_SR_BAD_MASTER_MAGIC = 1715,	/* Bad magic number if master block */
    SPX_ERR_SR_FILE_FORMAT = 1716,	/* Field format in screen
					 * block - magic number missing
					 */
    SPX_ERR_SR_END_FILE = 1717,		/* End of replay file reached. */
    SPX_ERR_SR_BUFFER_SZ = 1718,	/* An internal buffer is too small
					 * for the coded data.
					 */
    SPX_ERR_SR_LARGE_FILE = 1719,	/* Cannot fully support large files */

// Navigation data errors
    SPX_ERR_NAV_ERROR = 1800,		/* Generic nav data error */
    SPX_ERR_NAV_NOT_SUPPORTED = 1801,	/* Moving platforms not supported */
    SPX_ERR_NAV_DATA_EXPIRED = 1802,	/* Requested data is too old */
    SPX_ERR_NAV_SERIAL_OPEN = 1810,	/* Failed to open serial port */
    SPX_ERR_NAV_SERIAL_CFG_GET = 1811,	/* Failed to get serial port config */
    SPX_ERR_NAV_SERIAL_CFG_RATE = 1812,	/* Bad serial port baud rate */
    SPX_ERR_NAV_SERIAL_CFG_SET = 1813,	/* Failed to get serial port config */
    SPX_ERR_NAV_SERIAL_CFG_READ = 1814,	/* Failed to read from serial port */
    SPX_ERR_NAV_NMEA_LENGTH = 1820,	/* Bad NMEA message length */
    SPX_ERR_NAV_NMEA_CHECKSUM = 1821,	/* Wrong NMEA checksum */

// Plugin errors
    SPX_ERR_PLUGIN_ERROR = 1900,        /* Generic plugin error */
    SPX_ERR_PLUGIN_VERSION = 1901,      /* Library version mismatch */
    SPX_ERR_PLUGIN_FILE_OPEN = 1902,    /* Failed to open plugin library */
    SPX_ERR_PLUGIN_FILE_CLOSE = 1903,   /* Failed to close plugin library */
    SPX_ERR_PLUGIN_LOAD_SYMBOL = 1904,  /* Failed to load plugin symbol */
    SPX_ERR_PLUGIN_NOT_LOADED = 1905,   /* Failed as no plugin loaded */
    SPX_ERR_PLUGIN_BAD_OBJECT = 1906,   /* Bad plugin object handle */

// Cuda errors
    SPX_ERR_CUDA_ERROR = 2000,          /* Generic cuda error */
    SPX_ERR_CUDA_NO_DEVICE = 2001,      /* No suitable NVIDIA device found */
    SPX_ERR_CUDA_MALLOC = 2002,         /* Cuda malloc failed */
    SPX_ERR_CUDA_FREE = 2003,           /* Cuda free failed */
    SPX_ERR_CUDA_MEMCPY = 2004,         /* Cuda memcpy failed */

// Button Box
    SPX_ERR_BB_GRID_COUNT = 2100,	/* Bad number of buttons across/down
					 * grid.
					 */
    SPX_ERR_BB_RADIO_RANGE = 2101,	/* Bad range for radio buttons. */
    SPX_ERR_BB_RADIO_INTERSECT = 2102,	/* Intersecting range for radio
					 * buttons.
					 */

// Fusion errors
    SPX_ERR_FUSION_ERROR = 2200,	/* Generic fusion error */
    SPX_ERR_FUSION_LL_UTM = 2201,	/* Lat/long UTM conversion error */
    SPX_ERR_FUSION_SENSOR_STOP = 2202,	/* Sensor input has stopped */
    SPX_ERR_FUSION_SLAVE_EXIT = 2203,	/* Fusion slave mode has exited */
    SPX_ERR_FUSION_SLAVE_ENTER = 2204,	/* Fusion slave mode has entered */
    SPX_ERR_FUSION_SENSOR_RESTART = 2205, /* Sensor input has restarted */
    SPX_ERR_FUSION_ATTACH = 2210,	/* Engine/sensor attachment problem */
    SPX_ERR_FUSION_TRK_CREATE = 2220,	/* Track creation error */
    SPX_ERR_FUSION_TRK_UPDATE = 2221,	/* Track update error */
    SPX_ERR_FUSION_OBS_NOT_SET = 2230,	/* Field not set for observation */
    SPX_ERR_FUSION_OBS_OLD = 2231,	/* Observation is too old */
    SPX_ERR_FUSION_SENSOR_TIMES = 2232,	/* Bad fusion sensor timestamps */
    SPX_ERR_FUSION_NAV_DATA_NOT_SET = 2233, /* Nav data source not set. */
    SPX_ERR_FUSION_OBS_UNUSABLE = 2234,	/* Observation cannot be used. */

// GLX mixer errors
    SPX_ERR_GLX_MIXER_ERROR = 2300,     /* Generic GLX mixer error */

// OpenGL errors
    SPX_ERR_GL_GET = 2350,		/* OpenGL Get error */
    SPX_ERR_GL_FBO = 2351,		/* OpenGL FBO error */
    SPX_ERR_GL_PBO = 2352,		/* OpenGL PBO error */
    SPX_ERR_GL_OUT_OF_MEMORY = 2353,	/* Out of video card memory */
    SPX_ERR_GL_ERROR = 2354,		/* An OpenGL error has occurred */
    SPX_ERR_GL_BAD_WINDOW = 2355,       /* Invalid OpenGL window. */
    SPX_ERR_GL_VERSION = 2356,          /* Unsupported OpenGL version. */
    SPX_ERR_GL_EXTENSION = 2357,        /* Unsupported OpenGL extension. */
    SPX_ERR_GL_BAD_CONTEXT = 2358,      /* Invalid OpenGL context. */

// GLX errors	
    SPX_ERR_GLX_SUPPORT = 2375,		/* Failed to find full GLX support */
    SPX_ERR_GLX_FUNC_NOT_FOUND = 2376,	/* Failed to find GLX function */
    SPX_ERR_GLX_CONTEXT_CREATE = 2377,	/* GLX context creation error */
    SPX_ERR_GLX_CONTEXT_BIND = 2378,	/* GLX context bind error */
    SPX_ERR_GLX_FRAMEBUFFER = 2379,	/* GLX Framebuffer error */
    SPX_ERR_GLX_BIND_TEX_SLOW = 2380,   /* GLX texture_from_pixmap extension slow */
    SPX_ERR_GLX_DIFF_THREAD = 2381,	/* GLX called from incorrect thread */
    SPX_ERR_GLX_NOT_CREATED = 2382,	/* GLX class has not been created */

// World map errors
    SPX_ERR_WORLDMAP_ERROR = 2400,	/* Generic WorldMap error */
    SPX_ERR_WORLDMAP_RESOLUTION = 2401,	/* Unsupported world map resolution */
    SPX_ERR_WORLDMAP_FILE_LOAD = 2402,	/* Error loading WorldMap from file */

// AV errors
    SPX_ERR_AV_ERROR = 2500,		/* Generic Video Server/Library error */
    SPX_ERR_AV_NOT_READY = 2501,	/* AV not ready for action */
    SPX_ERR_AV_NO_DATA = 2502,		/* No data for buffer */
    SPX_ERR_AV_OVERLOAD = 2503,		/* Processing overloaded */
    SPX_ERR_AV_INVALID_FORMAT = 2504,	/* Invalid AV format */
    SPX_ERR_AV_PROC_NOT_FOUND = 2505,	/* AV process not found */
    SPX_ERR_AV_INVALID_SRC = 2506,	/* Invalid source provided */
    SPX_ERR_AV_INVALID_ENC = 2507,	/* Invalid encoder provided */
    SPX_ERR_AV_ENCODE = 2508,		/* Encoding error */
    SPX_ERR_AV_DECODE = 2509,		/* Decoding error */
    SPX_ERR_AV_CYCLIC_DATA = 2510,	/* Data is child of itself */
    SPX_ERR_AV_BAD_AV_INFO = 2511,	/* Bad video info message */
    SPX_ERR_AV_FONT_CREATE = 2512,	/* Font creation failed */
    SPX_ERR_AV_RENDER_TO_SCREEN = 2513, /* Render to screen failed */
    SPX_ERR_AV_NO_MORE_ITEMS = 2514,	/* No more items available */
    SPX_ERR_AV_WRONG_FRAME_RATE = 2515, /* Expected and actual frame rate differ */
    SPX_ERR_AV_PROC_CHAIN_ACTIVE = 2516,/* The process chain is active */
    SPX_ERR_AV_DEVICE_LOST = 2517,	/* AV device lost */
    SPX_ERR_AV_RTSP_ERROR = 2518,	/* RTSP error */
    SPX_ERR_AV_WIN_SIZE = 2519,		/* Window has zero size */
    SPX_ERR_AV_RTP_PORT_OVERRIDE = 2520,/* RTP port overridden */
    SPX_ERR_AV_SYS_TIMER_RES = 2521,    /* Windows system timer resolution */

// AV Direct 3D/2D errors
    SPX_ERR_AV_D3D_ERROR = 2600,	/* Generic Direct3D error */
    SPX_ERR_AV_D3D_SUPPORT = 2601,	/* Direct3D support not found */
    SPX_ERR_AV_D2D_ERROR = 2602,	/* Generic Direct2D error */

// AV OpenGL errors
    SPX_ERR_AV_GL_ERROR = 2605,		/* Generic OpenGL error */

// AV Direct Show errors
    SPX_ERR_AV_DSHOW_ERROR = 2610,	/* Generic DirectShow error */
    SPX_ERR_AV_DSHOW_NO_CAPTURE_DEVICES = 2611, /* No DirectShow capture devices available */
    SPX_ERR_AV_DSHOW_DEVICE_IN_USE = 2612,  /* DirectShow already in use */

// AV Advantech DVP errors
    SPX_ERR_AV_DVP_ERROR = 2615,	/* Generic Advantech DVP error */

// AV Media Foundation errors
    SPX_ERR_AV_MF_ERROR = 2620,		/* Media Foundation error */

// AV OpenVideo errors
    SPX_ERR_AV_OPENVIDEO_ERROR = 2625,	/* OpenVideo error */

// AV OpenH264 errors
    SPX_ERR_AV_OPEN_H264_ERROR = 2626,	/* OpenH264 error */

// AV GstH264 errors
    SPX_ERR_AV_GST_H264_ENC_ERROR = 2627,	/* GstH264 encode error */
    SPX_ERR_AV_GST_H264_DEC_ERROR = 2628,	/* GstH264 decode error */

// AV CUDA errors
    SPX_ERR_AV_CUDA_ERROR = 2630,	/* CUDA error */

// AV GigE Vision errors
    SPX_ERR_AV_GIGE_VISION = 2635,	/* GigE Vision error */

// AV Matrox errors
    SPX_ERR_AV_MATROX_ERROR = 2640,	/* Matrox error */
    SPX_ERR_AV_MATROX_NO_FREE_DIG = 2641,/* No free digitizer on Matrox board */
    SPX_ERR_AV_MATROX_CHANNEL_IN_USE = 2642, /* The requested channel is already in use */

// AV Direct Sound errors
    SPX_ERR_AV_DSOUND_ERROR = 2650,	/* DirectSound error */
    SPX_ERR_AV_DSOUND_NO_DEVICE = 2651, /* No DirectSound device */

// AV Nvidia errors
    SPX_ERR_AV_NVIDIA_ERROR = 2660,	/* NVIDIA error */

// AV VDPAU errors
    SPX_ERR_AV_VDPAU_ERROR = 2665,	/* VDPAU error */

// AV OpenCL errors
    SPX_ERR_AV_CL_ERROR = 2670,		/* OpenCL error */

// AV OpenCV errors
    SPX_ERR_AV_OPENCV_ERROR = 2675,	/* OpenCV error */

// AV Video for Linux errors
    SPX_ERR_AV_V4L_ERROR = 2680,	/* Video for Linux error */

// AV RSVG errors
    SPX_ERR_AV_RSVG_ERROR = 2690,	/* RSVG error */

// AV misc errors
    SPX_ERR_AV_BLACKBODY = 2695,	/* Blackbody reference error */


// AIS errors
    SPX_ERR_AIS_ERROR = 2700,		/* Generic AIS error */
    SPX_ERR_AIS_NOT_AVAIL = 2701,	/* AIS field not available */
    SPX_ERR_AIS_NOT_VALID = 2702,	/* AIS field not valid */
    SPX_ERR_AIS_EXPIRED = 2703,		/* AIS field expired */
    SPX_ERR_AIS_NO_IMO = 2704,		/* AIS IMO number not known */
    SPX_ERR_AIS_NO_IMAGE = 2705,	/* AIS image not found */
    SPX_ERR_AIS_NO_KEY = 2706,		/* AIS image key not valid */

// ADS-B errors
    SPX_ERR_ADSB_ERROR = 2750,		/* Generic ADS-B error */
    SPX_ERR_ADSB_NOT_AVAIL = 2751,	/* ADS-B field not available */
    SPX_ERR_ADSB_NOT_VALID = 2752,	/* ADS-B field not valid */
    SPX_ERR_ADSB_EXPIRED = 2753,	/* ADS-B field expired */

// Track errors
    SPX_ERR_TRACK_ERROR = 2800,         /* Generic track error */
    SPX_ERR_TRACK_NOT_REFERENCED = 2801, /* Track was not referenced */
    SPX_ERR_TRACK_INCORRECT_TYPE = 2802, /* Track is not the correct type */

// Serial port errors
    SPX_ERR_SERIAL_ERROR = 2900,	/* Generic serial port error */
    SPX_ERR_SERIAL_OPEN = 2901,         /* Failed to open serial port */
    SPX_ERR_SERIAL_CFG_GET = 2902,	/* Failed to get serial port config */
    SPX_ERR_SERIAL_CFG_RATE = 2903,	/* Bad serial port baud rate */
    SPX_ERR_SERIAL_CFG_SET = 2904,	/* Failed to get serial port config */
    SPX_ERR_SERIAL_CFG_READ = 2905,	/* Failed to read from serial port */
    SPX_ERR_SERIAL_CFG_WRITE = 2906,	/* Failed to write to serial port */
    SPX_ERR_SERIAL_CLOSE = 2907,	/* Failed to close serial port */
	
// ASTERIX errors
    SPX_ERR_ASTERIX_NO_DATA = 3000,      /* ASTERIX data not present */
    SPX_ERR_ASTERIX_DATA_EXPIRED = 3001, /* ASTERIX data has expired */
	
// ENC errors
    SPX_ERR_ENC = 3100,			/* ENC error */
    SPX_ERR_ENC_OPEN_LIBRARY = 3101,	/* Failed to open chart library */
    SPX_ERR_ENC_CLOSE_LIBRARY = 3102,	/* Failed to close chart library */
    SPX_ERR_ENC_RENDER = 3103,		/* Failed to render chart */
    SPX_ERR_ENC_BAD_SLAVE = 3104,	/* Incompatible slave renderer */
    SPX_ERR_ENC_BAD_GROUP_ID = 3105,    /* Invalid group ID. */

// ECDIS errors
    SPX_ERR_ECDIS = 3200,		/* ECDIS error */
    SPX_ERR_ECDIS_NO_SYS_CONFIG = 3201, /* ECDIS missing system config file */
    SPX_ERR_ECDIS_BAD_SYS_CONFIG = 3202,/* ECDIS corrupt system config file */
    SPX_ERR_ECDIS_COPROCESS_CONNECTION = 3203,/* ECDIS coprocess connection error */
    SPX_ERR_ECDIS_COPROCESS_EXIT = 3204,/* ECDIS coprocess has exited */

// Tiled map errors
    SPX_ERR_TILED_MAP = 3300,               /* Tiled map error */
    SPX_ERR_TILED_MAP_NO_TILE = 3301,       /* Failed to retrieve tile */
    SPX_ERR_TILED_MAP_FILE_READ = 3302,     /* Failed to read file */
    SPX_ERR_TILED_MAP_FILE_WRITE = 3303,    /* Failed to write file */
    SPX_ERR_TILED_MAP_DOWNLOAD = 3304,      /* Failed to download tile */
    SPX_ERR_TILED_MAP_DOWNLOAD_LIMIT = 3305,/* Download limit exceeded */
    SPX_ERR_TILED_MAP_AREA_CACHE = 3306,    /* Area caching error */
    SPX_ERR_TILED_MAP_AREA_NOT_SET = 3307,  /* Cache area not set */
    SPX_ERR_TILED_MAP_CACHE_DELETE = 3308,  /* Cache deletion error */
    SPX_ERR_TILED_MAP_OUT_OF_BOUNDS = 3309, /* Tiled map out of bounds */
    SPX_ERR_TILED_MAP_TILE_EXPIRED = 3310,  /* Tiled map file too old */
    SPX_ERR_TILED_MAP_BAD_FILENAME = 3311,  /* Bad tile filename. */
    SPX_ERR_TILED_MAP_BAD_URL = 3312,       /* Bad tile URL. */
    SPX_ERR_TILED_MAP_KEY_NOT_SET = 3313,   /* Tiled map key not set. */
    SPX_ERR_TILED_MAP_ACCESS_DENIED = 3314, /* Tiled map server access denied. */
    SPX_ERR_TILED_MAP_BAD_MESG = 3315,      /* Tiled map server message invalid. */

// Graphics widget library errors
    SPX_ERR_GW = 3400,                      /* Graphics widget library error */
    SPX_ERR_GW_BAD_XDISPLAY = 3401,         /* Bad X display. */
    SPX_ERR_GW_BAD_XDRAWABLE = 3402,        /* Bad X drawable. */
    SPX_ERR_GW_BAD_REF_POINT = 3403,        /* Bad reference point. */
    SPX_ERR_GW_BAD_RENDERER = 3404,         /* Renderer error. */
    SPX_ERR_GW_LOAD_IMAGE = 3405,           /* Failed to load image. */
    SPX_ERR_GW_BAD_COLOUR = 3406,           /* Bad colour name. */
    SPX_ERR_GW_WIDGET_ASSIGN = 3407, /* Failed to add widget to group. */
    SPX_ERR_GW_WIDGET_REMOVE = 3408, /* Failed to remove widget from group. */
    SPX_ERR_GW_GROUP_ASSIGN = 3409,  /* Failed to add group to picture. */
    SPX_ERR_GW_GROUP_REMOVE = 3410,  /* Failed to remove group from picture. */
    SPX_ERR_GW_NO_PICTURE = 3411,    /* No picture. */

// Video simulator errors
    SPX_ERR_VID_SIM = 3500,                  /* Video simulator error */
    SPX_ERR_VID_SIM_OPEN_GL = 3511,          /* OpenGL error */
    SPX_ERR_VID_SIM_BAD_CAM_REF = 3512,      /* Invalid camera reference */
    SPX_ERR_VID_SIM_LOAD_MODEL_FILE = 3513,  /* Failed to load model file */
    SPX_ERR_VID_SIM_READ_MODEL_FILE = 3514,  /* Failed to read model file */
    SPX_ERR_VID_SIM_LOAD_TEXTURE_FILE = 3515,/* Failed to load texture file */
    SPX_ERR_VID_SIM_SHADER = 3516,           /* General shader error */
    SPX_ERR_VID_SIM_SHADER_CODE = 3517,      /* Shader code error */
    SPX_ERR_VID_SIM_SHADER_PROG = 3518,      /* Shader program error */
    SPX_ERR_VID_SIM_CHILD_ADD = 3521,        /* Failed to add child object */
    SPX_ERR_VID_SIM_CHILD_REMOVE = 3522,     /* Failed to remove child object */

// Joystick interface errors
    SPX_ERR_JOYSTICK = 3600,                 /* Joystick interface error */
    SPX_ERR_JOYSTICK_NO_DEVICE = 3601,       /* Joystick not connected */

// VPX errors
    SPX_ERR_VPX = 3610,			    /* VPX error */
    SPX_ERR_VPX_NO_MORE_DATA = 3611,	    /* No more VPX data available */

// Radar Simulator errors
    SPX_ERR_RAD_SIM = 3620,			    /* Radar simulator error */
    SPX_ERR_RAD_SIM_TARGET_NAME_MISMATCH = 3621,    /* Target name mismatch */
    SPX_ERR_RAD_SIM_TARGET_NOT_FOUND = 3622,	    /* Target not found */
    SPX_ERR_RAD_SIM_TARGET_NOT_UNIQUE = 3623,	    /* Target is not unique */
    SPX_ERR_RAD_SIM_MOTION_NAME_MISMATCH = 3624,    /* Motion name mismatch */
    SPX_ERR_RAD_SIM_TGT_TEMPLATE_NAME_MISMATCH = 3625, /* Target template name mismatch */
    SPX_ERR_RAD_SIM_OVERLOAD = 3626,		/* Radar simulator overload */

// Terrain database errors
    SPX_ERR_TERRAIN_NO_INTERSECT = 3700,    /* No intersection */
    SPX_ERR_TERRAIN_DOWNLOAD = 3701,	    /* Failed to download */
    SPX_ERR_TERRAIN_AREA_CACHE = 3702,	    /* Area caching error */
    SPX_ERR_TERRAIN_CACHE_DELETE = 3703,    /* Cache deletion error */

// Timeline errors
    SPX_ERR_TIMELINE_NO_IMAGE = 3800,       /* No thumbnail image */

// View control errors
    SPX_ERR_VIEW_CTRL_FAILED = 3900,       /* Operation failed */
    SPX_ERR_VIEW_CTRL_VIEW_NOT_SET = 3901, /* View was not set */

// Web interface errors
    SPX_ERR_WEB_IF = 4000,                 /* Web interface error */
    SPX_ERR_WEB_IF_CMD_NOT_HANDLED = 4001, /* Command message not handled */
    SPX_ERR_WEB_IF_HTTPS_NOT_SUPPORTED = 4002, /* HTTPS not supported */
    SPX_ERR_WEB_IF_INVALID_UTF8 = 4003,	   /* Invalid UTF-8 string */
    SPX_ERR_WEB_IF_BIND = 4004,		   /* Failed to bind to web server port */

// SQL errors
    SPX_ERR_SQL = 4100,                    /* SQL error */

// RadarWatch errors
    SPX_ERR_RW = 4200,                     /* RadarWatch error */

// CANBus errors
    SPX_ERR_CANBUS_BAD_ADAPTER_ID = 4301,     /* Bad CANBus adapter ID */
    SPX_ERR_CANBUS_UNEXPECTED_MESSAGE = 4302, /* Unexpected CANBus message */
    SPX_ERR_CANBUS_ERROR_MESSAGE = 4303,      /* CANBus error message received */
    SPX_ERR_CANBUS_QUEUE_SIZE = 4304,	      /* Write buffer grown large */

// Environment variable related errors
    SPX_ERR_ENV_VAR_NOT_SET = 4400,		/* Required environment variable has not been set */

// ONVIF errors
    SPX_ERR_ONVIF_NO_SERVER = 4501,		/* ONVIF server not found */
    SPX_ERR_ONVIF_NO_PROFILE = 4502,		/* ONVIF profile not found */

// Monitoring and redundancy errors
    SPX_ERR_MON_GENERAL = 4600,		/* Monitoring error. */
    SPX_ERR_MON_BAD_HEALTH_STATUS = 4601,	/* Bad output health status. */
    SPX_ERR_MON_BAD_SOURCE_STATUS = 4602,	/* Bad output source status. */
    SPX_ERR_MON_BAD_PRF = 4603,			/* Bad output radar PRF. */
    SPX_ERR_MON_BAD_PERIOD = 4604,		/* Bad output radar period. */
    SPX_ERR_MON_NO_AVAILABLE_OUTPUTS = 4605,	/* No available outputs. */
    SPX_ERR_MON_NET_SEND_FAILURE = 4606,	/* Failed to send network data. */
    SPX_ERR_MON_NET_IF_DISABLED = 4607,		/* Network interface is disabled. */
    SPX_ERR_MON_NET_IF_DISCONNECTED = 4608,	/* Network interface is disconnected. */
    SPX_ERR_MON_NET_PACKET_TIMEOUT = 4609,	/* Network packets not received. */
    SPX_ERR_MON_SITE_NOT_AVAILABLE = 4610,	/* Site not available. */
    SPX_ERR_MON_SITE_NOT_LOCAL = 4611,		/* Site not local. */
    SPX_ERR_MON_SYSTEM_NOT_AVAILABLE = 4620,	/* System not available. */
    SPX_ERR_MON_SYSTEM_NOT_LOCAL = 4621,	/* System not local. */
    SPX_ERR_MON_APP_NOT_AVAILABLE = 4630,	/* Application not available. */
    SPX_ERR_MON_APP_IMAGE_NAME_NOT_SET = 4631,  /* Application image name not set. */
    SPX_ERR_MON_APP_BAD_IMAGE_NAME = 4632,	/* Application image name is invalid. */
    SPX_ERR_MON_APP_CMD_LINE_NOT_SET = 4633,	/* Application command line not set. */
    SPX_ERR_MON_APP_NOT_RUNNING = 4634,		/* Application not running. */
    SPX_ERR_MON_APP_ALREADY_RUNNING = 4635,	/* Application already running. */
    SPX_ERR_MON_APP_IS_CURRENT_PROCESS = 4636,	/* Application is the current process. */
    SPX_ERR_MON_APP_IS_AUTO_START = 4637,	/* Application set to auto start. */
    SPX_ERR_MON_APP_IS_UNEXPECTED = 4638,	/* Application not set to expected. */
    SPX_ERR_MON_STREAM_NOT_AVAILABLE = 4640,	/* Stream not available. */
    SPX_ERR_MON_STREAM_ASTERIX_NOGO = 4641,	/* ASTERIX stream has NOGO set. */
    SPX_ERR_MON_ITEM_NOT_FOUND = 4650,		/* Item not found. */


// JSON parsing errors
    SPX_ERR_JSON_PARSE_ERROR = 4700,		/* Failed to parse JSON text. */
    SPX_ERR_JSON_ROOT_OBJECT_NOT_FOUND = 4701,	/* JSON root object not found. */
    SPX_ERR_JSON_OBJECT_NOT_FOUND = 4702,	/* JSON expected object not found. */
    SPX_ERR_JSON_NOT_AN_OBJECT = 4703,		/* JSON element is not an object. */
    SPX_ERR_JSON_ARRAY_NOT_FOUND = 4704,	/* JSON expected array not found. */
    SPX_ERR_JSON_NOT_AN_ARRAY = 4705,		/* JSON element is not an array. */
    SPX_ERR_JSON_FIELD_NOT_FOUND = 4706,	/* JSON expected field not found. */
    SPX_ERR_JSON_NOT_A_FIELD = 4707,		/* JSON element is not a field. */
    SPX_ERR_JSON_DECODE_FIELD = 4708,		/* JSON field decode error. */

// SNMP errors
    SPX_ERR_SNMP_MESG_TOO_SHORT = 4800,			/* SNMP message too short. */
    SPX_ERR_SNMP_MESG_UNKNOWN_BER_TAG = 4801,		/* SNMP message has unknown BER type. */
    SPX_ERR_SNMP_MESG_UNEXPECTED_BER_TAG = 4802,	/* SNMP message has unexpected BER tag. */
    SPX_ERR_SNMP_MESG_BAD_BER_LENGTH = 4803,		/* SNMP message bad BER length. */
    SPX_ERR_SNMP_MESG_BAD_BER_VALUE = 4804,		/* SNMP message bad BER value. */
    SPX_ERR_SNMP_MESG_BAD_BER_VALUE_LENGTH = 4805,	/* SNMP message bad BER value length. */
    SPX_ERR_SNMP_VERSION_NOT_SUPPORTED = 4806,		/* SNMP version not supported. */
    SPX_ERR_SNMP_REQUEST_NOT_SUPPORTED = 4807,		/* SNMP request not supported. */

// MIB errors
    SPX_ERR_MIB_BAD_OID = 4900,				/* Bad MIB object ID. */
    SPX_ERR_MIB_BAD_VALUE = 4901,			/* Bad MIB object value. */
    SPX_ERR_MIB_OBJECT_NOT_FOUND = 4902,		/* MIB object not found. */
    SPX_ERR_MIB_OBJECT_EXISTS = 4903,			/* MIB object already exists. */
    SPX_ERR_MIB_OBJECT_READ_ONLY = 4904,		/* MIB object is read-only. */

// TIFF errors
    SPX_ERR_TIFF = 4950,		/* TIFF error */
    SPX_ERR_TIFF_NOT_SUPPORTED = 4951,	/* TIFF file not supported */
    SPX_ERR_TIFF_CORRUPT = 4952,	/* TIFF file corrupt */

// Alarm errors
    SPX_ERR_ALARM = 5000,               /* General alarm error. */
    SPX_ERR_ALARM_BAD_ID = 5001,        /* Bad alarm object ID. */
    SPX_ERR_ALARM_BAD_INDEX = 5002,     /* Bad alarm object index. */
    SPX_ERR_ALARM_BAD_TRACK_ID = 5003,  /* Bad alarm track ID. */

// Cairo library errors
    SPX_ERR_AV_CAIRO_ERROR = 5050,	/* Generic Cairo error */

// Track filter errors
    SPX_ERR_TRACK_FILTER = 5100,                /* General track filter error. */
    SPX_ERR_TRACK_FILTER_BAD_ID = 5101,         /* Bad track filter object ID. */
    SPX_ERR_TRACK_FILTER_BAD_INDEX = 5102,      /* Bad track filter object index. */

// Radar aligner errors
    SPX_ERR_RADAR_ALIGNER_ERROR = 5200,			/* General radar align error. */
    SPX_ERR_RADAR_ALIGNER_INSUFFICIENT_POINTS = 5201,	/* Insufficient number of points. */

// SkyWall errors
    SPX_ERR_SKY_WALL_ERROR = 5250,	/* General SkyWall error. */

// Aeroscope errors
    SPX_ERR_AEROSCOPE = 5300,			/* General AeroScope error. */
    SPX_ERR_AEROSCOPE_ADDRESS_NOT_SET = 5301,	/* AeroScope address not set. */
    SPX_ERR_AEROSCOPE_ID_NOT_SET = 5302,	/* AeroScope ID not set. */
    SPX_ERR_AEROSCOPE_USER_NAME_NOT_SET = 5303,	/* AeroScope user name not set. */
    SPX_ERR_AEROSCOPE_KEY_NOT_SET = 5304,	/* AeroScope key not set. */
    SPX_ERR_AEROSCOPE_ID_NOT_FOUND = 5305,	/* AeroScope ID not found. */
    SPX_ERR_AEROSCOPE_REQUEST_FAILED = 5306,	/* AeroScope request failed. */
    SPX_ERR_AEROSCOPE_BAD_RESPONSE = 5307,	/* AeroScope bad response. */

// Project-specific errors
    SPX_ERR_P100 = 10100,		/* Project-100 specific error */
    SPX_ERR_P121 = 10121,		/* Project-121 specific error */
    SPX_ERR_P122 = 10122,		/* Project-122 specific error */
    SPX_ERR_P125 = 10125,		/* Project-125 specific error */
    SPX_ERR_P126 = 10126,		/* Project-126 specific error */
    SPX_ERR_P127 = 10127,		/* Project-127 specific error */
    SPX_ERR_P226 = 10226,		/* Project-226 specific error */
    SPX_ERR_P313 = 10313,		/* Project-313 specific error */
    SPX_ERR_P999 = 10999		/* Project-999 specific error */

#define SPX_ERR_PLUGIN_CUDA_FADE_BITMAP_FORMAT ((SPxErrorCode) 200000)

} SPxErrorCode;

/* Error handler function pointer - will be deprecated */
typedef void (*SPxErrorHandlerFn_t)(SPxErrorType errType,
                                    SPxErrorCode errorCode,
                                    int arg1, int arg2,
                                    char *arg3, char *arg4);

/* V2 error handler function pointer - will be deprecated */
typedef void (*SPxErrorHandlerFnV2_t)(SPxErrorType errType,
					 SPxErrorCode errorCode,
					 int arg1, int arg2,
					 const char *arg3, const char *arg4);

/* New error handler function pointer - use this handler in new code. */
typedef void (*SPxErrorHandlerFnV3_t)(SPxErrorType errType,
					 SPxErrorCode errorCode,
					 int arg1, int arg2,
					 const char *arg3, const char *arg4,
					 void *userArg);

/* Handler for recording error packets. */
typedef SPxErrorCode (*SPxErrorRecorderFn_t)(void *userArg,
					     unsigned short packetType,
					     const void *payload,
					     unsigned int numBytes,
					     unsigned char channelIndex);

/*********************************************************************
*
*   Variable definitions
*
**********************************************************************/

/* Global value with the last error number. */
extern int SPxLastError;

/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

/* Global functions for reporting/handling errors. */
extern void SPxError(SPxErrorType errType,
		     SPxErrorCode errorCode,
		     int arg1=0, int arg2=0,
		     const char *arg3=NULL, const char *arg4=NULL);
extern void SPxErrorWithRecord(SPxErrorType errType,
			       SPxErrorCode errorCode,
			       int arg1, int arg2,
			       const char *arg3, const char *arg4,
			       int recordError);

/* Set and get error handler. */
extern void SPxSetErrorHandler(SPxErrorHandlerFn_t errFn, int writeErrorsToFile=0);
extern void SPxSetErrorHandler(SPxErrorHandlerFnV2_t errFn, int writeErrorsToFile=0);
extern void SPxSetErrorHandler(SPxErrorHandlerFnV3_t errFn, void *userArg, int writeErrorsToFile=0);
extern SPxErrorHandlerFn_t SPxGetErrorHandler(void);
extern SPxErrorHandlerFnV2_t SPxGetErrorHandlerV2(void);
extern SPxErrorHandlerFnV3_t SPxGetErrorHandlerV3(void);
extern void *SPxGetErrorHandlerUserArg(void);

/* Set error handler overloads support for NULL without cast - to remove error handler. */
extern void SPxSetErrorHandler(int nullArg, int writeErrorsToFile=0);
extern void SPxSetErrorHandler(long nullArg, int writeErrorsToFile=0);

/* Default error handler. */
extern void SPxDefaultErrorHandler(SPxErrorType errType,
				   SPxErrorCode errorCode,
				   int arg1, int arg2,
				   const char *arg3, const char *arg4,
				   void *userArg);

/* Get the last error. */
extern SPxErrorCode SPxGetLastError(void);

/* Enable reporting errors to connected clients? */
extern void SPxSetReportErrorsToClients(int enable);

/* Utility function to convert SPxErrorCode to a string. */
extern const char *SPxGetErrorString(SPxErrorCode errorCode);
extern const char *SPxGetErrorStringDetail(SPxErrorCode errorCode);

/* Utility function to convert SPxErrorType to a string. */
extern const char *SPxGetErrorSeverityString(SPxErrorType errType);

/* Utility function to convert error type, code and args into a string. */
extern void SPxGetErrorDescription(char *buffer, int bufLen,
				   SPxErrorType errType,
				   SPxErrorCode errorCode,
				   int arg1, int arg2,
				   const char *arg3, const char *arg4);

/* Functions to add/remove recorder objects that will
 * be given all errrors to record - automatically called by the
 * SPxRecord/ProRecord as needed.
 */
extern SPxErrorCode SPxAddErrorRecorder(void *userArg, SPxErrorRecorderFn_t fn);
extern SPxErrorCode SPxRemoveErrorRecorder(void *userArg);

#endif /* _SPX_ERROR_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/

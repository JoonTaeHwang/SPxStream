/*********************************************************************
*
* (c) Copyright 2007 - 2022, Cambridge Pixel Ltd.
*
* Purpose:
*	Version definitions for SPx libraries.
*
* Revision Control:
*   19/10/22 v1.532  AGC	V1.91
*
* Previous Changes:
*   05/10/22 1.531  AGC	V1.90.5
*   07/09/22 1.530  AGC	V1.90.4
*   11/08/22 1.529  AGC	V1.90.3
*   02/08/22 1.528  AGC	V1.90.2
*   26/07/22 1.527  AGC	V1.90.1
*   11/07/22 1.526  AGC	V1.90
*   30/06/22 1.525  AGC	V1.89.10
*   23/06/22 1.524  REW	V1.89.9
*   17/06/22 1.523  AGC	V1.89.8
*   15/06/22 1.522  AGC	V1.89.7
*   09/06/22 1.521  AGC	V1.89.6
*   26/05/22 1.520  AGC	V1.89.5
*   18/05/22 1.519  AGC	V1.89.4
*   10/05/22 1.518  AGC	V1.89.3
*   22/04/22 1.517  AGC	V1.89.2
*   31/03/22 1.516  AGC	V1.89.1
*   22/03/22 1.515  AGC	V1.89
*   09/03/22 1.514  AGC	V1.88.10
*   02/03/22 1.513  AGC	V1.88.9
*   22/02/22 1.512  AGC	V1.88.8
*   04/02/22 1.511  AGC	V1.88.7
*   29/12/21 1.510  REW	Update copyright year to 2022.
*   22/12/21 1.509  AGC	V1.88.6
*   15/12/21 1.508  REW	V1.88.5
*   08/12/21 1.507  AGC	V1.88.4
*   08/12/21 1.506  AGC	Move header/library version check to SPxInit.h.
*   06/12/21 1.505  AGC	Check for header/library version mismatch.
*   23/11/21 1.504  AGC	V1.88.3
*   10/11/21 1.503  AGC	V1.88.2
*   28/10/21 1.502  AGC	V1.88.1
*   26/10/21 1.501  AGC	V1.88
*   23/09/21 1.500  REW	V1.87.3
*   07/09/21 1.499  REW	V1.87.2
*   04/08/21 1.498  AGC	V1.87.1
*   13/07/21 1.497  AGC	V1.87
*   01/07/21 1.496  AGC	V1.86.9
*   29/06/21 1.495  AGC	V1.86.8
*   22/06/21 1.494  AGC	V1.86.7
*   09/06/21 1.493  AGC	V1.86.6
*   26/05/21 1.492  AGC	V1.86.5
*   25/05/21 1.491  AGC	V1.86.4
*   17/05/21 1.490  AGC	V1.86.3
*   06/05/21 1.489  AGC	V1.86.2
*   22/04/21 1.488  AGC	V1.86.1
*   21/04/21 1.487  AGC	Rework SPX_VERSION_STRING_RAW for use in resources.
*   29/03/21 1.486  AGC	V1.86
*   09/03/21 1.485  AGC	V1.85.12
*   04/03/21 1.484  AGC	V1.85.11
*   24/02/21 1.483  AGC	V1.85.10
*   16/02/21 1.482  REW	V1.85.9
*   15/02/21 1.481  REW	V1.85.8
*   04/02/21 1.480  REW	V1.85.7
*   08/01/21 1.479  REW	V1.85.6
*   06/01/21 1.478  REW	V1.85.5
*   04/01/21 1.477  REW	Update copyright year to 2021.
*   17/12/20 1.476  REW	V1.85.4
*   11/12/20 1.475  REW	V1.85.3
*   26/11/20 1.474  REW	V1.85.2
*   24/11/20 1.473  REW	V1.85.1
*   09/11/20 1.472  REW	V1.85
*   28/10/20 1.471  REW	V1.84.15
*   26/10/20 1.470  REW	V1.84.14
*   20/10/20 1.469  REW	V1.84.13
*   07/10/20 1.468  REW	V1.84.12
*   29/09/20 1.467  REW	V1.84.11
*   22/09/20 1.466  REW	V1.84.10
*   10/09/20 1.465  REW	V1.84.9
*   19/08/20 1.464  REW	V1.84.8
*   12/08/20 1.463  AGC	V1.84.7
*   06/08/20 1.462  REW	V1.84.6
*   15/07/20 1.461  REW	V1.84.5
*   02/07/20 1.460  REW	V1.84.4
*   24/06/20 1.459  REW	V1.84.3
*   03/06/20 1.458  REW	V1.84.2
*   15/04/20 1.457  REW	V1.84.1
*   18/03/20 1.456  REW	V1.84
*   09/03/20 1.455  REW	V1.83.8
*   26/02/20 1.454  REW	V1.83.7
*   24/02/20 1.453  AGC	V1.83.6
*   14/02/20 1.452  REW	V1.83.5
*   12/02/20 1.451  REW	V1.83.4
*   05/02/20 1.450  REW	V1.83.3
*   24/01/20 1.449  REW	V1.83.2
*   09/01/20 1.448  REW	V1.83.1
*   02/01/20 1.447  REW	Update copyright year to 2020.
*   17/12/19 1.446  REW	V1.83
*   11/12/19 1.445  REW	V1.82.10
*   06/12/19 1.444  AGC	Add SPX_VERSION_STRING_RAW.
*   05/11/19 1.443  REW	V1.82.9
*   31/10/19 1.442  REW	V1.82.8
*   14/10/19 1.441  REW	V1.82.7
*   07/10/19 1.440  AGC	V1.82.6
*   06/09/19 1.439  REW	V1.82.5
*   30/08/19 1.438  REW	V1.82.4
*   09/08/19 1.437  BTB	V1.82.3
*   23/07/19 1.436  REW	V1.82.2
*   10/07/19 1.435  REW	V1.82.1
*   03/07/19 1.434  REW	V1.82
*   18/06/19 1.433  REW	V1.81.12
*   29/05/19 1.432  REW	V1.81.11
*   21/05/19 1.431  REW	V1.81.10
*   03/05/19 1.430  REW	V1.81.9
*   02/05/19 1.429  REW	V1.81.8
*   11/04/19 1.428  REW	V1.81.7
*   27/03/19 1.427  REW	V1.81.6
*   15/03/19 1.426  REW	V1.81.5
*   06/03/19 1.425  REW	V1.81.4
*   28/02/19 1.424  REW	V1.81.3
*   12/02/19 1.423  REW	V1.81.2
*   07/02/19 1.422  REW	V1.81.1
*   29/01/19 1.421  REW	V1.81
*   21/01/19 1.420  REW	V1.80.13
*   17/01/19 1.419  REW	V1.80.12
*   09/01/19 1.418  REW	V1.80.11 and 2019
*   20/12/18 1.417  REW	V1.80.10
*   18/12/18 1.416  AGC	V1.80.9
*   07/12/18 1.415  REW	V1.80.8
*   27/11/18 1.414  REW	V1.80.7
*   13/11/18 1.413  REW	V1.80.6
*   06/11/18 1.412  REW	V1.80.5
*   24/10/18 1.411  REW	V1.80.4
*   18/10/18 1.410  REW	V1.80.3
*   16/10/18 1.409  REW	V1.80.2
*   03/10/18 1.408  REW	V1.80.1
*   19/09/18 1.407  REW	V1.80
*   04/09/18 1.406  REW	V1.79.15
*   29/08/18 1.405  REW	V1.79.14
*   23/08/18 1.404  REW	V1.79.13
*   21/08/18 1.403  REW	V1.79.12
*   14/08/18 1.402  REW	V1.79.11
*   09/08/18 1.401  AGC	V1.79.10
*   01/08/18 1.400  AGC	V1.79.9
*   26/07/18 1.399  REW	V1.79.8
*   23/07/18 1.398  REW	V1.79.7
*   18/07/18 1.397  REW	V1.79.6
*   17/07/18 1.396  REW	V1.79.5
*   10/07/18 1.395  REW	V1.79.4
*   27/06/18 1.394  REW	V1.79.3
*   14/06/18 1.393  REW	V1.79.2
*   12/06/18 1.392  REW	V1.79.1
*   10/05/18 1.391  REW	V1.79
*   26/04/18 1.390  REW	V1.78.13
*   17/04/18 1.389  REW	V1.78.12
*   10/04/18 1.388  REW	V1.78.11
*   04/04/18 1.387  AGC	V1.78.10
*   29/03/18 1.386  AJH	V1.78.9
*   27/03/18 1.385  REW	V1.78.8
*   15/03/18 1.384  REW	V1.78.7
*   08/03/18 1.383  REW	V1.78.6
*   27/02/18 1.382  AGC	V1.78.5
*   23/02/18 1.381  REW	V1.78.4
*   31/01/18 1.380  REW	V1.78.3
*   23/01/18 1.379  REW	V1.78.2
*   16/01/18 1.378  REW	V1.78.1
*   05/01/18 1.377  REW	V1.78
*   02/01/18 1.376  REW	V1.77.11 and 2018.
*   12/12/17 1.375  REW	V1.77.9
*   06/12/17 1.374  REW	V1.77.8
*   30/11/17 1.373  REW	V1.77.7
*   29/11/17 1.372  REW	V1.77.6
*   22/11/17 1.371  REW	V1.77.5-devF, checked in for GIT.
*   08/11/17 1.370  REW	V1.77.5
*   02/11/17 1.369  REW	V1.77.4
*   16/10/17 1.368  REW	V1.77.3
*   12/10/17 1.367  REW	V1.77.2
*   11/10/17 1.366  REW	V1.77.1
*   28/09/17 1.365  REW	V1.77
*   06/09/17 1.364  REW	V1.76.7
*   01/08/17 1.363  REW	V1.76.6
*   01/08/17 1.362  REW	V1.76.5
*   26/07/17 1.361  REW	V1.76.4
*   19/07/17 1.360  REW	V1.76.3
*   30/06/17 1.359  REW	V1.76.2
*   21/06/17 1.358  REW	V1.76.1
*   13/06/17 1.357  REW	V1.76
*   01/06/17 1.356  REW	V1.75.6
*   31/05/17 1.355  REW	V1.75.5
*   18/05/17 1.354  REW	V1.75.4
*   28/04/17 1.353  SP 	V1.75.2
*   26/04/17 1.352  REW	V1.75.1
*   29/03/17 1.351  REW	V1.75
*   23/03/17 1.350  REW	V1.74.3
*   15/03/17 1.349  REW	V1.74.2
*   28/02/17 1.348  REW	V1.74.1
*   08/02/17 1.347  REW	V1.74
*   25/01/17 1.346  REW	V1.73.6
*   05/01/17 1.345  REW	V1.73.5 and 2017.
*   22/12/16 1.344  REW	V1.73.4
*   19/12/16 1.343  REW	V1.73.3
*   01/12/16 1.342  REW	V1.73.2
*   22/11/16 1.341  REW	V1.73.1
*   17/11/16 1.340  REW	V1.73
*   21/10/16 1.339  REW	V1.72.14
*   10/10/16 1.338  REW	V1.72.13
*   03/10/16 1.337  REW	V1.72.12
*   27/09/16 1.336  REW	V1.72.11
*   22/09/16 1.335  REW	V1.72.10
*   19/09/16 1.334  REW	V1.72.9
*   14/09/16 1.333  REW	V1.72.8
*   09/09/16 1.332  REW	V1.72.7
*   06/09/16 1.331  REW	V1.72.6
*   24/08/16 1.330  REW	V1.72.5
*   05/08/16 1.329  REW	V1.72.3
*   02/08/16 1.328  REW	V1.72.2
*   29/07/16 1.327  REW	V1.72.1
*   20/07/16 1.326  REW	V1.72
*   11/07/16 1.325  REW	V1.71.5
*   09/06/16 1.324  REW	V1.71.4
*   24/05/16 1.323  REW	V1.71.3
*   12/05/16 1.322  REW	V1.71.2
*   13/04/16 1.321  REW	V1.71.1
*   18/03/16 1.320  REW	V1.71
*   11/03/16 1.319  REW	V1.70.9
*   26/02/16 1.318  REW	V1.70.8
*   24/02/16 1.317  REW	V1.70.7
*   08/02/16 1.316  REW	V1.70.6
*   20/01/16 1.315  REW	V1.70.5
*   20/01/16 1.314  REW	V1.70.4
*   20/01/16 1.313  REW	V1.70.3
*   14/01/16 1.312  REW	V1.70.2
*   12/01/16 1.311  REW	V1.70.1 and 2016.
*   16/12/15 1.310  REW	V1.70
*   18/11/15 1.309  REW	V1.69.4
*   04/11/15 1.308  REW	V1.69.3
*   22/10/15 1.307  REW	V1.69.2
*   15/10/15 1.306  REW	V1.69.1
*   23/09/15 1.305  REW	V1.69
*   10/09/15 1.304  REW	V1.68.10
*   04/09/15 1.303  REW	V1.68.9
*   07/08/15 1.302  REW	V1.68.8
*   29/07/15 1.301  REW	V1.68.7
*   27/07/15 1.300  REW	V1.68.6
*   17/07/15 1.299  REW	V1.68.5
*   03/07/15 1.298  REW	V1.68.4
*   29/06/15 1.297  REW	V1.68.3
*   15/06/15 1.296  REW	V1.68.2
*   12/06/15 1.295  REW	V1.68.1
*   18/05/15 1.294  REW	V1.68
*   05/05/15 1.293  REW	V1.67.5
*   27/03/15 1.292  REW	V1.67.4
*   02/03/15 1.291  REW	V1.67.3
*   16/02/15 1.290  REW	V1.67.2
*   11/02/15 1.289  REW	V1.67.1
*   14/01/15 1.288  REW	V1.67 and 2015.
*   26/11/14 1.287  REW	V1.66
*   24/11/14 1.286  REW	V1.65.4
*   12/11/14 1.285  REW	V1.65.3
*   11/11/14 1.284  REW	V1.65.2
*   23/10/14 1.283  REW	V1.65.1
*   26/09/14 1.282  REW	V1.65
*   10/09/14 1.281  REW	V1.64.6
*   27/08/14 1.280  REW	V1.64.5
*   20/08/14 1.279  REW	V1.64.4
*   12/08/14 1.278  SP 	V1.64.3
*   29/07/14 1.277  REW	V1.64.2
*   23/07/14 1.276  REW	V1.64.1
*   08/07/14 1.275  REW	V1.64
*   09/06/14 1.274  REW	V1.63
*   21/05/14 1.273  REW	V1.62.5
*   16/05/14 1.272  REW	V1.62.4
*   08/05/14 1.271  REW	V1.62.3
*   25/04/14 1.270  REW	V1.62.2
*   23/04/14 1.269  REW	V1.62.1
*   15/04/14 1.268  REW	V1.62
*   03/04/14 1.267  REW	V1.61.4
*   17/03/14 1.266  REW	V1.61.3
*   13/03/14 1.265  REW	V1.61.2
*   10/03/14 1.264  REW	V1.61.1
*   29/01/14 1.263  REW	V1.61 and 2014.
*   17/12/13 1.262  REW	V1.60.2
*   16/12/13 1.261  REW	V1.60.1
*   09/12/13 1.260  REW	V1.60
*   12/11/13 1.259  REW	V1.59.7
*   11/11/13 1.258  REW	V1.59.6
*   06/11/13 1.257  REW	V1.59.5
*   22/10/13 1.256  REW	V1.59.4
*   11/10/13 1.255  REW	V1.59.3
*   08/10/13 1.254  REW	V1.59.2
*   17/09/13 1.253  REW	V1.59.1
*   09/09/13 1.252  REW	V1.59
*   28/08/13 1.251  REW	V1.58.3
*   26/07/13 1.250  REW	V1.58.2
*   19/07/13 1.249  REW	V1.58.1
*   26/06/13 1.248  REW	V1.58
*   19/06/13 1.247  REW	V1.57.2
*   21/05/13 1.246  REW	V1.57.1
*   09/05/13 1.245  REW	V1.57
*   10/04/13 1.244  REW	V1.56.3
*   03/04/13 1.243  REW	V1.56.2
*   25/03/13 1.242  SP 	V1.56.1
*   20/03/13 1.241  REW	V1.56
*   22/02/13 1.240  REW	V1.55.1
*   13/02/13 1.239  REW	V1.55
*   29/01/13 1.238  REW	V1.54.1
*   15/01/13 1.237  REW	V1.54
*   03/01/13 1.236  REW	V1.53.5 and 2013.
*   19/12/12 1.235  REW	V1.53.4
*   13/12/12 1.234  REW	V1.53.3
*   07/12/12 1.233  REW	V1.53.2
*   03/12/12 1.232  REW	V1.53.1
*   29/11/12 1.231  REW	V1.53
*   15/11/12 1.230  REW	V1.52.1
*   03/10/12 1.229  REW	V1.52
*   26/09/12 1.228  REW	V1.51.2
*   25/09/12 1.227  REW	V1.51.1
*   12/09/12 1.226  REW	V1.51
*   24/08/12 1.225  REW	V1.50.4
*   08/08/12 1.224  SP 	V1.50.3
*   03/08/12 1.223  REW	V1.50.2
*   31/07/12 1.222  REW	V1.50.1
*   19/07/12 1.221  REW	V1.50
*   20/06/12 1.220  REW	V1.49.1
*   13/06/12 1.219  REW	V1.49
*   06/06/12 1.218  REW	V1.48.3
*   30/05/12 1.217  REW	V1.48.2
*   15/05/12 1.216  REW	V1.48.1
*   09/05/12 1.215  REW	V1.48
*   20/04/12 1.214  REW	V1.47.3
*   05/04/12 1.213  REW	V1.47.2
*   12/03/12 1.212  REW	V1.47.1
*   08/03/12 1.211  REW	V1.47
*   21/02/12 1.210  REW	V1.46.5
*   13/02/12 1.209  REW	V1.46.4
*   10/02/12 1.208  REW	V1.46.3
*   09/02/12 1.207  REW	V1.46.2
*   06/02/12 1.206  REW	V1.46.1
*   01/02/12 1.205  REW	V1.46
*   26/01/12 1.204  REW	V1.45.1
*   19/01/12 1.203  REW	V1.45
*   13/01/12 1.202  REW	V1.44.4, plus change COPYRIGHT to 2012.
*   09/12/11 1.201  REW	V1.44.3
*   02/12/11 1.200  REW	V1.44.2
*   25/11/11 1.199  REW	V1.44.1
*   18/11/11 1.198  REW	V1.44 and fix history back to 1.188.
*   07/11/11 1.197  REW	V1.43.5
*   21/10/11 1.196  REW	V1.43.4
*   11/10/11 1.195  REW	V1.43.3
*   10/10/11 1.194  REW	V1.43.2
*   05/10/11 1.193  REW	V1.43.1
*   28/09/11 1.192  REW	V1.43 (omitted the MINOR upgrade, so wrong total)
*   27/09/11 1.191  REW	V1.42.3
*   19/09/11 1.190  REW	V1.42.2
*   16/08/11 1.189  REW	V1.42.1
*   04/08/11 1.188  REW	V1.42
*   03/08/11 1.187  REW	V1.41.2
*   22/07/11 1.186  REW	V1.41.1
*   14/07/11 1.185  REW	V1.41
*   08/07/11 1.184  REW	V1.40.1
*   29/06/11 1.183  REW	V1.40
*   26/05/11 1.182  REW	V1.39
*   18/05/11 1.181  REW	V1.38.1
*   27/04/11 1.180  REW	V1.38
*   21/04/11 1.179  REW	V1.37.1
*   13/04/11 1.178  REW	V1.37
*   28/03/11 1.177  REW	V1.36
*   01/03/11 1.176  REW	V1.35.1
*   21/02/11 1.175  REW	V1.35
*   18/02/11 1.174  REW	V1.34.2
*   11/02/11 1.173  REW	V1.34.1
*   18/01/11 1.172  REW	V1.34
*   17/01/11 1.171  REW	V1.33.7
*   13/01/11 1.170  REW	V1.33.6
*   11/01/11 1.169  REW	V1.33.5
*   05/01/11 1.168  REW	V1.33.4 plus add SPX_RC_LEGALCOPYRIGHT.
*   30/11/10 1.167  REW	V1.33.2 branch made from V1.33 in other tree.
*   09/11/10 1.166  REW	V1.33.1
*   21/10/10 1.165  REW	V1.33
*   19/10/10 1.164  REW	V1.32.6
*   18/10/10 1.163  REW	V1.32.5
*   14/10/10 1.162  REW	V1.32.4
*   11/10/10 1.161  REW	V1.32.3
*   30/09/10 1.160  REW	V1.32.2
*   29/09/10 1.159  REW	V1.32.1
*   24/09/10 1.158  REW	V1.32
*   22/09/10 1.157  REW	V1.31.3
*   15/09/10 1.156  REW	V1.31.2
*   08/09/10 1.155  REW	V1.31.1
*   24/08/10 1.154  REW	V1.31
*   11/08/10 1.153  REW	V1.30
*   29/07/10 1.152  REW	V1.29
*   16/07/10 1.151  REW	V1.28.1-devB.
*   15/07/10 1.150  REW	V1.28.1 plus fix history back to 1.125.
*   08/07/10 1.149  REW	V1.28.1-devA
*   07/07/10 1.148  REW	V1.28
*   30/06/10 1.147  REW	V1.27.1-devC
*   16/06/10 1.146  REW	V1.27.1-devB
*   16/06/10 1.145  REW	V1.27.1
*   28/05/10 1.144  REW	V1.27.1-devA
*   27/05/10 1.143  REW	V1.27
*   25/05/10 1.142  REW	V1.26.1-devA
*   13/05/10 1.141  REW	V1.26.1
*   28/04/10 1.140  REW	V1.26
*   27/04/10 1.139  REW	V1.25.3-devA.
*   19/04/10 1.138  REW	V1.25.3
*   15/04/10 1.137  REW	V1.25.2
*   09/04/10 1.136  REW	V1.25.1
*   29/03/10 1.135  REW	V1.25
*   23/03/10 1.134  REW	V1.24.3-devA.
*   17/03/10 1.133  REW	V1.24.3
*   11/03/10 1.132  REW	Add SPX_VERSION_NANO for MSVC rc files.
*   11/03/10 1.131  REW	V1.24.2
*   04/03/10 1.130  REW	V1.24.1
*   26/02/10 1.129  REW	V1.24.1-devA
*   24/02/10 1.128  REW	V1.24
*   18/02/10 1.127  REW	V1.23.2
*   09/02/10 1.126  REW	V1.23.1
*   03/02/10 1.125  REW	V1.23
*   21/01/10 1.124  REW	V1.22.1-devB
*   21/12/09 1.123  REW	V1.22.1-devA
*   18/12/09 1.122  REW	V1.22
*   14/12/09 1.121  REW	V1.21.1-devA
*   10/12/09 1.120  REW	V1.21
*   18/11/09 1.119  REW	V1.20.1-devB
*   17/11/09 1.118  REW	V1.20.1
*   06/11/09 1.117  REW	V1.20.1-devA
*   05/11/09 1.116  REW	V1.20
*   09/10/09 1.115  REW	V1.19.0-devA
*   07/10/09 1.114  REW	V1.19
*   22/09/09 1.113  REW	V1.18.2-devA
*   21/09/09 1.112  REW	V1.18.2
*   16/09/09 1.111  REW	V1.18.1-devB
*   15/09/09 1.110  REW	V1.18.1
*   04/09/09 1.109  REW	V1.18.1-devA
*   28/08/09 1.108  REW	V1.18
*   13/08/09 1.107  REW	V1.17.8-devA
*   12/08/09 1.106  REW	Release V1.17.8
*   24/07/09 1.105  REW	Development V1.17.8
*   24/07/09 1.104  REW	Release V1.17.7
*   22/07/09 1.103  REW	Development V1.17.7
*   22/07/09 1.102  REW	Release V1.17.6
*   03/07/09 1.101  REW	Development V1.17.6
*   03/07/09 1.100  REW	Release V1.17.5
*   29/06/09 1.99   REW	Development V1.17.5
*   26/06/09 1.98   REW	Release V1.17.4
*   16/06/09 1.97   REW	Development V1.17.4
*   15/06/09 1.96   REW	Release V1.17.3
*   11/06/09 1.95   REW	Development V1.17.3
*   11/06/09 1.94   REW	Release V1.17.2
*   05/06/09 1.93   REW	Development V1.17.2
*   05/06/09 1.92   REW	Release V1.17.1
*   01/06/09 1.91   REW	Development V1.17.1
*   29/05/09 1.90   REW	Release V1.17
*   27/05/09 1.89   REW	Release V1.16.4
*   13/05/09 1.88   REW	Development V1.16.4
*   13/05/09 1.87   REW	Release V1.16.3
*   22/04/09 1.86   REW	Move functions from SPx.h to here.
*   22/04/09 1.85   REW	Development V1.16.3
*   21/04/09 1.84   REW	Release V1.16.2
*   08/04/09 1.83   REW	Development V1.16.2
*   08/04/09 1.82   REW	Release V1.16.1
*   27/03/09 1.81   REW	Development V1.16.1
*   27/03/09 1.80   REW	Release V1.16
*   04/03/09 1.79   REW	Development V1.15.5
*   02/03/09 1.78   REW	Release V1.15.4
*   23/02/09 1.77   REW	Development V1.15.4
*   20/02/09 1.76   REW	Release V1.15.3
*   10/02/09 1.75   REW	Development V1.15.3
*   10/02/09 1.74   REW	Release V1.15.2
*   09/02/09 1.73   REW	Development V1.15.2
*   06/02/09 1.72   REW	Release V1.15.1
*   26/01/09 1.71   REW	Development V1.15.1
*   23/01/09 1.70   REW	Release V1.15
*   22/01/09 1.69   REW	Development V1.14.4
*   21/01/09 1.68   REW	Release V1.14.3
*   19/01/09 1.67   REW	Development V1.14.3
*   16/01/09 1.66   REW	Release V1.14.2
*   23/12/08 1.65   REW	Development V1.14.2
*   08/12/08 1.64   REW	Release V1.14.1
*   04/12/08 1.63   REW	Development V1.14.1
*   01/12/08 1.62   REW	Release V1.14
*   30/10/08 1.61   REW	Development V1.13.1
*   30/10/08 1.60   REW	Release V1.13
*   15/10/08 1.59   REW	Development V1.12.1
*   13/10/08 1.58   REW	Release V1.12
*   29/09/08 1.57   REW	Development V1.11.1
*   29/09/08 1.56   REW	Release V1.11
*   08/09/08 1.55   REW	Development V1.10.1
*   05/09/08 1.54   REW	Release V1.10
*   11/08/08 1.53   REW	Development V1.9.1
*   08/08/08 1.52   REW	Release V1.9
*   29/07/08 1.51   REW	Development V1.8.4
*   29/07/08 1.50   REW	Release V1.8.3
*   21/07/08 1.49   REW	Development V1.8.3
*   18/07/08 1.48   REW	Release V1.8.2
*   14/07/08 1.47   REW	Development V1.8.2
*   11/07/08 1.46   REW	Release V1.8.1
*   05/06/08 1.45   REW	Development V1.8.1
*   04/06/08 1.44   REW	Release V1.8
*   22/05/08 1.43   REW	Development V1.7.3
*   20/05/08 1.42   REW	Release V1.7.2
*   16/05/08 1.41   REW	Development V1.7.2
*   29/04/08 1.40   REW	Release V1.7.1
*   03/04/08 1.39   REW	Development V1.7.1
*   01/04/08 1.38   REW	Release V1.7
*   06/03/08 1.37   REW	Development V1.6.1
*   03/03/08 1.36   REW	Release V1.6
*   20/02/08 1.35   REW	Release V1.5.2
*   11/02/08 1.34   REW	Development V1.5.2
*   11/02/08 1.33   REW	Release V1.5.1
*   04/02/08 1.32   REW	Development V1.5.1
*   04/02/08 1.31   REW	Release V1.5
*   25/01/08 1.30   REW	Development V1.4.1
*   23/01/08 1.29   REW	Release V1.4
*   14/12/07 1.28   REW	Development V1.3.1
*   13/12/07 1.27   REW	Release V1.3
*   06/12/07 1.26   REW	Development V1.2.2
*   06/12/07 1.25   REW	Release V1.2.1
*   22/11/07 1.24   REW	Development V1.2.1
*   21/11/07 1.23   REW	Release V1.2
*   19/11/07 1.22   REW	Development V1.1.2
*   12/11/07 1.21   REW	Release V1.1.1
*   31/10/07 1.20   REW	Development V1.1.1
*   30/10/07 1.19   REW	Release V1.1
*   16/10/07 1.18   REW	Development V1.0.2
*   16/10/07 1.17   REW	Release V1.0.1.
*   11/10/07 1.16   REW	Add closing bracket to SPX_VERSION.
*   28/09/07 1.15   REW	Development V1.0.1
*   27/09/07 1.14   REW	Release V1.0
*   25/09/07 1.13   REW	Development V0.9.5
*   25/09/07 1.12   REW	Release V0.9.4
*   31/08/07 1.11   REW	Development V0.9.4
*   31/08/07 1.10   REW	Release V0.9.3
*   21/08/07 1.9    REW	Development V0.9.3
*   20/08/07 1.8    REW	Release V0.9.2
*   16/08/07 1.7    REW	Development V0.9.2
*   16/08/07 1.6    REW	Release V0.9.1
*   15/08/07 1.5    REW	Development V0.9.1
*   08/08/07 1.4    REW	Release V0.9
*   16/07/07 1.3    REW	Development V0.8.1
*   10/07/07 1.2    REW	Release V0.8
*   09/07/07 1.1    REW	Initial Version.
**********************************************************************/

#ifndef _SPX_VERSION_H
#define _SPX_VERSION_H

/*********************************************************************
*
*   Constants
*
**********************************************************************/

/* Define the version as a 32-bit number with major/minor/micro components
 * in each byte.  The top byte is currently unused, and the micro byte should
 * be zero for releases.
 *
 * For example, Vx.y is 0x00xxyy00.
 */
/* Define the version as a string as well.  KEEP THIS CONSISTENT !!! */
#define	SPX_VERSION_STRING	"V1.91"

/* Define the sub-components. */
#define	SPX_VERSION_MAJOR	1		/* KEEP CONSISTENT */
#define	SPX_VERSION_MINOR	91		/* WITH STRING */
#define	SPX_VERSION_MICRO	0		/* ABOVE !!! */
#define	SPX_VERSION_NANO	0

/* Now combine them into one. */
#define	SPX_VERSION	((SPX_VERSION_MAJOR << 16) 		\
			| (SPX_VERSION_MINOR << 8)		\
			| (SPX_VERSION_MICRO))


/* Macros to generate version string in the form "1.28.1.0" suitable
 * for use by .NET assemblies.
 */
#define SPX_VERSION_STRINGISE2(str) #str
#define SPX_VERSION_STRINGISE(str) SPX_VERSION_STRINGISE2(str)
#define SPX_VERSION_STRING_RAW SPX_VERSION_STRINGISE(SPX_VERSION_MAJOR) "." \
			       SPX_VERSION_STRINGISE(SPX_VERSION_MINOR) "." \
			       SPX_VERSION_STRINGISE(SPX_VERSION_MICRO) "." \
			       SPX_VERSION_STRINGISE(SPX_VERSION_NANO)

/* Define a string for use in .rc files as the LegalCopyright value. */
#define	SPX_RC_LEGALCOPYRIGHT	\
	"(c) 2007 - 2022 Cambridge Pixel Ltd. All rights reserved."

/* Define the current year (which might be concatenated in copyright msgs). */
#define	SPX_CURRENT_YEAR_STRING	"2022"

/*********************************************************************
*
*   Function prototypes
*
**********************************************************************/

/* Need common types (e.g. UINT32) */
#include "SPxLibUtils/SPxCommon.h"

/* Run-time version/build retrieval. */
extern const char *SPxGetBuildString(void);
extern const char *SPxGetVersionString(void);
extern UINT32 SPxGetVersionNum(void);

#endif /* _SPX_VERSION_H */

/*********************************************************************
*
* End of file
*
**********************************************************************/

/***********************************************************************************************************************
 * Copyright [2020-2024] Renesas Electronics Corporation and/or its affiliates.  All Rights Reserved.
 *
 * This software and documentation are supplied by Renesas Electronics America Inc. and may only be used with products
 * of Renesas Electronics Corp. and its affiliates ("Renesas").  No other uses are authorized.  Renesas products are
 * sold pursuant to Renesas terms and conditions of sale.  Purchasers are solely responsible for the selection and use
 * of Renesas products and Renesas assumes no liability.  No license, express or implied, to any intellectual property
 * right is granted by Renesas. This software is protected under all applicable laws, including copyright laws. Renesas
 * reserves the right to change or discontinue this software and/or this documentation. THE SOFTWARE AND DOCUMENTATION
 * IS DELIVERED TO YOU "AS IS," AND RENESAS MAKES NO REPRESENTATIONS OR WARRANTIES, AND TO THE FULLEST EXTENT
 * PERMISSIBLE UNDER APPLICABLE LAW, DISCLAIMS ALL WARRANTIES, WHETHER EXPLICITLY OR IMPLICITLY, INCLUDING WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, AND NONINFRINGEMENT, WITH RESPECT TO THE SOFTWARE OR
 * DOCUMENTATION.  RENESAS SHALL HAVE NO LIABILITY ARISING OUT OF ANY SECURITY VULNERABILITY OR BREACH.  TO THE MAXIMUM
 * EXTENT PERMITTED BY LAW, IN NO EVENT WILL RENESAS BE LIABLE TO YOU IN CONNECTION WITH THE SOFTWARE OR DOCUMENTATION
 * (OR ANY PERSON OR ENTITY CLAIMING RIGHTS DERIVED FROM YOU) FOR ANY LOSS, DAMAGES, OR CLAIMS WHATSOEVER, INCLUDING,
 * WITHOUT LIMITATION, ANY DIRECT, CONSEQUENTIAL, SPECIAL, INDIRECT, PUNITIVE, OR INCIDENTAL DAMAGES; ANY LOST PROFITS,
 * OTHER ECONOMIC DAMAGE, PROPERTY DAMAGE, OR PERSONAL INJURY; AND EVEN IF RENESAS HAS BEEN ADVISED OF THE POSSIBILITY
 * OF SUCH LOSS, DAMAGES, CLAIMS OR COSTS.
 **********************************************************************************************************************/

#include "bsp_api.h"
#include "rm_wifi_reg_pwr_db.h"

/*
 * Regulatory and Tx Power Table
 */

/*****************************************************************************************
*** RRQ61X_BND_GRP_2G_OFDM ***************************************************************
*****************************************************************************************/
/* The unit of the value is dBm.
   e.g.) 0x4 (4 dBm), 0xe (14dBm), etc
         special value -> 0x1f (max power), 0x7f (unused channel)
*/

/* CH 1 ~ 11, all CH pwr lvl == 0x1f, default_1 */
/*                                                                            1    2      3     4     5     6     7     8     9    10    11 */
const struct pwr_lvl_2g_l pl_2gl_ofdm_def1 = {RRQ61X_BND_GRP_2G_OFDM, 11, {0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f}};
/* CH 1 ~ 11, all CH pwr lvl == 0x1f except CH 11 == 0xe, default_2 */
const struct pwr_lvl_2g_l pl_2gl_ofdm_def2 = {RRQ61X_BND_GRP_2G_OFDM, 11, {0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f}};
/* CH 1 ~ 11, all CH pwr lvl == 0x4 default_3 */
const struct pwr_lvl_2g_l pl_2gl_ofdm_def3 = {RRQ61X_BND_GRP_2G_OFDM, 11, {0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f}};
// custom
const struct pwr_lvl_2g_l pl_2gl_ofdm_cust = {RRQ61X_BND_GRP_2G_OFDM, 11, {0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f}};
// zz (for debug)
const struct pwr_lvl_2g_l pl_2gl_ofdm_zz  =  {RRQ61X_BND_GRP_2G_OFDM, 11, {0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f}};

/* CH 12 ~ 14, all CH pwr lvl == 0x1f, CH14 = disabled, default_1 */
/*                                                                          12    13    14   */
const struct pwr_lvl_2g_h pl_2gh_ofdm_def1 = {RRQ61X_BND_GRP_2G_OFDM, 2, {0x1f, 0x1f, 0x7f}};
/* CH 12 ~ 14, all CH pwr lvl == 0x4, CH14 = disabled, default_2 */
const struct pwr_lvl_2g_h pl_2gh_ofdm_def2 = {RRQ61X_BND_GRP_2G_OFDM, 2, {0x4,  0x4,  0x7f}};
/* CH 12 ~ 14, all CH pwr lvl == 0x1f, default_3 */
const struct pwr_lvl_2g_h pl_2gh_ofdm_def3 = {RRQ61X_BND_GRP_2G_OFDM, 2, {0x1f, 0x1f, 0x1f}};
// custom
const struct pwr_lvl_2g_h pl_2gh_ofdm_cust = {RRQ61X_BND_GRP_2G_OFDM, 2, {0x1f, 0x1f, 0x7f}};
// zz (for debug)
const struct pwr_lvl_2g_h pl_2gh_ofdm_zz   = {RRQ61X_BND_GRP_2G_OFDM, 2, {0x1f, 0x1f, 0x7f}};

/*****************************************************************************************
*** RRQ61X_BND_GRP_2G_DSSS ***************************************************************
*****************************************************************************************/
/* The unit of the value is dBm.
   e.g.) 0x4 (4 dBm), 0xe (14dBm), etc
         special value -> 0x1f (max power), 0x7f (unused channel)
*/

/* CH 1 ~ 11, all CH pwr lvl == 0x1f, default_1 */
/*                                                                            1     2     3     4     5     6     7     8     9    10    11 */
const struct pwr_lvl_2g_l pl_2gl_dsss_def1 = {RRQ61X_BND_GRP_2G_DSSS, 11, {0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f}};
/* CH 1 ~ 11, all CH pwr lvl == 0x1f except CH 11 == 0xe, default_2 */
const struct pwr_lvl_2g_l pl_2gl_dsss_def2 = {RRQ61X_BND_GRP_2G_DSSS, 11, {0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f}};
/* CH 1 ~ 11, all CH pwr lvl == 0x4 default_3 */
const struct pwr_lvl_2g_l pl_2gl_dsss_def3 = {RRQ61X_BND_GRP_2G_DSSS, 11, {0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f}};
// custom
const struct pwr_lvl_2g_l pl_2gl_dsss_cust = {RRQ61X_BND_GRP_2G_DSSS, 11, {0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f}};
// zz (for debug)
const struct pwr_lvl_2g_l pl_2gl_dsss_zz   = {RRQ61X_BND_GRP_2G_DSSS, 11, {0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f}};

/* CH 12 ~ 14, all CH pwr lvl == 0x1f, CH14 = disabled, default_1 */
/*                                                                          12    13    14   */
const struct pwr_lvl_2g_h pl_2gh_dsss_def1 = {RRQ61X_BND_GRP_2G_DSSS, 2, {0x1f, 0x1f, 0x7f}};
/* CH 12 ~ 14, all CH pwr lvl == 0x4, CH14 = disabled, default_2 */
const struct pwr_lvl_2g_h pl_2gh_dsss_def2 = {RRQ61X_BND_GRP_2G_DSSS, 2, {0x4,  0x4,  0x7f}};
/* CH 12 ~ 14, all CH pwr lvl == 0x1f, default_3 */
const struct pwr_lvl_2g_h pl_2gh_dsss_def3 = {RRQ61X_BND_GRP_2G_DSSS, 2, {0x1f, 0x1f, 0x1f}};
// custom
const struct pwr_lvl_2g_h pl_2gh_dsss_cust = {RRQ61X_BND_GRP_2G_OFDM, 2, {0x1f, 0x1f, 0x7f}};
// zz (for debug)
const struct pwr_lvl_2g_h pl_2gh_dsss_zz  = {RRQ61X_BND_GRP_2G_OFDM, 2, {0x1f, 0x1f, 0x7f}};

/*****************************************************************************************
*** RRQ61X_BND_GRP_5G_I *******************************************************************
*****************************************************************************************/
/* The unit of the value is dBm.
   e.g.) 0x4 (4 dBm), 0xe (14dBm), etc
         special value -> 0x1f (max power), 0x7f (unused channel)
*/

/* UNII-I CH 36, 40, 44, 48,  pwr lvl == 0x1f, flag=0, default_1 */
/*                                                                                         36    40    44    48  */
const struct pwr_lvl_5g_m pl_5g_i_def1 = {RRQ61X_BND_GRP_5G_I, 4, 0,                     {0x1f, 0x1f, 0x1f, 0x1f}};
// default_2
const struct pwr_lvl_5g_m pl_5g_i_def2 = {RRQ61X_BND_GRP_5G_I, 4, 0,                     {0x1f, 0x1f, 0x1f, 0x1f}};
// default_3, NO_IR
const struct pwr_lvl_5g_m pl_5g_i_def3 = {RRQ61X_BND_GRP_5G_I, 4, RRQ61X_REG_FLAG_NO_IR, {0x1f, 0x1f, 0x1f, 0x1f}};
// custom
const struct pwr_lvl_5g_m pl_5g_i_cust = {RRQ61X_BND_GRP_5G_I, 4, 0,                     {0x1f, 0x1f, 0x1f, 0x1f}};
// zz (for debug)
const struct pwr_lvl_5g_m pl_5g_i_zz   = {RRQ61X_BND_GRP_5G_I, 4, 0,                     {0x1f, 0x1f, 0x1f, 0x1f}};

/*****************************************************************************************
*** RRQ61X_BND_GRP_5G_II *****************************************************************
*****************************************************************************************/
/* The unit of the value is dBm.
   e.g.) 0x4 (4 dBm), 0xe (14dBm), etc
         special value -> 0x1f (max power), 0x7f (unused channel)
*/

/* UNII-II CH 52, 56, 60, 64, pwr lvl == 0x1f, flag=DFS, default_1 */
/*                                                                                            52    56    60    64  */
const struct pwr_lvl_5g_m pl_5g_ii_def1 = {RRQ61X_BND_GRP_5G_II, 4, RRQ61X_REG_FLAG_DFS,   {0x1f, 0x1f, 0x1f, 0x1f}};
// default_2, NO_IR
const struct pwr_lvl_5g_m pl_5g_ii_def2 = {RRQ61X_BND_GRP_5G_II, 4, RRQ61X_REG_FLAG_NO_IR, {0x1f, 0x1f, 0x1f, 0x1f}};
const struct pwr_lvl_5g_m pl_5g_ii_def3 = {RRQ61X_BND_GRP_5G_II, 4, 0,                     {0x1f, 0x1f, 0x1f, 0x1f}};
// custom
const struct pwr_lvl_5g_m pl_5g_ii_cust = {RRQ61X_BND_GRP_5G_II, 4, RRQ61X_REG_FLAG_DFS,   {0x1f, 0x1f, 0x1f, 0x1f}};
// zz (for debug)
const struct pwr_lvl_5g_m pl_5g_ii_zz   = {RRQ61X_BND_GRP_5G_II, 4, RRQ61X_REG_FLAG_DFS,   {0x1f, 0x1f, 0x1f, 0x1f}};

/*****************************************************************************************
*** RRQ61X_BND_GRP_5G_IIe ****************************************************************
*****************************************************************************************/
/* The unit of the value is dBm.
   e.g.) 0x4 (4 dBm), 0xe (14dBm), etc
         special value -> 0x1f (max power), 0x7f (unused channel)
*/

/* UNII-IIe CH 100, 104, 108, 112, 116, 120, 124, 128, 132, 136, 140, 144, pwr lvl == 0x1f, flag=DFS, default_1 */
/*                                                                                             100, 104,   108,  112,  116,  120,  124,  128,  132,  136,  140,  144  */
const struct pwr_lvl_5g_l pl_5g_iie_def1 = {RRQ61X_BND_GRP_5G_II_E, 12, RRQ61X_REG_FLAG_DFS, {0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f}};
// default_2, only 144 is disabled
const struct pwr_lvl_5g_l pl_5g_iie_def2 = {RRQ61X_BND_GRP_5G_II_E, 11, RRQ61X_REG_FLAG_DFS, {0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x7f}};
// default_3, only 100 ~ 116, 132 ~ 144 enabled
const struct pwr_lvl_5g_l pl_5g_iie_def3 = {RRQ61X_BND_GRP_5G_II_E,  9, RRQ61X_REG_FLAG_DFS, {0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f}};
// default_4, only 100 ~ 132 enabled
const struct pwr_lvl_5g_l pl_5g_iie_def4 = {RRQ61X_BND_GRP_5G_II_E,  9, RRQ61X_REG_FLAG_DFS, {0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x7f, 0x7f, 0x7f}};
// default_5, all CH enabled, no DFS
const struct pwr_lvl_5g_l pl_5g_iie_def5 = {RRQ61X_BND_GRP_5G_II_E, 12, 0,                   {0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f}};
// default_6, only 100 ~ 112 enabled
const struct pwr_lvl_5g_l pl_5g_iie_def6 = {RRQ61X_BND_GRP_5G_II_E,  4, RRQ61X_REG_FLAG_DFS, {0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x7f, 0x7f, 0x7f, 0x7f}};
// default_7, only 100 ~ 124 enabled
const struct pwr_lvl_5g_l pl_5g_iie_def7 = {RRQ61X_BND_GRP_5G_II_E,  7, RRQ61X_REG_FLAG_DFS, {0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f}};
// default_8, only 132 ~ 144 enabled, no DFS
const struct pwr_lvl_5g_l pl_5g_iie_def8 = {RRQ61X_BND_GRP_5G_II_E,  4, 0                 ,  {0x7f, 0x7f, 0x7f, 0x7f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f}};
// default_9, only 100 ~ 128 enabled
const struct pwr_lvl_5g_l pl_5g_iie_def9 = {RRQ61X_BND_GRP_5G_II_E,  8, RRQ61X_REG_FLAG_DFS, {0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x7f, 0x7f, 0x7f, 0x7f}};
// custom
const struct pwr_lvl_5g_l pl_5g_iie_cust = {RRQ61X_BND_GRP_5G_II_E, 12, RRQ61X_REG_FLAG_DFS, {0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f}};
// zz (for debug)
const struct pwr_lvl_5g_l pl_5g_iie_zz   = {RRQ61X_BND_GRP_5G_II_E, 12, RRQ61X_REG_FLAG_DFS, {0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f}};

/*****************************************************************************************
*** RRQ61X_BND_GRP_5G_III ****************************************************************
*****************************************************************************************/
/* The unit of the value is dBm.
   e.g.) 0x4 (4 dBm), 0xe (14dBm), etc
         special value -> 0x1f (max power), 0x7f (unused channel)
*/

/* UNII-III CH 149, 153, 157, 161, pwr lvl == 0x1f, flag=0, default_1 */
/*                                                                         149,  153,  157,  161 */
const struct pwr_lvl_5g_m pl_5g_iii_def1 = {RRQ61X_BND_GRP_5G_III, 4, 0, {0x1f, 0x1f, 0x1f, 0x1f}};
// default_2, only 149 ~ 153 enabled US
const struct pwr_lvl_5g_m pl_5g_iii_def2 = {RRQ61X_BND_GRP_5G_III, 2, 0, {0x1f, 0x1f, 0x1f, 0x1f}};
// default_2, only 149 ~ 153 enabled EU
const struct pwr_lvl_5g_m pl_5g_iii_def3 = {RRQ61X_BND_GRP_5G_III, 2, 0, {0x1f, 0x1f, 0x1f, 0x1f}};
// custom
const struct pwr_lvl_5g_m pl_5g_iii_cust = {RRQ61X_BND_GRP_5G_III, 4, 0, {0x1f, 0x1f, 0x1f, 0x1f}};
// zz (for debug)
const struct pwr_lvl_5g_m pl_5g_iii_zz   = {RRQ61X_BND_GRP_5G_III, 4, 0, {0x1f, 0x1f, 0x1f, 0x1f}};

/*****************************************************************************************
*** RRQ61X_BND_GRP_5G_ISM ****************************************************************
*****************************************************************************************/
/* The unit of the value is dBm.
   e.g.) 0x4 (4 dBm), 0xe (14dBm), etc
         special value -> 0x1f (max power), 0x7f (unused channel)
*/

/* 5G ISM CH 165, pwr lvl == 0x1f, flag=0, default_1 */
/*                                                                        165 */
const struct pwr_lvl_5g_s pl_5g_ism_def1 = {RRQ61X_BND_GRP_5G_ISM, 1, 0, {0x1f, 0x7f}};
//US
const struct pwr_lvl_5g_s pl_5g_ism_def2 = {RRQ61X_BND_GRP_5G_ISM, 1, 0, {0x1f, 0x7f}};
//EU
const struct pwr_lvl_5g_s pl_5g_ism_def3 = {RRQ61X_BND_GRP_5G_ISM, 1, 0, {0x1f, 0x7f}};
// custom
const struct pwr_lvl_5g_s pl_5g_ism_cust = {RRQ61X_BND_GRP_5G_ISM, 1, 0, {0x1f, 0x7f}};
// zz (for debug)
const struct pwr_lvl_5g_s pl_5g_ism_zz   = {RRQ61X_BND_GRP_5G_ISM, 1, 0, {0x1f, 0x7f}};

/*****************************************************************************************
*** RRQ61X_BND_GRP_5G_IV_1 ***************************************************************
*****************************************************************************************/
/* The unit of the value is dBm.
   e.g.) 0x4 (4 dBm), 0xe (14dBm), etc
         special value -> 0x1f (max power), 0x7f (unused channel)
*/

/* UNII-IV CH 169, pwr lvl == 0x1f, flag=0, default_1  */
/*                                                                          169 */
const struct pwr_lvl_5g_s pl_5g_iv1_def1 = {RRQ61X_BND_GRP_5G_IV_1, 1, 0, {0x1f, 0x7f}};
//US
const struct pwr_lvl_5g_s pl_5g_iv1_def2 = {RRQ61X_BND_GRP_5G_IV_1, 1, 0, {0x1f, 0x7f}};
//EU
const struct pwr_lvl_5g_s pl_5g_iv1_def3 = {RRQ61X_BND_GRP_5G_IV_1, 1, 0, {0x1f, 0x7f}};
// custom
const struct pwr_lvl_5g_s pl_5g_iv1_cust = {RRQ61X_BND_GRP_5G_IV_1, 1, 0, {0x1f, 0x7f}};
// zz (for debug)
const struct pwr_lvl_5g_s pl_5g_iv1_zz   = {RRQ61X_BND_GRP_5G_IV_1, 1, 0, {0x1f, 0x7f}};

/*****************************************************************************************
*** RRQ61X_BND_GRP_5G_IV_2 ***************************************************************
*****************************************************************************************/
/* The unit of the value is dBm.
   e.g.) 0x4 (4 dBm), 0xe (14dBm), etc
         special value -> 0x1f (max power), 0x7f (unused channel)
*/

/* UNII-IV CH 173, 177, pwr lvl == 0x1f, flag=0, default_1 */
/*                                                                                              173   177 */
const struct pwr_lvl_5g_s pl_5g_iv2_def1 = {RRQ61X_BND_GRP_5G_IV_2, 2,                    0,  {0x1f, 0x1f}};
// default_2, only CH 173 enabled
const struct pwr_lvl_5g_s pl_5g_iv2_def2 = {RRQ61X_BND_GRP_5G_IV_2, 1,                    0,  {0x1f, 0x7f}};
// default_3, NO_IR
const struct pwr_lvl_5g_s pl_5g_iv2_def3 = {RRQ61X_BND_GRP_5G_IV_2, 2, RRQ61X_REG_FLAG_NO_IR, {0x1f, 0x1f}};

// custom
const struct pwr_lvl_5g_s pl_5g_iv2_cust = {RRQ61X_BND_GRP_5G_IV_2, 2,                    0,  {0x1f, 0x1f}};
// zz (for debug)
const struct pwr_lvl_5g_s pl_5g_iv2_zz   = {RRQ61X_BND_GRP_5G_IV_2, 2,                    0,  {0x1f, 0x1f}};

/*****************************************************************************************
*** 2.4 GHz data set types ***************************************************************
*****************************************************************************************/
/* idx = enum reg_group_2g_set */
const struct cntry_reg_group_2g cc_regdb_2g_data_grp[REG_2G_GRP_SET_TOTAL_NUM] = {
    /* ch 1 ~ 11 (ofdm)   ch 1 ~ 11 (dsss)    ch 12 ~ 14 (ofdm)   ch 12 ~ 14 (dsss) */
    {/* FCC_2G_GRP_SET_1   */ {&pl_2gl_ofdm_def1, &pl_2gl_dsss_def1}, {0,                 0                 }}, // 1,1,0,0
    {/* FCC_2G_GRP_SET_2   */ {&pl_2gl_ofdm_def2, &pl_2gl_dsss_def2}, {0,                 0                 }}, // 2,2,0,0
    {/* FCC_2G_GRP_SET_3   */ {&pl_2gl_ofdm_def3, &pl_2gl_dsss_def3}, {&pl_2gh_ofdm_def2, &pl_2gh_dsss_def2 }}, // 3,3,2,2

    {/* ETSI_2G_GRP_SET_1  */ {&pl_2gl_ofdm_def3, &pl_2gl_dsss_def3}, {&pl_2gh_ofdm_def3, &pl_2gh_dsss_def3 }}, // 3,3,3,3

    {/* JP_2G_GRP_SET_1    */ {&pl_2gl_ofdm_def1, &pl_2gl_dsss_def1}, {&pl_2gh_ofdm_def3, &pl_2gh_dsss_def3 }}, // 1,1,3,3

    {/* OTH_2G_GRP_SET_1    */ {&pl_2gl_ofdm_def1, &pl_2gl_dsss_def1}, {&pl_2gh_ofdm_def1, &pl_2gh_dsss_def1 }}, // 1,1,1,1

    {/* WR_2G_GRP_SET_1    */ {&pl_2gl_ofdm_def1, &pl_2gl_dsss_def1}, {&pl_2gh_ofdm_def1, &pl_2gh_dsss_def1 }}, // 1,1,1,1

    {/* WR_2G_GRP_SET_ZZ   */ {&pl_2gl_ofdm_zz,   &pl_2gl_dsss_zz },  {&pl_2gh_ofdm_zz,   &pl_2gh_dsss_zz   }},
    {/* WR_2G_GRP_SET_CUST */ {&pl_2gl_ofdm_cust, &pl_2gl_dsss_cust}, {&pl_2gh_ofdm_cust, &pl_2gh_dsss_cust }}
};

/*****************************************************************************************
*** 5 GHz data set types *****************************************************************
*****************************************************************************************/
/* idx = enum reg_group_5g_set
   if you add a new one, add also to enum reg_group_5g_set */
const struct cntry_reg_group_5g cc_regdb_5g_data_grp[REG_5G_GRP_SET_TOTAL_NUM] = {
    /*                            unii-1         unii-2          unii-2e           unii-3          unii-ism          unii-4-1        unii-4-2 */
    {/* FCC_5G_GRP_SET_1  */  &pl_5g_i_def1, &pl_5g_ii_def1, &pl_5g_iie_def1, &pl_5g_iii_def1, &pl_5g_ism_def1,  &pl_5g_iv1_def1, &pl_5g_iv2_def1}, // 1,1,1,1,1,1,1
    {/* FCC_5G_GRP_SET_2  */  &pl_5g_i_def2, &pl_5g_ii_def2, &pl_5g_iie_def2, &pl_5g_iii_def2, &pl_5g_ism_def2,  &pl_5g_iv1_def2, &pl_5g_iv2_def2}, // 2,2,2,2,2,2,2
    {/* FCC_5G_GRP_SET_3  */  &pl_5g_i_def1, &pl_5g_ii_def1, &pl_5g_iie_def1, &pl_5g_iii_def1, &pl_5g_ism_def1,  0,               0              }, // 1,1,1,1,1,0,0
    {/* FCC_5G_GRP_SET_4  */  &pl_5g_i_def1, &pl_5g_ii_def1, 0,               &pl_5g_iii_def1, &pl_5g_ism_def1,  0,               0              }, // 1,1,0,1,1,0,0
    {/* FCC_5G_GRP_SET_5  */  &pl_5g_i_def2, &pl_5g_ii_def1, 0,               &pl_5g_iii_def1, &pl_5g_ism_def1,  0,               0              }, // 2,1,0,1,1,0,0
    {/* FCC_5G_GRP_SET_6  */  &pl_5g_i_def3, &pl_5g_ii_def2, 0,               &pl_5g_iii_def1, &pl_5g_ism_def1,  0,               0              }, // 3,2,0,1,1,0,0
    {/* FCC_5G_GRP_SET_7  */  &pl_5g_i_def2, &pl_5g_ii_def1, &pl_5g_iie_def1, &pl_5g_iii_def1, &pl_5g_ism_def1,  0,               0              }, // 2,1,1,1,1,0,0
    {/* FCC_5G_GRP_SET_8  */  &pl_5g_i_def1, &pl_5g_ii_def3, &pl_5g_iie_def5, &pl_5g_iii_def1, &pl_5g_ism_def1,  0,               0              }, // 1,3,5,1,1,0,0
    {/* FCC_5G_GRP_SET_9  */  &pl_5g_i_def1, &pl_5g_ii_def1, &pl_5g_iie_def1, &pl_5g_iii_def1, &pl_5g_ism_def1,  0,               &pl_5g_iv2_def3}, // 1,1,1,1,1,0,3
    {/* FCC_5G_GRP_SET_10 */  &pl_5g_i_def1, &pl_5g_ii_def1, &pl_5g_iie_def9, &pl_5g_iii_def1, &pl_5g_ism_def1,  0,               0              }, // 1,1,9,1,1,0,0

    /*                            unii-1         unii-2          unii-2e           unii-3          unii-ism          unii-4-1        unii-4-2 */
    {/* ETSI_5G_GRP_SET_1  */ &pl_5g_i_def1, &pl_5g_ii_def1, &pl_5g_iie_def1, &pl_5g_iii_def1, 0,                &pl_5g_iv1_def1, &pl_5g_iv2_def2}, // 1,1,1,1,0,1,2
    {/* ETSI_5G_GRP_SET_2  */ &pl_5g_i_def1, &pl_5g_ii_def1, &pl_5g_iie_def2, 0,               0,                0,               0              }, // 1,1,2,0,0,0,0
    {/* ETSI_5G_GRP_SET_3  */ &pl_5g_i_def3, &pl_5g_ii_def3, &pl_5g_iie_def3, &pl_5g_iii_def3, &pl_5g_ism_def3,  &pl_5g_iv1_def3, &pl_5g_iv2_def3}, // 3,3,3,3,3,3,3
    {/* ETSI_5G_GRP_SET_4  */ &pl_5g_i_def1, &pl_5g_ii_def1, 0,               0,               0,                0,               0              }, // 1,1,0,0,0,0,0
    {/* ETSI_5G_GRP_SET_5  */ &pl_5g_i_def1, &pl_5g_ii_def1, &pl_5g_iie_def3, &pl_5g_iii_def1, &pl_5g_ism_def1,	 0,               0              }, // 1,1,3,1,1,0,0
    {/* ETSI_5G_GRP_SET_6  */ &pl_5g_i_def1, &pl_5g_ii_def1, &pl_5g_iie_def2, &pl_5g_iii_def1, 0,                0,               0              }, // 1,1,2,1,0,0,0

    /*                            unii-1         unii-2          unii-2e           unii-3          unii-ism          unii-4-1        unii-4-2 */
    {/* JP_5G_GRP_SET_1    */ 0,             0,              0,               &pl_5g_iii_def1, &pl_5g_ism_def1,  0,               0              }, // 0,0,0,1,1,0,0
    {/* JP_5G_GRP_SET_2    */ 0,             &pl_5g_ii_def1, 0,               &pl_5g_iii_def1, &pl_5g_ism_def1,  0,               0              }, // 0,1,0,1,1,0,0
    {/* JP_5G_GRP_SET_3    */ &pl_5g_i_def1, &pl_5g_ii_def1, &pl_5g_iie_def4, 0,               0,                0,               0              }, // 1,1,4,0,0,0,0
    {/* JP_5G_GRP_SET_4    */ &pl_5g_i_def1, &pl_5g_ii_def3, 0,               &pl_5g_iii_def1, 0,                0,               0              }, // 1,3,0,1,0,0,0
    {/* JP_5G_GRP_SET_5    */ &pl_5g_i_def1, 0,              0,               &pl_5g_iii_def1, &pl_5g_ism_def1,  0,               0              }, // 1,0,0,1,1,0,0
    {/* JP_5G_GRP_SET_6    */ &pl_5g_i_def1, &pl_5g_ii_def1, &pl_5g_iie_def1, 0,               0,                0,               0              }, // 1,1,1,0,0,0,0
    {/* JP_5G_GRP_SET_7    */ &pl_5g_i_def1, 0             , &pl_5g_iie_def6, &pl_5g_iii_def2, 0,                0,               0              }, // 1,0,6,2,0,0,0
    {/* JP_5G_GRP_SET_8    */ &pl_5g_i_def1, &pl_5g_ii_def1, &pl_5g_iie_def7, &pl_5g_iii_def1, 0,                0,               0              }, // 1,1,7,1,0,0,0
    {/* JP_5G_GRP_SET_9    */ 0,             0,              0,               &pl_5g_iii_def1, &pl_5g_ism_def1,  &pl_5g_iv1_def1, &pl_5g_iv2_def2}, // 0,0,0,1,1,1,2

    /*                            unii-1         unii-2          unii-2e           unii-3          unii-ism          unii-4-1        unii-4-2 */
    {/* OTH_5G_GRP_SET_1   */ &pl_5g_i_def1, &pl_5g_ii_def1, &pl_5g_iie_def1, &pl_5g_iii_def1, &pl_5g_ism_def1,  &pl_5g_iv1_def1, &pl_5g_iv2_def1}, // 1,1,1,1,1,1,1
    {/* OTH_5G_GRP_SET_2   */ &pl_5g_i_def1, &pl_5g_ii_def3, &pl_5g_iie_def8, &pl_5g_iii_def1, &pl_5g_ism_def1,  &pl_5g_iv1_def1, 0              }, // 1,3,8,1,1,1,0
    {/* OTH_5G_GRP_SET_3   */ 0,             0,              0,               0,               0,                0,               0              }, // 0,0,0,0,0,0,0

    {/* WR_5G_GRP_SET_ZZ   */ &pl_5g_i_zz,  &pl_5g_ii_zz,  &pl_5g_iie_zz,  &pl_5g_iii_zz,  &pl_5g_ism_zz,   &pl_5g_iv1_zz,  &pl_5g_iv2_zz },
    {/* WR_5G_GRP_SET_CUST */ &pl_5g_i_cust, &pl_5g_ii_cust, &pl_5g_iie_cust, &pl_5g_iii_cust, &pl_5g_ism_cust,  &pl_5g_iv1_cust, &pl_5g_iv2_cust}
};

/*****************************************************************************************
*** index into 2.4GHz and 5 GHz data set types *******************************************
*****************************************************************************************/
/* idx = enum country_code */
const struct cntry_reg_group_table cc_regdb_data[CC_NUM] = {
    /* CNTRY_AD */	{ WR_2G_GRP_SET_1,	ETSI_5G_GRP_SET_3},
    /* CNTRY_AE */	{ WR_2G_GRP_SET_1,	 FCC_5G_GRP_SET_3},
    /* CNTRY_AF */	{ WR_2G_GRP_SET_1,	ETSI_5G_GRP_SET_2},
    /* CNTRY_AI */	{ WR_2G_GRP_SET_1,	ETSI_5G_GRP_SET_2},
    /* CNTRY_AL */	{ WR_2G_GRP_SET_1,	ETSI_5G_GRP_SET_3},
    /* CNTRY_AM */	{ WR_2G_GRP_SET_1,	ETSI_5G_GRP_SET_4},
    /* CNTRY_AN */	{ WR_2G_GRP_SET_1,	ETSI_5G_GRP_SET_2},
    /* CNTRY_AR */	{ WR_2G_GRP_SET_1,	 FCC_5G_GRP_SET_3},
    /* CNTRY_AS */	{FCC_2G_GRP_SET_1,	 FCC_5G_GRP_SET_3},
    /* CNTRY_AT */	{ WR_2G_GRP_SET_1,	ETSI_5G_GRP_SET_3},
    /* CNTRY_AU */	{ WR_2G_GRP_SET_1,	ETSI_5G_GRP_SET_5},
    /* CNTRY_AW */	{ WR_2G_GRP_SET_1,	ETSI_5G_GRP_SET_2},
    /* CNTRY_AZ */	{ WR_2G_GRP_SET_1,	ETSI_5G_GRP_SET_4},
    /* CNTRY_BA */	{ WR_2G_GRP_SET_1,	ETSI_5G_GRP_SET_3},
    /* CNTRY_BB */	{ WR_2G_GRP_SET_1,	 FCC_5G_GRP_SET_4},
    /* CNTRY_BD */	{ WR_2G_GRP_SET_1,	  JP_5G_GRP_SET_1},
    /* CNTRY_BE */	{ WR_2G_GRP_SET_1,	ETSI_5G_GRP_SET_3},
    /* CNTRY_BF */	{ WR_2G_GRP_SET_1,	 FCC_5G_GRP_SET_3},
    /* CNTRY_BG */	{ WR_2G_GRP_SET_1,	ETSI_5G_GRP_SET_3},
    /* CNTRY_BH */	{ WR_2G_GRP_SET_1,	 FCC_5G_GRP_SET_4},
    /* CNTRY_BL */	{ WR_2G_GRP_SET_1,	ETSI_5G_GRP_SET_2},
    /* CNTRY_BM */	{FCC_2G_GRP_SET_1,	 FCC_5G_GRP_SET_3},
    /* CNTRY_BN */	{ WR_2G_GRP_SET_1,	 FCC_5G_GRP_SET_4},
    /* CNTRY_BO */	{ WR_2G_GRP_SET_1,	  JP_5G_GRP_SET_2},
    /* CNTRY_BR */	{ WR_2G_GRP_SET_1,	 FCC_5G_GRP_SET_3},
    /* CNTRY_BS */	{ WR_2G_GRP_SET_1,	 FCC_5G_GRP_SET_3},
    /* CNTRY_BT */	{ WR_2G_GRP_SET_1,	ETSI_5G_GRP_SET_2},
    /* CNTRY_BY */	{ WR_2G_GRP_SET_1,	ETSI_5G_GRP_SET_2},
    /* CNTRY_BZ */	{ WR_2G_GRP_SET_1,	  JP_5G_GRP_SET_1},
    /* CNTRY_CA */	{FCC_2G_GRP_SET_1,	ETSI_5G_GRP_SET_5},
    /* CNTRY_CF */	{ WR_2G_GRP_SET_1,	 FCC_5G_GRP_SET_3},
    /* CNTRY_CH */	{ WR_2G_GRP_SET_1,	ETSI_5G_GRP_SET_3},
    /* CNTRY_CI */	{ WR_2G_GRP_SET_1,	 FCC_5G_GRP_SET_3},
    /* CNTRY_CL */	{ WR_2G_GRP_SET_1,	 FCC_5G_GRP_SET_4},
    /* CNTRY_CN */	{ WR_2G_GRP_SET_1,	 FCC_5G_GRP_SET_5},
    /* CNTRY_CO */	{ WR_2G_GRP_SET_1,	 FCC_5G_GRP_SET_3},
    /* CNTRY_CR */	{ WR_2G_GRP_SET_1,	 FCC_5G_GRP_SET_3},
    /* CNTRY_CU */	{ WR_2G_GRP_SET_1,	 FCC_5G_GRP_SET_6},
    /* CNTRY_CX */	{ WR_2G_GRP_SET_1,	 FCC_5G_GRP_SET_3},
    /* CNTRY_CY */	{ WR_2G_GRP_SET_1,	ETSI_5G_GRP_SET_3},
    /* CNTRY_CZ */	{ WR_2G_GRP_SET_1,	ETSI_5G_GRP_SET_3},
    /* CNTRY_DE */	{ WR_2G_GRP_SET_1,	ETSI_5G_GRP_SET_3},
    /* CNTRY_DK */	{ WR_2G_GRP_SET_1,	ETSI_5G_GRP_SET_3},
    /* CNTRY_DM */	{FCC_2G_GRP_SET_1,	 FCC_5G_GRP_SET_4},
    /* CNTRY_DO */	{FCC_2G_GRP_SET_1,	 FCC_5G_GRP_SET_4},
    /* CNTRY_DZ */	{ WR_2G_GRP_SET_1,	  JP_5G_GRP_SET_3},
    /* CNTRY_EC */	{ WR_2G_GRP_SET_1,	 FCC_5G_GRP_SET_7},
    /* CNTRY_EE */	{ WR_2G_GRP_SET_1,	ETSI_5G_GRP_SET_3},
    /* CNTRY_EG */	{ WR_2G_GRP_SET_1,	ETSI_5G_GRP_SET_4},
    /* CNTRY_ES */	{ WR_2G_GRP_SET_1,	ETSI_5G_GRP_SET_3},
    /* CNTRY_ET */	{ WR_2G_GRP_SET_1,	ETSI_5G_GRP_SET_2},
    /* CNTRY_EU */	{ ETSI_2G_GRP_SET_1,	ETSI_5G_GRP_SET_3},
    /* CNTRY_FI */	{ WR_2G_GRP_SET_1,	ETSI_5G_GRP_SET_3},
    /* CNTRY_FM */	{FCC_2G_GRP_SET_1,	 FCC_5G_GRP_SET_3},
    /* CNTRY_FR */	{ WR_2G_GRP_SET_1,	ETSI_5G_GRP_SET_3},
    /* CNTRY_GA */	{ WR_2G_GRP_SET_1,	 FCC_5G_GRP_SET_1},
    /* CNTRY_GB */	{ WR_2G_GRP_SET_1,	 FCC_5G_GRP_SET_3},
    /* CNTRY_GD */	{FCC_2G_GRP_SET_1,	 FCC_5G_GRP_SET_3},
    /* CNTRY_GE */	{ WR_2G_GRP_SET_1,	ETSI_5G_GRP_SET_4},
    /* CNTRY_GF */	{ WR_2G_GRP_SET_1,	ETSI_5G_GRP_SET_2},
    /* CNTRY_GH */	{ WR_2G_GRP_SET_1,	 FCC_5G_GRP_SET_3},
    /* CNTRY_GL */	{ WR_2G_GRP_SET_1,	ETSI_5G_GRP_SET_2},
    /* CNTRY_GP */	{ WR_2G_GRP_SET_1,	ETSI_5G_GRP_SET_2},
    /* CNTRY_GR */	{ WR_2G_GRP_SET_1,	ETSI_5G_GRP_SET_3},
    /* CNTRY_GT */	{FCC_2G_GRP_SET_1,	 FCC_5G_GRP_SET_4},
    /* CNTRY_GU */	{FCC_2G_GRP_SET_1,	 FCC_5G_GRP_SET_3},
    /* CNTRY_GY */	{ WR_2G_GRP_SET_1,	  JP_5G_GRP_SET_1},
    /* CNTRY_HK */	{ WR_2G_GRP_SET_1,	 FCC_5G_GRP_SET_3},
    /* CNTRY_HN */	{ WR_2G_GRP_SET_1,	 FCC_5G_GRP_SET_3},
    /* CNTRY_HR */	{ WR_2G_GRP_SET_1,	ETSI_5G_GRP_SET_3},
    /* CNTRY_HT */	{FCC_2G_GRP_SET_1,	 FCC_5G_GRP_SET_3},
    /* CNTRY_HU */	{ WR_2G_GRP_SET_1,	ETSI_5G_GRP_SET_3},
    /* CNTRY_ID */	{ WR_2G_GRP_SET_1,	  JP_5G_GRP_SET_4},
    /* CNTRY_IE */	{ WR_2G_GRP_SET_1,	ETSI_5G_GRP_SET_3},
    /* CNTRY_IL */	{ WR_2G_GRP_SET_1,	ETSI_5G_GRP_SET_3},
    /* CNTRY_IN */	{ WR_2G_GRP_SET_1,	 OTH_5G_GRP_SET_1},
    /* CNTRY_IR */	{ WR_2G_GRP_SET_1,	  JP_5G_GRP_SET_1},
    /* CNTRY_IS */	{ WR_2G_GRP_SET_1,	ETSI_5G_GRP_SET_3},
    /* CNTRY_IT */	{ WR_2G_GRP_SET_1,	ETSI_5G_GRP_SET_3},
    /* CNTRY_JM */	{ WR_2G_GRP_SET_1,	 FCC_5G_GRP_SET_3},
    /* CNTRY_JO */	{ WR_2G_GRP_SET_1,	  JP_5G_GRP_SET_5},
    /* CNTRY_JP */	{ JP_2G_GRP_SET_1,	  JP_5G_GRP_SET_6},
    /* CNTRY_KE */	{ WR_2G_GRP_SET_1,	  JP_5G_GRP_SET_7},
    /* CNTRY_KH */	{ WR_2G_GRP_SET_1,	ETSI_5G_GRP_SET_2},
    /* CNTRY_KN */	{ WR_2G_GRP_SET_1,	ETSI_5G_GRP_SET_6},
    /* CNTRY_KP */	{ WR_2G_GRP_SET_1,	  JP_5G_GRP_SET_8},
    /* CNTRY_KR */	{ OTH_2G_GRP_SET_1,	 OTH_5G_GRP_SET_1},
    /* CNTRY_KW */	{ WR_2G_GRP_SET_1,	ETSI_5G_GRP_SET_4},
    /* CNTRY_KY */	{ WR_2G_GRP_SET_1,	 FCC_5G_GRP_SET_3},
    /* CNTRY_KZ */	{ WR_2G_GRP_SET_1,	 FCC_5G_GRP_SET_3},
    /* CNTRY_LB */	{ WR_2G_GRP_SET_1,	 FCC_5G_GRP_SET_3},
    /* CNTRY_LC */	{ WR_2G_GRP_SET_1,	ETSI_5G_GRP_SET_6},
    /* CNTRY_LI */	{ WR_2G_GRP_SET_1,	  JP_5G_GRP_SET_6},
    /* CNTRY_LK */	{ WR_2G_GRP_SET_1,	 FCC_5G_GRP_SET_3},
    /* CNTRY_LS */	{ WR_2G_GRP_SET_1,	ETSI_5G_GRP_SET_2},
    /* CNTRY_LT */	{ WR_2G_GRP_SET_1,	ETSI_5G_GRP_SET_3},
    /* CNTRY_LU */	{ WR_2G_GRP_SET_1,	ETSI_5G_GRP_SET_3},
    /* CNTRY_LV */	{ WR_2G_GRP_SET_1,	ETSI_5G_GRP_SET_3},
    /* CNTRY_MA */	{ WR_2G_GRP_SET_1,	ETSI_5G_GRP_SET_4},
    /* CNTRY_MC */	{ WR_2G_GRP_SET_1,	ETSI_5G_GRP_SET_3},
    /* CNTRY_MD */	{ WR_2G_GRP_SET_1,	ETSI_5G_GRP_SET_3},
    /* CNTRY_ME */	{ WR_2G_GRP_SET_1,	ETSI_5G_GRP_SET_3},
    /* CNTRY_MF */	{ WR_2G_GRP_SET_1,	ETSI_5G_GRP_SET_2},
    /* CNTRY_MH */	{FCC_2G_GRP_SET_1,	 FCC_5G_GRP_SET_3},
    /* CNTRY_MK */	{ WR_2G_GRP_SET_1,	ETSI_5G_GRP_SET_3},
    /* CNTRY_MN */	{ WR_2G_GRP_SET_1,	 FCC_5G_GRP_SET_3},
    /* CNTRY_MO */	{ WR_2G_GRP_SET_1,	 FCC_5G_GRP_SET_3},
    /* CNTRY_MP */	{FCC_2G_GRP_SET_1,	 FCC_5G_GRP_SET_3},
    /* CNTRY_MQ */	{ WR_2G_GRP_SET_1,	ETSI_5G_GRP_SET_2},
    /* CNTRY_MR */	{ WR_2G_GRP_SET_1,	ETSI_5G_GRP_SET_2},
    /* CNTRY_MT */	{ WR_2G_GRP_SET_1,	ETSI_5G_GRP_SET_3},
    /* CNTRY_MU */	{ WR_2G_GRP_SET_1,	 FCC_5G_GRP_SET_3},
    /* CNTRY_MV */	{ WR_2G_GRP_SET_1,	 FCC_5G_GRP_SET_4},
    /* CNTRY_MW */	{ WR_2G_GRP_SET_1,	ETSI_5G_GRP_SET_2},
    /* CNTRY_MX */	{ WR_2G_GRP_SET_1,	 FCC_5G_GRP_SET_3},
    /* CNTRY_MY */	{ WR_2G_GRP_SET_1,	FCC_5G_GRP_SET_10},
    /* CNTRY_NG */	{ WR_2G_GRP_SET_1,	  JP_5G_GRP_SET_2},
    /* CNTRY_NI */	{FCC_2G_GRP_SET_1,	 FCC_5G_GRP_SET_3},
    /* CNTRY_NL */	{ WR_2G_GRP_SET_1,	ETSI_5G_GRP_SET_3},
    /* CNTRY_NO */	{ WR_2G_GRP_SET_1,	ETSI_5G_GRP_SET_3},
    /* CNTRY_NP */	{ WR_2G_GRP_SET_1,	 FCC_5G_GRP_SET_4},
    /* CNTRY_NZ */	{ WR_2G_GRP_SET_1,	 FCC_5G_GRP_SET_3},
    /* CNTRY_OM */	{ WR_2G_GRP_SET_1,	ETSI_5G_GRP_SET_2},
    /* CNTRY_PA */	{FCC_2G_GRP_SET_1,	 FCC_5G_GRP_SET_8},
    /* CNTRY_PE */	{ WR_2G_GRP_SET_1,	 FCC_5G_GRP_SET_3},
    /* CNTRY_PF */	{ WR_2G_GRP_SET_1,	ETSI_5G_GRP_SET_2},
    /* CNTRY_PG */	{ WR_2G_GRP_SET_1,	 FCC_5G_GRP_SET_3},
    /* CNTRY_PH */	{ WR_2G_GRP_SET_1,	 FCC_5G_GRP_SET_3},
    /* CNTRY_PK */	{ WR_2G_GRP_SET_1,	  JP_5G_GRP_SET_9},
    /* CNTRY_PL */	{ WR_2G_GRP_SET_1,	ETSI_5G_GRP_SET_3},
    /* CNTRY_PM */	{ WR_2G_GRP_SET_1,	ETSI_5G_GRP_SET_2},
    /* CNTRY_PR */	{FCC_2G_GRP_SET_1,	 FCC_5G_GRP_SET_3},
    /* CNTRY_PT */	{ WR_2G_GRP_SET_1,	ETSI_5G_GRP_SET_3},
    /* CNTRY_PW */	{FCC_2G_GRP_SET_1,	 FCC_5G_GRP_SET_3},
    /* CNTRY_PY */	{ WR_2G_GRP_SET_1,	 FCC_5G_GRP_SET_3},
    /* CNTRY_QA */	{ WR_2G_GRP_SET_1,	ETSI_5G_GRP_SET_3},
    /* CNTRY_RE */	{ WR_2G_GRP_SET_1,	ETSI_5G_GRP_SET_2},
    /* CNTRY_RO */	{ WR_2G_GRP_SET_1,	ETSI_5G_GRP_SET_3},
    /* CNTRY_RS */	{ WR_2G_GRP_SET_1,	ETSI_5G_GRP_SET_3},
    /* CNTRY_RU */	{ WR_2G_GRP_SET_1,	 OTH_5G_GRP_SET_2},
    /* CNTRY_RW */	{ WR_2G_GRP_SET_1,	 FCC_5G_GRP_SET_3},
    /* CNTRY_SA */	{ WR_2G_GRP_SET_1,	ETSI_5G_GRP_SET_2},
    /* CNTRY_SE */	{ WR_2G_GRP_SET_1,	ETSI_5G_GRP_SET_3},
    /* CNTRY_SG */	{ WR_2G_GRP_SET_1,	 FCC_5G_GRP_SET_3},
    /* CNTRY_SI */	{ WR_2G_GRP_SET_1,	ETSI_5G_GRP_SET_3},
    /* CNTRY_SK */	{ WR_2G_GRP_SET_1,	ETSI_5G_GRP_SET_3},
    /* CNTRY_SN */	{ WR_2G_GRP_SET_1,	 FCC_5G_GRP_SET_3},
    /* CNTRY_SR */	{ WR_2G_GRP_SET_1,	ETSI_5G_GRP_SET_2},
    /* CNTRY_SV */	{ WR_2G_GRP_SET_1,	 FCC_5G_GRP_SET_4},
    /* CNTRY_SY */	{ WR_2G_GRP_SET_1,	 OTH_5G_GRP_SET_3},
    /* CNTRY_TC */	{ WR_2G_GRP_SET_1,	 FCC_5G_GRP_SET_3},
    /* CNTRY_TD */	{ WR_2G_GRP_SET_1,	ETSI_5G_GRP_SET_2},
    /* CNTRY_TG */	{ WR_2G_GRP_SET_1,	ETSI_5G_GRP_SET_2},
    /* CNTRY_TH */	{ WR_2G_GRP_SET_1,	 FCC_5G_GRP_SET_3},
    /* CNTRY_TN */	{ WR_2G_GRP_SET_1,	ETSI_5G_GRP_SET_4},
    /* CNTRY_TR */	{ WR_2G_GRP_SET_1,	  JP_5G_GRP_SET_6},
    /* CNTRY_TT */	{ WR_2G_GRP_SET_1,	 FCC_5G_GRP_SET_3},
    /* CNTRY_TW */	{ WR_2G_GRP_SET_1,	 FCC_5G_GRP_SET_3},
    /* CNTRY_TZ */	{ WR_2G_GRP_SET_1,	  JP_5G_GRP_SET_1},
    /* CNTRY_UA */	{ WR_2G_GRP_SET_1,	 FCC_5G_GRP_SET_3},
    /* CNTRY_UG */	{ WR_2G_GRP_SET_1,	 FCC_5G_GRP_SET_3},
    /* CNTRY_UK */	{ WR_2G_GRP_SET_1,	 FCC_5G_GRP_SET_3},
    /* CNTRY_US */	{FCC_2G_GRP_SET_2,	 FCC_5G_GRP_SET_2},
    /* CNTRY_UY */	{ WR_2G_GRP_SET_1,	 FCC_5G_GRP_SET_4},
    /* CNTRY_UZ */	{ WR_2G_GRP_SET_1,	ETSI_5G_GRP_SET_4},
    /* CNTRY_VA */	{ WR_2G_GRP_SET_1,	 FCC_5G_GRP_SET_1},
    /* CNTRY_VC */	{ WR_2G_GRP_SET_1,	ETSI_5G_GRP_SET_2},
    /* CNTRY_VE */	{ WR_2G_GRP_SET_1,	 FCC_5G_GRP_SET_4},
    /* CNTRY_VI */	{FCC_2G_GRP_SET_1,	 FCC_5G_GRP_SET_3},
    /* CNTRY_VN */	{ WR_2G_GRP_SET_1,	 FCC_5G_GRP_SET_3},
    /* CNTRY_VU */	{ WR_2G_GRP_SET_1,	 FCC_5G_GRP_SET_3},
    /* CNTRY_WF */	{ WR_2G_GRP_SET_1,	ETSI_5G_GRP_SET_2},
    /* CNTRY_WS */	{ WR_2G_GRP_SET_1,	ETSI_5G_GRP_SET_2},
    /* CNTRY_YE */	{ WR_2G_GRP_SET_1,	 OTH_5G_GRP_SET_3},
    /* CNTRY_YT */	{ WR_2G_GRP_SET_1,	ETSI_5G_GRP_SET_2},
    /* CNTRY_ZA */	{ WR_2G_GRP_SET_1,	ETSI_5G_GRP_SET_2},
    /* CNTRY_ZW */	{ WR_2G_GRP_SET_1,	ETSI_5G_GRP_SET_2},

    /* CNTRY_ZZ */  {WR_2G_GRP_SET_ZZ,   WR_5G_GRP_SET_ZZ}
};


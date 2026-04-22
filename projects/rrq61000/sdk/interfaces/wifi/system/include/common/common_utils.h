/**
 ****************************************************************************************
 *
 * @file common_util.h
 *
 * @brief Define for System common
 *
 * Copyright (c) 2023 Renesas Electronics. All rights reserved.
 *
 * This software ("Software") is owned by Renesas Electronics.
 *
 * By using this Software you agree that Renesas Electronics retains all
 * intellectual property and proprietary rights in and to this Software and any
 * use, reproduction, disclosure or distribution of the Software without express
 * written permission or a license agreement from Renesas Electronics is
 * strictly prohibited. This Software is solely for use on or in conjunction
 * with Renesas Electronics products.
 *
 * EXCEPT AS OTHERWISE PROVIDED IN A LICENSE AGREEMENT BETWEEN THE PARTIES, THE
 * SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. EXCEPT AS OTHERWISE
 * PROVIDED IN A LICENSE AGREEMENT BETWEEN THE PARTIES, IN NO EVENT SHALL
 * RENESAS ELECTRONICS BE LIABLE FOR ANY DIRECT, SPECIAL, INDIRECT, INCIDENTAL,
 * OR CONSEQUENTIAL DAMAGES, OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF
 * USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE
 * OF THE SOFTWARE.
 *
 ****************************************************************************************
 */


#ifndef	__COMMON_UTILS_H__
#define	__COMMON_UTILS_H__


/* ANSI CODE */
#define ESCCODE						"\33"

/* Set  Attributes */
#define ANSI_BOLD					"\33[1m"
#define ANSI_UNDERLINE				"\33[4m"
#define ANSI_BLINK					"\33[5m"
#define ANSI_REVERSE				"\33[7m"

#define ANSI_R_BOLD					"\33[21m"
#define ANSI_R_UNDERLINE			"\33[24m"
#define ANSI_R_BLINK				"\33[25m"
#define ANSI_R_REVERSE				"\33[27m"

/* Rset  Attributes */
#define ANSI_NORMAL					"\33[0m"
#define ANSI_RESET_BOLD				"\33[21m"
#define ANSI_RESET_UNDERLINE		"\33[24m"
#define ANSI_RESET_BLINK			"\33[25m"
#define ANSI_RESET_REVERSE			"\33[27m"

/* Control */
#define ANSI_CLEAR					"\33[2J"
#define ANSI_CURON					"\33[?25h"
#define ANSI_CUROFF					"\33[?25l"
#define ANSI_BELL					"\7"
#define ANSI_ERASE					"\33[0J"
#define ANSI_LEFT          		  	"\33[1D"
#define ANSI_RIGHT         		  	"\33[1C"

/* Foreground Color (text) */
#define ANSI_COLOR_BLACK			"\33[30m"
#define ANSI_COLOR_RED				"\33[31m"
#define ANSI_COLOR_GREEN			"\33[32m"
#define ANSI_COLOR_YELLOW			"\33[33m"
#define ANSI_COLOR_BLUE				"\33[34m"
#define ANSI_COLOR_MAGENTA			"\33[35m"
#define ANSI_COLOR_CYAN				"\33[36m"
#define ANSI_COLOR_WHITE			"\33[37m"
#define ANSI_COLOR_LIGHT_RED		"\33[1;31m"
#define ANSI_COLOR_LIGHT_GREEN		"\33[1;32m"
#define ANSI_COLOR_LIGHT_YELLOW		"\33[1;33m"
#define ANSI_COLOR_LIGHT_BLUE		"\33[1;34m"
#define ANSI_COLOR_LIGHT_MAGENTA	"\33[1;35m"
#define ANSI_COLOR_LIGHT_CYAN		"\33[1;36m"
#define ANSI_COLOR_LIGHT_WHITE		"\33[1;37m"
#define ANSI_COLOR_DEFULT			"\33[0;39m"

/* Background Color */
#define ANSI_BCOLOR_BLACK			"\33[40m"
#define ANSI_BCOLOR_RED				"\33[41m"
#define ANSI_BCOLOR_GREEN			"\33[42m"
#define ANSI_BCOLOR_YELLOW			"\33[43m"
#define ANSI_BCOLOR_BLUE			"\33[44m"
#define ANSI_BCOLOR_MAGENTA			"\33[45m"
#define ANSI_BCOLOR_CYAN			"\33[46m"
#define ANSI_BCOLOR_WHITE			"\33[47m"
#define ANSI_BCOLOR_LIGHT_RED		"\33[1;41m"
#define ANSI_BCOLOR_LIGHT_GREEN		"\33[1;42m"
#define ANSI_BCOLOR_LIGHT_YELLOW	"\33[1;43m"
#define ANSI_BCOLOR_LIGHT_BLUE		"\33[1;44m"
#define ANSI_BCOLOR_LIGHT_MAGENTA	"\33[1;45m"
#define ANSI_BCOLOR_LIGHT_CYAN		"\33[1;46m"
#define ANSI_BCOLOR_LIGHT_WHITE		"\33[1:47m"

#define ANSI_BCOLOR_DEFULT			"\33[0;49m"



#define VT_CLEAR					printf("\33[2J")
#define VT_CURPOS(X,Y)				printf("\33[%d;%dH", Y, X)
#define VT_NORMAL					printf("\33[0m")
#define VT_BOLD						printf("\33[1m")
#define VT_BLINK					printf("\33[5m")
#define VT_REVERSE					printf("\33[7m")
#define VT_CURON					printf("\33[?25h")
#define VT_CUROFF					printf("\33[?25l")
#define VT_BELL						printf("\007")
#define VT_ERASE					printf("\33[0J")
#define VT_LEFT						printf("\33[1D")
#define VT_RIGHT					printf("\33[1C")
#define VT_LINECLEAR(X)				VT_CURPOS(1, X); printf("\33[2K")
#define VT_COLORBLACK				printf("\33[30m")
#define VT_COLORRED					printf("\33[31m")
#define VT_COLORGREEN				printf("\33[32m")
#define VT_COLORYELLOW				printf("\33[33m")
#define VT_COLORBLUE				printf("\33[34m")
#define VT_COLORMAGENTA				printf("\33[35m")
#define VT_COLORCYAN				printf("\33[36m")
#define VT_COLORWHITE				printf("\33[37m")
#define VT_COLORDEFULT				printf("\33[39m")
#define VT_BCOLORBLACK				printf("\33[40m")
#define VT_BCOLORRED				printf("\33[41m")
#define VT_BCOLORGREEN				printf("\33[42m")
#define VT_BCOLORYELLOW				printf("\33[43m")
#define VT_BCOLORBLUE				printf("\33[44m")
#define VT_BCOLORMAGENTA			printf("\33[45m")
#define VT_BCOLORCYAN				printf("\33[46m")
#define VT_BCOLORWHITE				printf("\33[47m")
#define VT_BDEFULT					printf("\33[49m")
#define VT_COLOROFF					VT_BOLD;VT_BCOLORBLACK;VT_COLORWHITE


//ISDIGIT onderzoekt of een karakter numeric is
#define ISDIGIT(c)( (c < '0' || c > '9') ? 0 : 1)

void pntdumpbin ( unsigned char* pbyBin, int nLen, int simple);

/**
 ****************************************************************************************
 * @brief      wpa_cli for RRQ61000 wpa_supplicant
 * @param[in]  cmdline    Input wpa_cli command string.
 * @param[in]  delimit    Command string delimiter
 * @param[in]  cli_reply  Result reply string buffer
 * @return     0 on success, others on fail
 ****************************************************************************************
 */
int  ra6w1_cli_reply(char *cmdline, char *delimit, char *cli_reply);

int  factory_reset(int reboot_flag);
long subnetRangeLastIP(long ip, long subnet);
UINT writeDataToFlash(UINT *destFlashAddr, UINT *srcMemAddr, UINT size);
#endif	/* __COMMON_UTILS_H__ */

/* EOF */

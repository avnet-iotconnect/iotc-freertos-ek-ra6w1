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

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/

#include "r_rtc_w.h"

#include <stdio.h>
#include <stdlib.h>

char *_fmt(const char *format, const struct tm *const t, char *pt,
           const char *const ptlim, int *warnp);

#define Locale    (&C_time_locale)

#ifndef YEAR_2000_NAME
#define YEAR_2000_NAME    "CHECK_STRFTIME_FORMATS_FOR_TWO_DIGIT_YEARS"
#endif /* !defined YEAR_2000_NAME */

#define IN_NONE        0

size_t ra6w1_strftime(char * ptr, size_t maxsize, const char * format, const struct tm * t)
{
    char * p;
    int warn;

    warn = IN_NONE;
    p = _fmt(((format == NULL) ? "%c" : format), t, ptr, ptr + maxsize, &warn);
#ifdef RUN_TIME_WARNINGS_ABOUT_YEAR_2000_PROBLEMS_THANK_YOU

    if (warn != IN_NONE && getenv(YEAR_2000_NAME) != NULL) {
        printf("\n");

        if (format == NULL) {
            printf("NULL strftime format ");
        } else {
            printf("strftime format \"%s\" ", format);
        }

        printf("yields only two digits of years in ");

        if (warn == IN_SOME) {
            printf("some locales");
        } else if (warn == IN_THIS) {
            printf("the current locale");
        } else {
            printf("all locales");
        }

        printf("\n");
    }

#endif /* defined RUN_TIME_WARNINGS_ABOUT_YEAR_2000_PROBLEMS_THANK_YOU */

    if (p == ptr + maxsize) {
        return 0;
    }

    *p = '\0';
    return (size_t)(p - ptr);
}

/* EOF */

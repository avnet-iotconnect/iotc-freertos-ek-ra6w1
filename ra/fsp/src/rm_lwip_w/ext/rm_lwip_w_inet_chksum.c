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

/*
 * rm_lwip_w_inet_checksum.c
 *
 *  Created on: 14-Oct-2024
 *      Author: renesas
 */

#include "lwip/opt.h"

#include "lwip/inet_chksum.h"
#include "lwip/def.h"

#if (LWIP_CHKSUM_ALGORITHM == 4) /* Alternative version #4 by Dialog */
u16_t
lwip_standard_chksum(const void *dataptr, int len)
{
	u32_t acc;
	u16_t src;
	int 	curlen;
	const u8_t *octetptr , *end_ptr;
	u32_t *long_ptr , *temp_ptr;
	unsigned long src_bin;

    	acc = 0;
	long_ptr = (u32_t *)(const void *)dataptr;

	end_ptr = dataptr + len;
	temp_ptr = long_ptr;
	/* 4 byte alignment checking */
	src_bin = dataptr;
	if (!(src_bin & ~3))
	{
		while(long_ptr < end_ptr)
		{
			temp_ptr = long_ptr;
			acc += (*long_ptr & LOWER_16_MASK);
			acc += (*long_ptr >> SHIFT_BY_16);
			long_ptr++;
		}
		long_ptr = temp_ptr;
	}

	/* after 4 byte checksum , remaining checking */
	octetptr = (char *)long_ptr;
	curlen = end_ptr - octetptr;

	while(curlen > 1)
	{
		src = (*octetptr) << 8;
		octetptr++;

		/* declare second octet as least significant */
		src |= (*octetptr);
		octetptr++;
		acc += src;
		curlen -= 2;
	}

	if (curlen > 0)
	{
  		/* accumulate remaining octet */
		src = (*octetptr) << 8;
		acc += src;
	}

	/* add deferred carry bits */
	acc = (acc >> 16) + (acc & 0x0000ffffUL);
	acc = (acc >> 16) + (acc & 0x0000ffffUL);

	return lwip_htons((u16_t)acc);
}
#endif



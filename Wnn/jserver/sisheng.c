/*
 * $Id: sisheng.c,v 1.1.1.1 2000/01/16 05:07:46 ura Exp $
 */

/*
 * FreeWnn is a network-extensible Kana-to-Kanji conversion system.
 * This file is part of FreeWnn.
 * 
 * Copyright Kyoto University Research Institute for Mathematical Sciences
 *                 1987, 1988, 1989, 1990, 1991, 1992
 * Copyright OMRON Corporation. 1987, 1988, 1989, 1990, 1991, 1992, 1999
 * Copyright ASTEC, Inc. 1987, 1988, 1989, 1990, 1991, 1992
 *
 * Author: OMRON SOFTWARE Co., Ltd. <freewnn@rd.kyoto.omronsoft.co.jp>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GNU Emacs; see the file COPYING.  If not, write to the
 * Free Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * Commentary:
 *
 * Change log:
 *
 * Last modified date: 8,Feb.1999
 *
 * Code:
 *
 */
/**  cWnn  Version 1.1	 **/

#ifdef CHINESE

#include <stdio.h>
#include "commonhd.h"
#include "cplib.h"
#include "de_header.h"

#define isfuyuanyin(c)  ((c=='m')||(c=='n')||(c=='g'))
#define isyuanyin(c)  \
	((c=='a')||(c=='e')||(c=='i')||(c=='o')||(c=='u')||(c=='v'))

/* yincod_flt_sisheng(): filter sisheng of Yincode strings */

static int 
yincod_flt_sisheng(yincod,si,siyincod)
w_char *yincod;				/* Yincode without sisheng */
char *si;				/* sisheng string */
w_char *siyincod;			/* Yincode with sisheng */
{
  register int pan_count = 0;
	for ( ; *siyincod; siyincod++ )  {
		if ( _cwnn_isyincod_d(*siyincod) )  {
			*si++ = (char)(_cwnn_sisheng(*siyincod) + 0x30);
			*yincod++ = _cwnn_yincod_0(*siyincod);
			} else {
			*si++ = '5';
			*yincod++ = *siyincod;
			}
			if (pan_count++ == 5)   *(si-1) = 0;
		}
	*yincod = 0;
	*si = 0;
	return(strlen(si));
}

int get_sisheng(yomi,si,yomi_tmp)
w_char  *yomi;
w_char  *yomi_tmp;
char    *si;
{
  int length;

	length = yincod_flt_sisheng(yomi_tmp,si,yomi);
        return(length);
}

w_char *biki_sisheng(yomi,si,yomi_tmp)
w_char  *yomi;
w_char  *yomi_tmp;
char    *si;
{
  int length;

	length = yincod_flt_sisheng(yomi_tmp,si,yomi);
        return(yomi_tmp);
}

unsigned int diff_sisheng(si1, si2)    /* PAN deleted by pan shilei */
int si1;
int si2;
{
  register int length;
  register unsigned int reslt;
  char s1[7];
  char s2[7];

	sprintf(s1,"%d",si1);
	sprintf(s2,"%d",si2);
	length = strlen(s1) - 1;
	for (reslt = 1; length >= 0; length--)
		if(s1[length] == s2[length])
			reslt = reslt*(10 - length);
        return(reslt);
}

#endif /* CHINESE */

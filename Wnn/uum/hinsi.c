/*
 * $Id: hinsi.c,v 1.1.1.1 2000/01/16 05:07:51 ura Exp $
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
/*	Version 4.0
 */
#include <stdio.h>
#include "commonhd.h"
#include "sdefine.h"
#include "sheader.h"
#include "jllib.h"

/*#define ROOT "�ʻ�/" */
#define ROOT "/"
/*
char *maeni = "�������";
*/

#define MAXHINSI 256

static w_char *
bunpou_search(node)
w_char *node;
{
    int cnt;
    int k;
    char *hbuf[MAXHINSI];
    char buf[1024];		/* iikagen */
    char *c;
    w_char **whbuf;
    w_char *ret;

    if(node == NULL) return(NULL);
    cnt = jl_hinsi_list(bun_data_, -1, node, &whbuf);
    if(cnt == -1){
	errorkeyin();
	return(NULL);
    }
    if(cnt == 0) return(node);
    for(k = 0, c = buf ; k < cnt ; k++){
	hbuf[k] = c;
	sStrcpy(c, whbuf[k]);
	c += strlen(c) + 1;
    }
    hbuf[cnt] = MSG_GET(15);
 TOP:
    k = select_one_element(hbuf, cnt + 1, 0, "", 0, 0, main_table[4]);
    if(k == -1)return(NULL);
    if(strcmp(hbuf[k], MSG_GET(15)) == 0) return((w_char *)MSG_GET(15));
    Sstrcpy(node, hbuf[k]);
    ret = bunpou_search(node);
    if(ret == NULL) return(NULL);
    if(strcmp((char *)ret, MSG_GET(15)) == 0) goto TOP;
    return(ret);
}


int
hinsi_in()
{
    w_char *a;
    w_char tmp[WNN_HINSI_NAME_LEN];

    Sstrcpy(tmp, ROOT);
    not_redraw = 1;
    if((a = bunpou_search(tmp)) == NULL ||
       strcmp((char *)a, MSG_GET(15)) == 0) {
	not_redraw = 0;
	return(-1);
    }
    not_redraw = 0;
    return(jl_hinsi_number(bun_data_, a));
}

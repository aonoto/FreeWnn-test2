/*
 * $Id: wnn_string.h,v 1.1 2000/01/16 05:07:45 ura Exp $
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

#ifndef _WNN_STRING_
#define _WNN_STRING_

extern int wnn_sStrcpy();
extern int wnn_Sstrcpy();
extern void wnn_Sreverse();
extern char * wnn_Stos();
extern char *wnn_sStrncpy();
extern w_char *wnn_Strcat();
extern w_char *wnn_Strncat();
extern int wnn_Strcmp();
extern int wnn_Substr();
extern int wnn_Strncmp();
extern w_char *wnn_Strncpy();
extern int wnn_Strlen();
extern w_char *wnn_Strcpy();
extern void wnn_delete_w_ss2();
extern int wnn_byte_count();
#ifndef JS
extern int check_pwd();
#endif
#ifdef CHINESE
extern int wnn_Sstrcat();
#endif
#endif
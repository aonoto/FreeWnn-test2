/*
 * $Id: malloc.c,v 1.2 2000/01/16 06:37:15 ura Exp $
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
#include "wnn_malloc.h"
#undef	malloc
#undef	realloc
#undef	free

#ifdef hpux
extern void *malloc(size_t);
extern void free(void *);
extern void *realloc(void *, size_t);
extern void *calloc(size_t, size_t);
#else
extern char *malloc();
extern void free();
extern char *realloc();
extern char *calloc();
#endif /* hpux */

#ifdef hpux
void *
malloc0(size)
size_t size;
#else
char *
malloc0(size)
int size;
#endif /* hpux */
{
    if(size == NULL){
	size = 1;
    }
    size = (size + 7) & 0xfffffff8;
    return(calloc(size,1));
}

#ifdef hpux
void
free0(pter)
void *pter;
#else
void
free0(pter)
char *pter;
#endif /* hpux */
{
    if(pter == NULL) return;
    free(pter);
    return;
}

#ifdef hpux
void *
realloc0(pter,size)
void *pter;
size_t size;
#else
char *
realloc0(pter,size)
char *pter;
int size;
#endif /* hpux */
{
    if(size == NULL){
	size = 1;
    }
    size = (size + 7) & 0xfffffff8;
    if(pter == NULL)
	return(malloc(size));
    return(realloc(pter,size));
}
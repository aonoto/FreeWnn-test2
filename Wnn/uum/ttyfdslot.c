/*
 * $Id: ttyfdslot.c,v 1.1 2000/01/16 05:07:51 ura Exp $
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
#include "commonhd.h"

/*
	BSD42
 */
#if defined(BSD42) && (! defined(BSD43)) || defined(linux)

#include <stdio.h>
#define SLOTSIZ 32

char *ttyname();
char *rindex();

int ttyfdslot(fd)
int fd;
{
    char eachslot[SLOTSIZ];
    register char *fullnamp;
    register char *ttynamp;
    register FILE *ttysfp;
    register char *p;
    register int slotnum;

    if ((fullnamp = ttyname(fd)) == NULL) return NULL;
    if ((ttynamp = rindex(fullnamp, '/')) == NULL) {
        ttynamp = fullnamp;
    } else {
        ttynamp++;
    }
    if ((ttysfp = fopen("/etc/ttys", "r")) == NULL) return NULL;
    for (slotnum = 0; fgets(eachslot, SLOTSIZ, ttysfp); ) {
        p = eachslot + strlen(eachslot) - 1;
        if (*p == '\n') *p = '\0';
        slotnum++;
        if (strcmp(ttynamp, &eachslot[2]) == 0) {
            fclose(ttysfp);
            return slotnum;
        }
    }
    fclose(ttysfp);
    return NULL;
}
#endif	/* defined(BSD42) && (! defined(BSD43)) || defined(linux) */


/*
	BSD43
 */
#if defined(BSD43) && !defined(linux)

#include <ttyent.h>
#include <stdio.h>

char *ttyname();
char *rindex();

int ttyfdslot(fd)
int fd;
{
    register char *fullnamp;
    register char *ttynamp;
    register int slotnum;
    register struct ttyent	*te ;
    if ((fullnamp = ttyname(fd)) == NULL) return NULL;
    if ((ttynamp = rindex(fullnamp, '/')) == NULL) {
        ttynamp = fullnamp;
    } else {
        ttynamp++;
    }
    for (slotnum = 1; (te = getttyent()) != NULL; slotnum++) {
	if ( strcmp(te->ty_name, ttynamp) == 0 ) {
		endttyent() ;
		return	slotnum ;
	}
    }
    endttyent() ;
    return NULL;
}

#endif	/* defined(BSD43) && !defined(linux) */
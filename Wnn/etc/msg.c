/*
 * $Id: msg.c,v 1.1 2000/01/16 05:07:45 ura Exp $
 */

/*
 * FreeWnn is a network-extensible Kana-to-Kanji conversion system.
 * This file is part of FreeWnn.
 * 
 * Copyright OMRON Corporation. 1987, 1988, 1989, 1990, 1991, 1992, 1999
 * Copyright 1991, 1992 Massachusetts Institute of Technology
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
/*
	struct msg_cat msg_open(name, nlspath, lang)
	char *name;
	char *nlspath;
	char *lang;

	char * msg_get(cd, id, s, lang)
	struct msg_cat cd;
	int id;
	char *s;

	void msg_close(cd)
	struct msg_cat cd;

	format of message file
	    <message id>\t<message>
*/

#include <stdio.h>
#include "commonhd.h"
#include "wnn_os.h"
#include "msg.h"

extern char *getenv();

static char *
bsearch(ky, bs, nel, width, compar)
char *ky;
char *bs;
unsigned long nel;
unsigned long width;
int (*compar)();
{
    char *key = ky;
    char *base = bs;
    int two_width = width + width;
    char *last = base + width * (nel - 1);

    register char *p;
    register int ret;
#ifdef hpux
    register int tmp;
#endif

    while (last >= base) {
	p = base + width * ((last - base)/two_width);
	ret = (*compar)((void *)key, (void *)p);

	if (ret == 0)
	    return ((char *)p);	/* found */
#ifdef hpux
	if (ret < 0) {
	    tmp = p;
	    tmp -= width;
	    last = tmp;
	} else {
	    tmp = p;
	    tmp += width;
	    base = tmp;
	}
#else /* hpux */
	if (ret < 0)
	    last = p - width;
	else
	    base = p + width;
#endif /* hpux */
    }
    return ((char *) 0);	/* not found */
}

static char *
getlang(lang)
char *lang;
{
    static char tmp[32];
    char *p;
    int i;

    if(lang == NULL || *lang == '\0'){
#ifdef	HAS_SETLOCALE
        lang = setlocale(LC_ALL, NULL);
        if(lang == NULL || *lang == '\0')
#endif
	{
	    lang = getenv("LC_MESSAGES");
	    if(lang == NULL || *lang == '\0'){
		lang = getenv("LANG");
		if(lang == NULL || *lang == '\0'){
		    lang = DEF_LANG;
		}
	    }
	}
    }
    for (i = 0, p = lang; *p && *p != '.'; i++, p++) {
	tmp[i] = *p;
    }
    tmp[i] = '\0';
    return(tmp);
    /*
    return(lang);
    */
}

static int
_search(id, bd)
register int id;
register struct msg_bd *bd;
{
    return(id - bd->msg_id);
}

static void
_escape(op, ip)
register char *op, *ip;
{
    for( ; *ip != 0; ip++, op++){
	if(*ip == '\\'){
	    switch(*++ip){
	    case 'n':
		*op = '\n';
		break;
	    case 't':
		*op = '\t';
		break;
	    case 'b':
		*op = '\b';
		break;
	    case 'r':
		*op = '\r';
		break;
	    case 'f':
		*op = '\f';
		break;
	    case 'v':
		*op = '\v';
		break;
	    case '0':
		*op = 0;
		break;
	    /*
	    case 'a':
		*op = '\a';
		break;
	    case 'e':
	    case 'E':
	    case 'o':
	    case 'd':
	    case 'x':
		break;
	    */
	    default:
		*op = *ip;
		break;
	    }
	} else {
	    if (*ip == '\n') {
		*op = '\0';
	    } else {
		*op = *ip;
	    }
	}
    }
    *op = 0;
}

static char *
get_msg_bd(cd, id)
register struct msg_cat *cd;
register int id;
{
    register struct msg_bd *bd;
    if(cd->msg_bd == 0 || cd->msg_cnt == 0)
	return(NULL);
    bd = (struct msg_bd *)
	bsearch(id, cd->msg_bd, cd->msg_cnt, sizeof(struct msg_bd), _search);
    if(bd == NULL)
	return(NULL);
    return(bd->msg);
}

/* expand
	%N: the value of the name parameter passed to msg_open()
	%L: the value of LANG
	%l: the language element from LANG
	%t: the territory element from LANG
	%c: the codeset element from LANG
	%%: a single % charctor 
*/
static int
expand(op, ip, name, lang)
register char *op, *ip, *name, *lang;
{
     if (!ip || !*ip) return(-1);
     for( ; *ip != 0; ip++){
	if(*ip == '%'){
	    switch(*++ip){
	    case 'N':
		if (!name || !*name) return(-1);
		strcpy(op, name);
		op += strlen(name);
		break;
	    case 'L':
		if (!lang || !*lang) return(-1);
		strcpy(op, lang);
		op += strlen(lang);
		break;
	    /*
	    case 'l':
		strcpy(op, language);
		op += strlen(language);
		break;
	    case 't':
		strcpy(op, terr);
		op += strlen(terr);
		break;
	    case 'c':
		strcpy(op, code);
		op += strlen(code);
		break;
	    case '%':
		strcpy(op, "%");
		op += strlen("%");
		break;
	    */
	    default:
		break;
	    }
	 }else{
	    *op = *ip;
	    op++;
	 }
     }
     *op = '\0';
     return(0);
}


struct msg_cat *
msg_open(name, nlspath, lang)
char *name;
char *nlspath;
char *lang;
{
    struct msg_cat *cd;

    char fn[128];
    FILE *fp;
    char data[1024];
    char save[1024];
    int msg_cnt = 0;
    int msg_byte = 0;
    register char *dp;
    register struct msg_bd *bd;
    register char *msg, *l;

    l = getlang(lang);
    if (name && *name == '/') {
	strcpy(fn, name);
    } else {
	if (expand(fn, nlspath, name, l) == -1) {
	    return(NULL);
	}
    }

    if (!(cd = (struct msg_cat *)malloc(sizeof(struct msg_cat))))
	return(NULL);

    strcpy(cd->name, name);
    strcpy(cd->lang, l);
    strcpy(cd->nlspath, nlspath);
    cd->nextp = NULL;
    cd->msg_cnt = 0;

    if((fp = fopen(fn, "r")) == NULL){
	/* message file not found */
	cd->msg_bd = 0;
	return(cd);
    }
    for( ; ; ){
	/* first: count bytes */
	if(fgets(data, 1024, fp) == NULL)
	    break;
	if(*data == '#')
	    continue;	/* comment */
	for(dp = data ; *dp != '\t'; dp++);	/* msg_id:message\n */
	dp++;
	msg_byte += strlen(dp);
	msg_cnt++;
    }
    rewind(fp);

    cd->msg_cnt = msg_cnt;
    if (!(bd = cd->msg_bd = (struct msg_bd *)
	       malloc((sizeof(struct msg_bd)) * msg_cnt + msg_byte + 1))) {
	fclose(fp);
	free(cd);
	return(NULL);
    }
    msg = (char *) bd + (sizeof(struct msg_bd)) * msg_cnt;

    for( ; ; ){
	/* second : get message */
	if(fgets(data, 1024, fp) == NULL)
	    break;
	if(*data == '#')
	    continue;	/* comment */
	for(dp = data ; *dp != '\t'; dp++);	/* msg_id:message\n */
	*dp = 0;
	dp++;
	bd->msg_id = atoi(data);
	bd->msg = msg;
	bd++;
	_escape(save, dp);
	strcpy(msg, save);
	msg += strlen(save);
	*msg = 0;
	msg++;
    }
    fclose(fp);
    return(cd);
}

char *
msg_get(catd, id, msg, lang)
struct msg_cat *catd;
int	id;
char	*msg;
register char	*lang;
{
    register struct msg_cat *cd;
    char ret[128];
    register char *msg_bd;

    if(catd == 0)
	goto error;
    cd = catd;
    if(lang == 0 || *lang == '\0'){
	lang = cd->lang;
    } else {
	for ( ; ; cd = cd->nextp) {
	    if(strcmp(lang, cd->lang) == 0)
		break;
	    if(cd->nextp == 0) {
		cd->nextp = msg_open(cd->name, cd->nlspath, lang);
		cd = cd->nextp;
		break;
	    }
	}
    }

    if(msg_bd = get_msg_bd(cd, id))
	return(msg_bd);
error:
    if(msg != 0 && *msg != '\0')
	return(msg);
    sprintf(ret, "mes_id = %d: %s", id, DEF_MSG);
    return(ret);
}

void
msg_close(cd)
register struct msg_cat *cd;
{
    if(cd->nextp)
	msg_close(cd->nextp);
    if(cd->msg_bd)
	free(cd->msg_bd);
    if(cd)
	free(cd);
}

#ifdef	not_use
/* test */
main()
{
	struct msg_cat *cd;

	cd = msg_open("msg","%L", "ja_JP");

	printf(msg_get(cd, 5, "message not found\n", "ja_JP"), 555);
	printf(msg_get(cd, 6, "message not found\n", "zh_CN"));
	printf(msg_get(cd, -1, "", "ja_JP"), 555);
	printf(msg_get(cd, 2, "message not found\n", "ja_JP"), "abc");
	printf(msg_get(cd, 100, "message not found\n", "zh_CN"), "abc");
}
#endif	/* not_use */

/*
 * $Id: jikouho.c,v 1.1 2000/01/16 05:07:45 ura Exp $
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
/************************************************
 *	��ʸ�ἡ���������			*
 ************************************************/
#include	<stdio.h>
#include	<ctype.h>
#include	"commonhd.h"
#include	"de_header.h"
#include	"jdata.h"

#include	"fzk.h"
#include	"kaiseki.h"
#include "wnn_malloc.h"

static struct JKT_SBN *find_jktsbn();
static int cmp_dsd_sbn(), cnt_jkt_sbn(), get_suuji_kouho(),
  get_eisuu_kouho(), get_kigou_kouho();

static int
jkt_sbn(yomi_sno, yomi_eno, endvect, endvect1, tjktsbn, bnst_num, parent)
int	yomi_sno;
int	yomi_eno;
int	endvect;		/* ��ü�٥��� */
int	endvect1;		/* ��ü�٥��� */
struct	JKT_SBN **tjktsbn;	/* ��ʸ����Ϸ�� */
int	bnst_num;
struct	JKT_SBN *parent;	/* �Ƥδ���Ρ��� */
{
    register int	fzkcnt, ii, jktcnt;
    register int	i;
    struct		JKT_SBN		*jktsbn;
    struct		JKT_SBN		*jktsbn_top;
    struct		ICHBNP		*ichbnpbp;
    int		cnt;
    int		gijiflag = 0;

    struct	SYO_BNSETSU	*getsbnsp();
    struct	SYO_BNSETSU	*giji_sbn;

    if (yomi_sno == yomi_eno)
	return(NOTHING);

    jktcnt = 0;
    if (fzk_ckvt(endvect) == NO && fzk_ckvt(endvect1) == NO)
	return (NOTHING);
    fzkcnt = fzk_kai(&bun[yomi_sno], &bun[yomi_eno], endvect, endvect1, &ichbnpbp);
    if (fzkcnt <= 0) 
	return (fzkcnt);	/* ERROR */
    for (ii = 0; ii < fzkcnt; ii++)
	getfzkoh(ichbnpbp, ii)->offset += yomi_sno;

    jktsbn_top = 0;

    for (ii = 0; ii < fzkcnt; ii++) {	/* ������� */
	i = getfzkoh(ichbnpbp, ii)->offset;
	if (jmtp[i] == (struct jdata **) UN_KNOWN) { /* �⤦������? */
	    jmt_set(i);
	}
    }

    if ((giji_sbn = getsbnsp()) == NO)
	return (-1);       /* ERROR */

    giji_sbn->bend_m = yomi_sno;
    if (getgiji(yomi_sno, yomi_eno, giji_sbn, ichbnpbp, fzkcnt, bnst_num) >= 0) {
	if ((giji_sbn->j_c == yomi_eno - 1) &&
	    (giji_sbn->hinsi_fk != katakanago_no &&
	     giji_sbn->hinsi_fk != giji_no &&
	     giji_sbn->hinsi_fk != fuzokugo_no)) {
		if (get_zen_giji(giji_sbn, &jktsbn) < 0)
		    return (-1);
		jktsbn->parent = parent;
		jktsbn->kbcnt = bnst_num;
		jktsbn_top = jktsbn;
		gijiflag = 1;
		jktcnt = 1;
	}
    }
    freesbn(giji_sbn);

    if ((cnt = jkt_sbn_one(yomi_sno, yomi_eno, &jktsbn_top, bnst_num, parent,
			   ichbnpbp, fzkcnt)) < 0)
	return (-1);
    jktcnt += cnt;

#ifndef	NO_KANA
    if (gijiflag == 0) { /* �Ҥ餬�ʸ��� */
	if ((jktsbn = get_hira_kouho(&jktsbn_top, yomi_sno, yomi_eno))
	     < (struct JKT_SBN *)0)
	    return (-1);
	if (jktsbn > (struct JKT_SBN *)0) {
	    jktsbn->parent = parent;	
	    jktsbn->bend_m = yomi_sno;	
	    jktsbn->kbcnt = bnst_num;
	    jktcnt++;
	}
    }
#endif	/* NO_KANA */

    *tjktsbn = jktsbn_top;
    freeibsp(ichbnpbp);
    return (jktcnt);
}

int
jkt_sbn_one(yomi_sno, yomi_eno, jktsbn_top, bnst_num, parent, ichbnpbp, fzkcnt)
int	yomi_sno;
int	yomi_eno;
struct	JKT_SBN	**jktsbn_top;
int	bnst_num;		/* ����ޤǤ˲��Ϥ�����ʸ��� */
struct	JKT_SBN *parent;	/* �Ƥδ���Ρ��� */
struct	ICHBNP	*ichbnpbp;
register int	fzkcnt;
{
    register int	ii, hyokaval, hyokaval0, hinsidt, jktcnt;
	     int	hinsidt_fk;	/* ʣ���ʻ�Υ����� */
    unsigned short	*hinsi_buf;
    struct	jdata	*jentptr;
    register int	i,		/* ����ν����Υ���ǥå��� */
			t;
    struct		JKT_SBN		*jktsbn;
    register struct	JKT_SONE	*jktsone;
    int		ll;
    struct		fzkkouho	*fzkptr;

    int			connect_flg;

    jktcnt = 0;

    for (ii = 0; ii < fzkcnt; ii++) { /* ��°�� */
	fzkptr = getfzkoh(ichbnpbp, ii);
	i = fzkptr->offset;
	jentptr = (i > yomi_eno - 1) ?
		(struct jdata *) NULL : (struct jdata *) C(i, yomi_eno - 1);
	for (; jentptr; jentptr = jentptr->jptr) { /* ���� */
	    for (t = 0; t < (jentptr->kosuu); t++) { /* ���� */
#ifdef	CONVERT_with_SiSheng
		hyokaval = hyoka1(jentptr->hindo[t],
			(jentptr->hindo_in == 0 ? 0 : jentptr->hindo_in[t]),
			diff_sisheng(jentptr->sisheng[t],jentptr->sisheng_int),
			yomi_eno - yomi_sno/2 - i/2,
			dic_table[jentptr->jishono].nice);
#else
		hyokaval = hyoka1(jentptr->hindo[t],
			(jentptr->hindo_in == 0 ? 0 : jentptr->hindo_in[t]),
			yomi_eno - i, yomi_eno - yomi_sno,
			dic_table[jentptr->jishono].nice);
#endif	/* CONVERT_with_SiSheng */
		    hinsidt_fk = hinsidt = jentptr->hinsi[t];
		    if ((ll = wnn_get_fukugou_component_body(hinsidt_fk, &hinsi_buf)) == 0) {
			error1("wnn_get_fukugou_component:erroe in jkt_sbn_one.\n");
			return (-1);
		    }
		    for ( ; ll > 0; ll--) {
			hinsidt = *hinsi_buf;
			hinsi_buf++;
			/* ����������ñ���ʻ�����ͤ���Ф��� */
			connect_flg = kan_ck_vector(hinsidt, fzkptr->vector);
			if (
			    !(jentptr->hindo_in && (jentptr->hindo_in[t]&0x7f) == 0x7f) &&
			    ((jentptr->hindo[t]&0x7f) != 0x7f) &&
			    /*�����ȥ����Ȥ��줿���񥨥�ȥ� */
			    (connect_flg != NO ||
			     (bnst_num == 1 && kan_ck_vector(hinsidt,
				    getfzkoh1(ichbnpbp, ii)->vector) == WNN_CONNECT_BK))) {

			    if (bnst_num == 1)
				/* endvect ����³�Ǥ��ʤ���Τ�ɾ���ͤ򲼤��� */
				hyokaval0 = (connect_flg != NO) ?
					hyokaval: _DIVID(hyokaval, 2);
			    else if (parent != 0) {
				/* ����ʸ�᤬ endvect ����³�Ǥ��ʤ���Τϡ�
				   ɾ���ͤ򲼤��� */
				struct JKT_SONE *tmp;
				hyokaval0 = _DIVID(hyokaval, 2);
				for (tmp = parent->sbn; tmp != 0;
				       tmp = tmp->lnk_br) {
				    if (tmp->status_bkwd == YES) {
					/* �¤���³�Ǥ��� */
					hyokaval0 = hyokaval;
					break;
				    }
				}
			    } else
				hyokaval0 = hyokaval;

			    if ((jktsone = getjktsone()) == NO)
				    return (-1);	/* ERROR */
			    jktsone->i_jc = i;
			    jktsone->v_jc = hyokaval0;
			    jktsone->jentptr = jentptr;
			    jktsone->t_jc = t;
			    jktsone->hinsi_fk = hinsidt_fk;
			    jktsone->status_bkwd = connect_flg;
			    if ((jktsbn =
				find_jktsbn(jktsbn_top, jktsone, hinsidt, yomi_eno - 1)) < (struct JKT_SBN *)0)
				    return (-1);
			    if (jktsbn > (struct JKT_SBN *)0) {
				    jktsbn->parent = parent;	
				    jktsbn->bend_m = yomi_sno;	
				    jktsbn->kbcnt = bnst_num;
				    jktcnt++;
			    }
			} /* ��³�Ǥ��� */
		    } /* ñ���ʻ� */
		} /* ���� */
	    } /* ���� */
#if	!defined(NO_KANA) && !defined(KOREAN)
	    /* �������ʸ��� */
	    if ((jktsbn = get_kata_kouho(jktsbn_top, yomi_sno, yomi_eno, i,
		fzkptr->vector, getfzkoh1(ichbnpbp, ii)->vector, ii)) < (struct JKT_SBN *)0)
		    return (-1);
	    if (jktsbn > (struct JKT_SBN *)0) {
		    jktsbn->parent = parent;	
		    jktsbn->bend_m = yomi_sno;	
		    jktsbn->kbcnt = bnst_num;
		    jktcnt++;
	    }
#endif	/* !defined(NO_KANA) && !defined(KOREAN) */
	} /* ��°�� */
    return (jktcnt);
}

#if	!defined(NO_KANA) && !defined(KOREAN)
/* �������ʸ��� */
struct JKT_SBN	*
get_kata_kouho(jktsbn_top, yomi_sno, yomi_eno, i_jc, vector, vector1, hyouka)
struct	JKT_SBN **jktsbn_top;
int	yomi_sno,
	yomi_eno,
	i_jc;
int	*vector;
int	*vector1;
int	hyouka;
{
    struct	JKT_SONE	*jktsone;
    int		connect_flg;

    if (!isjhira(bun[i_jc]) && !BAR_CODE(bun[i_jc]) &&
	(i_jc + 1 == yomi_eno || !isjhira(bun[i_jc+1])))
	return ((struct JKT_SBN *) 0);

    if (i_jc != yomi_eno && !(touten(bun[i_jc])) && !(kuten(bun[i_jc]))) {
	if (kan_ck_vector(giji_no, vector) == WNN_CONNECT_BK) {
	    connect_flg = WNN_CONNECT_BK;
	} else if (kan_ck_vector(giji_no, vector1) == WNN_NOT_CONNECT_BK) {
	    return ((struct JKT_SBN *) 0);
	} else {
	    connect_flg = WNN_NOT_CONNECT_BK;
	}
	if ((jktsone = getjktsone()) == 0)
	    return ((struct JKT_SBN *) 0);
	jktsone->i_jc = i_jc;
	jktsone->jentptr = 0;
	jktsone->t_jc = WNN_KATAKANA; /* �������� */
	jktsone->v_jc = (yomi_eno - i_jc ) * KATA_HYOUKA;
	jktsone->hinsi_fk = giji_no;
	jktsone->status_bkwd = connect_flg;
	return (find_jktsbn(jktsbn_top, jktsone, giji_no, yomi_eno - 1));
    } else {
	return ((struct JKT_SBN *) 0);
    }
}
#endif /* !defined(NO_KANA) && !defined(KOREAN) */

struct JKT_SBN	*
get_hira_kouho(jktsbn_top, yomi_sno, yomi_eno)
struct	JKT_SBN **jktsbn_top;
int	yomi_sno,
	yomi_eno;
{
    struct	JKT_SONE	*jktsone;

    if ((jktsone = getjktsone()) == 0)
	    return ((struct JKT_SBN *)-1);
    jktsone->i_jc = yomi_sno;
    jktsone->jentptr = 0;
    jktsone->t_jc = WNN_HIRAGANA;	/* �Ҥ餬�ʸ��� */
#ifdef KOREAN
    jktsone->hinsi_fk = katakanago_no;  /* Hangul */
#else
    jktsone->hinsi_fk = giji_no;
#endif
    jktsone->v_jc = (yomi_eno - yomi_sno) * HIRA_HYOUKA;
    jktsone->status_bkwd = WNN_NOT_CONNECT_BK;
    return (find_jktsbn(jktsbn_top, jktsone, giji_no, yomi_eno - 1));
}

static struct JKT_SBN *
find_jktsbn(jktsbn, jktsone, hinsi, j_c)
struct	JKT_SBN		**jktsbn;
struct	JKT_SONE	*jktsone;
int			hinsi;
int			j_c;
{
    register struct	JKT_SBN		*sbn;
    struct		JKT_SBN		*tsbn;
    register struct	JKT_SONE	*sone;
    int	v_jc;
    int	kangovect;

    kangovect = ft->kango_hinsi_area[hinsi];
    sbn = *jktsbn;
    if (sbn == 0 || sbn->j_c != j_c || sbn->kangovect > kangovect) {
	if ((sbn = getjktsbn()) == NO)
	    return ((struct JKT_SBN *)-1);
	sbn->lnk_br = *jktsbn;	
	*jktsbn = sbn;
	sbn->j_c = j_c;
	sbn->sbn = jktsone;
	sbn->kangovect = kangovect;
	return(sbn);
    }
    while (sbn != 0 && sbn->j_c == j_c) {
	if (sbn->kangovect == kangovect) {
	    /* ɾ���ͤ� SORT */
	    v_jc = jktsone->v_jc;
	    sone = sbn->sbn;
	    if (sone == 0 || sone->v_jc < v_jc) {
		sbn->sbn = jktsone;
		jktsone->lnk_br = sone;
		return (0);
	    }
	    while(sone) {
		if (sone->lnk_br == 0 || sone->lnk_br->v_jc < v_jc) {
		    jktsone->lnk_br = sone->lnk_br;
		    sone->lnk_br = jktsone;
		    return (0);
		}
		sone = sone->lnk_br;
	    }
	} else if (sbn->lnk_br == 0 || sbn->lnk_br->j_c != j_c ||
		   sbn->lnk_br->kangovect > kangovect) {
	    if ((tsbn = getjktsbn()) == NO)
		return ((struct JKT_SBN *)-1);
	    tsbn->lnk_br = sbn->lnk_br;
	    sbn->lnk_br = tsbn;
	    tsbn->sbn = jktsone;
	    tsbn->kangovect = kangovect;
	    tsbn->j_c = j_c;
	    return(tsbn);
	}
	sbn = sbn->lnk_br;
    }
    return(NULL);
}

/**************************************************
 * jkt_get_syo : ������ all ������ ��ʸ��
 **************************************************/
int
#ifdef	NO_FZK
jkt_get_syo(yomi_sno, yomi_eno, beginvect, endvect, endvect1, dsd_sbn)
#else
jkt_get_syo(yomi_sno, yomi_eno, beginvect, fzkchar, endvect, endvect1, dsd_sbn)
#endif	/* NO_FZK */
int	yomi_sno;
int	yomi_eno;
int	beginvect;		/* ��ü�٥���(-1:ʸ����Ƭ��-2:�ʤ�Ǥ�)�ʻ�No.*/
#ifndef	NO_FZK
w_char	*fzkchar;
#endif	/* NO_FZK */
int	endvect;		/* ��ü�٥��� */
int	endvect1;		/* ��ü�٥��� */
struct	DSD_SBN	**dsd_sbn;
{
    struct	DSD_SBN	*d_sbn;
    struct	JKT_SBN *tjktsbn = 0;	/* ��ʸ����Ϸ�� */
    register int	cnt = 0;	/* ������� */
    register struct	JKT_SBN		*jktsbn;
    struct	JKT_SBN			*next_sbn;
    register struct	JKT_SONE	*sone_p;
    int	sbn_cnt;
    int			divid;

    if(c_env->fzk_fid == -1){
	wnn_errorno = WNN_FZK_FILE_NO_LOAD;
	return(-1);
    }

    ft = (struct FT *)files[c_env->fzk_fid].area;

    if (jkt_sbn(yomi_sno, yomi_eno, endvect, endvect1, &tjktsbn, 1, 
			  (struct JKT_SBN *)0) < 0) {
	init_work_areas();
	return (-1);
    }

    sbn_cnt = cnt_jkt_sbn(tjktsbn);
    if ((*dsd_sbn = d_sbn = get_dsd_sbn(sbn_cnt)) <= (struct DSD_SBN *)0) {
	init_work_areas();
	return (-1);
    }
    for (jktsbn = tjktsbn; jktsbn != 0; ) {
	next_sbn = jktsbn->lnk_br;
	for (sone_p = jktsbn->sbn; sone_p != 0; sone_p = sone_p->lnk_br) {
	    d_sbn->bun_m = jktsbn->bend_m;
	    d_sbn->bun_jc = jktsbn->j_c;
	    d_sbn->i_jc = sone_p->i_jc;
	    d_sbn->jentptr = sone_p->jentptr;
	    d_sbn->t_jc = sone_p->t_jc;
	    d_sbn->hinsi = sone_p->hinsi_fk;
	    d_sbn->kangovect = jktsbn->kangovect;
	    d_sbn->status_bkwd = sone_p->status_bkwd;
	    divid = get_jkt_status(jktsbn->kangovect, beginvect,
#ifndef	NO_FZK
				   fzkchar,
#endif	/* NO_FZK */
				   &(jktsbn->status));
	    d_sbn->v_jc = DIVID_HYOUKA(sone_p->v_jc, divid);
	    if (sone_p->jentptr == 0 && jktsbn->status == WNN_NOT_CONNECT)
		d_sbn->status = WNN_GIJI;
	    else
		d_sbn->status = jktsbn->status;
	    cnt++;
	    d_sbn++;
	}
	freejktsbn(jktsbn);
	jktsbn = next_sbn;
    }
    qsort((char *) *dsd_sbn, cnt, sizeof(struct DSD_SBN), cmp_dsd_sbn);
    return (cnt);
}

static int
cmp_dsd_sbn(sbn1, sbn2)
struct	DSD_SBN	*sbn1;
struct	DSD_SBN	*sbn2;
{
    return (sbn2->v_jc - sbn1->v_jc);
}

static int
cnt_jkt_sbn(sbn)
register struct JKT_SBN *sbn;
{
    register int    cnt;
    cnt = 0;
    while(sbn) {
	cnt += cnt_sone(sbn->sbn);
	sbn = sbn->lnk_br;
    }
    return (cnt);
}

int
get_zen_giji(giji_sbn, jktsbn)
struct SYO_BNSETSU *giji_sbn;
struct	JKT_SBN	**jktsbn;
{
    int	hinsi;
    hinsi = giji_sbn->hinsi_fk;

    if ((*jktsbn = getjktsbn()) == NULL)
	return (-1);
    if (((*jktsbn)->sbn = getjktsone()) == NULL)
	return (-1);

    (*jktsbn)->j_c = giji_sbn->j_c;
    (*jktsbn)->bend_m = giji_sbn->bend_m;
    (*jktsbn)->kangovect = giji_sbn->kangovect;
    (*jktsbn)->sbn->i_jc = giji_sbn->i_jc;
    (*jktsbn)->sbn->v_jc = giji_sbn->v_jc;
    (*jktsbn)->sbn->jentptr = 0;
    (*jktsbn)->sbn->t_jc = giji_sbn->t_jc;
    (*jktsbn)->sbn->hinsi_fk = giji_sbn->hinsi_fk;
    (*jktsbn)->sbn->status_bkwd = giji_sbn->status_bkwd;

    if ((*jktsbn)->sbn->hinsi_fk == fuzokugo_no)
	return(1);
    if (hinsi == suuji_no)
	return (get_suuji_kouho(*jktsbn));
    if (isascii(bun[(*jktsbn)->sbn->i_jc])) {
	if (hinsi == eisuu_no)
	    return (get_eisuu_kouho(*jktsbn));
	else
	    return (get_kigou_kouho(*jktsbn));
    } else {
	return (1);
    }
}

static struct JKT_SONE *
cp_sone(d_sone, s_sone)
register struct JKT_SONE *d_sone, *s_sone;
{
    if ((d_sone->lnk_br = getjktsone()) == NO)
	return ((struct JKT_SONE *)-1);
    d_sone = d_sone->lnk_br;
    d_sone->i_jc = s_sone->i_jc;
    d_sone->v_jc = s_sone->v_jc - 10; /* ��1������ɾ���ͤ򲼤��� */
    d_sone->jentptr = 0;
    d_sone->hinsi_fk = s_sone->hinsi_fk;
    d_sone->status_bkwd = s_sone->status_bkwd;
    return (d_sone);
}

/* Ⱦ�ѿ��� */
static int
get_suuji_kouho(jktsbn)
register struct	JKT_SBN	*jktsbn;
{
    register struct JKT_SONE *jktsone;
    int	hinsi;
    int	len; 

    hinsi = jktsbn->sbn->t_jc;
    len = jktsbn->j_c - jktsbn->sbn->i_jc;
    if (isspace(bun[jktsbn->j_c]) || isjspace(bun[jktsbn->j_c]))
	len--;

    if ((jktsone = cp_sone(jktsbn->sbn, jktsbn->sbn)) < (struct JKT_SONE *)0)
	return (-1);

    if (hinsi != WNN_NUM_HAN) {
	jktsone->t_jc = WNN_NUM_HAN;	/* Ⱦ�ѿ��� */
	if ((jktsone = cp_sone(jktsone, jktsbn->sbn)) < (struct JKT_SONE *)0)
	    return (-1);
    }
    if (hinsi != WNN_NUM_ZEN) {
	jktsone->t_jc = WNN_NUM_ZEN;	/* ������ */
	if (len < 0 && hinsi == WNN_NUM_KAN)
	    return (3);
	if ((jktsone = cp_sone(jktsone, jktsbn->sbn)) < (struct JKT_SONE *)0)
	    return (-1);
    }
    if (hinsi != WNN_NUM_KAN) {
	jktsone->t_jc = WNN_NUM_KAN;	/* ���� */
	if (len < 0)
	    return (3);
	if ((jktsone = cp_sone(jktsone, jktsbn->sbn)) < (struct JKT_SONE *)0)
	    return (-1);
    }
    if (hinsi != WNN_NUM_KANSUUJI) {
	jktsone->t_jc = WNN_NUM_KANSUUJI;	/* ɴ�󽽻� */
	if (len < 3 && hinsi == WNN_NUM_KANOLD)
	    return (5);
	if ((jktsone = cp_sone(jktsone, jktsbn->sbn)) < (struct JKT_SONE *)0)
	    return (-1);
    }
    if (hinsi != WNN_NUM_KANOLD) {
	jktsone->t_jc = WNN_NUM_KANOLD;	/* ��ɴ������ */
	jktsone->v_jc -= 2;	/* ɾ���ͤ����äȲ����� */
	if (len < 3)
	    return (5);
	if ((jktsone = cp_sone(jktsone, jktsbn->sbn)) < (struct JKT_SONE *)0)
	    return (-1);
    }
    if (hinsi != WNN_NUM_ZENCAN) {
	jktsone->t_jc = WNN_NUM_ZENCAN;	/* ���������� */
	if (hinsi == WNN_NUM_HANCAN)
	    return (7);
	if ((jktsone = cp_sone(jktsone, jktsbn->sbn)) < (struct JKT_SONE *)0)
	    return (-1);
    }
    jktsone->t_jc = WNN_NUM_HANCAN;	/* 1,234 */
    return (7);
}

static int
get_eisuu_kouho(jktsbn)
struct	JKT_SBN	*jktsbn;
{
    struct JKT_SONE	*jktsone;
    if ((jktsone = cp_sone(jktsbn->sbn, jktsbn->sbn)) < (struct JKT_SONE *)0)
	return (-1);

    if (jktsbn->sbn->t_jc == WNN_ALP_ZEN)
	jktsone->t_jc = WNN_ALP_HAN;
    else
	jktsone->t_jc = WNN_ALP_ZEN;
    return (2);
}

static int
get_kigou_kouho(jktsbn)
struct	JKT_SBN	*jktsbn;
{
    struct JKT_SONE	*jktsone;
    if ((jktsone = cp_sone(jktsbn->sbn, jktsbn->sbn)) < (struct JKT_SONE *)0)
	return (-1);

    if (jktsbn->sbn->t_jc == WNN_KIG_JIS)
	jktsone->t_jc = WNN_KIG_HAN;
    else
	jktsone->t_jc = WNN_KIG_JIS;
    return (2);
}
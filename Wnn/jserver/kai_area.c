/*
 * $Id: kai_area.c,v 1.1 2000/01/16 05:07:45 ura Exp $
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
/*********************
 * kaiseki work area
**********************/

#include "commonhd.h"
#include "ddefine.h"

w_char	*bun;	/* D */

w_char giji_eisuu[20];	/* �����ֱѿ��פ���� */

int	maxchg;
int	initjmt;	/* I think initjmt is the length of jmt_ */

int	*maxj;	/* maxj is counts to entries in jmt_ */

/* jmt_ptr is used in jmt0.c only.
   but it must be changed by clients.
   jishobiki does not use it. this is sent to jishobiki by arguments
   */

/* j_e_p is used to hold the current point to which jmtw_ is used */
struct	jdata	*j_e_p;
struct	jdata	**jmt_;
struct	jdata	*jmtw_;
struct	jdata	**jmt_end;
struct	jdata	*jmtw_end;
struct	jdata	**jmt_ptr;
struct	jdata	***jmtp;

struct	FT	*ft;

#ifdef	nodef
/********************************
 *	������Ω����ʻ�	*
 ********************************/
int	sentou_no;	/* ����Ƭ��ʸ����Ƭ��������� */
int	suuji_no;	/* �ֿ����׿������� */
int	katakanago_no;	/* �֥��ʡ׳����ʤ� ̾������ */
int	eisuu_no;	/* �ֱѿ���*/
int	kigou_no;	/* �ֵ���� */
int	toji_kakko_no;	/* ���ĳ�̡� */
int	fuzokugo_no;	/* ��°����� */
int	kai_kakko_no;	/* �ֳ���̡�*/
int	giji_no;	/* ���� */
#endif	/* nodef */
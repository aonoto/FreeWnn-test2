/*
 * $Id: py_table.c,v 1.1 2000/01/16 05:07:45 ura Exp $
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

#include  <ctype.h>
#include "commonhd.h"
#ifdef CHINESE
#include "cplib.h"

/* pyshengmu_tbl[]: ShengMu table of Chinese PinYin */
char 	*py_shengmu_tbl[PY_NUM_SHENGMU] = {
	
	"",  "B", "C",  "Ch", "D",  
	"F", "G", "H",  "J",  "K", 
	"L", "M", "N",  "P",  "Q", 
	"R", "S", "Sh", "T",  "W",
	"X", "Y", "Z",  "Zh" 
	};
	
/* py_yunmu_tbl[]: YunMu table of Chinese ZhuYin */
char 	*py_yunmu_tbl[PY_NUM_YUNMU*5] = {

	"��",    "��",    "��",    "��",    "��",
	"a��",    "����",    "����",    "����",    "����",
	"ai��",   "��i��",   "��i��",   "��i��",   "��i��",
	"an��",   "��n��",   "��n��",   "��n��",   "��n��",
	"ang��",  "��ng��",  "��ng��",  "��ng��",  "��ng��",
	"ao��",   "��o��",   "��o��",   "��o��",   "��o��",
	"e��",    "����",    "����",    "����",    "����",
	"ei��",   "��i��",   "��i��",   "��i��",   "��i��",
	"en��",   "��n��",   "��n��",   "��n��",   "��n��",
	"eng��",  "��ng��",  "��ng��",  "��ng��",  "��ng��",
	"er��",   "��r��",   "��r��",   "��r��",   "��r��",
	"i��",    "����",    "����",    "����",    "����",
	"ia��",   "i����",   "i����",   "i����",   "i����",
	"ian��",  "i��n��",  "i��n��",  "i��n��",  "i��n��",
	"iang��", "i��ng��", "i��ng��", "i��ng��", "i��ng��",
	"iao��",  "i��o��",  "i��o��",  "i��o��",  "i��o��",
	"ie��",   "i����",   "i����",   "i����",   "i����",
	"in��",   "��n��",   "��n��",   "��n��",   "��n��",
	"ing��",  "��ng��",  "��ng��",  "��ng��",  "��ng��",
	"iong��", "i��ng��", "i��ng��", "i��ng��", "i��ng��",
	"iu��",   "i����",   "i����",   "i����",   "i����",
	"m��",    "m��",    "m��",    "m��",    "m��",
	"n��",    "n��",    "����",    "����",    "����",
	"ng��",   "ng��",   "ng��",   "ng��",   "ng��",
	"o��",    "����",    "����",    "����",    "����",
	"ong��",  "��ng��",  "��ng��",  "��ng��",  "��ng��",
	"ou��",   "��u��",   "��u��",   "��u��",   "��u��",
	"u��",    "����",    "����",    "����",    "����",
	"ua��",   "u����",   "u����",   "u����",   "u����",
	"uai��",  "u��i��",  "u��i��",  "u��i��",  "u��i��",
	"uan��",  "u��n��",  "u��n��",  "u��n��",  "u��n��",
	"uang��", "u��ng��", "u��ng��", "u��ng��", "u��ng��",
	"ue��",   "u����",   "u����",   "u����",   "u����",
	"ui��",   "u����",   "u����",   "u����",   "u����",
	"un��",   "��n��",   "��n��",   "��n��",   "��n��",
	"uo��",   "u����",   "u����",   "u����",   "u����",
	"����",    "����",    "����",    "����",    "����",
	"��e��",   "������",   "������",   "������",   "������",
	"0��",   "1��",   "2��",   "3��",   "4��",  /* for undefinited YunMu  */
	};


/* pinyin_tbl:  size is NUM_SHENGMU*NUM_YUNMU, including empty ShengMu */
/*     and empty YunMu , and undefinited YunMu'-' */

int  pinyin_tbl[PY_NUM_SHENGMU*PY_NUM_YUNMU] = {
					    	
  0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,
  0,1,1,1,1,1,0,1,1,1,0,1,0,1,0,1,1,1,1,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,1,
  0,1,1,1,1,1,1,0,1,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,1,0,0,1,1,1,0,0,1,
  0,1,1,1,1,1,1,0,1,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,1,1,1,0,1,1,1,0,0,1,
  0,1,1,1,1,1,1,1,1,1,0,1,1,1,0,1,1,0,1,0,1,0,0,0,0,1,1,1,0,0,1,0,0,1,1,1,0,0,1,
  0,1,0,1,1,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,1,0,0,0,0,0,0,0,0,0,0,1,
  0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,1,1,1,0,0,1,
  0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,1,1,1,1,1,1,1,0,1,1,1,0,0,1,
  0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,1,0,0,1,0,1,0,1,0,0,0,1,
  0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,1,1,1,0,0,1,
  0,1,1,1,1,1,1,1,0,1,0,1,1,1,1,1,1,1,1,0,1,0,0,0,1,1,1,1,0,0,1,0,0,0,1,1,1,1,1,
  0,1,1,1,1,1,1,1,1,1,0,1,0,1,0,1,1,1,1,0,1,0,0,0,1,0,1,1,0,0,0,0,0,0,0,0,0,0,1,
  0,1,1,1,1,1,1,1,1,1,0,1,0,1,1,1,1,1,1,0,1,0,0,0,0,1,1,1,0,0,1,0,0,0,0,1,1,1,1,
  0,1,1,1,1,1,0,1,1,1,0,1,0,1,0,1,1,1,1,0,0,0,0,0,1,0,1,1,0,0,0,0,0,0,0,0,0,0,1,
  0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,1,0,0,1,0,1,0,1,0,0,0,1,
  0,0,0,1,1,1,1,0,1,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,1,0,0,1,1,1,0,0,1,
  0,1,1,1,1,1,1,0,1,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,1,0,0,1,1,1,0,0,1,
  0,1,1,1,1,1,1,1,1,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,1,1,1,0,0,1,
  0,1,1,1,1,1,1,1,0,1,0,1,0,1,0,1,1,0,1,0,0,0,0,0,0,1,1,1,0,0,1,0,0,1,1,1,0,0,1,
  0,1,1,1,1,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,1,
  0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,1,0,0,1,0,1,0,1,0,0,0,1,
  0,1,0,1,1,1,1,0,0,0,0,1,0,0,0,0,0,1,1,0,0,0,0,0,1,1,1,1,0,0,1,0,1,0,1,0,0,0,1,
  0,1,1,1,1,1,1,1,1,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,1,0,0,1,1,1,0,0,1,
  0,1,1,1,1,1,1,1,1,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,1,1,1,0,0,1
	};
#endif /* CHINESE */

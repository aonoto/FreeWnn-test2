;
; $Id: cvt_key_tbl.kt,v 1.2.2.1 1999/02/08 02:54:28 yamasita Exp $
;

; FreeWnn is a network-extensible Kana-to-Kanji conversion system.
; This file is part of FreeWnn.
; 
; Copyright Kyoto University Research Institute for Mathematical Sciences
;                 1987, 1988, 1989, 1990, 1991, 1992
; Copyright OMRON Corporation. 1987, 1988, 1989, 1990, 1991, 1992, 1999
; Copyright ASTEC, Inc. 1987, 1988, 1989, 1990, 1991, 1992
;
; Author: OMRON SOFTWARE Co., Ltd. <freewnn@rd.kyoto.omronsoft.co.jp>
;
; This program is free software; you can redistribute it and/or modify
; it under the terms of the GNU General Public License as published by
; the Free Software Foundation; either version 2, or (at your option)
; any later version.
;
; This program is distributed in the hope that it will be useful,
; but WITHOUT ANY WARRANTY; without even the implied warranty of
; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
; GNU General Public License for more details.
;
; You should have received a copy of the GNU General Public License
; along with GNU Emacs; see the file COPYING.  If not, write to the
; Free Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
;
; Commentary:
;
; Change log:
;
; Last modified date: 8,Feb.1999
;

; key code conversion table for KTERM
;----------------------------------
; ↓、次候補
kcud1	\x92
; ←
kcub1	\x91
; →
kcuf1	\x90
; ↑、前候補
kcuu1	\x93
;
; ひらがな/カタカナ
kf1	\x81
kf25	\x81
; 全角/半角
kf2	\x82
kf26	\x82
; ローマ字
kf3	\x83
kf27	\x83
;
; 辞書操作
kf4	\x84
; ローマ字かな 数字全角
kf5	\x85
; 罫線 細い
kf6	\x86
; 罫線 太い
kf7	\x87
; 未使用 予約 勝手に使うな
kf8	\x88
kf9	\x89
kf10	\x8A
;
; 消去
kf11	\x8B
; 呼出し
kf12	\x8C
; 文節縮め
kf13	\x8D 
; 文節伸ばし
kf14	\x94
;
; 変換
kf15	\x9E
; 確定
kf16	\x9F
;
; 登録
kf17	\x95
; 辞書操作
kf18	\x96
; 区点
kf19	\x97
; ＪＩＳ
kf20	\x98
;
; 行頭
kf21	\x9A
; 消去、逆変換
kf22	\x99
; 行末
kf23	\x9B
; 全候補、解除
kf24	\x9C

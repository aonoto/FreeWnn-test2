/*
 *  $Id: setutmp.c,v 1.7 2006/03/04 19:01:46 aonoto Exp $
 */

/*
 * FreeWnn is a network-extensible Kana-to-Kanji conversion system.
 * This file is part of FreeWnn.
 * 
 * Copyright Kyoto University Research Institute for Mathematical Sciences
 *                 1987, 1988, 1989, 1990, 1991, 1992
 * Copyright OMRON Corporation. 1987, 1988, 1989, 1990, 1991, 1992, 1999
 * Copyright ASTEC, Inc. 1987, 1988, 1989, 1990, 1991, 1992
 * Copyright FreeWnn Project 1999, 2000, 2002, 2006
 *
 * Maintainer:  FreeWnn Project
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <stdio.h>
#if STDC_HEADERS
#  include <string.h>
#else
#  if HAVE_STRINGS_H
#    include <strings.h>
#  endif
#  ifdef HAVE_MEMORY_H
#    include <memory.h>
#  endif
#endif /* STDC_HEADERS */
#include <sys/types.h>
#include <pwd.h>
#if HAVE_FCNTL_H
#  include <fcntl.h>
#endif
#if TIME_WITH_SYS_TIME
#  include <sys/time.h>
#  include <time.h>
#else
#  if HAVE_SYS_TIME_H
#    include <sys/time.h>
#  else
#    include <time.h>
#  endif /* HAVE_SYS_TIME_H */
#endif /* TIME_WITH_SYS_TIME */
#if HAVE_UNISTD_H
#  include <unistd.h>
#endif
#if HAVE_UTMPX_H
#  include <utmpx.h>
#elif HAVE_UTMP_H
#  include <utmp.h>
#else
#  error "No utmp/utmpx header."
#endif /* HAVE_UTMP_X */

#include "commonhd.h"
#include "sdefine.h"
#include "sheader.h"

#define public

#if HAVE_UTMPX_H
static struct utmpx saveut;
static struct utmpx nullut;
#elif HAVE_UTMP_H
static struct utmp saveut;
static struct utmp nullut;
#endif /* HAVE_UTMPX_H */

#ifdef BSD42
static int savslotnum = 0;
static char savttynm[8];
static int suf = 0;

#ifndef _PATH_UTMP
# ifdef UTMP_FILE
# define _PATH_UTMP UTMP_FILE
# else
#  define _PATH_UTMP "/etc/utmp"
# endif
#endif

public int
saveutmp ()
{
  register int utmpFd;
  register char *p;

  if (suf > 0)
    return 0;
  suf = -1;

  bzero (&nullut, sizeof nullut);
  if ((p = ttyname (0)) == NULL)
    return -1;
  strncpy (savttynm, strrchr (p, '/') + 1, 8);
  if (!(savslotnum = ttyslot ()))
    return -1;
  if ((utmpFd = open (_PATH_UTMP, O_RDONLY, 0)) < 0)
    return -1;
  lseek (utmpFd, savslotnum * (sizeof saveut), 0);
  read (utmpFd, &saveut, sizeof saveut);
  close (utmpFd);
  strncpy (nullut.ut_line, saveut.ut_line, 8);
  strncpy (nullut.ut_host, saveut.ut_host, 16);
  nullut.ut_time = saveut.ut_time;
  suf = 1;
  return 0;
}

public int
setutmp (ttyFd)
     int ttyFd;
{
  int utmpFd;
  struct utmp ut;
  char *p;
  int i;

  if (suf <= 0)
    return -1;
  bzero (&ut, sizeof ut);
  if ((p = ttyname (ttyFd)) == NULL)
    return -1;

  if (!strcmp(p, "/dev/"))
    p += 5;
  strncpy (ut.ut_line, p, sizeof (ut.ut_line));
  strncpy (ut.ut_user, getpwuid (getuid ())->pw_name, 8); /* should be sizeof (ut.ut_user) */
  ut.ut_time = time (0);
  strncpy (ut.ut_host, savttynm, 8);
  if (!(i = ttyfdslot (ttyFd)))
    return -1;
  if ((utmpFd = open (_PATH_UTMP, O_RDWR, 0)) < 0)
    return -1;
  lseek (utmpFd, savslotnum * (sizeof nullut), 0);
  write (utmpFd, &nullut, sizeof nullut);
  lseek (utmpFd, i * (sizeof ut), 0);
  write (utmpFd, &ut, sizeof ut);
  close (utmpFd);
  return 0;
}
#endif /* BSD42 */

#ifdef SYSVR2
public int
setutmp (ttyFd)
     int ttyFd;
{
  struct utmp ut;
  char *p;
  struct passwd *getpwuid ();

  memset (&ut, 0, sizeof ut);
  if ((p = ttyname (ttyFd)) == NULL)
    return -1;

  if (!strcmp(p, "/dev/"))
    p += 5;
  strncpy (ut.ut_line, p, sizeof (ut.ut_line));
  strncpy (ut.ut_user, getpwuid (getuid ())->pw_name, 8); /* should be sizeof (ut.ut_user) */
  ut.ut_time = time (0);
#ifdef DGUX
  strncpy (ut.ut_id, &ut.ut_line[3], 4);
#else
  strncpy (ut.ut_id, &ut.ut_line[2], 4);
  ut.ut_id[0] = 't';
#endif /* DGUX */
  ut.ut_pid = getpid ();
  ut.ut_type = USER_PROCESS;
  setutent ();                  /* is it necessary? */
  getutid (&ut);
  pututline (&ut);
  endutent ();
  return 0;
}
#endif /* SYSVR2 */

#ifdef BSD42
public int
resetutmp (ttyFd)
     int ttyFd;
{
  int utmpFd;
  struct utmp ut;
  char *p;
  int i;

  bzero (&ut, sizeof ut);
  if ((p = ttyname (ttyFd)) == NULL)
    return -1;
  strncpy (ut.ut_line, strrchr (p, '/') + 1, 8);
  ut.ut_time = time (0);
  if (!(i = ttyfdslot (ttyFd)))
    return -1;
  if ((utmpFd = open (_PATH_UTMP, O_RDWR, 0)) < 0)
    return -1;
  lseek (utmpFd, savslotnum * (sizeof saveut), 0);
  write (utmpFd, &saveut, sizeof saveut);
  lseek (utmpFd, i * (sizeof ut), 0);
  write (utmpFd, &ut, sizeof ut);
  close (utmpFd);
  return 0;
}
#endif /* BSD42 */

#ifdef SYSVR2
public int
resetutmp (ttyFd)
     int ttyFd;
{
  struct utmp ut;
  char *p;
  struct passwd *getpwuid ();

  memset (&ut, 0, sizeof ut);
  if ((p = ttyname (ttyFd)) == NULL)
    return -1;
  strncpy (ut.ut_line, strrchr (p, '/') + 1, 12);
  strncpy (ut.ut_user, getpwuid (getuid ())->pw_name, 8);
  ut.ut_time = time (0);
#ifdef DGUX
  strncpy (ut.ut_id, &ut.ut_line[3], 4);
#else
  strncpy (ut.ut_id, &ut.ut_line[2], 4);
  ut.ut_id[0] = 't';
#endif /* DGUX */
  ut.ut_pid = getpid ();
  ut.ut_type = DEAD_PROCESS;    /* not sure */
  setutent ();                  /* is it necessary? */
  getutid (&ut);
  pututline (&ut);
  endutent ();
  return 0;
}
#endif /* SYSVR2 */

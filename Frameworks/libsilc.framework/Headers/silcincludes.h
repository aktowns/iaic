/*

  silcincludes.h

  Author: Pekka Riikonen <priikone@silcnet.org>

  Copyright (C) 1997 - 2005 Pekka Riikonen

  The contents of this file are subject to one of the Licenses specified 
  in the COPYING file;  You may not use this file except in compliance 
  with the License.

  The software distributed under the License is distributed on an "AS IS"
  basis, in the hope that it will be useful, but WITHOUT WARRANTY OF ANY
  KIND, either expressed or implied.  See the COPYING file for more
  information.

*/
/*
  This file includes common definitions for SILC. This file MUST be included
  by all files in SILC (directly or through other global include file).
*/

#ifndef SILCINCLUDES_H
#define SILCINCLUDES_H

#ifdef __cplusplus
extern "C" {
#endif

#define SILC_UNIX

#ifdef WIN32
#ifndef SILC_WIN32
#define SILC_WIN32
#undef SILC_UNIX
#endif
#endif

#if defined(__EPOC32__)
#ifndef SILC_EPOC
#define SILC_EPOC
#undef SILC_UNIX
#endif
#endif

#ifdef BEOS
#ifndef SILC_BEOS
#define SILC_BEOS
#undef SILC_UNIX
#endif
#elif defined(__BEOS__)
#ifndef SILC_BEOS
#define SILC_BEOS
#undef SILC_UNIX
#endif
#endif

#if defined(OS2)
#ifndef SILC_OS2
#define SILC_OS2
#undef SILC_UNIX
#endif
#endif

#if defined(__MACH__) && defined(__APPLE__)
#ifndef SILC_MACOSX
#define SILC_MACOSX
#endif
#endif

/* Types */
#define SILC_SIZEOF_LONG_LONG 8
#define SILC_SIZEOF_LONG 4
#define SILC_SIZEOF_INT 4
#define SILC_SIZEOF_SHORT 2
#define SILC_SIZEOF_CHAR 1
#define SILC_SIZEOF_VOID_P 4

/* Compilation time defines, for third-party software */
#define __SILC_HAVE_PTHREAD 1
#define __SILC_HAVE_SIM 1


#if defined(HAVE_SILCDEFS_H)
/* Automatically generated configuration header */
#include "silcdefs.h"
#include "silcdistdefs.h"
#endif /* HAVE_SILCDEFS_H */

/* Platform specific includes */

#if defined(SILC_WIN32)
#include "silcwin32.h"
#endif

#if defined(SILC_EPOC)
#include "silcepoc.h"
#endif

#if defined(SILC_BEOS)
#include "silcbeos.h"
#endif

#if defined(SILC_OS2)
#include "silcos2.h"
#endif

#ifndef DLLAPI
#define DLLAPI
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>

#ifdef HAVE_SIGNAL_H
#include <signal.h>
#endif

#ifdef HAVE_FCNTL_H
#include <fcntl.h>
#endif

#ifdef HAVE_ERRNO_H
#include <errno.h>
#endif

#ifdef HAVE_ASSERT_H
#include <assert.h>
#endif

#if !defined(SILC_WIN32)

#include <unistd.h>
#include <sys/time.h>
#include <pwd.h>
#include <grp.h>
#include <sys/times.h>

#if defined(HAVE_GETOPT_H) && defined(HAVE_GETOPT)
#include <getopt.h>
#else
#if defined(HAVE_SILCDEFS_H)
#include "getopti.h"
#endif /* HAVE_SILCDEFS_H */
#endif

#ifdef SOCKS5
#include "socks.h"
#endif

#include <sys/socket.h>
#ifdef HAVE_NETINET_IN_H
#include <netinet/in.h>
#endif

#ifdef HAVE_XTI_H
#include <xti.h>
#else
#ifdef HAVE_NETINET_TCP_H
#include <netinet/tcp.h>
#endif
#endif

#ifdef HAVE_NETDB_H
#include <netdb.h>
#endif

#ifdef HAVE_ARPA_INET_H
#include <arpa/inet.h>
#endif

#ifdef HAVE_SYS_MMAN_H
#include <sys/mman.h>
#endif

#ifdef HAVE_DLFCN_H
#include <dlfcn.h>
#endif

#ifdef HAVE_LIMITS_H
#include <limits.h>
#endif

#ifndef HAVE_REGEX_H
#if defined(HAVE_SILCDEFS_H)
#include "regexpr.h"
#endif /* HAVE_SILCDEFS_H */
#else
#include <regex.h>
#endif

#ifdef SILC_HAVE_PTHREAD
#include <pthread.h>
#endif

#ifdef HAVE_STDDEF_H
#include <stddef.h>
#endif

#ifdef HAVE_TERMIOS_H
#include <termios.h>
#endif

#ifdef HAVE_UTIME_H
#include <utime.h>
#endif

#ifdef HAVE_ICONV_H
#include <iconv.h>
#endif

#ifdef HAVE_LOCALE_H
#include <locale.h>
#endif

#ifdef HAVE_LANGINFO_H
#include <langinfo.h>
#endif

#ifdef HAVE_SYS_RESOURCE_H
#include <sys/resource.h>
#endif

#endif				/* !SILC_WIN32 */

/* Include generic SILC type definitions */
#include "silctypes.h"
#include "silcversion.h"

/* Math library includes */
#include "silcmp.h"
#include "silcmath.h"

/* SILC util library includes */
#include "silcmemory.h"
#include "silcbuffer.h"
#include "silcbuffmt.h"

/* Crypto library includes */
#include "silccipher.h"
#include "silchash.h"
#include "silchmac.h"
#include "silcrng.h"
#include "silcpkcs.h"
#include "silcpkcs1.h"

/* More SILC util library includes */
#include "silcmutex.h"
#include "silcthread.h"
#include "silcschedule.h"
#include "silchashtable.h"
#include "silclog.h"
#include "silclist.h"
#include "silcdlist.h"
#include "silcnet.h"
#include "silcfileutil.h"
#include "silcstrutil.h"
#include "silcutf8.h"
#include "silcstringprep.h"
#include "silcutil.h"
#include "silcconfig.h"
#include "silcprotocol.h"
#include "silcsockconn.h"
#include "silcvcard.h"
#include "silcapputil.h"

/* SILC core library includes */
#include "silcargument.h"
#include "silcstatus.h"
#include "silcid.h"
#include "silcidcache.h"
#include "silccommand.h"
#include "silcmessage.h"
#include "silcchannel.h"
#include "silcpacket.h"
#include "silcnotify.h"
#include "silcmode.h"
#include "silcauth.h"
#include "silcattrs.h"
#include "silcvcard.h"

#if defined(SILC_SIM)
/* SILC Module library includes */
#include "silcsim.h"
#include "silcsimutil.h"
#endif

/* SILC Key Exchange library includes */
#include "silcske.h"
#include "silcske_payload.h"
#include "silcske_groups.h"

/* SILC SFTP library */
#include "silcsftp.h"
#include "silcsftp_fs.h"

#ifdef __cplusplus
}
#endif

#endif /* SILCINCLUDES_H */

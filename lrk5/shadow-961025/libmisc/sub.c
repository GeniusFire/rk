/*
 * Copyright 1989 - 1991, John F. Haugh II
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by John F. Haugh, II
 *      and other contributors.
 * 4. Neither the name of John F. Haugh, II nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY JOHN HAUGH AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL JOHN HAUGH OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <config.h>

#include "rcsid.h"
RCSID("$Id: sub.c,v 1.2 1996/09/20 09:08:03 marekm Exp $")

#include <sys/types.h>
#include "defines.h"

#include <pwd.h>

#define	BAD_SUBROOT	"Invalid root directory \"%s\"\n"
#define	BAD_SUBROOT2	"invalid root `%s' for user `%s'\n"
#define	NO_SUBROOT	"Can't change root directory to \"%s\"\n"
#define	NO_SUBROOT2	"no subsystem root `%s' for user `%s'\n"

/*
 * subsystem - change to subsystem root
 *
 *	A subsystem login is indicated by the presense of a "*" as
 *	the first character of the login shell.  The given home
 *	directory will be used as the root of a new filesystem which
 *	the user is actually logged into.
 */

void
subsystem(pw)
	const struct passwd *pw;
{
	/*
	 * The new root directory must begin with a "/" character.
	 */

	if (pw->pw_dir[0] != '/') {
		printf (BAD_SUBROOT, pw->pw_dir);
		SYSLOG((LOG_WARN, BAD_SUBROOT2, pw->pw_dir, pw->pw_name));
		closelog();
		exit (1);
	}

	/*
	 * The directory must be accessible and the current process
	 * must be able to change into it.
	 */

	if (chdir (pw->pw_dir) || chroot (pw->pw_dir)) {
		printf (NO_SUBROOT, pw->pw_dir);
		SYSLOG((LOG_WARN, NO_SUBROOT2, pw->pw_dir, pw->pw_name));
		closelog();
		exit (1);
	}
}

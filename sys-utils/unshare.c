/*
 * unshare(1) - command-line interface for unshare(2)
 *
 * Copyright (C) 2009 Mikhail Gusarov <dottedmag@dottedmag.net>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2, or (at your option) any
 * later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <errno.h>
#include <getopt.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mount.h>

/* we only need some defines missing in sys/mount.h, no libmount linkage */
#include <libmount.h>

#include "nls.h"
#include "c.h"
#include "closestream.h"
#include "namespace.h"
#include "exec_shell.h"
#include "xalloc.h"
#include "pathnames.h"
#include "all-io.h"

/* 'private' is kernel default */
#define UNSHARE_PROPAGATION_DEFAULT	(MS_REC | MS_PRIVATE)

enum {
	SETGROUPS_NONE = -1,
	SETGROUPS_DENY = 0,
	SETGROUPS_ALLOW = 1,
};

static const char *setgroups_strings[] =
{
	[SETGROUPS_DENY] = "deny",
	[SETGROUPS_ALLOW] = "allow"
};

static int setgroups_str2id(const char *str)
{
	size_t i;

	for (i = 0; i < ARRAY_SIZE(setgroups_strings); i++)
		if (strcmp(str, setgroups_strings[i]) == 0)
			return i;

	errx(EXIT_FAILURE, _("unsupported --setgroups argument '%s'"), str);
}

static void setgroups_control(int action)
{
	const char *file = _PATH_PROC_SETGROUPS;
	const char *cmd;
	int fd;

	if (action < 0 || (size_t) action >= ARRAY_SIZE(setgroups_strings))
		return;
	cmd = setgroups_strings[action];

	fd = open(file, O_WRONLY);
	if (fd < 0) {
		if (errno == ENOENT)
			return;
		 err(EXIT_FAILURE, _("cannot open %s"), file);
	}

	if (write_all(fd, cmd, strlen(cmd)))
		err(EXIT_FAILURE, _("write failed %s"), file);
	close(fd);
}

static void map_id(const char *file, uint32_t from, uint32_t to)
{
	char *buf;
	int fd;

	fd = open(file, O_WRONLY);
	if (fd < 0)
		 err(EXIT_FAILURE, _("cannot open %s"), file);

	xasprintf(&buf, "%u %u 1", from, to);
	if (write_all(fd, buf, strlen(buf)))
		err(EXIT_FAILURE, _("write failed %s"), file);
	free(buf);
	close(fd);
}

static unsigned long parse_propagation(const char *str)
{
	size_t i;
	static const struct prop_opts {
		const char *name;
		unsigned long flag;
	} opts[] = {
		{ "slave",	MS_REC | MS_SLAVE },
		{ "private",	MS_REC | MS_PRIVATE },
		{ "shared",     MS_REC | MS_SHARED },
		{ "unchanged",        0 }
	};

	for (i = 0; i < ARRAY_SIZE(opts); i++) {
		if (strcmp(opts[i].name, str) == 0)
			return opts[i].flag;
	}

	errx(EXIT_FAILURE, _("unsupported propagation mode: %s"), str);
}

static void set_propagation(unsigned long flags)
{
	if (flags == 0)
		return;

	if (mount("none", "/", NULL, flags, NULL) != 0)
		err(EXIT_FAILURE, _("cannot change root filesystem propagation"));
}

static void usage(int status)
{
	FILE *out = status == EXIT_SUCCESS ? stdout : stderr;

	fputs(USAGE_HEADER, out);
	fprintf(out, _(" %s [options] <program> [<argument>...]\n"),
		program_invocation_short_name);

	fputs(USAGE_SEPARATOR, out);
	fputs(_("Run a program with some namespaces unshared from the parent.\n"), out);

	fputs(USAGE_OPTIONS, out);
	fputs(_(" -m, --mount               unshare mounts namespace\n"), out);
	fputs(_(" -u, --uts                 unshare UTS namespace (hostname etc)\n"), out);
	fputs(_(" -i, --ipc                 unshare System V IPC namespace\n"), out);
	fputs(_(" -n, --net                 unshare network namespace\n"), out);
	fputs(_(" -p, --pid                 unshare pid namespace\n"), out);
	fputs(_(" -U, --user                unshare user namespace\n"), out);
	fputs(_(" -f, --fork                fork before launching <program>\n"), out);
	fputs(_("     --mount-proc[=<dir>]  mount proc filesystem first (implies --mount)\n"), out);
	fputs(_(" -r, --map-root-user       map current user to root (implies --user)\n"), out);
	fputs(_("     --propagation <slave|shared|private|unchanged>\n"
	        "                           modify mount propagation in mount namespace\n"), out);
	fputs(_(" -s, --setgroups allow|deny  control the setgroups syscall in user namespaces\n"), out);

	fputs(USAGE_SEPARATOR, out);
	fputs(USAGE_HELP, out);
	fputs(USAGE_VERSION, out);
	fprintf(out, USAGE_MAN_TAIL("unshare(1)"));

	exit(status);
}

int main(int argc, char *argv[])
{
	enum {
		OPT_MOUNTPROC = CHAR_MAX + 1,
		OPT_PROPAGATION,
		OPT_SETGROUPS
	};
	static const struct option longopts[] = {
		{ "help", no_argument, 0, 'h' },
		{ "version", no_argument, 0, 'V'},
		{ "mount", no_argument, 0, 'm' },
		{ "uts", no_argument, 0, 'u' },
		{ "ipc", no_argument, 0, 'i' },
		{ "net", no_argument, 0, 'n' },
		{ "pid", no_argument, 0, 'p' },
		{ "user", no_argument, 0, 'U' },
		{ "fork", no_argument, 0, 'f' },
		{ "mount-proc", optional_argument, 0, OPT_MOUNTPROC },
		{ "map-root-user", no_argument, 0, 'r' },
		{ "propagation", required_argument, 0, OPT_PROPAGATION },
		{ "setgroups", required_argument, 0, OPT_SETGROUPS },
		{ NULL, 0, 0, 0 }
	};

	int setgrpcmd = SETGROUPS_NONE;
	int unshare_flags = 0;
	int c, forkit = 0, maproot = 0;
	const char *procmnt = NULL;
	unsigned long propagation = UNSHARE_PROPAGATION_DEFAULT;
	uid_t real_euid = geteuid();
	gid_t real_egid = getegid();;

	setlocale(LC_ALL, "");
	bindtextdomain(PACKAGE, LOCALEDIR);
	textdomain(PACKAGE);
	atexit(close_stdout);

	while ((c = getopt_long(argc, argv, "+fhVmuinpUr", longopts, NULL)) != -1) {
		switch (c) {
		case 'f':
			forkit = 1;
			break;
		case 'h':
			usage(EXIT_SUCCESS);
		case 'V':
			printf(UTIL_LINUX_VERSION);
			return EXIT_SUCCESS;
		case 'm':
			unshare_flags |= CLONE_NEWNS;
			break;
		case 'u':
			unshare_flags |= CLONE_NEWUTS;
			break;
		case 'i':
			unshare_flags |= CLONE_NEWIPC;
			break;
		case 'n':
			unshare_flags |= CLONE_NEWNET;
			break;
		case 'p':
			unshare_flags |= CLONE_NEWPID;
			break;
		case 'U':
			unshare_flags |= CLONE_NEWUSER;
			break;
		case OPT_MOUNTPROC:
			unshare_flags |= CLONE_NEWNS;
			procmnt = optarg ? optarg : "/proc";
			break;
		case 'r':
			unshare_flags |= CLONE_NEWUSER;
			maproot = 1;
			break;
		case OPT_SETGROUPS:
			setgrpcmd = setgroups_str2id(optarg);
			break;
		case OPT_PROPAGATION:
			propagation = parse_propagation(optarg);
			break;
		default:
			usage(EXIT_FAILURE);
		}
	}

	if (-1 == unshare(unshare_flags))
		err(EXIT_FAILURE, _("unshare failed"));

	if (forkit) {
		int status;
		pid_t pid = fork();

		switch(pid) {
		case -1:
			err(EXIT_FAILURE, _("fork failed"));
		case 0:	/* child */
			break;
		default: /* parent */
			if (waitpid(pid, &status, 0) == -1)
				err(EXIT_FAILURE, _("waitpid failed"));
			if (WIFEXITED(status))
				return WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				kill(getpid(), WTERMSIG(status));
			err(EXIT_FAILURE, _("child exit failed"));
		}
	}

	if (maproot) {
		if (setgrpcmd == SETGROUPS_ALLOW)
			errx(EXIT_FAILURE, _("options --setgroups=allow and "
					"--map-root-user are mutually exclusive"));

		/* since Linux 3.19 unprivileged writing of /proc/self/gid_map
		 * has s been disabled unless /proc/self/setgroups is written
		 * first to permanently disable the ability to call setgroups
		 * in that user namespace. */
		setgroups_control(SETGROUPS_DENY);
		map_id(_PATH_PROC_UIDMAP, 0, real_euid);
		map_id(_PATH_PROC_GIDMAP, 0, real_egid);

	} else if (setgrpcmd != SETGROUPS_NONE)
		setgroups_control(setgrpcmd);

	if ((unshare_flags & CLONE_NEWNS) && propagation)
		set_propagation(propagation);

	if (procmnt &&
	    (mount("none", procmnt, NULL, MS_PRIVATE|MS_REC, NULL) != 0 ||
	     mount("proc", procmnt, "proc", MS_NOSUID|MS_NOEXEC|MS_NODEV, NULL) != 0))
			err(EXIT_FAILURE, _("mount %s failed"), procmnt);

	if (optind < argc) {
		execvp(argv[optind], argv + optind);
		err(EXIT_FAILURE, _("failed to execute %s"), argv[optind]);
	}
	exec_shell();
}

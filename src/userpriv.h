/*
spacenavd - a free software replacement driver for 6dof space-mice.
Copyright (C) 2007-2010 John Tsiombikas <nuclear@member.fsf.org>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef USERPRIV_H_
#define USERPRIV_H_

#include <sys/types.h>

/* struct for user id's */
typedef struct {
    uid_t spnavd_daemon_runas_uid;      /* the uid for the daemon */
    gid_t spnavd_daemon_runas_gid;      /* the gid for the daemon */
    uid_t spnavd_invoked_as_uid;        /* spnavd was started with this uid */
    gid_t spnavd_invoked_as_gid;        /* spnavd was started with this gid */
    int   spnavd_can_restore_uid;       /* spnavd can restore the invoked uid */
    int   spnavd_can_restore_gid;       /* spnavd can restore the invoked gid */
    int   spnavd_runas_daemon;          /* flag for running in daemonmode */
} userid_struct;

void set_initial_user_privileges(userid_struct *userids);
void test_initial_user_privileges(userid_struct *userids);
int set_runas_uid(userid_struct *userids, char *runas_lname);
int set_runas_gid(userid_struct *userids, char *runas_gname);
void start_daemon_privileges(userid_struct *userids);
void stop_daemon_privileges(userid_struct *userids);

#endif	/* USERPRIV_H_ */

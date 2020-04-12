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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include "userpriv.h"

void set_initial_user_privileges(userid_struct *userids)
{
    if(userids != NULL) {
        /* get the effective uid and effective gid for the initial user
         * who started spnavd */
        userids->spnavd_invoked_as_uid = geteuid();
        userids->spnavd_invoked_as_gid = getegid();
        
        /* set the "runas" effective uid and gid to an invalid startvalue */
        userids->spnavd_daemon_runas_uid = -1;
        userids->spnavd_daemon_runas_gid = -1;
        
        /* default assumption: we can not change effective uid / gid */
        userids->spnavd_can_restore_uid = 0;
        userids->spnavd_can_restore_gid = 0;
    }
}

void test_initial_user_privileges(userid_struct *userids)
{
    /* default assumption: we can not change effective uid / gid */
    userids->spnavd_can_restore_uid = 0;
    userids->spnavd_can_restore_gid = 0;

    if(userids != NULL) {
        /* check the effective uid change */
        if(userids->spnavd_daemon_runas_uid != -1) {
            if (userids->spnavd_daemon_runas_uid != userids->spnavd_invoked_as_uid) {
                /* only run, if daemon uid differ from invoked uid */
                if(seteuid(userids->spnavd_daemon_runas_uid) == 0)
                {
                    /* succeded to get lower privileges
                     * -> restore uid */
                    if(seteuid(userids->spnavd_invoked_as_uid) == 0)
                    {
                        userids->spnavd_can_restore_uid = 1;
                    }
                }
            }
        }

        /* check the effective gid change */
        if(userids->spnavd_daemon_runas_gid != -1)
        {
            if (userids->spnavd_daemon_runas_gid != userids->spnavd_invoked_as_gid) {
                /* only run, if daemon gid differ from invoked gid */
                if(seteuid(userids->spnavd_daemon_runas_gid) == 0)
                {
                    /* succeded to get lower privileges
                     * -> restore uid */
                    if(seteuid(userids->spnavd_invoked_as_gid) == 0)
                    {
                        userids->spnavd_can_restore_gid = 1;
                    }
                }
            }
        }
    }
}

int set_runas_uid(userid_struct *userids, char *runas_lname)
{
    struct passwd *userinfo;
    userinfo = getpwnam(runas_lname);
    
    if(userids == NULL || userinfo == NULL) {
        /* error - but no distinction */
        return 0;
    } else {
        /* set the uid */
        userids->spnavd_daemon_runas_uid = userinfo->pw_uid;
    }
    
    return 1;
}

int set_runas_gid(userid_struct *userids, char *runas_gname)
{
    struct group *groupinfo;
    groupinfo = getgrnam(runas_gname);

    if(userids == NULL || groupinfo == NULL) {
        /* error - but no distinction */
        return 0;
    } else {
        /* set the gid */
        userids->spnavd_daemon_runas_gid = groupinfo->gr_gid;
    }
    
    return 1;
}

void start_daemon_privileges(userid_struct *userids)
{
    if(userids != NULL) {
        if (userids->spnavd_runas_daemon == 1) {
            if(userids->spnavd_can_restore_uid)
            {
                seteuid(userids->spnavd_daemon_runas_uid);
            }
            if(userids->spnavd_can_restore_gid) {
                setegid(userids->spnavd_daemon_runas_gid);
            }
        }
    }
}

void stop_daemon_privileges(userid_struct *userids)
{
    if(userids != NULL) {
        if (userids->spnavd_runas_daemon == 1) {
            if(userids->spnavd_can_restore_uid) {
                seteuid(userids->spnavd_invoked_as_uid);
            }
            if(userids->spnavd_can_restore_gid) {
                setegid(userids->spnavd_invoked_as_gid);
            }
        }
    }
}

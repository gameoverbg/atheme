/*
 * Copyright (c) 2005 William Pitcock, et al.
 * Rights to this code are as documented in doc/LICENSE.
 *
 * ping spammer thingy
 *
 */

#include "atheme.h"

DECLARE_MODULE_V1
(
	"operserv/pingspam", false, _modinit, _moddeinit,
	PACKAGE_STRING,
	"Atheme Development Group <http://www.atheme.org>"
);

char *notices[] =
{
	"Scanning for proxies.",
	"Killing off bottlers.",
	"LOL ok so like we are teh SKANZ0RZING j00 becuz well like OMG deze bots r h3r3 an liek they are FL00DING!!#@! ignore plz",
	"gaben",
	"Please ignore this notice.",
	"Scanning for warez.",
	"All your pr0n are belong to us!",
	"Move over! This is the police!",
	"This notice brought to you by Burma-Shave.",
	"They're coming...",
	":)(::)(::)(::)(::)(::)(::)(::)(::)(::)(::)(::)(::)(::)(::)(::)(::)(::)(::)(::)(::)(:",
	"lolz!",
	"<Hikaru> your a pagan",
	"* Ads needs to shower soon",
	"<Hik`Coding> Don't make me get Yakuza on you",
	"beu fails it",
	"BAN KAI~!$"
};

char *phrases[] =
{
	"",
	" please-ignore",
	" proxy scan",
	" ignore me",
	" <3 neostats",
};

void pingspam(user_t *u);
static void user_add_hook(hook_user_nick_t *data);
static void os_cmd_pingspam(sourceinfo_t *si, int parc, char *parv[]);
static void os_cmd_autopingspam(sourceinfo_t *si, int parc, char *parv[]);

command_t os_pingspam = { "PINGSPAM", "Spam a user with pings from every service, plus some bonus notices.", PRIV_OMODE, 1, os_cmd_pingspam };
command_t os_autopingspam = { "AUTOPINGSPAM", "Spam connecting users with pings from every service, plus some bonus notices (setting).", PRIV_ADMIN, 1, os_cmd_autopingspam };

int spamming;
list_t *os_cmdtree;
list_t *os_helptree;

void _modinit(module_t *m)
{
	spamming = 0;

	MODULE_USE_SYMBOL(os_cmdtree, "operserv/main", "os_cmdtree");
	MODULE_USE_SYMBOL(os_helptree, "operserv/main", "os_helptree");
	command_add(&os_pingspam, os_cmdtree);
	command_add(&os_autopingspam, os_cmdtree);

	help_addentry(os_helptree, "PINGSPAM", "help/contrib/pingspam", NULL);
	help_addentry(os_helptree, "AUTOPINGSPAM", "help/contrib/autopingspam", NULL);

	hook_add_event("user_add");
	hook_add_user_add(user_add_hook);
}

void _moddeinit()
{
	command_delete(&os_pingspam, os_cmdtree);
	command_delete(&os_autopingspam, os_cmdtree);
	help_delentry(os_helptree, "PINGSPAM");
	help_delentry(os_helptree, "AUTOPINGSPAM");
	hook_del_user_add(user_add_hook);
}

static void user_add_hook(hook_user_nick_t *data)
{
	user_t *u;

	u = data->u;
	if (u == NULL)
		return;
	if (is_internal_client(u))
		return;
	if (spamming)
		pingspam(u);
}

static void os_cmd_pingspam(sourceinfo_t *si, int parc, char *parv[])
{
	char *target = parv[0];
	user_t *u;

	if(!target)
	{
		notice(opersvs.nick, si->su->nick, "Usage: \2PINGSPAM\2 <target>");
		return;
	}

	if(!(u = user_find_named(target)))
	{
		notice(opersvs.nick, si->su->nick, "\2%s\2 is not on the network", target);
		return;
	}

	pingspam(u);
	notice(opersvs.nick, si->su->nick, "\2%s\2 has been pwned.", target);
	logcommand(si, CMDLOG_ADMIN, "PINGSPAM: \2%s\2", target);
}

static void os_cmd_autopingspam(sourceinfo_t *si, int parc, char *parv[])
{
	char *mode = parv[0];

	if(!mode)
	{
		notice(opersvs.nick, si->su->nick, "Auto-pingspam is currently \2%s\2", spamming ? "ON" : "OFF");
		return;
	}

	if(strcasecmp(mode, "on") == 0 || atoi(mode))
	{
		spamming = 1;
		notice(opersvs.nick, si->su->nick, "Auto-pingspam is now \2ON\2");
	}else{
		spamming = 0;
		notice(opersvs.nick, si->su->nick, "Auto-pingspam is now \2OFF\2");
	}
}

void pingspam(user_t *u)
{
	user_t *sptr;
	node_t *n;
	int i;

	if(*globsvs.nick)
		for(i = 0;i < 6;i++)
			notice(globsvs.nick, u->nick, "%s", notices[rand() % sizeof(notices) / sizeof(char*)]);

	LIST_FOREACH(n, me.me->userlist.head)
	{
		sptr = n->data;
		msg(sptr->nick, u->nick, "\001PING %ld%s\001",
				time(NULL),
				phrases[rand() % sizeof(phrases) / sizeof(char*)]
		   );
	}
}

/* vim:cinoptions=>s,e0,n0,f0,{0,}0,^0,=s,ps,t0,c3,+s,(2s,us,)20,*30,gs,hs
 * vim:ts=8
 * vim:sw=8
 * vim:noexpandtab
 */

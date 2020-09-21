/* dwm (dynamic window manager) configuration by: Benjamin Brady, 2020 */
/* See LICENSE file for copyright and license details. */

/* appearance */
static unsigned int borderpx        = 3;  /* border pixel of windows */
static unsigned int snap            = 32; /* snap pixel */
static const int swallowfloating    = 0;  /* 1 means always swallow floating */
static const unsigned int gappih    = 10; /* horiz inner gap between windows */
static const unsigned int gappiv    = 10; /* vert inner gap between windows */
static const unsigned int gappoh    = 10; /* horiz outer gap between windows */
static const unsigned int gappov    = 10; /* vert outer gap between windows */
static const unsigned int smartgaps = 0;  /* 1 means no outer gap for only one window */
static int showbar                  = 1;  /* 0 means no bar */
static int topbar                   = 1;  /* 0 means bottom bar */
static const char *fonts[]          = { "monospace:size=10" };
static const char dmenufont[]       = "monospace:size=10";
static char normbgcolor[]           = "#222222";
static char normbordercolor[]       = "#444444";
static char normfgcolor[]           = "#bbbbbb";
static char selfgcolor[]            = "#eeeeee";
static char selbordercolor[]        = "#005577";
static char selbgcolor[]            = "#005577";
static char *colors[][3] = {
       /*               fg           bg           border   */
       [SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
       [SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor  },
};

typedef struct {
	const char *name;
	const void *cmd;
} Sp;
const char *spcmd1[] = {"st", "-n", "spterm", "-g", "120x34", NULL };
const char *spcmd2[] = {"st", "-n", "spcalc", "-f", "monospace:size=16", "-g", "50x20", "e", "bc", "-lq", NULL };
static Sp scratchpads[] = {
	/* name       cmd */
	{"spterm",    spcmd1},
	{"spranger",  spcmd2},
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };
static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class     instance  title           tags mask  isfloating  isterminal  noswallow  monitor */
	{ "Gimp",    NULL,     NULL,           0,         1,          0,           0,        -1 },
	{ "Firefox", NULL,     NULL,           1 << 8,    0,          0,          -1,        -1 },
	{ "St",      NULL,     NULL,           0,         0,          1,           0,        -1 },
	{ NULL,      NULL,     "Event Tester", 0,         0,          0,           1,        -1 }, /* xev */
	{ NULL,      "spterm", NULL,           SPTAG(0),  1,          0,           0,        -1 },
	{ NULL,      "spfm",   NULL,           SPTAG(1),  1,          0,           0,        -1 },
};

/* layout(s) */
static float mfact     = 0.50; /* factor of master area size [0.05..0.95] */
static int nmaster     = 1;    /* number of clients in master area */
static int resizehints = 1;    /* 1 means use size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behaviour */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },
#define STACKKEYS(MOD,ACTION) \
	{ MOD, XK_j,     ACTION##stack, {.i = INC(+1) } }, \
	{ MOD, XK_k,     ACTION##stack, {.i = INC(-1) } }, \
	/* { MOD, XK_grave, ACTION##stack, {.i = PREVSEL } }, \ */
	/* { MOD, XK_a,     ACTION##stack, {.i = 1 } }, \ */
	/* { MOD, XK_z,     ACTION##stack, {.i = 2 } }, \ */
	/* { MOD, XK_x,     ACTION##stack, {.i = -1 } }, */

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbordercolor, "-sf", selfgcolor, NULL };
static const char *termcmd[]  = { "st", NULL };

/* Xresources preferences to load at startup */
ResourcePref resources[] = {
		{ "normbgcolor",        STRING,  &normbgcolor },
		{ "normbordercolor",    STRING,  &normbordercolor },
		{ "normfgcolor",        STRING,  &normfgcolor },
		{ "selbgcolor",         STRING,  &selbgcolor },
		{ "selbordercolor",     STRING,  &selbordercolor },
		{ "selfgcolor",         STRING,  &selfgcolor },
		{ "borderpx",           INTEGER, &borderpx },
		{ "snap",               INTEGER, &snap },
		{ "showbar",            INTEGER, &showbar },
		{ "topbar",             INTEGER, &topbar },
		{ "nmaster",            INTEGER, &nmaster },
		{ "resizehints",        INTEGER, &resizehints },
		{ "mfact",              FLOAT,   &mfact },
};

#include <X11/XF86keysym.h>
static Key keys[] = {
	/* modifier                     key        function        argument */
        { MODKEY,                       XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY|ShiftMask,             XK_Return, togglescratch,  {.ui = 0 } },
	{ MODKEY,                       XK_BackSpace, spawn,       SHCMD("slock") },
	{ MODKEY|ShiftMask,             XK_BackSpace, spawn,       SHCMD("sysact") },
	{ MODKEY|ShiftMask|ControlMask, XK_BackSpace, quit,        {0} },
	{ MODKEY,                       XK_equal,  view,           {0} },
	{ MODKEY,                       XK_d,      spawn,          {.v = dmenucmd } },
        { MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
        { MODKEY,                       XK_q,      killclient,     {0} },
        { MODKEY|ShiftMask,             XK_q,      spawn,          SHCMD("sysact") },
	{ MODKEY|ShiftMask|ControlMask, XK_q,      quit,           {0} },
        { MODKEY,                       XK_w,      spawn,          SHCMD("$BROWSER") },
        { MODKEY|ShiftMask,             XK_w,      spawn,          SHCMD("st -e sudo nmtui") },
        /* { MODKEY,                       XK_e,      spawn,          SHCMD("") }, */
        /* { MODKEY|ShiftMask,             XK_e,      spawn,          SHCMD("") }, */
	/* { MODKEY,                       XK_r,      spawn,          SHCMD("") }, */
	/* { MODKEY|ShiftMask,             XK_r,      spawn,          SHCMD("") }, */
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY|ShiftMask,             XK_t,      spawn,          SHCMD("st -e htop") },
	{ MODKEY,                       XK_y,      incrohgaps,     {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_y,      incrohgaps,     {.i = -1 } },
	{ MODKEY,                       XK_u,      incrovgaps,     {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_u,      incrovgaps,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incrivgaps,     {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_i,      incrivgaps,     {.i = -1 } },
	{ MODKEY,                       XK_o,      incrihgaps,     {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_o,      incrihgaps,     {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_p,      spawn,          SHCMD("mpc toggle") },
	{ MODKEY,                       XK_a,      togglegaps,     {0} },
	{ MODKEY|ShiftMask,             XK_a,      defaultgaps,    {0} },
	{ MODKEY,                       XK_s,      togglesticky,   {0} },
	/* { MODKEY|ShiftMask,             XK_s,      spawn,          SHCMD("") }, */
	/* { MODKEY|ShiftMask,             XK_d,      spawn,          SHCMD("") }, */
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY|ShiftMask,             XK_f,      fullscreen,     {0} },
	{ MODKEY,                       XK_g,      togglegaps,     {0} },
	{ MODKEY|ShiftMask,             XK_g,      defaultgaps,    {0} },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05 } },
	/* { MODKEY|ShiftMask,             XK_h,      spawn,          SHCMD("")} }, */
	/* { MODKEY|ShiftMask,             XK_j,      spawn,          SHCMD("") }, */
	/* { MODKEY|ShiftMask,             XK_k,      spawn,          SHCMD("") }, */
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05 } },
	{ MODKEY|ShiftMask,             XK_l,      spawn,          SHCMD("") },
	{ MODKEY,                       XK_z,      incrgaps,       {.i = +3 } },
	/* { MODKEY|ShiftMask,             XK_z,      spawn,          SHCMD("") }, */
	{ MODKEY,                       XK_x,      incrgaps,       {.i = -3 } },
	/* { MODKEY|ShiftMask,             XK_x,      spawn,          SHCMD("") }, */
	{ MODKEY,                       XK_c,      togglescratch,  {.ui = 1 } },
	{ MODKEY|ShiftMask,             XK_c,      spawn,          SHCMD("st -e bc -lq") },
	{ MODKEY,                       XK_v,      spawn,          SHCMD("st -e vim") },
	/* { MODKEY|ShiftMask,             XK_v,      spawn,          SHCMD("") }, */
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	/* { MODKEY|ShiftMask,             XK_b,      spawn,          SHCMD("") }, */
	{ MODKEY,                       XK_n,      spawn,          SHCMD("st -e rss") },
	/* { MODKEY|ShiftMask,             XK_n,      spawn,          SHCMD("") }, */
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY|ShiftMask,             XK_m,      spawn,          SHCMD("mpc pause ; pauseallmpv") },
        { MODKEY|ControlMask,           XK_m,      focusmaster,    {0} },
	{ MODKEY,                       XK_bracketleft, spawn,     SHCMD("mpc seek -10") },
	{ MODKEY|ShiftMask,             XK_bracketleft, spawn,     SHCMD("mpc seek -60") },
	{ MODKEY,                       XK_bracketright, spawn,    SHCMD("mpc seek +10") },
	{ MODKEY|ShiftMask,             XK_bracketright, spawn,    SHCMD("mpc seek +60") },
	{ MODKEY,                       XK_comma,  spawn,          SHCMD("mpc prev") },
	{ MODKEY|ShiftMask,             XK_comma,  spawn,          SHCMD("mpc seek 0%") },
	{ MODKEY,                       XK_period, spawn,          SHCMD("mpc next") },
	{ MODKEY|ShiftMask,             XK_period, spawn,          SHCMD("mpc repeat") },
	{ 0,                            XK_Print,  spawn,          SHCMD("maim screenshot-$(date '+%y%m%d-%H%M-%S').png") },
	{ MODKEY,                       XK_Insert, spawn,          SHCMD("notify-send \"Clipboard contents:\" \"$(xclip -o -selection clipboard)\"") },
	{ MODKEY,                       XK_F1,     spawn,          SHCMD("notify-send \"Help Menu:\" \"You made the system, master! You should know already, of course. Super+Enter yields a terminal for a start.\"") },
	STACKKEYS(MODKEY,                          focus)
	STACKKEYS(MODKEY|ShiftMask,                push)
	TAGKEYS(                        XK_1,      0)
	TAGKEYS(                        XK_2,      1)
	TAGKEYS(                        XK_3,      2)
	TAGKEYS(                        XK_4,      3)
	TAGKEYS(                        XK_5,      4)
	TAGKEYS(                        XK_6,      5)
	TAGKEYS(                        XK_7,      6)
	TAGKEYS(                        XK_8,      7)
	TAGKEYS(                        XK_9,      8)
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ 0, XF86XK_AudioMute,		spawn,     SHCMD("pamixer -t") },
	{ 0, XF86XK_AudioRaiseVolume,	spawn,     SHCMD("pamixer --allow-boost -i 3") },
	{ 0, XF86XK_AudioLowerVolume,	spawn,     SHCMD("pamixer --allow-boost -d 3") },
	{ 0, XF86XK_AudioMicMute,       spawn,     SHCMD("slock") },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button1,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

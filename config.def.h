/* See LICENSE file for copyright and license details. */

#define SESSION_FILE "/tmp/dwm-session"

/* appearance */
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static const unsigned int gappih    = 20;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 20;       /* vert inner gap between windows */
static const unsigned int gappoh    = 20;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 20;       /* vert outer gap between windows and screen edge */
static       int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "monospace:size=10" };
static const char dmenufont[]       = "monospace:size=10";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static const char col1[]            = "#ffffff";
static const char col2[]            = "#ffffff";
static const char col3[]            = "#ffffff";
static const char col4[]            = "#ffffff";
static const char col5[]            = "#ffffff";
static const char col6[]            = "#ffffff";

enum { SchemeNorm, SchemeCol1, SchemeCol2, SchemeCol3, SchemeCol4,
       SchemeCol5, SchemeCol6, SchemeSel }; /* color schemes */

static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm]  = { col_gray3, col_gray1, col_gray2 },
	[SchemeCol1]  = { col1,      col_gray1, col_gray2 },
	[SchemeCol2]  = { col2,      col_gray1, col_gray2 },
	[SchemeCol3]  = { col3,      col_gray1, col_gray2 },
	[SchemeCol4]  = { col4,      col_gray1, col_gray2 },
	[SchemeCol5]  = { col5,      col_gray1, col_gray2 },
	[SchemeCol6]  = { col6,      col_gray1, col_gray2 },
	[SchemeSel]   = { col_gray4, col_cyan,  col_cyan  },
};

static const char *const autostart[] = {
  /* Startup */
	"sh", "/home/bzenz/.screenlayout/default.sh", NULL,
  "/usr/bin/mpv", "--no-video", "/home/bzenz/Music/Sounds/startup.mp3", NULL,
  "/usr/local/bin/sigdwmblocks", "2", NULL, // Update volume display
  /* Daemons */
	"/usr/bin/dunst", NULL,
	"/usr/local/bin/dwmblocks", NULL,
	"/usr/bin/nitrogen", "--restore", NULL,
	"/usr/bin/picom", "--experimental-backend", NULL,
  /* Terminate */
	NULL 
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
	{ "Alacritty",NULL,    NULL,           0,         0,          1,           0,        -1 },
	{ NULL,      NULL,     "Event Tester", 0,         0,          0,           1,        -1 }, /* xev */
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

#define FORCE_VSPLIT 0  /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "[M]",      monocle },
	{ "[@]",      spiral },
	{ "[\\]",     dwindle },
	{ "H[]",      deck },
	{ "TTT",      bstack },
	{ "===",      bstackhoriz },
	{ "HHH",      grid },
	{ "###",      nrowgrid },
	{ "---",      horizgrid },
	{ ":::",      gaplessgrid },
	{ "|M|",      centeredmaster },
	{ ">M>",      centeredfloatingmaster },
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ NULL,       NULL },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(CHAIN,KEY,TAG) \
	{ MODKEY,                       CHAIN,    KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           CHAIN,    KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             CHAIN,    KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, CHAIN,    KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { "st", NULL };
static const char *suspendcmd[]  = { "systemctl", "suspend", NULL };
static const char *shutdowncmd[]  = { "shutdown", "now", NULL };
static const char *volupcmd[]  = { "sh", "-c", "pamixer -i 10 && sigdwmblocks 2", NULL };
static const char *voldowncmd[]  = { "sh", "-c", "pamixer -d 10 && sigdwmblocks 2", NULL };
static const char *mutecmd[]  = { "sh", "-c", "pamixer -t && sigdwmblocks 2", NULL };
static const char *nocmd[]  = { "notify-send", "Not bound", NULL };
static const char *screenshotfullcmd[]  = { "spectacle", "--fullscreen", NULL };
static const char *screenshotregioncmd[]  = { "spectacle", "--region", NULL };

static Key keys[] = {
	/* modifier                     chain key   key        function        argument */
	{ MODKEY,                       -1,         XK_Return, zoom,           {0} },
	{ MODKEY|ShiftMask,             -1,         XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       -1,         XK_Tab,    view,           {0} },
	//{ MODKEY,                       -1,         XK_Right,  incnmaster,     {.i = +1 } },
	//{ MODKEY,                       -1,         XK_Left,   incnmaster,     {.i = -1 } },
	{ MODKEY,                       -1,         XK_Left,   setmfact,       {.f = -0.05} },
	{ MODKEY,                       -1,         XK_Right,  setmfact,       {.f = +0.05} },
	{ MODKEY,                       -1,         XK_Up, spawn, {.v = volupcmd } },
	{ MODKEY,                       -1,         XK_Down, spawn, {.v = voldowncmd } },
	{ MODKEY,                       -1,         XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             -1,         XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       -1,         XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       -1,         XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             -1,         XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             -1,         XK_period, tagmon,         {.i = +1 } },
	//{ MODKEY|ControlMask,           -1,         XK_Return, spawn,          {.v = nocmd } },
	//{ MODKEY|ShiftMask,             -1,         XK_Return, spawn,          {.v = nocmd } },
	//{ MODKEY|ControlMask|ShiftMask, -1,         XK_Return, spawn,          {.v = nocmd } },
	//{ MODKEY,                       -1,         XK_a,      spawn,          {.v = nocmd } },
	//{ MODKEY|ControlMask,           -1,         XK_a,      spawn,          {.v = nocmd } },
	//{ MODKEY|ShiftMask,             -1,         XK_a,      spawn,          {.v = nocmd } },
	//{ MODKEY|ControlMask|ShiftMask, -1,         XK_a,      spawn,          {.v = nocmd } },
	//{ MODKEY,                       -1,         XK_b,      spawn,          {.v = nocmd } },
	//{ MODKEY|ControlMask,           -1,         XK_b,      spawn,          {.v = nocmd } },
	//{ MODKEY|ShiftMask,             -1,         XK_b,      spawn,          {.v = nocmd } },
	//{ MODKEY|ControlMask|ShiftMask, -1,         XK_b,      spawn,          {.v = nocmd } },
	//{ MODKEY,                       -1,         XK_c,      spawn,          {.v = nocmd } },
	//{ MODKEY|ControlMask,           -1,         XK_c,      spawn,          {.v = nocmd } },
	{ MODKEY|ShiftMask,             -1,         XK_c,      killclient,     {0} },
	//{ MODKEY|ControlMask|ShiftMask, -1,         XK_c,      spawn,          {.v = nocmd } },
	{ MODKEY,                       -1,         XK_d,      spawn,          {.v = dmenucmd } },
	//{ MODKEY|ControlMask,           -1,         XK_d,      spawn,          {.v = nocmd } },
	//{ MODKEY|ShiftMask,             -1,         XK_d,      spawn,          {.v = nocmd } },
	//{ MODKEY|ControlMask|ShiftMask, -1,         XK_d,      spawn,          {.v = nocmd } },
	//{ MODKEY,                       -1,         XK_e,      spawn,          {.v = nocmd } },
	//{ MODKEY|ControlMask,           -1,         XK_e,      spawn,          {.v = nocmd } },
	//{ MODKEY|ShiftMask,             -1,         XK_e,      spawn,          {.v = nocmd } },
	//{ MODKEY|ControlMask|ShiftMask, -1,         XK_e,      spawn,          {.v = nocmd } },
	//{ MODKEY,                       -1,         XK_f,      spawn,          {.v = nocmd } },
	//{ MODKEY|ControlMask,           -1,         XK_f,      spawn,          {.v = nocmd } },
	//{ MODKEY|ShiftMask,             -1,         XK_f,      spawn,          {.v = nocmd } },
	//{ MODKEY|ControlMask|ShiftMask, -1,         XK_f,      spawn,          {.v = nocmd } },
	//{ MODKEY,                       -1,         XK_g,      spawn,          {.v = nocmd } },
	//{ MODKEY|ControlMask,           -1,         XK_g,      spawn,          {.v = nocmd } },
	//{ MODKEY|ShiftMask,             -1,         XK_g,      spawn,          {.v = nocmd } },
	//{ MODKEY|ControlMask|ShiftMask, -1,         XK_g,      spawn,          {.v = nocmd } },
	//{ MODKEY,                       -1,         XK_h,      spawn,          {.v = nocmd } },
	//{ MODKEY|ControlMask,           -1,         XK_h,      spawn,          {.v = nocmd } },
	//{ MODKEY|ShiftMask,             -1,         XK_h,      spawn,          {.v = nocmd } },
	//{ MODKEY|ControlMask|ShiftMask, -1,         XK_h,      spawn,          {.v = nocmd } },
	//{ MODKEY,                       -1,         XK_i,      spawn,          {.v = nocmd } },
	//{ MODKEY|ControlMask,           -1,         XK_i,      spawn,          {.v = nocmd } },
	//{ MODKEY|ShiftMask,             -1,         XK_i,      spawn,          {.v = nocmd } },
	//{ MODKEY|ControlMask|ShiftMask, -1,         XK_i,      spawn,          {.v = nocmd } },
	{ MODKEY,                       -1,         XK_j,       focusstack,    {.i = +1 } },
	//{ MODKEY|ControlMask,           -1,         XK_j,      spawn,          {.v = nocmd } },
	//{ MODKEY|ShiftMask,             -1,         XK_j,      spawn,          {.v = nocmd } },
	//{ MODKEY|ControlMask|ShiftMask, -1,         XK_j,      spawn,          {.v = nocmd } },
	{ MODKEY,                       -1,         XK_k,      focusstack,     {.i = -1 } },
	//{ MODKEY,                       -1,         XK_k,      spawn,          {.v = nocmd } },
	//{ MODKEY|ControlMask,           -1,         XK_k,      spawn,          {.v = nocmd } },
	//{ MODKEY|ShiftMask,             -1,         XK_k,      spawn,          {.v = nocmd } },
	//{ MODKEY|ControlMask|ShiftMask, -1,         XK_k,      spawn,          {.v = nocmd } },
	{ MODKEY,                       XK_l,       XK_t,      setlayout,      {.v = &layouts[ 0]} }, // tile
	{ MODKEY,                       XK_l,       XK_m,      setlayout,      {.v = &layouts[ 1]} }, // monocle
	{ MODKEY,                       XK_l,       XK_s,      setlayout,      {.v = &layouts[ 2]} }, // spiral
	{ MODKEY,                       XK_l,       XK_w,      setlayout,      {.v = &layouts[ 3]} }, // dwindle
	{ MODKEY,                       XK_l,       XK_d,      setlayout,      {.v = &layouts[ 4]} }, // deck
	//{ MODKEY,                       XK_l,       XK_t,      setlayout,      {.v = &layouts[ 5]} }, // bstack
	//{ MODKEY,                       XK_l,       XK_t,      setlayout,      {.v = &layouts[ 6]} }, // bstackhoriz
	{ MODKEY,                       XK_l,       XK_g,      setlayout,      {.v = &layouts[ 7]} }, // grid
	//{ MODKEY,                       XK_l,       XK_t,      setlayout,      {.v = &layouts[ 8]} }, // nrowgrid
	{ MODKEY,                       XK_l,       XK_h,      setlayout,      {.v = &layouts[ 9]} }, // horizgrid
	//{ MODKEY,                       XK_l,       XK_t,      setlayout,      {.v = &layouts[10]} }, // gaplessgrid
	{ MODKEY,                       XK_l,       XK_c,      setlayout,      {.v = &layouts[11]} }, // centeredmaster
	//{ MODKEY,                       XK_l,       XK_t,      setlayout,      {.v = &layouts[12]} }, // centeredfloatingmaster
	{ MODKEY,                       XK_l,       XK_f,      setlayout,      {0} }, // floating
	//{ MODKEY,                       -1,         XK_m,      spawn,          {.v = nocmd } },     
	//{ MODKEY|ControlMask,           -1,         XK_m,      spawn,          {.v = nocmd } },     
	//{ MODKEY|ShiftMask,             -1,         XK_m,      spawn,          {.v = nocmd } },     
	//{ MODKEY|ControlMask|ShiftMask, -1,         XK_m,      spawn,          {.v = nocmd } },     
	//{ MODKEY,                       -1,         XK_n,      spawn,          {.v = nocmd } },     
	//{ MODKEY|ControlMask,           -1,         XK_n,      spawn,          {.v = nocmd } },     
	//{ MODKEY|ShiftMask,             -1,         XK_n,      spawn,          {.v = nocmd } },     
	//{ MODKEY|ControlMask|ShiftMask, -1,         XK_n,      spawn,          {.v = nocmd } },     
	//{ MODKEY,                       -1,         XK_o,      spawn,          {.v = nocmd } },     
	//{ MODKEY|ControlMask,           -1,         XK_o,      spawn,          {.v = nocmd } },     
	//{ MODKEY|ShiftMask,             -1,         XK_o,      spawn,          {.v = nocmd } },     
	//{ MODKEY|ControlMask|ShiftMask, -1,         XK_o,      spawn,          {.v = nocmd } },     
	//{ MODKEY,                       -1,         XK_p,      spawn,          {.v = nocmd } },     
	//{ MODKEY|ControlMask,           -1,         XK_p,      spawn,          {.v = nocmd } },
	//{ MODKEY|ShiftMask,             -1,         XK_p,      spawn,          {.v = nocmd } },
	//{ MODKEY|ControlMask|ShiftMask, -1,         XK_p,      spawn,          {.v = nocmd } },
	//{ MODKEY,                       -1,         XK_q,      spawn,          {.v = nocmd } },
	//{ MODKEY|ControlMask,           -1,         XK_q,      spawn,          {.v = nocmd } },
	{ MODKEY|ShiftMask,             XK_q,         XK_q,      quit,          {0} }, // logout
	{ MODKEY|ShiftMask,             XK_q,         XK_p,      spawn,          {.v = shutdowncmd} }, // power off
	{ MODKEY|ShiftMask,             XK_q,         XK_s,      spawn,          {.v = suspendcmd} }, // suspend
	//{ MODKEY|ControlMask|ShiftMask, -1,         XK_q,      spawn,          {.v = nocmd } },
	//{ MODKEY,                       -1,         XK_r,      spawn,          {.v = nocmd } },
	//{ MODKEY|ControlMask,           -1,         XK_r,      spawn,          {.v = nocmd } },
	{ MODKEY|ShiftMask,             -1,         XK_r,      quit,          {1} }, // restart dwm
	//{ MODKEY|ControlMask|ShiftMask, -1,         XK_r,      spawn,          {.v = nocmd } },
	//{ MODKEY,                       -1,         XK_s,      spawn,          {.v = nocmd } },
	{ MODKEY|ControlMask,           -1,         XK_s,      spawn,          {.v = screenshotfullcmd } },
	{ MODKEY|ShiftMask,             -1,         XK_s,      spawn,          {.v = screenshotregioncmd } },
	//{ MODKEY|ControlMask|ShiftMask, -1,         XK_s,      spawn,          {.v = nocmd } },
	{ MODKEY,                       XK_t,       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_t,       XK_f,      togglefloating, {0} },
	{ MODKEY,                       XK_t,       XK_g,      togglegaps,     {0} },
	{ MODKEY,                       XK_t,       XK_m,      spawn,     {.v = mutecmd} },
	//{ MODKEY,                       -1,         XK_u,      spawn,          {.v = nocmd } },
	//{ MODKEY|ControlMask,           -1,         XK_u,      spawn,          {.v = nocmd } },
	//{ MODKEY|ShiftMask,             -1,         XK_u,      spawn,          {.v = nocmd } },
	//{ MODKEY|ControlMask|ShiftMask, -1,         XK_u,      spawn,          {.v = nocmd } },
	//{ MODKEY,                       -1,         XK_v,      spawn,          {.v = nocmd } },
	//{ MODKEY|ControlMask,           -1,         XK_v,      spawn,          {.v = nocmd } },
	//{ MODKEY|ShiftMask,             -1,         XK_v,      spawn,          {.v = nocmd } },
	//{ MODKEY|ControlMask|ShiftMask, -1,         XK_v,      spawn,          {.v = nocmd } },
	//{ MODKEY,                       -1,         XK_w,      spawn,          {.v = nocmd } },
	//{ MODKEY|ControlMask,           -1,         XK_w,      spawn,          {.v = nocmd } },
	//{ MODKEY|ShiftMask,             -1,         XK_w,      spawn,          {.v = nocmd } },
	//{ MODKEY|ControlMask|ShiftMask, -1,         XK_w,      spawn,          {.v = nocmd } },
	//{ MODKEY,                       -1,         XK_x,      spawn,          {.v = nocmd } },
	//{ MODKEY|ControlMask,           -1,         XK_x,      spawn,          {.v = nocmd } },
	//{ MODKEY|ShiftMask,             -1,         XK_x,      spawn,          {.v = nocmd } },
	//{ MODKEY|ControlMask|ShiftMask, -1,         XK_x,      spawn,          {.v = nocmd } },
	//{ MODKEY,                       -1,         XK_y,      spawn,          {.v = nocmd } },
	//{ MODKEY|ControlMask,           -1,         XK_y,      spawn,          {.v = nocmd } },
	//{ MODKEY|ShiftMask,             -1,         XK_y,      spawn,          {.v = nocmd } },
	//{ MODKEY|ControlMask|ShiftMask, -1,         XK_y,      spawn,          {.v = nocmd } },
	//{ MODKEY,                       -1,         XK_z,      spawn,          {.v = nocmd } },
	//{ MODKEY|ControlMask,           -1,         XK_z,      spawn,          {.v = nocmd } },
	//{ MODKEY|ShiftMask,             -1,         XK_z,      spawn,          {.v = nocmd } },
	//{ MODKEY|ControlMask|ShiftMask, -1,         XK_z,      spawn,          {.v = nocmd } },
	TAGKEYS(                        -1,           XK_1,                      0)
	TAGKEYS(                        -1,           XK_2,                      1)
	TAGKEYS(                        -1,           XK_3,                      2)
	TAGKEYS(                        -1,           XK_4,                      3)
	TAGKEYS(                        -1,           XK_5,                      4)
	TAGKEYS(                        -1,           XK_6,                      5)
	TAGKEYS(                        -1,           XK_7,                      6)
	TAGKEYS(                        -1,           XK_8,                      7)
	TAGKEYS(                        -1,           XK_9,                      8)

  /*
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY|Mod1Mask,              XK_u,      incrgaps,       {.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_u,      incrgaps,       {.i = -1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_0,      defaultgaps,    {0} },
  */
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button1,        sigdwmblocks,   {.i = 1} },
	{ ClkStatusText,        0,              Button2,        sigdwmblocks,   {.i = 2} },
	{ ClkStatusText,        0,              Button3,        sigdwmblocks,   {.i = 3} },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};


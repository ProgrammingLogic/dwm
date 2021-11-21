/* See LICENSE file for copyright and license details. */
#include "fibonacci.c"
#include "X11/XF86keysym.h"


/* appearance */
static const unsigned int borderpx  = 3;        /* border pixel of windows */
static const unsigned int snap      = 24;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "monospace:size=12", "fontawesome:size=12" };
static const char dmenufont[]       = "monospace:size=12";

// background color
static const char col_gray1[]       = "#222222";
// inactive window border color
static const char col_gray2[]       = "#444444";
// font color
static const char col_gray3[]       = "#bbbbbb";
// current tag and current window font color
static const char col_gray4[]       = "#eeeeee";
// top bar second color (blue) and active window border color
static const char col_cyan[]        = "#005577";


static const unsigned int baralpha = 0xd0;
static const unsigned int borderalpha = OPAQUE;

static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_cyan  },
};

static const unsigned int alphas[][3]      = {
   /*               fg      bg        border     */
   [SchemeNorm] = { OPAQUE, baralpha, borderalpha },
   [SchemeSel]  = { OPAQUE, baralpha, borderalpha },
};


/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
    {  NULL,      NULL,       NULL,       0,            0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
    { "(@)",      spiral }, /* first entry is default */
	{ "[]=",      tile },    
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
    { "[\\]",     dwindle }, 
};

// MODKEY -- shortcuts to control dwm
// APPKEY -- shortcuts to launch applications
/* key definitions */
#define MODKEY Mod1Mask
#define APPKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }


/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] =           { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]  =           { "st", NULL };
static const char *browsercmd[]  =        { "firefox", NULL };
static const char *signalcmd[]  =         { "signal-desktop", NULL };
static const char *spotifycmd[]  =        { "spotify", NULL };
static const char *multicmd[]  =          { "multimc", NULL };
static const char *steamcmd[]  =          { "steam", NULL };
static const char *libreofficecmd[]  =    { "libreoffice", "--global", NULL};
static const char *bitwardencmd[]  =      { "bitwarden-desktop", NULL};
static const char *htopcmd[]  =      { "st", "-e", "htop", NULL};


/* shortcuts */
static const char *upvol[] =        { "/usr/bin/pactl", "set-sink-volume", "@DEFAULT_SINK@", "+5%", NULL };
static const char *downvol[] =      { "/usr/bin/pactl", "set-sink-volume", "@DEFAULT_SINK@", "-5%", NULL };
static const char *mutevol[] =      { "/usr/bin/pactl", "set-sink-volume", "@DEFAULT_SINK@", "toggle", NULL };


static Key keys[] = {
	/* modifier                     key        function        argument */
    // misc keys
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },

    // layout keys
	{ MODKEY,                       XK_r,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[3]} },
	{ MODKEY|ShiftMask,             XK_r,      setlayout,      {.v = &layouts[4]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },


    // volume
    // control via media keys
	{ 0,                            XF86XK_AudioLowerVolume,  spawn, {.v = downvol} },
	{ 0,                            XF86XK_AudioRaiseVolume,  spawn, {.v = upvol } },
	{ 0,                            XF86XK_AudioMute,         spawn, {.v = mutevol} },

    // control via shortcuts
	{ MODKEY,                       XK_bracketright,          spawn, {.v = upvol} },
	{ MODKEY,                       XK_bracketleft,           spawn, {.v = downvol } },
	{ MODKEY,                       XK_backslash,             spawn, {.v = mutevol} },


    // tag keys
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },


    /*
        Use WINDOWS key to launch applications

        WIN + r == dmenu
        WIN + return == terminal
        WIN + SHIFT + f == firefox
        WIN + SHIFT + b == bitwarden
        WIN + s == signal
        WIN + SHIFT + s == spotify
        WIN + g == steam
        WIN + SHIFT + g == multimc
        WIN + d == libreoffice (document)
    */
    { APPKEY,                       XK_r,         spawn,          {.v = dmenucmd } },
	{ APPKEY|ShiftMask,             XK_Return,    spawn,          {.v = termcmd } },
	{ APPKEY|ShiftMask,             XK_f,         spawn,          {.v = browsercmd } },
	{ APPKEY|ShiftMask,             XK_b,         spawn,          {.v = bitwardencmd } },
	{ APPKEY,	                    XK_s,         spawn,          {.v = signalcmd } },
	{ APPKEY|ShiftMask,	            XK_s,         spawn,          {.v = spotifycmd} },
	{ APPKEY,	                    XK_g,         spawn,          {.v = steamcmd} },
	{ APPKEY|ShiftMask,	            XK_g,         spawn,          {.v = multicmd} },
	{ APPKEY,	                    XK_d,         spawn,          {.v = libreofficecmd} },
	{ APPKEY,	                    XK_p,         spawn,          {.v = htopcmd} },
};


/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
    { 0,                       0,              0,              NULL,           {0} },
};


# Agent notes for dwm

This is a patched, personal build of [suckless dwm](https://dwm.suckless.org/) (version 6.3). It is plain C/X11 code with no package manager, tests, or CI.

## Build and install

- Use `./rebuild.sh` to rebuild and install. It does `rm config.h; make clean; make; sudo make install`.
- Plain `make` also works, but will not install and may reuse a stale `config.h`.
- `make install` requires root and writes to `${PREFIX}/bin` (default `/usr/local/bin`) and `${MANPREFIX}/man1`.

## Configuration

- **Edit `config.def.h`, not `config.h`.** The Makefile copies `config.def.h` to `config.h` on build, so any change to `config.h` is overwritten.
- `config.def.h` is the real source of truth for keybindings, colors, fonts, rules, layouts, autostart, etc.

## Patches and source layout

- Applied patches are stored in `patches/` for reference only; do **not** re-apply them.
- Notable patches in use: keychain (chord keybindings), systray, swallow, vanitygaps, pertag (per-tag layout, mfact, nmaster, bar, gaps, and last selected client), restartsig, restoreafterrestart, dwmblocks, cool-autostart, statusallmons, truecenteredtitle.
- Extra source files beyond upstream dwm: `vanitygaps.c` (textually included from `config.def.h`, i.e. from `config.h`) and `transient.c` (standalone helper, not compiled into dwm). Keep `vanitygaps.c` consistent with the layout definitions in `config.def.h`.
- Pertag caveat: `vanitygaps.c` is included before `struct Pertag` is defined in `dwm.c`, so the pertag-aware `togglegaps()` and `getgaps()` are defined in `dwm.c` after `struct Pertag` instead. `PERTAG_PATCH` is defined in `dwm.c` just before `#include "config.h"` and activates the per-tag gaps code. Do not re-add these functions when syncing `vanitygaps.c` from upstream.

## Dependencies

From `config.mk`, the build needs X11 headers/libraries plus: `libXinerama`, `fontconfig`, `libXft`, `libX11-xcb`, `libxcb`, `libxcb-res`.

## Generated / ignored files

- `config.h`, `dwm`, `*.o`, `*.orig`, `*.rej` are generated artifacts. Avoid editing or committing them.
- The repo already contains some local artifacts (`config.h`, `dwm.c.orig`, `*.o`); leave them ignored unless you are intentionally cleaning.

## Runtime notes

- The keychain patch means many shortcuts are two-key chords (e.g. `MOD+Shift+Q` then `Q`/`P`/`S`). See `README.md` for the keymap.
- `MOD+Shift+R` restarts dwm after a rebuild without logging out.

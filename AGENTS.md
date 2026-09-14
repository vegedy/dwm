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
- Notable patches in use: keychain (chord keybindings), systray, swallow, vanitygaps, restartsig, restoreafterrestart, dwmblocks, cool-autostart, statusallmons, truecenteredtitle.
- Extra compile units beyond upstream dwm: `vanitygaps.c`, `transient.c`. They are included via the Makefile and `dwm.c`, so keep them consistent with layout definitions in `config.def.h`.

## Dependencies

From `config.mk`, the build needs X11 headers/libraries plus: `libXinerama`, `fontconfig`, `libXft`, `libX11-xcb`, `libxcb`, `libxcb-res`.

## Generated / ignored files

- `config.h`, `dwm`, `*.o`, `*.orig`, `*.rej` are generated artifacts. Avoid editing or committing them.
- The repo already contains some local artifacts (`config.h`, `dwm.c.orig`, `*.o`); leave them ignored unless you are intentionally cleaning.

## Runtime notes

- The keychain patch means many shortcuts are two-key chords (e.g. `MOD+Shift+Q` then `Q`/`P`/`S`). See `README.md` for the keymap.
- `MOD+Shift+R` restarts dwm after a rebuild without logging out.

# dwm - dynamic window manager
dwm is an extremely fast, small, and dynamic window manager for X.

This repository contains a patched and modified version of dwm.

## Original source
The majority of the code in this repository originates from the suckless project, specifically from the dwm window manager. The original code is licensed under the MIT/X Consortium License.

Requirements
------------
In order to build dwm you need the Xlib header files.

## Modifications

The modifications and patches applied to the original dwm source code have been made by [Your Name]. These changes are also released under the MIT/X Consortium License.

## License

This project is licensed under the MIT/X Consortium License. See the [LICENSE](LICENSE) file for more details.

## Installation

To install or rebuild this patched version of dwm, just execute the [rebuild.sh](rebuild.sh). 
Afterwards you can log out of your session and back in or restart dwm with MOD+SHIFT+R.

## Usage

This version of dwm uses the [keychain](https://dwm.suckless.org/patches/keychain/) patch, therefore many function require two keypresses in quick succession.

### Basic movement
- MOD + Return: Make selected window the master
- MOD + LEFT: Decrease master size
- MOD + RIGHT: Increase master size
- MOD + J: Go stack down
- MOD + K: Go stack up

### Tag management
- MOD + TAB: Go to last tag(s)
- MOD + 0: Select all tags
- MOD + SHIFT + 0: Move window to all tags
- MOD + SHIFT + C: Kill window
- MOD + NUM: Go to tag NUM
- MOD + CONTROL + NUM: Go to tag NUM additionally
- MOD + SHIFT + NUM: Move window to tag NUM

### Starting programs
- MOD + SHIFT + Return: Open terminal
- MOD + D: Open dmenu

### Layout
- MOD + L: Change layout:
    - T: Tile
    - M: Monocle
    - S: Spiral
    - W: Dwindle
    - D: Deck
    - G: Grid
    - H: Horizgrid
    - C: Centeredmaster
    - F: Floating

### Power management
- MOD + SHIFT + Q: Quit:
    - Q: Quit (logout)
    - P: Power off
    - S: Suspend

### Toggle
- MOD + T: Toggle:
    - B: Toggle bar
    - F: Toggle floating
    - G: Toggle gaps
    - M: Toggle mute

### Additional tools
- MOD + SHIFT + R: Restart dwm
- MOD + UP: Raise volume
- MOD + DOWN: Lower volume
- MOD + CONTROL + S: Screenshot (fullscreen)
- MOD + SHIFT + S: Screenshot (region)


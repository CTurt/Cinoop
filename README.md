Cinoop
======
A Game Boy emulator for Windows and DS, written in C.

## Progress
**CPU:** Around half of the CPU instructions are implemented,

**GPU:** Can display tile maps, no sprites,

**Memory:** Support for 32KB ROMs, without mappers, only,

**Input:** Supported on Windows only,

**Sound:** None,

**Games:** It is unlikely that any games are playable yet. Cinoop used to be able to reach the menu screen of Tetris using a dirty hack, but it no longer works in the latest update.

![Running Tetris](tetris.png)

![Emulatorception](ds.png)

**Optimisations to do:** Currently the DS renders the Game Boy screen in framebuffer mode, this is slow. The DS supports hardware tile rendering, which will be much faster.

## Building
Just run `make` on the directory.

Requires [LDFS](https://github.com/CTurt/LDFS).

Tested with MinGW, using another compiler may require some tweaking.

## Usage
Pass the ROM you would like to run as the first argument. You can do this by either dragging the ROM onto Cinoop, or starting it from the command line:

    cinoop tetris.gb

To view the full debug log, you will need to redirect `stdout` to a file, like so:

    cinoop tetris.gb 1>debug.txt

### Controls
- B: Z
- A: X
- Start: Enter
- Select: Backspace
- DPad: Arrow keys

- Debug: Space
- Reset (not finished yet): *
- Quit: Escape
Cinoop
======
A Game Boy emulator for Windows, written in C.

## Progress
**CPU:** Less than half of the CPU instructions are implemented,

**GPU:** Can display tile maps, no sprites,

**Memory:** Support for 32KB ROMs, without mappers, only,

**Input:** None,

It can reach the copyright screen of Tetris.

![Running Tetris](tetris.png)

## Building
Just run `make` on the directory.

Requires [LDFS](https://github.com/CTurt/LDFS).

Tested with MinGW, using another compiler may require some tweaking.

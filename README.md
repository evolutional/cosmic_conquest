# Cosmic Conquest

```
            ---  ---  ---  --  --  ---  ---
           /    /  / / _  /  /  /  /   /
          /    /  /    / /     /  /   /
          ---  ---  --- -     - ---   ---
    ---  ---  -   -  ---   -  -  ---  ---  ----
   /    /  / / \ /  /  /  /  /  /_   / _    /
  /    /  / /   /  /  /  /  /  /       /   /
  ---  --- -   -   --\   ---  ---   ---   -

 An interstellar multiplayer game of War and Peace

                                      by Carl Edman
```

## Description

Cosmic Conquest is a 4-player turn-based tactical conquest game by Carl Edman and released for the Amiga in 1988.

The aim of Cosmic Conquest is to be the sole remaining ruler over the complete star cluster of 16 stars.

## Port Info

This is a minimal PC port of Cosmic Conquest version `1.035` by Carl Edman. It takes the original C source code and adds a thin facade around the Amiga Intuition libraries, just enough to get it working on a modern PC. The goal was to keep the original game as intact and as authentic as I could.

The port uses [dos-like](https://github.com/mattiasgustavsson/dos-like) by Mattias Gustavsson to provide an app/rendering framework. There are various other ways to do this, such as `bgfx`, `sokol`, `olcPixelGameEngine` and even straight `SDL2`, but `dos-like` was a very low friction path to getting this up and running.

As this game uses `dos-like`, you can toggle full-screen mode with `F11`. You can also provide `-w` on the commandline to start the game in windowed mode. Be aware that the mouse pointer is not shown in full-screen mode due to how `dos-like` handles it, so to play using the mouse you must run in windowed mode.

The Amiga Intuition library functions `SetAPen`, `RectFill`, `PrintIText` and `IntuiTextLength` are still referenced in code but have been shimmed onto equivalent code in `dos-like`. Anything that hits any rendering/input has been moved into `platform.h` / `platform_doslike.c` to make targeting another framework easier.

The original code made heavy use of in-place string literal modification. For all modern compilers other than MSVC this is undefined behaviour and resulted in a segfault. I've worked around this by making `DText.IText` a fixed-size buffer and then `strcpy` into it.

### Structure

```
    .
    ├── external
    │   └── dos-like
    ├── original
    │   ├── extracted
    │   └── Cosmic_Conquest_1989_Edman_Carl_PD_a.adf
    └── source
```

- `external` contains submodules of third-party code.
- `original` contains the unfettered original distribution ADF, and its extracted contents
- `source` contains the ported source

### Building

Clone the repo, fetch the submodules and build via `cmake`.

To build on Linux/MacOS you will need to install [dos-like's dependencies](https://github.com/mattiasgustavsson/dos-like#linux).

## Game Information

See the [original doc](original/extracted/cosmic.doc) for Carl Edman's documentation. There is also [an addendum](original/extracted/README) that covers Carl's final `1.035` release.

### Controls

This game is a text-based game, and as such the control are primarily text-based. This port uses the original control scheme, which can be a little bewildering.

- `h` shows the help screen
- `G` completes your turn
- `l` land a task force on a planet
- `c` carry ships
- `h` hyperspace travel for a task force
- `j` join two or more task forces
- `n` split a task force into two
- `b` opens the build menu
  - `,` and `.` moves left/right through the build menu
- `e` emigrate population from planet
- `d` declare war, peace or alliance on another player
- `p` possess a planet
- `k` kill a planet's population

### Ships

Name | Size | Capacity | Hyperspace Range | Hyperspace Speed | Speed | Attack | Number of Attacks | Shields | Tech Cost | Tech Level | Special
--- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | ---
Clearer | 3 | 0 | 0 | 0 | 1 | 1 | 20 | 4 | 100 | 620 | NOSPEC
UltraStinger | 1 | 0 | 50 | 50 | 3 | 8 | 1 | 4 | 60 | 800 | NOSPEC
Stinger | 1 | 0 | 0 | 0 | 3 | 7 | 1 | 2 | 25 | 150 | FASTEJECTABLE
Light Cruiser | 5 | 0 | 6 | 1 | 5 | 10 | 1 | 10 | 100 | 100 | NOSPEC
Space Cannon | 10 | 0 | 0 | 0 | 3 | 25 | 1 | 2 | 120 | 550 | NOSPEC
Destroyer | 12 | 0 | 9 | 2 | 6 | 18 | 1 | 10 | 110 | 310 | NOSPEC
Battleship | 25 | 3 | 8 | 2 | 6 | 18 | 3 | 18 | 150 | 450 | FASTEJECTER
Battlestar | 950 | 100 | 9 | 1 | 8 | 15 | 15 | 40 | 900 | 950 | FASTEJECTER
Heavy Cruiser | 8 | 0 | 8 | 1 | 7 | 13 | 1 | 14 | 120 | 220 | NOSPEC
Mothership | 100 | 20 | 11 | 1 | 6 | 12 | 2 | 24 | 230 | 620 | NOSPEC
Satellite | 2000 | 0 | 0 | 0 | 9 | 12 | 1 | 6 | 20 | 660 | NOSPEC
Trans III | 20 | 8 | 9 | 2 | 10 | 0 | 0 | 6 | 100 | 360 | NOSPEC
UltraTrans | 40 | 30 | 50 | 4 | 10 | 0 | 0 | 8 | 160 | 900 | NOSPEC
Glitter | 20 | 19 | 0 | 0 | 10 | 0 | 0 | 0 | 10 | 420 | NOSPEC
Scout | 2 | 0 | 11 | 3 | 10 | 0 | 0 | 6 | 50 | 180 | NOSPEC
Transporter | 10 | 4 | 4 | 1 | 10 | 0 | 0 | 6 | 40 | 50 | NOSPEC
Spy Sat | 1 | 0 | 9 | 1 | 10 | 0 | 0 | 70 | 280 | 750 | INVISIBLE
Torpedo | 1 | 0 | 0 | 0 | 1 | 20 | 1 | 0 | 45 | 240 | KAMIKAZE / FASTEJECTABLE
Pirate | 10 | 3 | 9 | 2 | 5 | 6 | 3 | 14 | 180 | 370 | NOSPEC
Carrier | 30 | 5 | 7 | 1 | 10 | 0 | 0 | 28 | 100 | 430 | FASTEJECTER
Q-Bomb | 30 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 350 | 950 | STARKILLER
Trans II | 10 | 4 | 9 | 1 | 0 | 0 | 0 | 6 | 70 | 210 | NOSPEC
Sidewinder | 1 | 0 | 0 | 0 | 1 | 4 | 1 | 0 | 5 | 240 | FASTEJECTABLE / KAMIKAZE
Space Fighter | 2 | 0 | 0 | 0 | 3 | 6 | 2 | 6 | 60 | 470 | FASTEJECTABLE
Light Carrier | 20 | 5 | 9 | 2 | 8 | 2 | 2 | 20 | 130 | 430 | FASTEJECTER
Fleet Carrier | 30 | 10 | 8 | 1 | 8 | 2 | 3 | 24 | 160 | 520 | FASTEJECTER
Defence Sat | 200 | 20 | 0 | 0 | 9 | 12 | 10 | 30 | 300 | 680 | FASTEJECTER
Space Station | 1000 | 120 | 0 | 0 | 9 | 12 | 30 | 50 | 800 | 900 | FASTEJECTER
Base Ship | 80 | 25 | 12 | 1 | 7 | 12 | 2 | 30 | 230 | 620 | FASTEJECTER
Ore Unit | 1 | 0 | 0 | 0 | 10 | 0 | 0 | 0 | MCOST | 0 | NOTPROD
Pop Unit | 1 | 0 | 0 | 0 | 10 | 0 | 0 | 0 | 20 | 0 | NOTPROD

### Trivia

- Only the `klingon` race has any actual AI implemented
- The `alien` and `romulan` races are implemented as a no-op, so they just sits there and mines their starting planet forever
- The `beserker` race declares war on everyone but does nothing additional to the `alien`/`romulan` code
- The source code and docs reference a `debug.c` (secret debug menu) which was not distributed with the source.

### Bugs / Problems

- The original game makes heavy use of string literal modification, which is undefined behaviour in most modern compilers (except MSVC). This has been corrected in the ported code.
- The original code used `\n` and `\r` interchangably, which caused input issues in Linux. This has been standardized to `\n` in the port.
- The load/save system uses direct memory saving/loading. This means that original save games are *not* compatible with this version as the size of the integers have changed (the original was 16-bit) and you'd need to account for struct padding. This is probably fixable relatively easily if anyone so wished. To distinguish "new" savegames from the originals, I've added in a `CCNQ` magic marker into new savegames to make it easier to add in the future.
- `dos-like` does not support displaying the pointer in fullscreen mode, to use the mouse, you must start in windowed mode (`-w` argument)
- `dos-like` does not support custom pointer graphics, so at this time the original mouse pointer is not displayed
- The code does not compile cleanly without warnings. Many of the issues are down to the old style of the code, such as functions not returning a value or the use of `long` numbers everywhere.

## Licenses

No additional licence or claim is asserted over any of the works here. All components are used under their respective licenses.

The port uses [dos-like](https://github.com/mattiasgustavsson/dos-like) by Mattias Gustavsson for the main application framework. See the [original dos-like license](https://github.com/mattiasgustavsson/dos-like/blob/main/LICENSE) for details.

The font used is [ZX Eurostile](https://damieng.com/typography/zx-origins/zx-eurostile/), which is part of the ZX Origins font collection Copyright (c) 1988-2022 Damien Guard.

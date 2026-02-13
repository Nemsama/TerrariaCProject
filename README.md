# Terraria Clone

## Description

This is a small spare time project about trying to recreate Terraria.
The project is not finished (far from it actually) but currently paused for an undetermined period.

I had a lot of fun trying to recreate Terraria using Python (with Pygame) and wanted to retry that experience using C and maybe some C++.
I ended up only using C with SDL library so I switched the files extentions lately.

I will maybe return to this project later, maybe making it really C++ or restarting from scratch to make it in Java (lets try them all :smirk:).

I have recently decided to upload this project to have anyone take a look at it and maybe get some feedback.

## Features

- Player movement using WASD keys[^1] (note that W is used for jump instead of the space bar in the real Terraria), camera movement using space bar to center it to the player and the keyboard arrows to move it freely; you can also zoom in and out by scrolling[^2].
- Rudimentary physics (character and items) that I am not sure what to think about (good but not quite good enough).
- Procedural terrain generation using perlin noise that I took from [this github page](https://github.com/Nicknakin/PerlinNoiseGeneratorCPP) which I found on [this reddit post](https://www.reddit.com/r/cpp/comments/greq8t/ndimensional_perlin_noise_generator/), It surely is not optimal but I didn't want to code it myself and it works.
- block mining (using pickaxe) and placement with an inventory that cannot be directly used by the player and a system of tools not fully implemented (but fonctional for the pickaxe).

[^1]: I tested it with an AZERTY keyboard so the controls may be different on a different keyboard
[^2]: May slow the game if too zoomed out but usefull to see world generation, the zoom still has display issues but since it is designed for debuging I didn't bother

## Requirements

- C compiler - e.g., gcc
- SDL2 and C standard Math libraries
- Make (for building)

## Building

```bash
make
```
or
```bash
make all
```

## Running

```bash
./build/terraria
```

## Controls

| Key | Action |
|-----|--------|
| [A] | Player left |
| [D] | Player right |
| [W] | Player jump |
| [Space] | Center camera on player |
| [Arrow Keys] | Move camera freely |
| [Mouse Scroll] | Zoom in/out |

## Project Structure

```
├── src/           # Source files
├── include/       # Header files
├── assets/        # Game assets
├── build/         # Build output
└── saves/         # Save files
```

## License

Perlin noise license (see [the dedicated github page](https://github.com/Nicknakin/PerlinNoiseGeneratorCPP) for more details):

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.


For the rest of the code, feel free to use this project as you wish, I don't care about the licensing. (But if you want to give me credit that would be nice :smile: )

## Authors

- [Nils Desbois-Renaudin] - Everything except perlin noise and some assets
- [Joshua Faure] - some assets such as some clouds

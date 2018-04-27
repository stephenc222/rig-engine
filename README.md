# RIG Engine

## Overview

provides a straightforward wrapper around OpenGL for abstracting away OpenGL specific handling and allowing
focus on game development itself. Scriptable with Lua.

## Assumptions

+ _The game is the **what**, and the engine is the **how**_
+ _**What** is more important than the **how**_

## Features

+ uses strictly a 2D orthographic projection
+ coordinates are mapped 1:1 from modelspace to screenspace
+ origin is fixed at the top left corner of the screen
+ frame-based animated sprites with z (group layer) and y (logical depth) sorting
+ sprites may be flipped horizontally and/or vertically
+ sprites may be scaled along x and/or y axes
+ sprites may be rotated about the z axis around a pivot point relative to the sprite's center (pivot defaults to 0, 0)
+ sprites may be tinted with any RGB value
+ sprites may have an opacity of 0 (fully invisible) to 1 (fully visible)
+ sprites may be static (single frame) or animated (multiple frames)
+ animated sprites may be in any of the following animate states: paused, playing, stopped
+ animated sprites may have multiple animation sequences defined per sprite
+ animation sequences may contain any number of frames
+ animation sequences may have a duration specified to set frame timing, or may use specific frame times per frame
+ animation sequences may contain sub-sequences to create complex animations
+ animation sequences may be single playthrough, looped, ping-ponged, or played back at random
+ animation sequences may contain events where instead of an animation frame, a function is called
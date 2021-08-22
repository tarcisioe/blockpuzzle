blockpuzzle
===========

Code for a clone of a famous block puzzle game, developed on-stream, on
[my YouTube channel](https://www.youtube.com/channel/UCooSJmwOlaz2Dx-iT2uvUuw?).

Building
--------

To build, run:

```
cmake -B build
cmake --build build
```

To enable the Raylib frontend (by @edricgarran) add `-DRAYLIB_FRONTEND=ON` to
the first line. Raylib has system dependencies on `opengl/system` and `xorg/system`.

You can inspect options with `ccmake build`.

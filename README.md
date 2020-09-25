# PurpleBox

![example workflow name](https://github.com/DidgeridooMH/PurpleBox/workflows/CI/badge.svg)

PurpleBox is an early-stage GameCube emulator. This emulator has no focus and is mostly used as a fun project to learn the system. It is written using modern C++17 and utilizes the Meson build system.

## Requirements

You must have a compiler that supports the C++17 standard and the Meson build system. Ninja is also recommended but not required.

```bash
sudo apt update
sudo apt install build-essentials
python -m pip install meson
```

## Building

```bash
meson build
meson compile -C build
```

## Running

*The arguments used currently are temporary and will most likely change*

```bash
purplebox [-v | -b [BOOTROM]]
```

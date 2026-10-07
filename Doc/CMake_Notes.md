# Cmake Notes

## Table of Contents

- [Cmake Notes](#cmake-notes)
  - [Table of Contents](#table-of-contents)
  - [Installation](#installation)
  - [Context](#context)
  - [Version number meaning](#version-number-meaning)
  - [CMake Benifits](#cmake-benifits)
  - [Explanation: the Global CMake](#explanation-the-global-cmake)
  - [Mistake: find\_package after add\_subdirectory](#mistake-find_package-after-add_subdirectory)
    - [Error](#error)
    - [Cause](#cause)
    - [Fix](#fix)
    - [Rule to remember](#rule-to-remember)
    - [Related mistakes found at the same time](#related-mistakes-found-at-the-same-time)
  - [What find\_package does](#what-find_package-does)
    - [Steps](#steps)
    - [Arguments](#arguments)
    - [Why it matters](#why-it-matters)
    - [find\_package vs FetchContent](#find_package-vs-fetchcontent)
  - [What target\_link\_libraries does](#what-target_link_libraries-does)
    - [Imported target](#imported-target)
    - [What happens when linking](#what-happens-when-linking)
    - [PUBLIC / PRIVATE / INTERFACE](#public--private--interface)
    - [Same idea for the game](#same-idea-for-the-game)
    - [Requirement](#requirement)

## Installation

```
sudo snap install cmake --classic
```

Verify version:
```
cmake --version
```


## Context

This document contains the information I learned for `cmake` whilie building my project

## Version number meaning

VERSION 1.0.0

- 3rd digit: bug fixes, something doesn't work and now it works, so I update the number
- 2nd digit: new features, and not craching <-> all is working with the new feature
- 1st digit: new iteration is going to the software


## CMake Benifits

- Cross platfrom between all OS (Linux, Window,...)
- generate several build files for make, ninja,Visual studio,...
- work with several compiler like GCC,Clang,MSVc,...
- work with external libraries like SDL,OpeGl,...


## Explanation: the Global CMake

## Mistake: find_package after add_subdirectory

### Error

```
CMake Error at Saga_Engine/CMakeLists.txt:17 (target_link_libraries):
  Target "Saga_Engine" links to: SFML::Graphics
  but the target was not found.
```

### Cause

CMake reads files from top to bottom. `find_package(SFML ...)` is what creates the
imported targets (`SFML::Graphics`, `SFML::Window`, ...). 

In the root `CMakeLists.txt` it was written **after** `add_subdirectory(Saga_Engine)`, 
so when CMake processed `Saga_Engine/CMakeLists.txt` and reached `target_link_libraries(... SFML::Graphics)`,
the target did not exist yet.

```cmake
# Wrong order
add_subdirectory(Saga_Engine)   # uses SFML::Graphics -> not defined yet
find_package(SFML 3.1 CONFIG REQUIRED COMPONENTS Graphics Window System Audio)
```

### Fix

Call `find_package` **before** any `add_subdirectory` that uses the targets:

```cmake
find_package(SFML 3.1 CONFIG REQUIRED COMPONENTS Graphics Window System Audio)

add_subdirectory(Saga_Engine)
add_subdirectory(Saga_Game)
```

### Rule to remember

Targets and variables must be defined before the subdirectory that uses them is added.
Subdirectories inherit what the parent defined up to that point.

### Related mistakes found at the same time

- `add_executable(Saga_Game STATIC ...)`: `STATIC` is only valid for `add_library`.
  In `add_executable` it is read as a source file name ("Cannot find source file: STATIC").
- `${CMAKE_CURRENT_SOURCE_DIR}` is the directory of the CMakeLists.txt being processed.
  In the root file it is `Saga/`, not `Saga/Saga_Engine/`. Put `target_include_directories`
  inside the CMakeLists.txt of the target's own directory.

## What find_package does

`find_package` looks for a library that is **already installed** on the machine and makes it
usable in the project. It does not download or build anything.

```cmake
find_package(SFML 3.1 CONFIG REQUIRED COMPONENTS Graphics Window System Audio)
```

### Steps

1. **Search**: CMake looks for the package's config file (`SFMLConfig.cmake`) in the standard
   install locations and in the paths listed in `CMAKE_PREFIX_PATH`
   (e.g. `-DCMAKE_PREFIX_PATH=/usr/local`, where SFML is installed here, in `/usr/local/lib/cmake/SFML`).
2. **Check**: it verifies the version (3.1 or compatible) and that the requested components exist.
3. **Define targets**: it creates imported targets such as `SFML::Graphics`, `SFML::Window`,
   `SFML::System`, `SFML::Audio`. Each target carries the include paths, library files and
   dependencies.

### Arguments

| Argument | Meaning |
|---|---|
| `SFML` | name of the package |
| `3.1` | minimum version required |
| `CONFIG` | use the package's own `SFMLConfig.cmake` file (not a `FindSFML.cmake` module) |
| `REQUIRED` | stop with an error if not found (without it, configuration continues) |
| `COMPONENTS ...` | the parts of the library needed |

### Why it matters

After `find_package`, a single line gives a target everything it needs:

```cmake
target_link_libraries(Saga_Engine PUBLIC SFML::Graphics SFML::Window SFML::System SFML::Audio)
```

No manual `-I`, `-L` or `-l` flags are required. Because of this, `find_package` must run
**before** any code that uses those targets (see the mistake above).

### find_package vs FetchContent

- `find_package`: use a library already installed on the system (fast, nothing to build).
- `FetchContent`: download the source from GitHub and build it with the project.

## What target_link_libraries does

```cmake
target_link_libraries(Saga_Engine PUBLIC SFML::Graphics)
```

It says: "the target `Saga_Engine` **uses** `SFML::Graphics`". CMake then applies everything
the used target carries to `Saga_Engine`.

### Imported target

`SFML::Graphics` is not a file. It is a named record created by `find_package` that stores:

- the path to the library file (e.g. `/usr/local/lib/libsfml-graphics.so`)
- the include directory with the headers (e.g. `/usr/local/include`)
- its own dependencies (`SFML::Window`, `SFML::System`)
- required compile flags and definitions

"Imported" means the library was built elsewhere and CMake did not build it.
Targets built by the project (like `Saga_Engine`) are used the same way.

### What happens when linking

- compile step: adds `-I/usr/local/include`, so `#include <SFML/Graphics.hpp>` works
- link step: adds `libsfml-graphics.so` and its dependencies (Window, System)
- runtime: the loader finds the `.so` files through RPATH or the system library path

### PUBLIC / PRIVATE / INTERFACE

The keyword controls whether targets that link to `Saga_Engine` also receive SFML.

| Keyword | Saga_Engine itself uses SFML | Targets linking Saga_Engine get SFML |
|---|---|---|
| `PRIVATE` | yes | no |
| `INTERFACE` | no | yes |
| `PUBLIC` | yes | yes |

In this project `Saga_Game` links to `Saga_Engine`. With `PUBLIC`, the game can
`#include <SFML/...>` and is linked to SFML automatically. With `PRIVATE`, the game
would have to link SFML itself.

Rule of thumb: use `PUBLIC` if SFML types appear in the engine's public headers
(`include/`), otherwise `PRIVATE`.

### Same idea for the game

```cmake
target_link_libraries(Saga_Game PUBLIC Saga_Engine)
```

The game gets the engine's library, the engine's `PUBLIC` include directory, and (through
the engine) SFML. Dependencies propagate along the chain.

### Requirement

The target must already exist when the command runs, so `find_package` has to come first
(see the mistake section above).

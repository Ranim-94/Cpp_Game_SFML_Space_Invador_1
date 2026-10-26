# World and Levels

## Table of Contents

- [World and Levels](#world-and-levels)
  - [Table of Contents](#table-of-contents)
  - [Overview](#overview)
  - [Worlds, Levels, and Applications](#worlds-levels-and-applications)
    - [Implementation](#implementation)
    - [World Switching Behaviour](#world-switching-behaviour)
    - [Alternative Code Desing](#alternative-code-desing)
  - [Run time and Call Stack](#run-time-and-call-stack)
    - [Part 1: creating the world with `LoadWorld<>()`](#part-1-creating-the-world-with-loadworld)
    - [Part 2: who owns what at each moment](#part-2-who-owns-what-at-each-moment)
    - [Part 3: replacing a world](#part-3-replacing-a-world)
    - [Part 4: when `LoadWorld` is not called](#part-4-when-loadworld-is-not-called)
    - [Notes](#notes)

## Overview

A `World` represents a running game context. It can contain or manage the
objects that make up a level, such as the player, enemies, terrain, and
missions.

An application manages the lifetime and transition of worlds. For example, an
application might switch between:

- a main-menu world;
- a gameplay world;
- a game-success world; and
- a game-over world.

So in other words:

- `World` will be managed and encapsualted by `Application`
- Once `Application` is out of scope, `World` also

In implementation, we need to have a reference to the owning application.

The exact class design can vary, but the important distinction is that an
application is not a world. The application manages worlds.

## Worlds, Levels, and Applications

A useful conceptual hierarchy is:

```text
Application
    manages
        World
            manages
                Level content and game objects
```

In a small game, `World` may itself represent one level. In a larger game, a
world can manage several levels or level sections. The names are design
choices; the ownership relationship is more important than the terminology.

For example:

```cpp
class Application {
    // Selects and updates the current world.
};

class World {
    // Updates the current level's gameplay state.
};
```

### Implementation

In code, ownership should normally be made explicit with a smart pointer, becasue the application is responsible for destroying the world

The choice in the code was `std::shared_ptr<T>`.

The implementation was done by adding several step:

1.In `Application.h/cpp` files: 

```cpp
// Application.h

#include <memory>

class Application {
private:
    std::shared_ptr<World> m_current_world;
};
```

2. Initializing the shared pointer using `LoadWorld<>()`:

Now the `m_current_world`, which is a shared pointer is being handled in a separate templated function called `LoadWorld<>()`, so we can have a more generic desing and use this function later on.

### World Switching Behaviour

By using a templated function, in `Game_App()` we can have another same function which can manage another **World** concept as for example `LoadWorld<Level_One>()`, and `Level_One` will be some class managed by `Application` class also.

<u>Example:</u>

```cpp
//Game_Application.cpp

Game_App::Game_App(){

	// loading some level
	LoadWorld<World>();

    LoadWord<Level_One>();

	}// End Game_App()
```

This design is called **world-switching**.

How the `Application` will do the switching from `World` to `Level_One`? It is using this reassignment statment inside `LoadWorld<>()`:

```cpp
// Application.h

this->m_current_world = new_world
```

Reassigning `m_current_world` drops the last owner of the old world, so the old world is destroyed. That is the **world-switching*** behaviour from the design

By this way and using smart pointers (shared one in this case), `Application` can manage `World` and destroy properly.

### Alternative Code Desing

To manage the `World` class, we could initialize the `m_current_world` directly inside the `Application()` as the following:

```cpp
// Application.cpp

Application::Application():
	m_window{sf::VideoMode({800,600}),"Space Invador"},
	m_frame_rate_target{60.0},
	m_tick_clock{},
	m_current_world{std::make_shared<World>()}
	{

	} // End Application()

```

The code will run directly and no need for `LoadWorld<>()`.
However, this means that `App()` always create a plain World first, and we have no way so the `App`class manage another World concept such as `Level_One` for example, that's why a templated function as `LoadWorld<>()` in `Application` class is better.

## Run time and Call Stack

<u>Prerequiste:</u> Must read [Entry_Point.md](Entry_Point.md) before this section, in particular:

- [The idea: inversion of control](Entry_Point.md#the-idea-inversion-of-control)
- [The contract](Entry_Point.md#the-contract)
- [Running the game: what happens in script_build.sh](Entry_Point.md#running-the-game-what-happens-in-script_buildsh)
  - [Part 1: the script (build time)](Entry_Point.md#part-1-the-script-build-time)
  - [Part 2: the call stack (runtime)](Entry_Point.md#part-2-the-call-stack-runtime)

`Entry_Point.md` describes the call stack of the application itself (window, loop, tick).

**This section only follows what is added for the `World`**: how it is created, who owns it,
and when its methods are called.

### Part 1: creating the world with `LoadWorld<>()`

Assume the game constructor loads a world:

```cpp
// Game_Application.cpp
Game_App::Game_App(){

	LoadWorld<World>();

}
```

```
main()                                            [Engine: Entry_Point.cpp]
 │
 ├─ 1. saga::get_application()                    [declared in engine, DEFINED in game]
 │      └─ new Game_App
 │           ├─ Application::Application()        [Engine, base ctor runs first]
 │           │     └─ m_current_world is empty (nullptr)
 │           └─ Game_App::Game_App()              [Game, body runs after the base]
 │                └─ LoadWorld<World>()           [Engine template, instantiated here]
 │                     ├─ std::make_shared<World>()
 │                     │     └─ new World()  -> World::World()   [the world now exists]
 │                     ├─ m_current_world = new_world
 │                     │     └─ Application now OWNS the world (reference count = 2)
 │                     └─ return weak_ptr<World>
 │                           └─ new_world goes out of scope -> reference count = 1
 │                              (only m_current_world owns it)
 │      returns Application*
 │
 ├─ 2. app->run()
 │      └─ ... every fixed 60 Hz step:
 │           └─ Application::tick_internal(dt)
 │                ├─ tick(dt)                       (application tick, prints frame rate)
 │                └─ if (m_current_world)           (true: LoadWorld filled it)
 │                     ├─ World::start_play_internal()
 │                     │     └─ 1st call only: m_is_play = true -> start_play()
 │                     └─ World::tick_internal(dt)
 │                           └─ tick(dt)            (virtual: game override if any)
 │
 ├─ 3. window closed -> run() returns
 └─ 4. delete app
        └─ ~Application() destroys m_current_world
             └─ last owner gone -> ~World() runs
```

### Part 2: who owns what at each moment

```
Inside LoadWorld, before the assignment:
    new_world ──────────────────────> World          count = 1

After  m_current_world = new_world:
    new_world ──────────────────────> World          count = 2
    Application::m_current_world ───> World

After LoadWorld returns (new_world is destroyed):
    Application::m_current_world ───> World          count = 1
    caller's weak_ptr (if kept) ····> World          observes, count unchanged
```

The `Application` is the only owner when `LoadWorld` returns. The returned `weak_ptr`
does not keep the world alive.

### Part 3: replacing a world

A second call to `LoadWorld<>()` swaps the world. `Level_One` below is a hypothetical
`World` subclass:

```
LoadWorld<Level_One>()
 ├─ make_shared<Level_One>()                       new world created, count = 1
 ├─ m_current_world = new_world
 │     ├─ old World loses its last owner  -> ~World() runs, old world destroyed
 │     └─ m_current_world now owns the Level_One
 └─ return weak_ptr<Level_One>

Next tick_internal(): m_current_world points at Level_One, so its
start_play_internal() runs first (m_is_play is false for the new world).
```

If this is called from inside a world's own `tick()`, the world would destroy itself
while running. In that case the swap should be delayed until the end of the frame.

### Part 4: when `LoadWorld` is not called

With the alternative design above, the world is created in the `Application` constructor.
That runs inside `new Game_App`, before `run()`, so the world already exists at the
first tick even when `Game_App` never calls `LoadWorld<>()`.

```
new Game_App
 └─ Application::Application()
      └─ m_current_world{std::make_shared<World>()}   [World created here]
```

Using `m_current_world{nullptr}` instead, `if (m_current_world)` is false. The world
methods are skipped without a crash and nothing is printed for the world.

### Notes

- **Constructor order:** the base `Application()` finishes before the body of `Game_App()`
  runs, so `m_current_world` always exists as a member before `LoadWorld<>()` assigns it.
- **Template instantiation:** `LoadWorld<World>()` is only compiled where it is called,
  so a constructor mismatch (for example `World` needs an argument) appears in the game
  build, not the engine build.
- **First tick:** `start_play()` runs only once per world, guarded by `m_is_play`.
- Read from the code, not confirmed with a debugger. To verify, put a breakpoint in
  `World::World()` and look at the call stack.

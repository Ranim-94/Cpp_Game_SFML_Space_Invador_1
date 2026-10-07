# Entry Point Notes

## Table of Contents

- [Entry Point Notes](#entry-point-notes)
  - [Table of Contents](#table-of-contents)
  - [The idea: inversion of control](#the-idea-inversion-of-control)
  - [The contract](#the-contract)
  - [Is this standard?](#is-this-standard)
  - [Why it is good](#why-it-is-good)
  - [Things to watch](#things-to-watch)
  - [Alternative: main in the game](#alternative-main-in-the-game)
  - [Design patterns involved](#design-patterns-involved)
    - [Template Method](#template-method)
    - [What Template Method does not cover](#what-template-method-does-not-cover)
  - [Running the game: what happens in script\_build.sh](#running-the-game-what-happens-in-script_buildsh)
    - [Part 1: the script (build time)](#part-1-the-script-build-time)
    - [Part 2: the call stack (runtime)](#part-2-the-call-stack-runtime)
    - [Who calls whom](#who-calls-whom)
    - [Notes](#notes)
  - [Status in Saga](#status-in-saga)

## The idea: inversion of control

In a normal program, my code owns `main()` and calls libraries. 

In an engine, the engine owns `main()` and calls my game. This is the Hollywood principle: "don't call us, we'll call you."

In other words:
- we hide the `main()`
- the game need only to provide `get_application()`
  - so the definition/implementation is inside the Game code
- engine take care of the initialization and running the game

The engine handles everything common to every game:

- window and platform setup
- the main loop and timing
- logging and crash handling
- shutdown

The game only supplies what is specific to it.

## The contract

The engine declares a function that it requires but does not define:

```cpp
// Engine: Application.h / Entry_Point.h
namespace saga {
    Application* get_application();   // declared, NOT defined in the engine
}
```

The engine's `main()` uses it:

```cpp
// Engine: Entry_Point.cpp
int main() {
    saga::Application* app = saga::get_application();
    app->run();
    delete app;
}
```

The game defines it and returns its own subclass:

```cpp
// Game: Game_Application.cpp
class GameApplication : public saga::Application { /* game-specific logic */ };

saga::Application* saga::get_application() {
    return new GameApplication();
}
```

The linker joins the two. The engine is a static library, so its `main()` is pulled in, and
`get_application()` resolves to the game's definition. This matches the CMake setup, where the
game executable links the engine library.

## Is this standard?

Yes. Examples:

- **Hazel** (The Cherno's engine) uses `CreateApplication()` the same way. `get_application`
  and the `saga` namespace follow this pattern.
- **Unreal** uses `IMPLEMENT_PRIMARY_GAME_MODULE` and the engine's `GuardedMain`.
- **SDL3** has an optional `SDL_main` callback entry-point system.
- **Qt** has `QApplication`, with a similar owned-event-loop idea.

## Why it is good

- **Separation:** the engine knows nothing about any game, only the `Application` base class.
- **Reuse:** a second game only needs its own `get_application()`.
- **Polymorphism:** the engine calls virtual functions such as `render()` and `tick()`, the game overrides them.
- **Platform handling:** the engine can use `WinMain` on Windows or `main` elsewhere without game code changing.

## Things to watch

1. **Declaration location:** declare `get_application` in a header the engine includes. The game
   must define it with the identical signature and namespace.
2. **Missing definition:** if the game forgets it, the linker reports
   `undefined reference to saga::get_application()`.
3. **Virtual hooks:** `render()` is `private virtual`. Overriding works, but subclasses cannot
   call the base version. Use `protected` if needed.
4. **Ownership:** `new` + `delete` works. Returning `std::unique_ptr<Application>` is safer.
5. **Static library caveat:** a static library only links object files that are referenced.
   `main()` is fine because nothing else defines it, but self-registering code could be dropped.
6. **One game per executable:** the engine's `main()` is in the library.

## Alternative: main in the game

Some engines keep `main()` in the game and expose `engine.run(game)`. The game has more control
and it is more explicit and easier to test, but every game repeats some boilerplate.
Both approaches are valid. Saga uses the engine-owned entry point.

## Design patterns involved

### Template Method

A base class defines the fixed skeleton of an algorithm and leaves some steps as virtual hooks
that subclasses override. `Application` fits this:

```cpp
void Application::run() {          // fixed skeleton, owned by the engine
    while (m_window.isOpen()) {
        tick_internal(dt);         // engine-side work, then calls virtual tick()
        render_internal();         // engine-side work, then calls virtual render()
    }
}
virtual void render();             // hook the game overrides
```

The engine controls the order (events, tick, render). The game fills in only its own steps.
The author's "template structure pattern" most likely means this pattern (not confirmed,
the source was not checked).

### What Template Method does not cover

The `get_application()` and `main()` arrangement is a different idea:

- **Inversion of control:** the engine owns `main()` and calls into the game.
- **Factory function:** `get_application()` creates the game's object without the engine
  knowing its type (close to Factory Method).

Full picture: Template Method for the `Application` class, plus a factory function and
inversion of control for the entry point.

## Running the game: what happens in script_build.sh

### Part 1: the script (build time)

```
script_build.sh
 ├─ cmake --fresh -S Saga -B Saga/build
 │    ├─ root CMakeLists.txt
 │    │    ├─ find_package(SFML)            -> creates SFML::* targets
 │    │    ├─ add_subdirectory(Saga_Engine) -> libSaga_Engine.a (+ SFML, PUBLIC)
 │    │    └─ add_subdirectory(Saga_Game)   -> executable Saga_Game, links the engine
 │    └─ generates Makefiles
 ├─ cmake --build Saga/build
 │    ├─ compile Application.cpp, Entry_Point.cpp -> pack into libSaga_Engine.a
 │    ├─ compile Game_Application.cpp
 │    └─ link: Game_Application.o + libSaga_Engine.a + SFML .so
 │             -> Saga/build/Saga_Game/Saga_Game
 └─ run the executable                      -> the OS starts the program
```

### Part 2: the call stack (runtime)

The shell starts the process. The OS loader loads the SFML `.so` files and jumps to `main()`.
That `main()` is the engine's, in `Entry_Point.cpp`.

```
main()                                        [Engine: Entry_Point.cpp]
 │
 ├─ 1. saga::get_application()                [declared in engine, DEFINED in game]
 │      └─ new Game_App
 │           └─ Game_App::Game_App()          [Game]
 │                └─ Application::Application()   [Engine, base ctor runs first]
 │                     └─ creates sf::RenderWindow 800x600, frame rate 60, clock
 │      returns Application* (really a Game_App)
 │
 ├─ 2. app->run()                             [Engine, Application::run]
 │      └─ while (window is open)
 │           ├─ pollEvent()  -> if Closed: window.close()
 │           ├─ render_internal()
 │           │     ├─ window.clear(Black)
 │           │     ├─ render()     (virtual: Game_App override if any,
 │           │     │                else Application::render -> red rectangle)
 │           │     └─ window.display()
 │           ├─ time_accumulated += clock.restart()
 │           └─ while (time_accumulated >= 1/60)
 │                 └─ tick_internal(dt) -> tick(dt)   (prints frame rate)
 │
 ├─ 3. window closed -> run() returns
 ├─ 4. delete app                             [destructor chain, window destroyed]
 └─ 5. return 0 -> process exits
```

### Who calls whom

```
Engine main() ──calls──> get_application()  (game code, found by the linker)
Engine run()  ──calls──> render() / tick()  (virtual, game can override)
```

The engine never names `Game_App`. It only knows `Application`.

### Notes

- **Frame order in `run()`:** drawing happens every loop iteration. The fixed 60 Hz `tick`
  runs separately, as many times as the accumulated time allows.
- **Constructor order:** `Game_App()` is entered first, but the base `Application()`
  constructor finishes before the body of `Game_App()` runs.
- **Virtual dispatch:** `render()` and `tick()` are virtual and `app` is really a `Game_App`,
  so a game override would be called with no engine change. Currently `Game_App` overrides
  nothing, so the base versions run.
- **Red rectangle:** drawn by the engine's own `Application::render()`.
- Read from the code, not confirmed with a debugger. To verify, put a breakpoint in
  `Application::render()` and look at the call stack.

## Status in Saga

- `get_application()` is declared in `Entry_Point.h` (namespace `saga`) and defined in
  `Game_Application.cpp`, returning a `Game_App`. The build links correctly.
- `script_build.sh` runs `Saga/build/Saga_Game/Saga_Game`.

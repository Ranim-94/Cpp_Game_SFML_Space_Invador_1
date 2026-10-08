# World and Levels

## Table of Contents

- [Overview](#overview)
- [Worlds, Levels, and Applications](#worlds-levels-and-applications)
- [Inheritance Versus Ownership](#inheritance-versus-ownership)
- [Correct World Declaration](#correct-world-declaration)
- [World Implementation](#world-implementation)
- [How the Classes Relate](#how-the-classes-relate)
- [Common Errors](#common-errors)

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

## Inheritance Versus Ownership

Inheritance uses the following syntax:

```cpp
class Derived : public Base {
};
```

It expresses an "`is-a`" relationship:

```cpp
class Game_App : public Application {
};
```

This means that a `Game_App` is an `Application`. It can use the public
interface of `Application` and can override its virtual functions.

However, an application is not a world, and a world is not an application.
Therefore, these designs would not normally be correct:

```cpp
class Application : public World {
};

class World : public Application {
};
```

An application managing a world is a "`has-a`" relationship, not an "`is-a`"
relationship. It should be represented with a member, pointer, or smart
pointer. For example, a world can keep a pointer to its owning application:

```cpp
class Application;

class World {
private:
    Application* m_owner_app;
};
```

The pointer means that the world knows which application owns or manages it.
It does not make `World` a subclass of `Application`.

## Correct World Declaration

The declaration in `World.h` should begin with `class World`, not
`class Application: class World`:

```cpp
#pragma once

namespace saga {

class Application;

class World {
public:
    explicit World(Application* owner_app);
    virtual ~World() = default;

    virtual void start_play();
    virtual void tick(float time);

private:
    Application* m_owner_app;
    bool m_is_play{false};

    void start_play_internal();
    void tick_internal(float time);
};

} // namespace saga
```

`class Application;` is a forward declaration. It is sufficient here because
the header only stores an `Application*`. This avoids unnecessarily including
the complete `Application.h` definition in `World.h`.

The initializer `{false}` gives `m_is_play` a known initial value. Without
initialization, a `bool` data member can contain an indeterminate value.

## World Implementation

Member functions defined outside the class must use the class qualifier
`World::`:

```cpp
#include "framework/World.h"
#include "framework/Core.h"

namespace saga {

World::World(Application* owner_app)
    : m_owner_app{owner_app} {
}

void World::start_play() {
    if (!m_is_play) {
        m_is_play = true;

        LOG("Starting the game");

        start_play_internal();
    }
}

void World::tick(float time) {
    tick_internal(time);
}

void World::start_play_internal() {
    // Start the world-specific gameplay here.
}

void World::tick_internal(float time) {
    // Update world-specific gameplay here.
}

} // namespace saga
```

For example, this function:

```cpp
void start_play() {
}
```

is a free function. It is not a member of `World`. The correct definition is:

```cpp
void World::start_play() {
}
```

The `World::` qualifier is also what allows the implementation to access
members such as `m_is_play` and `start_play_internal()`.

## How the Classes Relate

The existing application inheritance is appropriate when a game-specific
application specializes the engine application:

```cpp
class Game_App : public Application {
public:
    Game_App();
};
```

The application can then manage a world. A simplified example is:

```cpp
class Application {
public:
    void change_world(World* next_world);

private:
    World* m_current_world{};
};
```

In production code, ownership should normally be made explicit with a smart
pointer, such as `std::unique_ptr<World>`, if the application is responsible
for destroying the world:

```cpp
#include <memory>

class Application {
private:
    std::unique_ptr<World> m_current_world;
};
```

The choice between a raw pointer and a smart pointer depends on the ownership
rules of the engine. A raw `Application*` in `World` is commonly a non-owning
back-pointer: the world refers to the application but does not destroy it.

## Common Errors

### Invalid class declaration

This is invalid and also declares the wrong class name:

```cpp
class Application: class World {
```

Use this when declaring a world:

```cpp
class World {
```

### Missing semicolon in a member declaration

Every member-function declaration in a class needs a semicolon:

```cpp
void tick_internal(float time);
```

### Missing class qualifier in a definition

Use `World::` when defining a `World` member outside the class:

```cpp
void World::start_play() {
}
```

### Why autocomplete stops working

Syntax errors in a header can prevent the compiler and the editor's language
server from constructing a valid representation of the class. As a result,
member completion after `this->` or `world.` may disappear.

Fixing the invalid class declaration, adding the missing semicolon, and using
the correct `World::` qualifiers allows the editor to recognize the class and
its members again.
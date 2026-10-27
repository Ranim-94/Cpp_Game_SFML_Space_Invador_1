

## Table of Contents

- [Table of Contents](#table-of-contents)
- [Introduction](#introduction)
- [Desing Concept](#desing-concept)

## Introduction

The actor class represents an interactive object in the game world.

It's owned by a world instance and managed by the game framework.

Now each actor is going to need position and transformations.

And that's going to be something that we do in the future.

And then we have a way to render itself.

And then we also need the lifecycle management.

- So what happens when we have beginplay.
- What does it do during the tick.
- And then what does it do during the destruction.

## Desing Concept

We need the world to manage the list of all of our active actors.

And then we also need to think about actors that are pending.

And so what this means is what if we're iterating through a list of actors and then we add one in throughout the game, we need to have kind of a space that that actor that's supposed to go in has a place to live until the iterations have all finished.
So that way we don't have any dangling pointers or anything like that.
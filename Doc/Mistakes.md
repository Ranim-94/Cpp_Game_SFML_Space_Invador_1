# Errors and Mistaks

## Table of Contents

- [Errors and Mistaks](#errors-and-mistaks)
  - [Table of Contents](#table-of-contents)
  - [Purpose of this doc](#purpose-of-this-doc)
  - [Window Context](#window-context)
    - [Correct approach](#correct-approach)
    - [Wrong approach](#wrong-approach)
    - [Why the wrong approach gets stuck](#why-the-wrong-approach-gets-stuck)

## Purpose of this doc

Document my coding and concept mistakes while working on the project

## Window Context

SFML's `pollEvent()` checks the window's event queue and consumes an event when one is available. It returns an empty `std::optional` when there are no more events to process. The event loop should keep polling until the queue is empty, then continue rendering the frame.

### Correct approach

Call `pollEvent()` as part of the inner `while` condition. Each iteration retrieves the next event, and the loop ends when there are no more events:

```cpp
while(window.isOpen()){

while (const std::optional window_event = window.pollEvent()) {
    if (window_event->is<sf::Event::Closed>()) {
        window.close();
    }
}

} // End outer while()
```

The outer loop keeps the window running. The inner loop handles all events currently waiting in the queue before the frame is cleared and displayed.

### Wrong approach

In this approach I called the `window.pollEvent()` **before the `while()` loop**.


This version polls only once, before entering the loop:

```cpp

while(window.isOpen()){

const std::optional window_event = window.pollEvent();

while (window_event) {
    if (window_event->is<sf::Event::Closed>()) {
        window.close();
    }
}

} // End outer while()
```

### Why the wrong approach gets stuck

`window_event` is assigned once and never changes inside the loop. If `pollEvent()` returned an event, the optional remains engaged, so `while (window_event)` stays true forever, and the function `pollEvent()` will never be called again.

The program neither polls for another event nor discovers that the queue is empty. If the initial poll returns no event, the loop is skipped instead.

Putting `window.pollEvent()` in the loop condition fixes this: every iteration consumes the next available event, and an empty result ends the inner loop.


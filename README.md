# olympusMTG
Magic: The Gathering implementation in C++

This project is supposed to implement the complete rules of Magic: the Gathering in C++, and allow to play against bots or other players over the network. It is a clone of M:tG Arena or M:tG Online.

It is currently a work in progress; most parts do not work. My focus was more on adding new features and discovering new C++ concepts, and less on having an actual working program.

## Requirements

The program uses POSIX headers for TCP networking, and will only work on POSIX systems (not on Windows).
Testing uses the [Catch2](https://github.com/catchorg/Catch2) framework.

## Building

The project uses [CMake](https://cmake.org) as a meta-build system.

## Architecture & project organization

Game objects, such as cards, are defined in `include/classes`. The `oracles` subfolder defines how to serialize and deserialize these objects.

High-level gameplay definitions, such as steps and phases, are implemented in the `control` folder. Lower-level implementations of game behavior are found in the `gameplay` folder.

The `yggdrasil` folder contains Yggdrasil, a template-heavy, complicated sort-of database containing game objects.

### Agents

A game is an interaction between the game rules and decision-makers, represented by the interface Agent. Different implementations of Agents exist:

- Local agents are players with direct access to the server. A local agent communicates decisions via the user interface, implemented in the `frontend` folder.
- Network agents are connected via the network; their decisions are communicated to the server via a TCP socket.
- Bot agents make automated decisions. Currently, this means picking randomly and/or "the first option".
- (future feature) Python agents make decisions based on a Python script.

The `client` subfolder contains the code for the client, which is another program that connects to the server and sends decisions via TCP. The client asks the user for decisions, using the same interface than a local agent.

### The user interface

The user interface is found in the `frontend` folder. It has two different implementations, CLI (command-line interface) and GUI (graphical user interface). The GUI itself is composed of two parts:
- the (low-level) I/O library implements the AbstractIO interface and defines how exactly things are printed to the screen and how to receive user input. There are again two implementations of I/O, one using the [Ncurses](https://invisible-island.net/ncurses/announce.html) and the other using [Allegro](https://liballeg.org/)
- the (high-level) windowing system defines how the game elements are mapped onto different zones of the screen. Basically, we are using an MVC pattern, where the windowing system (the controller) maps the game elements (the model) onto the I/O elements (the view).

## Future directions

- Implement Python agents
- Make networking more portable
- Allow networking over WebSockets and JSON, instead of TCP and (the current binary format we're using)
- This would allow implementing a client in Javascript
- Allow choosing an I/O library at runtime instead of compile-time

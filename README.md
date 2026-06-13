# Minigin

Minigin is a small student Game-Engine based on the Howest Digital Arts & Entertainment - Programming 4 engine template of the same name. It was made as an exam assignment for said Programming 4 subject.

Minigin is available at https://github.com/TychovanEngelenburg/Minigin_Workspace/

# Goal

My goal was to make a small GameEngine, to showcase my knowledge of programming patterns and proper code architecture conventions. To show off what this engine can do I have used it to make a recreation of the 1982 Tron arcade game.

# Features
The engine has many features, these are the most important:
- Scenegraph system featuring component based gameobject
- Built in gamepad and keyboard support
- Built in audio system
- Easily expandable audio and input systems.
- Collisionsystem with support for boxcolliders and raycasting

As this engine is meant to showcase program pattern usage here are some of the used patterns:
- Update
- Scene
- Entity Component System
- Command
- Observer
- Event queue
- Service locator
- Object pool
- State

# Build
Make sure to always pull the latest version of minigin from https://github.com/TychovanEngelenburg/Minigin_Workspace/

## Windows version

Recommended build procedure:
Either
- Open the root folder in Visual Studio 2026; this will be recognized as a cmake project.
  
Or
- Install CMake 
- Install CMake and CMake Tools extensions in Visual Code
- Open the root folder in Visual Code,  this will be recognized as a cmake project.


## Emscripten (web) version

### On windows

For installing all of the needed tools on Windows I recommend using [Chocolatey](https://chocolatey.org/). You can then run the following in a terminal to install what is needed:

    choco install -y cmake
    choco install -y emscripten
    choco install -y ninja
    choco install -y python

In a terminal, navigate to the root folder. Run this: 

    mkdir build_web
    cd build_web
    emcmake cmake ..
    emmake ninja

To be able to see the webpage you can start a python webserver in the build_web folder

    python -m http.server

Then browse to http://localhost:8000 and you're good to go.

### On OSX

On Mac you can use homebrew

    brew install cmake
    brew install emscripten
    brew install python

In a terminal on OSX, navigate to the root folder. Run this: 

    mkdir build_web
    cd build_web
    emcmake cmake .. -DCMAKE_OSX_ARCHITECTURES=""
    emmake make

To be able to see the webpage you can start a python webserver in the build_web folder

    python3 -m http.server

Then browse to http://localhost:8000 and you're good to go.

## Github Actions

This project is build with github actions.
- The CMake workflow builds the project in Debug and Release for Windows and serves as a check that the project builds on that platform.
- The Emscripten workflow generates a web version of the project and publishes it as a [github page](https://avadae.github.io/minigin/). 
  - The url of that page will be `https://<username>.github.io/<repository>/`
- You can embed this page with 

```<iframe style="position: absolute; top: 0px; left: 0px; width: 1024px; height: 576px;" src="https://<username>.github.io/<repository>/" loading="lazy"></iframe>```

# Credits 
This project makes uses of the Digital Arts & Entertainment Minigin template:
https://github.com/avadae/minigin

As well as the following third-party libraries:

- SDL3 — Simple DirectMedia Layer
  https://github.com/libsdl-org/SDL

- SDL3_ttf — SDL text rendering extension
  https://github.com/libsdl-org/SDL_ttf

- SDL3_mixer — SDL audio and music playback extension
  https://github.com/libsdl-org/SDL_mixer

- GLM (OpenGL Mathematics) — Header-only mathematics library for graphics software
  https://github.com/g-truc/glm

- XInput (Windows only) — Microsoft game controller input API
  https://learn.microsoft.com/en-us/windows/win32/xinput/xinput-game-controller-apis

- Visual Leak Detector (Windows only, development builds)
  https://github.com/KindDragon/vld

- Emscripten (Web builds)
  https://emscripten.org/

And these Assets:

- Joystix Monospace
  https://typodermicfonts.com/joystix-monospace/

- Tron 1982 textures
These have manually been extracted from imagery of the original game by "Bally Midway"(Now defunct)

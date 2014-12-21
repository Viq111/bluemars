BlueMars - A modular procedural terrain generator [![Build Status](https://travis-ci.org/Viq111/bluemars.svg?branch=master)](https://travis-ci.org/Viq111/bluemars)
========

## Dependencies
The BlueMars library does not require any dependancies. However the 2DVisualizer relies on [SMFL](http://www.sfml-dev.org) & [SFGUI](http://sfgui.sfml-dev.de/).

You can build BlueMars on any compiler compliant with C++11 and above. Currently it has been tested on Windows, Linux & OS X with MSVC12 (Visual Studio 2013) and gcc > 4.8.

If you need to compile the 2DVisualizer, you will need to first have SFML & SFGUI

- You can get SFML from source ([here](http://www.sfml-dev.org)) or via your favorite repo manager. (Tested on SMFL 2.1)
- It's better to build SFGUI from source ([here](http://sfgui.sfml-dev.de/))

Once you have downloaded the file you can simply do:
```
cd <source_dir>
cmake .
make
make install
```

Be sure that `FindSFML.cmake` & `FindSFGUI.cmake` are placed inside the CMake module directory.

## Building BlueMars

BlueMars uses CMake to be platform-independant. You can either use CMake command-line of CMake GUI.
3 flags are availables for compilation:

- `BUILD_SHARED_LIBS` enables to build as shared lib, disables to build static
- `BlueMars_BUILD_2D_VISUALIZER` builds the 2D Visualizer (you need SFML & SFGUI)
- `BlueMars_BUILD_UNITTEST` builds the unittests (gtests dependancy already included)

For example, the following commands will let you build the BlueMars library + the 2D Visualizer
```
mkdir build && cd build
cmake -DBlueMars_BUILD_2D_VISUALIZER=TRUE ..
make
```

## Using the library

BlueMars does all the heavy lifting on random generation, caching and chunks continuity to create seemless infite-sized maps.

The core of BlueMars is based on the layer-management system.
Each part of your map: altitude, biome, vegetation, animals, civilization can be represented as a layer dependant on some others.

The most simple Layer implemented is the most versatile one: `ElevationLayer`. It is a Perli Noise-generated layer that you can depend on in your other layers. (For example, you can create a Biome Layer based on 2 "ElevationLayer", one for elevation and one for humidity.

For more examples, feel free to play with the 2D Visualizer.
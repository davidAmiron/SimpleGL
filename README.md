# SimpleGL
A simple graphics library based on OpenGL.

## Dependencies
This project depends on GLEW and SDL2. Install the appropriate versions on your computer.

TODO: Add more installation instructions for GLEW and SDL2

## Installation
Clone the repository
```bash
git clone git@github.com:davidAmiron/SimpleGL.git
```

Make the build space
```bash
cd simplegl
mkdir build
cd build
```

Build
```bash
cmake ..
make install
```

## Usage
In your CMakeLists.txt put the following lines:
```cmake
find_package(SimpleGL REQUIRED)
target_link_libraries([your_target] simplegl)
```
replace [your\_target] with the name of your target.

Now in your code, use the following:
```cpp
#include "simplegl.h"
```

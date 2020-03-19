# SimpleGL
A simple graphics library based on OpenGL.

Currently being developed on Mac, might be made more general for other systems later.

## Dependencies
This project depends on GLEW and SDL2. The relevant librares are currently included in this repository, however are only for mac.

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
replace [your\_target] with the name of your target

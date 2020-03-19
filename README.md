# SimpleGL
A simple graphics library based on OpenGL

Currently being developed on Mac, but might work on other systems

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

# SimpleGL
A simple graphics library based on OpenGL

## Installation
Clone the repository
```bash
git clone git@github.com:davidAmiron/SimpleGL.git
```

Make the build space (inside the cloned repository)
```bash
mkdir build
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

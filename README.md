# Description

Playground for bunch of OpenGL-related libraries.

Individual projects are within `projects/`.

# Dependencies

Initialize dependencies with:

`git submodule update --init --recursive`

and

`$ sh init_submodules.sh` - if you're on windows use MSYS `sh`

# Building

```sh
$ mkdir build
$ cd build
$ cmake .. [-DBUILD_ASSIMP_101]
$ make -j4
```

to build `assimp_101` project or `BUILD_ALL_DESKTOP_PROJECTS` to build all projects.

Results will be inside `<git-root>/bin` folder.

For list of all projects' variables take a look at `<git-root>/Projects.cmake`.

# Running the examples

Compiled output will be placed into `<git-root>/bin` folder.

**You must run compiled examples from the `bin` folder.**

# Windows requirements

* Git for windows in PATH with the rest of the tools - sed, unzip, sh is required in path
* wget - you can get it [here](http://gnuwin32.sourceforge.net/packages/wget.htm) - exe and dependencies need to be in the same folder which needs to be in PATH
* cmake
* Supported VS 2015
* Go to VS command line and then use build commands to build projects

git prompt:

`$ git submodule update --init --recursive -- lib`

Developer Command Prompt for VS2015:

`> cmake -DBUILD_ASSIMP_101=ON ..`

# Libraries

* GLEW
* GLFW
* ASSIMP - asset importer
* FreetypeGL - fonts
* PhysFS
* alure - OpenAL library + ogg and vorbis as dependencies
* DevIL - Developer's Image Library
* glm
* eigen
* easyloggingpp
* argh - argument parser

# License

MIT unless specified otherwise in source files.

See [License](LICENSE)

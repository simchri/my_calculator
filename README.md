# MyCalc

A Terminal-UI calculator.
This is purely a pet-project to practice coding and figure out a dev-friendly C++ repo structure.

## Build & Test

### Prerequisites:
- [Docker](https://docs.docker.com/engine/install/) correctly installed according to instructions for your platform. (Note: Development setup tested only on Ubuntu 20.04 with Docker version 27.2.1.)

### Open Dev Shell

The helper script
```
./devenv.bash
```
Does the following on first run:
- build a docker image with all build dependencies
- start a container from created image
- mount this project directory
- mount some files under `~`. This is so that the setup works well with IDEs that use the `.devcontainer` file and in particular with IDE git integrations. The docker process impersonates your regular user (not root) and can access your ssh keys. With this setup, the IDE can write to source files without causing ownership trouble.
- drop into a container shell

Perform all build, test & run commands in this dev shell.
If you want to do a host build instead, refer to the `docker/Dockerfile` for dependencies to be installed. Be aware that the base image may hide additional dependencies.

### Build

```
mkdir -p build && cd build
cmake .. -GNinja && cmake --build . --target package
```

### Test

All fast tests only:
```
ctest --output-on-failure -L fast
```
All others:
```
ctest --output-on-failure -LE fast
```
The test suite includes a google benchmark performance test, supplemented by a backstop logic. The performance is checked against a reference, if it has deteriorated, the test fails. This means this test can arbitrarily fail, when the check is run on a slower system. Exclude it from the suite with `-LE perf`.
Update the reference by replacing the file `components/parsing/test/baseline.json` with a recent result (`build/components/parsing/test/contender.json`)

Quicktest:
```
clear && cmake --build . --target all && ctest -E benchmark # quicktest
```
### Run
```
./build/apps/my_calc/my_calc
```

# Plugins
**Plugins application in c++**

## Installation

You have to clone the repo

```bash
git clone git clone <repo-url>
cd Plugins
```

When you are in the project, create a folder "build"

```bash
mkdir build
```

## Compilation

### Dependencies

Compile dependencies for the project

```bash
cmake -S . -B build
```

#### Tests

If you want to compile tests, do this:

```bash
cmake -DCAMI_BUILD_TESTS=ON -S . -B build
```

#### Benchmarks

If you want to compile benchmarks, do this:

```bash
cmake -DCAMI_BUILD_BENCHMARKS=ON -S . -B build
```

### Build

After dependencies, compile your build

```bash
cmake --build build
```

## Executable

Execute plugins

```bash
./plugins_cami
```

Execute tests

```bash
./tests_plugins_cami
```

Execute benchmarks

```bash
./benchmarks_plugins_cami
```

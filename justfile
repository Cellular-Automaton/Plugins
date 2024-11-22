# lint files with clang-format
lint:
    @clang-format -i --style='file:.clang-format' `find ./ -name *.cpp -o -name *.hh -o -name *.hpp`

set windows-shell := ["powershell.exe", "-NoLogo", "-Command"]
set shell := ["bash", "-uc"]

executable := "cami"

build_folder    :=  "build"

basic_options   := '-DCMAKE_EXPORT_COMPILE_COMMANDS=true -DCMAKE_CXX_COMPILER_LAUNCHER=ccache -DCMAKE_POSITION_INDEPENDENT_CODE=ON'
no_release  :=  '-DUSE_CLANG_TIDY=false -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ '

# build in release mode
release:
    cmake -B {{ build_folder }} -GNinja -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ {{ basic_options }} &&\
    ninja -C {{ build_folder }} && cd {{ build_folder }} && sudo cpack --config CPackConfig.cmake

# build with debug flags, using ninja
debug:
    cmake -B {{ build_folder }} -GNinja -DCMAKE_BUILD_TYPE=Debug {{ no_release }} {{ basic_options }} &&\
    ninja -C {{ build_folder }}

# build executable with ninja
ninja:
    cmake -B {{ build_folder }} -GNinja -DCMAKE_BUILD_TYPE=None {{ no_release }} {{ basic_options }} &&\
    ninja -C {{ build_folder }}

# build with thread sanitizer flags activated, good for multithreading apps
tsan:
    cmake -B {{ build_folder }} -GNinja -DCMAKE_BUILD_TYPE=tsan {{ no_release }} {{ basic_options }} &&\
    ninja -C {{ build_folder }}

# clean files produced by test (.gcda, .gcno)
clean_tests:
    @find -name *.gc* -delete

# build tests executables and put them in `tests` folder
tests: clean_tests
    cmake -B {{ build_folder }} -GNinja -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ {{ basic_options }} -DCAMI_BUILD_TESTS=true &&\
    ninja -C {{ build_folder }}

# build benchmarks and put them in `benchmarks` folder
benchmarks:
    cmake -B {{ build_folder }} -GNinja -DCMAKE_BUILD_TYPE=Release {{ no_release }} {{ basic_options }} -DCAMI_BUILD_BENCHMARKS=true &&\
    ninja -C {{ build_folder }}

# clean executables
clean:
    @rm -rf {{ executable }}

# clean executable and build folder
fclean: clean
    @rm -rf build tests libs assets

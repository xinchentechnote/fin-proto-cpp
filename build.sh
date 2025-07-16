
git config --global url."git@github.com:".insteadOf https://github.com/
rm -rf build
cmake -B build -DCMAKE_BUILD_TYPE=Debug --log-level=VERBOSE -DBUILD_TESTING=ON -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
cmake --build build
cd build && ctest -V
cd -

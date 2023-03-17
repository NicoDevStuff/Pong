#!/bin/sh
# super fancy build script
has_param() {
    local term="$1"
    shift
    for arg; do
        if [[ $arg == "$term" ]]; then
            return 0
        fi
    done
    return 1
}

debug() {
    premake5 gmake
    echo -e "\033[32m\033[1m=== Compiling in debug mode ===\033[32m\n\033[1m\033[0m"
	make config=debug
    ./build/Debug/Pong
    echo -e "\033[32m\n\033[1m=== Done running in debug mode ===\033[32m\n\033[1m\033[0m"
}

release() {
    premake5 gmake
    echo -e "\033[32m\033[1m=== Compiling in release mode ===\033[32m\n\033[1m\033[0m"
	make config=release
    ./build/Release/Pong
    echo -e "\n\033[32m\033[1m=== Done running in release mode ===\033[32m\n\033[1m\033[0m"
}

debugger() {
    premake5 gmake
    echo -e "\033[32m\033[1m=== Compiling in debug mode ===\033[32m\n\033[1m\033[0m"
	make config=debug
    valgrind --leak-check=full build/Debug/Pong
    echo -e "\n\033[32m\033[1m=== Done running in debug mode ===\033[32m\n\033[1m\033[0m"

}

if has_param '-d' "$@"; then
    debug
elif has_param '-r' "$@"; then
    release
elif has_param '-debugger' "$@"; then
    debugger
elif has_param '-h' "$@"; then
    echo -e "\n=== HELP ===\n-d for debug\n-r for release\n-h for help\n-deps for installing all of the dependencies"
elif has_param '-deps' "$@"; then
    echo -e ""
    ./install_deps-arch.sh
else
    echo -e "\033[31m\n\033[1m=== ERROR! Select either Run or Release mode; -h for help ===\n\033[0m\033[1m\033[0m"
fi

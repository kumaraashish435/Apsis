cmake_minimum_required(VERSION 3.20)
project(Apsis LANGUAGES CXX)

# ctest only discovers tests below directories where enable_testing() ran in
# THIS (the top-level) scope, not just in the included subdirectory below.
enable_testing()

# apps/engine is self-contained and independently buildable; this just wires
# it into a root-level build. Other apps/ directories (web, api, ...) are
# separate stacks with their own toolchains and aren't part of this build.
add_subdirectory(apps/engine)

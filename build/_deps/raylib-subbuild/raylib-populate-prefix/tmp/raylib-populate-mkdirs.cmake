# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/workspaces/chip-8/build/_deps/raylib-src"
  "/workspaces/chip-8/build/_deps/raylib-build"
  "/workspaces/chip-8/build/_deps/raylib-subbuild/raylib-populate-prefix"
  "/workspaces/chip-8/build/_deps/raylib-subbuild/raylib-populate-prefix/tmp"
  "/workspaces/chip-8/build/_deps/raylib-subbuild/raylib-populate-prefix/src/raylib-populate-stamp"
  "/workspaces/chip-8/build/_deps/raylib-subbuild/raylib-populate-prefix/src"
  "/workspaces/chip-8/build/_deps/raylib-subbuild/raylib-populate-prefix/src/raylib-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/workspaces/chip-8/build/_deps/raylib-subbuild/raylib-populate-prefix/src/raylib-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/workspaces/chip-8/build/_deps/raylib-subbuild/raylib-populate-prefix/src/raylib-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()

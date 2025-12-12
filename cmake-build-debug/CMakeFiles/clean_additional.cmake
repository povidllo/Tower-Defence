# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "editor/CMakeFiles/editor_autogen.dir/AutogenUsed.txt"
  "editor/CMakeFiles/editor_autogen.dir/ParseCache.txt"
  "editor/editor_autogen"
  "engine/CMakeFiles/engine_autogen.dir/AutogenUsed.txt"
  "engine/CMakeFiles/engine_autogen.dir/ParseCache.txt"
  "engine/engine_autogen"
  )
endif()

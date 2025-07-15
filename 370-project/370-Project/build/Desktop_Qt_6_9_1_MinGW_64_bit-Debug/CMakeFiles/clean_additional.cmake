# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\WhatAmIDoing_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\WhatAmIDoing_autogen.dir\\ParseCache.txt"
  "WhatAmIDoing_autogen"
  )
endif()

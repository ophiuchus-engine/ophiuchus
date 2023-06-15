macro(enable_clang_tidy WARNINGS_AS_ERRORS)
  find_program(CLANGTIDY clang-tidy)
  if(CLANGTIDY)
    # construct the clang-tidy command line
    set(CLANG_TIDY_COMMAND
        ${CLANGTIDY} -extra-arg=-Wno-unknown-warning-option -extra-arg=-Wno-ignored-optimization-argument
        -extra-arg=-Wno-unused-command-line-argument -p)

    if(NOT
       "${CMAKE_CXX_STANDARD}"
       STREQUAL
       "")
      if("${CLANG_TIDY_OPTIONS_DRIVER_MODE}" STREQUAL "cl")
        set(CLANG_TIDY_COMMAND ${CLANG_TIDY_COMMAND} -extra-arg=/std:c++${CMAKE_CXX_STANDARD})
      else()
        set(CLANG_TIDY_COMMAND ${CLANG_TIDY_COMMAND} -extra-arg=-std=c++${CMAKE_CXX_STANDARD})
      endif()
    endif()

    # set warnings as errors
    if(${WARNINGS_AS_ERRORS})
      list(APPEND CLANG_TIDY_COMMAND -warnings-as-errors=*)
    endif()

    message("Setting clang tidy at ${CLANGTIDY}")
    set(CMAKE_CXX_CLANG_TIDY ${CLANG_TIDY_COMMAND})
  else()
    message(WARNING "clang-tidy not found.")
  endif()
endmacro()

macro(standard_settings)
  set(CMAKE_CXX_STANDARD 20)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
  set(CMAKE_CXX_EXTENSIONS OFF)

  include("cmake/InterproceduralOptimization.cmake")
  myproject_enable_ipo()

  include("cmake/EnhanceErrorReporting.cmake")
  enhance_error_reporting()

  set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

  include("cmake/StaticAnalyzers.cmake")
  #enable_clang_tidy(ON)
endmacro()

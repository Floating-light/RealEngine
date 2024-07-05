add_library(Core)

add_subdirectory(Logging)

target_sources(Core
  PRIVATE
    "Core.h"
    "Core.cpp"

    Module/ModuleManager.h
    Module/ModuleManager.cpp
    Math/Matrix4.h
    Math/Matrix4.cpp
    Math/Vector4D.h
    Math/Color.cpp
    Math/MathUtility.h
    Math/MathUtilityDefine.h
    Math/Quat.h
    Math/RealSSE.h
    Math/Rotator.h
    Math/Transform.h
    Math/Vector.h
    Math/Vector2D.h
    Math/Math.cpp
    Math/Matrix3.h
) 

target_include_directories(Core
  PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}"
    ${CMAKE_CURRENT_SOURCE_DIR}/Math
)

if(WIN32)
  target_sources(Core
    PRIVATE  
      Windows/WindowsMisc.h
      Windows/WindowsMisc.cpp
      Windows/WindowsPlatformMisc.h
      Windows/WindowsPlatformMisc.cpp

  )
  target_include_directories(Core
    PUBLIC
      "${CMAKE_CURRENT_SOURCE_DIR}/Windows"
  ) 
endif()

set_target_properties(Core PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(Core 
  PUBLIC
    Logging
    DirectXMath
)


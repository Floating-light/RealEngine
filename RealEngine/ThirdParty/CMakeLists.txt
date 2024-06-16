# use submodule https://zhuanlan.zhihu.com/p/87053283

if(NOT TARGET assimp)
    add_subdirectory(assimp)
endif()

if(NOT TARGET spdlog)
    set(SPDLOG_WCHAR_TO_UTF8_SUPPORT ON)
    add_subdirectory(spdlog)
endif()

add_subdirectory(DirectXTex)

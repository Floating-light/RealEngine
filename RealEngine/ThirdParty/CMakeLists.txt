# use submodule https://zhuanlan.zhihu.com/p/87053283
# 自定义函数设置解决方案文件夹
function(set_solution_folder project_name folder_name)
    set_target_properties(${project_name} PROPERTIES FOLDER ${folder_name})
endfunction()

# 设置 ThirdPart 文件夹中的所有项目
function(set_folder_recursive source_dir folder_name)
    # 获取目录下的所有 targets
    get_property(SUBDIRECTORIES DIRECTORY ${source_dir} PROPERTY SUBDIRECTORIES)
    foreach(subdir ${SUBDIRECTORIES})
        # 获取所有的 targets
        get_property(SUBDIR_TARGETS DIRECTORY ${subdir} PROPERTY BUILDSYSTEM_TARGETS)

        foreach(target ${SUBDIR_TARGETS})
            set_solution_folder(${target} ${folder_name})
        endforeach()

        # 递归调用本函数对子目录处理
        set_folder_recursive(${subdir} ${folder_name})
    endforeach()
endfunction()

if(NOT TARGET assimp)
    add_subdirectory(assimp)
endif()

if(NOT TARGET spdlog)
    set(SPDLOG_WCHAR_TO_UTF8_SUPPORT ON)
    add_subdirectory(spdlog)
endif()

add_subdirectory(DirectXTex)
add_subdirectory(DirectXMath)
# set_target_properties(DirectXTex PROPERTIES FOLDER "ThirdParty")
# 获取当前CMakeLists文件所在的目录
set(THIRDPART_DIR ${CMAKE_CURRENT_SOURCE_DIR})
# 执行函数将 ThirdPart 目录下的所有项目放入 "ThirdPart" 文件夹
set_folder_recursive(${THIRDPART_DIR} "ThirdParty")
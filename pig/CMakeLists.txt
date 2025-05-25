cmake_minimum_required(VERSION 3.14)
project(main_pig)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置构建类型
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# 添加编译选项
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")

# 直接指定 predicates.c 文件路径
set(PREDICATES_C_FILE "${CMAKE_CURRENT_SOURCE_DIR}/../LayoutEmbedding/library/LayoutEmbedding/predicates.c")

if(EXISTS ${PREDICATES_C_FILE})
    message(STATUS "Found predicates.c at: ${PREDICATES_C_FILE}")
    # 编译 predicates.c 为静态库
    add_library(predicates STATIC ${PREDICATES_C_FILE})
    set_target_properties(predicates PROPERTIES
        C_STANDARD 99
        POSITION_INDEPENDENT_CODE ON
    )
    # 禁用可能的警告
    target_compile_options(predicates PRIVATE
        -Wno-unused-but-set-variable
        -Wno-unused-variable
        -Wno-implicit-function-declaration
    )
    set(HAS_PREDICATES TRUE)
else()
    message(FATAL_ERROR "predicates.c not found at ${PREDICATES_C_FILE}")
    set(HAS_PREDICATES FALSE)
endif()

# 引入 LayoutEmbedding 库
add_subdirectory("../LayoutEmbedding" layout_embedding_build)

# 查找必要的库
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

# 创建可执行文件
add_executable(main_pig main_pig.cpp)

# 设置包含目录
target_include_directories(main_pig PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../LayoutEmbedding/library
    ${CMAKE_CURRENT_SOURCE_DIR}/../LayoutEmbedding/extern
    ${CMAKE_CURRENT_SOURCE_DIR}/../LayoutEmbedding/extern/glow/src
    ${CMAKE_CURRENT_SOURCE_DIR}/../LayoutEmbedding/extern/glow-extras/src
    ${CMAKE_CURRENT_SOURCE_DIR}/../LayoutEmbedding/extern/polymesh/src
    ${CMAKE_CURRENT_SOURCE_DIR}/../LayoutEmbedding/extern/typed-geometry/src
)

# 链接库 - predicates 必须放在最前面
if(HAS_PREDICATES)
    target_link_libraries(main_pig PRIVATE
        predicates          # 必须在 layout_embedding 之前
        layout_embedding
        ${OPENGL_LIBRARIES}
        glfw
        dl
        pthread
    )
else()
    target_link_libraries(main_pig PRIVATE
        layout_embedding
        ${OPENGL_LIBRARIES}
        glfw
        dl
        pthread
    )
endif()

# 如果系统有数学库，也需要链接
if(UNIX AND NOT APPLE)
    target_link_libraries(main_pig PRIVATE m)
endif()

# 设置预处理器定义
target_compile_definitions(main_pig PRIVATE
    LE_DATA_PATH="${CMAKE_CURRENT_SOURCE_DIR}/../LayoutEmbedding/data"
)
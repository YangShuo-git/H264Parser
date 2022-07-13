cmake_minimum_required(VERSION 3.0.0)
project(ReadH264)

#include_directories (../)    #这个注释掉 也是没有影响

file(GLOB SRC_LIST     "*.cpp")
file(GLOB INCLUDE_LIST "*.h")

add_executable(ReadH264 ${SRC_LIST} ${INCLUDE_LIST})   #cpp文件和h文件都需要吗？


file(
    COPY         ${CMAKE_CURRENT_SOURCE_DIR}/../brave.h264
    DESTINATION  ${CMAKE_CURRENT_BINARY_DIR}/
)

INSTALL(
        TARGETS ReadH264
        RUNTIME DESTINATION bin     #可执行目标二进制
        LIBRARY DESTINATION lib     #动态库
        ARCHIVE DESTINATION lib     #静态库
)
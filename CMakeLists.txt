#
#    FileName : CMakeLists.txt
#    By : yzx
#    MakeTime : 2024年9月3日 下午 07:56:33
#    Copyright (C) 2023-2024 openGames
#
cmake_minimum_required(VERSION 3.10)
project(md5gen)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -O2")

# 添加可执行文件
add_executable(md5gen
md5gen.c
MakeMd5File.c
GetMd5File.c
modules/cJSON.c
modules/HashVal.c
modules/CRC32.c
)

# 链接库
target_link_libraries(md5gen ssl crypto)

# 安装
install(TARGETS md5gen
RUNTIME DESTINATION bin)
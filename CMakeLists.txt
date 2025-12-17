cmake_minimum_required(VERSION 3.11)
project(GIF_ascii)

set(CMAKE_CXX_STANDARD 17)

# OpenCVを探す
find_package(OpenCV REQUIRED)

add_executable(app main.cpp)

# Windowsコンソールアプリとして設定（WinMainエラー回避）
if(MINGW)
    target_link_options(app PRIVATE -mconsole)
endif()

# OpenCVをリンク
target_link_libraries(app PRIVATE ${OpenCV_LIBS})
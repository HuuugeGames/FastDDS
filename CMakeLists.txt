cmake_minimum_required(VERSION 2.8)
project( FastDDS )

#find_package(PNG REQUIRED)
#include_directories(${PNG_INCLUDE_DIR})

add_executable( FastDDS main.cpp contrib/DXTCompressorDLL.cpp contrib/Bitmap.cpp contrib/BitmapDownsampled.cpp contrib/Tables.cpp )
#target_link_libraries( FastDDS ${PNG_LIBRARY} )

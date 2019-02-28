cmake_minimum_required(VERSION 3.5)

#if(MSVC)
#    add_compile_options(/W4)
#else()
#    add_compile_options(-Wall -Wextra -Wpedantic)
#endif()

set(CMAKE_BUILD_TYPE Release)

include_directories(pge)


add_executable(editor
    main.cpp
    pge.cpp
    resource/road_skin.cpp
    states/edit.cpp
)
set_property(TARGET editor PROPERTY CXX_STANDARD 17)

if(UNIX)
    find_package(Threads)
    target_link_libraries(editor X11 GL png ${CMAKE_THREAD_LIBS_INIT})
endif()

#target_compile_features(editor ...)

if (APPLE)
    # Find packages
    add_subdirectory(3rdparty)
    find_package(SDL2 REQUIRED)
    find_package(freetype REQUIRED)
    find_package(ffmpeg REQUIRED)


    file(GLOB_RECURSE SDLMS_HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/sdlms/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/sdlms/*.hpp")
    file(GLOB_RECURSE SDLMS_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/sdlms/*.cpp")
    file(GLOB SDLMS_FILES ${SDLMS_HEADER_FILES} ${SDLMS_SOURCE_FILES})

    set(wzlibcpp ${PROJECT_SOURCE_DIR}/sdlms/3rdparty/wzlibcpp)


    add_executable(sdlMS ${CMAKE_CURRENT_SOURCE_DIR}/sdlms/main.cpp ${SDLMS_FILES})

    set_target_properties(sdlMS PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    )

    # macro to set the debug flag
    target_compile_definitions(sdlMS PUBLIC ECS_DEBUG)

    target_include_directories(sdlMS PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}/sdlms
            ${wzlibcpp}
            ${wzlibcpp}/include
            ${wzlibcpp}/mio/include
    )


    target_link_libraries(sdlMS PRIVATE SDL2::SDL2main
            PRIVATE SDL2::SDL2-static
            PRIVATE wzlib
            PRIVATE ffmpeg::avformat ffmpeg::avdevice ffmpeg::avcodec ffmpeg::avutil ffmpeg::swscale ffmpeg::avfilter ffmpeg::swresample
            PRIVATE Freetype::Freetype
    )
endif ()
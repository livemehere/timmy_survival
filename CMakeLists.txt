cmake_minimum_required(VERSION 4.2.1)

project(timmy-survival)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

set(TIMMY_IS_WEB OFF)
if(EMSCRIPTEN)
  set(TIMMY_IS_WEB ON)
endif()

fetchcontent_declare(
  raylib
  GIT_REPOSITORY https://github.com/raysan5/raylib.git
  GIT_TAG 5.5
)

fetchcontent_makeavailable(raylib)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.cpp" "src/*.hpp")

if(APPLE AND NOT TIMMY_IS_WEB)
  add_executable(${PROJECT_NAME} MACOSX_BUNDLE ${SOURCES})
else()
  add_executable(${PROJECT_NAME} ${SOURCES})
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE raylib)
target_compile_definitions(${PROJECT_NAME} PRIVATE TIMMY_SOURCE_DIR="${CMAKE_SOURCE_DIR}")

if(TIMMY_IS_WEB)
  set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".html")
  target_link_options(${PROJECT_NAME} PRIVATE
    "--preload-file" "${CMAKE_SOURCE_DIR}/assets@/assets"
    "-sUSE_GLFW=3"
    "-sALLOW_MEMORY_GROWTH=1"
    "-sASSERTIONS=1"
    "-sWASM=1"
  )
elseif(APPLE)
  add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_SOURCE_DIR}/assets"
            "$<TARGET_BUNDLE_DIR:${PROJECT_NAME}>/Contents/Resources/assets"
    COMMENT "Copying assets into app bundle"
  )
else()
  add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_SOURCE_DIR}/assets"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/assets"
    COMMENT "Copying assets next to executable"
  )
endif()

if(APPLE AND NOT TIMMY_IS_WEB)
  set_target_properties(${PROJECT_NAME} PROPERTIES
    MACOSX_BUNDLE_BUNDLE_NAME "Timmy Survival"
    MACOSX_BUNDLE_GUI_IDENTIFIER "com.timmysurvival.game"
    MACOSX_BUNDLE_BUNDLE_VERSION "1.0.0"
    MACOSX_BUNDLE_SHORT_VERSION_STRING "1.0.0"
  )
endif()

cmake_minimum_required(VERSION 3.26)
project(Minigin)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(EMSCRIPTEN)
  message("Using Emscripten!")
endif()

if(EMSCRIPTEN)
  set(GAME_TARGET TRON1982_web)
  set(TARGET_PARAM )
else()
  set(GAME_TARGET TRON1982)
  set(TARGET_PARAM) #WIN32)
endif()

set(LIBRARY_TARGET minigin_engine)

add_library(${LIBRARY_TARGET} STATIC
    Minigin/Minigin.cpp
    Minigin/Renderer.cpp
    Minigin/ResourceManager.cpp
    Minigin/DeltaClock.cpp
    Minigin/Transform2D.cpp

    Minigin/Scene/GameObject.cpp  
    Minigin/Scene/Scene.cpp
    Minigin/Scene/SceneManager.cpp
    
    Minigin/EngineComponents/TextComponent.cpp
    Minigin/EngineComponents/Sprite.cpp
    Minigin/EngineComponents/Component.cpp 
        
    Minigin/InputHandling/Gamepad.cpp
    Minigin/InputHandling/InputBinding.cpp
    Minigin/SDL_Implementation/SDL_Keyboard.cpp
    Minigin/InputHandling/GameObjectCommand.cpp 
    Minigin/InputHandling/SceneInput.cpp 
    Minigin/InputHandling/InputManager.cpp

    Minigin/SDL_Implementation/SDLFont.cpp
    Minigin/SDL_Implementation/SDLTexture2D.cpp
    Minigin/SDL_Implementation/SDLSoundSystem.cpp
    Minigin/SDL_Implementation/SDL_GamePad.cpp

    Minigin/SoundSystem/SoundServiceLocator.cpp
    
    Minigin/CollisionSystem/CollisionSystem.cpp
    Minigin/CollisionSystem/BoxCollider2D.cpp
    Minigin/InputSystem/AbstractInputSystem.cpp
    Minigin/InputSystem/InputServiceLocator.cpp
    Minigin/SDL_Implementation/SDL_InputSystem.cpp
    Minigin/Helpers/Random.cpp
    Game/Commands/ShootCommand.cpp)
target_include_directories(minigin_engine PUBLIC
  ${CMAKE_SOURCE_DIR}
)
target_compile_features(minigin_engine PUBLIC cxx_std_20)

# list source files here
add_executable(${GAME_TARGET} ${TARGET_PARAM}
    Game/Main.cpp
    Game/GameStates.cpp 
    Game/GameContext.cpp
    Game/DamageOnCollision.cpp
    Game/SceneLoading.cpp
    
    Game/Commands/MoveTankCommand.cpp
    Game/Commands/StartGameCommand.cpp

    Game/Grid/GameGrid.cpp
    
    Game/Tank/TankHealth.cpp
    
    Game/UI/FPS_UI.cpp
    Game/UI/PlayerHealth_UI.cpp
      

    Game/Tank/TankMovement.cpp
    Game/Tank/TankVisuals.cpp
    Game/Tank/Bullet/BulletMovement.cpp 
    Game/Tank/TankBarrel.cpp 
    Game/Tank/TankManager.cpp
    Game/Enemy/EnemyBehaviour.cpp
    Game/Commands/TurnBarrelCommand.cpp
    Game/Tank/Bullet/BulletPool.cpp
    "Game/Config/TankConfig.cpp"
    Game/Tank/RotateWithTank.cpp
 "Game/Enemy/AutoShooting.cpp")

# enable c++20 features
target_compile_features(${GAME_TARGET} PUBLIC cxx_std_20)

include(FetchContent)

# Add xInput
if(WIN32)
    TARGET_LINK_LIBRARIES(${GAME_TARGET} PRIVATE xinput )
endif()

# add glm
find_package(glm CONFIG QUIET)
if(NOT glm_FOUND)
  FetchContent_Declare(
    glm
    URL https://github.com/g-truc/glm/releases/download/1.0.3/glm-1.0.3.zip
    DOWNLOAD_NO_PROGRESS ON
    DOWNLOAD_DIR ${CMAKE_BINARY_DIR}/downloads
  )
  FetchContent_MakeAvailable(glm)
endif()

# Add SDL3
find_package(SDL3 3.4 CONFIG QUIET)
if(NOT SDL3_FOUND)
  if (WIN32)
    FetchContent_Declare(
      SDL3
      URL https://www.libsdl.org/release/SDL3-devel-3.4.0-VC.zip
      DOWNLOAD_NO_PROGRESS ON
      DOWNLOAD_DIR ${CMAKE_BINARY_DIR}/downloads
    )
    FetchContent_MakeAvailable(SDL3)
    list(PREPEND CMAKE_PREFIX_PATH "${sdl3_SOURCE_DIR}")
    find_package(SDL3 CONFIG REQUIRED)
  else()
    FetchContent_Declare(
      SDL3
      GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
      GIT_TAG release-3.4.0
      GIT_SHALLOW TRUE
      GIT_PROGRESS TRUE
    )
    FetchContent_MakeAvailable(SDL3)
  endif()
endif()

# Add SDL3_ttf
find_package(SDL3_ttf 3.2.2 CONFIG QUIET)
if(NOT SDL3_ttf_FOUND)
  if (WIN32)
    FetchContent_Declare(
      SDL3_ttf
      URL https://www.libsdl.org/projects/SDL_ttf/release/SDL3_ttf-devel-3.2.2-VC.zip
      DOWNLOAD_NO_PROGRESS ON
      DOWNLOAD_DIR ${CMAKE_BINARY_DIR}/downloads
    )
    FetchContent_MakeAvailable(SDL3_ttf)
    list(PREPEND CMAKE_PREFIX_PATH "${sdl3_ttf_SOURCE_DIR}")
    find_package(SDL3_ttf CONFIG REQUIRED)
  else()
    FetchContent_Declare(
      SDL3_ttf
      GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git
      GIT_TAG release-3.2.2
      GIT_SHALLOW TRUE
      GIT_PROGRESS TRUE
    )
    set(SDL3TTF_INSTALL OFF)
    if(EMSCRIPTEN)
      list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/emscripten")
      set(SDL3TTF_USE_SYSTEM_FREETYPE ON)
      set(SDL3TTF_VENDORED OFF)
      set(SDLTTF_PLUTOSVG OFF)
      add_compile_options(-sUSE_FREETYPE=1 -sUSE_HARFBUZZ=1)
      add_link_options(-sUSE_FREETYPE=1 -sUSE_HARFBUZZ=1)
    endif()
    FetchContent_MakeAvailable(SDL3_ttf)
  endif()
endif()

# Add SDL3_mixer
find_package(SDL3_mixer 3.2.0 CONFIG QUIET)
if(NOT SDL3_mixer_FOUND)
  if (WIN32)
    FetchContent_Declare(
      SDL3_mixer
      URL https://github.com/libsdl-org/SDL_mixer/releases/download/release-3.2.0/SDL3_mixer-devel-3.2.0-VC.zip
      DOWNLOAD_NO_PROGRESS ON
      DOWNLOAD_DIR ${CMAKE_BINARY_DIR}/downloads
    )
    FetchContent_MakeAvailable(SDL3_mixer)
    list(PREPEND CMAKE_PREFIX_PATH "${sdl3_mixer_SOURCE_DIR}")
    find_package(SDL3_mixer CONFIG REQUIRED)
  else()
    FetchContent_Declare(
      SDL3_mixer
      GIT_REPOSITORY https://github.com/libsdl-org/SDL_mixer.git
      GIT_TAG release-3.2.0
      GIT_SHALLOW TRUE
      GIT_PROGRESS TRUE
    )
    FetchContent_MakeAvailable(SDL3_mixer)
  endif()
endif()


# ONLY REAL CHANGE (ENGINE LINK)
target_link_libraries(minigin_engine PUBLIC 
  SDL3::SDL3 
  SDL3_ttf::SDL3_ttf
  SDL3_mixer::SDL3_mixer
  glm::glm
)

target_link_libraries(${GAME_TARGET} PUBLIC minigin_engine)

target_include_directories("${GAME_TARGET}" PUBLIC 
  ${CMAKE_SOURCE_DIR}/Minigin/
  ${CMAKE_SOURCE_DIR}/Game/
  ${glm_SOURCE_DIR}
  ${sdl3_source_dir}/include
  ${sdl3_image_SOURCE_DIR}/include
  ${sdl3_ttf_SOURCE_DIR}/include
  ${sdl3_mixer_SOURCE_DIR}/include
  ${VLD_INCLUDE_DIR}
)

if (WIN32)
  # Add VLD (Visual Leak Detector) for memory leak detection on Windows
  find_package(VLD CONFIG)
endif()

# Add steam if available
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/steam")
find_package(Steamworks QUIET)

target_include_directories("${GAME_TARGET}" PUBLIC
  ${CMAKE_SOURCE_DIR}/Minigin/
  ${VLD_INCLUDE_DIR}
)

target_link_libraries("${GAME_TARGET}" PUBLIC 
  SDL3::SDL3 
  SDL3_ttf::SDL3_ttf
  SDL3_mixer::SDL3_mixer
  glm::glm
  ${VLD_LIBRARIES}
)

if(MSVC)
  # compile at warning level 4 + treat warnings as errors
  target_compile_options(${GAME_TARGET} PRIVATE /W4 /WX)
else()
  target_compile_options(${GAME_TARGET} PRIVATE -Wall -Wextra -Werror)
endif()



if(EMSCRIPTEN)
  set(EMSCRIPTEN_COMPILE_FLAGS "${EMSCRIPTEN_LINK_FLAGS} -g")
  set(EMSCRIPTEN_LINK_FLAGS "${EMSCRIPTEN_LINK_FLAGS} -sUSE_FREETYPE=1 -sUSE_HARFBUZZ=1 -g -sALLOW_MEMORY_GROWTH --preload-file \"${CMAKE_SOURCE_DIR}/Data@/\"")

  # Use the Emscripten toolchain
  set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/cmake/EmscriptenToolchain.cmake)

  # Additional settings for Emscripten build
  set_target_properties(${GAME_TARGET} PROPERTIES
      COMPILE_FLAGS "${EMSCRIPTEN_COMPILE_FLAGS}"
      LINK_FLAGS "${EMSCRIPTEN_LINK_FLAGS}"
  )

  add_custom_command(TARGET ${GAME_TARGET} POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/web" "$<TARGET_FILE_DIR:${GAME_TARGET}>/"
  )

  # Have emscripten generate a html page too.
  set(CMAKE_EXECUTABLE_SUFFIX ".html")

elseif(WIN32)
  # Set minigin as the default startup project
  set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${GAME_TARGET})
  # Set working directory to output folder
  set_target_properties(${GAME_TARGET} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:${GAME_TARGET}>")

  # Copy SDL dlls to output folder
  set(SDL_RUNTIME_DLLS
    SDL3::SDL3
    SDL3_ttf::SDL3_ttf
    SDL3_mixer::SDL3_mixer
  )

  foreach(dllTarget IN LISTS SDL_RUNTIME_DLLS)
    add_custom_command(
      TARGET ${GAME_TARGET} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different
          "$<TARGET_FILE:${dllTarget}>"
          "$<TARGET_FILE_DIR:${GAME_TARGET}>"
      VERBATIM
    )
  endforeach()

  if(Steamworks_FOUND)
    target_link_libraries("${GAME_TARGET}" PUBLIC Steamworks::Steamworks)
    target_compile_definitions(${GAME_TARGET} PRIVATE USE_STEAMWORKS)
    add_custom_command(TARGET ${GAME_TARGET} POST_BUILD
      COMMAND "${CMAKE_COMMAND}" -E copy "${Steamworks_RUNTIME_DLL}" "$<TARGET_FILE_DIR:${GAME_TARGET}>")
    add_custom_command(TARGET ${GAME_TARGET} POST_BUILD
      COMMAND "${CMAKE_COMMAND}" -E echo "480" > "$<TARGET_FILE_DIR:${GAME_TARGET}>/steam_appid.txt"
      COMMENT "Generating steam_appid.txt"
    )
  endif()

  add_custom_command(TARGET ${GAME_TARGET} POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/Data" "$<TARGET_FILE_DIR:${GAME_TARGET}>/Data"
  )



elseif(APPLE)
  set_target_properties(${GAME_TARGET} PROPERTIES
    MACOSX_BUNDLE TRUE
    INSTALL_RPATH "@executable_path/../Frameworks"
    BUILD_WITH_INSTALL_RPATH TRUE
  )
  add_custom_command(TARGET ${GAME_TARGET} POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E make_directory "$<TARGET_BUNDLE_DIR:${GAME_TARGET}>/Contents/Frameworks"
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different "$<TARGET_FILE:SDL3::SDL3-shared>" "$<TARGET_BUNDLE_DIR:${GAME_TARGET}>/Contents/Frameworks/"
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different "$<TARGET_FILE:SDL3_ttf::SDL3_ttf-shared>" "$<TARGET_BUNDLE_DIR:${GAME_TARGET}>/Contents/Frameworks/"
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different "$<TARGET_FILE:SDL3_mixer::SDL3_mixer-shared>" "$<TARGET_BUNDLE_DIR:${GAME_TARGET}>/Contents/Frameworks/"
    COMMAND install_name_tool -change "@rpath/libSDL3_ttf.0.dylib" "@executable_path/../Frameworks/libSDL3_ttf.0.2.2.dylib" "$<TARGET_FILE:${GAME_TARGET}>"
    COMMAND install_name_tool -change "@rpath/libSDL3_mixer.0.dylib" "@executable_path/../Frameworks/libSDL3_mixer.0.dylib" "$<TARGET_FILE:${GAME_TARGET}>"
    COMMAND install_name_tool -change "@rpath/libSDL3.0.dylib" "@executable_path/../Frameworks/libSDL3.0.dylib" "$<TARGET_FILE:${GAME_TARGET}>"
  )

  if(Steamworks_FOUND)
    target_link_libraries("${GAME_TARGET}" PUBLIC Steamworks::Steamworks)
    target_compile_definitions(${GAME_TARGET} PRIVATE USE_STEAMWORKS)
    add_custom_command(TARGET ${GAME_TARGET} POST_BUILD
      COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${Steamworks_LIBRARIES}" "$<TARGET_BUNDLE_DIR:${GAME_TARGET}>/Contents/Frameworks/"
    )
    add_custom_command(TARGET ${GAME_TARGET} POST_BUILD
      COMMAND "${CMAKE_COMMAND}" -E echo "480" > "$<TARGET_FILE_DIR:${GAME_TARGET}>/steam_appid.txt"
      COMMENT "Generating steam_appid.txt"
    )
  endif()

  # Copy Data folder to output directory after build
  add_custom_command(TARGET ${GAME_TARGET} POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/Data" "$<TARGET_FILE_DIR:${GAME_TARGET}>/Data"
  )
else()
 if(Steamworks_FOUND)
    target_link_libraries("${GAME_TARGET}" PUBLIC Steamworks::Steamworks)
    target_compile_definitions(${GAME_TARGET} PRIVATE USE_STEAMWORKS)
    add_custom_command(TARGET ${GAME_TARGET} POST_BUILD
      COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${Steamworks_LIBRARIES}" "$<TARGET_FILE_DIR:${GAME_TARGET}>"
    )
    add_custom_command(TARGET ${GAME_TARGET} POST_BUILD
      COMMAND "${CMAKE_COMMAND}" -E echo "480" > "$<TARGET_FILE_DIR:${GAME_TARGET}>/steam_appid.txt"
      COMMENT "Generating steam_appid.txt"
    )
  endif()

  # Copy Data folder to output directory after build
  add_custom_command(TARGET ${GAME_TARGET} POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/Data" "$<TARGET_FILE_DIR:${GAME_TARGET}>/Data"
  )
endif()
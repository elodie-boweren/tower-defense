cmake_minimum_required(VERSION 3.16)
project(TowerDefense)

# Définir le standard C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Créer l'exécutable
add_executable(tower_defense tower_defense.cpp)

# Variables pour SFML
set(SFML_FOUND FALSE)

# Méthode 1: Essayer de trouver SFML avec CONFIG (installation moderne)
find_package(sfml COMPONENTS system window graphics audio CONFIG QUIET)
if(sfml_FOUND)
    message(STATUS "SFML trouvé via CONFIG")
    target_link_libraries(tower_defense 
        sfml::sfml-system 
        sfml::sfml-window 
        sfml::sfml-graphics 
        sfml::sfml-audio
    )
    set(SFML_FOUND TRUE)
endif()

# Méthode 2: Essayer avec SFML (nom de package alternatif)
if(NOT SFML_FOUND)
    find_package(SFML 2.5 COMPONENTS system window graphics audio QUIET)
    if(SFML_FOUND)
        message(STATUS "SFML trouvé via FindSFML")
        target_link_libraries(tower_defense 
            sfml-system 
            sfml-window 
            sfml-graphics 
            sfml-audio
        )
        set(SFML_FOUND TRUE)
    endif()
endif()

# Méthode 3: Utiliser PkgConfig
if(NOT SFML_FOUND)
    find_package(PkgConfig QUIET)
    if(PKG_CONFIG_FOUND)
        pkg_check_modules(SFML QUIET sfml-all>=2.5)
        if(SFML_FOUND)
            message(STATUS "SFML trouvé via PkgConfig")
            target_link_libraries(tower_defense ${SFML_LIBRARIES})
            target_include_directories(tower_defense PRIVATE ${SFML_INCLUDE_DIRS})
            target_compile_options(tower_defense PRIVATE ${SFML_CFLAGS_OTHER})
            set(SFML_FOUND TRUE)
        endif()
    endif()
endif()

# Méthode 4: Recherche manuelle des bibliothèques
if(NOT SFML_FOUND)
    message(STATUS "Recherche manuelle de SFML...")
    
    # Chemins communs pour SFML
    set(SFML_SEARCH_PATHS
        /usr/local
        /usr
        /opt/local
        /opt
        ${CMAKE_PREFIX_PATH}
        $ENV{SFML_ROOT}
        $ENV{SFML_DIR}
    )
    
    # Rechercher les headers
    find_path(SFML_INCLUDE_DIR 
        NAMES SFML/Graphics.hpp
        PATHS ${SFML_SEARCH_PATHS}
        PATH_SUFFIXES include
    )
    
    # Rechercher les bibliothèques
    find_library(SFML_SYSTEM_LIBRARY
        NAMES sfml-system sfml-system-s
        PATHS ${SFML_SEARCH_PATHS}
        PATH_SUFFIXES lib lib64
    )
    
    find_library(SFML_WINDOW_LIBRARY
        NAMES sfml-window sfml-window-s
        PATHS ${SFML_SEARCH_PATHS}
        PATH_SUFFIXES lib lib64
    )
    
    find_library(SFML_GRAPHICS_LIBRARY
        NAMES sfml-graphics sfml-graphics-s
        PATHS ${SFML_SEARCH_PATHS}
        PATH_SUFFIXES lib lib64
    )
    
    find_library(SFML_AUDIO_LIBRARY
        NAMES sfml-audio sfml-audio-s
        PATHS ${SFML_SEARCH_PATHS}
        PATH_SUFFIXES lib lib64
    )
    
    if(SFML_INCLUDE_DIR AND SFML_SYSTEM_LIBRARY AND SFML_WINDOW_LIBRARY AND SFML_GRAPHICS_LIBRARY)
        message(STATUS "SFML trouvé manuellement")
        target_include_directories(tower_defense PRIVATE ${SFML_INCLUDE_DIR})
        target_link_libraries(tower_defense 
            ${SFML_GRAPHICS_LIBRARY}
            ${SFML_WINDOW_LIBRARY}
            ${SFML_SYSTEM_LIBRARY}
        )
        if(SFML_AUDIO_LIBRARY)
            target_link_libraries(tower_defense ${SFML_AUDIO_LIBRARY})
        endif()
        set(SFML_FOUND TRUE)
    endif()
endif()

# Si SFML n'est toujours pas trouvé, afficher des instructions
if(NOT SFML_FOUND)
    message(FATAL_ERROR 
        "SFML non trouvé!\n"
        "Veuillez installer SFML:\n"
        "  Ubuntu/Debian: sudo apt install libsfml-dev\n"
        "  macOS: brew install sfml\n"
        "  Arch Linux: sudo pacman -S sfml\n"
        "  Windows: Téléchargez depuis https://www.sfml-dev.org/\n"
        "Ou définissez CMAKE_PREFIX_PATH vers votre installation SFML"
    )
endif()

# Options de compilation pour différentes plateformes
if(WIN32)
    # Windows
    if(MSVC)
        target_compile_options(tower_defense PRIVATE /W3)
    else()
        target_compile_options(tower_defense PRIVATE -Wall -Wextra)
    endif()
    
    # Copier les DLL SFML sur Windows (si nécessaire)
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(SFML_DLL_SUFFIX "-d-2")
    else()
        set(SFML_DLL_SUFFIX "-2")
    endif()
    
elseif(APPLE)
    # macOS
    target_compile_options(tower_defense PRIVATE -Wall -Wextra -Wno-unused-parameter)
    
    # Frameworks nécessaires sur macOS
    find_library(OPENGL_LIBRARY OpenGL)
    find_library(COCOA_LIBRARY Cocoa)
    find_library(IOKIT_LIBRARY IOKit)
    find_library(COREVIDEO_LIBRARY CoreVideo)
    
    target_link_libraries(tower_defense 
        ${OPENGL_LIBRARY}
        ${COCOA_LIBRARY}
        ${IOKIT_LIBRARY}
        ${COREVIDEO_LIBRARY}
    )
    
else()
    # Linux et autres Unix
    target_compile_options(tower_defense PRIVATE -Wall -Wextra -Wno-unused-parameter)
    
    # Bibliothèques système nécessaires sur Linux
    find_package(Threads REQUIRED)
    target_link_libraries(tower_defense Threads::Threads)
    
    # OpenGL et X11 pour Linux
    find_package(OpenGL REQUIRED)
    find_package(X11 REQUIRED)
    
    target_link_libraries(tower_defense 
        ${OPENGL_LIBRARIES}
        ${X11_LIBRARIES}
    )
endif()

# Configuration pour le mode Debug
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(tower_defense PRIVATE DEBUG)
    if(NOT MSVC)
        target_compile_options(tower_defense PRIVATE -g -O0)
    endif()
endif()

# Configuration pour le mode Release
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_definitions(tower_defense PRIVATE NDEBUG)
    if(NOT MSVC)
        target_compile_options(tower_defense PRIVATE -O3)
    endif()
endif()

# Définir le répertoire de sortie
set_target_properties(tower_defense PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Message d'information
message(STATUS "Configuration du projet Tower Defense:")
message(STATUS "  - Compilateur: ${CMAKE_CXX_COMPILER}")
message(STATUS "  - Standard C++: ${CMAKE_CXX_STANDARD}")
message(STATUS "  - Type de build: ${CMAKE_BUILD_TYPE}")
message(STATUS "  - Plateforme: ${CMAKE_SYSTEM_NAME}")

# Instructions d'installation (optionnel)
install(TARGETS tower_defense
    RUNTIME DESTINATION bin
)

# Cible pour nettoyer les fichiers générés
add_custom_target(clean-all
    COMMAND ${CMAKE_BUILD_TOOL} clean
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/bin
    COMMENT "Nettoyage complet du projet"
)
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

# ======================================
# GESTION DE NLOHMANN/JSON (FLEXIBLE)
# ======================================

# Variable pour tracker si nlohmann/json est trouvé
set(JSON_FOUND FALSE)

# Méthode 1: Essayer find_package (installation moderne)
find_package(nlohmann_json QUIET)
if(nlohmann_json_FOUND)
    message(STATUS "nlohmann/json trouvé via find_package")
    target_link_libraries(tower_defense nlohmann_json::nlohmann_json)
    set(JSON_FOUND TRUE)
endif()

# Méthode 2: Essayer PkgConfig
if(NOT JSON_FOUND)
    find_package(PkgConfig QUIET)
    if(PKG_CONFIG_FOUND)
        pkg_check_modules(NLOHMANN_JSON QUIET nlohmann_json)
        if(NLOHMANN_JSON_FOUND)
            message(STATUS "nlohmann/json trouvé via PkgConfig")
            target_include_directories(tower_defense PRIVATE ${NLOHMANN_JSON_INCLUDE_DIRS})
            set(JSON_FOUND TRUE)
        endif()
    endif()
endif()

# Méthode 3: Rechercher les headers dans les chemins système
if(NOT JSON_FOUND)
    find_path(NLOHMANN_JSON_INCLUDE_DIR
        NAMES nlohmann/json.hpp
        PATHS
            /usr/include
            /usr/local/include
            /opt/local/include
            ${CMAKE_PREFIX_PATH}/include
        NO_DEFAULT_PATH
    )
    
    if(NLOHMANN_JSON_INCLUDE_DIR)
        message(STATUS "nlohmann/json trouvé dans les headers système: ${NLOHMANN_JSON_INCLUDE_DIR}")
        target_include_directories(tower_defense PRIVATE ${NLOHMANN_JSON_INCLUDE_DIR})
        set(JSON_FOUND TRUE)
    endif()
endif()

# Méthode 4: Recherche dans le projet local
if(NOT JSON_FOUND)
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/include/nlohmann/json.hpp")
        message(STATUS "nlohmann/json trouvé localement dans include/")
        target_include_directories(tower_defense PRIVATE include)
        set(JSON_FOUND TRUE)
    endif()
endif()

# Si toujours pas trouvé, donner des instructions
if(NOT JSON_FOUND)
    message(WARNING 
        "nlohmann/json non trouvé!\n"
        "Solutions possibles:\n"
        "1. Installer via apt: sudo apt install nlohmann-json3-dev\n"
        "2. Télécharger json.hpp dans include/nlohmann/\n"
        "3. Le code peut compiler sans mais la sauvegarde ne fonctionnera pas"
    )
    # Ajouter une définition pour désactiver le JSON si nécessaire
    target_compile_definitions(tower_defense PRIVATE NO_JSON_SUPPORT)
endif()

# Options de compilation pour différentes plateformes
if(WIN32)
    # Windows
    if(MSVC)
        target_compile_options(tower_defense PRIVATE /W3)
    else()
        target_compile_options(tower_defense PRIVATE -Wall -Wextra)
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

# Créer le répertoire bin s'il n'existe pas
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

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
message(STATUS "  - SFML trouvé: ${SFML_FOUND}")
message(STATUS "  - nlohmann/json trouvé: ${JSON_FOUND}")

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

# Ajouter ces lignes à la fin de votre CMakeLists.txt

# Copier les assets vers le répertoire de build
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../assets")
    message(STATUS "Copie des assets depuis le répertoire parent")
    file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/../assets" 
         DESTINATION "${CMAKE_BINARY_DIR}")
    file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/../assets" 
         DESTINATION "${CMAKE_BINARY_DIR}/bin")
elseif(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/assets")
    message(STATUS "Copie des assets depuis le répertoire du projet")
    file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/assets" 
         DESTINATION "${CMAKE_BINARY_DIR}")
    file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/assets" 
         DESTINATION "${CMAKE_BINARY_DIR}/bin")
else()
    message(WARNING "Dossier assets non trouvé")
endif()

# Alternative: créer une cible personnalisée pour copier les assets
add_custom_target(copy_assets ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/../assets"
        "${CMAKE_BINARY_DIR}/bin/assets"
    COMMENT "Copie des assets vers le répertoire de build"
)
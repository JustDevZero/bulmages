# Copyright (C) 2008 by Leopold Palomo-Avellaneda                       #
# leo@alaxarxa.net                                                      #
# Modified (C) 2013 by Daniel Ripoll                                    #
# http://www.iglues.org                                                 #
#                                                                       #
# This program is free software; you can redistribute it and/or modify  #
# it under the terms of the GNU General Public License as published by  #
# the Free Software Foundation; either version 2 of the License, or     #
# (at your option) any later version.                                   #
#                                                                       #
# This program is distributed in the hope that it will be useful,       #
# but WITHOUT ANY WARRANTY; without even the implied warranty of        #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         #
# GNU General Public License for more details.                          #
#                                                                       #
# You should have received a copy of the GNU General Public License     #
# along with this program; if not, write to the                         #
# Free Software Foundation, Inc.,                                       #
# 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             #
#                                                                       #
#########################################################################
# ########## Project setup ##########



PROJECT(bulmages)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.10)

SET( BULMAGES_VERSION 1.5.0 )

IF(NOT WIN32)
  set(CMAKE_CXX_FLAGS "-fPIC")
ENDIF(NOT WIN32)

find_package(Qt5Core REQUIRED)

IF(WIN32)
set(QT5_MODULE_PATH "${_qt5Core_install_prefix}/lib/cmake")
ELSE(WIN32)
set(QT5_MODULE_PATH "${_qt5Core_install_prefix}/lib/x86_64-linux-gnu/cmake")
ENDIF(WIN32)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/ModulesCMake")

STRING( REGEX MATCHALL "[0-9]+" BULMAGES_VERSIONS ${BULMAGES_VERSION} )
LIST( GET BULMAGES_VERSIONS 0 BULMAGES_VERSION_MAJOR)
LIST( GET BULMAGES_VERSIONS 1 BULMAGES_VERSION_MINOR)
LIST( GET BULMAGES_VERSIONS 2 BULMAGES_VERSION_PATCH)

# Differences between CMake 2.4 and 2.6
if("${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}" GREATER 2.8.0)
   message(STATUS "Using cmake version ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}" )
   # some developers may be using an cmake cvs version which didn't have set_property() yet
   # Tell them that a more recent version is required.
   if(NOT COMMAND SET_PROPERTY)
      message(FATAL_ERROR "You are using an old version of CMake from cvs, please update to CMake >= 2.6.0 or cvs at least from Feb 20th, 2008")
   endif(NOT COMMAND SET_PROPERTY)


   # CMake 2.6 gives errors if there are multiple targets with the same name
   # we use this for the target "buildtests", which is created for the unit tests
   # and which depends on the tests, so building "buildtests" builds all the tests
   # enabling this property disables this check in CMake
   set_property(GLOBAL PROPERTY ALLOW_DUPLICATE_CUSTOM_TARGETS 1)
    
   # CMP0002: in KDE4 we have multiple targets with the same name for the unit tests
   cmake_policy(SET CMP0002 OLD)
   # CMP0003: add the link paths to the link command as with cmake 2.4
   cmake_policy(SET CMP0003 OLD)
   # CMake 2.6 uses the full file names of the libraries when linking and so 
   # doesn't use -L anymore to list the link dirs. Now the dependencies created
   # export_library_dependencies() lists the in-project libraries without 
   # path, i.e. with only the logical name ("kdecore"), so they don't link
   # Setting this variable to true has the effect that the link dirs are
   # listed nevertheless also with CMake 2.6.
   set(CMAKE_LINK_OLD_PATHS TRUE)

   # list command no longer ignores empty elements.
   cmake_policy(SET CMP0007 OLD)

endif("${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}" GREATER 2.8.0)
message(STATUS ${QT5_MODULE_PATH})
INCLUDE(${CMAKE_CURRENT_SOURCE_DIR}/ModulesCMake/FindQt.cmake)
INCLUDE(${CMAKE_CURRENT_SOURCE_DIR}/ModulesCMake/FindQt5.cmake)
INCLUDE(${QT5_MODULE_PATH}/Qt5Core/Qt5CoreMacros.cmake)
#INCLUDE(${CMAKE_CURRENT_SOURCE_DIR}/ModulesCMake/QtCTestMacros.cmake)
INCLUDE(${QT5_MODULE_PATH}/Qt5Widgets/Qt5WidgetsMacros.cmake)
INCLUDE(${QT5_MODULE_PATH}/Qt5Widgets/Qt5WidgetsConfig.cmake)



# ######### General setup ##########
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR})

if(NOT QT5_FOUND)
  message(FATAL_ERROR "Bulmages needs QT5 to compile")
endif(NOT QT5_FOUND)

IF (QT5_FOUND)
 find_package(Qt5Core REQUIRED)
ENDIF(QT5_FOUND)


###############################################
# and now the version string given by qmake
STRING(REGEX REPLACE "^([0-9]+)\\.[0-9]+\\.[0-9]+.*" "\\1" QT_MAJOR_VERSION "${QTVERSION}")
STRING(REGEX REPLACE "^[0-9]+\\.([0-9])+\\.[0-9]+.*" "\\1" QT_MINOR_VERSION "${QTVERSION}")
STRING(REGEX REPLACE "^[0-9]+\\.[0-9]+\\.([0-9]+).*" "\\1" QT_PATCH_VERSION "${QTVERSION}")

if (${QT_MAJOR_VERSION}.${QT_MINOR_VERSION} LESS 5.0)
    message(FATAL_ERROR " Bulmages needs >= QT5.0 to compile")
endif (${QT_MAJOR_VERSION}.${QT_MINOR_VERSION} LESS 5.0)

INCLUDE (${CMAKE_CURRENT_SOURCE_DIR}/ModulesCMake/FindPostgreSQL.cmake)
INCLUDE (${CMAKE_CURRENT_SOURCE_DIR}/ModulesCMake/FindGettext.cmake)

if (${CMAKE_VERSION} VERSION_GREATER 2.8.2)
   INCLUDE (${CMAKE_CURRENT_SOURCE_DIR}/ModulesCMake/FindPythonInterp.cmake)
else()
   IF(WIN32 AND NOT UNIX)
      if(EXISTS "C:/Python27/python.exe")
         message(STATUS "Found Python: C:/Python27/python.exe")
         set(PYTHON_EXECUTABLE "C:/Python27/python.exe" CACHE STRING "Where python is installed")
      endif()
   ENDIF(WIN32 AND NOT UNIX)
   IF(UNIX AND NOT WIN32)
      if(EXISTS "/usr/bin/python")
        message(STATUS "Found Python: /usr/bin/python")
        set(PYTHON_EXECUTABLE "/usr/bin/python" CACHE STRING "Where python is installed")
      endif()
      if(EXISTS "/usr/local/bin/python")
       message(STATUS "Found Python: /usr/local/bin/python")
       set(PYTHON_EXECUTABLE "/usr/local/bin/python" CACHE STRING "Where python is installed")
      endif()
   ENDIF(UNIX AND NOT WIN32)
endif()


if(NOT PYTHON_EXECUTABLE)
  message(STATUS "Python was not found: Without this you will not be able to run bulmasetup or create pdfs")
endif(NOT PYTHON_EXECUTABLE)

option(USE_GETTEXT "Enable use gettext mode" ON)
option(ALLOW_DEBUG "Enable Debugging Code" ON)

if(NOT PGSQL_FOUND)
  message(FATAL_ERROR "Bulmages needs libpq to compile")
endif(NOT PGSQL_FOUND)

if(WIN32)
set( BULMAGES_CONFIG_DIR $ENV{ProgramFiles}/bulmages/etc CACHE PATH "The location of bulmages configuration dir")
endif(WIN32)

if(NOT WIN32)
set( BULMAGES_CONFIG_DIR /etc/bulmages CACHE PATH "The location of bulmages configuration dir")
endif(NOT WIN32)


if(WIN32)
set( BULMAGES_OPTWARE_DIR $ENV{ProgramFiles}/bulmages/opt CACHE PATH "The location of bulmages optware dir")
endif(WIN32)

if(NOT WIN32)
set( BULMAGES_OPTWARE_DIR /opt/bulmages CACHE PATH "The location of bulmages optware dir")
endif(NOT WIN32)

if(WIN32)
set(HOME_DIR $ENV{USERPROFILE})
endif(WIN32)

if(NOT WIN32)
set(HOME_DIR $ENV{HOME})
endif(NOT WIN32)


# Remove directory to prevent that css files can grow with plugins css merge.
FILE(REMOVE_RECURSE ${CMAKE_BINARY_DIR}/css)


##############################################################################
# Sets the location of nonstandard shared libraries
##############################################################################

# Use, i.e. don't skip the full RPATH for the build tree
set(CMAKE_SKIP_BUILD_RPATH  FALSE)

# When building, don't use the install RPATH already
# (but later on when installing)
set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)

# The RPATH to be used when installing
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

# Add the automatically determined parts of the RPATH
# which point to directories outside the build tree to the install RPATH
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)


########### bulmages library ##########
message(STATUS "Working on bulmalib")
add_subdirectory(bulmalib)

########### bulmacont ##########
#message(STATUS "Working on bulmacont")
#add_subdirectory(bulmacont)

########### bulmafact ##########
message(STATUS "Working on bulmafact")
add_subdirectory(bulmafact)

########### bulmages launcher ##########
#message(STATUS "Working on bulmages")
#add_subdirectory(bulmages)

########### bulmatpv ##########
message(STATUS "Working on bulmatpv")
add_subdirectory(bulmatpv)

########### bulmasetup ##########
message(STATUS "Working on bulmasetup")
add_subdirectory(bulmasetup)


########### private code ########
if(EXISTS "${CMAKE_SOURCE_DIR}/CMakeLists.txt.private")
   message(STATUS "Private code inserted")
   include("${CMAKE_SOURCE_DIR}/CMakeLists.txt.private")
endif(EXISTS "${CMAKE_SOURCE_DIR}/CMakeLists.txt.private")


if(APPLE)
    message(STATUS "Compiling for OSX")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -undefined dynamic_lookup")
endif(APPLE)

########### some stuff ##########
message(STATUS "Working on installbulmages")
add_subdirectory(installbulmages)

message(STATUS "Working on documentation")
add_subdirectory(doc)


set (r_files README.es README.plugins.es)
install(FILES ${r_files} DESTINATION share/bulmages/)

install(DIRECTORY images DESTINATION share/bulmages/imagesrc PATTERN ".svn" EXCLUDE)

# We include some kind of xdg-sudo that will search for gtksudo,kdesudo, and if not found will prompt an xterm
IF(UNIX AND NOT WIN32)
    install(FILES scripts/xdg-sudo
            PERMISSIONS OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
            DESTINATION bin/)
ENDIF(UNIX AND NOT WIN32)

## We include cpack for others platforms differents than unix
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "The BulmaGes Suite")
SET(CPACK_PACKAGE_VENDOR "Iglues")
SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README.es")
SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/installbulmages/COPYING.txt")
SET(CPACK_PACKAGE_VERSION_MAJOR ${BULMAGES_VERSION_MAJOR})
SET(CPACK_PACKAGE_VERSION_MINOR ${BULMAGES_VERSION_MINOR})
SET(CPACK_PACKAGE_VERSION_PATCH ${BULMAGES_VERSION_PATCH})
SET(CPACK_PACKAGE_INSTALL_DIRECTORY "CMake ${CMAKE_VERSION_MAJOR}.${CMAKE_VERSION_MINOR}")
IF(WIN32 AND NOT UNIX)
  # There is a bug in NSI that does not handle full unix paths properly. Make
  # sure there is at least one set of four (4) backlasshes.
  SET(CPACK_PACKAGE_ICON "${CMAKE_SOURCE_DIR}/installbulmages/icons\\\\iconbulmacont.xpm")
  SET(CPACK_NSIS_INSTALLED_ICON_NAME "bin\\\\bulmacont")
  SET(CPACK_NSIS_DISPLAY_NAME "${CPACK_PACKAGE_INSTALL_DIRECTORY} The BulmaGes Suite")
  SET(CPACK_NSIS_HELP_LINK "http:\\\\\\\\developer.berlios.de/projects/bulmages")
  SET(CPACK_NSIS_URL_INFO_ABOUT "http:\\\\\\\\developer.berlios.de/projects/bulmages")
  SET(CPACK_NSIS_CONTACT "bulmages@bulma.net")
  SET(CPACK_NSIS_MODIFY_PATH ON)
ELSE(WIN32 AND NOT UNIX)
  SET(CPACK_STRIP_FILES "bin/bulmacont" "bin/bulmafact" "bin/bulmatpv")
  SET(CPACK_SOURCE_STRIP_FILES "")
ENDIF(WIN32 AND NOT UNIX)
SET(CPACK_PACKAGE_EXECUTABLES "bulmacont" "bulmafact" "bulmatpv")


INCLUDE(CPack)

MARK_AS_ADVANCED(
	BULMAGES_VERSION
)


###########################################
# Plugin for bulmafact                    #
###########################################
#pluginactividad

set (plugin pluginbf_recibo)

file(GLOB plugin_SRC *.c *.cpp *.cxx)
file(GLOB plugin_MOCS *.hpp *.h)
file(GLOB plugin_UIS *.ui)

configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/css/${plugin}_es_ES.css.in ${CMAKE_BINARY_DIR}/bulmafact/plugins/${plugin}/${plugin}_es_ES.css @ONLY)
configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/css/${plugin}_ca_ES.css.in ${CMAKE_BINARY_DIR}/bulmafact/plugins/${plugin}/${plugin}_ca_ES.css @ONLY)
configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/css/${plugin}_en_US.css.in ${CMAKE_BINARY_DIR}/bulmafact/plugins/${plugin}/${plugin}_en_US.css @ONLY)
configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/css/${plugin}_fr_FR.css.in ${CMAKE_BINARY_DIR}/bulmafact/plugins/${plugin}/${plugin}_fr_FR.css @ONLY)



# Exclude files with no QObject macro.
file(GLOB plugin_NO_MOCS pdefs_pluginbf_recibo.h)
list(REMOVE_ITEM plugin_MOCS ${plugin_NO_MOCS})

include_directories(
    ${QT_INCLUDE_DIR}
	${PGSQL_INCLUDE_DIR}
	${QT_QTXML_INCLUDE_DIR}
        ${QT_QTSCRIPT_INCLUDE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_BINARY_DIR}/bulmalib/src/
	${CMAKE_SOURCE_DIR}/bulmalib/src/
	${CMAKE_BINARY_DIR}/bulmafact/src/
	${CMAKE_SOURCE_DIR}/bulmafact/src/
	${CMAKE_BINARY_DIR}/bulmafact/plugins/pluginbf_actividad/
	${CMAKE_SOURCE_DIR}/bulmafact/plugins/pluginbf_actividad/
	${CMAKE_BINARY_DIR}/bulmafact/plugins/pluginbf_profesor/
	${CMAKE_SOURCE_DIR}/bulmafact/plugins/pluginbf_profesor/)

# generate rules for building source files from the resources
#QT5_WRAP_CPP(plugin_MOC_SRCS ${plugin_MOCS})

# generate rules for building source files that moc generates
QT5_WRAP_UI(plugin_UIS_H ${plugin_UIS} OPTIONS -tr N_)
QT5_WRAP_CPP(plugin_MOC_SRCS ${plugin_MOCS})
#QT5_ADD_RESOURCES(pluginactividad_RCC_SRCS ${pluginactividad_RCCS})
#QT5_ADD_RESOURCES(plugin_RCC_SRCS ${plugin_RCCS})


IF(WIN32)
SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-enable-auto-import")
ADD_DEFINITIONS(-DQWT_DLL -DQT_DLL -DQWT_MAKEDLL -DPLUGINBF_RECIBO)
ENDIF(WIN32)


#ADD_DEFINITIONS(${QT_DEFINITIONS})
ADD_DEFINITIONS(-DQT_SHARED)
# build sources, moc'd sources, and rcc'd sources

if(APPLE)
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -undefined dynamic_lookup")
endif(APPLE)

# i18n
if(WIN32)
ADD_DEFINITIONS("-D_(String)=blI18n(\\\"${plugin}\\\",String)" )
ADD_DEFINITIONS("-DN_(String,Int)=blI18n(\\\"${plugin}\\\",String)" )
endif(WIN32)

if(NOT WIN32)
ADD_DEFINITIONS("-D'_(String)=blI18n(\"${plugin}\",String)'" )
ADD_DEFINITIONS("-D'N_(String,Int)=blI18n(\"${plugin}\",String)'" )
endif(NOT WIN32)


if(USE_GETTEXT)
   set(plugin_i18n ${plugin_SRC} ${plugin_MOC_SRCS} ${plugin_UIS_H}
                        ${plugin_SRCS} ${plugin_MOCS})
   GETTEXT_CREATE_TEMPLATE(${plugin} ${CMAKE_CURRENT_BINARY_DIR}
                                ${CMAKE_CURRENT_SOURCE_DIR} plugin_i18n)
   GETTEXT_CREATE_TRANSLATIONS(${CMAKE_CURRENT_BINARY_DIR}/${plugin}.pot
                                ALL LANGUAGES es_ES ca_ES en_US fr_FR)
endif(USE_GETTEXT)

add_library(${plugin} SHARED ${plugin_SRC} ${plugin_MOC_SRCS}
	${plugin_UIS_H} ${plugin_SRCS} ${plugin_MOCS})


set_target_properties( ${plugin} PROPERTIES 
  SOVERSION "${BULMAGES_VERSION_MAJOR}.${BULMAGES_VERSION_MINOR}"
  VERSION "${BULMAGES_VERSION}")

target_link_libraries(${plugin} libbulmafact bulmalib ${QT_LIBRARIES} ${PGSQL_LIBRARIES})
install_targets(/lib/bulmafact/plugins ${plugin})

FILE(READ ${CMAKE_BINARY_DIR}/bulmafact/plugins/${plugin}/${plugin}_es_ES.css VAR_CSS_es_ES)
FILE(APPEND ${CMAKE_BINARY_DIR}/css/bulmafact_es_ES.css "${VAR_CSS_es_ES}" )

FILE(READ ${CMAKE_BINARY_DIR}/bulmafact/plugins/${plugin}/${plugin}_ca_ES.css VAR_CSS_ca_ES)
FILE(APPEND ${CMAKE_BINARY_DIR}/css/bulmafact_ca_ES.css "${VAR_CSS_ca_ES}" )

FILE(READ ${CMAKE_BINARY_DIR}/bulmafact/plugins/${plugin}/${plugin}_fr_FR.css VAR_CSS_fr_FR)
FILE(APPEND ${CMAKE_BINARY_DIR}/css/bulmafact_fr_FR.css "${VAR_CSS_fr_FR}" )

FILE(READ ${CMAKE_BINARY_DIR}/bulmafact/plugins/${plugin}/${plugin}_en_US.css VAR_CSS_en_US)
FILE(APPEND ${CMAKE_BINARY_DIR}/css/bulmafact_en_US.css "${VAR_CSS_en_US}" )


install(DIRECTORY images DESTINATION share/bulmages PATTERN ".svn" EXCLUDE)


configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/README.es ${CMAKE_CURRENT_BINARY_DIR}/README.es @ONLY)
install (FILES ${CMAKE_CURRENT_BINARY_DIR}/README.es DESTINATION share/bulmages/bulmafact-plugins/ RENAME README.es.${plugin})

configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/README.ca ${CMAKE_CURRENT_BINARY_DIR}/README.ca @ONLY)
install (FILES ${CMAKE_CURRENT_BINARY_DIR}/README.ca DESTINATION share/bulmages/bulmafact-plugins/ RENAME README.ca.${plugin})

configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/README.fr ${CMAKE_CURRENT_BINARY_DIR}/README.fr @ONLY)
install (FILES ${CMAKE_CURRENT_BINARY_DIR}/README.fr DESTINATION share/bulmages/bulmafact-plugins/ RENAME README.fr.${plugin})

configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/README.en ${CMAKE_CURRENT_BINARY_DIR}/README.en @ONLY)
install (FILES ${CMAKE_CURRENT_BINARY_DIR}/README.en DESTINATION share/bulmages/bulmafact-plugins/ RENAME README.en.${plugin})


add_dependencies(${plugin} bulmafact bulmalib pluginbf_actividad)

install(DIRECTORY openreports/ 
      DESTINATION share/bulmages/openreports
      PATTERN ".svn" EXCLUDE)



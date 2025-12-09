set(KRITA_TOGGLEDOCKER_SOURCES
    toggledocker.cpp
)

add_library(kritatoggledocker MODULE ${KRITA_TOGGLEDOCKER_SOURCES})

target_link_libraries(kritatoggledocker kritaui)

install(TARGETS kritatoggledocker DESTINATION ${KRITA_PLUGIN_INSTALL_DIR})
install( FILES  ToggleDocker.action  DESTINATION ${DATA_INSTALL_DIR}/krita/actions)

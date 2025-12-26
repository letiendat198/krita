set(KRITA_REFERENCEDOCKER_SOURCES
    ReferenceDocker.cpp
    ReferenceDockerDock.cpp
    ReferenceViewWidget.cpp
)

ki18n_wrap_ui(KRITA_REFERENCEDOCKER_SOURCES
    WdgReferenceDocker.ui
)

kis_add_library(kritareferencedocker MODULE ${KRITA_REFERENCEDOCKER_SOURCES})
target_link_libraries(kritareferencedocker kritaui)
install(TARGETS kritareferencedocker DESTINATION ${KRITA_PLUGIN_INSTALL_DIR})

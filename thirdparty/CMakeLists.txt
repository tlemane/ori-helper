include(ExternalProject)
ExternalProject_Add(SDSL
        PREFIX sdsl
        SOURCE_DIR ${PROJECT_SOURCE_DIR}/thirdparty/sdsl-lite
        INSTALL_COMMAND "")
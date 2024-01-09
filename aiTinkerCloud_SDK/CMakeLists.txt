sdk_generate_library()

# sdk_add_include_directories(.)

FIND_INCLUDE_DIR(INCLUDE_DIR .)
FIND_SOURCES_DIR(SOURCES_DIR .)

sdk_add_include_directories(${INCLUDE_DIR})
sdk_library_add_sources(${SOURCES_DIR})

sdk_add_compile_definitions(-DCONFIG_aiTinkerCloud_SDK)
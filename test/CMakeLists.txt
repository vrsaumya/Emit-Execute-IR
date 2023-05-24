configure_lit_site_cfg(
        ${CMAKE_CURRENT_SOURCE_DIR}/lit.site.cfg.py.in
        ${CMAKE_CURRENT_BINARY_DIR}/lit.site.cfg.py
        MAIN_CONFIG
        ${CMAKE_CURRENT_SOURCE_DIR}/lit.cfg.py
)

set(CUSTOM_TEST_DEPENDS
        FileCheck count not
        custom-capi-test
        custom-opt
        custom-translate
        CustomPlugin
        )

add_lit_testsuite(check-custom "Running the custom regression tests"
        ${CMAKE_CURRENT_BINARY_DIR}
        DEPENDS ${CUSTOM_TEST_DEPENDS}
        )
set_target_properties(check-custom PROPERTIES FOLDER "Tests")

add_lit_testsuites(CUSTOM ${CMAKE_CURRENT_SOURCE_DIR} DEPENDS ${CUSTOM_TEST_DEPENDS})

add_subdirectory(CAPI)

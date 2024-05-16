enable_testing()
set(TEST_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
file(GLOB TEST_SOURCES ${TEST_SOURCE_DIR}/*.cc)

foreach(TEST_SOURCE ${TEST_SOURCES})
    message(STATUS "Adding test: ${TEST_SOURCE}")
    get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)

    add_executable(${TEST_NAME} ${TEST_SOURCE})

    target_compile_definitions(${TEST_NAME} PRIVATE CATCH_CONFIG_MAIN)
    target_include_directories(${TEST_NAME} PRIVATE ../3rd)
    target_link_libraries(${TEST_NAME} PRIVATE cpp_sugar)

    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endforeach()

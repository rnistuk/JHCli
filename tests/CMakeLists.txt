include(GoogleTest)

function(add_gmock_test target)
    add_executable(${target} ${ARGN} ../src/data_parser.cpp)
    add_dependencies(${target} googletest)
    target_link_libraries(${target}
            ${GMOCK_BOTH_LIBRARIES}
            #${XDG_BASEDIR_LIBRARIES}
            #${Boost_LIBRARIES}
            ${CURL_LIBRARIES}
            #${JSONCPP_LIBRARIES} pthread
            )
    target_include_directories(${target} PRIVATE ../src/data_parser.h ${JSONCPP_INCLUDE_DIRS})
    gtest_discover_tests(${target})
endfunction()

add_gmock_test(covid19_tests data_parser_test.cpp)
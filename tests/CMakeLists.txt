find_package(Threads REQUIRED)

set(SOURCES socket.cpp server.cpp)

set(BINARY ${CMAKE_PROJECT_NAME}_Test)

add_executable(${BINARY} ${SOURCES})

enable_testing()
add_test(NAME ${BINARY} COMMAND ${BINARY})

if(GTest_Found)
    target_link_libraries(${BINARY} PUBLIC ${CMAKE_PROJECT_NAME} Threads::Threads GTest::Main)
else()
    target_link_libraries(${BINARY} PUBLIC ${CMAKE_PROJECT_NAME} Threads::Threads gtest_main)
endif()
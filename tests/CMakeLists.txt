set(SOURCES tests.cpp)

set(BINARY ${CMAKE_PROJECT_NAME}_Test)

add_executable(${BINARY} ${SOURCES})

enable_testing()
add_test(NAME ${BINARY} COMMAND ${BINARY})

target_link_libraries(${BINARY} PUBLIC ${CMAKE_PROJECT_NAME} GTest::Main)
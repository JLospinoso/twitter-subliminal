cmake_minimum_required(VERSION 3.2)

project(twitter_subliminal)

add_subdirectory(googletest/googlemock)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

find_package(Threads REQUIRED)
find_package(Poco REQUIRED)
find_library(gmock REQUIRED)
find_library(gtest REQUIRED)
find_library(gtest_main REQUIRED)
include_directories(Poco_INCLUDE_DIR)

set(LIBS ${LIBS} ${Poco_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
message(STATUS "Libraries: ${LIBS}")

set(TEST_FILES tests/StringBitIteratorTest.cpp tests/ShortBitIteratorTest.cpp tests/TweetTest.cpp
        tests/TweetBlockCalculatorTest.cpp tests/IntegrationTest.cpp tests/TwitterFactoryTest.cpp
        tests/Sha1CalculatorTest.cpp tests/TweetBitIteratorTest.cpp)

set(CMAKE_CXX_STANDARD 14)

add_executable(tse encode_main.cpp ${SOURCE_FILES})
add_executable(tsd decode_main.cpp ${SOURCE_FILES})
add_executable(tsr reset_main.cpp ${SOURCE_FILES})
add_executable(tsl limit_main.cpp ${SOURCE_FILES})
add_executable(tsp performance_main.cpp ${SOURCE_FILES})
add_executable(tst ${SOURCE_FILES} ${TEST_FILES})

target_link_libraries(tse ${LIBS})
target_link_libraries(tsd ${LIBS})
target_link_libraries(tsr ${LIBS})
target_link_libraries(tsp ${LIBS})
target_link_libraries(tsl ${LIBS})
target_link_libraries(tst ${LIBS} gmock gtest gtest_main)
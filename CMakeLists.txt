cmake_minimum_required(VERSION 3.15)
project(Task_11)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_compile_options(-fcoroutines)
endif()

find_package(Boost REQUIRED COMPONENTS system)
find_package(Threads REQUIRED)

# Task_7_1 – Echo server
add_executable(Task_7_1
        Task_7_1/main.cpp
        Task_7_1/echo_server.cpp
)
target_include_directories(Task_7_1 PRIVATE Task_7_1)
target_link_libraries(Task_7_1 PRIVATE Boost::system Threads::Threads ws2_32 mswsock)
target_compile_definitions(Task_7_1 PRIVATE BOOST_ASIO_HAS_CO_AWAIT)

# Task_7_2 – Multiplexer
add_executable(Task_7_2
        Task_7_2/main.cpp
        Task_7_2/multiplexer.cpp
)
target_include_directories(Task_7_2 PRIVATE Task_7_2)
target_link_libraries(Task_7_2 PRIVATE Boost::system Threads::Threads ws2_32 mswsock)
target_compile_definitions(Task_7_2 PRIVATE BOOST_ASIO_HAS_CO_AWAIT)

# Task_7_3 – Bank account
add_executable(Task_7_3
        Task_7_3/main.cpp
        Task_7_3/bank_account.cpp
)
target_include_directories(Task_7_3 PRIVATE Task_7_3)
target_link_libraries(Task_7_3 PRIVATE Boost::system Threads::Threads ws2_32 mswsock)
target_compile_definitions(Task_7_3 PRIVATE BOOST_ASIO_HAS_CO_AWAIT)
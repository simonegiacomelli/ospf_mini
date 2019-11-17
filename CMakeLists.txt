cmake_minimum_required(VERSION 3.15)
project(ospf_mini_lib)

set(CMAKE_CXX_STANDARD 11)

add_executable(ospf_mini_lib ospf_mini_dep main/main.cpp)

add_executable(sketch_ospf_mini_app ospf_mini_dep sketch/ospf_mini_app/ospf_mini_app.ino)
add_executable(sketch_BLE_client3 ospf_mini_dep sketch/BLE_client3/BLE_client3.ino)
add_executable(sketch_BLE_clientdetect_multiple_server ospf_mini_dep sketch/BLE_client_detect_multiple_server/BLE_client_detect_multiple_server.ino)

FILE(GLOB ArduinoBLE  ArduinoBLE/src/*.cpp ArduinoBLE/src/*.h  ArduinoBLE/src/local/*.cpp ArduinoBLE/src/local/*.h  ArduinoBLE/src/utility/*.cpp ArduinoBLE/src/utility/*.h)
add_executable(fake_BLE ${ArduinoBLE})
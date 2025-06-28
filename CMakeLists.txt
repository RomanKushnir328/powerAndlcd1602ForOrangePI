cmake_minimum_required(VERSION 3.22.1)
project(powerAndDisplayManagement VERSION 1.0)

add_executable(powerManagement powerManagement/script.cpp)
target_link_libraries(powerManagement wiringPi)
install(TARGETS powerManagement DESTINATION /usr/local/bin)

add_executable(displayManagement displayManagement/script.cpp)
target_link_libraries(displayManagement wiringPi)
target_link_libraries(displayManagement wiringPiDev)
install(TARGETS displayManagement DESTINATION /usr/local/bin)

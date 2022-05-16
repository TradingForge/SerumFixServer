
find_package(Threads REQUIRED)
find_package(Boost 1.79.0 REQUIRED)
find_package(Poco REQUIRED Util XML JSON Net Foundation)
find_package(Fix8 REQUIRED)
find_package(P7 REQUIRED)

add_executable(fixserver main.cpp)

target_include_directories(fixserver PRIVATE
        ".."
        ${FIX8_INCLUDE_DIRS}
        ${P7_INCLUDE_DIRS}
        )

target_link_libraries(fixserver PRIVATE
    ${SERUM_NAME}
#    ${MARKETLIB_NAME}
#    ${SHAREDLIB_NAME}
    ${CMAKE_THREAD_LIBS_INIT}
    PocoUtil
    PocoXML
    PocoJSON
    PocoNet
    PocoFoundation
    ${FIX8_LIBRARIES}
    ${P7_LIBRARY_STATIC}
    )





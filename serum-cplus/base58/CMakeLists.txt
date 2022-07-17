set(CMAKE_POSITION_INDEPENDENT_CODE ON)
# find_package(GMP REQUIRED)

add_library("base58" STATIC
        base58.cpp
)

# find_package(Fix8 REQUIRED)
# find_package(P7 REQUIRED)
# find_package(GMP REQUIRED)

# target_link_libraries("base58" PRIVATE
#         "${GMP_DIR}/lib/libgmp.dll.a"
#         "${GMP_DIR}/lib/libgmp.a"
#         )
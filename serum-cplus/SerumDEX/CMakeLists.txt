set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_library("${SERUM_DEX_NAME}" STATIC
        SerumMD.cpp
        SerumTrade.cpp
        PoolRequester/InstrumentJSON.cpp
        PoolRequester/InstrumentsJSON.cpp
        PoolRequester/PoolsRequester.cpp
        SerumMarket/Market.cpp
        # SerumMarket/Instruments.cpp
        SerumMarket/sol_sdk/Transaction.cpp
        SerumMarket/sol_sdk/Hash.cpp
        SerumMarket/sol_sdk/Instruction.cpp
        SerumMarket/sol_sdk/Keypair.cpp
        SerumMarket/sol_sdk/PublicKey.cpp
)

find_package(Fix8 REQUIRED)
find_package(P7 REQUIRED)
# find_package(GMP REQUIRED)

target_include_directories(${SERUM_DEX_NAME} PRIVATE
        "include"
        "rapidjson"
        ".."
        ${FIX8_INCLUDE_DIRS}
        ${P7_INCLUDE_DIRS}
        )
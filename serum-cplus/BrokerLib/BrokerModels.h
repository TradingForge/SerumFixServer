#pragma once

#include <string>
#include <chrono>
#include <map>
#include <vector>

namespace BrokerModels {
    enum class OrderType {
        Limit,
        Market
    };

    enum class OrderSide {
        Buy,
        Sell,
        None
    };
    
    enum class OrderState {
        Open,
        Filled,
        Replaced,
        Canceled
    };

    struct Order {
        OrderType orderType;
        OrderState orderState;
        OrderSide orderSide;

        double amount;
        double price;

        std::string clientId;
        std::string exchangeId;
        std::string initDate;

        std::string exchange;
        std::string symbol;
        std::string first;
        std::string second;

        std::string text;
    };

    struct ExecutionReport {
        std::string clientId;
        std::string exchId;
        std::string symbol;
        OrderSide side;
        OrderState ordStatus;
        OrderType ordType;
        double price;
        double amountExecuted;
        double amountRemaining;
        std::string text;
    }; 

    struct Instrument {
        std::string exchange;
        std::string symbol;
        std::string first;
        std::string second;
        std::string description;
        int decimals;
    };

    struct MarketBook {
        std::chrono::time_point < std::chrono::system_clock > time;
        double bidPrice;
        double bidSize;
        double askPrice;
        double askSize;
        double lastPrice;
        double lastSize;
    };

    struct Market {
        std::map < double, double > bidSet;
        std::map < double, double > askSet;
    };

    struct MarketUpdate {
        double price;
        double volume;
        std::string entryId;
    };

    struct DepthSnapshot {
        std::vector < MarketUpdate > bids;
        std::vector < MarketUpdate > asks;
        std::chrono::time_point < std::chrono::system_clock > time;
        size_t sequence;
    };

};


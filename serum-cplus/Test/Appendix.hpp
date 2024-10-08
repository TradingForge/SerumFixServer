#pragma once

#include <sharedlib/include/ISettings.h>
#include <sharedlib/include/ILogger.h>
#include <marketlib/include/market.h>
#include <marketlib/include/BrokerModels.h>
#include <boost/format.hpp>
#include <iostream>
#include <sstream>
#include <instruments/converter.hpp>

using namespace std;
using namespace marketlib;
using namespace BrokerModels;


class Logger: public ILogger
{
private:

	typedef std::string string;

public:

	void Info(const char *content, ...) override;
	void Debug(const char *content, ...) override;
	void Error(const char *content, ...) override;
    void Critical(const char *content, ...) override;
	void Warn(const char *content, ...) override;
    void Trace(const char *content, ...) override;

	~Logger() = default;
};

void Logger::Info(const char *content, ...) {
	cout << "INFO | " << content << "\n";
}
void Logger::Debug(const char *content, ...) {
	cout << "INFO | " << content << "\n";
}
void Logger::Error(const char *content, ...) {
	cout << "INFO | " << content << "\n";
}
void Logger::Warn(const char *content, ...) {
	cout << "INFO | " << content << "\n";
}

void Logger::Critical(const char *content, ...) {
	cout << "CRITICAL | " << content << "\n";
}

void Logger::Trace(const char *content, ...) {
	cout << "TRACE | " << content << "\n";
}



class SerumSettings : public ISettings {

private:

    typedef std::string string;

public:

    string get(Property property) const override {
        switch (property) {
            case Property::ExchangeName:
                return "Serum";
            case Property::WebsocketEndpoint:
                return "wss://vial.mngo.cloud/v1/ws";
            default:
                return "";
        }
    }
};

string formatExecutionReport(const string& exchangeName, const string &symbol, const execution_report_t& report) {
    std::string state = "";
    if ( report.state == order_state_t::ost_New ) {
        state = "new";
    }
    else if ( report.state == order_state_t::ost_Filled ) {
        state = "filled";
    }
    else if ( report.state == order_state_t::ost_Canceled ) {
        state = "cancelled";
    }
    else if ( report.state == order_state_t::ost_Replaced ) {
        state = "replaced";
    }


    return (boost::format("Symbol(%1%)\nType(%2%)\nSide(%3%)\nPrice(%4%)\nAmountExecuted(%5%)\nAmountRemaining(%6%)\nStatus(%7%)\nExchId(%8%)\n\n") 
        % symbol
        % (report.orderType == order_type_t::ot_Limit ? "limit" : "market")
        % (report.side == order_side_t::os_Buy ? "buy" : "sell")
        % report.limitPrice 
        % report.cumQty
        % report.leavesQty
        % state
        % uint128toa(report.exchId)
    ).str().c_str();
}

string formatTopInfo(const string &exchangeName, const string &symbol, const MarketBook &marketBook) {
    time_t update_time = std::chrono::system_clock::to_time_t(marketBook.time);
    struct tm *_tm = gmtime(&update_time);
    std::chrono::system_clock::duration duration = marketBook.time.time_since_epoch();
    auto milli_total = std::chrono::duration_cast<std::chrono::milliseconds> (duration).count();
    int milli_since = milli_total % 1000;
    char buff[64];
    //  "timestamp": "2021-03-24 01:00:55.586",
    sprintf(buff, "%.4d%.2d%.2d %.2d:%.2d:%.2d.%.3d",
            _tm->tm_year+1900, _tm->tm_mon, _tm->tm_mday,_tm->tm_hour,_tm->tm_min,_tm->tm_sec, milli_since);

    return (boost::format("%1%\nAsk(%2%) AskSize(%3%) --- Bid(%4%) BidSize(%5%), update_time(%6%)")
        % symbol 
        % marketBook.askPrice 
        % marketBook.askSize 
        % marketBook.bidPrice 
        % marketBook.bidSize
        % buff).str().c_str();
} 


string formatDepthInfo(const string &exchangeName, const string &symbol, const BrokerModels::DepthSnapshot& depth){
    int count = 7;
    std::ostringstream strs;
    strs << symbol << "\nAsks\n";

    // std::list<BrokerModels::MarketUpdate>::const_iterator it; // объявляем итератор
    auto ask = depth.asks.cbegin();
    auto ask_e = --depth.asks.cbegin();
    advance(ask, count - 1);
    for (; ask != ask_e; ask--) {
        strs << ask->volume << "  " << ask->price << std::endl;
    }

    auto bid = depth.bids.cbegin();
    auto bid_e = depth.bids.cbegin();
    advance(bid_e, count);
    strs << "\nBids\n";
    for (; bid != bid_e; bid++) {
        strs << bid->volume << "  " << bid->price << std::endl;
    }

   return strs.str().c_str();
};
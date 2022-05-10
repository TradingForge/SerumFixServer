//
// Created by traid on 19.02.2020.
//

#pragma once

#include <unordered_map>
#include <string>
#include <marketlib/include/market.h>
#include <marketlib/include/API.h>
#include <sharedlib/include/Logger.h>

class LMAXPrice_session;
class LMAXPrice_channel : public marketlib::IPriceChannel {
public:
    LMAXPrice_channel()=delete;
    explicit LMAXPrice_channel(const LoggerPtr& logger):_logger(logger){}
    ~LMAXPrice_channel() noexcept override = default;

    void start(const std::string &cfg, const std::string& session, const marketlib::ChannelListenerPtr& ) override;
    void stop() noexcept override;
    const std::string &name() const noexcept override;
    const std::string& platform() const noexcept override;
    bool IsConnected() const  noexcept override;

    bool get_properties(const marketlib::price_channel_properties &) const noexcept override;
    void top_subscribe  (marketlib::UserId , const marketlib::InstrumentPtr& , const marketlib::OnTopListener &) override;
    void top_unsubscribe(marketlib::UserId , const marketlib::InstrumentPtr& ) override ;
private:
    void onPriceUpdateListener(const std::string& symbol, const marketlib::md_snapshot_t& snapshot);
    std::unique_ptr<FIX8::ClientSessionBase> _session_ptr;
    std::string _session;
    std::unordered_map<std::string,marketlib::OnTopListener> _listeners;
    std::mutex _mutex;
    LoggerPtr _logger;
};


//
// Created by traid on 23.02.2020.
//

#pragma once

#include <unordered_map>
#include <string>

#include <marketlib/include/market.h>
#include <marketlib/include/API.h>
#include <sharedlib/include/Logger.h>

class LMAXTrade_session;
class LMAXTrade_channel : public marketlib::ITradeChannel {
public:
    LMAXTrade_channel()=delete;
    explicit LMAXTrade_channel(const LoggerPtr& logger) : _logger(logger){}
     ~LMAXTrade_channel() noexcept override = default;

    void start(const std::string &cfg, const std::string& session, const marketlib::ChannelListenerPtr& ) override;
    void stop() noexcept override;
    const std::string& name() const noexcept override;
    const std::string& platform() const noexcept override;
    bool IsConnected() const  noexcept override;

    std::string generateOrderId() const noexcept ;
    bool  get_properties(const marketlib::trade_channel_properties& option) const  noexcept override;
    void send  (const marketlib::InstrumentPtr& instr, const marketlib::order_t& order) override;
    void cancel (const marketlib::InstrumentPtr& instr, const marketlib::order_t& order) override;
    void replace (const marketlib::InstrumentPtr& instr, const marketlib::order_t& order, const marketlib::order_update_t& update) override;

private:
    std::unique_ptr<FIX8::ClientSessionBase> _session_ptr;
    std::string _session;
    LoggerPtr _logger;
};


//
// Created by Admin on 2/2/2020.
//

#pragma once

#include <fix8/f8includes.hpp>
#include <fix8/usage.hpp>
#include <fix8/consolemenu.hpp>
#include <fix8/multisession.hpp>

#include "FIX8_LMAXTrade_router.hpp"
#include "FIX8_LMAXTrade_types.hpp"
#include "FIX8_LMAXTrade_classes.hpp"

#include <marketlib/include/market.h>
#include <marketlib/include/API.h>
#include <sharedlib/include/Logger.h>

class LMAXTrade_session : public FIX8::Session , public FIX8::LMAXTrade::FIX8_LMAXTrade_Router {
public:
    LMAXTrade_session(const FIX8::F8MetaCntx& ctx,
                       const FIX8::SessionID& sid,
                       FIX8::Persister *persist=nullptr,
                       FIX8::Logger *logger=nullptr,
                       FIX8::Logger *plogger=nullptr);
    void setChannelListener(const marketlib::ChannelListenerPtr& listener)noexcept{ _channel_listener = listener;}
    void setName(const std::string& name)noexcept{  _name=name;}
    void setConfig(const XmlElement * session_cfg)noexcept{_session_cfg=session_cfg;}
    void setLogger(const LoggerPtr& logger){_logger=logger;}

    void send (const marketlib::InstrumentPtr& instr, const marketlib::order_t& order);
    void cancel (const marketlib::InstrumentPtr& instr, const marketlib::order_t& order);
    void replace (const marketlib::InstrumentPtr& instr, const marketlib::order_t& order, const marketlib::order_update_t& update);

    bool isConnected() const noexcept {return _is_connected;}
private:

    bool handle_application(const unsigned seqnum, const FIX8::Message *&msg) override;
    bool handle_logon(const unsigned seqnum, const FIX8::Message *msg) override;
    bool handle_logout(const unsigned seqnum, const FIX8::Message *msg) override;
    void modify_outbound(FIX8::Message *msg) override;
    bool process(const FIX8::f8String& from) override;

    FIX8::Message *generate_logon(const unsigned heartbeat_interval, const FIX8::f8String davi=FIX8::f8String()) override;
    bool operator() (const class FIX8::LMAXTrade::ExecutionReport *msg) const override ;
    bool operator() (const class FIX8::LMAXTrade::OrderCancelReject *msg) const override;
    bool operator() (const class FIX8::LMAXTrade::TradeCaptureReport *msg) const override;
    bool operator() (const class FIX8::LMAXTrade::TradeCaptureReportRequestAck *msg) const override;

    void getTrades(int last_days);

private:
    marketlib::ChannelListenerPtr _channel_listener;
    std::string _name;
    const XmlElement * _session_cfg;
    bool _display_debug=true;
    bool _is_connected=false;
    std::chrono::high_resolution_clock::time_point ping_start ;
    LoggerPtr _logger;

};




#pragma once

#include <fix8/f8includes.hpp>
#include <fix8/usage.hpp>
#include <fix8/consolemenu.hpp>
#include <fix8/multisession.hpp>

#include "FIX8_LMAXPrice_router.hpp"
#include "FIX8_LMAXPrice_types.hpp"
#include "FIX8_LMAXPrice_classes.hpp"

#include <marketlib/include/API.h>
#include <marketlib/include/market.h>
#include <sharedlib/include/Logger.h>

class LMAXPrice_session : public FIX8::Session ,  public FIX8::LMAXPrice::FIX8_LMAXPrice_Router{
public:
    LMAXPrice_session(const FIX8::F8MetaCntx& ctx,
    const FIX8::SessionID& sid,
            FIX8::Persister *persist=nullptr,
            FIX8::Logger *logger=nullptr,
            FIX8::Logger *plogger=nullptr);
    void setTopListener(const marketlib::OnTopListener & top_listener) noexcept{ _top_listener=top_listener;}
    void setChannelListener(const marketlib::ChannelListenerPtr& listener)noexcept{ _channel_listener = listener;}
    void setName(const std::string& name) noexcept{  _name=name;}
    void setConfig(const XmlElement * session_cfg) noexcept{_session_cfg=session_cfg;}
    void setLogger(const LoggerPtr& logger){_logger=logger;}

    bool handle_application(const unsigned seqnum, const FIX8::Message *&msg) override;
    void top_subscribe   (int reqId, const marketlib::InstrumentPtr&  instr );
    void top_unsubscribe (int reqId, const marketlib::InstrumentPtr&  instr);

    bool isConnected() const noexcept {return _is_connected;}
private:
    bool operator() (const class FIX8::LMAXPrice::MarketDataSnapshotFullRefresh* msg) const override;
    bool operator() (const class FIX8::LMAXPrice::MarketDataRequestReject* msg) const override;

    FIX8::Message *generate_logon(const unsigned heartbeat_interval, const FIX8::f8String davi=FIX8::f8String()) override;
    bool handle_logon(const unsigned seqnum, const FIX8::Message *msg) override;
    bool handle_logout(const unsigned seqnum, const FIX8::Message *msg)override;
    void modify_outbound(FIX8::Message *msg)override;
    bool process(const FIX8::f8String& from)override;

private:
    static std::vector<marketlib::InstrumentPtr> download_symbols_from_file(const std::string& file_name);
    marketlib::OnTopListener _top_listener;
    marketlib::ChannelListenerPtr _channel_listener;
    std::string _name;
    FIX8::LMAXPrice::FIX8_LMAXPrice_Router _router;
    const XmlElement * _session_cfg;
    bool _display_debug=false;
    bool _is_connected=false;
    LoggerPtr _logger;
};



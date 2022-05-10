//
// Created by traid on 19.02.2020.
//

#include <memory>
#include <functional>
#include <mutex>

#include <fix8/f8includes.hpp>
#include <fix8/usage.hpp>
#include <fix8/consolemenu.hpp>
#include <fix8/multisession.hpp>
#include <lmax/include/LMAXPrice_channel.hpp>

#include "LMAXPrice_session.hpp"
#include <lmax/include/LMAXPrice_channel.hpp>

using namespace marketlib;

using namespace FIX8::LMAXPrice;

void LMAXPrice_channel::start(const std::string &cfg, const std::string& session, const ChannelListenerPtr& listener)
{
    _session = session;
    _session_ptr = std::unique_ptr<FIX8::ClientSessionBase> (new FIX8::ReliableClientSession<LMAXPrice_session>(FIX8::LMAXPrice::ctx(), cfg, session));
     auto* mySession = (LMAXPrice_session*)_session_ptr->session_ptr();
     mySession->setTopListener(std::bind(&LMAXPrice_channel::onPriceUpdateListener,this, std::placeholders::_1, std::placeholders::_2));
     mySession->setChannelListener(listener);
     mySession->setName(_session);
     mySession->setConfig(_session_ptr->get_session_element());
     mySession->setLogger(_logger);

    _logger->LOG_INFO("Starting adapter, platform(%s) config(%s) session(%s), model(%d)\n",
                      platform().c_str(), cfg.c_str(), session.c_str(),
                      (int)_session_ptr->get_process_model(_session_ptr->get_session_element()));

     //_session_ptr->session_ptr()->control() |= FIX8::Session::print;
    _session_ptr->start(false , 1, 1, _session_ptr->session_ptr()->get_login_parameters()._davi() );
}

void LMAXPrice_channel::stop() noexcept
{
    _session_ptr->session_ptr()->stop();
}

const std::string& LMAXPrice_channel::name() const noexcept
{
    return _session;
}

const std::string& LMAXPrice_channel::platform() const noexcept {
    static const std::string platform = "LMAX";
    return platform;
}

bool LMAXPrice_channel::IsConnected() const  noexcept
{
    auto* mySession = (LMAXPrice_session*)_session_ptr->session_ptr();
    return mySession->isConnected();
}

bool LMAXPrice_channel::get_properties(const price_channel_properties & prop) const noexcept
{
    if(prop==cp_depth)
    {
        return true;
    }
    return false;
}

void LMAXPrice_channel::top_subscribe  (marketlib::UserId reqId, const marketlib::InstrumentPtr& instr, const OnTopListener&listener)
{
    const std::lock_guard<std::mutex> lock(_mutex);
    if(_listeners.find(instr->sec_id)==_listeners.end()) {
        auto pair=_listeners.insert( std::make_pair(instr->sec_id, listener) );
        if(pair.second) {
            auto *mySession = (LMAXPrice_session *) _session_ptr->session_ptr();
            mySession->top_subscribe(reqId, instr);

            _logger->LOG_INFO("Subscribe to market data: %s\n", instr->sec_id.c_str());
        }
        else  _logger->LOG_ERROR("Error Subscribe to market data: %s\n", instr->sec_id.c_str());
    }
    else
    {
        _logger->LOG_INFO("Already Subscribed to market data: %s\n", instr->sec_id.c_str());
    }
}

void LMAXPrice_channel::top_unsubscribe(marketlib::UserId reqId, const marketlib::InstrumentPtr& instr)
{
    const std::lock_guard<std::mutex> lock(_mutex);
    auto* mySession = (LMAXPrice_session* )_session_ptr->session_ptr();
    mySession->top_unsubscribe(reqId, instr);
    if(_listeners.find(instr->sec_id)!=_listeners.end()) {
        _listeners.erase(instr->sec_id);
        auto *mySession = (LMAXPrice_session *) _session_ptr->session_ptr();
        mySession->top_unsubscribe(reqId, instr);

        _logger->LOG_INFO("Unsubscribe to market data: %s\n", instr->sec_id.c_str());
    }
    else
    {
        _logger->LOG_INFO("Already Unsubscribed to market data: %s\n", instr->sec_id.c_str());
    }
}

void LMAXPrice_channel::onPriceUpdateListener(const std::string& sec_id, const marketlib::md_snapshot_t& snapshot)
{
    const std::lock_guard<std::mutex> lock(_mutex);
    auto it=_listeners.find(sec_id);
    if(it!=_listeners.end()){
        it->second(sec_id,snapshot);
    }
}


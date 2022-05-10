//
// Created by traid on 23.02.2020.
//

#include <fix8/f8includes.hpp>
#include <fix8/usage.hpp>
#include <fix8/consolemenu.hpp>
#include <fix8/multisession.hpp>
#include <fix8/sessionwrapper.hpp>

#include "LMAXTrade_session.hpp"
#include <lmax/include/LMAXTrade_channel.hpp>

void LMAXTrade_channel::start(const std::string &cfg, const std::string& session, const marketlib::ChannelListenerPtr& listener)
{
    _session = session;
    _session_ptr = std::unique_ptr<FIX8::ClientSessionBase> (new FIX8::ReliableClientSession<LMAXTrade_session>(FIX8::LMAXTrade::ctx(), cfg, session));
    auto* mySession = (LMAXTrade_session*)_session_ptr->session_ptr();
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

void LMAXTrade_channel::stop() noexcept
{
    _session_ptr->session_ptr()->stop();
}

const std::string &LMAXTrade_channel::name() const noexcept
{
    return _session;
}

const std::string& LMAXTrade_channel::platform() const noexcept
{
    static const std::string platform = "LMAX";
    return platform;
}

bool LMAXTrade_channel::IsConnected() const  noexcept
{
    auto* mySession = (LMAXTrade_session*)_session_ptr->session_ptr();
    return mySession->isConnected();
}

 std::string LMAXTrade_channel::generateOrderId() const noexcept
{
    static const char alphanum[] =
            "0123456789"
            "!@#$%^&*"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz";
    constexpr int stringLength = sizeof(alphanum) - 1;
    auto getChar = []()
    {
        return alphanum[rand() % stringLength];
    };
    srand(time(0));
    std::string Id;
    Id.reserve(32);
    static int tail = 0;
    for(unsigned int i = 0; i < 14; ++i)
        Id += getChar();
    Id += alphanum[tail++];
    if(tail >= stringLength)
        tail=0;
    return Id;
}

bool  LMAXTrade_channel::get_properties(const marketlib::trade_channel_properties& prop) const  noexcept
{
   switch(prop)
   {
       case marketlib::tc_stop:
           return true;
       case marketlib::tc_stop_limit:
           return true;
       case marketlib::tc_hist_trades:
           return true;
       case marketlib::tc_positions:
           return false;
       case marketlib::tc_account_details:
            return false;
       default:
           return false;
   }
}

void LMAXTrade_channel::send  (const marketlib::InstrumentPtr& instr, const marketlib::order_t& order)
{
    LMAXTrade_session *mySession = (LMAXTrade_session *) _session_ptr->session_ptr();
    mySession->send(instr, order);
}

void LMAXTrade_channel::cancel (const marketlib::InstrumentPtr& instr, const marketlib::order_t& order)
{
    LMAXTrade_session *mySession = (LMAXTrade_session *) _session_ptr->session_ptr();
    mySession->cancel(instr, order);
}

void LMAXTrade_channel::replace (const marketlib::InstrumentPtr& instr, const marketlib::order_t& order, const marketlib::order_update_t& update)
{
    LMAXTrade_session *mySession = (LMAXTrade_session *) _session_ptr->session_ptr();
    mySession->replace(instr, order, update);
}


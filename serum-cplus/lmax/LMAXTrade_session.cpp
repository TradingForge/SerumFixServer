//
// Created by Admin on 2/2/2020.
//

#include <chrono>
#include <string.h>
#include <functional>

#include "LMAXTrade_session.hpp"

using namespace FIX8::LMAXTrade;

LMAXTrade_session::LMAXTrade_session(const FIX8::F8MetaCntx& ctx,
                                     const FIX8::SessionID& sid,
                                     FIX8::Persister *persist,
                                     FIX8::Logger *logger,
                                     FIX8::Logger *plogger):
                                     Session(ctx, sid, persist, logger, plogger),
                                     FIX8::LMAXTrade::FIX8_LMAXTrade_Router(),
                                     _channel_listener(),
                                     _session_cfg(nullptr){
}

bool LMAXTrade_session::handle_application(const unsigned seqnum, const FIX8::Message *&msg)
{
   // _logger->LOG_INFO("LMAXTrade_session: handle_application\n");
    /*
    if(enforce(seqnum,msg))
    {
        std::cout << " enforce checking problem\n";
        return false;
    }
    if(!msg->process(*this) )
        detach(msg);
    return true;
    */
    return enforce(seqnum, msg) || msg->process(*this);
}

void LMAXTrade_session::send (const marketlib::InstrumentPtr& instr, const marketlib::order_t& order)
{
    auto *nos(new NewOrderSingle);
    *nos << new ClOrdID(order.clId)
         << new Side(order.side)
         << new TransactTime()
         << new OrdType(order.type)
         << new TimeInForce(order.tif)
         << new OrderQty(order.remaining_qty)
         << new SecurityID(instr->sec_id)
         << new SecurityIDSource("8")
            ;

    if (order.type == marketlib::order_type_t::ot_Limit)
    {
        *nos << new Price(order.price, instr->tick_precision);
    }
    else if (order.type == marketlib::order_type_t::ot_Stop_Loss)
    {
        *nos << new StopPx(order.stopPrice, instr->tick_precision);
    }
    else if (order.type == marketlib::order_type_t::ot_Stop_Limit)
    {
        *nos << new Price(order.price, instr->tick_precision)
             << new StopPx(order.stopPrice, instr->tick_precision);
    }

    ping_start = std::chrono::high_resolution_clock::now();
    FIX8::Session::send(nos);
}

void LMAXTrade_session::cancel (const marketlib::InstrumentPtr& instr, const marketlib::order_t& order)
{
    static int cancelId = 1;
    auto *cancel(new OrderCancelRequest);
    *cancel << new ClOrdID(std::to_string(cancelId++))
            << new OrigClOrdID(order.clId)
            << new Side(order.side)
            << new TransactTime()
            << new SecurityID(instr->sec_id)
            << new SecurityIDSource("8")
            << new OrderQty(order.remaining_qty)
            ;
    FIX8::Session::send(cancel);
}

void LMAXTrade_session::replace (const marketlib::InstrumentPtr& instr, const marketlib::order_t& order, const marketlib::order_update_t& update)
{
    auto *replace(new OrderCancelReplaceRequest);
    *replace<< new ClOrdID(update.clId)
            << new OrigClOrdID(update.origId)
            << new Side(order.side)
            << new TransactTime()
            << new OrderQty(update.qty)
            << new OrdType(order.type)
            << new TimeInForce(order.tif)
            << new SecurityIDSource("8")
            ;
    if (order.type == marketlib::order_type_t::ot_Limit)
    {
        *replace << new Price(update.price,  instr->tick_precision);
    }
    else if (order.type == marketlib::order_type_t::ot_Stop_Loss)
    {
        *replace << new StopPx(update.stopPrice,  instr->tick_precision);
    }
    else if (order.type == marketlib::order_type_t::ot_Stop_Limit)
    {
        *replace << new Price(update.price,  instr->tick_precision)
                << new StopPx(update.stopPrice,  instr->tick_precision);
    }
    FIX8::Session::send(replace);
}

void LMAXTrade_session::getTrades(int last_days) {

    static int reqId = 1;
    auto *request(new TradeCaptureReportRequest);
    *request<< new TradeRequestID(std::to_string(reqId++))
            << new TradeRequestType(1)
            << new NoDates(2)
            ;
    std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
    FIX8::GroupBase * noDatesGroup(request->find_group< TradeCaptureReportRequest::NoDates >());
    {
        FIX8::MessageBase *noDatesGroup_from(noDatesGroup->create_group());
        //*noDatesGroup_to << new TransactTime("20200218-00:00:00.000");
        std::time_t start = std::chrono::system_clock::to_time_t(now - std::chrono::hours(24 * last_days-1));
        std::tm tm = *std::localtime(&start);
        char buffer[64];
        std::strftime (buffer,64,"%Y%m%d-%H:%M:%S.000", &tm);
        *noDatesGroup_from << new TransactTime(buffer);
        *noDatesGroup << noDatesGroup_from;
    }
    {
        FIX8::MessageBase *noDatesGroup_to(noDatesGroup->create_group());
        std::time_t end = std::chrono::system_clock::to_time_t(now );
        std::tm tm = *std::localtime(&end);
        char buffer[64];
        std::strftime (buffer,64,"%Y%m%d-%H:%M:%S.000", &tm);
        *noDatesGroup_to << new TransactTime(buffer);
        *noDatesGroup << noDatesGroup_to;
    }

    *request << noDatesGroup;
    FIX8::Session::send(request);
}

bool LMAXTrade_session::operator() (const ExecutionReport *msg) const
{
    marketlib::execution_report_t report;

    ExecType exec_type;
    if (msg->get(exec_type) && exec_type.is_valid())
        report.type = (marketlib::report_type_t)exec_type.get();
    else printf("ExecutionReport-> can't get ExecType");
    // calculate ping for an order
    if(report.type == marketlib::report_type_t::rt_new)  {
        std::chrono::nanoseconds sec = std::chrono::high_resolution_clock::now() - ping_start;
        report.ping_milli = sec.count() / 1000.0 / 1000.0;
    }
    ClOrdID clid;
    if (msg->get(clid) && clid.is_valid())
        report.clId = clid.get();
    else printf("ExecutionReport-> can't get ClOrdID");
    OrdStatus ord_status;
    if (msg->get(ord_status) && ord_status.is_valid())
        report.state = (marketlib::order_state_t)ord_status.get();
    else printf("ExecutionReport-> can't get OrdStatus");
    OrderID id;
    if (msg->get(id) && id.is_valid())
        report.exchId = id.get();
    else printf("ExecutionReport-> can't get OrderID");
    OrigClOrdID origClId;
    if (msg->get(origClId) && origClId.is_valid())
        report.origClId = origClId.get();
    ExecID execId;
    if (msg->get(execId) && execId.is_valid())
        report.tradeId = execId.get();
    else printf("ExecutionReport-> can't get ExecID");
    OrdRejReason rej_reason;
    if (msg->get(rej_reason) && rej_reason.is_valid())
        report.rejReason = (marketlib::ord_rej_reason)rej_reason.get();
    Side side;
    if (msg->get(side) && side.is_valid())
        report.side = (marketlib::order_side_t)side.get();
    else printf("ExecutionReport-> can't get Side");
    OrdType ordType;
        if (msg->get(ordType) && ordType.is_valid())
        report.orderType = (marketlib::order_type_t)ordType.get();
    TimeInForce tif;
    if (msg->get(tif) && tif.is_valid())
        report.tif = (marketlib::time_in_force_t)tif.get();
    Price price;
    if (msg->get(price) && price.is_valid())
        report.limitPrice = price.get();
    StopPx stopPrice;
    if (msg->get(stopPrice) && stopPrice.is_valid())
        report.stopPrice = stopPrice.get();
    LastPx last_px;
    if (msg->get(last_px) && last_px.is_valid())
        report.lastPx = last_px.get();
    AvgPx avgPrice;
    if (msg->get(avgPrice) && avgPrice.is_valid())
        report.avgPx = avgPrice.get();

    LastQty exec_qty;
    if (msg->get(exec_qty) && exec_qty.is_valid())
        report.lastShares = exec_qty.get();
    CumQty cum_qty;
    if (msg->get(cum_qty) && cum_qty.is_valid())
        report.cumQty = cum_qty.get();
    LeavesQty leaves_qty;
    if (msg->get(leaves_qty) && leaves_qty.is_valid())
        report.leavesQty = leaves_qty.get();

    Text text;
    if (msg->get(text) && text.is_valid())
        report.text = text.get();
    std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
    report.time =std::chrono::system_clock::to_time_t(now);

    SecurityID secID;
    if (msg->get(secID) && secID.is_valid())
        report.secId = secID.get();
    else printf("ExecutionReport-> can't get SecurityID");

    // TransactTime, tag 60
    // SettleDate. tag 64

    _channel_listener->onReport(_name, report);
   //printf("<--- ExecutionReport 39=%d 150=%d for %s", (char)report.state,(char)report.type,report.clId.c_str() );
    return false;
}

bool LMAXTrade_session::operator() (const OrderCancelReject *msg) const
{
    marketlib::execution_report_t report;
    std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
    report.time =std::chrono::system_clock::to_time_t(now);

    OrderID id;
    if (msg->get(id) && id.is_valid())
        report.exchId = id.get();
    else printf("ExecutionReport-> can't get OrderID");
    ClOrdID clid;
    if (msg->get(clid) && clid.is_valid())
        report.clId = clid.get();
    else printf("ExecutionReport-> can't get ClOrdID");
    OrigClOrdID origClId;
    if (msg->get(origClId) && origClId.is_valid())
        report.origClId = origClId.get();
    else printf("ExecutionReport-> can't get OrigClOrdID");

    OrdStatus ord_status;
    if (msg->get(ord_status) && ord_status.is_valid())
        report.state = (marketlib::order_state_t)ord_status.get();
    else printf("ExecutionReport-> can't get OrdStatus");

    CxlRejResponseTo response;
    if (msg->get(response) && response.is_valid())
    {
        char reason = response.get();
        if (reason == CxlRejResponseTo_ORDCXLREQ )
            report.type = marketlib::report_type_t::rt_cancel_rejected;
        else if (reason == CxlRejResponseTo_ORDCXLREPREQ)
            report.type = marketlib::report_type_t::rt_replace_rejected;
    }

    Text text;
    if (msg->get(text) && text.is_valid())
        report.text = text.get();

    _channel_listener->onReport(_name, report);
    //printf("<--- ExecutionReport 39=%d 150=%d for %s", (char)report.state,(char)report.type,report.clId.c_str() );
    return false;
}

bool LMAXTrade_session::operator() (const TradeCaptureReportRequestAck *msg) const
{
    //748	TotNumTradeReports	Total number of trade reports returned. Will only be set when no trades were found for the requested criteria when it's value will be 0.
    //749	TradeRequestResult	Result of Trade Request
    //LMAX Supported values:
    //  0 = Successful (Default)
    //  99 = Other
    //750	TradeRequestStatus	Status of Trade Request.
    //        LMAX Supported values:
    //0 = Accepted
    //2 = Rejected
    //58	Text	Contains error message when Trade Capture Report Request is rejected.

    TradeRequestID id;
    msg->get(id);
    std::string reqId = id.get();
    TotNumTradeReports totNum;
    int numReports = 0;
    if (msg->get(totNum) && totNum.is_valid())
        numReports = totNum.get();
    TradeRequestResult tradeResult;
    msg->get(tradeResult);
    int result= tradeResult.get();
    TradeRequestStatus tradeStatus;
    msg->get(tradeStatus);
    int status= tradeStatus.get();
    Text text;
    std::string errorText("");
    if (msg->get(text) && text.is_valid())
        errorText = text.get();
    /*
    Result of Trade Request
    MAX Supported values:
        0 = Successful (Default)
        99 = Other
    */
    if(result != 0)
      {
          _channel_listener->onEvent(_name, marketlib::channel_info::ci_trade_capture_error, errorText);
      }
    /*
    Status of Trade Request.
    LMAX Supported values:
      0 = Accepted
      2 = Rejected
    */
    if(status == 2)
    {
        _channel_listener->onEvent(_name, marketlib::channel_info::ci_trade_capture_reject, errorText);
    }
    _logger->LOG_INFO("<--- TradeCaptureReportRequestAck id=%s, totNum=%d, result=%d, status=%d\n", reqId.c_str(), numReports, result, status);
    return false;
}

bool LMAXTrade_session::operator() (const TradeCaptureReport *msg) const
{
/*
     Listener::onEvent, lmax_fix8_ord logged on
---> header ("header")
   BeginString (8): FIX.4.4
   BodyLength (9): 0
   MsgType (35):
   SenderCompID (49): void19
   TargetCompID (56): LMXBD
   MsgSeqNum (34): 2
   SendingTime (52): 20200311-00:36:40.250
TradeCaptureReportRequest ("AD")
   TradeRequestID (568): 1
   TradeRequestType (569): 1
   NoDates [TrdCapDtGrp] (580): 2
   NoDates (Repeating group 1/2)
      TransactTime (60): 20200304-01:36:40.000
   NoDates (Repeating group 2/2)
      TransactTime (60): 20200311-00:36:40.000
trailer ("trailer")
   CheckSum (10):
Listener::onEvent, lmax_fix8_md logged on
<--- 8=FIX.4.49=8535=AQ49=LMXBD56=void1934=252=20200311-00:36:40.254568=1569=1263=0749=0750=010=010
<--- 8=FIX.4.49=23635=AE49=LMXBD56=void1934=352=20200311-00:36:40.254568=1912=N17=PAoE5wAAAABGCOiz527=QACESAAAATKANRYP48=400122=832=131=1.1455575=2020030960=20200309-13:15:17.797552=154=137=AAAESQAAAAAcR4M511=792cG0z%756b&e1=100728957510=016
<--- 8=FIX.4.49=23635=AE49=LMXBD56=void1934=452=20200311-00:36:40.254568=1912=N17=PAoE5wAAAABGCOjc527=QACESAAAATKANW5W48=400122=832=131=1.1455275=2020030960=20200309-13:15:27.796552=154=137=AAAESQAAAAAcR4NU11=kQz^AB8KvK#ZqK1=100728957510=235
<--- 8=FIX.4.49=23835=AE49=LMXBD56=void1934=552=20200311-00:36:40.254568=1912=Y17=PAoE5wAAAABGEWj6527=QACESAAAATLMESRU48=400122=832=1.131=1.1361475=2020031060=20200310-14:29:11.904552=154=237=AAAESQAAAAAcS7S411=IgQbHBuH*^wJEX1=100728957510=025
 */

    marketlib::execution_report_t report;
    ExecID execId;// tag 17 is not required, stupid!
    if(msg->get(execId) && execId.is_valid()){
        report.tradeId = execId.get();
    }
    LastQty lastQty;
    msg->get(lastQty);
    report.lastShares = lastQty.get();
    LastPx lastPrice;
    msg->get(lastPrice);
    report.lastPx = lastPrice.get();
    TradeDate  date;
    msg->get(date);
    TransactTime  time;
    msg->get(date);
    const FIX8::GroupBase *noSidesGroup(msg->find_group<TradeCaptureReport::NoSides>());
    assert(noSidesGroup!=nullptr);
    auto element = noSidesGroup->get_element(0);

    Side side;
    element->get(side);
    report.side = (marketlib::order_side_t)side.get();
    OrderID orderId;
    element->get(orderId);
    report.exchId = orderId.get();
    ClOrdID clId;// not required tag
    if(element->get(clId) && clId.is_valid())
    {
        report.clId = clId.get();
    }
    SecurityID secID;
    msg->get(secID);
    report.secId = secID.get();
    _channel_listener->onHistReport(_name, report);

    FIX8::LMAXTrade::LastRptRequested isLast;
    if(msg->get(isLast) && isLast.get())
    {
        _channel_listener->onEvent(_name, marketlib::channel_info::ci_trade_downloaded, "");
    }
    return false;
}

FIX8::Message *LMAXTrade_session::generate_logon(const unsigned heartbeat_interval, const FIX8::f8String davi)
{
    FIX8::Message* logon = FIX8::Session::generate_logon(heartbeat_interval, davi);
    std::string username;
    if (_session_cfg->GetAttr("username", username))
    {
        *logon << new FIX8::LMAXTrade::Username(username);
    }
    std::string password;
    if (_session_cfg->GetAttr("password", password))
    {
        *logon << new FIX8::LMAXTrade::Password(password);
    }
    return logon;
}


 bool LMAXTrade_session::handle_logon(const unsigned seqnum, const FIX8::Message *msg)
{
    //std::cout << "LMAXTrade_session: handle_logon\n";
    _logger->LOG_INFO("LMAXTrade_session: handle_logon\n");
    _is_connected=true;
    _channel_listener->onEvent(_name, marketlib::channel_info::ci_logon,"");
    getTrades(7);
    return FIX8::Session::handle_logon(seqnum, msg);
}

 bool LMAXTrade_session::handle_logout(const unsigned seqnum, const FIX8::Message *msg)
{
   // std::cout << "LMAXTrade_session: handle_logout\n";
    _logger->LOG_INFO("LMAXTrade_session: handle_logout\n");
    _is_connected=false;
    _channel_listener->onEvent(_name, marketlib::channel_info::ci_logout,"");
    return FIX8::Session::handle_logon(seqnum, msg);
}

 void LMAXTrade_session::modify_outbound(FIX8::Message *msg)
{

    if(_display_debug) {
       // std::ostringstream stream;
       // msg->print(stream);
       // _logger->LOG_DEBUG("---> %.200s\n",  stream.str().c_str());
    }
    return FIX8::Session::modify_outbound(msg);
}

bool LMAXTrade_session::process(const FIX8::f8String& from)
{
    if(_display_debug) {
        if(from.find("35=0") == -1)
            _logger->LOG_INFO("<--- %s\n", from.c_str());
    }
    return FIX8::Session::process(from);
}






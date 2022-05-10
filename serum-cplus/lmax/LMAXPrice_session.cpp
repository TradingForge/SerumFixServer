#include <string.h>
#include <iostream>     // std::cout, std::ios
#include <sstream>      // std::ostringstream
#include <fstream>

#include "LMAXPrice_session.hpp"

using namespace FIX8::LMAXPrice;

LMAXPrice_session::LMAXPrice_session(const FIX8::F8MetaCntx& ctx,
                                     const FIX8::SessionID& sid,
                                     FIX8::Persister *persist,
                                     FIX8::Logger *logger,
                                     FIX8::Logger *plogger):
                                     Session(ctx, sid, persist, logger, plogger),
                                     FIX8::LMAXPrice::FIX8_LMAXPrice_Router(),
                                     _channel_listener(),
                                     _top_listener(),
                                     _session_cfg(nullptr){
}

bool LMAXPrice_session::handle_application(const unsigned seqnum, const FIX8::Message *&msg)
 {
    return enforce(seqnum, msg) || msg->process(*this);
}

void LMAXPrice_session::top_subscribe   (int subscrId,const marketlib::InstrumentPtr& instr)
{
    auto *mdr(new MarketDataRequest);
    *mdr    << new MDReqID(instr->sec_id)
            << new SubscriptionRequestType(FIX8::LMAXPrice::SubscriptionRequestType_SNAPSHOTUPDATE)
            << new MarketDepth(1)
            << new MDUpdateType(FIX8::LMAXPrice::MDUpdateType_FULL)
            << new NoMDEntryTypes(2)
            << new NoRelatedSym(1);
    {
        FIX8::GroupBase *nomd(mdr->find_group< MarketDataRequest::NoMDEntryTypes >());
        FIX8::MessageBase *nomd_bid(nomd->create_group());
        *nomd_bid << new MDEntryType(MDEntryType_BID); // bids
        *nomd << nomd_bid;
        FIX8::MessageBase *nomd_ask(nomd->create_group());
        *nomd_ask << new MDEntryType(MDEntryType_OFFER); // offers
        *nomd << nomd_ask;
        *mdr << nomd;
    }

    {
        FIX8::GroupBase *noin(mdr->find_group<MarketDataRequest::NoRelatedSym >());
        FIX8::MessageBase *noin_sym(noin->create_group());
        *noin_sym << new SecurityID(instr->sec_id);
        *noin_sym << new SecurityIDSource("8");
        *noin << noin_sym;
        *mdr << noin;
    }
    FIX8::Session::send(mdr);
}

void LMAXPrice_session::top_unsubscribe (int subscrId,const marketlib::InstrumentPtr& instr)
{
    auto *mdr(new MarketDataRequest);
    *mdr    << new MDReqID(instr->sec_id)
            << new SubscriptionRequestType(FIX8::LMAXPrice::SubscriptionRequestType_UNSUBSCRIBE)
            << new MarketDepth(1)
            << new MDUpdateType(FIX8::LMAXPrice::MDUpdateType_FULL)
            << new NoMDEntryTypes(2)
            << new NoRelatedSym(1);
    {
        FIX8::GroupBase *nomd(mdr->find_group< MarketDataRequest::NoMDEntryTypes >());
        FIX8::MessageBase *nomd_bid(nomd->create_group());
        *nomd_bid << new MDEntryType(MDEntryType_BID); // bids
        *nomd << nomd_bid;
        FIX8::MessageBase *nomd_ask(nomd->create_group());
        *nomd_ask << new MDEntryType(MDEntryType_OFFER); // offers
        *nomd << nomd_ask;
        *mdr << nomd;
    }

    {
        FIX8::GroupBase *noin(mdr->find_group<MarketDataRequest::NoRelatedSym >());
        FIX8::MessageBase *noin_sym(noin->create_group());
        *noin_sym << new SecurityID(instr->sec_id);
        *noin_sym << new SecurityIDSource("8");
        *noin << noin_sym;
        *mdr << noin;
    }
    FIX8::Session::send(mdr);
}

bool LMAXPrice_session::operator() (const class MarketDataSnapshotFullRefresh* msg) const
{
    SecurityID secId;
    if (!msg->get(secId) || !secId.is_valid()) {
        printf("MarketDataSnapshotFullRefresh, can't get secId\n");
        return false;
    }
    NoMDEntries numMdEntries;
    if (!msg->get(numMdEntries) || !numMdEntries.is_valid()) {
        printf("MarketDataSnapshotFullRefresh-> can't get NoMDEntries\n");
        return false;
    }
    int count = numMdEntries.get();
    marketlib::md_snapshot_t snapshot;
    for (int i =0; i < count; ++i)
    {
        const FIX8::GroupBase *mdEntriesGroup(msg->find_group<MarketDataSnapshotFullRefresh::NoMDEntries>());
        if(mdEntriesGroup == 0) {
            continue;
        }
        auto element = mdEntriesGroup->get_element(i);
        MDEntryType   entry;
        if (!element->get(entry) || !entry.is_valid()) {
            continue;
        }
        MDEntryPx  price;// Required=N
        if (!element->get(price) || !price.is_valid()) {
            continue;
        }
        MDEntrySize   size;// Required=N
        if (!element->get(size) || !size.is_valid()) {
            continue;
        }
        if(i==0)
        {
            /*
             272	MDEntryDate	Date of Market Data Entry. This tag will only be present on the first entry in the repeating group	N	UTCDateOnly
             273	MDEntryTime	Time of Market Data Entry. This tag will only be present on the first entry in the repeating group	N	UTCTimeOnly

              <--- 8=FIX.4.49=16335=W49=LMXBDM56=void1934=952=20200327-03:58:15.905262=400148=400122=8268=2269=0270=1.10828271=46272=20200327273=03:58:15.832269=1270=1.10833271=4610=216
             272=20200327 273=03:58:15.832269=1
             */
            MDEntryDate date;
            if (!element->get(date)){
                continue;
            }
            MDEntryTime time;
            if (!element->get(time)){
                continue;
            }
            FIX8::Tickval d = date.get();
            FIX8::Tickval t = time.get();
            auto dt = d+t;
            snapshot.time = std::chrono::time_point<std::chrono::system_clock>
                    (std::chrono::seconds(dt.secs())+std::chrono::milliseconds(dt.msecs()));
            //printf("TICKVAL: (%u - %llu) : (%u - %llu) : (%u - %llu)\n",
            //        d.secs(), d.msecs(), t.secs(), t.msecs(), dt.secs(), dt.msecs());
        }
        switch (entry.get()) {
            case MDEntryType_BID:
                snapshot.bid = price.get();
                snapshot. bid_size = size.get();
                // printf("type=%c, price=%.5f",  entry.get() , price.get());
                break;
            case MDEntryType_OFFER:
                snapshot.ask = price.get();
                snapshot. ask_size = size.get();
                // printf("type=%c, price=%.5f",  entry.get() , price.get());
                break;
        }
    }
    _top_listener(secId.get(), snapshot);
    // next listener was added by testing purposes
    _channel_listener->onTop(_name, secId.get(), snapshot);
    return false;
}

bool LMAXPrice_session::operator() (const class MarketDataRequestReject* msg)const
{
    MDReqID reqId;
    if (!msg->get(reqId) || !reqId.is_valid()){
        printf("MsgType_MARKETDATAREJECT->Tag 'MDReqID' is missing\n");
        return false;
    }
    /*
     tag 281 is not required
     Reason for the rejection of a Market Data request.
      LMAX supported values:
      0 = Unknown symbol
      1 = Duplicate MDReqID
      4 = Unsupported SubscriptionRequestType
      5 = Unsupported MarketDepth
      6 = Unsupported MDUpdateType
      8 = Unsupported MDEntryType
    */
    MDReqRejReason rejReason;
    std::string rejReasonStr;
    if (msg->get(rejReason) && rejReason.is_valid()) {
        rejReasonStr = rejReason.get();
    }
    Text text;
    std::string textStr;
    if (msg->get(text) && text.is_valid()) {
        textStr = text.get();
    }

    _channel_listener->onEvent(_name, marketlib::channel_info::ci_md_reject, reqId.get() + ":" + rejReasonStr + ":" + textStr);
    return false;

}

FIX8::Message *LMAXPrice_session::generate_logon(const unsigned heartbeat_interval, const FIX8::f8String davi)
{
    FIX8::Message* logon = FIX8::Session::generate_logon(heartbeat_interval, davi);
    std::string username;
    if (_session_cfg->GetAttr("username", username))
    {
        *logon << new FIX8::LMAXPrice::Username(username);
    }
    std::string password;
    if (_session_cfg->GetAttr("password", password))
    {
        *logon << new FIX8::LMAXPrice::Password(password);
    }
    return logon;
}

std::vector<marketlib::InstrumentPtr> LMAXPrice_session::download_symbols_from_file(const std::string& file_name)
{
    std::vector<marketlib::InstrumentPtr> data;
    std::fstream fin;
    // Open an existing file
    fin.open("LMAX-Instruments.csv", std::ios::in);
    if(!fin.is_open())
        return data;
    std::string line, header;
    std::getline(fin, header, '\n');
    while(std::getline(fin, line, '\n'))
    {
        std::stringstream s(line);
       //std::cout << line << std::endl;
       // Instrument Name,LMAX ID,LMAX symbol ,Contract Multiplier,Tick Size,Tick Value,Effective Date,Expiry Date,Quoted CCY,Margin Rate (%),Position Threshold (Contracts),LMAX Trading Hours
       //AUD/CAD,100667,AUD/CAD,10000,0.00001,0.1,14/03/2012,,CAD,1,2530,17:05 Sun - 17:00 Fri EST / EDT (except 17:00 - 17:05 daily)
       marketlib::InstrumentPtr i = std::make_shared<marketlib::instrument_descr_t>();
       std::string item;
       std::vector<std::string> lines;
        while (std::getline(s, item, ',')) {
            // add all the column data
            // of a row to a vector
            lines.push_back(item);
        }
        i->sec_id     = lines[1];
        i->symbol     = lines[2];
        i->lot_size   = std::stoi(lines[3]);
        i->tick_value = std::stod(lines[4]);
        i->currency   = lines[8];
        i->tick_precision = 0;
        double value =  i->tick_value;
        while(true)
        {
            // 0.010001
            // 0.00000999999
            value*=10;
            i->tick_precision++;
            if(value + 0.5 > 1.0)
                break;
        }
        data.push_back(i);
    }
    fin.close();
    return std::move(data);
}

bool LMAXPrice_session::handle_logon(const unsigned seqnum, const FIX8::Message *msg)
{
    _is_connected=true;
    _channel_listener->onEvent(_name, marketlib::channel_info::ci_logon,"");
    try {
        auto symbols = download_symbols_from_file("LMAX-Instruments.csv");
        std::for_each(symbols.begin(), symbols.end(), [this](const marketlib::InstrumentPtr &i) {
            _channel_listener->onInstrument(_name, i);
        });
        _channel_listener->onEvent(_name, marketlib::channel_info::ct_symbol_downloaded,"");

    }
    catch(std::exception& ex) {
        _channel_listener->onEvent(_name, marketlib::channel_info::ci_channel_error,ex.what());
    }
    return FIX8::Session::handle_logon(seqnum, msg);
}

 bool LMAXPrice_session::handle_logout(const unsigned seqnum, const FIX8::Message *msg)
{
    _is_connected=false;
    _channel_listener->onEvent(_name, marketlib::channel_info::ci_logout,"");
    return FIX8::Session::handle_logout(seqnum, msg);
}

void LMAXPrice_session::modify_outbound(FIX8::Message *msg)
{
    if(_display_debug) {
        std::ostringstream stream;
        msg->print(stream);
        //_logger->LOG_DEBUG("---> %s\n",  stream.str().c_str());
    }
    return FIX8::Session::modify_outbound(msg);
}

 bool LMAXPrice_session::process(const FIX8::f8String &from)
{
    if(_display_debug) {
        if(from.find("35=0") == -1)
            _logger->LOG_DEBUG("<--- %s\n", from.c_str());
    }
    return FIX8::Session::process(from);
}









#pragma once 

#include "enums.h"
#include "structs.h"
// #include "instruments.h"
#include "sol_sdk/key.h"

#include <boost/json.hpp>
#include <boost/format.hpp>
#include <algorithm>
#include <string>
#include <map>

#include <marketlib/include/enums.h>
#include <marketlib/include/market.h>
#include <sharedlib/include/IPoolsRequester.h>
#include <sharedlib/include/HTTPClient.h>
#include <sharedlib/include/IMarket.h>




class SerumMarket : IMarket
{
private:
    typedef std::string string;
    typedef std::shared_ptr < IPoolsRequester > pools_ptr;
    typedef marketlib::order_t Order;
    typedef marketlib::instrument_descr_t Instrument;
    typedef std::function <void(const string&, const Instrument&, const string&)> Callback;

    struct MarketChannel
    {
        string base;
        string quote;
		Instrument instr;
        SolPubkey market_address;
        SolPubkey payer_sell;
        SolPubkey payer_buy;
        SolPubkey open_order_account;
        MarketLayout parsed_market;
    };

    using MarketChannels = boost::multi_index::multi_index_container<
        MarketChannel,
        boost::multi_index::indexed_by<
            boost::multi_index::hashed_unique<
                boost::multi_index::tag<struct MarketChannelsByPool>,
                boost::multi_index::composite_key<
                    MarketChannel,
                    boost::multi_index::member<MarketChannel, decltype(MarketChannel::base), &MarketChannel::base >,
					boost::multi_index::member<MarketChannel, decltype(MarketChannel::quote), &MarketChannel::quote >
                >
            >
        >
    >;
    // typedef std::shared_ptr < ILogger > logger_ptr;

    string pubkey_;
    SolPubkey decoded_pubkey_;
    string secretkey_;
    SolKeyPair decoded_secretkey_;
    string http_address_;
    pools_ptr pools_;
    Callback callback_;
    MarketChannels markets_info;
    std::map<string, string> mint_addresses_;

    
    void place_order(
        const MarketChannel&,
        OrderType,
        Side,
        double,
        double,
        uint64_t
    );
    void get_mint_addresses();
    MarketChannel create_market_info(const Instrument&);
    MarketLayout deserialize_market_info(const char*, size_t);
    string get_account_info(const string&);
    string get_token_account_by_owner(const string&, const string&);
    string get_token_program_account(const string&, const string&, const string&);
public:
    SerumMarket(const string&, const string&, const string&, pools_ptr, Callback);
    SerumMarket(const SerumMarket&) {};

    void send_new_order(const Instrument&, const Order&) override;
    void cancel_order(const Instrument&, const Order&) override;

    ~SerumMarket();
};
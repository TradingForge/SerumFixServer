#pragma once 

#include "enums.h"
#include "structs.h"
// #include "instruments.h"
#include "sol_sdk/pubkey.h"

#include <boost/json.hpp>
#include <boost/format.hpp>
#include <algorithm>
#include <string>
#include <map>

#include <marketlib/include/enums.h>
#include <marketlib/include/market.h>
#include <sharedlib/include/IPoolsRequester.h>
#include <sharedlib/include/HTTPClient.h>




class SerumMarket
{
private:
    typedef std::string string;
    typedef std::shared_ptr < IPoolsRequester > pools_ptr;
    typedef marketlib::order_t Order;
    typedef marketlib::instrument_descr_t Instrument;

    // typedef std::shared_ptr < ILogger > logger_ptr;

    string pubkey;
    string secretkey;
    string address;
    pools_ptr pools;
    std::map<string, string> mint_addresses;


    void place_order(
        const SolPubkey&,
        const SolKeyPair&,
        const SolPubkey&,
        OrderType,
        Side,
        double,
        double,
        uint64_t
    );

    void get_mint_addresses();
    string get_token_account_by_owner(const string& owner_pubkey, const string& token_address);
    string get_token_program_account(const string& market_key, const string& pool_key, const string& pubkey_owner);
public:
    SerumMarket(const string&, const string&, const string&, pools_ptr);
    ~SerumMarket();

    void send_new_order(const Instrument&, const Order&);
    // string get_account_info(const string& key);
};
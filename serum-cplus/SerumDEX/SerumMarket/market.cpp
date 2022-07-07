#include "market.h"
#include <boost/format.hpp>
#include <sharedlib/include/HTTPClient.h>
#include "instruments.h"
#include "enums.h"
#include <marketlib/include/enums.h>
#include <algorithm>

SerumMarket::SerumMarket(const string& pubkey_, const string& secretkey_, const string& address_, pools_ptr pools_) 
: pubkey(pubkey_), secretkey(secretkey_), address(address_), pools(pools_)
{
    get_mint_addresses();
    // std::cout << get_token_program_account(
    //     "9xQeWvG816bUx9EPjHmaT23yvVM2ZWbrrpZb9PusVFin", 
    //     "9wFFyRfZBsuAha4YcuxcXLKwMxJR43S7fPfQLusDBzvT", 
    //     "GKvwL3FmQRHuB9mcZ3WuqTuVjbGDzdW51ec8fYdeHae1"
    // ) << std::endl;
}
SerumMarket::~SerumMarket()
{
    mint_addresses.clear();
}


// // 'url': 'https://solana-api.projectserum.com', 
// // 'headers': {'Content-Type': 'application/json'}, 
// // 'data': '{"jsonrpc": "2.0", "id": 1, "method": "getAccountInfo", "params": ["9wFFyRfZBsuAha4YcuxcXLKwMxJR43S7fPfQLusDBzvT", {"encoding": "base64", "commitment": "finalized"}]}'}
// std::string Market::get_account_info(const string& key)
// {

// }

void SerumMarket::place_order(
    const SolPubkey& payer,
    const SolKeyPair& owner,
    const SolPubkey& open_order,
    OrderType order_type,
    Side side,
    double limit_price,
    double max_quantity,
    uint64_t client_id
){



}

void SerumMarket::send_new_order(const Instrument& instrument, const Order& order) 
{
    string payer;
    if (order.side == marketlib::order_side_t::os_Buy) {
        try {
            payer = get_token_account_by_owner(pubkey, instrument.quote_currency);
        }
        catch (std::exception e ) {

        }
    }
    else 
        payer = pubkey;

    auto pls = pools->getPools();
    auto t = std::find_if(pls.begin(), pls.end(), [&instrument](const Instrument& i){ 
            return instrument.base_currency == i.base_currency && 
                instrument.quote_currency == i.quote_currency;
        });

    // TODO place_order_open_order_account if open_order_account not exist
    string open_order_account = get_token_program_account(
        "9xQeWvG816bUx9EPjHmaT23yvVM2ZWbrrpZb9PusVFin",
        t->address,
        pubkey
    );

    place_order(
        base58str_to_pubkey(pubkey),
        base58str_to_keypair(secretkey),
        base58str_to_pubkey(open_order_account),
        OrderType::LIMIT,
        order.side == marketlib::order_side_t::os_Buy ? Side::BUY : Side::SELL,
        order.price,
        order.original_qty,
        // TODO Cli_id
        1234
    );
}

void SerumMarket::get_mint_addresses()
{
    string data_str;
    try{
        data_str = HttpClient::request(
            "", 
            "https://raw.githubusercontent.com/project-serum/serum-ts/master/packages/serum/src/token-mints.json", 
            HttpClient::HTTPMethod::GET
        );
    }
    catch(std::exception e) {

    }

    auto data = boost::json::parse(data_str).as_array();
    for(const auto& el : data) {
        mint_addresses[el.at("name").as_string().c_str()] = el.at("address").as_string().c_str();
    }
}

std::string SerumMarket::get_token_account_by_owner(const string& owner_pubkey, const string& token_address) 
{
    string data_str;
    try{
        data_str = HttpClient::request(
            (boost::format(R"({
                "jsonrpc": "2.0", 
                "id": 1, 
                "method": "getTokenAccountsByOwner", 
                "params": [
                    "%1%", 
                    {"mint": "%2%"}, 
                    {"commitment": "finalized", "encoding": "base64"}
                ]
            })") % owner_pubkey % token_address).str(), 
            address, 
            HttpClient::HTTPMethod::POST,
            std::vector<string>({"Content-Type: application/json"})
        );
    }
    catch(std::exception e) {

    }

    return boost::json::parse(data_str).at("result").at("value").as_array()[0].at("pubkey").as_string().c_str();
}


// '{"jsonrpc": "2.0", "id": 3, "method": "getProgramAccounts", "params": ["9xQeWvG816bUx9EPjHmaT23yvVM2ZWbrrpZb9PusVFin", {"filters": [{"memcmp": {"offset": 13, "bytes": "9wFFyRfZBsuAha4YcuxcXLKwMxJR43S7fPfQLusDBzvT"}}, {"memcmp": {"offset": 45, "bytes": "GKvwL3FmQRHuB9mcZ3WuqTuVjbGDzdW51ec8fYdeHae1"}}, {"dataSize": 3228}], "encoding": "base64", "commitment": "recent"}]}'

std::string SerumMarket::get_token_program_account(const string& market_key, const string& pool_key, const string& pubkey_owner)
{
    string data_str;
    try{
        data_str = HttpClient::request(
            (boost::format(R"({
                "jsonrpc": "2.0", 
                "id": 1, 
                "method": "getProgramAccounts", 
                "params": [
                    "%1%", 
                    {"filters": [
                        {"memcmp": {"offset": 13, "bytes": "%2%"}}, 
                        {"memcmp": {"offset": 45, "bytes": "%3%"}}, 
                        {"dataSize": 3228}
                    ], 
                    "encoding": "base64", 
                    "commitment": "recent"}
                ]
            })") % market_key % pool_key % pubkey_owner).str(), 
            address, 
            HttpClient::HTTPMethod::POST,
            std::vector<string>({"Content-Type: application/json"})
        );
    }
    catch(std::exception e) {

    }

    return boost::json::parse(data_str).at("result").as_array()[0].at("pubkey").as_string().c_str();
}
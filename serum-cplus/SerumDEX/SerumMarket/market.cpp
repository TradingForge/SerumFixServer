#include "market.h"
#include "instructions.h"

SerumMarket::SerumMarket(const string& pubkey, const string& secretkey, const string& http_address, pools_ptr pools, const Callback& callback) 
: pubkey_(pubkey), secretkey_(secretkey), http_address_(http_address), pools_(pools), callback_(callback),
  decoded_pubkey_(base58str_to_pubkey(pubkey)), decoded_secretkey_(base58str_to_keypair(secretkey))
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
    mint_addresses_.clear();
}


// // 'url': 'https://solana-api.projectserum.com', 
// // 'headers': {'Content-Type': 'application/json'}, 
// // 'data': '{"jsonrpc": "2.0", "id": 1, "method": "getAccountInfo", "params": ["9wFFyRfZBsuAha4YcuxcXLKwMxJR43S7fPfQLusDBzvT", {"encoding": "base64", "commitment": "finalized"}]}'}
// std::string Market::get_account_info(const string& key)
// {

// }

void SerumMarket::place_order(
    const MarketChannel& info,
    OrderType order_type,
    Side side,
    double limit_price,
    double max_quantity,
    uint64_t client_id
)
{
    auto order_instruction = NewOrderV3Params
    {
        market: info.market_address,
        open_orders: info.open_order_account,
        payer: side == Side::BUY ? info.payer_buy : info.payer_sell,
        owner: decoded_pubkey_,

    };


}

void SerumMarket::send_new_order(const Instrument& instrument, const Order& order) 
{
    auto market_info = markets_info.get<MarketChannelsByPool>()
		.find(boost::make_tuple(
			instrument.base_currency,
			instrument.quote_currency
		));

    if (market_info == markets_info.end()) {
        auto pls = pools_->getPools();
        auto pool = *std::find_if(pls.begin(), pls.end(), [&instrument](const Instrument& i){ 
                return instrument.base_currency == i.base_currency && 
                    instrument.quote_currency == i.quote_currency;
            });

        markets_info.insert(create_market_info(pool));
        market_info = markets_info.begin();
    }

    auto payer = order.side == marketlib::order_side_t::os_Buy ? market_info->payer_buy : market_info->payer_sell;

    place_order(
        *market_info,
        OrderType::LIMIT,
        order.side == marketlib::order_side_t::os_Buy ? Side::BUY : Side::SELL,
        order.price,
        order.original_qty,
        // TODO Cli_id
        1234
    );
}

SerumMarket::MarketChannel SerumMarket::create_market_info(const Instrument& instr)
{
    auto payer = get_token_account_by_owner(pubkey_, instr.quote_mint_address);

    // TODO place_order_open_order_account if open_order_account not exist
    auto open_order_account = get_token_program_account(
        "9xQeWvG816bUx9EPjHmaT23yvVM2ZWbrrpZb9PusVFin",
        instr.address,
        pubkey_
    );

    return MarketChannel {
        base: instr.base_currency,
        quote: instr.quote_currency,
        instr: instr,
        market_address: base58str_to_pubkey(instr.address),
        payer_sell: base58str_to_pubkey(pubkey_),
        payer_buy: base58str_to_pubkey(payer),
        open_order_account: base58str_to_pubkey(open_order_account)
    };
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
        mint_addresses_[el.at("name").as_string().c_str()] = el.at("address").as_string().c_str();
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
            http_address_, 
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
            http_address_, 
            HttpClient::HTTPMethod::POST,
            std::vector<string>({"Content-Type: application/json"})
        );
    }
    catch(std::exception e) {

    }

    return boost::json::parse(data_str).at("result").as_array()[0].at("pubkey").as_string().c_str();
}
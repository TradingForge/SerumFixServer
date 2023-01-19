#include "Market.hpp"
#include <base64/base64.h>
#include <iostream>
#include <cmath>
#include <algorithm> 
#include "sol_sdk/Transaction.hpp"
#include "sol_sdk/enums.hpp"
#include "sysvar.hpp"
#include <random>
// #include <nacl/crypto_stream.h>

#define LAMPORTS_PER_SOL 1000000000

using namespace solana; 

SerumMarket::SerumMarket(const string& pubkey, const string& secretkey, const string& http_address, pools_ptr pools, Callback callback, OrdersCallback orders_callback) 
: pubkey_(pubkey), secretkey_(secretkey), http_address_(http_address), pools_(pools), callback_(callback), message_count(1), orders_callback_(orders_callback)
{
    get_mint_addresses();
}

SerumMarket::~SerumMarket()
{
    mint_addresses_.clear();
}

SerumMarket::Order SerumMarket::send_new_order(const Instrument& instrument, const Order& order_) 
{
    MarketChannel market_info;
    OpenOrdersAccountInfo orders_account_info;
    Transaction txn;
    Transaction::Signers signers;
    signers.push_back(secretkey_);
    try {
        market_info = get_market_info(instrument);
        orders_account_info = get_orders_account_info(instrument);

        if (order_.side == marketlib::order_side_t::os_Buy && market_info.payer_buy.get_str_key().empty()) {
            auto payer_buy = get_token_account_by_owner(pubkey_.get_str_key(), market_info.instr.quote_mint_address);
            if (!payer_buy.empty()) {
                market_info.payer_buy = payer_buy;
            }
            else {
                auto balance_needed = get_balance_needed();
                Keypair payer_buy;
                market_info.payer_buy = payer_buy.get_pubkey();
                signers.push_back(payer_buy);
                txn.add_instruction(
                    create_account(
                        CreateAccountParams{
                            owner: pubkey_,
                            new_account: payer_buy.get_pubkey(),
                            lamports: balance_needed,
                            program_id: PublicKey("11111111111111111111111111111111")
                        }
                    )
                );
            }
        }
    }
    catch (string e) {
        callback_(Name, instrument, "Failed to get information: " + e);
        return order_;
    }

    auto order = std::make_shared<Order>(order_);
    if (order->clId == 0) {
        std::random_device rd; 
        std::mt19937_64 mersenne(rd());
        order->clId = mersenne();
    };
    // auto payer = order.side == marketlib::order_side_t::os_Buy ? market_info->payer_buy : market_info->payer_sell;

    try{
        auto res = place_order(
            market_info,
            orders_account_info,
            OrderType::LIMIT,
            order->side == marketlib::order_side_t::os_Buy ? Side::BUY : Side::SELL,
            order->price,
            order->original_qty,
            order->clId,
            txn,
            signers
        );
        callback_(Name, instrument, "The order is sent: " + res);
        order->transaction_hash = res;
    }
    catch (string e) {
        callback_(Name, instrument, "Failed to send the order: " + e);
        return order_;
    }

    order->state = marketlib::order_state_t::ost_Undefined;
    order->init_time = current_time();
    open_orders_.insert(order);
    return *order;
}

SerumMarket::Order SerumMarket::cancel_order(const Instrument& instrument, const Order& order_)
{
    MarketChannel market_info;
    OpenOrdersAccountInfo orders_account_info;
    try {
        market_info = get_market_info(instrument);
        orders_account_info = get_orders_account_info(instrument);        
    }
    catch (string e) {
        callback_(Name, instrument, "Failed to get information: " + e);
        return order_;
    }
    
    Transaction txn;
    txn.add_instruction(
        new_cancel_order_by_client_id_v2(
            CancelOrderV2ByClientIdParams {
                market: market_info.market_address,
                bids: market_info.parsed_market.bids,
                asks: market_info.parsed_market.asks,
                event_queue: market_info.parsed_market.event_queue,
                open_orders: orders_account_info.account,
                owner: pubkey_,
                client_id: order_.clId,
                program_id: MARKET_KEY
            }
        )
    );
    Transaction::Signers signers;
    signers.push_back(secretkey_);
    try{
        auto res = send_transaction(txn, signers);
        callback_(Name, instrument, "The order is sent: " + res);
    }
    catch (string e)
    {
        callback_(Name, instrument, "Failed to send the order: " + e);
        return order_;
    }
    
    auto order = order_;
    order.state = marketlib::order_state_t::ost_Canceled;
    return order;
}

// // 'url': 'https://solana-api.projectserum.com', 
// // 'headers': {'Content-Type': 'application/json'}, 
// // 'data': '{"jsonrpc": "2.0", "id": 1, "method": "getAccountInfo", "params": ["9wFFyRfZBsuAha4YcuxcXLKwMxJR43S7fPfQLusDBzvT", {"encoding": "base64", "commitment": "finalized"}]}'}

SerumMarket::string SerumMarket::place_order(
    const MarketChannel& info,
    const OpenOrdersAccountInfo& orders_account_info,
    OrderType order_type,
    Side side,
    double limit_price,
    double max_quantity,
    uint64_t client_id,
    Transaction& txn,
    Transaction::Signers& signers
)
{
    bool should_wrap_sol = (side == Side::BUY && info.instr.quote_mint_address == WRAPPED_SOL_MINT) || 
    (side == Side::SELL && info.instr.base_mint_address == WRAPPED_SOL_MINT);

    Keypair wrapped_sol_account;
    PublicKey payer = side == Side::BUY ? info.payer_buy : info.payer_sell;
    if (should_wrap_sol)
    {
        payer = wrapped_sol_account.get_pubkey();
        signers.push_back(wrapped_sol_account);
        txn.add_instruction(
            create_account(
                CreateAccountParams{
                    owner: pubkey_,
                    new_account: payer,
                    lamports: get_lamport_need_for_sol_wrapping(
                        limit_price, max_quantity, side, orders_account_info
                    ),
                    program_id: PublicKey("11111111111111111111111111111111")
                }
            )
        );
        txn.add_instruction(
            initialize_account(
                InitializeAccountParams{
                    account: wrapped_sol_account.get_pubkey(),
                    mint: WRAPPED_SOL_MINT,
                    owner: pubkey_,
                    program_id: TOKEN_PROGRAM_ID
                }
            )
        );
    }
    
    txn.add_instruction(
        new_order_v3(
            NewOrderV3Params{
                market: info.market_address,
                open_orders: orders_account_info.account,
                payer: payer,
                owner: pubkey_,
                request_queue: info.parsed_market.request_queue,
                event_queue: info.parsed_market.event_queue,
                bids: info.parsed_market.bids,
                asks: info.parsed_market.asks,
                base_vault: info.parsed_market.base_vault,
                quote_vault: info.parsed_market.quote_vault,
                side: side,
                limit_price: price_number_to_lots(limit_price, info),
                max_base_quantity: base_size_number_to_lots(max_quantity, info), 
                max_quote_quantity: base_size_number_to_lots(max_quantity, info) 
                * info.parsed_market.quote_lot_size
                * price_number_to_lots(limit_price, info),
                order_type: order_type,
                self_trade_behavior: SelfTradeBehavior::DECREMENT_TAKE,
                limit: 65535,
                client_id: client_id,
                program_id: MARKET_KEY
            }
        )
    );
    
    // signers.push_back(secretkey_);
    if(should_wrap_sol) {
        txn.add_instruction(
            close_account(
                CloseAccountParams {
                    account: wrapped_sol_account.get_pubkey(),
                    owner: pubkey_,
                    dest: pubkey_,
                    program_id: TOKEN_PROGRAM_ID
                }
            )
        );
    }
    
    return send_transaction(txn, signers);
}

const SerumMarket::MarketChannel& SerumMarket::get_market_info(const Instrument& instrument)
{
    auto market_info = markets_info_.get<MarketChannelsByPool>()
		.find(boost::make_tuple(
			instrument.base_currency,
			instrument.quote_currency
		));

    if (market_info == markets_info_.end()) {
        // auto pls = pools_->getPools();
        auto pool = pools_->getPool(instrument);
        // auto pool = *std::find_if(pls.begin(), pls.end(), [&instrument](const Instrument& i){ 
        //         return instrument.base_currency == i.base_currency && 
        //             instrument.quote_currency == i.quote_currency;
        //     });

        markets_info_.insert(create_market_info(pool));
        market_info = markets_info_.begin();
    }

    return *market_info;
}

// TODO
SerumMarket::MarketChannel SerumMarket::create_market_info(const Instrument& instr)
{
    return MarketChannel {
        base: instr.base_currency,
        quote: instr.quote_currency,
        instr: instr,
        market_address: instr.address,
        parsed_market: get_market_layout(instr.address),
        base_spl_token_multiplier: static_cast<uint64_t>(pow(10, get_mint_decimals(instr.base_mint_address))),
        quote_spl_token_multiplier: static_cast<uint64_t>(pow(10, get_mint_decimals(instr.quote_mint_address))),
        payer_sell: pubkey_
    };
}

SerumMarket::MarketLayout SerumMarket::get_market_layout(const string& market_address)
{
    string account_data = base64_decode(
        string(boost::json::parse(get_account_info(market_address)).at("result").at("value").at("data").as_array()[0].as_string().c_str())
    );

    SolMarketLayout market; 
    memcpy((void*)&market, account_data.data(), account_data.size());

    return MarketLayout {
        request_queue: PublicKey(market.request_queue),
        event_queue: PublicKey(market.event_queue),
        bids: PublicKey(market.bids),
        asks: PublicKey(market.asks),
        base_vault: PublicKey(market.base_vault),
        quote_vault: PublicKey(market.quote_vault),
        base_lot_size: market.base_lot_size,
        quote_lot_size: market.quote_lot_size
    };
}

uint64_t SerumMarket::get_balance_needed()
{
    return boost::json::parse(get_minimum_balance_for_rent_exemption())
    .at("result")
    .as_int64();;
}

OpenOrdersAccountInfo SerumMarket::get_orders_account_info(const Instrument& instr)
{
    auto orders_accounts_info = get_token_program_accounts(
        MARKET_KEY.get_str_key(),
        instr.address,
        pubkey_.get_str_key()
    );

    if (orders_accounts_info.empty() || boost::json::parse(orders_accounts_info).at("result").as_array().empty())
        throw -1;

    auto decoded = base64_decode(string(boost::json::parse(orders_accounts_info)
        .at("result")
        .as_array()[0]
        .at("account")
        .at("data")
        .as_array()[0].as_string().c_str()));

    auto open_order_layout = SolOpenOrderLayout{};
    memcpy(&open_order_layout, decoded.data(), sizeof(SolOpenOrderLayout));
    //boost::json::parse(data_str).at("result").as_array()[0].at("pubkey").as_string().c_str();

    return OpenOrdersAccountInfo {
        account: PublicKey(boost::json::parse(orders_accounts_info).at("result").as_array()[0].at("pubkey").as_string().c_str()),
        base_token_free: open_order_layout.base_token_free,
        quote_token_free: open_order_layout.quote_token_free
    };
}

uint8_t SerumMarket::get_mint_decimals(const string& mint_address)
{
    if (mint_address == WRAPPED_SOL_MINT) 
        return 9;

    return (uint8_t)boost::json::parse(get_account_info(mint_address))
        .at("result")
        .at("value")
        .at("data")
        .at("parsed")
        .at("info")
        .at("decimals").as_int64();
}

Instruction SerumMarket::new_cancel_order_by_client_id_v2(const CancelOrderV2ByClientIdParams& params) const
{
    Instruction instruction;
    instruction.set_account_id(params.program_id);
    instruction.set_accounts( Instruction::AccountMetas({
        Instruction::AccountMeta { pubkey: params.market, is_writable: false, is_signer: false },
        Instruction::AccountMeta { pubkey: params.bids, is_writable: true, is_signer: false },
        Instruction::AccountMeta { pubkey: params.asks, is_writable: true, is_signer: false },
        Instruction::AccountMeta { pubkey: params.open_orders, is_writable: true, is_signer: false },
        Instruction::AccountMeta { params.owner, is_writable: true, is_signer: true },
        Instruction::AccountMeta { params.event_queue, is_writable: true, is_signer: false }
    }));
    auto ord_layout = InstructionLayoutCancelOrderByClientIdV2 {
        0,
        ::InstructionType::CANCEL_ORDER_BY_CLIENT_ID_V2,
        CancelOrderByClientIdV2{
            order_id: params.client_id
        }
    };
    instruction.set_data(&ord_layout, sizeof(InstructionLayoutCancelOrderByClientIdV2));
    return instruction;
}

Instruction SerumMarket::new_order_v3(const NewOrderV3Params& params) const
{
    Instruction instruction;
    instruction.set_account_id(params.program_id);
    instruction.set_accounts( Instruction::AccountMetas({
        Instruction::AccountMeta { pubkey: params.market, is_writable: true, is_signer: false },
        Instruction::AccountMeta { pubkey: params.open_orders, is_writable: true, is_signer: false },
        Instruction::AccountMeta { pubkey: params.request_queue, is_writable: true, is_signer: false },
        Instruction::AccountMeta { pubkey: params.event_queue, is_writable: true, is_signer: false },
        Instruction::AccountMeta { pubkey: params.bids, is_writable: true, is_signer: false },
        Instruction::AccountMeta { pubkey: params.asks, is_writable: true, is_signer: false },
        Instruction::AccountMeta { pubkey: params.payer, is_writable: true, is_signer: false },
        Instruction::AccountMeta { pubkey: params.owner, is_writable: true, is_signer: true },
        Instruction::AccountMeta { pubkey: params.base_vault, is_writable: true, is_signer: false },
        Instruction::AccountMeta { pubkey: params.quote_vault, is_writable: true, is_signer: false },
        Instruction::AccountMeta { pubkey: TOKEN_PROGRAM_ID, is_writable: false, is_signer: false },
        Instruction::AccountMeta { pubkey: SYSVAR_RENT_PUBKEY, is_writable: false, is_signer: false }
    }));

    auto ord_layout = InstructionLayoutOrderV3 {
        0,
        10,
        NewOrderV3 {
            side: params.side,
            limit_price: params.limit_price,
            max_base_quantity: params.max_base_quantity,
            max_quote_quantity: params.max_quote_quantity,
            self_trade_behavior: params.self_trade_behavior,
            order_type: params.order_type,
            client_id: params.client_id,
            limit: 65535
        }
    };
    instruction.set_data(&ord_layout, sizeof(InstructionLayoutOrderV3));
    return instruction;
}

Instruction SerumMarket::create_account(const CreateAccountParams& params) const
{
    Instruction instruction;
    instruction.set_account_id(params.program_id);
    instruction.set_accounts( Instruction::AccountMetas({
        Instruction::AccountMeta { pubkey: params.owner, is_writable: true, is_signer: true },
        Instruction::AccountMeta { pubkey: params.new_account, is_writable: true, is_signer: true }
    }));

    auto ord_layout = InstructionLayoutCreateOrder {
        0,
        params.lamports,
        ACCOUNT_LEN
    };
    memcpy(ord_layout.owner, TOKEN_PROGRAM_ID.data(), SIZE_PUBKEY);
    instruction.set_data(&ord_layout, sizeof(InstructionLayoutCreateOrder));
    return instruction;
}

Instruction SerumMarket::initialize_account(const InitializeAccountParams& params) const
{
    Instruction instruction;
    instruction.set_account_id(params.program_id);
    instruction.set_accounts( Instruction::AccountMetas({
        Instruction::AccountMeta { pubkey: params.account, is_writable: true, is_signer: true },
        Instruction::AccountMeta { pubkey: params.mint, is_writable: false, is_signer: false },
        Instruction::AccountMeta { pubkey: params.owner, is_writable: true, is_signer: true },
        Instruction::AccountMeta { pubkey: SYSVAR_RENT_PUBKEY, is_writable: false, is_signer: false }
    }));
    auto data = Instruction::bytes();
    data.push_back((uint8_t)solana::InstructionType::INITIALIZE_ACCOUNT);
    instruction.set_data(data);
    return instruction;
}

Instruction SerumMarket::close_account(const CloseAccountParams& params) const
{
    Instruction instruction;
    instruction.set_account_id(params.program_id);
    instruction.set_accounts( Instruction::AccountMetas({
        Instruction::AccountMeta { pubkey: params.account, is_writable: true, is_signer: true },
        Instruction::AccountMeta { pubkey: params.owner, is_writable: true, is_signer: true },
        Instruction::AccountMeta { pubkey: params.dest, is_writable: true, is_signer: true },
    }));
    auto data = Instruction::bytes();
    data.push_back((uint8_t)solana::InstructionType::CLOSE_ACCOUNT);
    instruction.set_data(data);
    return instruction;
}

void SerumMarket::get_mint_addresses()
{
    string data_str;
    string address = "https://raw.githubusercontent.com/project-serum/serum-ts/master/packages/serum/src/token-mints.json";
    try{
        data_str = HttpClient::request(
            "", 
            address, 
            HttpClient::HTTPMethod::GET
        );
    }
    catch(std::exception e) {
        throw string("Failed to make a request to " + address);
    }

    auto data = boost::json::parse(data_str).as_array();
    for(const auto& el : data) {
        mint_addresses_[el.at("name").as_string().c_str()] = el.at("address").as_string().c_str();
    }
}

std::string SerumMarket::get_latest_blockhash()
{
    string data_str;
    try{
        data_str = HttpClient::request(
            (boost::format(R"({
                "jsonrpc": "2.0", 
                "id": "%1%", 
                "method": "getLatestBlockhash", 
                "params": [{"commitment": "finalized"}]
            })") % message_count++).str(), 
            http_address_, 
            HttpClient::HTTPMethod::POST,
            std::vector<string>({"Content-Type: application/json"})
        );
    }
    catch(std::exception e) {
        throw string("Failed to make a request to " + http_address_);
    }
    if (data_str.find("error") != std::string::npos) {
        throw data_str;
    }
    return boost::json::parse(data_str).at("result").at("value").at("blockhash").as_string().c_str();
}

std::string SerumMarket::get_token_account_by_owner(const string& owner_pubkey, const string& token_address) 
{
    string data_str;
    try{
        data_str = HttpClient::request(
            (boost::format(R"({
                "jsonrpc": "2.0", 
                "id": "%1%", 
                "method": "getTokenAccountsByOwner", 
                "params": [
                    "%2%", 
                    {"mint": "%3%"}, 
                    {"commitment": "finalized", "encoding": "base64"}
                ]
            })") % message_count++ % owner_pubkey % token_address).str(), 
            http_address_, 
            HttpClient::HTTPMethod::POST,
            std::vector<string>({"Content-Type: application/json"})
        );
    }
    catch(std::exception e) {
        throw string("Failed to make a request to " + http_address_);
    }
    if (data_str.find("error") != std::string::npos) {
        throw data_str;
    }
    if (boost::json::parse(data_str).at("result").at("value").as_array().empty()) {
        return "";
    }

    try {
        return boost::json::parse(data_str).at("result").at("value").as_array()[0].at("pubkey").as_string().c_str();
    }
    catch (std::exception e){
        throw string("Failed to retrieve the data");
    }
}

SerumMarket::string SerumMarket::get_minimum_balance_for_rent_exemption()
{
    string data_str;
    try{
        data_str = HttpClient::request(
            (boost::format(R"({
                "jsonrpc": "2.0", 
                "id": "%1%", 
                "method": 
                "getMinimumBalanceForRentExemption", 
                "params": [%2%, {"commitment": "finalized"}]
            })") % message_count++ % sizeof(SolOpenOrderLayout)).str(), 
            http_address_, 
            HttpClient::HTTPMethod::POST,
            std::vector<string>({"Content-Type: application/json"})
        );
    }
    catch(std::exception e) {
        throw string("Failed to make a request to " + http_address_);
    }
    if (data_str.find("error") != std::string::npos) {
        throw data_str;
    }
    return data_str;
}

// '{"jsonrpc": "2.0", "id": 3, "method": "getProgramAccounts", "params": ["9xQeWvG816bUx9EPjHmaT23yvVM2ZWbrrpZb9PusVFin", {"filters": [{"memcmp": {"offset": 13, "bytes": "9wFFyRfZBsuAha4YcuxcXLKwMxJR43S7fPfQLusDBzvT"}}, {"memcmp": {"offset": 45, "bytes": "GKvwL3FmQRHuB9mcZ3WuqTuVjbGDzdW51ec8fYdeHae1"}}, {"dataSize": 3228}], "encoding": "base64", "commitment": "recent"}]}'
SerumMarket::string SerumMarket::get_token_program_accounts(const string& market_key, const string& pool_key, const string& pubkey_owner)
{
    string data_str;
    try{
        data_str = HttpClient::request(
            (boost::format(R"({
                "jsonrpc": "2.0", 
                "id": "%1%", 
                "method": "getProgramAccounts", 
                "params": [
                    "%2%", 
                    {"filters": [
                        {"memcmp": {"offset": 13, "bytes": "%3%"}}, 
                        {"memcmp": {"offset": 45, "bytes": "%4%"}}, 
                        {"dataSize": 3228}
                    ], 
                    "encoding": "base64", 
                    "commitment": "recent"}
                ]
            })") % message_count++ % market_key % pool_key % pubkey_owner).str(), 
            http_address_, 
            HttpClient::HTTPMethod::POST,
            std::vector<string>({"Content-Type: application/json"})
        );
    }
    catch(std::exception e) {
        throw string("Failed to make a request to " + http_address_);
    }
    if (data_str.find("error") != std::string::npos) {
        throw data_str;
    }
    return data_str ;
}

//'{"jsonrpc": "2.0", "id": 1, "method": "getAccountInfo", "params": ["9wFFyRfZBsuAha4YcuxcXLKwMxJR43S7fPfQLusDBzvT", {"encoding": "base64", "commitment": "finalized"}]}'
std::string SerumMarket::get_account_info(const string& account)
{
    string data_str;
    try{
        data_str = HttpClient::request(
            (boost::format(R"({
                "jsonrpc": "2.0", 
                "id": "%1%", 
                "method": "getAccountInfo", 
                "params": [
                    "%2%", 
                    {"encoding": "jsonParsed", "commitment": "finalized"}
                ]
            })") % message_count++ % account).str(), 
            http_address_, 
            HttpClient::HTTPMethod::POST,
            std::vector<string>({"Content-Type: application/json"})
        );
    }
    catch(std::exception e) {
        throw string("Failed to make a request to " + http_address_);
    }
    if (data_str.find("error") != std::string::npos) {
        throw data_str;
    }
    return data_str;
}

std::string SerumMarket::send_transaction(Transaction &txn, const Transaction::Signers &signers)
{
    txn.set_recent_blockhash(get_latest_blockhash());
    txn.sign(signers);
    auto msg = txn.serialize();

    // auto hex_msg = to_hex_string(msg);
    // std::cout << hex_msg << std::endl;

    auto decode_msg = base64_encode(msg);
    string data_str;
    try{
        data_str = HttpClient::request(
            (boost::format(R"({
                "jsonrpc": "2.0", 
                "id": "%1%", 
                "method": "sendTransaction", 
                "params": [
                    "%2%", 
                    {
                        "skipPreflight": false, 
                        "preflightCommitment": "finalized", 
                        "encoding": "base64"
                    }
                ]
            })") % message_count++ % decode_msg).str(), 
            http_address_, 
            HttpClient::HTTPMethod::POST,
            std::vector<string>({"Content-Type: application/json"})
        );
    }
    catch(std::exception e) {
        throw string("Failed to make a request to " + http_address_);
    }
    if (data_str.find("error") != std::string::npos) {
        throw data_str;
    }
    return data_str;
}

void SerumMarket::order_checker(const string& exch_name, const string& cli_id, const ExecutionReport& exec_report)
{
    auto order = open_orders_.get<OrderByCliId>()
        .find(strtoull(exec_report.clId.c_str(), nullptr, 0));

    if (order == open_orders_.end()) 
        return;

    open_orders_.modify(order, change_order_status(exec_report.state));
    open_orders_.modify(order, change_order_remaining_qty(exec_report.leavesQty));
    orders_callback_(Name, *order);

    if ((*order)->isCompleted()) {
        open_orders_.erase(order);
        return;
    }
}

uint64_t SerumMarket::price_number_to_lots(long double price, const MarketChannel& info) const
{
    return static_cast<uint64_t>(
        (price * info.quote_spl_token_multiplier * info.parsed_market.base_lot_size) 
        / (info.base_spl_token_multiplier * info.parsed_market.quote_lot_size)
    ); 
}

uint64_t SerumMarket::base_size_number_to_lots(long double size, const MarketChannel& info) const
{
    return static_cast<uint64_t>(
        std::floor(size * info.base_spl_token_multiplier) / info.parsed_market.base_lot_size
    );
}

uint64_t SerumMarket::get_lamport_need_for_sol_wrapping(double limit_price, double max_quantity, Side side, const OpenOrdersAccountInfo& orders_account_info) const
{
    uint64_t lamports = 0;

    if (side == Side::BUY) {
        lamports = static_cast<uint64_t>(limit_price * max_quantity * 1.01 * LAMPORTS_PER_SOL);
        lamports -= orders_account_info.quote_token_free;
    }
    else {
        lamports = static_cast<uint64_t>(max_quantity * LAMPORTS_PER_SOL);
        lamports -= orders_account_info.base_token_free;
    }

    return std::max(lamports, (uint64_t)0) + 10000000;
}
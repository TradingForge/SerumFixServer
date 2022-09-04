#pragma once 

#include "enums.hpp"
#include "structs.hpp"
#include "constants.hpp"

#include "sol_sdk/PublicKey.hpp"
#include "sol_sdk/Keypair.hpp"
#include "sol_sdk/Transaction.hpp"

#include <boost/json.hpp>
#include <boost/format.hpp>
#include <algorithm>
#include <functional>
#include <string>
#include <map>

#include <marketlib/include/enums.h>
#include <marketlib/include/market.h>
#include <sharedlib/include/IPoolsRequester.h>
#include <sharedlib/include/HTTPClient.h>
#include <sharedlib/include/IMarket.h>

#define MARKET_KEY PublicKey("9xQeWvG816bUx9EPjHmaT23yvVM2ZWbrrpZb9PusVFin")

class SerumMarket : IMarket
{
private:
    typedef std::string string;
    typedef std::shared_ptr < IPoolsRequester > pools_ptr;
    typedef marketlib::order_t Order;
    typedef marketlib::instrument_descr_t Instrument;
    typedef std::function <void(const string&, const Instrument&, const string&)> Callback;
    typedef solana::PublicKey PublicKey;

    struct MarketLayout
    {
        PublicKey request_queue;
        PublicKey event_queue;
        PublicKey bids;
        PublicKey asks;
        PublicKey base_vault;
        PublicKey quote_vault;
        uint64_t base_lot_size;
        uint64_t quote_lot_size;
    };

    struct MarketChannel
    {
        string base;
        string quote;
		Instrument instr;
        PublicKey market_address;
        PublicKey payer_sell;
        PublicKey payer_buy;
        // PublicKey open_order_account;
        MarketLayout parsed_market;
        uint64_t base_spl_token_multiplier;
        uint64_t quote_spl_token_multiplier;
    };

    using MarketChannels = boost::multi_index::multi_index_container<
        MarketChannel*,
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

    PublicKey pubkey_;
    Keypair secretkey_;
    string http_address_;
    pools_ptr pools_;
    Callback callback_;
    MarketChannels markets_info;
    std::map<string, string> mint_addresses_;

    
    void place_order(
        const MarketChannel&,
        const OpenOrdersAccountInfo& orders_account_info,
        OrderType,
        Side,
        double,
        double,
        uint64_t
    );

    void get_mint_addresses();
    MarketChannel* create_market_info(const Instrument&);
    MarketChannel* get_market_info(const Instrument&);
    MarketLayout get_market_layout(const string&);
    string get_account_info(const string&);
    string get_latest_blockhash();
    string get_token_account_by_owner(const string&, const string&);
    string get_token_program_accounts(const string&, const string&, const string&);
    string send_transaction(Transaction& transaction, const Transaction::Signers&);
    uint8_t get_mint_decimals(const string&);
    OpenOrdersAccountInfo get_orders_account_info(const Instrument& instruction);

    Instruction new_order_v3(const NewOrderV3Params&);
    Instruction new_cancel_order_by_client_id_v2(const CancelOrderV2ByClientIdParams&);
    Instruction create_account(const CreateAccountParams&);
    Instruction initialize_account(const InitializeAccountParams&);
    Instruction close_account(const CloseAccountParams&);

    // precision
    uint64_t price_number_to_lots(long double price, const MarketChannel& info);
    uint64_t base_size_number_to_lots(long double price, const MarketChannel& info);
    uint64_t get_lamport_need_for_sol_wrapping(double, double, Side, const OpenOrdersAccountInfo&);
public:
    SerumMarket(const string&, const string&, const string&, pools_ptr, Callback);
    SerumMarket(const SerumMarket&) {};

    void send_new_order(const Instrument&, const Order&) override;
    void cancel_order(const Instrument&, const Order&) override;

    ~SerumMarket();
};
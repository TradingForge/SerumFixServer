#pragma once 

#include "enums.hpp"
#include "models.hpp"
#include "constants.hpp"

#include "sol_sdk/PublicKey.hpp"
#include "sol_sdk/Keypair.hpp"
#include "sol_sdk/Transaction.hpp"

#include <SerumDEX/SerumTrade.h>

#include <boost/json.hpp>
#include <boost/format.hpp>
#include <algorithm>
#include <functional>
#include <string>
#include <map>
#include <ctime>

#include <marketlib/include/enums.h>
#include <marketlib/include/market.h>
#include <sharedlib/include/IPoolsRequester.h>
#include <sharedlib/include/HTTPClient.h>
#include <sharedlib/include/IMarket.h>

#include <boost/lexical_cast.hpp>

#define MARKET_KEY PublicKey("9xQeWvG816bUx9EPjHmaT23yvVM2ZWbrrpZb9PusVFin")

class SerumMarket : IMarket
{
private:
    typedef std::string string;
    typedef std::shared_ptr < IPoolsRequester > pools_ptr;
    typedef std::shared_ptr < IListener > Listener;
    typedef marketlib::order_t Order;
    typedef std::shared_ptr < Order > Order_ptr;
    typedef marketlib::execution_report_t ExecutionReport;
    typedef marketlib::instrument_descr_t Instrument;
    typedef std::function <void(const string&, const Instrument&, const string&)> Callback;
    typedef std::function <void(const string&, Order_ptr)> OrdersCallback;
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
        // PublicKey open_order_account;
        MarketLayout parsed_market;
        uint64_t base_spl_token_multiplier;
        uint64_t quote_spl_token_multiplier;
        PublicKey payer_sell;
        PublicKey payer_buy;
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

    using Orders = boost::multi_index::multi_index_container<
        Order_ptr ,
        boost::multi_index::indexed_by<
            boost::multi_index::hashed_unique<
                boost::multi_index::tag<struct OrderByCliId>,
                boost::multi_index::member<Order, decltype(Order::clId), &Order::clId >
            >
        >
    >;

    struct change_order_status
    {
        change_order_status(const marketlib::order_state_t& new_state):new_state_(new_state){}
        void operator()(Order_ptr o)
        {
            o->state=new_state_;
        }

    private:
        marketlib::order_state_t new_state_;
    };

    struct change_order_remaining_qty
    {
        change_order_remaining_qty(double new_qty):new_qty_(new_qty){}
        void operator()(Order_ptr o)
        {
            o->remaining_qty=new_qty_;
        }

    private:
        double new_qty_;
    };

    PublicKey pubkey_;
    Keypair secretkey_;
    string http_address_;
    pools_ptr pools_;
    Orders open_orders_;
    Callback callback_;
    OrdersCallback orders_callback_;
    Listener trade_channel_;
    MarketChannels markets_info_;
    // pair - count
    std::map<string, uint64_t> subscribed_channels_;
    std::map<string, string> mint_addresses_;
    uint64_t message_count;
    string Name = "SerumMarket";
    
    string place_order(
        const MarketChannel&,
        const OpenOrdersAccountInfo& orders_account_info,
        OrderType,
        Side,
        double,
        double,
        uint64_t,
        Transaction&,
        Transaction::Signers&
    );

    void get_mint_addresses();
    MarketChannel create_market_info(const Instrument&);
    const MarketChannel& get_market_info(const Instrument&);
    MarketLayout get_market_layout(const string&);
    string get_account_info(const string&);
    string get_latest_blockhash();
    string get_minimum_balance_for_rent_exemption();
    string get_token_account_by_owner(const string&, const string&);
    string get_token_program_accounts(const string&, const string&, const string&);
    string send_transaction(Transaction& transaction, const Transaction::Signers&);
    uint8_t get_mint_decimals(const string&);
    OpenOrdersAccountInfo get_orders_account_info(const Instrument& instruction);
    uint64_t get_balance_needed();

    Instruction new_order_v3(const NewOrderV3Params&) const;
    Instruction new_cancel_order_by_client_id_v2(const CancelOrderV2ByClientIdParams&) const;
    Instruction create_account(const CreateAccountParams&) const;
    Instruction initialize_account(const InitializeAccountParams&) const;
    Instruction close_account(const CloseAccountParams&) const;

    // precision
    uint64_t price_number_to_lots(long double price, const MarketChannel& info) const;
    uint64_t base_size_number_to_lots(long double price, const MarketChannel& info) const;
    uint64_t get_lamport_need_for_sol_wrapping(double, double, Side, const OpenOrdersAccountInfo&) const;

    time_t current_time() const { return std::time(nullptr);};

    void order_checker(const string&, const string&, const ExecutionReport&);
public:
    SerumMarket(const string&, const string&, const string&, pools_ptr, Callback, OrdersCallback);
    // SerumMarket(const SerumMarket&) {};

    Order send_new_order(const Instrument&, const Order&) override;
    Order cancel_order(const Instrument&, const Order&) override;

    ~SerumMarket();
};
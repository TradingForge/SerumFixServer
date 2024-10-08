#pragma once

#include <string>
#include <memory>
#include <chrono>

#include "enums.h"

namespace marketlib
{
    struct market_data_request_t
    {
        const std::string   engine;
        const std::string   symbol;
        const market_depth_t depth;/* top,full*/
        const subscription_update_type_t update_type;/*full, incremental*/
    };

    /*
    * @class instrument_descr_
    * @brief instrument_descr_t represent fields of pair
    */
    struct instrument_descr_t
    {
        std::string     engine;
        std::string     sec_id;
        std::string     symbol;
        std::string     base_currency;
        std::string     quote_currency;
        std::string     address;
        std::string     program_id;
        std::string     base_mint_address;
        std::string     quote_mint_address;
        int             tick_precision;
        double          min_order_size;
        bool            deprecated;
        const std::string& hash () const		{ return  sec_id;}
    };

    using InstrumentPtr = std::shared_ptr<instrument_descr_t>;
    /*
    * @class md_snapshot_t
    * @brief md_snapshot_t represent top of book update
    */
    struct md_snapshot_t
    {
        std::chrono::time_point<std::chrono::system_clock> time;
        double	 bid;
        double   ask;
        double   trade;
        unsigned bid_size;
        unsigned ask_size;
        unsigned trade_size;
        int      pad[7]={};
        md_snapshot_t ():
                time(),
                bid(-1.0),
                ask(-1.0),
                trade(-1.0),
                bid_size(0),
                ask_size(0),
                trade_size(0){
        }
        bool is_bid() const{return bid != -1.0 && bid != 0.0;}
        bool is_ask() const{return ask != -1.0 && ask != 0.0;}
    };

    /*
  * @class md_snapshot_t
  * @brief md_snapshot_t represent execution report from a broker
  */
    struct execution_report_t
    {
        std::string     tradeId ;
        uint64_t         clId ;
        std::string     origClId;
        __uint128_t     exchId;
        std::string     secId;
        time_t          time ;
        order_type_t    orderType ;
        report_type_t   type;
        exec_trans_t    transType;
        time_in_force_t tif;
        order_state_t   state;
        order_side_t    side ;
        ord_rej_reason  rejReason;
        double          limitPrice;
        double          avgPx;
        double          lastPx;
        double          leavesQty;
        double          cumQty;
        double          lastShares ;
        std::string     text;
    };

    struct order_t
    {
        uint64_t clId;
        __uint128_t exchId;
        std::string secId;
        std::string transaction_hash;
        double original_qty  = 0.0;
        double remaining_qty = 0.0;
        double price     = 0.0;
        double stopPrice = 0.0;
        order_side_t    side  = os_Undefined;
        order_state_t   state = ost_Undefined;
        time_in_force_t tif   = tf_Undefined;
        order_type_t type  = ot_Undefined;
        time_t init_time = 0;
        std::string currency;
    };

    struct order_update_t
    {
        std::string clId;
        std::string origId;
        std::string exchId;
        double price ;
        double stopPrice ;
        double qty ;
    };
}


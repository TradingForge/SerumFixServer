#include <iostream>
#include <string>
#include <atomic>

#include <SerumDEX/SerumMarket/Market.hpp>
#include <SerumDEX/SerumMarket/models.hpp>
#include <SerumDEX/PoolRequester/PoolsRequester.h>
#include <marketlib/include/BrokerModels.h>
#include <marketlib/include/enums.h>
#include "Appendix.hpp"
#include "settings.h"

using namespace std;
using namespace BrokerModels;
using namespace marketlib;
typedef instrument_descr_t Instrument;
// typedef order_t Order;


int main () 
{
    shared_ptr < ILogger > logger(new Logger);
    shared_ptr < ISettings > settings(new SerumSettings);
    shared_ptr < IPoolsRequester > pools(new PoolsRequester(logger, settings, "./market.json"));

    auto instr = Instrument{
        engine: "",
        sec_id: "",
        symbol: "SOL/USDC",
        base_currency: "SOL",
        quote_currency: "USDC"
    };

    // const auto p = pools->getPool(instr);
    // cout << p.address << endl;
    // cout << p.quote_mint_address << endl;
    // cout << p.base_mint_address << endl;
    // auto tt = 0;
    auto market = SerumMarket(PUBKEY, SECRETKEY, "https://solana-api.projectserum.com", pools, [](const string& name, const Instrument& inst, const string& info){
        std::cout << name << " || " << inst.symbol << " || " + info;
    });

    // cout << sizeof(Instruction) << endl;
    Instrument instrument{"", "", "SOL/USDC", "SOL", "USDC" };
    order_t order_sell;
    order_sell.price = 39;
    order_sell.original_qty = 0.1;
    order_sell.side = order_side_t::os_Sell;
    order_sell.clId = 1234567;

    order_t order_buy;
    order_buy.price = 15;
    order_buy.original_qty = 0.1;
    order_buy.side = order_side_t::os_Buy;
    order_buy.clId = 7654321;

    while (1)
    {
        string msg;
        cin >> msg;
        if (msg == "so") {
            order_sell = market.send_new_order(instrument, order_sell);
        }
        else if (msg == "sc") {
            order_sell = market.cancel_order(instrument, order_sell);
        }
        else if (msg == "bo") {
           order_buy = market.send_new_order(instrument, order_buy);
        }
        else if (msg == "bc") {
            order_buy = market.cancel_order(instrument, order_buy);
        }
        else if (msg == "stop") {
            break;
        }
    }
    
    
    // market.cancel_order(instrument, order);
}
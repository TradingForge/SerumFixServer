#include <iostream>
#include <string>
#include <atomic>

#include <SerumDEX/SerumMarket/Market.hpp>
#include <SerumDEX/SerumMarket/structs.hpp>
#include <SerumDEX/SerumPoolsRequester.h>
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
    shared_ptr < IPoolsRequester > pools(new SerumPoolsRequester(logger, settings));
    auto market = SerumMarket(PUBKEY, SECRETKEY, "https://solana-api.projectserum.com", pools, [](const string& a, const Instrument& b, const string& info){});

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
            market.send_new_order(instrument, order_sell);
        }
        else if (msg == "sc") {
            market.cancel_order(instrument, order_sell);
        }
        else if (msg == "bo") {
            market.send_new_order(instrument, order_buy);
        }
        else if (msg == "bc") {
            market.cancel_order(instrument, order_buy);
        }
        else if (msg == "stop") {
            break;
        }
    }
    
    
    // market.cancel_order(instrument, order);
}
#include <iostream>
#include <string>
#include <atomic>

#include <SerumDEX/SerumMD.h>
#include <marketlib/include/BrokerModels.h>
#include <marketlib/include/enums.h>
#include "Appendix.hpp"

using namespace std;
using namespace BrokerModels;
using namespace marketlib;
typedef marketlib::instrument_descr_t Instrument;


int main () {
    shared_ptr < ILogger > logger(new Logger);
    shared_ptr < ISettings > settings(new SerumSettings);
    shared_ptr < IBrokerApplication > application(new BrokerNullApplication(logger));

    SerumApp client(
        logger,
        application.get(),
        settings
    );

    Instrument instrument{"", "", "ETH/USDC", "USDC" };

    client.start();
    
    this_thread::sleep_for(1s);

    while (true) {
        string cmd;
        cin >> cmd;
        if (cmd == "start") {
            client.start();
        } else if (cmd == "stop") {
            client.stop();
        } else if (cmd == "quit") {
            break;
        } else if (cmd == "st") {
            client.subscribe(instrument, market_depth_t::top);
        } else if (cmd == "ut") {
            client.unsubscribe(instrument, market_depth_t::top);
        } else if (cmd == "sd") {
            client.subscribe(instrument, market_depth_t::full);
        } else if (cmd == "ud") {
            client.unsubscribe(instrument, market_depth_t::full);
        } else if (cmd == "inst") {
            auto instruments = client.getInstruments();
            for(auto instr : instruments) {
                cout << "Exch: " << instr.engine << "  Market: " << instr.symbol << "  Currency: "  << instr.currency << endl;
            }
        }
    }
}


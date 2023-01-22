#pragma once
#include "ILogger.h"

#include <string>
#include <functional>
#include <marketlib/include/market.h>

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/composite_key.hpp>

#include <SerumDEX/SerumMarket/sol_sdk/Keypair.hpp>

class IMarket
{
private:
    typedef std::string string;
    typedef marketlib::instrument_descr_t Instrument;
    typedef marketlib::order_t Order;
    typedef Keypair SecretKey;
public:

    IMarket() {}
	IMarket(const IMarket&) = delete;
	IMarket& operator=(const IMarket&) = delete;
    virtual ~IMarket() = default;

    virtual Order send_new_order(const Instrument& instrument, const Order& order, const SecretKey& key) = 0;
    virtual Order cancel_order(const Instrument& instrument, const Order& order, const SecretKey& key) = 0;
};
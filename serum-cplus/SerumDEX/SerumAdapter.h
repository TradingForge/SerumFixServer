#pragma once

#include <string>
#include <type_traits>
#include <boost/format.hpp>
#include <boost/json/src.hpp>
#include <boost/json.hpp>

#include <sharedlib/include/IBrokerClient.h>
#include <marketlib/include//BrokerModels.h>
#include <marketlib/include/enums.h>

namespace SerumAdapter {
	typedef marketlib::market_depth_t SubscriptionModel;
	typedef marketlib::order_side_t OrderSide;

	static std::string subscriptionModelToString(SubscriptionModel model) {
		static const std::string values[]{
			"level1", "level2"
		};
		return values[static_cast < int > (model)];
	}

	static OrderSide stringToOrderSide(std::string side) {
		if (side == "buy") {
			return OrderSide::os_Buy;
		}
		else if (side == "sell") {
			return OrderSide::os_Sell;
		}
		return OrderSide::os_Undefined;
	}

	static std::string getMarket(const BrokerModels::Instrument& instr) {
		return instr.first + "/" + instr.second;
	}
};
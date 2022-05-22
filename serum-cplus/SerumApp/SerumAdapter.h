#pragma once

#include <string>
#include <type_traits>
#include <boost/format.hpp>
#include <boost/json/src.hpp>
#include <boost/json.hpp>

#include "BrokerLib/IBrokerClient.h"
#include "BrokerLib/BrokerModels.h"

namespace SerumAdapter {
	static std::string subscriptionModelToString(IBrokerClient::SubscriptionModel model) {
		static const std::string values[]{
			"level1", "level2"
		};
		return values[static_cast < int > (model)];
	}

	static std::string orderSideToString(BrokerModels::OrderSide side) {
		static const std::string values[]{
			"buy", "sell"
		};
		return values[static_cast < int > (side)];
	}

	static std::string orderTypeToString(BrokerModels::OrderType type) {
		static const std::string values[]{
			"limit", "market"
		};
		return values[static_cast < int > (type)];
	}

	static std::string orderStateToString(BrokerModels::OrderState state) {
		static const std::string values[]{
			"Open", "Filled", "Replaced", "Canceled"
		};
		return values[static_cast < int > (state)];
	}

	static BrokerModels::OrderSide stringToOrderSide(std::string side) {
		if (side == "buy") {
			return BrokerModels::OrderSide::Buy;
		}
		else if (side == "sell") {
			return BrokerModels::OrderSide::Sell;
		}

		return BrokerModels::OrderSide::None;
	}

	static std::string getMarket(const BrokerModels::Instrument& instr) {
		return instr.first + "/" + instr.second;
	}
};
#include "SerumMD.h"
// #include <boost/json/src.hpp>
#include "SerumAdapter.h"

#define SERUM_DEBUG

using namespace std;
using namespace std::chrono;
// using namespace SerumAdapter;
using namespace BrokerModels;
using namespace marketlib;

void SerumMD::onOpen() {
#ifdef SERUM_DEBUG
_logger->Debug("> SerumMD::onOpen");
#endif
	_application->onEvent(getName(), BrokerEvent::SessionLogon, "Serum DEX Logon: " + getName());
}
void SerumMD::onClose() {
#ifdef SERUM_DEBUG
	_logger->Debug("> SerumMD::onClose");
#endif
	_application->onEvent(getName(), BrokerEvent::SessionLogout, "Serum DEX Logout: " + getName());
	clearMarkets();
}
void SerumMD::onFail() {
#ifdef SERUM_DEBUG
_logger->Debug("> SerumMD::onFail");
#endif
	_application->onEvent(getName(), BrokerEvent::SessionLogout, "Serum DEX Logout: " + getName());
	clearMarkets();
}
void SerumMD::onMessage(const string& message) {
	if (message[0] == '{') {
		onEventHandler(message);
	} else {
		onUpdateHandler(message);
	}
}

void SerumMD::onEventHandler(const string &message) {
	// logger->Info(message.c_str());

	auto parsed_data = boost::json::parse(message);

	string type = parsed_data.at("type").as_string().c_str();
	if (type == "subscribed" || type == "unsubscribed") {
		_logger->Info(message.c_str());
		return;
	} 

	// logger->Info(message.c_str());
	string market = parsed_data.at("market").as_string().c_str();
	if (type == "quote") {
			_top_snapshot[market] = MarketBook{
				system_clock::now(), 
				stod(parsed_data.at("bestBid").at(0).as_string().c_str()),
				stod(parsed_data.at("bestBid").at(1).as_string().c_str()),
				stod(parsed_data.at("bestAsk").at(0).as_string().c_str()),
				stod(parsed_data.at("bestAsk").at(1).as_string().c_str())
			};
		auto chnl = _channels
			.get<SubscribeChannelsByMarketAndSubscribeModel>()
			.equal_range(boost::make_tuple(
				market, 
				SubscriptionModel::top
			));
		_application->onReport(_name, chnl.first->instr.symbol, _top_snapshot[market]);
		// auto chnls = _channels
		// 	.get<SubscribeChannelsByMarketAndSubscribeModel>()
		// 	.equal_range(boost::make_tuple(
		// 		market, 
		// 		SubscriptionModel::top
		// 	));
		// while(chnls.first != chnls.second) {
		// 	chnls.first->callback_top(
		// 		_name,
		// 		chnls.first->instr,
		// 		top_snapshot[market]
		// 	);
		// 	++chnls.first;
  		// }
	} else if (type == "l2snapshot") {
		auto jsonToObject = [](const boost::json::value& val, std::list<BrokerModels::MarketUpdate>& vec) {
			for(auto set : val.as_array()) {
  				vec.push_back(MarketUpdate{
					stod(set.at(0).as_string().c_str()),
					stod(set.at(1).as_string().c_str())
				});
			}
		};
		string key =  parsed_data.at("market").as_string().c_str();
		_depth_snapshot[key] = DepthSnapshot{};
		auto& depth = _depth_snapshot[key];
		depth.bids = std::list<BrokerModels::MarketUpdate>();
		depth.asks = std::list<BrokerModels::MarketUpdate>();
		jsonToObject(parsed_data.at("asks"), depth.asks);
		jsonToObject(parsed_data.at("bids"), depth.bids);
		// application->onReport(name, key, depth);

		auto chnl = _channels
			.get<SubscribeChannelsByMarketAndSubscribeModel>()
			.equal_range(boost::make_tuple(
				market, 
				SubscriptionModel::full
			));
		_application->onReport(_name, chnl.first->instr.symbol, depth);
		// while(chnls.first != chnls.second){
		// 	chnls.first->callback_depth(
		// 		_name,
		// 		chnls.first->instr,
		// 		depth
		// 	);
		// 	++chnls.first;
  		// }
	} else if (type == "l2update") {
		// logger->Info(message.c_str());
		auto updateDepth = [](const boost::json::value& val, std::list<BrokerModels::MarketUpdate>& list, bool is_ask) {
			for(auto set : val.as_array()) {
  				if (stod(set.at(1).as_string().c_str()) == 0.) {
					list.erase(std::find_if(
						begin(list), 
						end(list), 
						[price = stod(set.at(0).as_string().c_str())](MarketUpdate market){ return market.price==price; }
					));
				}
				else {
					list.push_back(MarketUpdate{
						stod(set.at(0).as_string().c_str()),
						stod(set.at(1).as_string().c_str())
					});
				}
			}

			if (is_ask)
				list.sort([](const MarketUpdate&a, const MarketUpdate&b){ return a.price < b.price; });
				// std::sort(begin(vec), end(vec), 
				// 	[](const MarketUpdate&a, const MarketUpdate&b){ return a.price < b.price; });
			else
				list.sort([](const MarketUpdate&a, const MarketUpdate&b){ return a.price > b.price; });
				// std::sort(begin(vec), end(vec), 
				// 	[](const MarketUpdate&a, const MarketUpdate&b){ return a.price > b.price; });
		};
		string key =  parsed_data.at("market").as_string().c_str();
		auto& depth = _depth_snapshot[key];
		updateDepth(parsed_data.at("asks"), depth.asks, true);
		updateDepth(parsed_data.at("bids"), depth.bids, false);
		_application->onReport(_name, key, depth);
	// 	auto chnls = _channels
	// 		.get<SubscribeChannelsByMarketAndSubscribeModel>()
	// 		.equal_range(boost::make_tuple(
	// 			market, 
	// 			SubscriptionModel::full
	// 		));
	// 	while(chnls.first != chnls.second){
	// 		chnls.first->callback_depth(
	// 			_name,
	// 			chnls.first->instr,
	// 			depth
	// 		);
	// 		++chnls.first;
  	// 	}
	}
}

void SerumMD::onUpdateHandler(const string &message) {
	
}

bool SerumMD::enabledCheck() const {
	if (!isEnabled()) {
		_logger->Warn("Attempt to request disabled client");
	}
	return isEnabled();
}

bool SerumMD::connectedCheck() const {
	if (!isConnected()) {
		_logger->Warn("Attempt to request disconnected client");
	}
	return isConnected();
}

bool SerumMD::activeCheck() const {
	return enabledCheck() && connectedCheck();
}

SerumMD::SerumMD(logger_ptr logger, application_ptr application,settings_ptr settings, pools_ptr pools):
	_logger(logger), _application(application), _settings(settings), _pools(pools),
	_connection(this, settings->get(ISettings::Property::WebsocketEndpoint), logger), 
	_depth_snapshot(depth_snapshots()), _top_snapshot(top_snapshots()) {
		// pools->loadPools();
	}

bool SerumMD::isEnabled() const {
	return _connection.enabled;
}

bool SerumMD::isConnected() const {
	return _connection.connected;
}

void SerumMD::clearMarkets() {
#ifdef SERUM_DEBUG
	_logger->Debug("> SerumMD::clearMarkets");
#endif
	_depth_snapshot.clear();
	_top_snapshot.clear();
	_channels.clear();
}

void SerumMD::start() {
	_connection.async_start();
}
void SerumMD::stop() {
	_connection.async_stop();
	clearMarkets();
}

void SerumMD::subscribe(const instrument& instr, SubscriptionModel model) {
	auto chnls = _channels
		.get<SubscribeChannelsByMarketAndSubscribeModel>()
		.equal_range(boost::make_tuple(
			SerumAdapter::getMarketFromInstrument(instr), 
			model
		));
	
	if (chnls.first == chnls.second) {
		_connection.async_send((boost::format(R"({
			"op": "subscribe",
			"channel": "%1%",
			"markets": ["%2%"]
		})") % SerumAdapter::subscriptionModelToString(model) % SerumAdapter::getMarketFromInstrument(instr)).str());
	}

	_channels.insert(
		SubscribeChannel{
			market: SerumAdapter::getMarketFromInstrument(instr),
			instr: instr,
			smodel: model
		}
	); 
}

// void SerumMD::subscribe(const instrument& instr, const string& clientId, callbackTop callback) {
// 	auto chnls = channels
// 		.get<SubscribeChannelsByMarketAndSubscribeModel>()
// 		.equal_range(boost::make_tuple(
// 			getMarketFromInstrument(instr), 
// 			SubscriptionModel::top
// 		));
	
// 	if (chnls.first == chnls.second) {
// 		subscribe(instr, SubscriptionModel::top);
// 	} else {
// 		callback(
// 			name,
// 			instr,
// 			top_snapshot[getMarketFromInstrument(instr)]
// 		);
// 	}

	
// 	channels.insert(
// 		SubscribeChannel{
// 			clientId: clientId,
// 			market: getMarketFromInstrument(instr),
// 			instr: instr,
// 			smodel: SubscriptionModel::top,
// 			callback_top: callback,
// 			callback_depth: nullptr
// 		}
// 	); 
// }

// void SerumMD::subscribe(const instrument& instr, const string& clientId, callbackDepth callback) {
// 	auto chnls = channels
// 		.get<SubscribeChannelsByMarketAndSubscribeModel>()
// 		.equal_range(boost::make_tuple(
// 			getMarketFromInstrument(instr), 
// 			SubscriptionModel::full
// 		));
	
// 	if (chnls.first == chnls.second) {
// 		subscribe(instr, SubscriptionModel::full);
// 	} else {
// 		callback(
// 			name,
// 			instr,
// 			depth_snapshot[getMarketFromInstrument(instr)]
// 		);
// 	}

// 	channels.insert(
// 		SubscribeChannel{
// 			clientId: clientId,
// 			market: getMarketFromInstrument(instr),
// 			instr: instr,
// 			smodel: SubscriptionModel::full,
// 			callback_top: nullptr,
// 			callback_depth: callback
// 		}
// 	); 
// }

void SerumMD::unsubscribe(const instrument& instr, SubscriptionModel model) {
	auto chnl = _channels
		.get<SubscribeChannelsByMarketAndSubscribeModel>()
		.find(boost::make_tuple(
			SerumAdapter::getMarketFromInstrument(instr), 
			model
		));

	if (chnl == _channels.end()) {
		_logger->Error("Subscription not found");
		return;
	}

	_connection.async_send((boost::format(R"({
		"op": "unsubscribe",
		"channel": "%1%",
		"markets": ["%2%"]
	})") % SerumAdapter::subscriptionModelToString(model) % SerumAdapter::getMarketFromInstrument(instr)).str());

	_channels.erase(chnl);
	if (model == SubscriptionModel::top) {
		_top_snapshot.erase(SerumAdapter::getMarketFromInstrument(instr));
	} else {
		_depth_snapshot.erase(SerumAdapter::getMarketFromInstrument(instr));
	}
}

static size_t writeCallback(void* content, size_t size, size_t count, void* result) {
	((string*)result)->append((char*)content, size * count);
	return size * count;
}

std::list< SerumMD::instrument > SerumMD::getInstruments() {
    return _pools->getPools();
}

string SerumMD::getName() const {
	return _settings->get(ISettings::Property::ExchangeName);
}

SerumMD::~SerumMD() {
	_connection.async_stop();
	while (isConnected()) continue;
}
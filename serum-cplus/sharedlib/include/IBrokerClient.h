#pragma once

#include <string>
#include "ILogger.h"
#include <marketlib/include/BrokerModels.h>
#include <marketlib/include/enums.h>
#include <marketlib/include/market.h>
#include <list>

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/composite_key.hpp>

class IBrokerClient {
private:
	typedef std::string string;
	typedef marketlib::market_depth_t SubscriptionModel;
	typedef marketlib::instrument_descr_t instrument;
public:

	struct SubscribeChannel 
	{
		string market;
		instrument instr;
		SubscriptionModel smodel;
	};

	using SubscribedChannels = boost::multi_index::multi_index_container<
        SubscribeChannel,
        boost::multi_index::indexed_by<
            boost::multi_index::hashed_unique<
                boost::multi_index::tag<struct SubscribeChannelsByMarketAndSubscribeModel>,
                boost::multi_index::composite_key<
                    SubscribeChannel,
					boost::multi_index::member<SubscribeChannel, decltype(SubscribeChannel::market), &SubscribeChannel::market >,
					boost::multi_index::member<SubscribeChannel, decltype(SubscribeChannel::smodel), &SubscribeChannel::smodel >
                >
            >
        >
    >;

	enum class BrokerEvent {
		Info,
		Debug,
		Error,
		SessionLogon,
		SessionLogout,
		CoinSubscribed,
		CoinUnsubscribed,
		ConnectorStarted,
		ConnectorStopped,
		CoinSubscribedFault,
		CoinUnsubscribedFault,
		SubscribedCoinIsNotValid
	};

	IBrokerClient() {}
	IBrokerClient(const IBrokerClient&) = delete;
	IBrokerClient& operator = (const IBrokerClient&) = delete;

	virtual bool isEnabled() const = 0;
	virtual bool isConnected() const = 0;
	virtual string getName() const = 0;

	virtual void start() = 0;
	virtual void stop() = 0;

	virtual void subscribe(const instrument&, SubscriptionModel) = 0;
	virtual void unsubscribe(const instrument&, SubscriptionModel) = 0;
	virtual std::list< instrument > getInstruments() = 0;

	virtual ~IBrokerClient() = default;

};
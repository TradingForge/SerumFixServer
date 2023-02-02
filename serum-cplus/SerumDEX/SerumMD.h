#pragma once

#include <unordered_map>
#include <memory>
#include <set>
#include <vector>

#include <sharedlib/include/IBrokerClient.h>
#include <sharedlib/include/IBrokerApplication.h>
#include <sharedlib/include/ConnectionWrapper.h>
#include <sharedlib/include/ISettings.h>
#include <sharedlib/include/IPoolsRequester.h>
#include <marketlib/include/BrokerModels.h>
#include <marketlib/include/enums.h>

class SerumMD : public IBrokerClient {

private:

	friend ConnectionWrapper < SerumMD >;

	typedef std::string string;
	typedef std::shared_ptr < ILogger > logger_ptr;
	typedef std::shared_ptr < ISettings > settings_ptr;
	typedef std::shared_ptr < IPoolsRequester > pools_ptr;
	typedef IBrokerApplication* application_ptr;
	typedef BrokerModels::Market Market;
	typedef std::map < string,  BrokerModels::DepthSnapshot > depth_snapshots;
	typedef std::map < string,  BrokerModels::MarketBook > top_snapshots;
	typedef marketlib::market_depth_t SubscriptionModel;
	typedef marketlib::instrument_descr_t instrument;

protected:
	logger_ptr _logger;
	settings_ptr _settings;
	pools_ptr _pools;
    application_ptr _application;
	SubscribedChannels _channels;
	ConnectionWrapper < SerumMD > _connection;
	depth_snapshots _depth_snapshot;
	top_snapshots _top_snapshot;
	string _name;
	

	void onOpen();
	void onClose();
	void onFail();
	void onMessage(const string&);

	void onEventHandler(const string&);
	void onUpdateHandler(const string&);

	void clearMarkets();

	bool enabledCheck() const;
	bool connectedCheck() const;
	bool activeCheck() const;

public:
	SerumMD(logger_ptr, application_ptr, settings_ptr, pools_ptr);
	~SerumMD();

	bool isEnabled() const override;
	bool isConnected() const override;
	string getName() const override;

	void start() override;
	void stop() override;

	void subscribe(const instrument&, SubscriptionModel) override;
	void unsubscribe(const instrument&, SubscriptionModel) override;
	std::list< instrument > getInstruments() override;
};
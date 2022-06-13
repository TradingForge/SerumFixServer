import random
import time
import quickfix as fix
import quickfix44 as fix44
import traceback
from fix_client_wrapper import FixApp
from fix_client_wrapper import BrokerEvent


class Client:
    def __init__(self, config):
        self.price_settings = fix.SessionSettings(config)
        self.price_storeFactory = fix.MemoryStoreFactory()
        self.price_logFactory = fix.FileLogFactory(self.price_settings)
        self.price_application = FixApp('serum')
        self.price_application.event_func = self.on_event
        self.price_application.snapshot_func = self.on_full_snapshot
        self.instrument = {
            'First': "ETH",
            'Second': "USDC",
            'Symbol': "ETHUSDC",
            'SecurityID': "ETHUSDC",
            'SecurityType': "COIN",
            'SecurityExchange': "Serum",
        }

    def on_event(self, data):
        print('! {}-{}'.format(data["broker"], data["event"]))
        if data["event"] is BrokerEvent.SessionLogon:

            # do some logic
            # self.price_application.subscribe(self.instrument, True, False)
            pass

    def on_full_snapshot(self, broker, snapshot):
        print("{} | full for {}, rows {}".format(broker, snapshot['pool'], len(snapshot['data'])))
        for item in snapshot['data']:
            print(item)


if __name__ == '__main__':
    try:
        logic = Client('client_trade.cfg')
        price_initiator = fix.SocketInitiator(logic.price_application, logic.price_storeFactory, logic.price_settings,
                                              logic.price_logFactory)
        price_initiator.start()
        while True:
            message = input('enter e to exit the app\n')
            if message == "e":
                break
        price_initiator.stop()
        time.sleep(1)

    except Exception as e:
        print("Exception error: '%s'." % e)
        traceback.print_exc()
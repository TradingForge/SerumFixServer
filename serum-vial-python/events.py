from serum_modules.models import *
COUNT = 10

class EventHandler:
    def __init__(self, logger):
        self.__logger = logger

    def message_event(self, event, msg):
        match event:
            case Channels.Level1: 
                self.__logger.info(f"{msg['market']} BestAsk({msg['bestAsk']}) BestBid({msg['bestBid']})")
            case Channels.Level2:
                self.__logger.info(f"{msg['market']} \n")
                # print(f"{now.hour}:{now.minute}:{now.second} | <-- {msg['market']}")
                # print('Asks')
                # pprint(msg['asks'][:COUNT]) 
                # print()
                # print('Bids')
                # pprint(msg['bids'][:COUNT])
                # print()
            # case Channels.Level3:
            #     if isinstance(msg, list):
            #         print(f"{now.hour}:{now.minute}:{now.second} | <-- {msg[0]['market']}")
            #     else:
            #         print(f"{now.hour}:{now.minute}:{now.second} | <-- {msg['market']}")
            #     pprint(msg)
            #     print()
            


    def information_event(self, event, msg = None):
        match event:
            case MessageType.Event.Subscribed:
                self.__logger.info(f'channel: {msg.channel} | market: {msg.market}')
            case MessageType.Event.Unsubscribed:
                self.__logger.info(f'channel: {msg.channel} | market: {msg.market}')
            case _:
                self.__logger.info(f'{event} -- msg({msg if msg != None else ""})\n')
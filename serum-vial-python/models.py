class BrokerEvent:
    SessionLogon = "SessionLogon"
    SessionLogout = "SessionLogout"
    SessionError = "SessionError"
    InternalError = "InternalError"
    WarningDelay = "WarningDelay"
    MaximumDelay = "MaximumDelay"
    ConnectorStarted = "ConnectorStarted"
    ConnectorStopped = "ConnectorStopped"
    CoinSubscribed = "CoinSubscribed"
    CoinUnsubscribed = "CoinUnsubscribed"
    CoinSubscribedFault = "CoinSubscribedFault"
    CoinUnsubscribedFault = "CoinUnsubscribedFault"

class Ops:
    Subscribe = 'subscribe'
    Unsubscribe = 'unsubscribe'
    
class Channels:
    Level1 = 'level1'
    Level2 = 'level2'
    Level3 = 'level3'
    Trades = 'trades'
   
class MessageType: 
    class Event:
        Error = 'error'
        Subscribed = 'subscribed'
        Unsubscribed = 'unsubscribed'
        
    class Level1:
        Quote = 'quote'
        
    class Level2:
        Snapshot = 'l2snapshot'
        Update = 'l2update'
        
    class Level3:
        Snapshot = 'l3snapshot'
        Open = 'open'
        Fill = 'fill'
        Change = 'change'
        Done = 'done'
        
    class Trade:
        RecentTrades = 'recent_trades'
        Trade = 'trade'
    
class Instrument:
    def __init__(self, first: str, second: str):
        self.__first = first.upper()
        self.__second = second.upper()
        
    @property
    def first(self):
        return self.__first
    
    @property
    def second(self):
        return self.__second
        
    @property
    def market(self):
        return f'{self.__first}/{self.__second}'
    
class SubscribedChannel:
    def __init__(self, channel: Channels, market: str):
        self.__channel = channel
        self.__market = market
        
    def __eq__(self, other):
        return  self.__channel == other.__channel and \
                self.__market ==  other.__market
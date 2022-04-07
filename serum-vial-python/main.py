from wrapper import Wrapper, SocketObj
import json
from models import *
from pprint import pprint
import datetime 

COUNT = 10

class Logger:
    def info(self, mess):
        print(f'info -- {mess}')
        
        
    def debug(self, mess):
        print(f'debug -- {mess}')
        
        
def m_event(event, msg):
    now = datetime.datetime.now()
    match event:
        case Channels.Level1:
            print(f"{now.hour}:{now.minute}:{now.second} | <-- {msg['market']} BestAsk({msg['bestAsk']}) BestBid({msg['bestBid']})\n")
        case Channels.Level2:
            print(f"{now.hour}:{now.minute}:{now.second} | <-- {msg['market']}")
            print('Asks')
            pprint(msg['asks'][:COUNT]) 
            print()
            print('Bids')
            pprint(msg['bids'][:COUNT])
            print()

def i_event(event, msg = ''):
    pass

if __name__ == '__main__':
    wr = Wrapper(Logger(), m_event, i_event)
    
    wr.start()
    
    btc_usdt = Instrument('BTC', 'USDT')
    eth_usdt = Instrument('ETH', 'USDT')
    
    while True:
        msg = input()
        if msg == "l1":
            wr.subscribe(Channels.Level1, btc_usdt)
        elif msg == "l2":
            wr.subscribe(Channels.Level2, eth_usdt)
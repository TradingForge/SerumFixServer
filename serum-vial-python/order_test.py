from pyserum.connection import conn
from pyserum.market import Market
from pyserum.enums import OrderType, Side
from solana.rpc.types import TxOpts
from solana.keypair import Keypair
from solana.publickey import PublicKey
from solana.rpc.api import Client
# from pyserum import conn

from random import randint

from pyserum.connection import get_live_markets, get_token_mints
from pyserum.open_orders_account import OpenOrdersAccount
from pyserum import instructions
all_mint_address = get_token_mints()
all_market_address = get_live_markets()

from base58 import b58decode as decode

from config import KEYPAIR_PHANTOM

URL = "https://solana-api.projectserum.com"
# URL = "https://solana-mainnet.phantom.app"
# KEYPAIR_PHANTOM = '3n6zmw3hS5Hyo5duuhnNvwjAbjzC42uzCA3TTsrgr9htUonzDUXdK1d8b8J77XoeSherqWQM8mD8E1TMYCpksS2r'

# SOL/USDC
# 9wFFyRfZBsuAha4YcuxcXLKwMxJR43S7fPfQLusDBzvT
# 9xQeWvG816bUx9EPjHmaT23yvVM2ZWbrrpZb9PusVFin

def get_id():
    return randint(0, 0xffffffffffffffff)

SOL_USDC = '9wFFyRfZBsuAha4YcuxcXLKwMxJR43S7fPfQLusDBzvT'

class Order:
    def __init__(self, id, amount, price):
        self.__id = id
        self.__amount = amount
        self.__price = price
        
    def __eq__(self, other) -> bool:
        return self.__id == other.__id and self.__amount == other.__amount and self.__price == other.__price
    
    @property
    def id(self):
        return self.__id
    
    @property
    def amount(self):
        return self.__amount
    
    @property
    def price(self):
        return self.__price
    
    @property
    def side(self):
        return self.__side

if __name__ == "__main__":
    base = 'SOL'
    quote = 'USDC'
    
    cc = conn(URL)
    market_address = [i.address for i in all_market_address if i.name == f'{base}/{quote}'][0]
    
    base_wallet=PublicKey(decode("FPKRMqqYjVHohTgeB5o8TBvFru9MJoFBuxGbeWcNAGwm"))
    quote_wallet=PublicKey(decode("8mH43xRUYsvDmCJhUYjpcypLDBDf9XrgPXX15VKgZb3T"))
        
    owner = Keypair(decode(KEYPAIR_PHANTOM)[:32])
    payer = [i.address for i in all_mint_address if i.name == quote][0]

    market = Market.load(cc, market_address)
    client = Client(URL)
    
    open_orders_account = OpenOrdersAccount.find_for_market_and_owner(client, market_address, owner.public_key, instructions.DEFAULT_DEX_PROGRAM_ID)[0]
    
    
    my_orders = [Order(id=5, amount=5, price=5)]

    
    
        
    while True:
        i = input()
        
        match i :
            case  "b" :
                id = get_id()
                price = 81.23
                quantity = 0.1
                
                my_orders.append(Order(id=id, amount=quantity, price=price))
                
                tx = market.place_order(
                    payer=payer,
                    owner=owner,
                    side=Side.BUY,
                    order_type=OrderType.LIMIT,
                    limit_price=price,
                    max_quantity=quantity,
                    client_id=id,
                    opts = TxOpts(skip_preflight=True)
                )
                
                print(tx)
            
            case  "s" :     
                id = get_id()
                price = 80
                quantity = 0.1
                
                my_orders.append(Order(id=id, amount=quantity, price=price))
                           
                tx = market.place_order(
                    payer=payer,
                    owner=owner,
                    side=Side.SELL,
                    order_type=OrderType.LIMIT,
                    limit_price=price,
                    max_quantity=quantity,
                    client_id=id,
                    opts = TxOpts(skip_preflight=True)
                )
                
                print(tx)
                
            # cancel all
            case  "c" :
                my_orders.append(Order(id=8813322823396022418, amount=0.1, price=91.23))
                
                orders = [order for order in market.load_bids() 
                            if Order(id=order.client_id, amount=order.info.size, price=order.info.price) in my_orders] + \
                        [order for order in market.load_asks() 
                            if Order(id=order.client_id, amount=order.info.size, price=order.info.price) in my_orders]                
                
                for order in orders:
                    market.cancel_order(owner, order, opts=TxOpts(skip_confirmation=False))
                    
                    
                # tt = market.settle_funds(
                #     owner=owner, 
                #     open_orders=open_orders_account,
                #     base_wallet=base_wallet,
                #     quote_wallet=quote_wallet
                # )
                
                # print(tt)
                    
            case "asks" :
                asks = market.load_asks()
                for ask in asks:
                    print(f"Order id: {ask.client_id}, price: {ask.info.price}, size: {ask.info.size}")
                    
            case "bids" :
                bids = market.load_bids()
                for bid in bids:
                    print(f"Order id: {bid.client_id}, price: {bid.info.price}, size: {bid.info.size}")
pub use std::borrow::Borrow;
pub use std::hash::{Hash, Hasher};
use std::str::FromStr;
pub use std::time;
pub use std::time::UNIX_EPOCH;

pub use hmacsha::HmacSha;
pub use sha2::Sha256;
use crate::all::enums::{InstrumentClass, OrderAmount, OrderPrice, OrderSide, OrderStatus, RestError};

#[derive(Debug, Clone)]
pub struct Order {
    pub size: f64,
    pub price: Option<f64>,
    outer_id: String,
    meta: OrderMeta,
    status: OrderStatus,
}

impl Order {
    pub fn get_amount(&self) -> OrderAmount {
        OrderAmount::Fixed(self.size)
    }
    pub fn get_price(&self) -> OrderPrice {
        match self.price {
            None => OrderPrice::Market,
            Some(price) => OrderPrice::Limit(price),
        }
    }
    pub fn get_side(&self) -> OrderSide {
        self.meta.side
    }
    pub fn get_instrument(&self) -> Instrument {
        self.meta.instrument.instrument.clone()
    }
    pub fn get_exchange_id(&self) -> String {
        self.meta.id.to_string()
    }
    pub fn get_client_id(&self) -> &str {
        self.outer_id.as_str()
    }
}

impl Order {
    pub fn new(
        id: u64,
        size: f64,
        price: Option<f64>,
        side: OrderSide,
        instrument: MxFullInstrument,
    ) -> Self {
        let status = OrderStatus::Pending;
        let meta = OrderMeta::new(id, side, instrument);
        let outer_id = format!("{:x}", fxhash::hash64(&meta));
        Self {
            size,
            price,
            outer_id,
            meta,
            status,
        }
    }
}

impl Hash for Order {
    fn hash<H: Hasher>(&self, state: &mut H) {
        self.meta.hash(state);
    }
}

impl PartialEq for Order {
    fn eq(&self, other: &Self) -> bool {
        self.meta == other.meta
    }
}

impl Eq for Order {}

#[derive(Debug, Hash, PartialEq, Eq, Clone)]
pub struct OrderMeta {
    id: u64,
    side: OrderSide,
    instrument: MxFullInstrument,
}

impl OrderMeta {
    pub fn new(id: u64, side: OrderSide, instrument: MxFullInstrument) -> Self {
        Self {
            id,
            side,
            instrument,
        }
    }
}

impl Borrow<OrderMeta> for Order {
    fn borrow(&self) -> &OrderMeta {
        &self.meta
    }
}

pub enum GatewayAction {
    Place(Order),
    Move(Order),
    Cancel(Order),
    Terminate,
}

pub enum GatewayEvent {
    Trade(MxTrade),
    // OrderBook((MxInstrument, SharedOrderBook)),
    OrderStatus(Order),
    BalanceUpdate(Balance),
}

#[derive(Debug, Clone)]
pub struct Balance {
    pub total: f64,
    pub available: f64,
    pub locked: f64,
    meta: BalanceMeta,
}

impl Hash for Balance {
    fn hash<H: Hasher>(&self, state: &mut H) {
        self.meta.hash(state);
    }
}

impl PartialEq for Balance {
    fn eq(&self, other: &Self) -> bool {
        self.meta.eq(&other.meta)
    }
}

impl Eq for Balance {}

#[derive(Debug, Clone)]
pub struct BalanceMeta {
    pub account: String,
    pub gateway: String,
    pub coin: String,
}

impl BalanceMeta {
    pub fn new(account: String, gateway: String, coin: String) -> Self {
        Self {
            account,
            gateway,
            coin,
        }
    }

    pub fn into_default_balance(self) -> Balance {
        Balance::new(self.account, self.gateway, self.coin, Default::default())
    }
}

impl Hash for BalanceMeta {
    fn hash<H: Hasher>(&self, state: &mut H) {
        self.account.hash(state);
        self.gateway.hash(state);
        self.coin.hash(state);
    }
}

impl PartialEq for BalanceMeta {
    fn eq(&self, other: &Self) -> bool {
        self.account.eq(&other.account)
            & self.gateway.eq(&other.gateway)
            & self.coin.eq(&other.coin)
    }
}

impl Eq for BalanceMeta {}

impl Balance {
    pub fn new(account: String, gateway: String, coin: String, value: f64) -> Self {
        let meta = BalanceMeta::new(account, gateway, coin);
        let available = value;
        let locked = Default::default();
        Self {
            total: value,
            available,
            locked,
            meta,
        }
    }
}

#[derive(Debug)]
pub struct OrderBookData {
    pub asks: Vec<Level>,
    pub bids: Vec<Level>,
    pub instrument: Instrument,
}

pub struct OrderDescriptor {
    symbol: String,
    exchange_id: String,
    client_id: Option<String>,
}

impl OrderDescriptor {
    pub fn new(symbol: String, exchange_id: String, client_id: Option<String>) -> Self {
        Self {
            symbol,
            exchange_id,
            client_id,
        }
    }
}

impl Clone for OrderDescriptor {
    fn clone(&self) -> Self {
        Self::new(
            self.symbol.clone(),
            self.exchange_id.clone(),
            self.client_id.clone().map(|x| x.clone()),
        )
    }
}

impl OrderDescriptor {
    pub fn get_exchange_id(&self) -> &str {
        self.exchange_id.as_str()
    }
    pub fn get_client_id(&self) -> Option<&str> {
        if let Some(client_id) = &self.client_id {
            Some(client_id.as_str())
        } else {
            None
        }
    }
    pub fn get_symbol(&self) -> &str {
        self.symbol.as_str()
    }
}

impl OrderBookData {
    pub fn new(instrument: Instrument) -> Self {
        Self {
            asks: Vec::new(),
            bids: Vec::new(),
            instrument,
        }
    }

    pub fn update_ask(&mut self, level: Level) {
        // FIXME: potentially too slow function
        let res = self
            .asks
            .binary_search_by(|l| l.price.partial_cmp(&level.price).unwrap());
        match res {
            Ok(idx) => unsafe {
                *self.asks.get_unchecked_mut(idx) = level;
            },
            Err(idx) => self.asks.insert(idx, level),
        }
    }

    pub fn update_bid(&mut self, level: Level) {
        let res = self
            .bids
            .binary_search_by(|l| l.price.partial_cmp(&level.price).unwrap());
        match res {
            Ok(idx) => unsafe {
                *self.bids.get_unchecked_mut(idx) = level;
            },
            Err(idx) => self.bids.insert(idx, level),
        }
    }

    pub fn drop_zero_levels(&mut self) {
        self.asks.retain(|level| level.size != 0.0);
        self.bids.retain(|level| level.size != 0.0);
    }

    pub fn best_ask(&self) -> Option<&Level> {
        self.asks.get(0)
    }

    pub fn best_bid(&self) -> Option<&Level> {
        self.bids.get(0)
    }

    pub fn cbook_ask(&self, size: f64) -> Option<f64> {
        Self::cbook(&self.asks, size)
    }

    pub fn cbook_bid(&self, size: f64) -> Option<f64> {
        Self::cbook(&self.bids, size)
    }

    fn cbook(levels: &[Level], size: f64) -> Option<f64> {
        let mut res: f64 = 0.0;
        let mut valid_levels = Vec::with_capacity(30);
        for level in levels.iter() {
            res += level.size;
            valid_levels.push(level);
            if res >= size {
                break;
            }
        }

        if res < size {
            None
        } else {
            let sum = valid_levels
                .into_iter()
                .fold(0.0, |acc, level| acc + level.size)
                / res;
            Some(sum)
        }
    }

    pub fn get_instrument(&self) -> &Instrument {
        &self.instrument
    }
}

pub struct MxTrade {
    pub id: String,
    pub order_id: Option<String>,
    pub instrument: MxFullInstrument,
    pub real_account: String,
    pub base_qty: f64,
    pub quote_qty: f64,
    pub price: f64,
    pub side: OrderSide,
    pub commission_asset: String,
    pub commission: f64,
    pub is_market: bool,
    pub timestamp: chrono::DateTime<chrono::Utc>,
}

impl MxTrade {
    pub fn new(
        id: String,
        order_id: Option<String>,
        instrument: MxFullInstrument,
        real_account: String,
        base_qty: f64,
        quote_qty: f64,
        price: f64,
        side: OrderSide,
        commission_asset: String,
        commission: f64,
        is_market: bool,
        timestamp: chrono::DateTime<chrono::Utc>,
    ) -> Self {
        Self {
            id,
            order_id,
            instrument,
            real_account,
            base_qty,
            quote_qty,
            price,
            side,
            commission_asset,
            commission,
            is_market,
            timestamp,
        }
    }
}

#[derive(Debug, Copy, Clone)]
pub struct Level {
    pub price: f64,
    pub size: f64,
}

#[derive(Debug, Clone)]
pub struct TickerData {
    pub instrument: Instrument,
    pub bid: Level,
    pub ask: Level,
}

impl TickerData {
    pub fn get_trade_price(&self) -> f64 {
        (self.bid.price + self.ask.price) / 2.0
    }
}

#[derive(Hash, Debug, Clone, PartialEq, Eq)]
pub struct Instrument {
    symbol: String,
    gateway: Option<String>,
    class: Option<InstrumentClass>,
}

impl Instrument {
    pub fn new(symbol: String, gateway: Option<String>, class: Option<InstrumentClass>) -> Self {
        Self {
            symbol,
            gateway,
            class,
        }
    }
    pub fn get_asset(&self) -> &str {
        self.symbol.as_str()
    }
    pub fn get_gateway(&self) -> Option<&str> {
        if let Some(gateway) = &self.gateway {
            Some(gateway.as_str())
        } else {
            None
        }
    }
    pub fn get_class(&self) -> Option<InstrumentClass> {
        self.class.clone()
    }
}

#[derive(Debug, Hash, Clone, PartialEq, Eq)]
pub struct MxFullInstrument {
    account: Option<String>,
    instrument: Instrument,
}

impl MxFullInstrument {
    pub fn new(
        account: Option<String>,
        symbol: String,
        gateway: Option<String>,
        class: Option<InstrumentClass>,
    ) -> Self {
        let instrument = Instrument::new(symbol, gateway, class);
        Self {
            account,
            instrument,
        }
    }
    pub fn get_account(&self) -> Option<&str> {
        if let Some(account) = &self.account {
            Some(account.as_str())
        } else {
            None
        }
    }
    pub fn get_symbol(&self) -> &str {
        self.instrument.get_asset()
    }
    pub fn get_class(&self) -> Option<InstrumentClass> {
        self.instrument.get_class()
    }
    pub fn get_gateway(&self) -> Option<&str> {
        self.instrument.get_gateway()
    }
}

impl Default for InstrumentClass {
    fn default() -> Self {
        InstrumentClass::U
    }
}

impl ToString for Instrument {
    fn to_string(&self) -> String {
        self.get_asset().into()
    }
}

pub fn get_unix_time_millis() -> u128 {
    time::SystemTime::now()
        .duration_since(UNIX_EPOCH)
        .unwrap()
        .as_millis()
}

pub fn make_signature(api_secret: &str, message: &str) -> String {
    hex::encode(
        HmacSha::new(api_secret.as_bytes(), message.as_bytes(), Sha256::default()).compute_digest(),
    )
}

pub struct ApiCredentials {
    pub api_key: String,
    pub api_secret: String,
}

impl Clone for ApiCredentials {
    fn clone(&self) -> Self {
        ApiCredentials::new(self.api_key.as_str(), self.api_secret.as_str())
    }
}

impl ApiCredentials {
    pub fn new(api_key: &str, api_secret: &str) -> ApiCredentials {
        ApiCredentials {
            api_key: api_key.to_string(),
            api_secret: api_secret.to_string(),
        }
    }
    pub fn get_api_key(&self) -> &str {
        self.api_key.as_str()
    }
    pub fn get_api_secret(&self) -> &str {
        self.api_secret.as_str()
    }
}

pub type RestResult<T> = Result<T, RestError>;

impl FromStr for Instrument {
    type Err = ();
    fn from_str(symbol: &str) -> Result<Self, Self::Err> {
        Ok(Instrument::new(symbol.to_string(), None, None))
    }
}

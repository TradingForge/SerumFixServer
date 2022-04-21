#[derive(Clone, PartialEq)]
pub enum OrderAmount {
    All,
    Fixed(f64),
}

#[derive(Clone, PartialEq)]
pub enum OrderPrice {
    Market,
    Limit(f64),
}

#[derive(Debug, Hash, Clone, Copy, PartialEq, Eq)]
pub enum OrderSide {
    Ask,
    Bid,
}

#[derive(Debug, Hash, Clone, Copy, PartialEq, Eq)]
pub enum OrderStatus {
    Pending,
    Filled,
    Canceled,
    Placed,
}

#[derive(Debug)]
pub enum RestError {
    Request(reqwest::Error),
    Parse(serde_json::Error),
}

#[non_exhaustive]
#[derive(Hash, Debug, Clone, PartialEq, Eq)]
pub enum InstrumentClass {
    U,
    R,
}

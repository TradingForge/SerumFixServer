use async_trait::async_trait;
use serde::{Deserialize, Serialize};
use ws::{Factory, Message};

use crate::all::base::{Instrument, Order, OrderBookData, OrderDescriptor, RestResult, TickerData};


// #[async_trait]
// pub trait Connector {
//     async fn get_instruments(&self) -> BinanceRestResult<Vec<Instrument>>;
//     async fn get_ticker(&self, instrument: Instrument) -> BinanceRestResult<TickerData>;
//     async fn get_order_book(
//         &self,
//         instrument: Instrument,
//         levels: Option<i64>,
//     ) -> BinanceRestResult<OrderBookData>;
//     async fn send_order(&self, order: Order) -> BinanceRestResult<OrderDescriptor>;
//     async fn cancel_order(&self, order: Order) -> BinanceRestResult<OrderDescriptor>;
// }

#[async_trait]
pub trait Exchange {
    fn name() -> &'static str;
    fn url() -> &'static str;
    async fn ping() -> bool;
}

pub trait WebSocketHandler<Event>: Factory + Sync {
    type HandlerSend: ws::Handler + Send;
    fn new(handler: Self::HandlerSend) -> Self;
}

#[async_trait]
pub trait RestApiEndpoint<Request, Response> {
    async fn execute(&self, request: Request) -> RestResult<Response>;
}

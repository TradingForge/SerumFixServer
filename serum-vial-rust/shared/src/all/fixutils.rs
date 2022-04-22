

use crate::all::enums::{OrderSide, OrderStatus, OrderType, ExecType, TimeInForce, ExecTransType, OrdRejectResponse};

pub trait From { fn from(&self) -> char; }

impl From for OrderType {
    fn from(&self) -> char{
        match self{
            OrderType::Market => '0',
            OrderType::Limit => '1',
            OrderType::Stop => '2',
            OrderType::StopLimit => '3',
        }
    }
}

impl From for OrderSide {
    fn from(&self) -> char{
        match self{
            OrderSide::Buy => '1',
            OrderSide::Sell => '2',
        }
    }
}

impl From for OrderStatus {
    fn from(&self) -> char{
        match self{
            OrderStatus::New => '0',
            OrderStatus::PartFilled => '1',
            OrderStatus::Filled => '2',
            OrderStatus::Canceled => '4',
            OrderStatus::Replaced => '5',
            OrderStatus::PendingCancel => '6',
            OrderStatus::Rejected => '8',
        }
    }
}

impl From for ExecType {
    fn from(&self) -> char{
        match self{
            ExecType::New => '0',
            ExecType::PartFilled => '1',
            ExecType::Filled => '2',
            ExecType::Canceled => '4',
            ExecType::Replaced => '5',
            ExecType::PendingCancel => '6',
            ExecType::Rejected => '8',
            ExecType::PendingNew => 'A',
            ExecType::Calculated => 'B',
            ExecType::Expired => 'C',
            ExecType::PendingReplace => 'E',
            ExecType::Trade => 'F',
            ExecType::OrderStatus => 'I',
        }
    }
}

impl From for TimeInForce {
    fn from(&self) -> char{
        match self{
            TimeInForce::Day => '0',
            TimeInForce::GTC => '1',
            TimeInForce::OPG => '2',
            TimeInForce::IOC => '3',
            TimeInForce::FOK => '4',
        }
    }
}

impl From for ExecTransType {
    fn from(&self) -> char{
        match self{
            ExecTransType::New => '0',
            ExecTransType::Cancel => '1',
            ExecTransType::Correct => '2',
            ExecTransType::Status => '3',
        }
    }
}

impl From for OrdRejectResponse {
    fn from(&self) -> char{
        match self{
            OrdRejectResponse::CancelRequest => '1',
            OrdRejectResponse::CancelReplaceRequest => '2',
        }
    }
}

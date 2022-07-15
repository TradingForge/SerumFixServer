#pragma once
#include "sol_sdk/types.h"
#include "sol_sdk/key.h"
#include "enums.h"
#include <string>
#define VERSION 0


struct MarketLayout
{
    // uint8_t serum[5];
    uint64_t account_flags;
    SolPubkey own_address;
    uint64_t vault_signer_nonce;
    SolPubkey base_mint;
    SolPubkey quote_mint;
    SolPubkey base_vault;
    uint64_t base_deposits_total;
    uint64_t base_fees_accrued;
    SolPubkey quote_vault;
    uint64_t quote_deposits_total;
    uint64_t quote_fees_accrued;
    uint64_t quote_dust_threshold;
    SolPubkey request_queue;
    SolPubkey event_queue;
    SolPubkey bids;
    SolPubkey asks;
    uint64_t base_lot_size;
    uint64_t quote_lot_size;
    uint64_t fee_rate_bps;
    uint64_t referrer_rebate_accrued;
    // uint8_t padding[7];
};

struct Order
{
  std::string first;
  std::string second;
  double amount;
  double price;
  Side side;
  long long unsigned int client_id = 0;
};

struct InitializeMarket
{
    uint64_t base_lot_size;
    uint64_t quote_lot_size;
    uint16_t fee_rate_bps;
    uint64_t vault_signer_nonce;
    uint64_t quote_dust_threshold;
};

struct NewOrder
{
    uint32_t side;
    uint64_t limit_price;
    uint64_t max_quantity;
    uint32_t order_type;
    uint64_t client_id;
};

struct CancelOrder
{
    uint32_t side;
    uint8_t order_id[16];
    uint8_t open_orders[32];
    uint8_t open_orders_slot;
};

struct CancelOrderByClient
{
    uint64_t client_id;
};

struct NewOrderV3
{
    uint32_t side;
    uint64_t limit_price;
    uint64_t max_base_quantity;
    uint64_t max_quote_quantity;
    uint32_t self_trade_behavior;
    uint32_t order_type;
    uint64_t client_id;
    uint16_t limit;
};

struct CancelOrderV2
{
    uint32_t side;
    uint8_t order_id[16];
};

struct CancelOrderByClientV2
{
    uint64_t client_id;
};

struct InstructionLayout
{
    const uint8_t version = VERSION;
    uint32_t instruction_type;
};

struct InstructionLayoutNewOrderV3 : InstructionLayout
{
    NewOrderV3 args;
};

struct InstructionLayoutCancelOrderV2 : InstructionLayout
{
    CancelOrderV2 args;
};
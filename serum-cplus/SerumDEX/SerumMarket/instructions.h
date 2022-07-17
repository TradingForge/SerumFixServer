#pragma once

// #include "sol_sdk/key.h"
// #include "sol_sdk/cpi.h"
// #include "enums.h"
// #include "constants.h"
// #include "sysvar.h"
// #include "instruments.h"


// struct NewOrderV3Params 
// {
//     SolPubkey market;
//     SolPubkey open_orders;
//     SolPubkey payer;
//     SolPubkey owner;
//     SolPubkey request_queue;
//     SolPubkey event_queue;
//     SolPubkey bids;
//     SolPubkey asks;
//     SolPubkey base_vault;
//     SolPubkey quote_vault;
//     Side side;
//     uint64_t limit_price;
//     uint64_t max_base_quantity;
//     uint64_t max_quote_quantity;
//     OrderType order_type;
//     SelfTradeBehavior self_trade_behavior;
//     uint64_t limit;
//     uint64_t client_id;
//     SolPubkey program_id;//= base58str_to_pubkey("9xQeWvG816bUx9EPjHmaT23yvVM2ZWbrrpZb9PusVFin");
// };

// void new_order_v3(NewOrderV3Params params, SolInstruction& instruction) 
// {
//     instruction.program_id = params.program_id;
//     instruction.accounts = new SolAccountMeta[12] {
//         SolAccountMeta { pubkey: params.market, is_writable: true, is_signer: false },
//         SolAccountMeta { pubkey: params.open_orders, is_writable: true, is_signer: false },
//         SolAccountMeta { pubkey: params.request_queue, is_writable: true, is_signer: false },
//         SolAccountMeta { pubkey: params.event_queue, is_writable: true, is_signer: false },
//         SolAccountMeta { pubkey: params.bids, is_writable: true, is_signer: false },
//         SolAccountMeta { pubkey: params.asks, is_writable: true, is_signer: false },
//         SolAccountMeta { pubkey: params.payer, is_writable: true, is_signer: false },
//         SolAccountMeta { pubkey: params.owner, is_writable: false, is_signer: true },
//         SolAccountMeta { pubkey: params.base_vault, is_writable: true, is_signer: false },
//         SolAccountMeta { pubkey: params.quote_vault, is_writable: true, is_signer: false },
//         SolAccountMeta { pubkey: base58str_to_pubkey(TOKEN_PROGRAM_ID), is_writable: false, is_signer: false },
//         SolAccountMeta { pubkey: base58str_to_pubkey(SYSVAR_RENT_PUBKEY), is_writable: false, is_signer: false }
//     };
//     instruction.account_len = 12;

//     instruction.data;
//     // return SolInstruction {
//     //     program_id: &params.program_id,
//     //     accounts: touched_keys,
//     //     account_len: 12,
//     //     data: 0,
//     //     //TODO: serialize new_order
//     //     // NewOrderV3 {
//     //     //     side: params.side,
//     //     //     limit_price: params.limit_price,
//     //     //     max_base_quantity: params.max_base_quantity,
//     //     //     max_quote_quantity: params.max_quote_quantity,
//     //     //     self_trade_behavior: params.self_trade_behavior,
//     //     //     order_type: params.order_type,
//     //     //     client_id: params.client_id,
//     //     //     limit: 65535
//     //     // }
//     //     data_len: sizeof(NewOrderV3)
//     // };
// }

// {"jsonrpc":"2.0","result":{"context":{"slot":141767266},"value":{"lamports":146335070945,"data":{"program":"spl-token","parsed":{"info":{"decimals":6,"freezeAuthority":"3sNBr7kMccME5D55xNgsmYpZnzPgP2g12CixAajXypn6","isInitialized":true,"mintAuthority":"2wmVCSfPxGPjrnMMn7rchp4uaeoTqN39mXFC2zhPdri9","supply":"5034999491965473"},"type":"mint"},"space":82},"owner":"TokenkegQfeZyiNwAJbNbGKPFXCWuBvf9Ss623VQ5DA","executable":false,"rentEpoch":328}},"id":1}
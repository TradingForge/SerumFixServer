#include "transaction.h"
#include "instruments.h"
#include <algorithm>
#include <functional>
#include <numeric>
#include <string.h>
#include <iostream>


std::string to_hex(const uint8_t* vec, size_t len)
{
    std::string map = "0123456789abcdef";
    std::string res = "";
    for (size_t i = 0; i < len; i++, vec++) {
        res.push_back(map[(*vec >> 4) & 0xf]);
        res.push_back(map[*vec & 0xf]);
    }

    return res;
}


void Transaction::add_instruction(const SolInstruction& instruction)
{
    instructions.push_back(instruction);
}

void Transaction::set_recent_blockhash(const string& blockhash)
{
    _message.recent_blockhash = base58str_to_hash(blockhash);
}

std::string Transaction::get_message_for_sign()
{
    size_t datas_len = std::accumulate(
        instructions.begin(), 
        instructions.end(), 
        0, 
        [](size_t sum, const SolInstruction& a) {return sum + a.data_len;}
    );

    _message.data = std::vector<uint8_t>(datas_len, 0);
    size_t occupied = 0;

    std::vector<SolPubkey*> signed_writable_accounts;
    std::vector<SolPubkey*> signed_accounts;
    std::vector<SolPubkey*> writable_accounts;
    std::vector<SolPubkey*> other_accounts;

    for (auto instr: instructions) {
        _message.header.num_accounts+=instr.account_len;
        auto tmp = instr.accounts;
        for (size_t i = 0; i < instr.account_len; i++, tmp++) {
            if (tmp->is_signer) {
                ++_message.header.num_required_signatures;
                if (tmp->is_writable) {
                    signed_writable_accounts.push_back(&tmp->pubkey);
                }
                else {
                    ++_message.header.num_readonly_signed_accounts;
                    signed_accounts.push_back(&tmp->pubkey);
                }
            }
            else {
                if (tmp->is_writable) {
                    writable_accounts.push_back(&tmp->pubkey);
                }
                else {
                    ++_message.header.num_readonly_unsigned_accounts;
                    other_accounts.push_back(&tmp->pubkey);
                }
            }
        }
        memcpy(_message.data.data() + occupied, instr.data, instr.data_len);
        occupied += instr.data_len;
    }
    other_accounts.push_back(&instructions[0].program_id);
    // _message.header.num_accounts = _message.header.num_required_signatures 
    //     + _message.header.num_readonly_signed_accounts 
    //     + _message.header.num_readonly_unsigned_accounts;
    _message.header.num_readonly_unsigned_accounts++;
    _message.header.num_accounts++;

    _message.account_keys = std::vector<uint8_t>(SIZE_PUBKEY * _message.header.num_accounts, 0);

    size_t shift = 0;
    add_keys(_message.account_keys, signed_writable_accounts, shift);
    shift += signed_writable_accounts.size() * SIZE_PUBKEY;

    add_keys(_message.account_keys, signed_accounts, shift);
    shift += signed_accounts.size() * SIZE_PUBKEY;

    add_keys(_message.account_keys, writable_accounts, shift);
    shift += writable_accounts.size() * SIZE_PUBKEY;

    add_keys(_message.account_keys, other_accounts, shift);
    shift += other_accounts.size() * SIZE_PUBKEY;

    size_t size = 4+_message.account_keys.size()+SIZE_HASH+_message.data.size();
    uint8_t messag[size];

    shift = 0;
    memcpy(messag, &_message.header, 4); shift += 4;
    memcpy(messag + shift, _message.account_keys.data(), _message.account_keys.size()); shift += _message.account_keys.size();
    memcpy(messag + shift, &_message.recent_blockhash, SIZE_HASH); shift += SIZE_HASH;
    memcpy(messag + shift, _message.data.data(), _message.data.size()); shift += _message.data.size();

    auto tt = to_hex(messag, 498);
    std::cout << tt << std::endl;
    return "";
}

void Transaction::add_keys(std::vector<uint8_t>& data, const std::vector<SolPubkey*> new_keys, size_t shift)
{
    for (auto key : new_keys) {
        memcpy(data.data() + shift, key, SIZE_PUBKEY);
        shift += SIZE_PUBKEY;
    }
}
    
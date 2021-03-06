﻿#ifndef CKX_PRELEXED_TYPE_HPP
#define CKX_PRELEXED_TYPE_HPP

#include "frontend/lex/ckx_token.hpp"
#include "saber/vector.hpp"
#include "saber/string_pool.hpp"

namespace ckx
{

class ckx_prelexed_type
{
public:
    /// @note once determined, it's immutable.
    explicit ckx_prelexed_type(saber::vector<ckx_token>&& _prelexed_tokens) :
        prelexed_tokens(saber::move(_prelexed_tokens)) {}
    ~ckx_prelexed_type() = default;
    ckx_prelexed_type(ckx_prelexed_type&& _another) :
        prelexed_tokens(saber::move(_another.prelexed_tokens)) {}

    ckx_prelexed_type(const ckx_prelexed_type&) = delete;

    const saber::vector<ckx_token>& get_prelexed_tokens() const
    {
        return prelexed_tokens;
    }

    saber_string to_string() const
    {
        saber_string ret = "";
        for (auto& token : prelexed_tokens)
            ret += to_string_helper(token) + " ";
        return ret;
    }

private:
    const saber::vector<ckx_token> prelexed_tokens;
};

}

#endif // CKX_PRELEXED_TYPE_HPP

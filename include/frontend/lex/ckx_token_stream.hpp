﻿/**
    The Opensource Compiler CKX -- for my honey ChenKX
    Copyright (C) 2017  CousinZe

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see http://www.gnu.org/licenses/.
  */

#ifndef CKX_TOKEN_STREAM_HPP
#define CKX_TOKEN_STREAM_HPP

#include "saber/memory.hpp"
#include "saber/string.hpp"
#include "saber/vector.hpp"
#include "saber/unordered_map.hpp"
#include "we/file_reader.hpp"

#include "frontend/lex/ckx_token.hpp"
#include "frontend/util/ckx_error.hpp"

namespace ckx
{

namespace detail
{
class ckx_token_stream_impl;
} // namespace detail


class ckx_token_stream final
{
public:
    explicit ckx_token_stream(we::we_file_reader &_file_reader);
    ~ckx_token_stream();

    ckx_token& operator[] (int _offset);
    saber::vector<ckx_error>& get_error();
    void operator++ ();

private:
    detail::ckx_token_stream_impl *impl;
};

} // namespace ckx

#endif // CKX_TOKEN_STREAM_HPP

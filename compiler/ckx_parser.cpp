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

#include "ckx_parser.hpp"

#include "ckx_ast_node.hpp"
#include "ckx_type.hpp"

namespace ckx
{

namespace detail
{

template <typename CkxTokenStream>
class ckx_parser_impl
{
public:
    ckx_parser_impl() = default;
    ~ckx_parser_impl() = default;

    ckx_ast_translation_unit* parse_impl(
            saber_ptr<CkxTokenStream> _token_stream);

private:
    ckx_ast_stmt* parse_global_stmt();
    ckx_ast_stmt* parse_stmt();

    ckx_ast_decl_stmt* parse_decl_stmt();
    ckx_ast_func_stmt* parse_func_stmt();
    ckx_ast_struct_stmt* parse_struct_stmt();
    ckx_ast_variant_stmt* parse_variant_stmt();
    ckx_ast_enum_stmt* parse_enum_stmt();
    ckx_ast_compound_stmt* parse_ckx_block();

    ckx_ast_expr_stmt* parse_expr_stmt();
    ckx_ast_if_stmt* parse_if_stmt();
    ckx_ast_while_stmt* parse_while_stmt();
    ckx_ast_do_while_stmt *parse_do_while_stmt();
    ckx_ast_for_stmt* parse_for_stmt();
    ckx_ast_break_stmt* parse_break_stmt();
    ckx_ast_continue_stmt* parse_continue_stmt();
    ckx_ast_return_stmt* parse_return_stmt();
    ckx_ast_compound_stmt* parse_compound_stmt();

    ckx_ast_param_decl* parse_param();
    ckx_ast_expr* parse_expr();

    ckx_ast_struct* parse_struct();
    ckx_ast_variant* parse_variant();
    ckx_ast_enum* parse_enum();

    saber_ptr<ckx_type> parse_type();

    inline saber_ptr<ckx_token> current_token();
    inline saber_ptr<ckx_token> peek_next_token();

    inline void move2_next_token();
    inline bool expect_n_eat(ckx_token::type _token_type);
    inline bool expect(ckx_token::type _token_type);

    inline ckx_env_table* env();
    inline void enter_scope(ckx_env_table* _new_scope_env);
    inline void leave_scope();

    // For easing coding we replace the template TokenStream
    // with ckx_token_stream. Remember to replace back!
    Q_ON_HOLD(inline constexpr CkxTokenStream& stream();)
    inline ckx_token_stream& stream();

    bool is_typename(saber_ptr<ckx_token> _token) const;

    saber_ptr<CkxTokenStream> token_stream;
    ckx_env_table *current_env;
};

} // namespace detail



template <typename CkxTokenStream>
ckx_parser<CkxTokenStream>::ckx_parser()
{
    p_impl = new detail::ckx_parser_impl<CkxTokenStream>;
}

template <typename CkxTokenStream>
ckx_parser<CkxTokenStream>::~ckx_parser()
{
    delete p_impl;
}

template <typename CkxTokenStream>
ckx_ast_translation_unit*
ckx_parser<CkxTokenStream>::parse(saber_ptr<CkxTokenStream> _token_stream)
{
    return p_impl->parse_impl(_token_stream);
}

namespace detail
{

template <typename CkxTokenStream>
ckx_ast_stmt*
ckx_parser_impl<CkxTokenStream>::parse_global_stmt()
{
    switch ( current_token()->token_type )
    {
    case ckx_token::type::token_int8:
    case ckx_token::type::token_int16:
    case ckx_token::type::token_int32:
    case ckx_token::type::token_int64:
    case ckx_token::type::token_uint8:
    case ckx_token::type::token_uint16:
    case ckx_token::type::token_uint32:
    case ckx_token::type::token_uint64:
    case ckx_token::type::token_char:
    case ckx_token::type::token_real32:
    case ckx_token::type::token_real64:
        return parse_decl_stmt();

    case ckx_token::type::token_function:
        return parse_func_stmt();

    case ckx_token::type::token_struct:
        return parse_struct_stmt();

    case ckx_token::type::token_variant:
        return parse_variant_stmt();

    case ckx_token::type::token_ckx:
        return parse_ckx_block();

    // Module manage system still designing.
    // We will have C-like preprocessors before creating a powerful
    // module manage system.
    Q_ON_HOLD(case ckx_token::type::token_import:)
    Q_ON_HOLD(case ckx_token::type::token_export:)

    case ckx_token::type::token_identifier:
        if ( is_typename(current_token()) )
            return parse_decl_stmt();

    default:
        Q_ON_HOLD( issue_error(current_token(), "...") )
        ;
    }
}

template <typename CkxTokenStream>
ckx_ast_stmt*
ckx_parser_impl<CkxTokenStream>::parse_stmt()
{
    switch ( current_token()->token_type )
    {
    case ckx_token::type::token_int8:
    case ckx_token::type::token_int16:
    case ckx_token::type::token_int32:
    case ckx_token::type::token_int64:
    case ckx_token::type::token_uint8:
    case ckx_token::type::token_uint16:
    case ckx_token::type::token_uint32:
    case ckx_token::type::token_uint64:
    case ckx_token::type::token_char:
    case ckx_token::type::token_real32:
    case ckx_token::type::token_real64:
        return parse_decl_stmt();

    case ckx_token::type::token_identifier:
        if ( current_env->lookup_type(*(current_token()->v.p_str))
             != nullptr )
            return parse_decl_stmt();
        else
            return parse_expr_stmt();

    case ckx_token::type::token_add:
    case ckx_token::type::token_sub:
    case ckx_token::type::token_mul:
    case ckx_token::type::token_bit_and:
    case ckx_token::type::token_bit_not:
    case ckx_token::type::token_logic_not:

    case ckx_token::type::token_static_cast:
    case ckx_token::type::token_reinterpret_cast:
    case ckx_token::type::token_const_cast:
    case ckx_token::type::token_ckx_cast:
        return parse_expr_stmt();

    case ckx_token::type::token_if:
        return parse_if_stmt();

    case ckx_token::type::token_while:
        return parse_while_stmt();

    case ckx_token::type::token_do:
        return parse_do_while_stmt();

    case ckx_token::type::token_for:
        return parse_for_stmt();

    case ckx_token::type::token_break:
        return parse_break_stmt();

    case ckx_token::type::token_continue:
        return parse_continue_stmt();

    case ckx_token::type::token_return:
        return parse_return_stmt();

    case ckx_token::type::token_lbrace:
        return parse_compound_stmt();

    case ckx_token::type::token_semicolon:
        Q_ON_HOLD("warning : empty declaration not permitted")

    default:
        Q_ON_HOLD( issue_error(current_token(), "...") )
        ;
    }
}

template <typename CkxTokenStream>
ckx_ast_decl_stmt*
ckx_parser_impl<CkxTokenStream>::parse_decl_stmt()
{
    ckx_ast_decl_stmt* ret = new ckx_ast_decl_stmt(current_token());

    saber_ptr<ckx_type> type = parse_type();
    while (1)
    {
        if ( !expect(ckx_token::type::token_identifier) )
            Q_ON_HOLD("Error recover");

        saber_ptr<ckx_token> token = current_token();
        ckx_var_entry *entry = new ckx_var_entry(type);
        ckx_ast_expr *init = nullptr;
        if ( expect_n_eat(ckx_token::type::token_assign) )
            init = parse_expr();

        env()->add_new_var(*(token->v.p_str), type);
        ret->add_decl(new ckx_ast_init_decl(token, entry, init));

        if ( !expect_n_eat(ckx_token::type::token_comma) ) break;
    }

    if ( !expect_n_eat(ckx_token::type::token_semicolon) )
        Q_ON_HOLD("Error recover");
    return ret;
}

template <typename CkxTokenStream>
saber_ptr<ckx_type>
ckx_parser_impl<CkxTokenStream>::parse_type()
{
    saber_ptr<ckx_type> type;
    if (current_token()->token_type == ckx_token::type::token_identifier)
    {
        type = env()->lookup_var( *(current_token()->v.p_str) );
    }
    else
    {
        type = ckx_type_helper::get_type( current_token()->token_type );
    }
    move2_next_token();

    while (1)
    {
        switch ( current_token() )
        {
        case ckx_token::type::token_const:
            type = ckx_type_helper::qual_const(type); break;

        case ckx_token::type::token_mul:
            type = ckx_type_helper::pointer_to(type); break;

        default:
            return type;
        }
    }
}



template <typename CkxTokenStream>
inline saber_ptr<ckx_token>
ckx_parser_impl<CkxTokenStream>::current_token()
{
    return stream()[0];
}

template <typename CkxTokenStream>
inline saber_ptr<ckx_token>
ckx_parser_impl<CkxTokenStream>::peek_next_token()
{
    return stream()[1];
}

template <typename CkxTokenStream>
inline void
ckx_parser_impl<CkxTokenStream>::move2_next_token()
{
    stream().operator ++();
}

template <typename CkxTokenStream>
inline bool
ckx_parser_impl<CkxTokenStream>::expect_n_eat(ckx_token::type _token_type)
{
    return current_token()->token_type == _token_type ?
                (move2_next_token(), true) : false;
}

template <typename CkxTokenStream>
inline bool
ckx_parser_impl<CkxTokenStream>::expect(ckx_token::type _token_type)
{
    return (current_token()->token_type == _token_type);
}

template <typename CkxTokenStream>
inline ckx_env_table*
ckx_parser_impl<CkxTokenStream>::env()
{
    return current_env;
}

template <typename CkxTokenStream>
inline void
ckx_parser_impl<CkxTokenStream>::enter_scope(ckx_env_table *_new_scope_env)
{
    current_env = _new_scope_env;
}

template <typename CkxTokenStream>
inline void
ckx_parser_impl<CkxTokenStream>::leave_scope()
{
    current_env = current_env->get_parent();
}

template <typename CkxTokenStream>
inline ckx_token_stream&
Q_ON_HOLD(CkxTokenStream&)
ckx_parser_impl<CkxTokenStream>::stream()
{
    return *(token_stream.get());
}

template <typename CkxTokenStream>
bool
ckx_parser_impl<CkxTokenStream>::is_typename(saber_ptr<ckx_token> _token) const
{
    return env()->lookup_type(_token->v.p_str);
}

} // namespace detail

} // namespace ckx

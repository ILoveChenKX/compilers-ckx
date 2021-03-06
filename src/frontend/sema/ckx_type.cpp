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

#include "frontend/sema/ckx_type.hpp"
#include "saber/unordered_map.hpp"
#include "saber/c8assert.hpp"

namespace ckx
{

ckx_type::ckx_type(category _category) :
    type_category(_category),
    qual(0)
{}

bool ckx_type::equal_to(ckx_type *_another) const
{
    return get_qual_bits() == _another->get_qual_bits()
           && this->equal_to_no_cvr(_another);
}

ckx_type::~ckx_type() {}

ckx_type::category
ckx_type::get_category() const
{
    return type_category;
}

bool ckx_type::is_basic() const
{
    return get_category() >= category::type_vi8
           && get_category() <= category::type_void;
}

bool ckx_type::is_scalar() const
{
    return is_basic() || is_pointer();
}

bool ckx_type::is_signed() const
{
    return get_category() >= category::type_vi8
           && get_category() <= category::type_vi64;
}

bool ckx_type::is_unsigned() const
{
    return get_category() >= category::type_vu8
           && get_category() <= category::type_vu64;
}

bool ckx_type::is_integral() const
{
    return is_signed() || is_unsigned();
}

bool ckx_type::is_floating() const
{
    return get_category() == category::type_vr32
           || get_category() == category::type_vr64;
}

bool ckx_type::is_numeric() const
{
    return is_integral() || is_floating();
}

bool ckx_type::is_bool() const
{
    return get_category() == category::type_vbool;
}

bool ckx_type::is_pointer() const
{
    return get_category() == ckx_type::category::type_pointer
            || get_category() == ckx_type::category::type_vnullptr_t;
}

bool ckx_type::is_nullptr() const
{
    return get_category() == ckx_type::category::type_vnullptr_t;
}

bool ckx_type::is_function() const
{
    return get_category() == ckx_type::category::type_function;
}

bool ckx_type::is_record() const
{
    return get_category() == ckx_type::category::type_struct
            || get_category() == ckx_type::category::type_variant;
}

bool ckx_type::is_enum() const
{
    return get_category() == ckx_type::category::type_enum;
}

bool ckx_type::is_alias() const
{
    return get_category() == ckx_type::category::type_alias;
}

bool ckx_type::is_const() const
{
    return qual & qual_const;
}

bool ckx_type::is_volatile() const
{
    return qual & qual_volatile;
}

bool ckx_type::is_restrict() const
{
    return qual & qual_volatile;
}

void ckx_type::add_const()
{
    qual |= qual_const;
}

void ckx_type::add_volatile()
{
    qual |= qual_volatile;
}

void ckx_type::add_restrict()
{
    qual |= qual_restrict;
}

void ckx_type::remove_const()
{
    qual &=  ~qual_const;
}

void ckx_type::remove_volatile()
{
    qual &=  ~qual_volatile;
}

void ckx_type::remove_restrict()
{
    qual &=  ~qual_restrict;
}

unsigned char ckx_type::get_qual_bits() const
{
    return qual;
}

void ckx_type::from_qual_bits(unsigned char _qual_bits)
{
    qual = _qual_bits;
}

bool ckx_type::more_qual_than(unsigned char _another_qual_bits) const
{
    return (qual & _another_qual_bits) == _another_qual_bits;
}


ckx_basic_type::ckx_basic_type(category _basic_category) :
    ckx_type(_basic_category)
{ C8ASSERT(this->is_basic()); }

bool ckx_basic_type::equal_to_no_cvr(ckx_type *_another) const
{
    return get_category() == _another->get_category();
}


ckx_struct_type::ckx_struct_type(saber_string_view _struct_name) :
    ckx_type(ckx_type::category::type_struct),
    struct_name(_struct_name) {}

bool ckx_struct_type::equal_to_no_cvr(ckx_type *_another) const
{
    if (_another->get_category() != category::type_struct)
        return false;

    ckx_struct_type *struct_type = static_cast<ckx_struct_type*>(_another);
    return this->struct_name == struct_type->struct_name;
}

saber_string_view ckx_struct_type::get_name() const
{
    return struct_name;
}

ckx_struct_type::add_status
ckx_struct_type::add_field(saber_string_view _name, ckx_type* _type)
{
    for (field& x : fields)
        if (x.name == _name)
            return add_status::add_duplicate;

    fields.emplace_back(saber::move(_name), _type);
    return add_status::add_success;
}

saber::vector<ckx_struct_type::field>& ckx_struct_type::get_fields()
{
    return fields;
}


ckx_variant_type::ckx_variant_type(saber_string_view _variant_name) :
    ckx_type(ckx_type::category::type_variant),
    variant_name(_variant_name)
{}

bool ckx_variant_type::equal_to_no_cvr(ckx_type *_another) const
{
    if (_another->get_category() != category::type_variant)
        return false;

    ckx_variant_type *variant_type = static_cast<ckx_variant_type*>(_another);
    return this->variant_name == variant_type->variant_name;
}

saber_string_view
ckx_variant_type::get_name() const
{
    return variant_name;
}

ckx_variant_type::add_status
ckx_variant_type::add_field(saber_string_view _name, ckx_type* _type)
{
    for (field& x : fields)
        if (x.name == _name)
            return add_status::add_duplicate;

    fields.emplace_back(saber::move(_name), _type);
    return add_status::add_success;
}


ckx_enum_type::ckx_enum_type(saber_string_view _enum_name) :
    ckx_type(ckx_type::category::type_enum),
    enum_name(_enum_name)
{}

bool ckx_enum_type::equal_to_no_cvr(ckx_type *_another) const
{
    if (_another->get_category() != category::type_enum)
        return false;

    ckx_enum_type *enum_type = static_cast<ckx_enum_type*>(_another);
    return this->enum_name == enum_type->enum_name;
}

saber_string_view
ckx_enum_type::get_name() const
{
    return enum_name;
}

ckx_enum_type::add_status
ckx_enum_type::add_enumerator(saber_string_view _name, qint64 _value)
{
    for (enumerator& x : enumerators)
        if (x.name == _name)
            return add_status::add_duplicate;

    enumerators.emplace_back(_name, _value);
    return add_status::add_success;
}


ckx_func_type::ckx_func_type(
        ckx_type* _return_type,
        saber::vector<ckx_type*> &&_param_type_list) :
    ckx_type(ckx_type::category::type_function),
    return_type(saber::move(_return_type)),
    param_type_list(saber::move(_param_type_list)) {}

bool ckx_func_type::equal_to_no_cvr(ckx_type *_another) const
{
    if (_another->get_category() != category::type_function)
        return false;

    ckx_func_type *func_type = static_cast<ckx_func_type*>(_another);
    return true
        && return_type->equal_to(func_type->get_return_type())
        && [&]()->bool {
            if (param_type_list.size() != func_type->param_type_list.size())
                return false;
            for (int i = 0; i < param_type_list.size(); i++)
                if (!param_type_list[i]->equal_to(
                    func_type->param_type_list[i]))
                    return false;
            return true;
        }();
}

ckx_type*
ckx_func_type::get_return_type()
{
    return return_type;
}

const saber::vector<ckx_type*>&
ckx_func_type::get_param_type_list()
{
    return param_type_list;
}


ckx_pointer_type::ckx_pointer_type(ckx_type* _target) :
    ckx_type(ckx_type::category::type_pointer),
    target(_target) {}

bool ckx_pointer_type::equal_to_no_cvr(ckx_type *_another) const
{
    if (_another->get_category() != category::type_pointer)
        return false;

    ckx_pointer_type *pointer_type = static_cast<ckx_pointer_type*>(_another);
    return this->target->equal_to(pointer_type->get_pointee());
}

ckx_type*
ckx_pointer_type::get_pointee()
{
    return target;
}


ckx_array_type::ckx_array_type(ckx_type* _element) :
    ckx_type(ckx_type::category::type_array),
    element_type(_element)
{}

/// @note array is deprecated. it will be removed soon.
bool ckx_array_type::equal_to_no_cvr(ckx_type *_another) const
{
    return false;
}

ckx_type* ckx_array_type::get_element_type()
{
    return element_type;
}


ckx_type_alias::ckx_type_alias(ckx_type *_origin) :
    ckx_type(ckx_type::category::type_alias),
    origin(_origin)
{}

bool ckx_type_alias::equal_to_no_cvr(ckx_type *_another) const
{
    ckx_type *cannonical_type =
        _another->is_alias() ?
            static_cast<ckx_type_alias*>(_another)->get_aliasee() : _another;

    return this->origin->equal_to(cannonical_type);
}

ckx_type* ckx_type_alias::get_aliasee()
{
    return origin;
}


ckx_type*
ckx_type_helper::get_type_by_token(ckx_token::type _basic_type_token)
{
    switch ( _basic_type_token )
    {
    case ckx_token::type::tk_vi8:        return get_vi8_type();
    case ckx_token::type::tk_vi16:       return get_vi16_type();
    case ckx_token::type::tk_vi32:       return get_vi32_type();
    case ckx_token::type::tk_vi64:       return get_vi64_type();
    case ckx_token::type::tk_vu8:        return get_vu8_type();
    case ckx_token::type::tk_vu16:       return get_vu16_type();
    case ckx_token::type::tk_vu32:       return get_vu32_type();
    case ckx_token::type::tk_vu64:       return get_vu64_type();
    case ckx_token::type::tk_vch:        return get_vch_type();
    case ckx_token::type::tk_vr32:       return get_vr32_type();
    case ckx_token::type::tk_vr64:       return get_vr64_type();
    case ckx_token::type::tk_vbool:      return get_vbool_type();
    case ckx_token::type::tk_vnullptr_t: return get_vnullptr_type();
    case ckx_token::type::tk_void:       return get_void_type();
    default: C8ASSERT(false);
    }
}

ckx_type *ckx_type_helper::get_basic_type(ckx_type::category _basic_type_categ)
{
    switch (_basic_type_categ)
    {
    case ckx_type::category::type_vi8:        return get_vi8_type();
    case ckx_type::category::type_vi16:       return get_vi16_type();
    case ckx_type::category::type_vi32:       return get_vi32_type();
    case ckx_type::category::type_vi64:       return get_vi64_type();
    case ckx_type::category::type_vu8:        return get_vu8_type();
    case ckx_type::category::type_vu16:       return get_vu16_type();
    case ckx_type::category::type_vu32:       return get_vu32_type();
    case ckx_type::category::type_vu64:       return get_vu64_type();
    case ckx_type::category::type_vch:        return get_vch_type();
    case ckx_type::category::type_vr32:       return get_vr32_type();
    case ckx_type::category::type_vr64:       return get_vr64_type();
    case ckx_type::category::type_vbool:      return get_vbool_type();
    case ckx_type::category::type_vnullptr_t: return get_vnullptr_type();
    case ckx_type::category::type_void:       return get_void_type();
    default: C8ASSERT(false);
    }
}



thread_local saber::object_pool<ckx_struct_type>
ckx_type_helper::struct_type_pool;
thread_local saber::object_pool<ckx_variant_type>
ckx_type_helper::variant_type_pool;
thread_local saber::object_pool<ckx_enum_type>
ckx_type_helper::enum_type_pool;
thread_local saber::object_pool<ckx_type_alias>
ckx_type_helper::type_alias_pool;
thread_local saber::object_pool<ckx_func_type>
ckx_type_helper::func_type_pool;
thread_local saber::object_pool<ckx_array_type>
ckx_type_helper::array_type_pool;
thread_local saber::object_pool<ckx_pointer_type>
ckx_type_helper::pointer_type_pool;

ckx_struct_type*
ckx_type_helper::create_struct_type(saber_string_view _name)
{
    return struct_type_pool.store(ckx_struct_type(_name));
}

ckx_variant_type*
ckx_type_helper::create_variant_type(saber_string_view _name)
{
    return variant_type_pool.store(ckx_variant_type(_name));
}

ckx_enum_type*
ckx_type_helper::create_enum_type(saber_string_view _name)
{
    return enum_type_pool.store(ckx_enum_type(_name));
}

ckx_type_alias*
ckx_type_helper::create_alias(ckx_type *_type)
{
    return type_alias_pool.store(ckx_type_alias(_type));
}

ckx_func_type*
ckx_type_helper::create_func_type(ckx_type *_ret_type,
                                  saber::vector<ckx_type*> &&_params)
{
    return func_type_pool.store(ckx_func_type(_ret_type, saber::move(_params)));
}

ckx_array_type* ckx_type_helper::create_array_type(ckx_type *_elem_type)
{
    return array_type_pool.store(ckx_array_type(_elem_type));
}

ckx_pointer_type *ckx_type_helper::pointer_to(ckx_type* _base)
{
    return pointer_type_pool.store(ckx_pointer_type(_base));
}

ckx_type* ckx_type_helper::qual_const(ckx_type* _base)
{
    _base->add_const(); return _base;
}

ckx_type* ckx_type_helper::get_vi8_type()
{
    static ckx_basic_type ret(ckx_type::category::type_vi8);
    return &ret;
}

ckx_type* ckx_type_helper::get_vi16_type()
{
    static ckx_basic_type ret(ckx_type::category::type_vi16);
    return &ret;
}

ckx_type* ckx_type_helper::get_vi32_type()
{
    static ckx_basic_type ret(ckx_type::category::type_vi32);
    return &ret;
}

ckx_type* ckx_type_helper::get_vi64_type()
{
    static ckx_basic_type ret(ckx_type::category::type_vi64);
    return &ret;
}

ckx_type* ckx_type_helper::get_vu8_type()
{
    static ckx_basic_type ret(ckx_type::category::type_vu8);
    return &ret;
}

ckx_type* ckx_type_helper::get_vu16_type()
{
    static ckx_basic_type ret(ckx_type::category::type_vu16);
    return &ret;
}

ckx_type* ckx_type_helper::get_vu32_type()
{
    static ckx_basic_type ret(ckx_type::category::type_vu32);
    return &ret;
}

ckx_type* ckx_type_helper::get_vu64_type()
{
    static ckx_basic_type ret(ckx_type::category::type_vu64);
    return &ret;
}

ckx_type* ckx_type_helper::get_vch_type()
{
    static ckx_basic_type ret(ckx_type::category::type_vch);
    return &ret;
}

ckx_type* ckx_type_helper::get_vr32_type()
{
    static ckx_basic_type ret(ckx_type::category::type_vr32);
    return &ret;
}

ckx_type* ckx_type_helper::get_vr64_type()
{
    static ckx_basic_type ret(ckx_type::category::type_vr64);
    return &ret;
}

ckx_type *ckx_type_helper::get_vbool_type()
{
    static ckx_basic_type ret(ckx_type::category::type_vbool);
    return &ret;
}

ckx_type *ckx_type_helper::get_vnullptr_type()
{
    static ckx_basic_type ret(ckx_type::category::type_vnullptr_t);
    return &ret;
}

ckx_type* ckx_type_helper::get_void_type()
{
    static ckx_basic_type ret(ckx_type::category::type_void);
    return &ret;
}

qint8 ckx_type_helper::rank_of(ckx_type::category _type_category)
{
    return static_cast<qint8>(_type_category);
}

bool ckx_type_helper::can_implicit_cast(ckx_type *_from, ckx_type *_dest)
{
    if (_from->equal_to_no_cvr(_dest)
        && _dest->more_qual_than(_from->get_qual_bits()))
        return true;

    if (_from->is_signed() && _dest->is_signed())
        if (rank_of(_from->get_category()) < rank_of(_dest->get_category()))
            return true;

    if (_from->is_unsigned() && _dest->is_unsigned())
        if (rank_of(_from->get_category()) < rank_of(_dest->get_category()))
            return true;

    if (_from->is_floating() && _dest->is_floating())
        if (rank_of(_from->get_category()) < rank_of(_dest->get_category()))
            return true;

    if ((_from->is_pointer() && _dest->is_nullptr())
        || (_from->is_nullptr() && _dest->is_pointer()))
            return true;

    return false;
}

bool ckx_type_helper::can_static_cast(ckx_type *_from, ckx_type *_dest)
{
    if (_dest->more_qual_than(_from->get_qual_bits()))
    {
        if (_from->is_numeric() && _dest->is_numeric())
            return true;

        if ((_from->is_numeric() && _dest->is_bool())
            || (_from->is_basic() && _dest->is_numeric()))
            return true;

        if ((_from->is_integral() && _dest->is_enum())
            || (_from->is_enum() && _dest->is_integral()))
            return true;
    }
    return false;
}

bool ckx_type_helper::can_reinterpret_cast(ckx_type *_from,
                                           ckx_type *_dest)
{
    return _from->more_qual_than(_dest->get_qual_bits())
           && _from->is_pointer()
           && _dest->is_pointer();
}

bool ckx_type_helper::can_const_cast(ckx_type *_from, ckx_type *_dest)
{
    return _from->equal_to_no_cvr(_dest);
}

ckx_type*
ckx_type_helper::common_numeric_type(ckx_type *_ty1, ckx_type *_ty2)
{
    C8ASSERT(_ty1->is_numeric());
    C8ASSERT(_ty2->is_numeric());

    if ((_ty1->is_signed() && _ty2->is_signed())
        || (_ty1->is_unsigned() && _ty2->is_unsigned())
        || (_ty1->is_floating() && _ty2->is_floating()) )
        return rank_of(_ty1->get_category()) > rank_of(_ty2->get_category()) ?
               _ty1 : _ty2;
    return nullptr;
}

ckx_type_helper::function_relation
ckx_type_helper::predicate_function_relation(ckx_func_type *_f1,
                                             ckx_func_type *_f2)
{
    if (_f1->param_type_list.size() != _f2->param_type_list.size())
        return function_relation::fr_overload;
    for (qsizet i = 0; i < _f1->param_type_list.size(); i++)
    {
        if (!_f1->param_type_list[i]->equal_to(_f2->param_type_list[i]))
            return function_relation::fr_overload;
    }

    return function_relation::fr_duplicate;
}

} // namespace ckx

﻿#ifndef GGLEX
#define GGLEX(X, Y)
#endif

GGLEX("vi8", vi8)
GGLEX("vi16", vi16)
GGLEX("vi32", vi32)
GGLEX("vi64", vi64)
GGLEX("vu8", vu8)
GGLEX("vu16", vu16)
GGLEX("vu32", vu32)
GGLEX("vu64", vu64)
GGLEX("vch", vch)
GGLEX("vr32", vr32)
GGLEX("vr64", vr64)
GGLEX("vnptr_t", vnullptr_t)
GGLEX("vb1", vbool)
GGLEX("void", void)
GGLEX("fn", function)
GGLEX("struct", struct)
GGLEX("variant", variant)
GGLEX("enum", enum)
GGLEX("alias", alias)
GGLEX("const", const)
GGLEX("if", if)
GGLEX("else", else)
GGLEX("for", for)
GGLEX("while", while)
GGLEX("do", do)
GGLEX("switch", switch)
GGLEX("case", case)
GGLEX("return", return)
GGLEX("break", break)
GGLEX("continue", continue)
GGLEX("operator", operator)
GGLEX("nullptr", nullptr)
GGLEX("true", true)
GGLEX("false", false)
GGLEX("static_cast", static_cast)
GGLEX("reinterpret_cast", reinterpret_cast)
GGLEX("const_cast", const_cast)
GGLEX("ckx_cast", ckx_cast)
GGLEX("sizeof", sizeof)

#ifndef GG2LEX
#define GG2LEX(X, Y)
#endif

GG2LEX("=", assign)
GG2LEX("+", add)
GG2LEX("-", sub)
GG2LEX("*", mul)
GG2LEX("/", div)
GG2LEX("%", mod)
GG2LEX("++", inc)
GG2LEX("--", dec)
GG2LEX("+=", add_assign)
GG2LEX("-=", sub_assign)
GG2LEX("*=", mul_assign)
GG2LEX("/=", div_assign)
GG2LEX("%=", mod_assign)
GG2LEX("&", bit_and)
GG2LEX("|", bit_or)
GG2LEX("~", bit_not)
GG2LEX("^", bit_xor)
GG2LEX("&&", logic_and)
GG2LEX("||", logic_or)
GG2LEX("!", logic_not)
GG2LEX("^^", logic_xor)
GG2LEX("<", lt)
GG2LEX("==", eq)
GG2LEX(">", gt)
GG2LEX("<=", leq)
GG2LEX(">=", geq)
GG2LEX("!=", neq)
GG2LEX(":", colon)
GG2LEX("::", scope)
GG2LEX(";", semicolon)
GG2LEX(",", comma)
GG2LEX(".", dot)
GG2LEX("{", lbrace)
GG2LEX("}", rbrace)
GG2LEX("[", lbracket)
GG2LEX("]", rbracket)
GG2LEX("[]", arr)
GG2LEX("(", lparen)
GG2LEX(")", rparen)
GG2LEX("?", ques)

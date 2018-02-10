﻿#include "frontend/lex/ckx_token_stream.hpp"
#include "frontend/parse/ckx_parser.hpp"
#include "frontend/sema/ckx_sema.hpp"
#include "saber/string_pool.hpp"

saber_string str =
R"cc(

struct s { vi8 a; vi16 b, c; s* d; }

fn one(): vi32 {
    return 1;
}

fn fuck(): s {
    s s1;
    s1.a = 0;
    s1.b = s1.c = 0;
    return s1;
}

fn print(vi16 number): vi16;

fn main(vi8 argc, vi8** argv): vi32 {
    vi64 number;
    number = one();
    s shit;
    shit = fuck();
    print(fuck().c);
    return 0;
}

)cc";

int main()
{
    using namespace ckx;
    using namespace we;

    we_test_filereader reader1(saber::move(str));
    ckx_token_stream stream1(reader1);

    ckx_parser parser1;
    ckx_parser::parse_result parse_result = parser1.parse(&stream1);

    ckx_sema_engine engine;
    parse_result.trans_unit->accept(engine);

    we_fp_writer writer(stdout);
    engine.test_print(writer);
}

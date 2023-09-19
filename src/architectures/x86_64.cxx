/*
 * Created Date: Sunday September 17th 2023
 * Author: Lilith
 * -----
 * Last Modified: Sunday September 17th 2023 4:54:36 am
 * Modified By: Lilith (definitelynotagirl115169@gmail.com)
 * -----
 * Copyright (c) 2023-2023 DefinitelyNotAGirl@github
 * 
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#include <compiler.h>
#include <codegen.h>

#include "x86_64/mov.cxx"
#include "x86_64/arith.cxx"
#include "x86_64/pcall.cxx"
#include "x86_64/symbols.cxx"
#include "x86_64/cjmp.cxx"

#define constructor __attribute__ ((constructor))

namespace x86_64
{
    void activate()
    {
        ARCH_REGISTER_MASK = 0b0000000000000000000000000000000000000000000000000000000000000001;
    }

    constructor void init()
    {
        arch* a = new arch;
        a->name = "x86-64";
        //add & sub
        a->addRR = &x86_64::add;
        a->addUR = &x86_64::add;
        a->addVVV = &x86_64::add;
        a->inc = &x86_64::inc;
        a->subRR = &x86_64::sub;
        a->subUR = &x86_64::sub;
        a->subVVV = &x86_64::sub;
        a->dec = &x86_64::dec;
        //misc
        a->putComment = &x86_64::putComment;
        a->CodePlaceSymbol = &x86_64::CodePlaceSymbol;
        a->jmp = &x86_64::jmp;
        a->activate = &x86_64::activate;
        a->primitiveCall = &x86_64::primitiveCall;
        //mul,div&mod
        a->mulVVV = &x86_64::mul;
        a->divVVV = &x86_64::div;
        a->modVVV = &x86_64::mod;
        //mov
        a->movRR = &x86_64::mov;
        a->movUR = &x86_64::mov;
        a->movUL = &x86_64::mov;
        a->movLL = &x86_64::mov;
        a->movLR = &x86_64::mov;
        a->movRL = &x86_64::mov;
        a->movVR = &x86_64::mov;
        a->movRV = &x86_64::mov;
        a->movVV = &x86_64::mov;
        a->movLV = &x86_64::mov;
        a->movVL = &x86_64::mov;

        a->cmp = &x86_64::cmp;
        a->jnz = &x86_64::jnz;
        a->jne = &x86_64::jne;
        a->jno = &x86_64::jno;
        a->jns = &x86_64::jns;
        a->jns = &x86_64::jns;
        a->js = &x86_64::js;
        a->js = &x86_64::js;
        a->jz = &x86_64::jz;
        a->je = &x86_64::je;
        a->jo = &x86_64::jo;
        a->jb = &x86_64::jb;
        a->jnae = &x86_64::jnae;
        a->jc = &x86_64::jc;
        a->jnb = &x86_64::jnb;
        a->jae = &x86_64::jae;
        a->jnc = &x86_64::jnc;
        a->jbe = &x86_64::jbe;
        a->jna = &x86_64::jna;
        a->jnbe = &x86_64::jnbe;
        a->ja = &x86_64::ja;
        a->jnge = &x86_64::jnge;
        a->jl = &x86_64::jl;
        a->jge = &x86_64::jge;
        a->jnl = &x86_64::jnl;
        a->jle = &x86_64::jle;
        a->jng = &x86_64::jng;
        a->jnle = &x86_64::jnle;
        a->jg = &x86_64::jg;
        a->jp = &x86_64::jp;
        a->jpe = &x86_64::jpe;
        a->jnp = &x86_64::jnp;
        a->jpo = &x86_64::jpo;
        a->jcxz = &x86_64::jcxz;
        a->jecxz = &x86_64::jecxz;

        architectures.push_back(a);
    }
}
/*
 * Created Date: Wednesday August 2nd 2023
 * Author: Lilith
 * -----
 * Last Modified: Wednesday August 2nd 2023 5:15:44 am
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

u64_largeFunc_u64_u64_u64:
    push %rbx # save rbx as mandated by the SystemV amd64 ABI
    push %rsp # save rsp as mandated by the SystemV amd64 ABI
    push %rbp # save rbp as mandated by the SystemV amd64 ABI
    push %r12 # save r12 as mandated by the SystemV amd64 ABI
    push %r13 # save r13 as mandated by the SystemV amd64 ABI
    push %r14 # save r14 as mandated by the SystemV amd64 ABI
    push %r15 # save r15 as mandated by the SystemV amd64 ABI
    sub $16, %rsp
u64_largeFunc_u64_u64_u64__epilogue:
    add $16, %rsp
    pop %rbx # restore rbx as mandated by the SystemV amd64 ABI
    pop %rsp # restore rsp as mandated by the SystemV amd64 ABI
    pop %rbp # restore rbp as mandated by the SystemV amd64 ABI
    pop %r12 # restore r12 as mandated by the SystemV amd64 ABI
    pop %r13 # restore r13 as mandated by the SystemV amd64 ABI
    pop %r14 # restore r14 as mandated by the SystemV amd64 ABI
    pop %r15 # restore r15 as mandated by the SystemV amd64 ABI
    ret

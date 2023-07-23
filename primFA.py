#!/usr/bin/env python3
# -*- coding:utf-8 -*-
###
# Created Date: Sunday July 23rd 2023
# Author: Lilith
# -----
# Last Modified: Sunday July 23rd 2023 6:46:02 am
# Modified By: Lilith (definitelynotagirl115169@gmail.com)
# -----
# Copyright (c) 2023-2023 DefinitelyNotAGirl@github
# 
# Permission is hereby granted, free of charge, to any person
# obtaining a copy of this software and associated documentation
# files (the "Software"), to deal in the Software without
# restriction, including without limitation the rights to use, copy,
# modify, merge, publish, distribute, sublicense, and/or sell copies
# of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be
# included in all copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
# NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
# HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
# WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
# DEALINGS IN THE SOFTWARE.
###

bits = [
    ["8", "0"],
    ["16","1"],
    ["32","2"],
    ["64","3"]
]
ops=[
    # compare
    ["primitiveEqual","0x01"],
    ["primitiveNotEqual","0x02"],
    ["primitiveGreater","0x03"],
    ["primitiveGreaterEqual","0x04"],
    ["primitiveLess","0x05"],
    ["primitiveLessEqual","0x06"],
    ["primitiveNotZero","0x07"],
    ["primitiveEqualZero","0x08"],
    # shift
    ["primitiveShiftLeft","0x09"],
    ["primitiveShiftRight","0x0A"],
    # unary
    ["primitiveNegate","0x0B"],
    # binary
    ["primitiveAdd","0x0C"],
    ["primitiveSub","0x0D"],
    ["primitiveMul","0x0E"],
    ["primitiveDiv","0x0F"],
    ["primitiveOr","0x10"],
    ["primitiveAnd","0x11"],
    ["primitiveXor","0x12"],
    ["primitiveMod","0x13"],
    ["primitiveInc","0x15"],
    ["primitiveDec","0x16"],
    # binary in place
    ["primitiveAdd_A","0x0C"],
    ["primitiveSub_A","0x0D"],
    ["primitiveMul_A","0x0E"],
    ["primitiveDiv_A","0x0F"],
    ["primitiveOr_A","0x10"],
    ["primitiveAnd_A","0x11"],
    ["primitiveXor_A","0x12"],
    ["primitiveMod_A","0x13"]
]

for b in bits:
    print("//",b[0]," bits\n",sep='',end='')
    for op in ops:
        print("else if(cstrcmp(i->text,(codechar*)\"",op[0],"_",b[0],"\")){\n",sep='',end='')
        print("    SETBIT_06(attr);\n",sep='',end='')
        print("    *((uint8_t*)((&attr)+1)) &= ",op[1],";\n",sep='',end='')
        print("    *((uint8_t*)((&attr)+2)) &= 0x",b[1],b[1],";\n",sep='',end='')
        if(op[0].endswith("_A")):
            print("    SETBIT_07(attr);\n",sep='',end='')
        print("}",sep='',end='')
    for b2 in bits:
        print("else if(cstrcmp(i->text,(codechar*)\"primitiveAssign_",b[0],"_",b2[0],"\")){\n",sep='',end='')
        print("    SETBIT_06(attr);\n",sep='',end='')
        print("    *((uint8_t*)((&attr)+1)) &= 0x14;\n",sep='',end='')
        print("    *((uint8_t*)((&attr)+2)) &= 0x",b[1],b2[1],";\n",sep='',end='')
        print("}",sep='',end='')

print("\n",sep='',end='')
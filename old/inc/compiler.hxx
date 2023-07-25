/*
 * Created Date: Monday July 10th 2023
 * Author: Lilith
 * -----
 * Last Modified: Tuesday July 18th 2023 4:48:23 pm
 * Modified By: Lilith (definitelynotagirl115169@gmail.com)
 * -----
 * Copyright (c) 2023 DefinitelyNotAGirl@github
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

#pragma once

#include <common.h>
#include <vector>

extern std::list<std::string> dependencies;

enum class exprType : uint32_t {
    IMM,CALL,VAR
};
enum class defineType : uint8_t {
    INT,STRING
};

enum class langTypes : uint64_t {
    _void_,
    primitive64,
    primitive32,
    primitive16,
    primitive8,
    u64,
    u32,
    u16,
    u8,
    i64,
    i32,
    i16,
    i8,
    f64,
    f32
};//do not add more than 0x1000 types to this enum

//attributes layout:
// 0: macro
// 1: const
// 2: static
// 3: extern
// 4: volatile
// 5: constexpr
// 6: primitive
// 7: primitive-in-place
// 8-15: primitive operation
// 16-19: primitive bits left
// 20-23: primitive bits right
// all unspecified bits are unused and must be set to 0

//primitive ops enum:
// 0x00: invalid
// 0x01: Equal
// 0x02: Not Equal
// 0x03: Greater
// 0x04: Greater Equal
// 0x05: Less
// 0x06: Less Equal
// 0x07: Not Zero
// 0x08: Equal Zero
// 0x09: shift left
// 0x0A: shift right
// 0x0B: negate
// 0x0C: Add
// 0x0D: Sub
// 0x0E: Mul
// 0x0F: Div
// 0x10: Or
// 0x11: And
// 0x12: Xor
// 0x13: Mod
// 0x14: Assign
// 0x15: inc
// 0x16: dec

//primitive bits enum:
// 0x0: 8
// 0x1: 16
// 0x2: 32
// 0x3: 64
// 0x4: 128
// 0x5: 256
// 0x6: 512
// 0x7: 1024
// 0x8: 2048
// 0x9: 4096
// 0xA: 8192
// 0xB: 16384
// 0xC: 32768
// 0xD: 65536
// 0xE: 131072
// 0xF: 262144

struct expression
{
    exprType type;
} packed;

struct exprIMM
{
    exprType type;
    uint64_t vType;
    uint64_t value;
} packed;

struct exprCall
{
    exprType type;
    uint64_t func;
    expression** args;
} packed;

struct type;

struct variable
{
    codechar* name;
    type* dType;
    uint64_t attr;
} packed;

struct function
{
    codechar* name;
    variable* args;
    type* returnType;
    uint64_t attr;
} packed;

struct type
{
    codechar* name;
    function* functions;
    variable* members;
    uint64_t size;
} packed;

struct define
{
    codechar* name;
    defineType type;
} packed;

enum class litopType : uint8_t {
    FUNC,IMM
};
enum class arithType : uint8_t {
    ADD,SUB,MUL,DIV
};

struct litop
{
    codechar* name;
    litopType type;
} packed;
struct litopFunc
{
    codechar* name;
    litopType type;
    function* func;
} packed;
struct litopIMM
{
    codechar* name;
    litopType type;
    arithType op;
    bool rIsSigned;
    uint64_t rightHand;
} packed;

expression* resolveIMM(codechar* token);

extern std::vector<type*> types;
extern std::vector<function*> functions;

codechar* manglePseudoName(codechar* name);

struct scope
{
    scope* parent;
    std::vector<variable*> variables;
    std::vector<define*> defines;
    uint64_t subScope;
};

extern scope* currentScope;
extern std::vector<litop*> litops;

void popScope();
void pushScope();

expression* resolveExpression(std::string& token);

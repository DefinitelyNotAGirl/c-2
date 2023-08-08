/*
 * Created Date: Sunday July 30th 2023
 * Author: Lilith
 * -----
 * Last Modified: Sunday July 30th 2023 9:11:56 am
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

#include <class_variable.h>

__register__ registerID(std::string name)
{
    using enum __register__;

    if(name == "rax")
        return rax;
    else if(name == "rbx")
        return rbx;
    else if(name == "rcx")
        return rcx;
    else if(name == "rdx")
        return rdx;
    else if(name == "rsi")
        return rsi;
    else if(name == "rdi")
        return rdi;
    else if(name == "rbp")
        return rbp;
    else if(name == "rsp")
        return rsp;
    else if(name == "rip")
        return rip;
    else if(name == "r8")
        return r8;
    else if(name == "r9")
        return r9;
    else if(name == "r10")
        return r10;
    else if(name == "r11")
        return r11;
    else if(name == "r12")
        return r12;
    else if(name == "r13")
        return r13;
    else if(name == "r14")
        return r14;
    else if(name == "r15")
        return r15;
    else if(name == "cr0")
        return cr0;
    else if(name == "cr1")
        return cr1;
    else if(name == "cr2")
        return cr2;
    else if(name == "cr3")
        return cr3;
    else if(name == "cr4")
        return cr4;
    else if(name == "cr5")
        return cr5;
    else if(name == "cr6")
        return cr6;
    else if(name == "cr7")
        return cr7;
    else if(name == "cr8")
        return cr8;
    else if(name == "cr9")
        return cr9;
    else if(name == "cr10")
        return cr10;
    else if(name == "cr11")
        return cr11;
    else if(name == "cr12")
        return cr12;
    else if(name == "cr13")
        return cr13;
    else if(name == "cr14")
        return cr14;
    else if(name == "cr15")
        return cr15;
    else if(name == "EFER")
        return EFER;
    else if(name == "dr0")
        return DR0;
    else if(name == "dr1")
        return DR1;
    else if(name == "dr2")
        return DR2;
    else if(name == "dr3")
        return DR3;
    else if(name == "dr6")
        return DR6;
    else if(name == "dr7")
        return DR7;
    else if(name == "gdtr")
        return GDTR;
    else if(name == "idtr")
        return IDTR;
    else if(name == "ldtr")
        return LDTR;
    else if(name == "tr")
        return TR;
    else if(name == "cs")
        return CS;
    else if(name == "ds")
        return DS;
    else if(name == "ss")
        return SS;
    else if(name == "es")
        return ES;
    else if(name == "fs")
        return FS;
    else if(name == "gs")
        return GS;
    return invalid;
}

std::string registerNAME(__register__ reg)
{
    using enum __register__;
    
    switch(reg){
    case(rax):
        return "rax";
    case(rbx):
        return "rbx";
    case(rcx):
        return "rcx";
    case(rdx):
        return "rdx";
    case(rsi):
        return "rsi";
    case(rdi):
        return "rdi";
    case(rbp):
        return "rbp";
    case(rsp):
        return "rsp";
    case(rip):
        return "rip";
    case(r8):
        return "r8";
    case(r9):
        return "r9";
    case(r10):
        return "r10";
    case(r11):
        return "r11";
    case(r12):
        return "r12";
    case(r13):
        return "r13";
    case(r14):
        return "r14";
    case(r15):
        return "r15";
    case(cr0):
        return "cr0";
    case(cr1):
        return "cr1";
    case(cr2):
        return "cr2";
    case(cr3):
        return "cr3";
    case(cr4):
        return "cr4";
    case(cr5):
        return "cr5";
    case(cr6):
        return "cr6";
    case(cr7):
        return "cr7";
    case(cr8):
        return "cr8";
    case(cr9):
        return "cr9";
    case(cr10):
        return "cr10";
    case(cr11):
        return "cr11";
    case(cr12):
        return "cr12";
    case(cr13):
        return "cr13";
    case(cr14):
        return "cr14";
    case(cr15):
        return "cr15";
    case(EFER):
        return "EFER";
    case(DR0):
        return "DR0";
    case(DR1):
        return "DR1";
    case(DR2):
        return "DR2";
    case(DR3):
        return "DR3";
    case(DR6):
        return "DR6";
    case(DR7):
        return "DR7";
    case(GDTR):
        return "GDTR";
    case(IDTR):
        return "IDTR";
    case(LDTR):
        return "LDTR";
    case(TR):
        return "TR";
    case(CS):
        return "CS";
    case(DS):
        return "DS";
    case(SS):
        return "SS";
    case(ES):
        return "ES";
    case(FS):
        return "FS";
    case(GS):
        return "GS";
    }
    return "invalid";
}
/*
 * Created Date: Wednesday July 12th 2023
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

#include <common.h>
#include <options.h>
#include <util.h>

typedef char codechar;

static std::vector<token_t*> tokens;

std::string tokenToString(TokenType type)
{
    switch(type)
    {
    case TokenType::INVALID:
        return "INVALID";
    case TokenType::OPERATOR:
        return "OPERATOR";
    case TokenType::MISC:
        return "MISC";
    case TokenType::SCOPE:
        return "SCOPE";
    case TokenType::PA:
        return "PA";
    case TokenType::TERMINATOR:
        return "TERMINATOR";
    default:
        return "UNKNOWN";
    }
}

static token_t* token = nullptr;
static uint64_t mdata;
static uint64_t line = 0;
static uint64_t col = 0;
static uint64_t tlen = 0;
static char* file;
static codechar* str;

void newToken()
{
    tlen = 0;
    if(token!=nullptr){if(!(token->text == nullptr && token->type == TokenType::INVALID))
    {
        uint64_t len = strlen(token->text);
        if(len == 0)
            goto ntl0;

        std::cout << "\n###############\n";
        std::cout << "ttext: " <<std::hex<< (void*)token->text << std::endl;
        std::cout << "ttext size: " <<std::hex<< len << std::endl;
        if(len > 0)
            token->text = realloc(token->text,len+1);
        std::cout << "tlist size: " <<std::dec<< tokens.size() << std::endl;
        tokens.push_back(token);
        if(options::dprintTokens)
            std::cout << "Token: " <<std::dec<< token->line << ":" << token->column << ":" << tokenToString(token->type) << ":" << token->text << std::endl;
    }}
    //allocate new token
    token = (token_t*)calloc(1,sizeof(token_t)+1);
    token->line = line;
    token->column = col;
    token->file = file;
    token->text = (codechar*)calloc(1,sizeof(codechar)*((uint64_t)mdata-(uint64_t)str)+1);
    ntl0:;
}

std::vector<token_t*> tokenize(codechar* __str)
{
    std::cout << "\033[32m";
    tokens.clear();
    line = 1;
    col = 0;
    tlen = 0;
    str = __str;
    mdata = strlen(str)+str;
    newToken();

    bool isSingleQuote = false;
    bool isDoubleQuote = false;
    while(*str!=0x00)
    {
        if(isSingleQuote)
            goto singleQuoteSwitch;
        if (isDoubleQuote)
            goto doubleQuoteSwitch;
        switch(*str)
        {
            //handle operators
            case('+'):
                newToken();
                token->type = TokenType::OPERATOR;
                token->text[tlen++] = '+';
                switch(*(str+(sizeof(codechar)*1)))
                {
                    case('+'):
                        token->text[tlen++] = '+';
                        str+=sizeof(codechar);
                        //increment operator (++)
                        break;
                    case('='):
                        token->text[tlen++] = '=';
                        str+=sizeof(codechar);
                        //add-assign operator (+=)
                        break;
                    default:break;//normal add operator
                }
                newToken();
                break;
            case('-'):
                newToken();
                token->type = TokenType::OPERATOR;
                token->text[tlen++] = '-';
                switch(*(str+(sizeof(codechar)*1)))
                {
                    case('-'):
                        token->text[tlen++] = '-';
                        str+=sizeof(codechar);
                        //decrement operator (--)
                        break;
                    case('='):
                        token->text[tlen++] = '=';
                        str+=sizeof(codechar);
                        //subtract-assign operator (-=)
                        break;
                    default:break;//normal subtract operator
                }
                newToken();
                break;
            case('*'):
                newToken();
                token->type = TokenType::OPERATOR;
                token->text[tlen++] = '*';
                switch(*(str+(sizeof(codechar)*1)))
                {
                    case('=')://multiply-assign operator (*=)
                        token->text[tlen++] = '=';
                        str+=sizeof(codechar);
                        break;
                    default:break;//normal multiply operator
                }
                newToken();
                break;
            case('/'):
                switch(*(str+(sizeof(codechar)*1)))
                {
                    case('=')://divide-assign operator (/=)
                        newToken();
                        token->type = TokenType::OPERATOR;
                        token->text[tlen++] = '/';
                        token->text[tlen++] = '=';
                        str+=sizeof(codechar);
                        break;
                    //comments
                    case('/'):
                        while(*str!=0x00 && *str!=0x0a && *str!=0x0d)
                        {
                            str+=sizeof(codechar);
                        }
                        line++;
                        break;
                    case('*'):
                        std::cout << "block comment started" << std::endl;
                        while(*str!=0x00)
                        {
                            switch(*str)
                            {
                                case('*'):
                                    if(*(str+(sizeof(codechar)*1)) == '/')
                                        goto breakBlockComment;
                                    break;
                                case('\n'):
                                    line++;
                                    break;
                            }
                            str+=sizeof(codechar);
                        }
                        breakBlockComment:;
                        std::cout << "block comment ended" << std::endl;
                        std::cout << "end char: " << std::hex << (uint64_t)*str << std::endl;
                        str+=sizeof(codechar);
                        break;
                    default:
                        newToken();
                        token->type = TokenType::OPERATOR;
                        token->text[tlen++] = '/';
                        newToken();
                        break;//normal division operator
                }
                break;
            case('^'):
                newToken();
                token->type = TokenType::OPERATOR;
                token->text[tlen++] = '^';
                switch(*(str+(sizeof(codechar)*1)))
                {
                    case('=')://exp-assign operator (^=)
                        token->text[tlen++] = '=';
                        str+=sizeof(codechar);
                        break;
                    default:break;//normal exp operator
                }
                newToken();
                break;
            case('='):
                newToken();
                token->type = TokenType::OPERATOR;
                token->text[tlen++] = '=';
                switch(*(str+(sizeof(codechar)*1)))
                {
                    case('=')://compare operator (==)
                        token->text[tlen++] = '=';
                        str+=sizeof(codechar);
                        switch(*(str+(sizeof(codechar)*2)))
                        {
                            case('=')://extra compare operator (===)
                                token->text[tlen++] = '=';
                                str+=sizeof(codechar);
                                break;
                            default:break;
                                break;
                        }
                        break;
                    default:break;//assign operator
                }
                newToken();
                break;
            case('>'):
                switch(*(str+(sizeof(codechar)*1)))
                {
                    case('=')://greater-or-equal operator (>=)
                        newToken();
                        token->type = TokenType::OPERATOR;
                        token->text[tlen++] = '>';
                        token->text[tlen++] = '=';
                        newToken();
                        str+=sizeof(codechar);
                        break;
                    case('>')://bitshift right operator (>>)
                        newToken();
                        token->type = TokenType::OPERATOR;
                        token->text[tlen++] = '>';
                        token->text[tlen++] = '>';
                        switch(*(str+(sizeof(codechar)*2)))
                        {
                            case('=')://bitshift-right-assign operator (>>=)
                                token->text[tlen++] = '=';
                                str+=sizeof(codechar);
                                break;
                            default:break;//normal bitshift-right operator
                        }
                        newToken();
                        break;
                    default:
                        if(tokens.size()>1 && cstrcmp("#include",tokens[tokens.size()-2]->text))
                        {
                            token->text[tlen++] = '>';
                        }
                        else
                        {
                            std::cout << "prev token text: " << tokens[tokens.size()-1]->text << std::endl;
                            newToken();
                            token->type = TokenType::OPERATOR;
                            token->text[tlen++] = '>';
                            newToken();
                        }
                        break;//normal greater operator
                }
                break;
            case('<'):
                switch(*(str+(sizeof(codechar)*1)))
                {
                    case('=')://greater-or-equal operator (>=)
                        newToken();
                        token->type = TokenType::OPERATOR;
                        token->text[tlen++] = '<';
                        token->text[tlen++] = '=';
                        newToken();
                        str+=sizeof(codechar);
                        break;
                    case('<')://bitshift right operator (>>)
                        newToken();
                        token->type = TokenType::OPERATOR;
                        token->text[tlen++] = '<';
                        token->text[tlen++] = '<';
                        switch(*(str+(sizeof(codechar)*2)))
                        {
                            case('=')://bitshift-right-assign operator (>>=)
                                token->text[tlen++] = '=';
                                str+=sizeof(codechar);
                                break;
                            default:break;//normal bitshift-right operator
                        }
                        newToken();
                        break;
                    default:
                        if(tokens.size()>0 && cstrcmp("#include",tokens[tokens.size()-1]->text))
                        {
                            token->text[tlen++] = '<';
                        }
                        else
                        {
                            std::cout << "prev token text: " << tokens[tokens.size()-1]->text << std::endl;
                            newToken();
                            token->type = TokenType::OPERATOR;
                            token->text[tlen++] = '<';
                            newToken();
                        }
                        break;//normal greater operator
                }
                break;
            case('&'):
                newToken();
                token->type = TokenType::OPERATOR;
                token->text[tlen++] = '&';
                switch(*(str+(sizeof(codechar)*1)))
                {
                    case('=')://and-assign operator (&=)
                        token->text[tlen++] = '=';
                        str+=sizeof(codechar);
                        break;
                    default:break;//normal and operator
                }
                newToken();
                break;
            case('|'):
                newToken();
                token->type = TokenType::OPERATOR;
                token->text[tlen++] = '|';
                switch(*(str+(sizeof(codechar)*1)))
                {
                    case('=')://or-assign operator (|=)
                        token->text[tlen++] = '=';
                        str+=sizeof(codechar);
                        break;
                    default:break;//normal or operator
                }
                newToken();
                break;
            case('%'):
                newToken();
                token->type = TokenType::OPERATOR;
                token->text[tlen++] = '%';
                switch(*(str+(sizeof(codechar)*1)))
                {
                    case('=')://mod-assign operator (%=)
                        token->text[tlen++] = '=';
                        str+=sizeof(codechar);
                        break;
                    default:break;//normal mod operator
                }
                newToken();
                break;

            //handle whitespaces
            case('\n'):
                line++;
                col = 0;
            case(' '):
            case(0x09)://horizontal tab
            case(0x0b)://vertical tab
                newToken();
                break;

            //handle terminators
            case(';'):
                newToken();
                token->text[tlen++] = *str;
                token->type = TokenType::TERMINATOR;
                newToken();
                break;

            //misc
            case(','):
                newToken();
                token->text[tlen++] = *str;
                token->type = TokenType::TERMINATOR;
                newToken();

            //open scopes
            case('{'):
                newToken();
                token->text[tlen++] = *str;
                token->type = TokenType::SCOPE;
                newToken();
                break;
            case('('):
                newToken();
                token->text[tlen++] = *str;
                token->type = TokenType::PA;
                newToken();
                break;
            case('['):
                newToken();
                token->text[tlen++] = *str;
                token->type = TokenType::PA;
                newToken();
                break;

            //close scopes
            case('}'):
                newToken();
                token->text[tlen++] = *str;
                token->type = TokenType::SCOPE;
                newToken();
                break;
            case(')'):
                newToken();
                token->text[tlen++] = *str;
                token->type = TokenType::PA;
                newToken();
                break;
                newToken();
            case(']'):
                newToken();
                token->text[tlen++] = *str;
                token->type = TokenType::PA;
                newToken();
                break;

            //handle strings
            case('"'):
                newToken();
                isDoubleQuote = true;
                token->text[tlen++] = *str;
                break;
            case('\''):
                newToken();
                isSingleQuote = true;
                token->text[tlen++] = *str;
                break;

            //all other characters
            default:
                token->text[tlen++] = *str;
                break;
        }
        goto afterSwitches;

        singleQuoteSwitch:;
        switch(*str)
        {
            case('\''):
                isSingleQuote = false;
                token->text[tlen++] = *str;
                newToken();
                break;
            default:
                token->text[tlen++] = *str;
        }
        goto afterSwitches;

        doubleQuoteSwitch:;
        switch(*str)
        {
            case('"'):
                isDoubleQuote = false;
                token->text[tlen++] = *str;
                newToken();
                break;
            default:
                token->text[tlen++] = *str;
        }

        afterSwitches:;
        col++;
        str+=sizeof(codechar);
    }

    return tokens;
}

std::vector<token_t*> tokenizeFile(char* fname)
{
    FILE* f = fopen(fname, "rb");
    if(f == NULL)
    {
        std::cerr << "ERROR: could not open input file " << fname << std::endl;
        exit(-1);
    }
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);  /* same as rewind(f); */

    char* content = malloc(fsize + 1);
    fread(content, fsize, 1, f);
    fclose(f);
    content[fsize] = 0x00;//terminate content string

    file = fname;

    return tokenize(content);
}

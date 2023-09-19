TARGET=x86_64
ARCHDIR=architectures
FORMATDIR=formats
MANGLERDIR=manglers
ABIDIR=ABIs

SOURCE_asm_asm=$(wildcard src/*.asm)
SOURCE_asm_asm+=$(wildcard src/$(ARCHDIR)/*.asm)
SOURCE_asm_asm+=$(wildcard src/$(FORMATDIR)/*.asm)
SOURCE_asm_asm+=$(wildcard src/$(MANGLERDIR)/*.asm)
SOURCE_asm_asm+=$(wildcard src/$(ABIDIR)/*.asm)
OBJECTS_asm_asm=$(patsubst src/%.asm,build/%.o,$(SOURCE_asm_asm))
DEPFILES_asm_asm=$(patsubst src/%.asm,build/%.d,$(SOURCE_asm_asm))
SOURCE_asm_s=$(wildcard src/*.s)
SOURCE_asm_s+=$(wildcard src/$(ARCHDIR)/*.s)
SOURCE_asm_s+=$(wildcard src/$(FORMATDIR)/*.s)
SOURCE_asm_s+=$(wildcard src/$(MANGLERDIR)/*.s)
SOURCE_asm_s+=$(wildcard src/$(ABIDIR)/*.s)
OBJECTS_asm_s=$(patsubst src/%.s,build/%.o,$(SOURCE_asm_s))
DEPFILES_asm_s=$(patsubst src/%.s,build/%.d,$(SOURCE_asm_s))
SOURCE_cpp_cpp=$(wildcard src/*.cpp)
SOURCE_cpp_cpp+=$(wildcard src/$(ARCHDIR)/*.cpp)
SOURCE_cpp_cpp+=$(wildcard src/$(FORMATDIR)/*.cpp)
SOURCE_cpp_cpp+=$(wildcard src/$(MANGLERDIR)/*.cpp)
SOURCE_cpp_cpp+=$(wildcard src/$(ABIDIR)/*.cpp)
OBJECTS_cpp_cpp=$(patsubst src/%.cpp,build/%.o,$(SOURCE_cpp_cpp))
DEPFILES_cpp_cpp=$(patsubst src/%.cpp,build/%.d,$(SOURCE_cpp_cpp))
SOURCE_cpp_cxx=$(wildcard src/*.cxx)
SOURCE_cpp_cxx+=$(wildcard src/$(ARCHDIR)/*.cxx)
SOURCE_cpp_cxx+=$(wildcard src/$(FORMATDIR)/*.cxx)
SOURCE_cpp_cxx+=$(wildcard src/$(MANGLERDIR)/*.cxx)
SOURCE_cpp_cxx+=$(wildcard src/$(ABIDIR)/*.cxx)
OBJECTS_cpp_cxx=$(patsubst src/%.cxx,build/%.o,$(SOURCE_cpp_cxx))
DEPFILES_cpp_cxx=$(patsubst src/%.cxx,build/%.d,$(SOURCE_cpp_cxx))
SOURCE_c_c=$(wildcard src/*.c)
SOURCE_c_c+=$(wildcard src/$(ARCHDIR)/*.c)
SOURCE_c_c+=$(wildcard src/$(FORMATDIR)/*.c)
SOURCE_c_c+=$(wildcard src/$(MANGLERDIR)/*.c)
SOURCE_c_c+=$(wildcard src/$(ABIDIR)/*.c)
OBJECTS_c_c=$(patsubst src/%.c,build/%.o,$(SOURCE_c_c))
DEPFILES_c_c=$(patsubst src/%.c,build/%.d,$(SOURCE_c_c))

CXXARGS=
CCARGS=
ASARGS=

all: compiler

build/%.o: src/%.asm
	@$(AS) $(ASARGS) -M -MD -c -o $@ $<
	$(info  	$(AS)	$<)
build/%.o: src/%.s
	@$(AS) $(ASARGS) -M -MD -c -o $@ $<
	$(info  	$(AS)	$<)
build/%.o: src/%.cpp
	@$(CXX) -O0 $(CXXARGS) -std=c++20 -g -Wno-write-strings -rdynamic -pedantic -Wunreachable-code -Wno-literal-suffix -Wno-pointer-arith -MP -MD -fpermissive -Iinc/ -c -o $@ $<
	$(info 	$(CXX)	$<)
build/%.o: src/%.cxx
	@$(CXX) -O0 $(CXXARGS) -std=c++20 -g -Wno-write-strings -rdynamic -pedantic -Wno-multichar -Wunreachable-code -Wno-literal-suffix -Wno-pointer-arith -MP -MD -fpermissive -Iinc/ -c -o $@ $<
	$(info 	$(CXX)	$<)
build/%.o: src/%.c
	@$(CC) $(CCARGS) -Wunreachable-code -Iinc/ -c -o $@ $<
	$(info  	$(CC)	$<)
clean:
	@-rm -r build/*.o
	@-rm -r build/$(ARCHDIR)/*.o
	@-rm -r build/$(FORMATDIR)/*.o
	@-rm -r build/$(MANGLERDIR)/*.o
	@-rm -r build/$(ABIDIR)/*.o
	$(info  	DELETE	build/*.o)
	$(info  	DELETE	build/$(ARCHDIR)/*.o)
	$(info  	DELETE	build/$(FORMATDIR)/*.o)
	$(info  	DELETE	build/$(MANGLERDIR)/*.o)
	$(info  	DELETE	build/$(ABIDIR)/*.o)
	@-rm -r build/*.d
	@-rm -r build/$(ARCHDIR)/*.d
	@-rm -r build/$(FORMATDIR)/*.d
	@-rm -r build/$(MANGLERDIR)/*.d
	@-rm -r build/$(ABIDIR)/*.d
	$(info  	DELETE	build/*.d)
	$(info  	DELETE	build/$(ARCHDIR)/*.d)
	$(info  	DELETE	build/$(FORMATDIR)/*.d)
	$(info  	DELETE	build/$(MANGLERDIR)/*.d)
	$(info  	DELETE	build/$(ABIDIR)/*.d)

compiler:  $(OBJECTS_asm_asm) $(OBJECTS_asm_s) $(OBJECTS_cpp_cxx) $(OBJECTS_cpp_cpp) $(OBJECTS_c_c)
	@$(LD) -g -rdynamic $(OBJECTS_asm_asm) $(OBJECTS_asm_s) $(OBJECTS_cpp_cxx) $(OBJECTS_cpp_cpp) $(OBJECTS_c_c) -Llib/ -lc -lstdc++ -lcargparse -o cp2
	$(info  	$(LD)	$@)

countLines:
	@find src inc -type f \( -iname \*.cxx -o -iname \*.cpp -o -iname \*.c -o -iname \*.h -o -iname \*.hpp -o -iname \*.hxx \) -exec cat {} \; | wc -l

listCodeFiles:
	@find src inc -type f \( -iname \*.cxx -o -iname \*.cpp -o -iname \*.c -o -iname \*.h -o -iname \*.hpp -o -iname \*.hxx \) -print

-include $(DEPFILES_asm_asm)
-include $(DEPFILES_asm_s)
-include $(DEPFILES_cpp_cpp)
-include $(DEPFILES_cpp_cxx)
-include $(DEPFILES_c_c)


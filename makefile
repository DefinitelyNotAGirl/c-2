TARGET=x86_64

SOURCE_asm_asm=$(wildcard src/*.asm)
SOURCE_asm_asm+=$(wildcard src/$(TARGET)/*.asm)
OBJECTS_asm_asm=$(patsubst src/%.asm,build/%.o,$(SOURCE_asm_asm))
DEPFILES_asm_asm=$(patsubst src/%.asm,build/%.d,$(SOURCE_asm_asm))
SOURCE_asm_s=$(wildcard src/*.s)
SOURCE_asm_s+=$(wildcard src/$(TARGET)/*.s)
OBJECTS_asm_s=$(patsubst src/%.s,build/%.o,$(SOURCE_asm_s))
DEPFILES_asm_s=$(patsubst src/%.s,build/%.d,$(SOURCE_asm_s))
SOURCE_cpp_cpp=$(wildcard src/*.cpp)
SOURCE_cpp_cpp+=$(wildcard src/$(TARGET)/*.cpp)
OBJECTS_cpp_cpp=$(patsubst src/%.cpp,build/%.o,$(SOURCE_cpp_cpp))
DEPFILES_cpp_cpp=$(patsubst src/%.cpp,build/%.d,$(SOURCE_cpp_cpp))
SOURCE_cpp_cxx=$(wildcard src/*.cxx)
SOURCE_cpp_cxx+=$(wildcard src/$(TARGET)/*.cxx)
OBJECTS_cpp_cxx=$(patsubst src/%.cxx,build/%.o,$(SOURCE_cpp_cxx))
DEPFILES_cpp_cxx=$(patsubst src/%.cxx,build/%.d,$(SOURCE_cpp_cxx))
SOURCE_c_c=$(wildcard src/*.c)
SOURCE_c_c+=$(wildcard src/$(TARGET)/*.c)
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
	$(info  	DELETE	build/*.o)
	@-rm -r build/x86_64/*.o
	$(info  	DELETE	build/x86_64/*.o)
	@-rm -r build/*.d
	$(info  	DELETE	build/*.d)
	@-rm -r build/x86_64/*.d
	$(info  	DELETE	build/x86_64/*.d)

compiler:  $(OBJECTS_asm_asm) $(OBJECTS_asm_s) $(OBJECTS_cpp_cxx) $(OBJECTS_cpp_cpp) $(OBJECTS_c_c)
	@$(LD) -g -rdynamic $(OBJECTS_asm_asm) $(OBJECTS_asm_s) $(OBJECTS_cpp_cxx) $(OBJECTS_cpp_cpp) $(OBJECTS_c_c) -Llib/ -lc -lstdc++ -lcargparse -o cp2
	$(info  	$(LD)	$@)

-include $(DEPFILES_asm_asm)
-include $(DEPFILES_asm_s)
-include $(DEPFILES_cpp_cpp)
-include $(DEPFILES_cpp_cxx)
-include $(DEPFILES_c_c)


scan-build -o anal\
	--keep-cc\
	--use-cc gcc\
	--use-c++ g++\
	-enable-checker core.CallAndMessage\
	-enable-checker core.NullDereference\
	-enable-checker core.NonNullParamChecker\
	-enable-checker core.StackAddressEscape\
	-enable-checker cplusplus.StringChecker\
	-enable-checker cplusplus.InnerPointer\
	-enable-checker core.uninitialized.UndefReturn\
	-enable-checker core.uninitialized.Branch\
	-enable-checker unix.Malloc\
	make
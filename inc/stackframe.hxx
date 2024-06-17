#include <cstdint>

class stackframe {
protected:
	uint64_t size = 0;
	uint64_t workingOffset = 0;
public:
	stackframe();
	stackframe(uint64_t size);
	uint64_t setSize(uint64_t bytes);
	uint64_t push(uint64_t bytes);
	uint64_t pop(uint64_t bytes);
};

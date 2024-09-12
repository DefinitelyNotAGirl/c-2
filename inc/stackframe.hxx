#include <cstdint>

class stackframe {
protected:
	uint64_t m_size = 0;
	uint64_t m_workingOffset = 0;
public:
	stackframe();
	stackframe(uint64_t size);
	/**
	 * @brief sets the size
	 */
	uint64_t size(uint64_t bytes);
	/**
	 * @brief gets the size
	 */
	uint64_t size();
	/**
	 * @brief adds ${bytes} to the current stack size
	 */
	uint64_t push(uint64_t bytes);
	/**
	 * @brief removes ${bytes} from the current stack size
	 */
	uint64_t pop(uint64_t bytes);
};

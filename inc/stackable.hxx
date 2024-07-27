#include <stack>

template<typename T>
class stackable {
protected:
	std::stack<T> data;
public:
	stackable(T& obj) : data(std::stack<T>({obj})) {
	}

	void push(T& obj) {
		this->data.push(obj);
	}

	void pop() {
		this->data.pop();
	}

    operator T&() {
        return this->data.top();
    }
}

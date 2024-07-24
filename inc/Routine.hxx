#pragma once

typedef void(*RoutineCallback)(void*);
class Routine {
public:
	void* data;
	RoutineCallback callback;

	Routine(void* data,RoutineCallback callback)
		:callback(callback),data(data){}

	Routine(RoutineCallback callback)
		:callback(callback),data(nullptr){}

	void run(){
		this->callback(this->data);
	}
};

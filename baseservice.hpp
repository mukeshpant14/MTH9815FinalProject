#ifndef BASESERVICE_HPP
#define BASESERVICE_HPP

#include <string>
#include "soa.hpp"

using namespace std;

enum Action {ADD, UPDATE};

template <typename T>
class BaseService : public Service<string, T>
{
public:
	// Add a listener to the Service for callbacks on add, remove, and update events
	// for data to the Service.
	void AddListener(ServiceListener<T> *listener)
	{
		listeners.push_back(listener);
	}

	// Get all listeners on the Service.
	const vector<ServiceListener<T>*>& GetListeners() const
	{
		return listeners;
	}

	void callListeners(T data, Action action)
	{
		for (auto it = listeners.begin(); it != listeners.cend(); ++it)
		{
			T d(data);
			if (action == Action::ADD)
				(*it)->ProcessAdd(d);
			else if (action == Action::UPDATE)
				(*it)->ProcessUpdate(d);
			else
				throw "Unsupported action";
		}
	}

	void printMessage(string message)
	{
		if(++printCount <= MAX_PRINT_COUNT)
			std::cout << message << std::endl;
	}
private:
	vector<ServiceListener<T>*> listeners;

	int printCount = 0; // 
	int MAX_PRINT_COUNT = 10;
};

#endif // !BASESERVICE_HPP


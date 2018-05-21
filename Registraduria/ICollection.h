#pragma once

template<typename T>
class ICollection {

public:

	virtual void insert(T) = 0;
};
#pragma once

#include <memory>

template<typename T>
using SharedPtr = std::shared_ptr<T>;
//class SharedPtr : public std::shared_ptr<T> { };

template<typename T, typename... _Types>
inline static SharedPtr<T> MakeShared(_Types&&... _Args) {
	return std::make_shared<T>(_Args...);
}
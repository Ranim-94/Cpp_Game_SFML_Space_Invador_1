
#pragma once

#include <cstdio>

#include <memory>
#include <list>
#include <vector>
#include <map>
#include <unordered_map>

#define LOG(M,...) printf(M"\n",__VA_ARGS__)

/*
 * __VA_ARGS__ stands for variadic arguments
 *
 * so if extra arguments don't exist, the '...' in LOG()
 * will be eliminated
 *
 *
 *	Also printf() is much faster then std::cout for logging
 *
 *
 * */


 // define data types

template<typename T>
using uniq_ptr = std::unique_ptr<T>; 

template<typename T>
using shar_ptr = std::shared_ptr<T>;

template<typename T>
using weak_ptr = std::weak_ptr<T>;

// Containers

template<typename T>
using list = std::list<T>;

template<typename T>
using vector = std::vector<T>;

template<typename key, typename value, typename pr = std::less<key>>
using map = std::map<key,value,pr>;

template<typename key, typename value, typename hash = std::hash<key>>
using dict = std::unordered_map<key,value,hash>;









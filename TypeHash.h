// Copyright(C) 2021 Kobe Vrijsen <kobevrijsen@posteo.be>
// 
// A simple ECS example
// 
// This file is free software and distributed under the terms of the European Union
// Public Lincense as published by the European Commision; either version 1.2 of the
// License, or , at your option, any later version.

#pragma once

#include <string>

#include "FastHash.h"

template <typename ... T>
consteval std::string_view TypeName() noexcept
{
	using namespace std::literals;
	constexpr std::string_view func{
#if defined(_MSC_VER)
		__FUNCSIG__
#else
		__PRETTY_FUNCTION__
#endif
	};
	auto const first{ func.find(__func__) + std::size(__func__) };
	return func.substr(first, func.find(">("sv, first) - first);
}

template <typename ... T>
consteval uint64_t TypeHash(uint64_t seed = {}) noexcept
{
	return FastHash(TypeName<T...>(), seed);
}

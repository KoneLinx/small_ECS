// Copyright(C) 2021 Kobe Vrijsen <kobevrijsen@posteo.be>
// 
// A simple ECS example
// 
// This file is free software and distributed under the terms of the European Union
// Public Lincense as published by the European Commision; either version 1.2 of the
// License, or , at your option, any later version.

#pragma once

#include <tuple>
#include <typeinfo>
#include <array>
#include <algorithm>
#include <vector>
#include <functional>
#include "TypeHash.h"
#include "System.h"

struct EntityHandle
{
	virtual void Handle(SystemHandle&) = 0;

	virtual constexpr std::span<const uint64_t> ComponentHashes() noexcept = 0;
};

template <typename ... Components> requires (std::same_as<Components, std::remove_cvref_t<Components>> && ...)
class Entity : public EntityHandle, public std::tuple<Components...>
{
public:

	using std::tuple<Components...>::tuple;

	auto      &  Base()      &  { return *this; }
	auto const&  Base() const&  { return *this; }
	auto      && Base()      && { return std::move(*this); }
	auto const&& Base() const&& { return std::move(*this); }

	virtual void Handle(SystemHandle& system) override
	{
		auto const keys{ system.ArgumentHashes() };
		std::array<void*, sizeof...(Components)> args{};
		((args[std::distance(keys.begin(), std::ranges::find(keys, TypeHash<Components>()))] = &std::get<Components>(Base())), ...);
		system.Handle(args);
	}

	consteval static uint64_t Hash() noexcept
	{
		return TypeHash<Entity>();
	}

	virtual constexpr std::span<const uint64_t> ComponentHashes() noexcept override
	{
		return m_ComponentHashes;
	}

private:

	consteval static auto ComponentHashesArray() noexcept
	{
		std::array<uint64_t, sizeof...(Components)> hashes{ TypeHash<Components>() ... };
		std::ranges::sort(hashes);
		return hashes;
	}

	constexpr static auto m_ComponentHashes{ ComponentHashesArray() };

};

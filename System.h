// Copyright(C) 2021 Kobe Vrijsen <kobevrijsen@posteo.be>
// 
// A simple ECS example
// 
// This file is free software and distributed under the terms of the European Union
// Public Lincense as published by the European Commision; either version 1.2 of the
// License, or , at your option, any later version.

#pragma once

#include <tuple>
#include "TypeHash.h"

struct SystemHandle 
{
	virtual void Handle(std::span<void*>) noexcept = 0;

	virtual constexpr std::span<const uint64_t> ArgumentHashes() const noexcept = 0;
	virtual constexpr std::span<const uint64_t> ArgumentHashesSorted() const noexcept = 0;
};

template <typename Fn>
class System : public SystemHandle, public Fn
{

	template <typename Ret, typename Obj, typename ... Arg>
	static std::tuple<Arg...> Fn_t(Ret(Obj::*)(Arg...));
	// Not defined

	template <typename Ret, typename Obj, typename ... Arg>
	static std::tuple<Arg...> Fn_t(Ret(Obj::*)(Arg...) const);
	// Not defined

public:

	template <typename Fnr>
	constexpr System(Fnr&& fn)
		: Fn{ std::forward<Fn>(fn) }
	{}

	using Fn::operator();
	
	using Args = decltype(Fn_t(&Fn::operator()));
	
	virtual void Handle(std::span<void*> args) noexcept override
	{
		HandleArgs(args, std::make_index_sequence<std::tuple_size_v<Args>>{});
	}

	virtual constexpr std::span<const uint64_t> ArgumentHashes() const noexcept override
	{
		return m_ArgumentHashes;
	}

	virtual constexpr std::span<const uint64_t> ArgumentHashesSorted() const noexcept override
	{
		return m_SortedHashes;
	}

private:
	
	template <size_t ... INDEX>
	void HandleArgs(std::span<void*> args, std::index_sequence<INDEX...>) noexcept
	{
		this->operator()( *static_cast<std::remove_cvref_t<std::tuple_element_t<INDEX, Args>>*>(args[INDEX]) ... );
	}

	template <typename ... Args>
	consteval static std::array<uint64_t, sizeof...(Args)> ComponentHashesArray(std::tuple<Args...>*) noexcept
	{
		return { TypeHash<std::remove_cvref_t<Args>>() ... };
	}

	template <size_t N>
	consteval static std::array<uint64_t, N> SortHashes(std::array<uint64_t, N> hashes) noexcept
	{
		std::ranges::sort(hashes);
		return hashes;
	}

	constexpr static auto m_ArgumentHashes{ ComponentHashesArray(static_cast<Args*>(nullptr)) };
	constexpr static auto m_SortedHashes{ SortHashes(m_ArgumentHashes) };

};

template <typename T>
System(T) -> System<std::decay_t<T>>;
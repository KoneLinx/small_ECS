// Copyright(C) 2021 Kobe Vrijsen <kobevrijsen@posteo.be>
// 
// A simple ECS example
// 
// This file is free software and distributed under the terms of the European Union
// Public Lincense as published by the European Commision; either version 1.2 of the
// License, or , at your option, any later version.

/* The MIT License

   Copyright (C) 2012 Zilong Tan (eric.zltan@gmail.com)
   
   Permission is hereby granted, free of charge, to any person
   obtaining a copy of this software and associated documentation
   files (the "Software"), to deal in the Software without
   restriction, including without limitation the rights to use, copy,
   modify, merge, publish, distribute, sublicense, and/or sell copies
   of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be
   included in all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
   NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
   BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
   ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
   CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
   SOFTWARE.
*/

// Source: fast-hash https://github.com/ztanml/fast-hash (26 Nov 2021)
// Changes: General uplift and providing a general constant hash for ranges

#pragma once

#include <ranges>

// Compression function for Merkle-Damgard construction.
// This function is generated using the framework provided.
//#define mix(h) ({
//			(h) ^= (h) >> 23;
//			(h) *= 0x2127599bf4325c37ULL;
//			(h) ^= (h) >> 47; })

template <std::ranges::range Range> requires std::convertible_to<std::ranges::range_value_t<std::remove_cvref_t<Range>>, uint64_t>
constexpr uint64_t FastHash(Range&& range, uint64_t seed/*, uint64_t back = {}*/)
{
	auto const mix = [](uint64_t h)
	{
		h ^= h >> 23;
		h *= 0x2127599bf4325c37ULL;
		h ^= h >> 47;
		return h;
	};

	std::ranges::subrange rest{ range };

	auto const size = [&]
	{
		if constexpr (std::ranges::sized_range<Range>)
			return static_cast<uint64_t>(rest.size());
		else
			return static_cast<uint64_t>(1);
	};

	const uint64_t m = 0x880355f21e6d1965ULL;

	uint64_t h = seed ^ (size() * m);

	for (; !rest.empty(); rest.advance(1))
	{
		h ^= mix(static_cast<uint64_t>(*rest.begin()));
		h *= m;
	}

	//if (back != uint64_t())
	//{
	//	h ^= mix(back);
	//	h *= m;
	//}

	return mix(h);
}

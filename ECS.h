// Copyright(C) 2021 Kobe Vrijsen <kobevrijsen@posteo.be>
// 
// A simple ECS example
// 
// This file is free software and distributed under the terms of the European Union
// Public Lincense as published by the European Commision; either version 1.2 of the
// License, or , at your option, any later version.

#pragma once

#include "Entity.h"
#include "System.h"

#include <unordered_map>
#include <any>
#include <vector>
#include <memory>

class EntityStore
{
public:

	template <typename Entity>
	void AddEntity(Entity entity)
	{
		auto const [node, isNew] = m_EntityStore.emplace(std::piecewise_construct, std::make_tuple(entity.Hash()), std::make_tuple());
		node->second.push_back(std::unique_ptr<EntityHandle>{ new Entity{ std::move(entity) } });
		
		if (!isNew)
			return;
		
		for (auto& [system, entities] : m_SystemStore)
			if (std::ranges::includes(entity.ComponentHashes(), system->ArgumentHashesSorted()))
				entities.push_back(entity.Hash());
	}

	template <typename System>
	void AddSystem(System system)
	{
		auto& [systemPtr, matchedEntities] = m_SystemStore.emplace_back();
		systemPtr.reset(new System{ std::move(system) });
		
		auto const& hashes{ systemPtr->ArgumentHashesSorted() };
		for (auto& [key, entities] : m_EntityStore)
			if (std::ranges::includes(entities.front()->ComponentHashes(), hashes))
				matchedEntities.push_back(key);
	}

	inline void CallHandles()
	{
		for (auto& [system, entityKeys] : m_SystemStore)
			for (auto& entityKey : entityKeys)
				for (auto& entity : m_EntityStore[entityKey])
					entity->Handle(*system);
	}

private:

	std::unordered_map<
		uint64_t,
		std::vector<std::unique_ptr<EntityHandle>>,
		std::identity
	> m_EntityStore{};

	std::vector<
		std::pair<
			std::unique_ptr<SystemHandle>,
			std::vector<uint64_t>
		>
	> m_SystemStore{};

};
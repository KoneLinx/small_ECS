// Copyright(C) 2021 Kobe Vrijsen <kobevrijsen@posteo.be>
// 
// ActionTree - Tree based decision / action stucture helper.An alternative to branches.
// 
// This file is free software and distributed under the terms of the European Union
// Public Lincense as published by the European Commision; either version 1.2 of the
// License, or , at your option, any later version.

#include "ECS.h"
#include <iostream>

int main()
{
	// Component store

	EntityStore store{};

	// Adding various entities

	using Person = Entity<std::string, int>;
	store.AddEntity(Person{ "Sam", 19 });
	store.AddEntity(Person{ "Jeb", 32 });
	store.AddEntity(Person{ "Ann", 25 });
	store.AddEntity(Person{ "Ron", 47 });

	using Animal = Entity<float, std::string, int>;
	store.AddEntity(Animal{ 375.f, "Jack", 7917 });
	store.AddEntity(Animal{ 537.f, "Jony", 9771 });

	using Object = Entity<std::string, float>;
	store.AddEntity(Object{ "Ice", .956f });
	store.AddEntity(Object{ "Air", .102f });
	store.AddEntity(Object{ "Log", .753f });

	// Adding systems

	store.AddSystem(System{
		[] (int age, std::string& name)
		{
			printf("%s is %d years old\n", name.c_str(), age);
			name += 'y';
		}
	});
	store.AddSystem(System{
		[] (std::string const& mat, float density)
		{
			printf("%s weighs %f kg/l\n", mat.c_str(), density);
		}
	});
	
	// Later on, match all systems and execute them

	for ([[maybe_unused]] auto i : {1,2,3,4,5})
	{
		store.CallHandles();
	}
}
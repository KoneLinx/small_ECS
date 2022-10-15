# small_ECS

A small Entity Component System (ECS) as a proof of concept.

## How to use

### Store

A store holds entities, components and systems. It will handle all the storage and logic for you.

```cpp
EntityStore store{};
```

### Components

They can be anything!

### Entity

An entity is a collection of components. Just define one as a type and start using it. Use `AddEntity` to add an entity to the store.

```cpp
using DenseObject = Entity<std::string, float>;
store.AddEntity(DenseObject{ "Ice" , .956f });
store.AddEntity(DenseObject{ "Wood", .753f });
```

### System

A system is a function that will execute on each matching entity. The parameter list of the system is matched to the entities containing the correct components.

Add a system to the store with `AddSystem`

```cpp
store.AddSystem(
  [] (int i, float& f)
  {
    f += 0.123f * i;
  }
)
```

### Execution

By calling `CallHandles`, all systems will be matched and executed when entities apply to them.


```cpp
store.CallHandles();
```

##

This project is a proof of concept and I hope it helps.

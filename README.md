# Deki RTC

Documentation: https://dekiengine.github.io/deki-rtc/ (components and properties, generated from the code)

Real-time clock peripheral interface for the Deki Engine, with a DS3231 (I²C) backend.

Part of the [Deki Engine](https://github.com/dekiengine/deki-engine) package ecosystem.

## Namespace

This package's types live in `DekiRtc`. Scene files store the qualified
name, so a component is `DekiRtc::SomeComponent` there, and code naming one
needs the namespace:

```cpp
using namespace DekiRtc;
obj->AddComponent<SomeComponent>();
```

Scenes saved before 0.16.0 used bare names and still load: every component
records what it used to be called, and a save writes the current name.

## Dependencies

| Dependency | Type |
|---|---|
| `deki-i2c` | Deki package |

## Installation

Install via the Package Manager inside the Deki Editor.

## License

Licensed under the Apache License, Version 2.0. See [LICENSE](LICENSE) for details.

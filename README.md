# Deki RTC

Docs: https://dekiengine.github.io/deki-rtc/ (components and properties, generated from the code)

Real-time clock peripheral interface for the Deki Engine, with a DS3231 (I²C) backend.

Part of [Deki Engine](https://github.com/dekiengine/deki-engine).

## Namespace

Types live in `DekiRtc`. Scene files store the qualified name, and so does code:

```cpp
using namespace DekiRtc;
obj->AddComponent<SomeComponent>();
```

Scenes saved before 0.16.0 used bare names and still load; saving writes the current one.

## Dependencies

| Dependency | Type |
|---|---|
| `deki-i2c` | Deki package |

## Install

Package Manager in the Deki Editor, or `DekiEditor --packages-add deki-rtc <project>`.

## License

Apache 2.0. See [LICENSE](LICENSE).

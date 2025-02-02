# changelog

This file contains all changes made to _phoenix_ in reverse order, meaning the newest change is listed first. This
file is updated whenever a new version of _phoenix_ is released. More information about how versioning works can be
found in [readme.md](readme.md#versioning).

---

## v1.0.1

This version contains patches for many bugs as well as some performance improvements in the _VDF_ and _ZenGin Archive_
parsers. The following changes have been made since [v1.0.0](https://github.com/lmichaelis/phoenix/releases/tag/v1.0.0):

### Bugfixes

* [d9357f6] Previously, only the non-`const` version of `script::find_symbol_by_name` was case-insensitive. Now
  the `const` version is too.
* [7d87485] Non-free waypoints are now correctly included in the `way_net::waypoint` lookup
* [c8c2294] Return `null`-instances from the default VM external set by `vm::register_default_external` if needed
* [ce320ad] If a default external is set the stack guard in the `opcode::be` branch of `vm::exec` is now inhibited
  preventing stack corruption
* [272c2a3] The stack guard for the `opcode::bl` instruction in `vm::exec` used `sym` as uninitialized which caused VM
  crashes if any script function was overridden
* [612b078] Fix incorrect usage of `std::isspace` in multiple locations which could cause crashes using compilers which
  treat `char` as signed

### Performance

* [88c43eb] Improve `archive_reader::read_object_begin` performance by up to 30% by using `sscanf` instead
  of `std::stringstream` internally

### Misc

* [52e3136, 21d59b8, 6bc9967] Allow using Git submodules in addition to CMake's `FetchContent` for resolving
  dependencies
* [ccf8ce6] Updated old SHA1 hash of the lexy dependency (thanks, @thokkat!)

## v1.0.0 – Initial release

After about a year of development, _phoenix_ released version `1.0.0` in October 2022. After replacing
[Try/ZenLib](https://github.com/Try/ZenLib) which was forked from [ataulien/ZenLib](https://github.com/ataulien/ZenLib)
in the _Gothic_ engine re-implementation [OpenGothic](https://github.com/Try/OpenGothic) and a bit more than 500
commits, _phoenix_ has finally reached a point at which I would consider it to be stable enough to make a full release.

Thanks to everyone who helped test the implementation in [OpenGothic#271](https://github.com/Try/OpenGothic/pull/271).
I want to especially thank [@Try](https://github.com/Try) for allowing me to integrate into
[OpenGothic](https://github.com/Try/OpenGothic)! It really helped motivate me to keep working on _phoenix_ and to make
it better!

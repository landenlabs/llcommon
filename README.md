# llcommon

Shared utility library for the landenlabs `llXXXX` command-line tools (`lldu`, `lldupdir`, `llrename`, `llreplace`, `llbin22`, `lljson`, `llxml`, ...).

Each consuming project previously kept its own copy-pasted fork of these files, which had already drifted and accumulated inconsistent bug fixes across repos. This repo is now the single source of truth.

## Layout

Flat layout, matching the existing per-project convention, so `#include "directory.hpp"` keeps working unchanged from consuming projects:

- `ll_stdhdr.hpp` — common platform typedefs/defines (`HAVE_WIN`, etc.)
- `lstring.hpp` — `std::string` wrapper used throughout
- `directory.cpp/hpp` — cross-platform directory enumeration (POSIX `dirent` / Win32 `FindFirstFile`) and file utilities (`DirUtil::*`)
- `parseutil.cpp/hpp` — CLI option parsing helpers, `Colors::showError`
- `signals.cpp/hpp` — Ctrl-C / `SIGINT` handling on both platforms
- `colors.hpp` — terminal color codes
- `split.hpp` — string split helpers

## Usage

Add as a git submodule at `<project>/llcommon` (sibling to `<project>/<project>/` and `<project>/<project>-ms/`), then point the project's build files (Xcode header search paths / `ClCompile` includes in the `.vcxproj` / `Makefile`+`build.csh`) at `../llcommon` in addition to the project's own source directory.

## License

Apache License 2.0 — see `LICENSE.txt`.

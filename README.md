# OmniKit

OmniKit is a reusable C++17 utility library that bundles common building
blocks needed in embedded and robotics-style applications: math helpers,
geometry primitives, signal filters, controllers (e.g. PID), a lightweight
logger, a progress reporter, and time utilities. Each module lives under
its own directory in `include/omnikit/` and `src/`, and can be linked
independently via CMake targets under the `omnikit::` namespace.

## Modules

- `controller` - control algorithms (e.g. PID controller)
- `filter`     - signal filtering utilities
- `geometry`   - geometric primitives and helpers
- `log`        - lightweight logging utility
- `math`       - general-purpose math helpers
- `progress`   - progress reporting utility
- `time`       - time-related utilities

## Requirements

- CMake >= 3.14
- A C++17 compiler
- clang-format and clang-tidy (for code quality targets)

## Build

```
make build
```

This configures CMake (using the `default` preset) if needed, then
compiles the project in debug mode.

## Other Make Targets

Build:
- `make configure`   - configure CMake without compiling
- `make build`       - compile in debug mode
- `make release`     - compile an optimized release build
- `make rebuild`     - clean, then build
- `make test`        - build and run the test suite
- `make clean`       - remove all build artifacts

Install:
- `make install`     - install to the system
- `make uninstall`   - remove previously installed files

Code Quality:
- `make format`               - check formatting (no changes made)
- `make format-fix`           - auto-format all sources
- `make lint`                 - run clang-tidy on the whole project
- `make lint-module MODULE=<name>` - run clang-tidy on a single module (e.g. `MODULE=progress`)
- `make lint-fix`             - run clang-tidy with automatic fixes
- `make quality`               - run `format` and `lint` together

Info:
- `make info`   - show project information (sources count, build dir, etc.)
- `make help`   - show the list of available targets

## Commit Message Format

```
(feat|fix|update|docs|style|refactor|test|chore): Message
```

Each commit must start with one of the following types, followed by a
colon and a short, imperative summary of the change:

- `feat`     - a brand new feature or capability that did not exist before
               (e.g. `feat(filter): add moving average filter`)
- `fix`      - a bug fix; the code was behaving incorrectly and now it is
               corrected (e.g. `fix(controller): correct PID integral windup`)
- `update`   - an enhancement or change to existing, already-working
               functionality (not a new feature, not a bug fix)
               (e.g. `update(log): add millisecond timestamp support`)
- `docs`     - documentation-only changes: README, comments, guides
               (e.g. `docs: explain make targets`)
- `style`    - formatting-only changes with no effect on logic
               (whitespace, clang-format, renaming for readability)
               (e.g. `style: run clang-format on src/`)
- `refactor` - internal code restructuring that does not change external
               behavior (e.g. `refactor(math): simplify vector normalize`)
- `test`     - adding or updating tests only
               (e.g. `test(geometry): add edge case for zero-length vector`)
- `chore`    - maintenance tasks that do not touch library source or tests
               (build scripts, CI, dependencies, .gitignore, etc.)
               (e.g. `chore(CI): update build workflow`)

Guidelines:

- Use the module name in parentheses when the change is scoped to one
  module, e.g. `fix(filter): ...`. Omit it for project-wide changes.
- Write the summary in imperative mood ("add", "fix", "update"), not past
  tense ("added", "fixed").
- Keep the summary short (ideally under ~72 characters); add more detail
  in the commit body if needed.
- If a change touches multiple categories (e.g. a feature plus its
  tests), pick the type that reflects the primary intent of the commit,
  and consider splitting into separate commits when they are logically
  independent.

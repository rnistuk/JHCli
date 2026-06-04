JHCli — CLAUDE.md
============================

A C++ command-line tool that fetches and displays COVID-19 statistics from the Johns
Hopkins University CSSE data repository — case counts, deaths, and recoveries by region.
A learning project for HTTP fetching with libcurl, CSV parsing, local caching, and
structuring a CMake C++ CLI with GoogleTest.

Project Structure
-----------------

*   `main.cpp` — entry point; parses args and drives the command handler.
*   `src/command_handler.{h,cpp}` — dispatches CLI commands.
*   `src/johns_hopkins.{h,cpp}` — fetches data from the Johns Hopkins CSSE repo (libcurl).
*   `src/data_parser.{h,cpp}` — parses the CSV data into records. The unit-tested core.
*   `src/region.h` — the region/record data type.
*   `src/filesystem_helpers.{h,cpp}` — local cache / file helpers.
*   `tests/data_parser_test.cpp` — GoogleTest suite (via the `add_gmock_test` helper in
    `tests/CMakeLists.txt`); currently covers `data_parser`.
*   `cmake-build-debug/` — CLion's generated build dir (not source).

Environment
-----------

*   IDE: CLion
*   Language: C++20
*   Build system: CMake 3.16+
*   Key dependencies: libcurl (`find_package(CURL REQUIRED)`); GoogleTest 1.8.1 (fetched
    at configure time via `ExternalProject`)

Building and Testing
--------------------

```
mkdir build && cd build
cmake ..
make
./jhcli          # run the CLI
ctest            # run the unit tests
```

`enable_testing()` is on and tests are discovered via `gtest_discover_tests`. Currently
the parser is the covered surface; fetching/IO is exercised manually.

Conventions
-----------

*   TDD — write failing tests first, then minimum code to pass
*   No comments unless the why is non-obvious
*   No speculative code — only implement what is needed right now
*   Commit messages: short, descriptive, imperative tense ("add region filter" not "added" or "adding")
*   C++20; keep network fetching (`johns_hopkins`) separate from parsing (`data_parser`)
    so the parser stays unit-testable without hitting the network
*   New tests register through `add_gmock_test(<target> <sources>)` in `tests/CMakeLists.txt`

Current State
-------------

### Completed

*   CLI scaffold: arg handling → `command_handler`
*   Johns Hopkins CSSE fetch over libcurl with local caching
*   CSV parsing into region records, covered by a GoogleTest suite

### In Progress

*   Nothing in flight

### Next

*   Document example invocations (country/region queries, date ranges) in the README
*   Broaden test coverage beyond `data_parser` (command handling, filesystem helpers)
*   Pin GoogleTest version is already done (1.8.1) — consider bumping to a current release

### Decisions Pending

*   None

Do Not Touch
------------

*   Nothing off limits at this time.

Pair Programming
----------------

*   Driver writes all code — navigator (Claude) does not write code unless explicitly asked
*   Navigator provides: direction, design decisions, code review, and course corrections
*   One step at a time — navigator waits for driver to confirm before moving to the next
*   Navigator confirms tests pass before suggesting the next step
*   Navigation is terse — no lengthy explanations unless explicitly asked
*   If navigator disagrees with an approach, say so once then follow the driver's lead

Karpathy Rules
--------------

All rules apply to every task unless explicitly overridden.

### Rule 1 — Think Before Coding

State assumptions explicitly. If uncertain, ask rather than guess. Present multiple interpretations when ambiguity exists. Push back when a simpler approach exists. Stop when confused. Name what's unclear.

### Rule 2 — Simplicity First

Minimum code that solves the problem. Nothing speculative. No features beyond what was asked. No abstractions for single-use code. Test: would a senior engineer say this is overcomplicated? If yes, simplify.

### Rule 3 — Surgical Changes

The driver should touch only what is necessary to make the current test pass. Don't "improve" adjacent code, comments, or formatting. Don't refactor what isn't broken. Match existing style. Navigator warns the driver if this rule is being broken.

### Rule 4 — Agree on Done Before Starting

Before implementing anything, both driver and navigator should agree on what done looks like. A failing test is the preferred success criterion. Don't start until done is defined.

### Rule 5 — Dropped

Original rule was written for agentic coding and does not apply to this pair programming model.

### Rule 6 — Know When to Stop

If the session is getting long or context feels stale, checkpoint before continuing. Summarise what's done, what's verified, what's next. Update CLAUDE.md to reflect current state, then commit before ending the session. A fresh session with good context beats a stale one with accumulated confusion.

### Rule 7 — Surface Conflicts, Don't Average Them

If two patterns contradict, pick one (more recent / more tested). Explain why. Flag the other for cleanup. Don't blend conflicting patterns.

### Rule 8 — Read Before You Write

Before adding code, read exports, immediate callers, shared utilities. "Looks orthogonal" is dangerous. If unsure why code is structured a way, ask.

### Rule 9 — Test Behaviours, Not Functionality

Tests verify what the code does from the outside, not how it does it internally. Tests must encode WHY the behaviour matters, not just WHAT it does. A test that can't fail when business logic changes is wrong.

### Rule 10 — Checkpoint After Every Significant Step

Summarize what was done, what's verified, what's left. Don't continue from a state you can't describe back. If you lose track, stop and restate.

### Rule 11 — Match the Codebase's Conventions, Even If You Disagree

Conformance > taste inside the codebase. If you genuinely think a convention is harmful, surface it. Don't fork silently.

### Rule 12 — Fail Loud

"Completed" is wrong if anything was skipped silently. "Tests pass" is wrong if any were skipped. Default to surfacing uncertainty, not hiding it.

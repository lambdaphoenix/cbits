# CHANGELOG
## [0.2.1]
### Added
 - GitHub Actions workflow extended with an optimized CI pipeline including C tests, a Python version matrix, and manual release approval.
 - CI status badge added to the README for clearer visibility of build health.

### Changed
 - Test workflow cleaned up by removing requirements.txt in favor of dev extras.
 - Python 3.14 added to the supported/tested versions.
 - Internal CI structure improved by consolidating C and Python test execution.

## [0.2.0]
### Added
- Implement `__iter__` for Python BitVector.
- Add slicing support, including optimized fast‑path for single‑step slices.
- Introduce range methods `set_range`, `clear_range`, and `flip_range` for efficient bulk operations.
- Add prefetching in hot loops and hash caching for faster dictionary/set lookups.
- Additional unit tests for new functionality.

### Changed
- Removed redundant atomics in Python BitVector binding.
- Adjusted loops to reduce index calculations per iteration.
- Refactored hash method: switched to `METH_O` and improved inline documentation.
- Reduced overhead and ensured tail bits are masked in bitwise operations (`and`, `iand`, `or`, …).
- Improved performance in `rank` and `contains` queries.

## [0.1.1]
### Added
- Introduce `compat_dispatch.c` for runtime CPU feature detection and automatic selection of the fastest popcount implementation.
- Additional unit tests

### Changed
- Refactor `compat.h` dispatch logic to clarify fallback paths and cleanup macros.  

## [0.1.0]
### Added
- C backend with SIMD popcount support
- Python sequence and numeric protocol support (`getitem`, `and`, `or`, etc.)
- Unit tests for all methods
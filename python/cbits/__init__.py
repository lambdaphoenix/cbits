## @module cbits
# @file python/cbits/__init__.py
# @brief cbits - High-performance BitVector for Python.
# 
# This module exposes the Python-level API for the BitVector C extension.
# 
# @author lambdaphoenix
# @version 0.3.0
# @copyright Copyright (c) 2026 lambdaphoenix
"""
High-performance BitVector for Python.

This module exposes the Python-level API for the BitVector C extension.

Author lambdaphoenix

Version 0.3.0

Copyright (c) 2026 lambdaphoenix
"""
from ._cbits import BitVector, __author__, __version__, __license__, __license_url__

## @brief Package author name (forwarded from the C extension).
__author__ = _cbits.__author__
"""The package author's name."""

## @brief Package version, synchronized with the C extension.
__version__ = _cbits.__version__
"""Package version, synchronized with the C extension."""

## @brief SPDX license identifier.
__license__ = _cbits.__license__
"""SPDX lisence identifier."""

## @brief URL to the full license text.
__license_url__ = _cbits.__license_url__
"""URL pointing to the full license text."""

## @brief Symbols exposed to Python users
## @ingroup cbits_api
__all__ = [
    "BitVector",
]
"""cbits_api - Symbols exposed to Python users"""
# python/cbits/__init__.py
"""
cbits – High-performance BitVector for Python.

This package wraps a C implementation of a packed bitvector with O(1) rank/select and subvector searches.

Copyright (c) 2025 lambdaphoenix
"""
from ._cbits import BitVector

__all__ = ["BitVector"]
/// @file include/compat.h
/// @brief Cross-platform aligned allocators, atomics, popcount, prefetch.
///
/// Provides wrappers around posix_memalign or _aligned_malloc,
/// C11 atomics or MSVC intrinsics, and optimized popcount-block routines.
#ifndef CBITS_COMPAT_H
#define CBITS_COMPAT_H

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

#if defined(_MSC_VER)
    #include <immintrin.h>
#else
    #include <x86intrin.h>
#endif

/* Aligned malloc / free */
#if defined(_MSC_VER)
    #include <malloc.h>
    #include <errno.h>
static inline int
posix_memalign(void **memptr, size_t alignment, size_t size)
{
    void *p = _aligned_malloc(size, alignment);
    if (!p) {
        return ENOMEM;
    }
    *memptr = p;
    return 0;
}
static inline void
cbits_free_aligned(void *ptr)
{
    _aligned_free(ptr);
}
#else
static inline void
cbits_free_aligned(void *ptr)
{
    free(ptr);
}
#endif
static inline void *
cbits_malloc_aligned(size_t size, size_t align)
{
    void *p = NULL;
    if (posix_memalign(&p, align, size) != 0) {
        return NULL;
    }
    return p;
}

/* Atomics */
#if defined(_MSC_VER)
    #ifndef WIN32_LEAN_AND_MEAN
        #define WIN32_LEAN_AND_MEAN
    #endif
    #include <windows.h>
    #include <intrin.h>
static inline uint64_t
cbits_atomic_load(const uint64_t *ptr)
{
    return (uint64_t) InterlockedCompareExchange64((volatile LONG64 *) ptr, 0,
                                                   0);
}
static inline uint64_t
cbits_atomic_fetch_or(uint64_t *ptr, uint64_t v)
{
    return (uint64_t) InterlockedOr64((volatile LONG64 *) ptr, v);
}
static inline uint64_t
cbits_atomic_fetch_and(uint64_t *ptr, uint64_t v)
{
    return (uint64_t) InterlockedAnd64((volatile LONG64 *) ptr, v);
}
static inline uint64_t
cbits_atomic_fetch_xor(uint64_t *ptr, uint64_t v)
{
    return (uint64_t) InterlockedXor64((volatile LONG64 *) ptr, v);
}
#elif defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L && \
    !defined(__STDC_NO_ATOMICS__)
    #include <stdatomic.h>
static inline uint64_t
cbits_atomic_load(const uint64_t *ptr)
{
    return atomic_load((_Atomic uint64_t *) ptr);
}
static inline uint64_t
cbits_atomic_fetch_or(uint64_t *ptr, uint64_t v)
{
    return atomic_fetch_or((_Atomic uint64_t *) ptr, v);
}
static inline uint64_t
cbits_atomic_fetch_and(uint64_t *ptr, uint64_t v)
{
    return atomic_fetch_and((_Atomic uint64_t *) ptr, v);
}
static inline uint64_t
cbits_atomic_fetch_xor(uint64_t *ptr, uint64_t v)
{
    return atomic_fetch_xor((_Atomic uint64_t *) ptr, v);
}
#else
static inline uint64_t
cbits_atomic_load(const uint64_t *ptr)
{
    return __atomic_load_n(ptr, __ATOMIC_RELAXED);
}
static inline uint64_t
cbits_atomic_fetch_or(uint64_t *ptr, uint64_t v)
{
    return __atomic_fetch_or(ptr, v, __ATOMIC_RELAXED);
}
static inline uint64_t
cbits_atomic_fetch_and(uint64_t *ptr, uint64_t v)
{
    return __atomic_fetch_and(ptr, v, __ATOMIC_RELAXED);
}
static inline uint64_t
cbits_atomic_fetch_xor(uint64_t *ptr, uint64_t v)
{
    return __atomic_fetch_xor(ptr, v, __ATOMIC_RELAXED);
}
#endif

/* Prefetch */
#if defined(_MSC_VER)
    #include <intrin.h>
static inline void
cbits_prefetch(const void *ptr)
{
    _mm_prefetch((const char *) ptr, _MM_HINT_T0);
}
#else
static inline void
cbits_prefetch(const void *ptr)
{
    __builtin_prefetch(ptr);
}
#endif

/* Popcount */
#if defined(_MSC_VER)
    #pragma intrinsic(__popcnt64)
static inline uint64_t
cbits_popcount64(const uint64_t *ptr)
{
    return (uint64_t) __popcnt64(*ptr);
}
#else
static inline uint64_t
cbits_popcount64(const uint64_t *ptr)
{
    return (uint64_t) __builtin_popcountll(*ptr);
}
#endif
#if defined(__AVX512VPOPCNTDQ__)
static inline uint64_t
cbits_popcount_block(const uint64_t *ptr)
{
    __m512i v = _mm512_load_si512((const void *) ptr);
    __m512i c = _mm512_popcnt_epi64(v);
    return _mm512_reduce_and_epi64(c);
}
#elif defined(__AVX2__)
static inline uint64_t
cbits_popcount_block(const uint64_t *ptr)
{
    __m256i v0 = _mm256_loadu_si256((const __m256i *) ptr);
    __m256i v1 = _mm256_loadu_si256((const __m256i *) (ptr + 4));
    uint64_t tmp[4];
    _mm256_storeu_si256((__m256i *) tmp, v0);
    _mm256_storeu_si256((__m256i *) (tmp + 2), v1);
    return cbits_popcount64(tmp[0]) + cbits_popcount64(tmp[1]) +
           cbits_popcount64(tmp[2]) + cbits_popcount64(tmp[3]);
}
#else
static inline uint64_t
cbits_popcount_block(const uint64_t *ptr)
{
    return cbits_popcount64(ptr);
}
#endif

#endif /* CBITS_COMBAT_H */
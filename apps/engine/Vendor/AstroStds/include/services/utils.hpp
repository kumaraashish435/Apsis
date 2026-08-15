#pragma once

// ============================================================================
// Platform Types (must come first)
// ============================================================================
#ifdef _WIN32
    #include <windows.h>
    #define STDCALL __cdecl
    // Windows already defines __int64
#else
    #include <dlfcn.h>
    #define STDCALL
    typedef long long __int64;   // Linux/Unix compatibility
#endif

// 64-bit Fortran address type
typedef __int64 fAddr;

// ============================================================================
// Standard Library Includes
// ============================================================================
#include <stdexcept>
#include <string>
#include <cstring>     // <-- required for memset, strncpy, strnlen
#include <iostream>    // <-- required for print helpers
#include <vector>
#include <array>

// ============================================================================
// SharedLibrary Loader
// ============================================================================
class SharedLibrary {
public:
    explicit SharedLibrary(const char* path) {
#ifdef _WIN32
        handle = LoadLibraryA(path);
        if (!handle) {
            throw std::runtime_error(std::string("Failed to load library: ") + path);
        }
#else
        handle = dlopen(path, RTLD_NOW);
        if (!handle) {
            const char* err = dlerror();
            throw std::runtime_error(
                std::string("Failed to load library: ") + path +
                (err ? std::string(" — ") + err : "")
            );
        }
#endif
    }

    ~SharedLibrary() {
#ifdef _WIN32
        if (handle) {
            FreeLibrary((HMODULE)handle);
        }
#else
        if (handle) {
            dlclose(handle);
        }
#endif
    }

    SharedLibrary(const SharedLibrary&) = delete;
    SharedLibrary& operator=(const SharedLibrary&) = delete;

    template<typename T>
    T load(const char* symbol) {
#ifdef _WIN32
        void* sym = reinterpret_cast<void*>(GetProcAddress((HMODULE)handle, symbol));
#else
        void* sym = dlsym(handle, symbol);
#endif
        if (!sym) {
            throw std::runtime_error(std::string("Missing symbol: ") + symbol);
        }
        return reinterpret_cast<T>(sym);
    }

private:
    void* handle{};
};

// ============================================================================
// Utility Helpers
// ============================================================================
static inline std::string trim(const std::string& s) {
    const char* ws = " \t\n\r\f\v";
    size_t start = s.find_first_not_of(ws);
    if (start == std::string::npos)
        return "";

    size_t end = s.find_last_not_of(ws);
    return s.substr(start, end - start + 1);
}

// ============================================================================
// Print Helpers
// ============================================================================
template <typename T, size_t N>
void print1D(const std::string& label, const std::array<T, N>& arr) {
    std::cout << label << " = ";
    for (size_t i = 0; i < N; ++i) {
        std::cout << arr[i];
        if (i + 1 < N) std::cout << ", ";
    }
    std::cout << "\n";
}

template <typename T>
void print1D(const std::string& label, const std::vector<T>& vec) {
    std::cout << label << " = ";
    for (size_t i = 0; i < vec.size(); ++i) {
        std::cout << vec[i];
        if (i + 1 < vec.size()) std::cout << ", ";
    }
    std::cout << "\n";
}

template <typename T, size_t R, size_t C>
void print2D(const std::string& label, const std::array<std::array<T, C>, R>& arr) {
    for (size_t r = 0; r < R; ++r) {
        std::cout << label << "(" << (r + 1) << ",:) = ";
        for (size_t c = 0; c < C; ++c) {
            std::cout << arr[r][c];
            if (c + 1 < C) std::cout << ", ";
        }
        std::cout << "\n";
    }
}

template <typename T, size_t C>
void print2D(const std::string& label, const std::vector<std::array<T, C>>& vec) {
    for (size_t r = 0; r < vec.size(); ++r) {
        std::cout << label << "(" << (r + 1) << ",:) = ";
        for (size_t c = 0; c < C; ++c) {
            std::cout << vec[r][c];
            if (c + 1 < C) std::cout << ", ";
        }
        std::cout << "\n";
    }
}

// ============================================================================
// Unified to_c / from_c Conversion Helpers
// ============================================================================

// --- string <-> char buffer ---
template <size_t N>
inline void to_c(const std::string& s, char (&buf)[N]) {
    std::memset(buf, 0, N);
    std::snprintf(buf, N, "%s", s.c_str()); //automatically adds the null character
    //std::strncpy(buf, s.c_str(), N - 1);
    //buf[N - 1] = '\0';
}

template <size_t N>
inline void from_c(const char (&buf)[N], std::string& out) {
    out.assign(buf, strnlen(buf, N));
    out = trim(out);
}

// --- std::array<T,N> <-> C array ---
template <typename T, size_t N>
inline void to_c(const std::array<T, N>& src, T (&dst)[N]) {
    for (size_t i = 0; i < N; ++i)
        dst[i] = src[i];
}

template <typename T, size_t N>
inline void from_c(const T (&src)[N], std::array<T, N>& dst) {
    for (size_t i = 0; i < N; ++i)
        dst[i] = src[i];
}

// --- 2D std::array <-> C 2D array ---
template <typename T, size_t R, size_t C>
inline void to_c(const std::array<std::array<T, C>, R>& src,
                 T (&dst)[R][C]) {
    for (size_t r = 0; r < R; ++r)
        for (size_t c = 0; c < C; ++c)
            dst[r][c] = src[r][c];
}

template <typename T, size_t R, size_t C>
inline void from_c(const T (&src)[R][C],
                   std::array<std::array<T, C>, R>& dst) {
    for (size_t r = 0; r < R; ++r)
        for (size_t c = 0; c < C; ++c)
            dst[r][c] = src[r][c];
}

// --- vector<T> <-> pointer + size ---
template <typename T>
inline void to_c(const std::vector<T>& src, T*& ptr, size_t& count) {
    ptr = const_cast<T*>(src.data());
    count = src.size();
}

template <typename T>
inline void from_c(const T* src, size_t count, std::vector<T>& dst) {
    dst.assign(src, src + count);
}

// --- vector<array<T,C>> <-> C 2D array ---
template <typename T, size_t C>
inline void to_c(const std::vector<std::array<T, C>>& src,
                 T (*&ptr)[C], size_t& count) {
    ptr = reinterpret_cast<T (*)[C]>(const_cast<std::array<T, C>*>(src.data()));
    count = src.size();
}

template <typename T, size_t C>
inline void from_c(const T (*src)[C], size_t count,
                   std::vector<std::array<T, C>>& dst) {
    dst.resize(count);
    for (size_t r = 0; r < count; ++r)
        for (size_t c = 0; c < C; ++c)
            dst[r][c] = src[r][c];
}


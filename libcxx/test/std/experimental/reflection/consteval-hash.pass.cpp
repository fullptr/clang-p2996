//===----------------------------------------------------------------------===//
//
// Copyright 2025 Bloomberg Finance L.P.
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03 || c++11 || c++14 || c++17 || c++20
// ADDITIONAL_COMPILE_FLAGS: -freflection
// ADDITIONAL_COMPILE_FLAGS: -fparameter-reflection
// ADDITIONAL_COMPILE_FLAGS: -Wno-unneeded-internal-declaration
// ADDITIONAL_COMPILE_FLAGS: -Wno-unused-private-field
// ADDITIONAL_COMPILE_FLAGS: -Wno-unused-variable

// <experimental/reflection>
//
// [reflection]
//
// RUN: %{exec} %t.exe > %t.stdout

#include <meta>

namespace check_hash_of_every_reflected_entity {
    namespace A {}
    // namespace
    static_assert(std::consteval_hash<std::meta::info>{}(^^A) != 0);
    
    struct Test {
        int a;
        double b;
        bool c;
    };

    // types
    static_assert(std::consteval_hash<std::meta::info>{}(^^int) != 0);
    static_assert(std::consteval_hash<std::meta::info>{}(^^bool) != 0);
    static_assert(std::consteval_hash<std::meta::info>{}(^^std::string) != 0);
    static_assert(std::consteval_hash<std::meta::info>{}(^^Test) != 0);

    // null reflection
    constexpr std::meta::info null_reflection;
    static_assert(std::consteval_hash<std::meta::info>{}(null_reflection) != 0);

    // values
    static_assert(std::consteval_hash<std::meta::info>{}(std::meta::reflect_constant(42)) != 0);
    static_assert(std::consteval_hash<std::meta::info>{}(std::meta::reflect_constant(3.14)) != 0);
    static_assert(std::consteval_hash<std::meta::info>{}(std::meta::reflect_constant(true)) != 0);
    static_assert(std::consteval_hash<std::meta::info>{}(std::meta::reflect_constant('A')) != 0);
  
    // objects
    static int static_var = 100;
    const double const_var = 2.718;
    thread_local bool tl_var = false;
    
    // TODO: static_assert(std::consteval_hash<std::meta::info>{}(std::meta::reflect_object(static_var)) != 0);
    // TODO: static_assert(std::consteval_hash<std::meta::info>{}(std::meta::reflect_object(const_var)) != 0);
    // TODO: static_assert(std::consteval_hash<std::meta::info>{}(std::meta::reflect_object(tl_var)) != 0);

    // functions
    int fn(int i) {
        return i;
    }
    
    void void_fn() {}
    
    constexpr int constexpr_fn(int x) {
        return x * 2;
    }
    
    template<typename T>
    T template_fn(T value) {
        return value;
    }
    
    static int static_fn(double d) {
        return static_cast<int>(d);
    }

    static_assert(std::consteval_hash<std::meta::info>{}(std::meta::reflect_function(fn)) != 0);
    static_assert(std::consteval_hash<std::meta::info>{}(std::meta::reflect_function(void_fn)) != 0);
    static_assert(std::consteval_hash<std::meta::info>{}(std::meta::reflect_function(constexpr_fn)) != 0);
    static_assert(std::consteval_hash<std::meta::info>{}(std::meta::reflect_function(template_fn<int>)) != 0);
    static_assert(std::consteval_hash<std::meta::info>{}(std::meta::reflect_function(static_fn)) != 0);

    // templates
    template <class T>
    struct TestWithTemplate
    {
        T data;
    };
    
    static_assert(std::consteval_hash<std::meta::info>{}(^^TestWithTemplate) != 0);
    static_assert(std::consteval_hash<std::meta::info>{}(^^TestWithTemplate<int>) != 0);
    static_assert(std::consteval_hash<std::meta::info>{}(^^TestWithTemplate<double>) != 0);

    // parameters    
    static_assert(std::consteval_hash<std::meta::info>{}(std::meta::parameters_of(^^fn)[0]) != 0);

    // annotation

    // TODO:

    // data member spec

    // TODO:

    // base specifier

    // TODO:

    // entity proxy

    // TODO:
}

namespace check_type_of_with_hash {
  int var = 12;
  static_assert(std::consteval_hash<std::meta::info>{}(^^int) == std::consteval_hash<std::meta::info>{}(std::meta::type_of(^^var)));

  struct A {
    const int var = 42;
  } test;

  static_assert(std::consteval_hash<std::meta::info>{}(^^A) == std::consteval_hash<std::meta::info>{}(std::meta::type_of(^^test)));
}

namespace check_hash_of_similar_entity_with_different_features {
    static_assert(std::consteval_hash<std::meta::info>{}(std::meta::reflect_constant(42)) != std::consteval_hash<std::meta::info>{}(std::meta::reflect_constant(42.0f)));

    // namespaces
    namespace A {
        struct Test {
            int a;
            double b;
            bool c;
        };
    }

    namespace B {
        struct Test {
            int a;
            double b;
            bool c;
        };
    }

    static_assert(std::consteval_hash<std::meta::info>{}(^^A::Test) == std::consteval_hash<std::meta::info>{}(^^A::Test));
    static_assert(std::consteval_hash<std::meta::info>{}(^^A::Test) != std::consteval_hash<std::meta::info>{}(^^B::Test));

    // templates
    template <class T>
    struct TestWithTemplate
    {
        T data;
    };

    static_assert(std::consteval_hash<std::meta::info>{}(^^TestWithTemplate<double>) == std::consteval_hash<std::meta::info>{}(^^TestWithTemplate<double>));
    static_assert(std::consteval_hash<std::meta::info>{}(^^TestWithTemplate<double>) != std::consteval_hash<std::meta::info>{}(^^TestWithTemplate<int>));
}


int main() {
    return 0;
}
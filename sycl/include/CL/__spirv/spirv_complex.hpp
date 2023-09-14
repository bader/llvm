//===- spirv_complex.hpp - SPIRV complex types ----------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#pragma once

#include <CL/__spirv/spirv_common.hpp> // for __SYCL_CONVERGENT__
#include <sycl/half_type.hpp>          // for half

#include <complex> // for std::complex

namespace __spv {

struct complex_float {
  complex_float() = default;
  complex_float(std::complex<float> x) : real(x.real()), imag(x.imag()) {}
  operator std::complex<float>() { return {real, imag}; }
  float real, imag;
};

struct complex_double {
  complex_double() = default;
  complex_double(std::complex<double> x) : real(x.real()), imag(x.imag()) {}
  operator std::complex<double>() { return {real, imag}; }
  double real, imag;
};

struct complex_half {
  complex_half() = default;
  complex_half(std::complex<sycl::half> x) : real(x.real()), imag(x.imag()) {}
  operator std::complex<sycl::half>() { return {real, imag}; }
  sycl::half real, imag;
};

} // namespace __spv

#if __SYCL_DEVICE_ONLY__
__SYCL_CONVERGENT__ extern __DPCPP_SYCL_EXTERNAL
    __SYCL_EXPORT __spv::complex_half
    __spirv_GroupCMulINTEL(unsigned int, unsigned int,
                           __spv::complex_half) noexcept;
__SYCL_CONVERGENT__ extern __DPCPP_SYCL_EXTERNAL
    __SYCL_EXPORT __spv::complex_float
    __spirv_GroupCMulINTEL(unsigned int, unsigned int,
                           __spv::complex_float) noexcept;
__SYCL_CONVERGENT__ extern __DPCPP_SYCL_EXTERNAL
    __SYCL_EXPORT __spv::complex_double
    __spirv_GroupCMulINTEL(unsigned int, unsigned int,
                           __spv::complex_double) noexcept;
#endif
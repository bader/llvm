; REQUIRES: system-linux, x86-registered-target, sycl-post-link, spirv-to-ir-wrapper
;
; Both wrapped and fatbin-only SYCL output dump finalized device images.
; OpenMP images must not be dumped, and dry runs must not copy images.
;
; RUN: rm -rf %t
; RUN: mkdir -p %t/fatbin-dump %t/wrapped-dump %t/openmp-dump %t/dry-dump
; RUN: split-file %s %t
; RUN: llvm-as %t/sycl.ll -o %t/input.bc
; RUN: llvm-offload-binary -o %t/sycl.pack --image=file=%t/input.bc,triple=spir64-unknown-unknown,arch=generic,kind=sycl
; RUN: clang-linker-wrapper --host-triple=x86_64-unknown-linux-gnu --linker-path=/usr/bin/ld --emit-fatbin-only --sycl-post-link-options=-split=none --sycl-dump-device-code=%t/fatbin-dump %t/sycl.pack -o %t/sycl.fatbin
; RUN: llvm-offload-binary %t/sycl.fatbin --image=file=%t/sycl.spv,triple=spir64-unknown-unknown,kind=sycl
; RUN: cmp %t/sycl.spv %t/fatbin-dump/*
;
; No host executable is needed to check the wrapped pipeline's dump.
; RUN: clang-linker-wrapper --host-triple=x86_64-unknown-linux-gnu --linker-path=/bin/true --sycl-post-link-options=-split=none --sycl-dump-device-code=%t/wrapped-dump %t/sycl.pack -o %t/wrapped
; RUN: cmp %t/sycl.spv %t/wrapped-dump/*
;
; OpenMP uses the generic Clang pipeline and must leave its dump directory empty.
; RUN: %clang_cc1 -triple x86_64-unknown-linux-gnu -emit-obj %t/openmp.c -o %t/openmp.o
; RUN: llvm-offload-binary -o %t/openmp.pack --image=file=%t/openmp.o,triple=x86_64-unknown-linux-gnu,kind=openmp
; RUN: clang-linker-wrapper --host-triple=x86_64-unknown-linux-gnu --linker-path=/usr/bin/ld --emit-fatbin-only --sycl-dump-device-code=%t/openmp-dump %t/openmp.pack -o %t/openmp.fatbin
; RUN: test -s %t/openmp.fatbin
; RUN: rmdir %t/openmp-dump
;
; RUN: clang-linker-wrapper --dry-run --host-triple=x86_64-unknown-linux-gnu --linker-path=/usr/bin/ld --emit-fatbin-only --sycl-dump-device-code=%t/dry-dump %t/sycl.pack -o %t/dry.fatbin
; RUN: clang-linker-wrapper --dry-run --host-triple=x86_64-unknown-linux-gnu --linker-path=/usr/bin/ld --sycl-dump-device-code=%t/dry-dump %t/sycl.pack -o %t/dry.wrapped
; RUN: rmdir %t/dry-dump
;
; Copy errors must propagate from fatbin-only output.
; RUN: not clang-linker-wrapper --host-triple=x86_64-unknown-linux-gnu --linker-path=/usr/bin/ld --emit-fatbin-only --sycl-post-link-options=-split=none --sycl-dump-device-code=%t/missing-dump %t/sycl.pack -o %t/error.fatbin 2>&1 | FileCheck %s
; CHECK: error: failed to copy file. From:
; CHECK-SAME: to: {{.*}}missing-dump
;
;--- sycl.ll
target triple = "spir64-unknown-unknown"
define spir_kernel void @kernel() {
  ret void
}
;
;--- openmp.c
void kernel(void) {}

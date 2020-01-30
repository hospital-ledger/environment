// Copyright 2017 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// PLEASE READ BEFORE CHANGING THIS FILE!
//
// This file implements the out of bounds signal handler for
// WebAssembly. Signal handlers are notoriously difficult to get
// right, and getting it wrong can lead to security
// vulnerabilities. In order to minimize this risk, here are some
// rules to follow.
//
// 1. Do not introduce any new external dependencies. This file needs
//    to be self contained so it is easy to audit everything that a
//    signal handler might do.
//
// 2. Any changes must be reviewed by someone from the crash reporting
//    or security team. See OWNERS for suggested reviewers.
//
// For more information, see https://goo.gl/yMeyUY.
//
// This file contains most of the code that actually runs in a signal handler
// context. Some additional code is used both inside and outside the signal
// handler. This code can be found in handler-shared.cc.

#include <signal.h>
#include <stddef.h>
#include <stdlib.h>

#include "src/trap-handler/trap-handler-internal.h"
#include "src/trap-handler/trap-handler.h"

namespace v8 {
namespace internal {
namespace trap_handler {

namespace {

#if V8_TRAP_HANDLER_SUPPORTED
bool IsKernelGeneratedSignal(siginfo_t* info) {
  return info->si_code > 0 && info->si_code != SI_USER &&
         info->si_code != SI_QUEUE && info->si_code != SI_TIMER &&
         info->si_code != SI_ASYNCIO && info->si_code != SI_MESGQ;
}

class SigUnmaskStack {
 public:
  explicit SigUnmaskStack(sigset_t sigs) {
    // TODO(eholk): consider using linux-syscall-support for calling this
    // syscall.
    pthread_sigmask(SIG_UNBLOCK, &sigs, &old_mask_);
  }

  ~SigUnmaskStack() { pthread_sigmask(SIG_SETMASK, &old_mask_, nullptr); }

 private:
  sigset_t old_mask_;

  // We'd normally use DISALLOW_COPY_AND_ASSIGN, but we're avoiding a dependency
  // on base/macros.h
  SigUnmaskStack(const SigUnmaskStack&) = delete;
  void operator=(const SigUnmaskStack&) = delete;
};
#endif
}  // namespace

#if V8_TRAP_HANDLER_SUPPORTED && V8_OS_LINUX
bool TryHandleSignal(int signum, siginfo_t* info, ucontext_t* context) {
  // Bail out early in case we got called for the wrong kind of signal.
  if (signum != SIGSEGV) {
    return false;
  }

  // Make sure the signal was generated by the kernel and not some other source.
  if (!IsKernelGeneratedSignal(info)) {
    return false;
  }

  // Ensure the faulting thread was actually running Wasm code.
  if (!IsThreadInWasm()) {
    return false;
  }

  // Clear g_thread_in_wasm_code, primarily to protect against nested faults.
  g_thread_in_wasm_code = false;

  // Begin signal mask scope. We need to be sure to restore the signal mask
  // before we restore the g_thread_in_wasm_code flag.
  {
    // Unmask the signal so that if this signal handler crashes, the crash will
    // be handled by the crash reporter.  Otherwise, the process might be killed
    // with the crash going unreported.
    sigset_t sigs;
    // Fortunately, sigemptyset and sigaddset are async-signal-safe according to
    // the POSIX standard.
    sigemptyset(&sigs);
    sigaddset(&sigs, SIGSEGV);
    SigUnmaskStack unmask(sigs);

    uintptr_t fault_addr = context->uc_mcontext.gregs[REG_RIP];

    // TODO(eholk): broad code range check

    // Taking locks in a signal handler is risky because a fault in the signal
    // handler could lead to a deadlock when attempting to acquire the lock
    // again. We guard against this case with g_thread_in_wasm_code. The lock
    // may only be taken when not executing Wasm code (an assert in
    // MetadataLock's constructor ensures this). This signal handler will bail
    // out before trying to take the lock if g_thread_in_wasm_code is not set.
    MetadataLock lock_holder;

    for (size_t i = 0; i < gNumCodeObjects; ++i) {
      const CodeProtectionInfo* data = gCodeObjects[i].code_info;
      if (data == nullptr) {
        continue;
      }
      const uintptr_t base = reinterpret_cast<uintptr_t>(data->base);

      if (fault_addr >= base && fault_addr < base + data->size) {
        // Hurray, we found the code object. Check for protected addresses.
        const ptrdiff_t offset = fault_addr - base;

        for (unsigned i = 0; i < data->num_protected_instructions; ++i) {
          if (data->instructions[i].instr_offset == offset) {
            // Hurray again, we found the actual instruction. Tell the caller to
            // return to the landing pad.
            context->uc_mcontext.gregs[REG_RIP] =
                data->instructions[i].landing_offset + base;
            return true;
          }
        }
      }
    }
  }  // end signal mask scope

  // If we get here, it's not a recoverable wasm fault, so we go to the next
  // handler.
  g_thread_in_wasm_code = true;
  return false;
}
#endif  // V8_TRAP_HANDLER_SUPPORTED && V8_OS_LINUX

#if V8_TRAP_HANDLER_SUPPORTED
void HandleSignal(int signum, siginfo_t* info, void* context) {
  ucontext_t* uc = reinterpret_cast<ucontext_t*>(context);

  if (!TryHandleSignal(signum, info, uc)) {
    // Since V8 didn't handle this signal, we want to re-raise the same signal.
    // For kernel-generated SEGV signals, we do this by restoring the default
    // SEGV handler and then returning. The fault will happen again and the
    // usual SEGV handling will happen.
    //
    // We handle user-generated signals by calling raise() instead. This is for
    // completeness. We should never actually see one of these, but just in
    // case, we do the right thing.
    struct sigaction action;
    action.sa_handler = SIG_DFL;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    sigaction(signum, &action, nullptr);
    if (!IsKernelGeneratedSignal(info)) {
      raise(signum);
    }
  }
  // TryHandleSignal modifies context to change where we return to.
}
#endif
}  // namespace trap_handler
}  // namespace internal
}  // namespace v8

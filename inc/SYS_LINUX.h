/**
 * Created Date: Tuesday May 21st 2024
 * Author: Lilith
 * -----
 * Last Modified: Tuesday May 21st 2024 7:49:57 pm
 * Modified By: Lilith (definitelynotagirl115169@gmail.com)
 * -----
 * Copyright (c) 2023-2024 DefinitelyNotAGirl@github
 * 
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */
#pragma once

#include <cstdint>

// Undefine any existing definitions from sched.h
#undef CLONE_VM
#undef CLONE_FS
#undef CLONE_FILES
#undef CLONE_SIGHAND
#undef CLONE_PTRACE
#undef CLONE_VFORK
#undef CLONE_PARENT
#undef CLONE_THREAD
#undef CLONE_NEWNS
#undef CLONE_SYSVSEM
#undef CLONE_SETTLS
#undef CLONE_PARENT_SETTID
#undef CLONE_CHILD_CLEARTID
#undef CLONE_DETACHED
#undef CLONE_UNTRACED
#undef CLONE_CHILD_SETTID
#undef CLONE_NEWCGROUP
#undef CLONE_NEWUTS
#undef CLONE_NEWIPC
#undef CLONE_NEWUSER
#undef CLONE_NEWPID
#undef CLONE_NEWNET
#undef CLONE_IO

namespace linux_6 {
	constexpr uint64_t CLONE_VM = 1ULL << 0;              // Share the same memory space
	constexpr uint64_t CLONE_FS = 1ULL << 1;              // Share the same filesystem information
	constexpr uint64_t CLONE_FILES = 1ULL << 2;           // Share the same file descriptor table
	constexpr uint64_t CLONE_SIGHAND = 1ULL << 3;         // Share the same signal handlers
	constexpr uint64_t CLONE_PTRACE = 1ULL << 4;          // Allow the tracing process to be traced by the parent
	constexpr uint64_t CLONE_VFORK = 1ULL << 5;           // Parent is suspended until child releases VM
	constexpr uint64_t CLONE_PARENT = 1ULL << 6;          // The parent of the new process will be the same as the parent of the caller
	constexpr uint64_t CLONE_THREAD = 1ULL << 7;          // Create a new thread in the same thread group
	constexpr uint64_t CLONE_NEWNS = 1ULL << 8;           // Create new namespace for the child
	constexpr uint64_t CLONE_SYSVSEM = 1ULL << 9;         // Share System V SEM_UNDO semantics
	constexpr uint64_t CLONE_SETTLS = 1ULL << 10;         // Create a new TLS (Thread Local Storage) for the child
	constexpr uint64_t CLONE_PARENT_SETTID = 1ULL << 11;  // Store the TID in the user-level buffer in the parent
	constexpr uint64_t CLONE_CHILD_CLEARTID = 1ULL << 12; // Clear the TID in the child upon exit
	constexpr uint64_t CLONE_DETACHED = 1ULL << 13;       // Unused, but reserved for backward compatibility
	constexpr uint64_t CLONE_UNTRACED = 1ULL << 14;       // Start in the detached state
	constexpr uint64_t CLONE_CHILD_SETTID = 1ULL << 15;   // Store the TID in the user-level buffer in the child
	constexpr uint64_t CLONE_NEWCGROUP = 1ULL << 16;      // Create a new cgroup namespace
	constexpr uint64_t CLONE_NEWUTS = 1ULL << 17;         // Create a new UTS namespace
	constexpr uint64_t CLONE_NEWIPC = 1ULL << 18;         // Create a new IPC namespace
	constexpr uint64_t CLONE_NEWUSER = 1ULL << 19;        // Create a new user namespace
	constexpr uint64_t CLONE_NEWPID = 1ULL << 20;         // Create a new PID namespace
	constexpr uint64_t CLONE_NEWNET = 1ULL << 21;         // Create a new network namespace
	constexpr uint64_t CLONE_IO = 1ULL << 22;             // Create a new I/O context
}

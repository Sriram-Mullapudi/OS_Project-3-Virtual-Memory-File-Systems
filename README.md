# Xv6 OS — Virtual Memory & Page Allocation

> **Course Project | Operating Systems | University of South Florida**
> Built on top of MIT's xv6 teaching OS

## Team Members
| Name | UID |
|------|-----|
| Sriram Mullapudi | U69942711 |
| Sai Mukesh Reddy Gutha | — |
| Yaswanth Bellamkonda | — |

**Same group as Project 2:** Yes

---

## Environment
| Tool | Version |
|------|---------|
| OS | Ubuntu (WSL) |
| Xv6 Version | xv6-public |
| Compiler | GCC |
| Emulator | QEMU (i386) |

---

## Overview

This project explores virtual memory management inside the xv6 kernel. Starting from xv6's default eager page allocation model, the project implements **lazy page allocation** and then extends it to **locality-aware allocation** — both selectable at compile time. All changes are implemented in C and wired into the kernel's trap handler and memory management subsystem.

---

## Part 1 — Understanding `sbrk()` (20 pts)

Modified `sys_sbrk()` in `sysproc.c` to remove the call to `growproc()`, making `sbrk(n)` only increment `proc->sz` by `n` without allocating any physical memory.

**Why the system breaks:**

The original xv6 `growproc()` uses `allocuvm()` to allocate physical pages and `mappages()` to map virtual addresses to physical memory immediately on each `sbrk()` call. Removing this means virtual addresses are extended but never backed by real memory. When any process (including `echo`, `cat`, `ls`) tries to access those addresses, the CPU triggers a **page fault (trap 14, error 6)** — a segmentation fault — and xv6 kills the process.

**Significance of `sbrk()`:**

`sbrk()` is the kernel interface for heap growth and is called internally by `malloc()`. It defines where a process's heap ends (`proc->sz`) and is the gateway to all dynamic memory in xv6. Without correct physical memory backing, any user program relying on `malloc()` crashes immediately.

---

## Part 2 — Lazy Page Allocation (45 pts)

Instead of crashing on page faults, the kernel now handles them dynamically by allocating and mapping a physical page on demand at the faulting address.

**Implementation in `trap.c`:**
- Added a `T_PGFLT` (page fault) case to the trap handler
- On page fault, retrieves the faulting virtual address via `rcr2()`
- Aligns it to the nearest page boundary with `PGROUNDDOWN()`
- Allocates a physical page with `kalloc()`
- Maps it to the faulting virtual address using `mappages()`
- Returns to user space — execution continues as if nothing happened

**Changes to `vm.c`:**
- Removed the `static` keyword from `mappages()` to make it accessible from `trap.c`
- Declared `mappages()` in `defs.h` for cross-file access

**Result:** `echo`, `ls`, `cat`, `wc`, and `mkdir` all execute successfully in any directory with no segmentation faults.

---

## Part 3 — Locality-Aware Allocation (15 pts)

Extended lazy allocation to allocate **three contiguous pages** per fault instead of one — exploiting spatial locality to reduce future page fault overhead.

**Implementation:**
- Modified the `T_PGFLT` trap handler to allocate and map the faulting page plus the next two consecutive pages
- Made `walkpgdir()` globally accessible (removed `static` in `vm.c`, declared in `vm.h`) so `trap.c` can check whether a page is already mapped before attempting to remap it
- Updated `copyuvm()` in `vm.c` to skip unmapped pages during `fork()`, preventing errors when a child process inherits a partially-allocated address space

**Compile-time selection via Makefile:**
```bash
make qemu ALLOCATOR=LAZY       # Pure lazy: 1 page per fault
make qemu ALLOCATOR=LOCALITY   # Locality-aware: 3 pages per fault
```

---

## Part 4 — Evaluating & Explaining Allocators (20 pts)

Added diagnostic print statements to the page fault handler in `trap.c` to log allocation behavior at runtime.

### Lazy Allocation Behavior
- Logs: `LAZY -- Pagefault for pid: X` then `LAZY -- Allocated & mapped page: pid X va YYYY`
- Allocates exactly **1 page per fault**
- Running `evaluate_lazy_locality_test` produces **8 page faults** for 8 distinct memory accesses

### Locality-Aware Allocation Behavior
- Logs: `LOCALITY -- Pagefault for pid: X` followed by **3 consecutive** `LOCALITY -- Allocated and mapped page` entries
- Allocates **3 pages per fault** (faulting page + 2 subsequent pages)
- Running `evaluate_lazy_locality_test` produces only **4 page faults** — half as many, because each fault pre-maps the next two pages

### Comparison

| Metric | Lazy | Locality-Aware |
|--------|------|----------------|
| Pages per fault | 1 | 3 |
| Page faults (test) | 8 | 4 |
| Log entries per fault | 1 | 3 consecutive addresses |
| Upfront memory overhead | Minimal | Slightly higher |
| Performance for sequential access | Lower | Higher |

The halved page fault count directly demonstrates that locality-aware pre-allocation works: once a region is first accessed, the surrounding two pages are already mapped and subsequent accesses to them require no trap handling at all.

---

## Modified Files

| File | Change |
|------|--------|
| `sysproc.c` | Removed `growproc()` from `sys_sbrk()` to enable lazy allocation |
| `trap.c` | Added `T_PGFLT` handler for on-demand page allocation (lazy & locality modes) |
| `vm.c` | Removed `static` from `mappages()` and `walkpgdir()`; updated `copyuvm()` to skip unmapped pages |
| `defs.h` / `vm.h` | Declared `mappages()` and `walkpgdir()` for cross-file access |
| `Makefile` | Added `ALLOCATOR` flag: `LAZY` or `LOCALITY` |

---

## Key Concepts Demonstrated

- How `sbrk()` drives dynamic heap memory in xv6
- How **page faults** (trap 14) are handled in the kernel trap handler
- The difference between **eager** and **lazy** page allocation
- **Spatial locality** as a principle for pre-fetching memory pages
- Making kernel functions globally accessible by removing `static` linkage
- Supporting **compile-time feature flags** in a kernel Makefile

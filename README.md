# Synchronization-Free-Memory-Allocator
Memory allocator for the x86-64 architecture with the following features:   
  Main free list using first-fit placement policy, augmented with a set of "quick lists" holding small blocks segregated by size. 
  Immediate coalescing of large blocks on free with adjacent free blocks; delayed coalescing on free of small blocks. 
  Boundary tags with footer optimization that allows footers to be omitted from allocated blocks. 
  Block splitting without creating splinters. Allocated blocks aligned to "double memory row" (16-byte) boundaries. 
  Free lists maintained using last in first out (LIFO) discipline.   

malloc, realloc, and free functions.  

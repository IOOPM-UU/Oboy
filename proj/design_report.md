# Design Documentation
This is a memory management library that implements a basic version of reference counting. 
It is designed to replace malloc/calloc and free in a program with library functions like 
allocate/release. This would make memory management safer avoiding memory leaks. The library 
also supports limiting cascading frees.

# High level Design

Metadata management:
For each allocated object a metadata struct is stored in a hash table. The struct includes:
- Reference count (rc): Tracks number of active references
- Destructor function pointer: To allow custom destructors for common complex data structures
- Object size in bytes: Used for memory release and validation

Reference counting:
- Functions retain and release modify the reference count (rc)
- Objects are deallocated when the rc is 0
- The release function schedules objects for destruction

Scheduling:
- Objects with rc = 0 are scheduled for destruction using a linked list
- Scheduled objects are processed using free_scheduled_tasks, which frees a set number of 
    objects per call, as defined by a global cascade limit

Destructors:
- Default destructor is implemented that can free complex data structures. It manages recursive 
    destruction of pointer-based objects.
- Custom destructors for hash-tables and linked-lists are made which improve performance for 
    those data structures

Memory allocation:
- Objects are allocated using allocate or allocate_array
- A metadata_t struct is associated with each allocated object, storing size and destructor.

Cleanup:
- Cleanup clears out all objects scheduled for deallocation.

Shutdown:
- Cleans up all system resources. Frees all objects and destroys hash table and linked list 
    used in library functions

# User implementation
For this library to work with any program, one would need to do the following:

1) Replace malloc/calloc with allocate (or allocate_array)
2) Replace free with release
3) Call shutdown() when program ends.

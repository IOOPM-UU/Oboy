**Reference Counting Garbage Collection in C**
# Design Documentation
Memory safety and resource management remain persistent challenges in systems programming with C. Manual memory management can lead to issues such as memory leaks, use-after-free errors, and double frees. Reference counting offers a solution by maintaining a simple invariant: every object tracks how many references currently point to it, and when this count reaches zero, the object can be reclaimed immediately. The implementation herein provides a pragmatic C-based reference counting library, which integrates seamlessly into existing code by replacing direct malloc/calloc and free calls with allocate, release, and related functions. Our implementation does not stray from instructions for project 1 in IOOPM (1DL221) Course at UU.

# System Goals
This library aims to replace manual *malloc* and *free* usage in a C program through a set of functions such as *allocate*, *retain*, and *release*. It aspires to do so while maintaining:
- Deferred Reclamation:
Objects that should be freed (reference count of zero) are put in a queue. When a new allocation is made a set number of objects from the queue are freed from memory.
- Controlled cascading:
The queue supports limiting the number of items freed per time. This scheduling algorithm prevents deeply recursive, or expensive frees from taking up too much system memory.
- Customizable Destructors
Users can specify how nested data structures should be released, enabling efficient cleanup of linked data.

**Core Data Structures**
1. Global Metadata Table
A global hash table (metadata_ht) tracks the status of each allocated object. The key is the object’s pointer address, cast to uintptr_t, and the value is a metadata_t struct that contains:
```
struct metadata {
	uint8_t rc;                  	// Reference count
	function1_t destructor;      	// Destructor function pointer
	size_t size;                 	// Object allocation size
};
```
Lookups during retain and release identify the metadata_t of a given object, enabling operations on its reference count or invocation of its destructor. The hash table is  initialized in initialize_collector().

2. Scheduling Linked List
A linked list (schedule_linked_list) temporarily holds objects that have transitioned to zero references. When release detects that an object’s count has dropped to zero, it places the object here instead of deallocating it immediately. This deferred processing helps manage bursts of object frees. The linked list and related logic are likewise initialized on demand.
3. Cascade limit
A global variable CASCADE_LIMIT imposes a maximum number of frees (or total memory size freed) per pass, preventing a single user action from triggering an unbounded chain of deallocations. This limit is enforced within the code’s task manager loop to throttle how many objects are handled at once.

# Functions and Control Flow
1. **Initialization**
Whenever the library’s allocation or reference-handling functions are called, it checks if the hash table and scheduling list exist. The function creates these structures if they are missing.
```
static void initialize_collector() {
	if (!metadata_ht) {
    	metadata_ht = lib_hash_table_create(int_equal, NULL, default_hash_function);
	}
	if (!schedule_linked_list) {
    	schedule_linked_list = lib_linked_list_create(int_equal);
	}
}
```
2. **Memory Allocation**
Users replace calls to *malloc* or *calloc* with allocate(size_t bytes, function1_t destructor) or allocate_array(size_t elements, size_t elem_size, function1_t destructor). Internally, allocate:
	1. Initializes the collector data structures if necessary
	2. Possibly frees a number of already scheduled objects by calling     	schedule_task_manager(NULL, bytes); doing so prevents runaway accumulation in the scheduling list.
	3. Allocates memory with calloc(1, bytes).
	4. Generates a new metadata_t struct via metadata_generate, setting reference count to 0 and storing the destructor pointer.
	5. Inserts this metadata_t struct into the hash table metadata_ht using. The key is the object’s pointer address, cast to uintptr_t

	The resulting pointer is returned to the user, ready to use.
```
obj *allocate(size_t bytes, function1_t destructor) {
	initialize_collector();
	schedule_task_manager(NULL, bytes);
	obj *object = calloc(1, bytes);
	uintptr_t key_as_int = (uintptr_t) object;
	metadata_t *metadata = metadata_generate(destructor, bytes);
	lib_hash_table_insert(get_metadata_ht(), lib_int_elem(key_as_int), lib_ptr_elem(metadata));
	return object;
}
```
3. **Retain and Release**

**Retain**
	Once an object is allocated, its reference count starts at 0. Calling retain(obj *object) increments this to reflect that the caller is now holding a valid reference.
```
	void retain(obj *object) {
	if (!object) return;
	uintptr_t key_as_int = (uintptr_t)object;
	lib_option_t option = lib_hash_table_lookup(get_metadata_ht(),         	lib_int_elem(key_as_int));
	if (option.success) {
    	metadata_t *metadata = (metadata_t *)(option.value.p);
    	metadata->rc++;
    	}
	}
```
**Release**
	Conversely, release(obj *object) decrements the reference count, indicating the caller no longer needs the object. If this drops to zero, the object is passed into the scheduling flow. The release logic checks whether another release cascade is in progress. If not, it starts the process of deallocating the zero-reference object. If a release cascade is ongoing, the object is simply appended to the linked list.

```
void release(obj *object) {
	if (!object) return;
	metadata_t *metadata = get_metadata(object);
	if (!metadata) return;
	else if(metadata->rc > 0) {
    	metadata->rc--;
	}
	if(metadata->rc == 0) {
    	if(!release_in_progress){
        	schedule_task_manager(object, 0);
    	}
    	else {
        	add_to_schedule(object);   
    	}
	}
}
```
4. **Scheduling and Cascading**

**Adding to the Schedule**
Any object whose reference count hits zero goes through add_to_schedule. This function appends the object to the scheduling list, then immediately invokes its destructor (or the default destructor) to handle nested references.
```
static void add_to_schedule(obj *object) {
	metadata_t *metadata = get_metadata(object);
	if(!metadata) return;
	lib_linked_list_append(get_schedule_linked_list(), lib_ptr_elem(object));
	if (metadata->destructor) {
    	metadata->destructor(object);
	}
	else {
    	default_destructor(object);
	}
}
```
**Managing Tasks**
The function task_manager_loop(size_t size) processes scheduled objects, removing them in batches constrained by the cascade limit and optionally by the size parameter. Within a loop, it calls deallocate on each item until it either hits the limit or exhausts the queue. This design helps prevent deep or unbounded recursion when many objects become unreachable.
```
static void task_manager_loop(size_t size) {
	size_t freed_size = 0;
	size_t cascade_amount = 0;
	while ((freed_size < size || cascade_amount < CASCADE_LIMIT) &&
       	lib_linked_list_size(get_schedule_linked_list()) > 0)
	{
    	bool successful1 = false;
    	obj *to_remove = lib_linked_list_get(get_schedule_linked_list(), 0, &successful1).p;
    	deallocate(to_remove);
    	cascade_amount++;
	}
	release_in_progress = false;
}
```
5. **Object Deallocation and Destructors**

**Deallocate**
When an object is actually removed from the scheduling list, the library calls deallocate. This procedure ensures the metadata struct is removed from the global table and the memory is freed safely. Removing the object from the scheduling list also prevents double frees or repeated processing.
```
void deallocate(obj* object) {
	if(!object) return;
	uintptr_t key_as_int = (uintptr_t)object;
	metadata_t *metadata = get_metadata(object);
	if(!metadata) return;
	assert(rc(object) == 0);
	lib_list_option_t exist = lib_linked_list_contains(schedule_linked_list, lib_ptr_elem(object));
	if(exist.success) {
    	bool successful1 = false;
    	lib_linked_list_remove(schedule_linked_list, exist.value.i, &successful1);
	}
	lib_hash_table_remove(get_metadata_ht(), lib_int_elem(key_as_int));
	free(metadata);
	free(object);
}
```
**Destructors**
Before deallocation, each object’s destructor is invoked. If a custom destructor is provided, it can finalize or release internal resources. If not, the system applies a default destructor that scans the allocated memory range for valid pointers (i.e., addresses found in the hash table) and calls release on each. This effectively cascades reference decrementing through an entire object graph. In the demo/ folder our hash table, linked list, and iterator have custom destructors.
```
static void default_destructor(obj* object) {
	if(!object) return;
	uintptr_t key_as_int = (uintptr_t) object;
	lib_option_t option = lib_hash_table_lookup(get_metadata_ht(), lib_int_elem(key_as_int));
	if (!option.success) return;
	metadata_t *metadata = (metadata_t *)(option.value.p);
	size_t object_size = metadata->size;
	destructor_loop(object_size, object);
}

static void destructor_loop(size_t object_size, obj *object) {
	for (size_t offset = 0; offset + sizeof(void *) <= object_size; offset++)
	{
    	void **possible_pointer = (void **)((char *)object + offset);
    	if (is_valid_pointer(*possible_pointer))
    	{
        	release(*possible_pointer);
    	}
	}
}
```
6. **Library Cleanup and Shutdown**
The function cleanup() calls schedule_task_manager() with no cascade limit, freeing all scheduled objects.
```
void cleanup() {
schedule_task_manager(NULL, SIZE_MAX);
}
```
When finished with the program, you should call shutdown(), which frees all allocations left, and destroys the hash table and linked list.
```
static void free_all() {
	cleanup();
	lib_linked_list_destroy(get_schedule_linked_list());
	schedule_linked_list = NULL;
	lib_hash_table_destroy(get_metadata_ht());
}

void shutdown() {
	set_cascade_limit(STANDARD_CASCADE_LIMIT);
	free_all();
}
```

# File structure
The codebase is divided into several files for clarity and maintainability.

Inside lib/
- refmem.c
	- Contains all reference counting functions, including allocate, retain, release, the scheduling logic, and the core deallocation routines.
- lib_linked_list.c
	- Custom implementation of a linked list. Does not use reference counting. Dependency for refmem.c. Mainly used for scheduling frees.
- lib_hash_table.c
    	- Custom implementation of a hash table. Does not use reference counting. Dependency for refmem.c. Mainly used for storing metadata about each items' reference count.
- lib_common.h
	- Mostly macros and typedefs
Inside demo/
Includes an example program for a shop using our reference counting library.

# Workflow

When a user wants to allocate memory it calls allocate() / allocate_array(). First time this is run, a metadata hash table and queue linked list is initialized using initialize_collector() called by allocate(). If specified, a custom destructor function can be included as a function pointer argument. 

If a user wants to make copies, it can call retain(). 

When freeing an object, the user calls release(). This release(obj *object) decrements the reference count, indicating the caller no longer needs the object. If this drops to zero, the object is passed into the scheduling flow. 

The function task_manager_loop(size_t size) processes scheduled objects, removing them in batches constrained by the cascade limit and optionally by the size parameter. Within a loop, it calls deallocate on each item until it either hits the limit or exhausts the queue. This design helps prevent deep or unbounded recursion when many objects become unreachable.

When a program is finished, the user should call shutdown(), which in turn calls free_all(), calling destructors on all not freed elements and destroying the hash table and linked list needed for garbage collection. 
# Slab [![Build Status](https://travis-ci.org/ernestrc/slab.svg)](https://travis-ci.org/ernestrc/slab)

Slab provides efficient memory allocation facilities to eliminate memory fragmentation caused by frequent allocations and deallocations.

## Usage
```c
test_struct* ptr1;
test_struct* ptr2;
test_struct* ptr3;

size_t slab_size = 2;

// allocate once
slab_t* my_slab = slab_create(slab_size, sizeof(test_struct));
// get storage without allocations
ptr1 = slab_get(my_slab);
ptr2 = slab_get(my_slab);
// ptr3 is NULL, since we ran out of slots
ptr3 = slab_get(my_slab);

// allocate a new block of slots
slab_reserve(my_slab, 10);
ptr3 = slab_get(my_slab);

// return storage
slab_put(my_slab, ptr1);
slab_put(my_slab, ptr2);
slab_put(my_slab, ptr3);

// free all
slab_free(my_slab);
```

You can compile the library with `-DLIBSLAB_AUTO` to enable auto-reserving when storage is exhausted:

```c
size_t slab_size = 2;

// allocate once
slab_t* my_slab = slab_create(slab_size, sizeof(test_struct));

// get storage without allocations
ptr1 = slab_get(my_slab);
ptr2 = slab_get(my_slab);
// automatic allocation
ptr3 = slab_get(my_slab);

// get storage without allocations
ptr4 = slab_get(my_slab);
...
```

# Contribute
If you would like to contribute to the project, please fork the project, include your changes and submit a pull request back to the main repository.

# License
MIT License

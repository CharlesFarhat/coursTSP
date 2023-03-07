#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <setjmp.h>
#include "cmocka.h"
#include "memory_alloc.h"

/* Initialize the memory allocator */
void memory_init() {
  // Init variables fields
    m.first_block = 0;
    m.available_blocks = 16;
    // Chain init
    for (int i=0; i<DEFAULT_SIZE-1; i++) {
        m.blocks[i] = i+1;
    }
    m.blocks[DEFAULT_SIZE-1] = NULL_BLOCK;
    m.error_no = E_SUCCESS; // Is it really necessary ?
}

/* Return the number of consecutive blocks starting from first */
int nb_consecutive_blocks(int first) {
    // Some check, for my sanity
    if (m.first_block == NULL_BLOCK) {
        return 0;
    }
    int nb_consec_block = 1;
    memory_page_t cur = first;
    while (m.blocks[cur] != NULL_BLOCK && m.blocks[cur] == cur + 1) {
        nb_consec_block++;
        cur = m.blocks[cur];
    }
  return nb_consec_block;
}

/* Reorder memory blocks */
void memory_reorder() {
    // Some default check
    if (m.first_block == NULL_BLOCK) {
        return;
    }
    // Some variables used later
    memory_page_t temp;
    memory_page_t current = m.first_block;
    memory_page_t next;
    memory_page_t previous = current;
    // implemented bubble sort for chained list !
    int swapped;
    do {
        swapped = 0;
        next = m.blocks[current];
        while (next != NULL_BLOCK) {
            if (current > next) {
                if (m.first_block == current)
                    m.first_block = next;
                else
                    m.blocks[previous] = next;
                temp = m.blocks[next];
                m.blocks[next] = current;
                m.blocks[current] = temp;
                previous = next;
                current = m.first_block; // start bubble sort from the beginning
                swapped = 1;
                break; // released loop to resume sorting
            } else {
                previous = current;
                current = next;
            }
            next = m.blocks[next];
        }
    } while (swapped);
}

/* Allocate size bytes
 * return -1 in case of an error
 */
int memory_allocate(size_t size) {
    // Convert size to number of requested blocks
    // Warning : long long to double conversion, but for our use it's OK
    int nb_block_requested = ceil(size / 8.0f);

    // first we check memory space, if it fail, exit.
    if (m.available_blocks < nb_block_requested) {
        m.error_no = E_NOMEM;
        return -1;
    }

    // start_id will be used as the starting point for our memory allocation
    int start_id = m.first_block;
    // Nb of consecutive blocks that can be allocated starting from our start_id
    int nb_available_blocks = 0;
    // The cursor used to jump when not enough blocks can be allocated
    int cur = -1;
    // We loop over the chained list
    while (start_id != NULL_BLOCK) {
         nb_available_blocks = nb_consecutive_blocks(start_id);
         // check if we found enough blocks to allocate
         if (nb_available_blocks >= nb_block_requested) {
             // We then correctly rearange the list
             // if cur == -1 means we can allocate from the start, we just need to change m.first_block
             // to where point the last block of the allocation. Otherwise we just change the last block
             // before the allocation to point to the last (as it is consecutive, we have it's location).
             if (cur == -1) {
                 m.first_block = m.blocks[start_id + nb_block_requested -1];
             } else {
                 m.blocks[cur] = start_id + nb_block_requested - 1;
             }
             // Allocate and change global variables to take the change into account
             m.available_blocks = m.available_blocks - nb_block_requested;
             initialize_buffer(start_id, size);
             m.error_no = E_SUCCESS;
             return start_id;
         }
         // Update the cursor to jump if not enough consecutive blocks
         cur = start_id + nb_available_blocks - 1;
         // Updtade the start_id to where point the cur block.
         start_id = m.blocks[cur];
    }
    // If we reach this, we didn't find consecutive blocks and
    // m.available_blocks > nb_block_requested (as we check earlier)
    // We then try to reorder
    printf("Reordering memory \n");
    memory_reorder();

    // We run again our system to find a place in memmory
    // Would have been easier to create a different function but I don't know if I am allowed to do that...
    int start_id_2 = m.first_block;
    cur = -1;
    while (start_id_2 != NULL_BLOCK) {
        nb_available_blocks = nb_consecutive_blocks(start_id_2);
        if (nb_available_blocks >= nb_block_requested) {
            if (cur == -1) {
                m.first_block = m.blocks[start_id_2 + nb_block_requested -1];
            } else {
                m.blocks[cur] = start_id_2 + nb_block_requested - 1;
            }
            m.available_blocks = m.available_blocks - nb_block_requested;
            initialize_buffer(start_id_2, size);
            m.error_no = E_SUCCESS;
            return start_id_2;
        }
        // Update the cursor to jump if not enough consecutive blocks
        cur = start_id_2 + nb_available_blocks - 1;
        // Updtade the start_id to where point the cur block.
        start_id_2 = m.blocks[cur];
    }

    m.error_no = E_SHOULD_PACK;
    return -1;
}

/* Free the block of data starting at address */
void memory_free(int address, size_t size) {
    // get number of chunk to free
    int nb_block_free = ceil(size / 8.0f);
    // relink free chunk, we consider here the consecutive assumption
    for (int i=0; i<nb_block_free-1; i++) {
        m.blocks[address + i] = address + i + 1;
    }
    // Link to start of the chain
    m.blocks[address + nb_block_free-1] = m.first_block;
    // Change new start
    m.first_block = address;
    // change parameters
    m.available_blocks += nb_block_free;
    m.error_no = E_SUCCESS;
}

/* Print information on the available blocks of the memory allocator */
void memory_print() {
  printf("---------------------------------\n");
  printf("\tBlock size: %lu\n", sizeof(m.blocks[0]));
  printf("\tAvailable blocks: %lu\n", m.available_blocks);
  printf("\tFirst free: %d\n", m.first_block);
  printf("\tStatus: "); memory_error_print(m.error_no);
  printf("\tContent:  ");

  // Affiche la chaine de blocks
  printf("[%d]", m.first_block);
  memory_page_t cur = m.first_block;
  while (cur != NULL_BLOCK) {
        printf(" -> ");
        if (m.blocks[cur] == NULL_BLOCK) {
            printf("NULL_BLOCK");
        } else {
            printf("[%lld]", m.blocks[cur]);
        }
        cur = m.blocks[cur];
    }
    printf("\n");

    printf("nb de block consecutifs : %d", nb_consecutive_blocks(5));
  printf("\n");
  printf("---------------------------------\n");
}


/* print the message corresponding to error_number */
void memory_error_print(enum memory_errno error_number) {
  switch(error_number) {
  case E_SUCCESS:
    printf("Success\n");
    break;
  case E_NOMEM:
    printf("Not enough memory\n");
    break;
  case  E_SHOULD_PACK:
    printf("Not enough contiguous blocks\n");
    break;
  default:
    break;
  }
}

/* Initialize an allocated buffer with zeros */
void initialize_buffer(int start_index, size_t size) {
  char* ptr = (char*)&m.blocks[start_index];
  for(int i=0; i<size; i++) {
    ptr[i]=0;
  }
}




/************* Test functions ********************/

/* write something in the buffer */
void use_buffer(int start_index, size_t size) {
  char* ptr = (char*)&m.blocks[start_index];
  for(int i=0; i<size; i++) {
    ptr[i] = rand() % 32;
  }
}

/* write something in the buffer */
void check_empty_buffer(int start_index, size_t size) {
  char* ptr = (char*)&m.blocks[start_index];
  for(int i=0; i<size; i++) {
    assert_int_equal(ptr[i], 0); // each byte of an allocated buffer should be 0
  }
}

/* allocate size bytes (or nb_pages pages), and free the allocated buffer
 * available_blocks is the number of available blocks at the beginning
 */
void test_alloc_free(size_t size, int nb_pages, int available_blocks) {
  /* allocate nb_pages pages */
  int res = memory_allocate(size);
  assert_int_equal(m.error_no, E_SUCCESS); // memory_allocate should be successfull
  assert_int_not_equal(res, -1); 		   // memory_allocate should return something != -1
  assert_int_equal(m.available_blocks, available_blocks-nb_pages); // all the blocks but nb_pages should be available
  assert(res >= 0 && res < DEFAULT_SIZE);

  use_buffer(res, size);

  /* free the allocated buffer */
  memory_free(res, size);
  assert_int_equal(m.available_blocks, available_blocks); // all the blocks should be available
}

/* allocate size bytes */
int test_alloc(size_t size) {
  int res = memory_allocate(size);
  assert_int_equal(m.error_no, E_SUCCESS); // memory_allocate should be successfull
  assert_int_not_equal(res, -1); 		   // memory_allocate should return something != -1
  assert_true(res >= 0 && res < DEFAULT_SIZE);
  check_empty_buffer(res, size);
  use_buffer(res, size);
  return res;
}

/* free the buffer located at address addr */
void test_memory_free(int addr, size_t size) {
  memory_free(addr, size);
}

void test_exo1_memory_init() {
 /* initialize memory */
  memory_init();
  assert_int_equal(m.available_blocks, DEFAULT_SIZE);   // all the blocks should be available
}

void test_exo1_memory_print() {
  test_exo1_memory_init();
  memory_print();
}

/* allocate size bytes (eg. nb_pages blocks) */
void test_alloc_blocks(size_t size, int nb_pages) {
  test_exo1_memory_init();

  int allocated_blocks;
  /* allocate a few blocks */
  allocated_blocks = test_alloc(size); /* allocate 1 byte */
  assert_int_equal(m.available_blocks, DEFAULT_SIZE-nb_pages);
  assert_in_range(allocated_blocks, 0, DEFAULT_SIZE);
}

/* allocate 1 byte */
void test_exo1_alloc_one_byte() {
  test_alloc_blocks(1, 1);
}

/* allocate 1 page */
void test_exo1_alloc_one_page() {
  test_alloc_blocks(sizeof(memory_page_t), 1);
}

/* allocate 2 pages */
void test_exo1_alloc_two_pages() {
  test_alloc_blocks(2*sizeof(memory_page_t), 2);
}

/* allocate a few blocks and free them */
void test_exo1_free_blocks() {
  test_exo1_memory_init();

  int allocated_blocks[DEFAULT_SIZE];
  /* allocate a few blocks */
  allocated_blocks[0] = test_alloc(1); /* allocate 1 byte */
  assert_int_equal(m.available_blocks, DEFAULT_SIZE-1);

  allocated_blocks[1] = test_alloc(sizeof(memory_page_t)); /* allocate 1 memory block */
  assert_int_equal(m.available_blocks, DEFAULT_SIZE-2);

  allocated_blocks[2] = test_alloc(2*sizeof(memory_page_t)); /* allocate 2 memory blocks */
  assert_int_equal(m.available_blocks, DEFAULT_SIZE-4);

  test_memory_free(allocated_blocks[0], 1); /* free 1 byte */

  test_memory_free(allocated_blocks[1], sizeof(memory_page_t)); /* free 1 block */

  test_memory_free(allocated_blocks[2], 2*sizeof(memory_page_t)); /* free 2 blocks */
  assert_int_equal(m.available_blocks, DEFAULT_SIZE);

  /* allocate a few blocks and free them */
  test_alloc_free(1, 1, DEFAULT_SIZE);                       /* allocate 1 byte */
  test_alloc_free(sizeof(memory_page_t), 1, DEFAULT_SIZE);   /* allocate 1 memory page */
  test_alloc_free(2*sizeof(memory_page_t), 2, DEFAULT_SIZE); /* allocate 2 memory page */
}

/* allocate multiple blocks and free them */
void test_exo1_multiple_alloc() {
  test_exo1_memory_init();

  /* test multiple allocations */
  int allocated_blocks[DEFAULT_SIZE];
  for(int i=0; i<DEFAULT_SIZE; i++) {
    allocated_blocks[i] = test_alloc(1); /* allocate 1 byte */
    assert_int_equal(m.available_blocks, DEFAULT_SIZE-(i+1));
  }
  // the available blocks should be something like:
  // NULL_BLOCK
  // memory_print();

  /* free all the allocated blocks */
  for(int i=0; i<DEFAULT_SIZE; i++) {
    test_memory_free(allocated_blocks[i], 1);
    assert_int_equal(m.available_blocks, i+1);
  }
}

/* test allocation when the system is running out of memory */
void test_exo1_out_of_memory() {
  test_exo1_memory_init();

  int allocated_blocks[DEFAULT_SIZE];
  /* First, use all the memory */
  for(int i=0; i<DEFAULT_SIZE; i++) {
    allocated_blocks[i] = test_alloc(1); /* allocate 1 byte */
    // these allocations should succeed
    assert_int_equal(m.available_blocks, DEFAULT_SIZE-(i+1));
  }
  assert_int_equal(m.available_blocks, 0); // no more memory

  /* Now, try to allocate one more byte */
  allocated_blocks[1] = memory_allocate(1);
  assert_int_equal(allocated_blocks[1], -1); // memory_allocate should return an error
  assert_int_equal(m.error_no, E_NOMEM); // memory_allocate should set error_no to E_NOMEM
  assert_int_equal(m.available_blocks, 0); // no more memory
}


/* Run a few tests for exercise 2.
 *
 * If you implemented correctly the functions, all these tests should be successfull
 * Of course this test suite may not cover all the tricky cases, and you are free to add
 * your own tests.
 */
void test_exo2_reorder() {
  test_exo1_memory_init();

  /* test multiple allocations */
  int allocated_blocks[DEFAULT_SIZE];
  for(int i=0; i<DEFAULT_SIZE; i++) {
    allocated_blocks[i] = test_alloc(1); /* allocate 1 byte */
    assert_int_equal(m.available_blocks, DEFAULT_SIZE-(i+1));
  }
  assert_int_equal(m.available_blocks, 0);

  /* free half of the allocated blocks */
  int freed_pages=0;
  for(int i=0; i<DEFAULT_SIZE; i+=2) {
    test_memory_free(allocated_blocks[i], 1);
    freed_pages++;
    assert_int_equal(m.available_blocks, freed_pages);
  }
  // The available blocks should be:
  // [0] -> [2] -> [4] -> [6] -> [8] -> [10] -> [12] -> [14] -> NULL_BLOCK
  memory_print();

  int res = memory_allocate(sizeof(memory_page_t)*2);
  memory_print();
  // allocation should fail as there's no 2 consecutive blocks
  assert_int_equal(res, -1);
  assert_int_equal(m.error_no, E_SHOULD_PACK);

  // free the remaining allocated blocks
  for(int i=1; i<DEFAULT_SIZE; i+=2) {
    test_memory_free(allocated_blocks[i], 1);
    freed_pages++;
    assert_int_equal(m.available_blocks, freed_pages);
  }
  // the available blocks should be something like:
  // [15] -> [13] -> [11] -> [9] -> [7] -> [5] -> [3] -> [1] -> [0] -> [2] -> [4] -> [6] -> [8] -> [10] -> [12] -> [14] -> NULL_BLOCK
  memory_print();

  // Now, there are 16 available blocks (but probably randomly distributed)
  // This call may trigger the memory reordering function, and successfully allocate 2 blocks
  res = memory_allocate(sizeof(memory_page_t)*2);
  // allocation should fail as there's no 2 consecutive blocks
  assert_int_equal(m.error_no, E_SUCCESS);

  // the available blocks should be something like:
  // [2] -> [3] -> [4] -> [5] -> [6] -> [7] -> [8] -> [9] -> [10] -> [11] -> [12] -> [13] -> [14] -> [15] -> NULL_BLOCK
  memory_print();
}

// Update test to check code
void test_did_you_really_code_well() {
    // We define a constant to store in a block which is supposed to be allocated
    // (the value of this constant is such as it is different form NULL_BLOCK *and*
    // it guarantees a segmentation vioaltion in case our code does something like
    // m.blocks[AB])
#define AB INT32_MIN

    // we define initial value of m as : [8] -> [9] -> [3] -> [4] -> [5] -> [12] -> [13] -> [14] -> [11] -> [1] -> NULL_BLOCK
    struct memory_alloc_t m_init = {
            //0            1   2   3   4   5   6   7   8   9  10  11  12  13  14  15
            {AB,  NULL_BLOCK, AB,  4,  5, 12, AB, AB,  9,  3, AB,  1, 13, 14, 11, AB},
            10,
            8,
            INT32_MIN
    };

    // Few tests on nb_consecutive_blocks()
    m = m_init; // Now m contains [8] -> [9] -> [3] -> [4] -> [5] -> [12] -> [13] -> [14] -> [11] -> [1] -> NULL_BLOCK
    assert_int_equal(nb_consecutive_blocks(8), 2);
    assert_int_equal(nb_consecutive_blocks(1), 1);
    assert_int_equal(nb_consecutive_blocks(12), 3);

    //
    // Few tests on memory_allocate
    //

    // We check that an allocation in the middle of the linked list works coreectly
    m = m_init; // Now m contains [8] -> [9] -> [3] -> [4] -> [5] -> [12] -> [13] -> [14] -> [11] -> [1] -> NULL_BLOCK
    assert_int_equal(memory_allocate(24), 3);
    // We check that if we try to allocate too many bytes compared to the number of bytes
    // available in m, the allocation fails and m.error_no is E_NOMEM
    m = m_init; // Now m contains [8] -> [9] -> [3] -> [4] -> [5] -> [12] -> [13] -> [14] -> [11] -> [1] -> NULL_BLOCK
    assert_int_equal(memory_allocate(1024), -1);
    assert_int_equal(m.error_no, E_NOMEM);
    // We check that if we try to allocate a number bytes which is lower than the number
    // of bytes available in m but greater than the number of consecutive bytes in m
    // even after reordering m.blocks, the allocation fails and m.error_no is E_SHOULD_PACK
    // NB : If you coded memory_reorder, this test will trigger invocation of memory_reorder.
    //      If your memory_reorder() does not work properly, maybe it would be a good idea
    //      to comment the two next assert_int_equal() (that is assert_int_equal(memory_allocate(33), -1);
    //      and assert_int_equal(m.error_no, E_SHOULD_PACK);)
    //      so that you can test if your memory_reorder works properly.
    m = m_init; // Now m contains [8] -> [9] -> [3] -> [4] -> [5] -> [12] -> [13] -> [14] -> [11] -> [1] -> NULL_BLOCK
    assert_int_equal(memory_allocate(33), -1);
    assert_int_equal(m.error_no, E_SHOULD_PACK);

    //
    // Test of memory_reorder()
    //
    m = m_init; // Now m contains [8] -> [9] -> [3] -> [4] -> [5] -> [12] -> [13] -> [14] -> [11] -> [1] -> NULL_BLOCK
    memory_reorder();
    // m should be : [1] -> [3] -> [4] -> [5] -> [8] -> [9] -> [11] -> [12] -> [13] -> [14] -> NULL_BLOCK
    assert_int_equal(m.first_block, 1);
    assert_int_equal(m.blocks[ 1],  3);
    assert_int_equal(m.blocks[ 3],  4);
    assert_int_equal(m.blocks[ 4],  5);
    assert_int_equal(m.blocks[ 5],  8);
    assert_int_equal(m.blocks[ 8],  9);
    assert_int_equal(m.blocks[ 9], 11);
    assert_int_equal(m.blocks[11], 12);
    assert_int_equal(m.blocks[12], 13);
    assert_int_equal(m.blocks[13], 14);
    assert_int_equal(m.blocks[14], NULL_BLOCK);

    //
    // We try an allocation which can succeed only after correct memory_reorder()
    //
    m = m_init; // Now m contains [8] -> [9] -> [3] -> [4] -> [5] -> [12] -> [13] -> [14] -> [11] -> [1] -> NULL_BLOCK
    assert_int_equal(memory_allocate(32), 11);
    assert_int_equal(m.error_no, E_SUCCESS);

    //
    // Final test: We check memory_free
    //
    m = m_init; // Now m contains [8] -> [9] -> [3] -> [4] -> [5] -> [12] -> [13] -> [14] -> [11] -> [1] -> NULL_BLOCK
    memory_free(6, 9);
    // m should contain [6] -> [7] -> [8] -> [9] -> [3] -> [4] -> [5] -> [12] -> [13] -> [14] -> [11] -> [1] -> NULL_BLOCK
    assert_int_equal(m.first_block, 6);
    assert_int_equal(m.blocks[ 6],  7);
    assert_int_equal(m.blocks[ 7],  8);
    assert_int_equal(m.blocks[ 8],  9);
    assert_int_equal(m.blocks[ 9],  3);
    assert_int_equal(m.blocks[ 3],  4);
    assert_int_equal(m.blocks[ 4],  5);
    assert_int_equal(m.blocks[ 5], 12);
    assert_int_equal(m.blocks[12], 13);
    assert_int_equal(m.blocks[13], 14);
    assert_int_equal(m.blocks[14], 11);
    assert_int_equal(m.blocks[11],  1);
    assert_int_equal(m.blocks[ 1], NULL_BLOCK);
    assert_int_equal(m.available_blocks, 12);
}

int main(int argc, char**argv) {
    const struct CMUnitTest tests[] = {
            /* a few tests for exercise 1.
             *
             * If you implemented correctly the functions, all these tests should be successfull
             * Of course this test suite may not cover all the tricky cases, and you are free to add
             * your own tests.
             */
            cmocka_unit_test(test_exo1_memory_init),
            cmocka_unit_test(test_exo1_memory_print),
            cmocka_unit_test(test_exo1_alloc_one_byte),
            cmocka_unit_test(test_exo1_alloc_one_page),
            cmocka_unit_test(test_exo1_alloc_two_pages),
            cmocka_unit_test(test_exo1_free_blocks),
            cmocka_unit_test(test_exo1_multiple_alloc),
            cmocka_unit_test(test_exo1_out_of_memory),

            /* Run a few tests for exercise 2.
             *
             * If you implemented correctly the functions, all these tests should be successfull
             * Of course this test suite may not cover all the tricky cases, and you are free to add
             * your own tests.
             */

            cmocka_unit_test(test_exo2_reorder),
            cmocka_unit_test(test_did_you_really_code_well)

    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}

// Nisal Panditha
// CMPT 201 lab 5

// Part 1:
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct header {
  uint64_t size;
  struct header *next;
  int id;
};

void initialize_block(struct header *block, uint64_t size, struct header *next,
                      int id) {
  block->size = size;
  block->next = next;
  block->id = id;
}

int find_first_fit(struct header *free_list_ptr, uint64_t size) {
  // First-fit: return the ID of the first block with size >= requested size
  struct header *cur = free_list_ptr;
  while (cur != NULL) {
    if (cur->size >= size) {
      return cur->id;
    }
    cur = cur->next;
  }
  return -1; // not found
}

int find_best_fit(struct header *free_list_ptr, uint64_t size) {
  // Best-fit: smallest block that still fits (size >= requested size)
  int best_fit_id = -1;
  uint64_t best_size = 0; // only valid when best_fit_id != -1

  struct header *cur = free_list_ptr;
  while (cur != NULL) {
    if (cur->size >= size) {
      if (best_fit_id == -1 || cur->size < best_size) {
        best_fit_id = cur->id;
        best_size = cur->size;
      }
    }
    cur = cur->next;
  }
  return best_fit_id;
}

int find_worst_fit(struct header *free_list_ptr, uint64_t size) {
  // Worst-fit: largest block that fits (size >= requested size)
  int worst_fit_id = -1;
  uint64_t worst_size = 0;

  struct header *cur = free_list_ptr;
  while (cur != NULL) {
    if (cur->size >= size) {
      if (worst_fit_id == -1 || cur->size > worst_size) {
        worst_fit_id = cur->id;
        worst_size = cur->size;
      }
    }
    cur = cur->next;
  }
  return worst_fit_id;
}

int main(void) {

  struct header *free_block1 = (struct header *)malloc(sizeof(struct header));
  struct header *free_block2 = (struct header *)malloc(sizeof(struct header));
  struct header *free_block3 = (struct header *)malloc(sizeof(struct header));
  struct header *free_block4 = (struct header *)malloc(sizeof(struct header));
  struct header *free_block5 = (struct header *)malloc(sizeof(struct header));

  initialize_block(free_block1, 6, free_block2, 1);
  initialize_block(free_block2, 12, free_block3, 2);
  initialize_block(free_block3, 24, free_block4, 3);
  initialize_block(free_block4, 8, free_block5, 4);
  initialize_block(free_block5, 4, NULL, 5);

  struct header *free_list_ptr = free_block1;

  int first_fit_id = find_first_fit(free_list_ptr, 7);
  int best_fit_id = find_best_fit(free_list_ptr, 7);
  int worst_fit_id = find_worst_fit(free_list_ptr, 7);

  printf("The ID for First-Fit algorithm is: %d\n", first_fit_id);
  printf("The ID for Best-Fit algorithm is: %d\n", best_fit_id);
  printf("The ID for Worst-Fit algorithm is: %d\n", worst_fit_id);

  // Clean up (good practice)
  free(free_block1);
  free(free_block2);
  free(free_block3);
  free(free_block4);
  free(free_block5);

  return 0;
}

/*
Part 2:
Pseudo-code:

function coalesce_insert(free_list_head, newly_freed):
  if free_list_head is NULL:
     newly_freed.next = NULL
     return newly_freed

  // 1) Insert newly_freed into the free list in address-sorted order
  prev = NULL
  cur = free_list_head
  while cur != NULL and cur_address(cur) < cur_address(newly_freed):
      prev = cur
      cur = cur.next

  newly_freed.next = cur
  if prev == NULL:
      free_list_head = newly_freed
  else:
      prev.next = newly_freed

  // 2) Try coalescing with the next block (right neighbor)

  next = newly_freed.next
  if next != NULL and end_address(newly_freed) == start_address(next):
      newly_freed.size = newly_freed.size + header_size + next.size
      newly_freed.next = next.next   // remove next from list

  // 3) Try coalescing with the previous block (left neighbor)

  if prev != NULL and end_address(prev) == start_address(newly_freed):
      prev.size = prev.size + header_size + newly_freed.size
      prev.next = newly_freed.next   // remove newly_freed from list
      newly_freed = prev            // merged block is now prev

      // 4) After merging with prev, you may also need to merge again with its
next next = newly_freed.next if next != NULL and end_address(newly_freed) ==
start_address(next): newly_freed.size = newly_freed.size + header_size +
next.size newly_freed.next = next.next

  return free_list_head
*/

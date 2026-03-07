#include <stdio.h>
#include <stdlib.h>

#define PAGE_SIZE 1024
#define NUM_FRAMES 16

int main(void){
    //one process, 4 pages
    const int number_pages = 4;

    //Hardcoded page -> frame
    int page_table[] = {5, 2, 9, 1};

    //Print page table (messages)
    printf("Paging config:\n");
    printf("PAGE_SIZE = %d bytes \n", PAGE_SIZE);
    printf("NUM_FRAMES = %d (Physical memory = %d bytes)\n", NUM_FRAMES, NUM_FRAMES * PAGE_SIZE);

    //Print page table
    printf("Page Table:\n");
    for (int i = 0; i < number_pages; ++i){
        printf(" page %d -> frame %d\n", i, page_table[i]);
    }
    printf("\n");

    //get user input for number of logical addresses
    int n;
    printf("Enter number of logical addresses (N): ");
    if (!scanf("%d", &n)){
        fprintf(stderr, "Invalid N.\n");
        return EXIT_FAILURE;
    }

    
    long logical, page, offset, physical;
    int frame;
    //prompt user for input
    printf("Enter %d logical addresses, one per line: \n", n);
    for (int i = 0; i < n; ++i){
        //get user input for each logical address
        if (!scanf("%ld", &logical)){
            fprintf(stderr, "Invalid logical address input..\n");
            return EXIT_FAILURE;
        }

        //make sure address is valid
        if (logical < 0){
            printf(" [#%d] logical = %ld -> INVALID (negative address) \n", i, logical);
            continue;
        }

        //calculate page and offset
        page = logical / PAGE_SIZE;
        offset = logical % PAGE_SIZE;
        //print results
        printf(" [#%d] logical = %ld -> page = %ld, offset = %ld", i, logical, page, offset);

        //check that address is in range
        if (page >= number_pages){
            printf(" -> INVALID (page %ld out of range; valid pages.. %d) \n", page, number_pages - 1);
            continue;
        }

        //get frame
        frame = page_table[page];
        //check that page is valid
        if (frame < 0){
            printf(" -> Page %ld INVALID \n", page);
            continue;
        }

        //calculate final physical address
        physical = (long)frame * PAGE_SIZE + offset;
        //print results
        printf(" -> frame = %d physical = %ld\n", frame, physical);
    }
    return EXIT_SUCCESS;
}


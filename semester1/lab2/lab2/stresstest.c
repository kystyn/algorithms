#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "memallocator.h"

#define SIZE 100000
#define SAMPLES 20000

void main( void ) {
  /*
  void *mem = malloc(SIZE);
  void *ptr[SAMPLES];
  int i = 0;

  memset(mem, 0, SIZE);

  meminit(mem, SIZE);
  ptr[0] = memalloc((rand() % 6) * 4 + 1);
  ptr[1] = memalloc((rand() % 6) * 4 + 1);
  ptr[2] = memalloc((rand() % 6) * 4 + 1);
  ptr[3] = memalloc((rand() % 6) * 4 + 1);
  ptr[4] = memalloc((rand() % 6) * 4 + 1);
  ptr[5] = memalloc((rand() % 6) * 4 + 1);
  ptr[6] = memalloc((rand() % 6) * 4 + 1);
  ptr[7] = memalloc((rand() % 6) * 4 + 1);
  ptr[8] = memalloc((rand() % 6) * 4 + 1);
  ptr[9] = memalloc((rand() % 6) * 4 + 1);

  memfree(ptr[5]);
  memfree(ptr[1]);
  memfree(ptr[7]);
  memfree(ptr[3]);
  memfree(ptr[4]);
  memfree(ptr[6]);
  memfree(ptr[2]);
  memfree(ptr[9]);
  memfree(ptr[0]);
  memfree(ptr[8]);
  */
  
  //ptr[0] = memalloc(12);
  //ptr[1] = memalloc(7);
  //memfree(ptr[1]);
  //ptr[2] = memalloc(6);
  //memfree(ptr[0]);
  //memfree(ptr[2]);
  //ptr[3] = memalloc(13);
  //memfree(ptr[3]);

  void *mem = malloc(SIZE);
  void *ptrs[SAMPLES];
  int i;

  if (mem == 0)
    // Allocation failed
    return;
  //memset(mem, 0, SIZE);

  srand(time(0));

  meminit(mem, SIZE);

  for (i = 0; i < SAMPLES; i++) {
    int a = (rand() % 6 + 1) * 4;
    if (a < 4)
      printf("ALARRMMMMM!!!!");
    ptrs[i] = memalloc(a);
    if (ptrs[i] != 0)
      *(int *)ptrs[i] = i;
  }

  memfree(ptrs[7]);
  for (i = 0; i < SAMPLES; i++) {
    int r = rand() % SAMPLES;
    if (ptrs[r] != 0) {
      memfree(ptrs[r]);
      ptrs[r] = 0;
    }
  }

  for (i = 0; i < SAMPLES; i++)
    if (ptrs[i] == 0) {
      ptrs[i] = memalloc((rand() % 6 + 1) * 4);
      if (ptrs[i] != 0)
        *(int *)ptrs[i] = i;
    }

  for (i = 0; i < SAMPLES * SAMPLES; i++) {
    int r = rand() % SAMPLES;
    if (ptrs[r] != 0) {
      memfree(ptrs[r]);
      ptrs[r] = 0;
    }
  }
                             /*
  for (i = 0; i < SAMPLES; i++)
    if (ptrs[i] == 0) {
      ptrs[i] = memalloc((rand() % 6 + 1) * 4);
      if (ptrs[i] != 0)
        *(int *)ptrs[i] = i;
    }

  for (i = 0; i < SAMPLES * SAMPLES * SAMPLES; i++) {
    int r = rand() % SAMPLES;
    if (ptrs[r] != 0) {
      memfree(ptrs[r]);
      ptrs[r] = 0;
    }
  }                        */

  memdone();
  free(mem);

}

#if 0
#include<stdio.h>
#include<stdlib.h>
#include"memallocator.h"
#define NUM (20)
#define TESTNUM (2000)
#define __MY__CONFIGURATION__ (RELEASE)
/*memallocator stress test
 *compatible with any type of memallocator (one-dir/two-dir, best-/first-fit)
 *there is no try-catch mechanism in C, so you may only detect an error via
 *receiving "<myproject> has stopped working" message, or in memdone
 *copyleft ZUEVV 2018, all rights violated*/

int main(void) {
	enum {
		RELEASE,
		DEBUG1,
		DEBUG2
	} myconfig;
	int len = memgetminimumsize();
	char **a = (char **)malloc(sizeof(char *)*NUM);
	int sizes[NUM];
	int i, j, k, r;
	int allocSensor = 0;
	int freeSensor = 0;
	void *pMem;
	myconfig = __MY__CONFIGURATION__;
	sizes[0] = 0; //zero-len block
	for (i = 1; i < NUM; i++) {
		sizes[i] = i;
		len += i + memgetblocksize();
	}
	pMem = malloc(len);
	meminit(pMem, len);
	for (i = 0; i < TESTNUM; i++) {
		for (j = 0; j < NUM; j++) {
			r = rand() % 2;
			if (r && sizes[j] > 0) {
				if (myconfig == DEBUG2)
					printf("i=%d, j=%d, allocSensor=%d; starting memalloc\n", i, j, allocSensor);
				sizes[j] = -1;
				a[j] = (char *)memalloc(j);
				allocSensor++;
				if (myconfig == DEBUG1)
					printf("Allocation no. %d, everything's OK\n", allocSensor);
				else if (myconfig == DEBUG2)
					printf("i=%d, j=%d, allocSensor=%d; memalloc finished\n", i, j, allocSensor);
				for (k = 0; k < j; k++) {
					if (myconfig == DEBUG2)
						printf("i=%d, j=%d, k=%d; starting filling data\n", i, j, k);
					a[j][k] = 'a'; //filling in user's data
					if (myconfig == DEBUG2)
						printf("i=%d, j=%d, k=%d; data cell filled\n", i, j, k);
				}
			}
		}
		for (j = 0; j < NUM; j++) {
			printf("%d\n", r);
			if (r&&sizes[j] < 0) {
				if (myconfig == DEBUG2)
					printf("i=%d, j=%d, freeSensor=%d; starting memfree\n", i, j, freeSensor);
				sizes[j] = j;
				memfree(a[j]);
				freeSensor++;
				if(myconfig == DEBUG1)
					printf("Free No. %d, everything's OK\n", i);
				else if (myconfig == DEBUG2)
					printf("i=%d, j=%d, freeSensor=%d; memfree finished\n", i, j, freeSensor);
			}
		}
	}
	for (j = 0; j < NUM; j++) {
		if (sizes[j] < 0) {
			sizes[j] = j;
			memfree(a[j]);
			freeSensor++;
			if (myconfig == DEBUG1)
				printf("Free No. %d, everything's OK\n", freeSensor);
		}
	}
	memdone();
	free(a);
	free(pMem);
	return 0;

}
#endif /* 0 */
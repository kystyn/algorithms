#include <stdio.h>

#include "planner.h"
#include "gtest/gtest.h"

/* Memory leak checker */
#ifdef _DEBUG
# include <crtdbg.h>
# define SetDbgMemHooks() \
  _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF | \
  _CRTDBG_ALLOC_MEM_DF | _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG))
#endif /* _DEBUG */

/* Scanner unit test: basic functional. */
TEST(Scanner, TestBasicFunctional) {
  project p = scan("test1.in");

  EXPECT_TRUE(p.deadline == 3 &&p.numWorkers == 2 && p.numTask == 5);
  freeProject(&p);
  //EXPECT_EQ(2, sqrt(4));
} /* End of 'Scanner Test1' test */

/* Scanner unit test: unexisting file. */
TEST(Scanner, TestUnexistingFile) {
  project p = scan("unexisting.in");

  EXPECT_TRUE(p.deadline == 0 &&p.numWorkers == 0 && p.numTask == 0);
  freeProject(&p);
} /* End of 'Scanner Test2' test */

/* Generate of previous permutation test : last '1' */
TEST(PreviousPermutationGenerator, Last_digit_is_1) {
  char mask[] = {1, 0, 0, 1};
  char newmask[] = {1, 0, 0, 0};
  generatePreviousMask(mask, sizeof(mask) / sizeof(mask[0]));
  EXPECT_FALSE(strcmp(mask, newmask));
} /* End of 'PermutationGenerator Test1' test */

/* Generate of previous permutation test : last '0' */
TEST(PreviousPermutationGenerator, Last_digit_is_0) {
  char mask[] = {1, 0, 1, 0, 0, 0};
  char newmask[] = {1, 0, 0, 1, 1};
  generatePreviousMask(mask, sizeof(mask) / sizeof(mask[0]));
  EXPECT_FALSE(strcmp(mask, newmask));
} /* End of 'PermutationGenerator Test2' test */

/* Summarizer of task length test. */
TEST(TaskLengthSummarizerTest, Test1) {
  project p = { 0 };
  // we have tasks of such length
  int lens[] =     {1, 2, 3, 4, 5, 6, 7};
  // we should summarize tasks with indices mentioned if leftover[] and where mask is true
  char mask[] =    {0, 1, 0, 1, 1, 0, 1}; // really it is 1, 0, 1, 1, 0, 1, 0
  // so we will check tasks lens[1], lens[2], lens[4]
  int leftover[] = {1, 2, 4};
  int taskIdx[MAX_TASK_LEN + 1];
  int sum;

  memcpy(p.taskLength, lens, sizeof(lens));
  p.numTask = sizeof(lens) / sizeof(int);
  sum = summarize(&p, mask, leftover, sizeof(leftover) / sizeof(leftover[0]), 1, taskIdx);
  ASSERT_TRUE(sum == 3 && taskIdx[0] == 2 && taskIdx[1] == -1);
  freeProject(&p);
} /* End of 'TaskLengthSummarizerTest, Test1' test */

/* Summarizer of task length test. */
TEST(TaskLengthSummarizerTest, Test2) {
  project p = { 0 };
  // we have tasks of such length
  int lens[] =     {1, 2, 3, 4, 5, 6, 7};
  // we should summarize tasks with indices mentioned if leftover[] and where mask is true
  char mask[] =    {0, 1, 1, 0, 1, 0, 1}; // really it is 1, 0, 1, 0, 1, 1, 0
  // so we will check inlu tasks lens[1], lens[2], lens[4]
  int leftover[] = {1, 2, 4, 5};
  int taskIdx[MAX_TASK_LEN + 1];
  int sum;

  memcpy(p.taskLength, lens, sizeof(lens));

  p.numTask = sizeof(lens) / sizeof(int);
  sum = summarize(&p, mask, leftover, sizeof(leftover) / sizeof(leftover[0]), 1, taskIdx);
  ASSERT_TRUE(sum == 8 && taskIdx[0] == 2 && taskIdx[1] == 4 && 
    taskIdx[2] == -1);
  freeProject(&p);
} /* End of 'TaskLengthSummarizerTest, Test2' test */

/* Test distribution of tasks */
TEST(TasksDistribution, SmallTest1) {
  project p = { 0 };
  int i, sum1 = 0, sum2 = 0;
  int tlen[] = {1, 1, 3, 2, 1};

  p.numTask = 5;
  p.deadline = 4;
  p.numWorkers = 2;

  memcpy(p.taskLength, tlen, sizeof(tlen));

  p.tasks = (int **)malloc(p.numWorkers * sizeof(int *));
  if (p.tasks == NULL) if (p.tasks == NULL) {
    printf("No memory! Exiting...\n");
    exit(-1);
  }

  for (i = 0; i < p.numWorkers; i++) {
    p.tasks[i] = (int *)malloc(sizeof(int) * p.numTask);

    if (p.tasks == NULL) {
      printf("No memory! Exiting...\n");
      exit(-1);
    }

    memset(p.tasks[i], 0, sizeof(int) * p.numTask);
  }

  formPlan(&p);

  // for visual check
  outputResult("smalltest1.out", &p);

  // Check that sum for each worker is less than deadline
  for (i = 0; i < p.numTask; i++) {
    sum1 += p.tasks[0][i];
    sum2 += p.tasks[1][i];
  }
  ASSERT_TRUE(sum1 <= p.deadline && sum2 <= p.deadline);

  freeProject(&p);
}

/* Test distribution of tasks */
TEST(TasksDistribution, SmallTest2) {
  project p = { 0 };
  int i, sum1 = 0, sum2 = 0, sum3 = 0;
  int tlen[] = {1, 1, 4, 5, 3, 2, 2, 3, 3, 1, 1, 4, 5, 2, 2, 3};

  p.numTask = 16;
  p.deadline = 14;
  p.numWorkers = 3;

  memcpy(p.taskLength, tlen, sizeof(tlen));

  p.tasks = (int **)malloc(p.numWorkers * sizeof(int *));

  if (p.tasks == NULL) {
    printf("No memory! Exiting...\n");
    exit(-1);
  }


  for (i = 0; i < p.numWorkers; i++) {
    p.tasks[i] = (int *)malloc(sizeof(int) * p.numTask);

    if (p.tasks[i] == NULL) {
      printf("No memory! Exiting...\n");
      exit(-1);
    }

    memset(p.tasks[i], 0, sizeof(int) * p.numTask);
  }

  formPlan(&p);
  // for visual check
  outputResult("smalltest2.out", &p);

  // Check that sum for each worker is less than deadline
  for (i = 0; i < p.numTask; i++) {
    sum1 += p.tasks[0][i];
    sum2 += p.tasks[1][i];
    sum3 += p.tasks[2][i];
  }
  ASSERT_TRUE(sum1 <= p.deadline && sum2 <= p.deadline && sum3 <= p.deadline);

  freeProject(&p);
}

/* Test distribution of tasks */
TEST(TasksDistribution, BigTest_1000) {
  project p = { 0 };
  int i,sum1 = 0, sum2 = 0;

  p.numTask = 1000;
  p.deadline = 999;
  p.numWorkers = 2;

  for (i = 0; i < p.numTask; i++)
    p.taskLength[i] = 1;

  p.tasks = (int **)malloc(p.numWorkers * sizeof(int *));

  if (p.tasks == NULL) {
    printf("No memory! Exiting...\n");
    exit(-1);
  }

  for (i = 0; i < p.numWorkers; i++) {
    p.tasks[i] = (int *)malloc(sizeof(int) * p.numTask);

    if (p.tasks[i] == NULL) {
      printf("No memory! Exiting...\n");
      exit(-1);
    }

    memset(p.tasks[i], 0, sizeof(int) * p.numTask);
  }

  formPlan(&p);
  // for visual check
  outputResult("bigtest.out", &p);

  // Check that sum for each worker is less than deadline
  for (i = 0; i < p.numTask; i++) {
    sum1 += p.tasks[0][i];
    sum2 += p.tasks[1][i];
  }
  ASSERT_TRUE(sum1 <= p.deadline && sum2 <= p.deadline);

  freeProject(&p);
}


int main( int argc, char *argv[] ) {
#ifdef _DEBUG
    SetDbgMemHooks();
#endif
    malloc(1);
    testing::InitGoogleTest(&argc, argv);
    RUN_ALL_TESTS();

  return 0;
}

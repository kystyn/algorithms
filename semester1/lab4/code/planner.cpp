#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "planner.h"

/* Scan project info function.
 * ARGUMENTS:
 *   - file name:
 *       char *fileName;
 * RETURNS:
 *   (project) project info.
 */
project scan( char *fileName ) {
  project p = { { 0 }, 0, 0, 0, NULL };
  FILE *F = fopen(fileName, "rt");
  int i;

  if (F == NULL)
    return p;

  fscanf(F,"%i%i%i", &p.numTask, &p.deadline, &p.numWorkers);

  p.tasks = (int **)malloc(p.numWorkers * sizeof(int *));

  if (p.tasks == NULL)
    return p;

  for (i = 0; i < p.numWorkers; i++) {
    p.tasks[i] = (int *)malloc(sizeof(int) * p.numTask);

    if (p.tasks[i] == NULL)
      return p;

    memset(p.tasks[i], 0, sizeof(int) * p.numTask);
  }

  for (i = 0; i < p.numTask; i++)
    fscanf(F, "%i", &p.taskLength[i]);

  fclose(F);

  return p;
} /* End of 'scan' function */

/* Summarize values of the following permutation function.
 * ARGUMENTS:
 *   - project:
 *       project *p;
 *   - mask (permutation):
 *       char *mask;
 *   - leftover array:
 *       int *leftoverIdx;
 *   - leftover size:
 *       int loSize;
 *   - flag to generate the list of values or not to:
 *       int toSet;
 *   - task indices (size of array is MAX_TASK_LEN + 1!!!)
 */
int summarize( project *p, char *mask, int *leftoverIdx, int loSize, int toSet, int *taskIdx ) {
  int s = 0;
  int i, c = 0;

  for (i = 0; i < loSize; i++)
    if (mask[loSize - 1 - i]) {
      s += p->taskLength[leftoverIdx[i]];

      if (toSet)
        taskIdx[c++] = leftoverIdx[i];
    }
  taskIdx[c] = -1;

  return s;
} /* End of 'summarize' function */

/* Generate the following mask function.
 * ARGUMENTS:
 *   - current mask (to be changed):
 *       char *Mask;
 *   - mask size:
 *       int maskSize;
 * RETURNS: None.
 */
void generateFollowingMask( char *mask, int maskSize ) {
  int i;

  for (i = maskSize - 1; i >= 0; i--) {
    mask[i] = !mask[i];
    if (mask[i]) // before addition - 0
      return;
  }
} /* End of 'generateFollowingMask' function */

/* Generate the previous mask function.
 * ARGUMENTS:
 *   - current mask (to be changed):
 *       char *Mask;
 *   - mask size:
 *       int maskSize;
 * RETURNS: None.
 */
void generatePreviousMask( char *mask, int maskSize ) {
  int i;

  for (i = maskSize - 1; i >= 0; i--) {
    mask[i] = !mask[i];
    if (!mask[i]) // before substitution - 1
      return;
  }
} /* End of 'generatePreviousMask' function */

/* Generate leftover task array function. 
 *   [array idx] -> task idx
 * ARGUMENTS:
 *   - project:
 *       project *p;
 *   - array to fill:
 *       int *arr;
 *   - array size to fill:
 *       int *arrSize;
 * RETURNS: None.
 */
void generateLeftoverTaskArray( project *p, int *arr, int *arrSize ) {
  int idx = 0, i;

  for (i = 0; i < p->numTask; i++)
    if (!p->distributed[i])
      arr[idx++] = i;

  *arrSize = idx;
} /* End of 'generateLeftoverTaskArray' function */

/* Distribute tasks to one programmer function.
 * ARGUMENTS:
 *   - project:
 *       project *p;
 *   - programmer number:
 *       int programmerNum;
 * RETURNS: None.
 */
void distribute( project *p, int programmerNum ) {
  int bestSetSum = 0;
  int bestSet[MAX_TASK_LEN + 1], x;
  char
    *mask, *zeroMask;
  int leftover[MAX_TASK_LEN] = { 0 };
  int loSize = 0;

  generateLeftoverTaskArray(p, leftover, &loSize);

  mask = (char *)malloc(loSize);
  zeroMask = (char *)malloc(loSize);

  if (mask == NULL || zeroMask == NULL) {
    printf("ALARM! No mem\n");
    return;
  }

  memset(mask, 1, loSize);
  memset(zeroMask, 0, loSize);

  // evaluate best subconsequence
  for (; strcmp(mask, zeroMask) != 0; generatePreviousMask(mask, loSize)) {
    int summ = summarize(p, mask, leftover, loSize, 0, bestSet);

    if (summ <= p->deadline && summ > bestSetSum) {
      memset(bestSet, 0, sizeof(int) * (MAX_TASK_LEN + 1));
      summarize(p, mask, leftover, loSize, 1, bestSet);
      bestSetSum = summ;
    }

    if (summ == p->deadline)
      break;
  }

  // notice best subcons
  for (x = 0; bestSet[x] != -1; x++) {
    p->tasks[programmerNum][bestSet[x]] = 1;
    p->distributed[bestSet[x]] = 1;
  }

  // free all
  free(mask);
  free(zeroMask);
} /* End of 'distribute' function */

/* Form plan function.
 * ARGUMENTS:
 *   - project:
 *       project *p;
 * RETURNS: None.
 */
void formPlan( project *p ) {
  int progNo;

  for (progNo = 0; progNo < p->numWorkers; progNo++)
    distribute(p, progNo);
} /* End of 'formPlan' function */

/* Output result function.
 * ARGUMENTS:
 *   - file name to output:
 *       char *fileName;
 *   - project:
 *       project *p;
 * RETURNS: None.
 */
void outputResult( char *fileName, project *p ) {
  FILE *F = fopen(fileName, "wt");
  int i, j;

  if (p->tasks == NULL) {
    fprintf(F, "0\n");
    return;
  }

  // check possibility
  for (i = 0; i < p->numTask; i++)
    if (!p->distributed[i]) {
      fprintf(F, "0\n");
      return;
    }

  for (i = 0; i < p->numWorkers; i++) {
    fprintf(F, "%i ", i + 1);
    for (j = 0; j < p->numTask; j++)
      if (p->tasks[i][j])
        fprintf(F, "%i ", j + 1);
    fprintf(F, "\n");
  }

  fclose(F);
} /* End of 'outputResult' function */

/* Output result function.
 * ARGUMENTS:
 *   - project:
 *       project *p;
 * RETURNS: None.
 */
void freeProject( project *p ) {
  int i;

  for (i = 0; i < p->numWorkers; i++)
    if (p->tasks[i] != NULL)
      free(p->tasks[i]);
  if (p->tasks != NULL)
    free(p->tasks);
} /* End of 'freeProject' function */

#pragma once

//#ifdef __cplusplus
//extern "C" {
//#endif

#define MAX_TASK_LEN 1000
#define MAX_PROGRAMMERS (MAX_TASK_LEN - 1)

/* Project structure */
typedef struct {
  int taskLength[MAX_TASK_LEN];
  int numTask;
  int numWorkers;
  int deadline;
  int **tasks; // idx is programmer number
  int distributed[MAX_TASK_LEN];
} project; /* End of 'project' structure */

///extern list *Set;

/* Scan project info function.
 * ARGUMENTS:
 *   - file name:
 *       char *fileName;
 * RETURNS:
 *   (project) project info.
 */
project scan( char *fileName );

/* Form plan function.
 * ARGUMENTS:
 *   - project:
 *       project *p;
 * RETURNS: None.
 */
void formPlan( project *p );

/* Output result function.
 * ARGUMENTS:
 *   - file name to output:
 *       char *fileName;
 *   - project:
 *       project *p;
 * RETURNS: None.
 */
void outputResult( char *fileName, project *p );

/* Output result function.
 * ARGUMENTS:
 *   - project:
 *       project *p;
 * RETURNS: None.
 */
void freeProject( project *p );

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
int summarize( project *p, char *mask, int *leftoverIdx, int loSize, int toSet, int *taskIdx );

/* Generate the following mask function.
 * ARGUMENTS:
 *   - current mask (to be changed):
 *       char *Mask;
 *   - mask size:
 *       int maskSize;
 * RETURNS: None.
 */
void generateFollowingMask( char *mask, int maskSize );

/* Generate the previous mask function.
 * ARGUMENTS:
 *   - current mask (to be changed):
 *       char *Mask;
 *   - mask size:
 *       int maskSize;
 * RETURNS: None.
 */
void generatePreviousMask( char *mask, int maskSize );

//#ifdef __cplusplus
//}
//#endif
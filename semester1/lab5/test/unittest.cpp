#include <stdio.h>

#include "memchk.h"
#include "heap.h"

/* Simple cycled heap generator test */
TEST(HeapGenerator, Test1) {
  heap h1 = { NULL };

  pushNode(&h1, 1);
  pushNode(&h1, 5);
  pushNode(&h1, 6);

  /******************************
   *           1 _              *
   *    down> / |\  <up         *
   *        |/_   \             *
   *        5----->6 x <down    *
   *           ^                *
   *          up                *
   ******************************/

  ASSERT_TRUE(
    h1.root->key == 1 &&
    h1.root->down->key == 5 &&
    h1.root->down->up->key == 6 &&
    h1.root->down->up->up->key == 1 &&
    h1.majorPath->key == 6 &&
    h1.majorPath->up->key == 1);

  clearHeap(&h1);
} /* End of 'HeapGenerator, Test1' test */

/* Heap checker test.
 * Check that heap structure is OK and
 * no element is lost.
 */
TEST(HeapChecker__Gen__Extract, Test1) {
  int prev = 0, cur = 0, n = 12; // n is heap size
  heap h1 = { NULL };

  // Generate difficult heap
  pushNode(&h1, 1);
  pushNode(&h1, 5);
  pushNode(&h1, 6);
  pushNode(&h1, 7);
  pushNode(&h1, 10);
  pushNode(&h1, 8);
  pushNode(&h1, 9);
  pushNode(&h1, 4);
  pushNode(&h1, 3);
  pushNode(&h1, 2);
  pushNode(&h1, 20);
  pushNode(&h1, 18);

  while (h1.root != NULL) {
    cur = extractMin(&h1);
    ASSERT_TRUE(cur >= prev);
    prev = cur;
    n--;
  }
  ASSERT_EQ(n, 0);
} /* End of 'HeapChecker, Test1' test */

TEST(DeleteNodeUpParent, LeafOnlyChild) {
  heap h1 = { NULL };

  /************
   *     1    *
   *  5     6 *
   *       7  *
   ************/

  pushNode(&h1, 1);
  pushNode(&h1, 5);
  pushNode(&h1, 6);
  node *d = pushNode(&h1, 7);

  int n = deleteNode(&h1, d);
  ASSERT_EQ(n, 7);
  /*****************
   * After delete: *
   *      1        *
   *   5     6     *
   *****************/
  ASSERT_TRUE(h1.root->key == 1);
  ASSERT_TRUE(h1.root->down->key == 5);
  ASSERT_TRUE(h1.root->down->up->key == 6);
  ASSERT_TRUE(h1.root->down->up->up->key == 1);
  ASSERT_TRUE(h1.majorPath->key == 6);
  ASSERT_TRUE(h1.majorPath->up->key == 1);

  clearHeap(&h1);
}

TEST(DeleteNodeUpSibling, LeafSibling) {
  heap h1 = { NULL };

  /**************
   *     1      *
   *  5     6   *
   *      7   8 *
   *            *
   **************/

  pushNode(&h1, 1);
  pushNode(&h1, 5);
  pushNode(&h1, 6);
  node *d = pushNode(&h1, 7);
  pushNode(&h1, 8);
  int n = deleteNode(&h1, d);
  ASSERT_EQ(n, 7);
  /*****************
   * After delete: *
   *      1        *
   *   5     6     *
   *       8       *
   *****************/
  ASSERT_TRUE(h1.root->key == 1);
  ASSERT_TRUE(h1.root->down->key == 5);
  ASSERT_TRUE(h1.root->down->up->key == 6);
  ASSERT_TRUE(h1.root->down->up->up->key == 1);
  ASSERT_TRUE(h1.root->down->up->down->key == 8);
  ASSERT_TRUE(h1.root->down->up->down->up->key == 6);
  ASSERT_TRUE(h1.majorPath->key == 6);
  ASSERT_TRUE(h1.majorPath->up->key == 1);

  clearHeap(&h1);
} /* End of 'DeleteNode, LeafSibling' test */

TEST(DeleteNodeUpParent, NotLeafSibling) {
  heap h1 = { NULL };

  /************
   *     1    *
   *  5     6 *
   *       7  *
   ************/

  pushNode(&h1, 1);
  pushNode(&h1, 5);
  node *d = pushNode(&h1, 6);
  pushNode(&h1, 7);

  int n = deleteNode(&h1, d);
  ASSERT_EQ(n, 6);
  /*****************
   * After delete: *
   *      1        *
   *   5     7     *
   *****************/
  ASSERT_TRUE(h1.root->key == 1);
  ASSERT_TRUE(h1.root->down->key == 5);
  ASSERT_TRUE(h1.root->down->up->key == 7);
  ASSERT_TRUE(h1.root->down->up->up->key == 1);
  ASSERT_TRUE(h1.majorPath->key == 7);
  ASSERT_TRUE(h1.majorPath->up->key == 1);

  clearHeap(&h1);
}

TEST(DecreaseKey, Test1) {
  heap h1 = { NULL };

  /**************
   *     1      *
   *  5     6   *
   *      7   8 *
   *            *
   **************/

  pushNode(&h1, 1);
  pushNode(&h1, 5);
  pushNode(&h1, 6);
  node *d = pushNode(&h1, 7);
  pushNode(&h1, 8);

  decreaseKey(&h1, d, 5);

  /******************
   * After decrease *
   *     1          *
   *  5     5       *
   *      6         *
   *   8            *
   ******************/
  ASSERT_TRUE(h1.root->key == 1);
  ASSERT_TRUE(h1.root->down->key == 5);
  ASSERT_TRUE(h1.root->down->up->key == 5);
  ASSERT_TRUE(h1.root->down->up->up->key == 1);
  ASSERT_TRUE(h1.root->down->up->down->key == 6);
  ASSERT_TRUE(h1.root->down->up->down->up->key == 5);
  ASSERT_TRUE(h1.root->down->up->down->down->key == 8);
  ASSERT_TRUE(h1.root->down->up->down->down->up->key == 6);
  ASSERT_TRUE(h1.majorPath->key == 5);
  ASSERT_TRUE(h1.majorPath->up->key == 1);

  clearHeap(&h1);
} /* End of 'DeleteNode, LeafSibling' test */

/* Main tester function.
 * ARGUMENTS:
 *   - command line params:
 *       int argc, char *argv[];
 * RETURNS:
 *   (int) 0 if success.
 */
int main( int argc, char *argv[] ) {

  testing::InitGoogleTest(&argc, argv);
  testing::UnitTest::GetInstance()->listeners().Append(new testing::MemoryLeakDetector());
  RUN_ALL_TESTS();

  return 0;
} /* End of 'main' function */

#include <gtest/gtest.h>
#include "list.h"
#include "Tools.h"

#define ASSERT_THAT_LIST_IS_EQUAL_TO(count, ...) INVOKE_WITH_VAR_ARGS(int, assertThatListIsEqualTo, count, __VA_ARGS__)

class ListTest : public testing::Test {
protected:
    List<int> list;
    virtual void SetUp()
    {
        for(int i = 0;i < 4;++i) {
            list.pushBack(i);
        }
    }

    void assertThatListIsEmpty() {
        ASSERT_TRUE(list.isEmpty());
        ASSERT_EQ(0, list.getSize());
    }


    void assertThatListIsEqualTo(int count, int* elements) {
        assertThatListIsEqualTo(elements, count);
        delete [] elements;
    }

    void assertThatListIsEqualTo(int* elements, int count) {
        assertListSizeEqualTo(count);
        assertElementsEqualityUsingGet(elements, count);
        assertFrontAndBackEquality(elements, count);
        assertIteratorElementsEquality(elements, count);
    }

    void assertListSizeEqualTo(int count) {
        ASSERT_FALSE(list.isEmpty());
        ASSERT_EQ(count, list.getSize());
    }

    void assertElementsEqualityUsingGet(int* elements, int count) {
        for(int i = 0; i < count; ++i) {
            ASSERT_EQ(elements[i], list.getElementAt(i));
        }
    }

    void assertFrontAndBackEquality(int* elements, int count) {
        ASSERT_EQ(elements[0], list.getFront());
        ASSERT_EQ(elements[count - 1], list.getBack());
    }

    void assertIteratorElementsEquality(int* elements, int count) {
        List<int> :: Iterator it = list.getIterator();

        for(int i = 0; i < count; ++i) {
            ASSERT_TRUE(it.hasNext());
            ASSERT_EQ(elements[i], it.next());
        }
        ASSERT_FALSE(it.hasNext());
    }
};

TEST_F(ListTest, shouldContainInitialElements) {
    ASSERT_THAT_LIST_IS_EQUAL_TO(4, /**/ 0, 1, 2, 3);
}

TEST_F(ListTest, shouldShortenTheListAfterPops) {
    ASSERT_EQ(0, list.popFront());
    ASSERT_THAT_LIST_IS_EQUAL_TO(3, /**/ 1, 2, 3);

    ASSERT_EQ(3, list.popBack());
    ASSERT_THAT_LIST_IS_EQUAL_TO(2, /**/ 1, 2);

    ASSERT_EQ(2, list.popBack());
    ASSERT_THAT_LIST_IS_EQUAL_TO(1, /**/ 1);

    ASSERT_EQ(1, list.popFront());
    assertThatListIsEmpty();
}

TEST_F(ListTest, shouldAddElementsWithPush) {
	list.pushFront(7);
	ASSERT_THAT_LIST_IS_EQUAL_TO(5, /**/ 7, 0, 1, 2, 3);

	list.pushBack(22);
	ASSERT_THAT_LIST_IS_EQUAL_TO(6, /**/ 7, 0, 1, 2, 3, 22);

	list.pushBack(13);
	ASSERT_THAT_LIST_IS_EQUAL_TO(7, /**/ 7, 0, 1, 2, 3, 22, 13);
}

TEST_F(ListTest, shouldAddElementsWithInsert) {
	list.insert(24, 2);
	ASSERT_THAT_LIST_IS_EQUAL_TO(5, /**/ 0, 1, 24, 2, 3);
}

TEST_F(ListTest, shouldRemoveElementAtPosition) {
	list.remove(2);
	ASSERT_THAT_LIST_IS_EQUAL_TO(3, /**/ 0, 1, 3);
}

TEST_F(ListTest, shouldMakeVariousOperationCorrectly) {
	for(int i = 0; i < 4; ++i) {
		list.popFront();
	}
	SetUp();
	list.remove(2);
	list.insert(20, 2);
	list.popBack();
	list.pushBack(30);
	list.insert(10, 1);
	list.remove(2);

	ASSERT_THAT_LIST_IS_EQUAL_TO(4, /**/ 0, 10, 20, 30);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

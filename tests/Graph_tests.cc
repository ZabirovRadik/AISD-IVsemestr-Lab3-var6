#include <gtest/gtest.h>
#include <Hash_table/Hash_table.h>
#include <time.h>
#include <string>
#include <vector>
#include <deque>
#include <map>
#include <cctype>

TEST(HashTableTests, CopyTest) {
	HashTable<size_t, size_t, std::deque> h_table(7, 1), second = h_table;
	EXPECT_TRUE(h_table == second);
	h_table.insert_or_assigned(10, 20);
	second.erase_all_occurences(10);
	EXPECT_TRUE(!h_table.find(10));
	EXPECT_FALSE(second.find(10));
	EXPECT_FALSE(h_table == second);
}


TEST(HashTableTestsr, EraseTest) {
	HashTable<size_t, size_t, std::list> h_table(10, 1);
	h_table.insert(3794, 2765);
	std::cout << h_table;
	h_table.erase(3794);
	EXPECT_FALSE(h_table.find(3794));
	h_table.resize(30);
	std::cout << h_table;
}

TEST(TaskTest, TaskTest) {
	EXPECT_EQ(roman_to_arabish("MMMDCCLXXII"), 3772);
	EXPECT_EQ(roman_to_arabish("MCCCLXXV"), 1375);
	EXPECT_EQ(roman_to_arabish("MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMCMXCIX"), 99999);
	EXPECT_EQ(roman_to_arabish("MMMMMDCCXLIII"), 5743);
	EXPECT_ANY_THROW(roman_to_arabish(""), 0);
	EXPECT_EQ(roman_to_arabish("CCV"), 205);
}

#include "my_json.h"
#include "gtest/gtest.h"
#include <iostream>
#include <fstream>

struct Test :testing::Test
{

};

TEST_F(Test, addEl)
{
	json j;
	int a = 7;
	pushEl("test.json", j, a);
	ASSERT_EQ(1, j.size());
	ASSERT_EQ(7, j.begin().value());

	j.clear();
	ClearFile("test.json");
}

TEST_F(Test, delEl)
{
	json j;
	int a = 7;
	pushEl("test.json", j, a);
	a = 28;
	pushEl("test.json", j, a);
	a = 33;
	pushEl("test.json", j, a);

	ASSERT_EQ(3, j.size());
	ASSERT_EQ(33, (j.end() -1).value());

	deleteEl("test.json", j, a);

	ASSERT_EQ(2, j.size());
	ASSERT_EQ(28, (j.end() -1).value());

	j.clear();
	ClearFile("test.json");
}

TEST_F(Test, clearFile)
{
	json j;
	int a = 7;
	pushEl("test.json", j, a);
	pushEl("test.json", j, a);
	pushEl("test.json", j, a);

	j.clear();
	ASSERT_EQ(0,j.size());
	ClearFile("test.json");
	std::ifstream i("test.json");
	i >> j;
	ASSERT_EQ(0,j.size());

	j.clear();
	i.close();
}

TEST_F(Test, maxEl)
{
	json j;
	int a = 7;
	pushEl("test.json", j, a);
	a = 3;
	pushEl("test.json", j, a);
	a = 35;
	pushEl("test.json", j, a);

	int max;
	maxEl(j, max);
	ASSERT_EQ(35, max);

	j.clear();
	ClearFile("test.json");
}

TEST_F(Test, minEl)
{
	json j;
	int a = 7;
	pushEl("test.json", j, a);
	a = 3;
	pushEl("test.json", j, a);
	a = 35;
	pushEl("test.json", j, a);

	int min;
	minEl(j, min);
	ASSERT_EQ(3, min);

	j.clear();
	ClearFile("test.json");
}

TEST_F(Test, posEl)
{
	json j;
	int a = 7;
	pushEl("test.json", j, a);
	a = 3;
	pushEl("test.json", j, a);
	a = 35;
	pushEl("test.json", j, a);

	int find = 3;
	ASSERT_EQ(1, posFind(j,find));

	j.clear();
	ClearFile("test.json");
}

int main(int argc, char **argv)
{
	testing::InitGoogleTest(&argc, argv);
	auto result = RUN_ALL_TESTS();
	std::cin.get();
	return result;
}
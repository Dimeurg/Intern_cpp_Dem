
#include "gtest/gtest.h"
#include "sparseMatrix/sparseMatrix.h"
#include <iostream>
#include <string>

struct Test :testing::Test{};


TEST_F(Test, addEl)
{
	sparseMatrix<std::string> m(20, 20);
	m.insert(2,3,"one");
	
	ASSERT_EQ("one", m.at(2,3));

}

TEST_F(Test, removeEl)
{
	sparseMatrix<std::string> m(20, 20);
	ASSERT_EQ(m.begin(), m.end());
	
	m.insert(2,3,"one");
	ASSERT_NE(m.begin(), m.end());
	
	m.remove(2,3);
	ASSERT_EQ(m.begin(), m.end());
}

TEST_F(Test, atEl)
{
	sparseMatrix<std::string> m(20, 20);	
	m.insert(2,3,"one");
	m.insert(5,13,"other");
	m.insert(15,7,"hy");
	
	ASSERT_EQ("hy", m.at(15,7));
}

TEST_F(Test, findEl)
{
	sparseMatrix<std::string> m(20, 20);	
	m.insert(2,3,"one");
	m.insert(5,13,"other");
	m.insert(15,7,"hy");
	
	std::pair<Point, std::string> par({5,13}, "other");
	auto f = std::find(m.begin(), m.end(), par);
	
	ASSERT_EQ(par, *f);
}

TEST_F(Test, distanceEl)
{
	sparseMatrix<std::string> m(20, 20);	
	Point a(2,2);
	Point b(8,2);
	Point b2(8,4);
	ASSERT_EQ(6, m.distance(a, b));
	ASSERT_EQ(46, m.distance(a, b2));
}

int main(int argc, char **argv)
{
	testing::InitGoogleTest(&argc, argv);
	auto result = RUN_ALL_TESTS();
	std::cin.get();
	return result;
}
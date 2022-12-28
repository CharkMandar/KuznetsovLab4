#include <gtest.h>

#include<iostream>
#include <map>
#include <vector>
#include <stack>
#include <set>
#include <string>
#include "Iterator.h"

using namespace std;

TEST(translator, correct_check_is_working)
{
	std::string expr1 = { "2+8-1" };
	EXPECT_EQ(1, correct_expression(expr1));

	std::string expr = { "2+*(8-1)" };
	EXPECT_EQ(0, correct_expression(expr));
}

TEST(translator, throws_if_brackets_are_incorrect)
{
	std::string expr = { "2+((8-1)" };
	std::string expr1 = { "2+(8-1))" };

	EXPECT_EQ(0, correct_bracket(expr));

	EXPECT_EQ(0, correct_bracket(expr1));
}

//TEST(translator, throws_if_number_have_multiple_points)
//{
//	std::string expr = { "2.89.12+8-1" };
//	std::string expr1 = { "2.8912+8-1" };
//	
//	analysis test(expr);
//	analysis test1(expr1);
//	
//	EXPECT_EQ(1, test1.multiple_points() );
//	EXPECT_EQ(0, test.multiple_points());
//}

TEST(translator, can_count_simple_expressions)
{
	std::string expr = { "2+8-1" };
	elements texpr, pexpr;
	divide_expression_by_elements(expr, texpr);
	make_polish_notation(texpr, pexpr);

	EXPECT_EQ(9, calculate(pexpr));
}

TEST(translator, can_count_with_negative_number)
{
	std::string expr = { "2*(-8)-1" };
	elements texpr, pexpr;
	divide_expression_by_elements(expr, texpr);
	make_polish_notation(texpr, pexpr);

	EXPECT_EQ(-17, calculate(pexpr));
}

TEST(translator, can_count_float_numbers)
{
	std::string expr = { "2.4+8.19-1.5" };
	elements texpr, pexpr;
	divide_expression_by_elements(expr, texpr);
	make_polish_notation(texpr, pexpr);

	EXPECT_EQ(9.09, calculate(pexpr));
}

TEST(translator, cant_divide_by_zero)
{
	std::string expr = { "5/0" };
	elements texpr, pexpr;
	divide_expression_by_elements(expr, texpr);
	make_polish_notation(texpr, pexpr);

	EXPECT_EQ(-1, calculate(pexpr));
}

TEST(translaor, can_count_more_difficult_expressions)
{
	std::string expr = { "2.14+((-8.9)*2-2^4)/2^3" };
	elements texpr, pexpr;
	divide_expression_by_elements(expr, texpr);
	make_polish_notation(texpr, pexpr);
	double test = -2.0849999999999995;
	EXPECT_DOUBLE_EQ(test, calculate(pexpr));
}
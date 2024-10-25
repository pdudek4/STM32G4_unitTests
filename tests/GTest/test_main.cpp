/*
 * test_main.cpp
 *
 *  Created on: Oct 24, 2024
 *      Author: pdude
 */


/* Testing/test_main.cpp */
#include "gtest/gtest.h"


int main(int ac, char* av[])
{
  testing::InitGoogleTest(&ac, av);
  return RUN_ALL_TESTS();
}

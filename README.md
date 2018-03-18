# Bakery
A program in C++ to exercise metaprogramming on the 3rd semester of the studies written with B. S.

The definition is in bakery_tresc.txt

Program creates templates for classes and counts logarithm and Pi in compile-time.

Compile:
g++ -Wall -Wextra -std=c++17 -O2 -DTEST_NUM=<desired_test> bakery.h cake.h pie.h  bakery_test.cc -o bakery_test 

or if g++ version 7 isn't set as default:
g++-7 -Wall -Wextra -std=c++17 -O2 -DTEST_NUM=<desired_test> bakery.h cake.h pie.h  bakery_test.cc -o bakery_test




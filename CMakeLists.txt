cmake_minimum_required(VERSION 3.15)
project(BPlusTree C)

set(CMAKE_C_STANDARD 99)

#add_executable(BPlusTree main.c BPlusTree.c BPlusTree.h BPlusTree_1.h BPlusTree_1.c)


add_executable(BPlusTree main.c BPlusTree_1.h BPlusTree_1.c)
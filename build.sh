#! /bin/bash
g++ -c Compiler.cpp -o Compiler.o -O2
g++ Compiler.o -o Compiler -lm

g++ -c Interpreter.cpp -o Interpreter.o -O2
g++ Interpreter.o -o Interpreter -lm
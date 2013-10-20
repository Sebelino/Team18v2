#!/bin/bash
echo "Doing local tests"
for f in inmaps/test0*.in; do
echo $f >> res
cat $f | ./sokoban >> res
done


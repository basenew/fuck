#!/bin/bash

echo "experssion by \`\`"
for f in `ls .`
do
	echo ${f}
done


echo "experssion by \$()"
for f in $(ls .)
do
	echo $f
done

string="runoob is a great site"
echo `expr index "$string" io`  # 输出 4

echo "experssion by expr"
for f in `expr ls .`
do
	echo ${f}
done


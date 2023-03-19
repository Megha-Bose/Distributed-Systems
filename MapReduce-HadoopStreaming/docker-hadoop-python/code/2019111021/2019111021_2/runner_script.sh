#!/bin/bash

jar_path=$1
input_file=$2
input_dir=$3
output_dir=$4
mapper_reducer_dir=$5

hdfs dfs -rm -r -f $output_dir

hdfs dfs -mkdir $input_dir

touch temp.txt

chmod 777 $input_file

cat $input_file | python3 inp.py > temp.txt

hdfs dfs -copyFromLocal temp.txt $input_dir/data1.txt

chmod 777 $mapper_reducer_dir/mapper.py
chmod 777 $mapper_reducer_dir/reducer.py

hadoop jar $jar_path \
-input $input_dir/data1.txt \
-output $output_dir \
-mapper "python3 $mapper_reducer_dir/mapper.py" \
-reducer "python3 $mapper_reducer_dir/reducer.py" \
-file $mapper_reducer_dir/mapper.py \
-file $mapper_reducer_dir/reducer.py

echo "\n----------OUTPUT----------\n"

hdfs dfs -cat $output_dir/part*

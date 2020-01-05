from pyspark import SparkContext
import re

sc = SparkContext(appName="wordcount")
textFile= sc.textFile('twilight.txt')

result = textFile.flatMap(lambda line: re.split('[;./+,?\" ]',line)) \
    .map(lambda word: (word, 1)) \
    .reduceByKey(lambda a,b:a+b) \
    .sortBy(lambda x: x[1], False)

for word, count in result.take(10):
    print(word, count)


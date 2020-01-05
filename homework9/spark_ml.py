import findspark 
findspark.init()
from pyspark.sql import SparkSession
from pyspark import SparkConf, SparkContext
from pyspark.ml.feature import StringIndexer, VectorAssembler

spark = SparkSession.builder.master('local[1]').appName('classification').getOrCreate()
#load data
df0 = spark.read.csv('mushrooms.csv', header=True, inferSchema=True, encoding='utf-8')
old_columns_names = df0.columns
new_columns_names = [name+'-new' for name in old_columns_names]
#stringindexer transfer feature value to num
for i in range(len(old_columns_names)):
    indexer = StringIndexer(inputCol=old_columns_names[i], outputCol=new_columns_names[i])
    df0 = indexer.fit(df0).transform(df0)
#feature to vector
vecAss = VectorAssembler(inputCols=new_columns_names[1:], outputCol='features')
df0 = vecAss.transform(df0)
#change label name
df0 = df0.withColumnRenamed(new_columns_names[0], 'label')
#df0.show()
#init new data,only label and feature
dfi = df0.select(['label', 'features'])
#print 5 row
dfi.show(5, truncate=0)
#split data into train and test
train_data, test_data = dfi.randomSplit([4.0, 1.0], 100)

from pyspark.ml.classification import LogisticRegression
blor = LogisticRegression(regParam=0.01)
blorModel = blor.fit(train_data)
result = blorModel.transform(test_data)
print("LogisticRegression:"+str(float(result.filter(result.label == result.prediction).count())/result.count()))

from pyspark.ml.classification import GBTClassifier
gbt = GBTClassifier(maxDepth=5)
gbtModel = gbt.fit(train_data)
result = gbtModel.transform(test_data)
print("GBTClassifier:"+str(float(result.filter(result.label == result.prediction).count())/result.count()))

from pyspark.ml.classification import DecisionTreeClassifier
dt = DecisionTreeClassifier(maxDepth=5)
dtModel = dt.fit(train_data)
result = dtModel.transform(test_data)
print("DecisionTreeClassifier:"+str(float(result.filter(result.label == result.prediction).count())/result.count()))

from pyspark.ml.classification import LinearSVC
svm = LinearSVC(maxIter=10, regParam=0.01)
svmModel = svm.fit(train_data)
result = svmModel.transform(test_data)
print("LinearSVC:"+str(float(result.filter(result.label == result.prediction).count())/result.count()))
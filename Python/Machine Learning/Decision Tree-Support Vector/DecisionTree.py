import pandas as pd
import matplotlib.pyplot as plt
from sklearn.model_selection import train_test_split
from sklearn.tree import DecisionTreeClassifier
from sklearn.preprocessing import StandardScaler
from sklearn.metrics import confusion_matrix, accuracy_score
from matplotlib.colors import ListedColormap
import numpy as np
file=open("data.csv")

datas=pd.read_csv(file,header=None)
coord=datas.iloc[:, :-1].values
Pattern=datas.iloc[:,-1].values

file.close()

X_train, X_test, p_train, p_test = train_test_split(coord,Pattern, test_size = 0.2, random_state = 0)
sc = StandardScaler()
X_train = sc.fit_transform(X_train)
X_test = sc.transform(X_test)
X_all=sc.fit_transform(coord)
# Default
#classifier = DecisionTreeClassifier()
classifier=DecisionTreeClassifier(criterion='entropy',random_state=109,min_samples_leaf=2)
#criterion='gini',random_state=109,min_samples_leaf=2

classifier.fit(X_train,p_train)
p_pred= classifier.predict(X_test) # test
pred_train=classifier.predict(X_train)
p_all=classifier.predict(X_test)
cm = confusion_matrix(p_test, p_pred)
acc=accuracy_score(p_test, p_pred)
print("Accuracy={}".format(acc))

X_set, y_set = sc.inverse_transform(X_test), p_pred

X1, X2 = np.meshgrid(np.arange(start = X_set[:, 0].min() - 1, stop = X_set[:, 0].max() + 1, step = 0.1),
                     np.arange(start = X_set[:, 1].min() - 1, stop = X_set[:, 1].max() + 1, step = 0.1))
plt.contourf(X1, X2, classifier.predict(sc.transform(np.array([X1.ravel(), X2.ravel()]).T)).reshape(X1.shape),
             alpha = 0.8, cmap = ListedColormap(('salmon', 'dodgerblue')))
plt.xlim(X1.min(), X1.max())
plt.ylim(X2.min(), X2.max())
for i, j in enumerate(np.unique(y_set)):
    plt.scatter(X_set[y_set == j, 0], X_set[y_set == j, 1], c = ListedColormap(('salmon', 'dodgerblue'))(i), label = j)
plt.title('Decision Tree Classification (Test set)')
plt.xlabel('X coord')
plt.ylabel('Y')
plt.legend()
plt.show()
print(cm)
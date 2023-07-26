import matplotlib.pyplot as plt
from sklearn.linear_model import LinearRegression, SGDRegressor
from sklearn.metrics import mean_squared_error
from sklearn.model_selection import train_test_split
from sklearn import datasets
import numpy as np
import pandas as pd
import openpyxl

#(500,500) -> (250,250)
diabetes_X, diabetes_y = datasets.load_diabetes(return_X_y=True)

X = 0.4 * np.linspace(-5, 5, 500).reshape(500, 1)
y = 6 + 4 * X + np.random.randn(500, 1)
train_x, test_x, train_y, test_y = train_test_split(X, y, test_size=1 / 2, random_state=0)

Lin_Reg = LinearRegression()
Lin_Reg.fit(train_x, train_y)
prediction = Lin_Reg.predict(test_x)

plt.scatter(test_x, test_y, color='red')
plt.plot(test_x, prediction, color='blue', linewidth=3, label="Linear Regression")

# error

mse_lin_train = mean_squared_error(train_y, prediction)
mse_lin_test = mean_squared_error(test_y, Lin_Reg.predict(test_x))
df_lin = pd.DataFrame(prediction, columns=["Least squares Method"])
df_x_test = pd.DataFrame(test_x, columns=['X values'])
print('linear coeff={}\nLinear Constant= {}'.format(Lin_Reg.coef_, Lin_Reg.intercept_))

plt.title('Linear Regression')
plt.xlabel('X values')
plt.ylabel('Y values')
plt.legend(loc='upper left')
plt.show()
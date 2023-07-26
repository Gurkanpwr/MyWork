import matplotlib.pyplot as plt
from sklearn.linear_model import LinearRegression, SGDRegressor
from sklearn.metrics import mean_squared_error
from sklearn.model_selection import train_test_split
import numpy as np
import pandas as pd
import openpyxl

#(500,500) -> (250,250)
X = 0.4 * np.linspace(-3, 3, 500).reshape(500, 1)
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
# SGD Method
SGD_Reg = SGDRegressor(alpha=0.005,learning_rate='constant')
SGD_Reg.fit(train_x, train_y.ravel())
prediction = SGD_Reg.predict(test_x)


plt.plot(test_x, prediction, color='green', linewidth=3, label='SGD Regression')
plt.title('Linear Regression')
plt.xlabel('X values')
plt.ylabel('Y values')
plt.legend(loc='upper left')
plt.show()

# Error
mse_sgd_train = mean_squared_error(train_y, prediction)
mse_sgd_test = mean_squared_error(test_y, SGD_Reg.predict(test_x))
df_sgd = pd.DataFrame(prediction, columns=["SGD Regression"])
actual = pd.DataFrame(train_y, columns=["Actual"])
print('SGD ERROR:{}\nSimple Reg Error:{}'.format(mse_sgd_test, mse_lin_test))
print('SGD coeff={}\nSGD Constant= {}'.format(SGD_Reg.coef_, SGD_Reg.intercept_))
df = df_lin.add(df_sgd, fill_value=0)
df = df.add(actual, fill_value=0)
df = df.add(df_x_test, fill_value=0)
df.to_excel('Datas_Task1.xlsx')

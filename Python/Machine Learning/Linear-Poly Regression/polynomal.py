import matplotlib.pyplot as plt
import pandas as pd
from sklearn.linear_model import LinearRegression
from sklearn.preprocessing import PolynomialFeatures
import numpy as np
from sklearn.metrics import mean_squared_error


#poly regression
X=0.1*np.linspace(-10,10,500).reshape(500,1)
y=3*X**3+0.5*X**2+X+2+np.random.randn(500,1)

def pol_reg(X,y,dgr=4):
    #Polinom Regression
    polreg=PolynomialFeatures(degree=dgr)
    X_pol=polreg.fit_transform(X)
    linreg_pol=LinearRegression()
    linreg_pol.fit(X_pol,y)
    prediction=linreg_pol.predict(X_pol)
    fig, graph = plt.subplots()
    graph.scatter(X,y,color='red')
    plt.plot(X,prediction,color='blue',linewidth=4)
    graph.set_xlabel("X values")
    graph.set_ylabel("y values")
    fig.suptitle("Polinomal Regression Method degree={}".format(dgr))
    plt.show()
    Error=mean_squared_error(y,y_pred=prediction)
    print('Error {} at degree{}'.format(Error,dgr))
    df=pd.DataFrame(prediction,columns=['Pol Reg'])
    df_x=pd.DataFrame(X,columns=['X values'])
    df=df.add(df_x,fill_value=0)
    df.to_excel('Datas_polReg_Task1.xlsx')
    print('Coeefficients:\nConstant={}\nparams={}'.format(linreg_pol.intercept_,linreg_pol.coef_))
    return Error
# best degree calcucaliton
#pol_reg(X,y)
errors=[]
degrees=np.linspace(1,10,10)
for dgre in degrees:
    dgre=int(dgre)
    errors.append(pol_reg(X,y,dgr=dgre))
print('min error {} at '
      'degree of {}'.format(np.min(errors),
                                            errors.index(np.min(errors))+1))

fig,graph=plt.subplots()
plt.plot(degrees,errors)
graph.set_xlabel("Degree")
graph.set_ylabel("Error")
fig.suptitle('Error vs Degree')
plt.show()
pol_reg(X,y,dgr=4)
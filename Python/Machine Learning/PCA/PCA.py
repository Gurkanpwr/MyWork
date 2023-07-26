import matplotlib.pyplot as plt
import numpy as np

X=np.array([[2.5, 0.5, 2.2, 1.9, 3.1, 2.3, 2, 1, 1.5, 1.1],
            [2.4, 0.7, 2.9, 2.2, 3, 2.7, 1.6, 1.1, 1.6, 0.9]]).transpose()
# Shape of Array is called Shape
Shape = X.shape

#Data's column and row numbers
print("Number of rows = {}\n number of columns={}".format(Shape[0], Shape[1]))

# Centered X data
X_centered = X-X.mean(axis=0)

# STEP 4 Datas
#Graph Of X data
fig,ax =plt.subplots(figsize=(10,10))
ax.grid(which='both', color='grey', linewidth=1, linestyle='-', alpha=0.2)
ax.scatter(X[:,0],X[:,1],c='blue',label="Data Points",s=250,marker='.')
ax.scatter(*(X.mean(axis=0)),c='red',label='Center')
plt.axvline(x=0)
ax.legend()
plt.axhline(y=0)
ax.set_title('Data and its center')
plt.show()
plt.close(fig)
##### Step 5 Centered Data
## Graph of CENTERED DATA
fig2,ax =plt.subplots(figsize=(10,10))
ax.set_title('Centered Data')
ax.scatter(X_centered[:,0],
           X_centered[:,1],
           c="blue",
           s=250,
           marker = '.')
ax.set_xlabel('X axis')
ax.set_ylabel('Y axis')
ax.axhline(y=0)
ax.axvline()
ax.grid(which='both', color='grey', linewidth=1, linestyle='-', alpha=0.2)
plt.tight_layout()
plt.show()
plt.close(fig2)

# Covariance Matrix of centered Data
cov=np.cov(X_centered.T)
print(cov)

#Eigen decomposition
eig_vals,eig_vect =np.linalg.eig(cov)
D=eig_vals #Assignin D to eigen values
V= eig_vect # Assign V to eigen vectors

# Step 9 Graph of eigen vectors
fig3,ax3 = plt.subplots(figsize=(10,10))
ax3.set_title('Eigen Vectors')
ax3.scatter(X_centered[:,0],
           X_centered[:,1],
           c="blue",
           s=250,
           marker = '.')
ax3.quiver(V[0,0],V[1,0],color="red",scale =5)
ax3.quiver(V[0,1],V[1,1],color="purple",scale =5)
ax3.set_xlabel('X axis')
ax3.set_ylabel('Y axis')
ax3.axhline(y=0)
ax3.axvline()
ax3.grid(which='both', color='grey', linewidth=1, linestyle='-', alpha=0.2)
plt.tight_layout()
plt.show()
plt.close(fig3)

#Step 10 Assign Eigens
u=V[:,eig_vals.argmax()]
z=V[:,eig_vals.argmin()]
#Expand Dims
u=np.expand_dims(u,1) # Highest eigen value
z=np.expand_dims(z,1) # lowest eigen value

#projections
pc1= (X_centered @ u) @ u.T
pc2= X_centered @ z @ z.T

# Step 12 plot the projection
fig4,ax4 = plt.subplots()
ax4.set_title('Projected Eigens')

ax4.scatter(X_centered[:,0],
           X_centered[:,1],
           c="blue",
           s=50,
           marker = '.',label='Centered Data')

ax4.set_xlabel('X axis')
ax4.set_ylabel('Y axis')
ax4.axhline()
ax4.axvline()
ax4.grid(which='both', color='grey', linewidth=1, linestyle='-', alpha=0.2)

plt.quiver(eig_vect[0,eig_vals.argmax()],
           eig_vect[1,eig_vals.argmax()],color='red',scale=5,label='Eigen U')

plt.quiver(eig_vect[0,eig_vals.argmin()],
           eig_vect[1,eig_vals.argmin()],color='purple',scale=5,label='Eigen Z')

ax4.scatter(pc1[:,0],pc1[:,1],c="orange",s=200,label="projection on u",marker='.')
ax4.legend()
plt.show()
plt.close(fig4)

#Step 12 other projection
fig5,ax5 = plt.subplots()
ax5.set_title('Projected Eigens 2')

ax5.scatter(X_centered[:,0],
           X_centered[:,1],
           c="blue",
           s=50,
           marker = '.',label='Centered Data')

ax5.set_xlabel('X axis')
ax5.set_ylabel('Y axis')
ax5.axhline()
ax5.axvline()
ax5.grid(which='both', color='grey', linewidth=1, linestyle='-', alpha=0.2)

plt.quiver(eig_vect[0,eig_vals.argmax()],
           eig_vect[1,eig_vals.argmax()],color='red',scale=5,
           label="Eigen U")

plt.quiver(eig_vect[0,eig_vals.argmin()],
           eig_vect[1,eig_vals.argmin()],color='purple',scale=5,label='Eigen Z')

ax5.scatter(pc2[:,0],pc2[:,1],c="brown",s=200,label="projection on z",marker='.')
ax5.legend()
plt.show()
plt.close(fig5)
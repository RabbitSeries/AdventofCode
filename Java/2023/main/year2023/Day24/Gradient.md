# Gradient
Unknown variable: $V=(V_x,V_y,V_z)^T$, $R=(R_x,R_y,R_z)^T$, $T=(t_1,t_2,t_3)$

Known parameters: 
$$
    V_{3\times 3} 
    =  \left(V_1, V_2, V_3\right) 
    = \left(
            \begin{matrix}
                V_{1x} & V_{2x} & V_{3x}\\
                V_{1y} & V_{2y} & V_{3y}\\
                V_{1z} & V_{2z} & V_{3z}
            \end{matrix}
        \right)
$$
$$
R_{3\times 3} = \left(R_1, R_2, R_3\right) =
\left(
    \begin{matrix}
        R_{1x} & R_{2x} & R_{3x}\\
        R_{1y} & R_{2y} & R_{3y}\\
        R_{1z} & R_{2z} & R_{3z}
    \end{matrix}
\right)
$$
Equations:
$$
\left\{R_i+V_i\cdot t_i = R+V\cdot t_i \mid i=1,2,3 \right\}$$
For positive T solution, let:
$$t_i = e^{u_i} \quad (i=1,2,3)$$
$$T=U=(e^{u_1},e^{u_2},e^{u_3})^T$$
Residual:
$$
    L(R_j,V_j,t_i)=R_{ij}-R_j+(V_{ij}-V_j)\cdot t_i \quad (i=1,2,3\quad j =x,y,z)\\
$$
MSE:
$$
\begin{aligned}
    MSE(R,V,t_i)
    &=\sum_{j}^{x,y,z} L^2(R_j,V_j,t_i)
    &=\sum_{j}^{x,y,z} \left[R_{ij}-R_j+(V_{ij}-V_j)\cdot t_i\right]^2\quad (i=1,2,3)
\end{aligned}$$
Total MSE:
$$L(R,V,T)=\sum_{i=1}^{3}MSE(R,V,t_i)$$
Gradients:
$$
\begin{aligned}
    \frac{\partial{L}}{R_j}&=\sum_{i=1}^{3}\frac{\partial{MSE(R,V,t_i)}}{\partial{R_j}}\\
    &=\sum_{i=1}^{3}\frac{\partial{\left\{\left[R_{ij}-R_j+(V_{ij}-V_j)\cdot t_i\right]^2\right\}}}{\partial{R_j}}\\
    &=\sum_{i=1}^{3}-2(R_{ij}-R_j+(V_{ij}-V_j)\cdot t_i)\\
\end{aligned}
\qquad (j=x,y,z)
$$
$$
\begin{aligned}
    \frac{\partial{L}}{\partial{V_j}}&=\sum_{i=1}^{3}\frac{\partial{MSE(R,V,t_i)}}{\partial{V_j}}\\
    &=\sum_{i=1}^{3}\frac{\partial{\left\{\left[R_{ij}-R_j+(V_{ij}-V_j)\cdot t_i\right]^2\right\}}}{\partial{V_j}}\\
    &=\sum_{i=1}^{3}-2t_i(R_{ij}-R_j+(V_{ij}-V_j)\cdot t_i)
\end{aligned}
\qquad (j=x,y,z)
$$
$$
\begin{aligned}
    \frac{\partial{L}}{\partial{u_i}}&=\frac{\partial{L}}{\partial{t_i}}\cdot\frac{\partial{t_i}}{\partial{u_i}}\\
    &=\frac{\partial{MSE(R,V,t_i)}}{\partial{t_i}}\cdot e^{u_i}\\
    &=e^{u_i}\sum_{j}^{x,y,z}\frac{\partial{\left\{\left[R_{ij}-R_j+(V_{ij}-V_j)\cdot t_i\right]^2\right\}}}{\partial{t_i}}\\
    &=e^{u_i}\sum_{j}^{x,y,z}2(V_{ij}-V_j)\left[R_{ij}-R_j+(V_{ij}-V_j)\cdot t_i\right]\\
    &=t_i\sum_{j}^{x,y,z}2(V_{ij}-V_j)\left[R_{ij}-R_j+(V_{ij}-V_j)\cdot t_i\right]
\end{aligned}
\qquad (i=1,2,3)
$$
Single target Jacobian:
$$
  \begin{aligned}
   Jacobian&=\frac{\partial{L(R,V,T(U))}}{\partial{(R,V,U)}}\\
   &=
   \left(\begin{matrix}
        \frac{\partial{L(R,V,T)}}{\partial{R_x}}&
        \frac{\partial{L(R,V,T)}}{\partial{R_y}}&
        \frac{\partial{L(R,V,T)}}{\partial{R_z}}&
        \frac{\partial{L(R,V,T)}}{\partial{V_x}}&
        \frac{\partial{L(R,V,T)}}{\partial{V_y}}&
        \frac{\partial{L(R,V,T)}}{\partial{V_z}}&
        \frac{\partial{L(R,V,T)}}{\partial{u_1}}&
        \frac{\partial{L(R,V,T)}}{\partial{u_2}}&
        \frac{\partial{L(R,V,T)}}{\partial{u_3}}
   \end{matrix}\right)
  \end{aligned}
$$

Split MSE target:
$$
    \begin{aligned}
        Residual(R,V,T(U))&=\left(
            \begin{matrix}
                L(R_x,V_x,t_1)&
                L(R_x,V_x,t_1)&
                L(R_x,V_x,t_1)&
                L(R_y,V_y,t_2)&
                L(R_y,V_y,t_2)&
                L(R_y,V_y,t_2)&
                L(R_z,V_z,t_3)&
                L(R_z,V_z,t_3)&
                L(R_z,V_z,t_3)
            \end{matrix}
        \right)\\
    \end{aligned}
$$
Gradient:
$$
    \begin{aligned}
        \frac{\partial{L(R_j,V_j,t_i)}}{\partial{R_k}} &=
            \begin{cases}
                -1 & \text{if } k==j\\
                0 & else
            \end{cases}\\
        \frac{\partial{L(R_j,V_j,t_i)}}{\partial{V_k}} &= 
            \begin{cases}
                -t_i & \text{if } k==j\\
                0 & else
            \end{cases}\\
        \frac{\partial{L(R_j,V_j,t_i)}}{\partial{u_k}} &=
            \begin{cases}
                (V_{ij} - V_j)\frac{dt_i}{du_i}=(V_{ij} - V_j)t_i & \text{if } i==k\\
                0 & else
            \end{cases}
    \end{aligned}
$$
Gradient Jacobian:
$$
    \begin{aligned}
        J&=\frac{\partial{Residual(R,V,T(U))}}{\partial{(R,V,U)}}\\
        &=\left(
            \begin{matrix}
                \frac{\partial{L(R_x,V_x,t_1)}}{\partial{R_x}}&
                \frac{\partial{L(R_x,V_x,t_1)}}{\partial{R_y}}&
                \frac{\partial{L(R_x,V_x,t_1)}}{\partial{R_z}}&
                \frac{\partial{L(R_x,V_x,t_1)}}{\partial{V_x}}&
                \frac{\partial{L(R_x,V_x,t_1)}}{\partial{V_y}}&
                \frac{\partial{L(R_x,V_x,t_1)}}{\partial{V_z}}&
                \frac{\partial{L(R_x,V_x,t_1)}}{\partial{u_1}}&
                \frac{\partial{L(R_x,V_x,t_1)}}{\partial{u_2}}&
                \frac{\partial{L(R_x,V_x,t_1)}}{\partial{u_3}}\\
                &&&&...\\
                \frac{\partial{L(R_z,V_z,t_1)}}{\partial{R_x}}&
                \frac{\partial{L(R_z,V_z,t_1)}}{\partial{R_y}}&
                \frac{\partial{L(R_z,V_z,t_1)}}{\partial{R_z}}&
                \frac{\partial{L(R_z,V_z,t_1)}}{\partial{V_x}}&
                \frac{\partial{L(R_z,V_z,t_1)}}{\partial{V_y}}&
                \frac{\partial{L(R_z,V_z,t_1)}}{\partial{V_z}}&
                \frac{\partial{L(R_z,V_z,t_1)}}{\partial{u_1}}&
                \frac{\partial{L(R_z,V_z,t_1)}}{\partial{u_2}}&
                \frac{\partial{L(R_z,V_z,t_1)}}{\partial{u_3}}\\
                &&&&...\\
                &&&&...\\
                \frac{\partial{L(R_x,V_x,t_3)}}{\partial{R_x}}&
                \frac{\partial{L(R_x,V_x,t_3)}}{\partial{R_y}}&
                \frac{\partial{L(R_x,V_x,t_3)}}{\partial{R_z}}&
                \frac{\partial{L(R_x,V_x,t_3)}}{\partial{V_x}}&
                \frac{\partial{L(R_x,V_x,t_3)}}{\partial{V_y}}&
                \frac{\partial{L(R_x,V_x,t_3)}}{\partial{V_z}}&
                \frac{\partial{L(R_x,V_x,t_3)}}{\partial{u_1}}&
                \frac{\partial{L(R_x,V_x,t_3)}}{\partial{u_2}}&
                \frac{\partial{L(R_x,V_x,t_3)}}{\partial{u_3}}\\
                &&&&...\\
                \frac{\partial{L(R_z,V_z,t_3)}}{\partial{R_x}}&
                \frac{\partial{L(R_z,V_z,t_3)}}{\partial{R_y}}&
                \frac{\partial{L(R_z,V_z,t_3)}}{\partial{R_z}}&
                \frac{\partial{L(R_z,V_z,t_3)}}{\partial{V_x}}&
                \frac{\partial{L(R_z,V_z,t_3)}}{\partial{V_y}}&
                \frac{\partial{L(R_z,V_z,t_3)}}{\partial{V_z}}&
                \frac{\partial{L(R_z,V_z,t_3)}}{\partial{u_1}}&
                \frac{\partial{L(R_z,V_z,t_3)}}{\partial{u_2}}&
                \frac{\partial{L(R_z,V_z,t_3)}}{\partial{u_3}}\\
            \end{matrix}
        \right)\\\\
        &=\left(
            \begin{matrix}
                \frac{\partial{L(R_x,V_x,t_1)}}{\partial{R_x}}&
                0&
                0&
                \frac{\partial{L(R_x,V_x,t_1)}}{\partial{V_x}}&
                0&
                0&
                \frac{\partial{L(R_x,V_x,t_1)}}{\partial{u_1}}&
                0&
                0\\
                    0&
                    \frac{\partial{L(R_y,V_y,t_1)}}{\partial{R_y}}&
                    0&
                    0&
                    \frac{\partial{L(R_y,V_y,t_1)}}{\partial{V_y}}&
                    0&
                    \frac{\partial{L(R_y,V_y,t_1)}}{\partial{u_1}}&
                    0&
                    0\\
                0&
                0&
                \frac{\partial{L(R_z,V_z,t_1)}}{\partial{R_z}}&
                0&
                0&
                \frac{\partial{L(R_z,V_z,t_1)}}{\partial{V_z}}&
                \frac{\partial{L(R_z,V_z,t_1)}}{\partial{u_1}}&
                0&
                0\\
                % 
                \frac{\partial{L(R_x,V_x,t_2)}}{\partial{R_x}}&
                0&
                0&
                \frac{\partial{L(R_x,V_x,t_2)}}{\partial{V_x}}&
                0&
                0&
                0&
                \frac{\partial{L(R_x,V_x,t_2)}}{\partial{u_2}}&
                0\\
                    0&
                    \frac{\partial{L(R_y,V_y,t_2)}}{\partial{R_y}}&
                    0&
                    0&
                    \frac{\partial{L(R_y,V_y,t_2)}}{\partial{V_y}}&
                    0&
                    0&
                    \frac{\partial{L(R_y,V_y,t_2)}}{\partial{u_2}}&
                    0\\
                0&
                0&
                \frac{\partial{L(R_z,V_z,t_2)}}{\partial{R_z}}&
                0&
                0&
                \frac{\partial{L(R_z,V_z,t_2)}}{\partial{V_z}}&
                0&
                \frac{\partial{L(R_z,V_z,t_2)}}{\partial{u_2}}&
                0\\
                % 
                \frac{\partial{L(R_x,V_x,t_3)}}{\partial{R_x}}&
                0&
                0&
                \frac{\partial{L(R_x,V_x,t_3)}}{\partial{V_x}}&
                0&
                0&
                0&
                0&
                \frac{\partial{L(R_x,V_x,t_3)}}{\partial{u_3}}&\\
                    0&
                    \frac{\partial{L(R_y,V_y,t_3)}}{\partial{R_y}}&
                    0&
                    0&
                    \frac{\partial{L(R_y,V_y,t_3)}}{\partial{V_y}}&
                    0&
                    0&
                    0&
                    \frac{\partial{L(R_y,V_y,t_3)}}{\partial{u_3}}&\\
                0&
                0&
                \frac{\partial{L(R_z,V_z,t_3)}}{\partial{R_z}}&
                0&
                0&
                \frac{\partial{L(R_z,V_z,t_3)}}{\partial{V_z}}&
                0&
                0&
                \frac{\partial{L(R_z,V_z,t_3)}}{\partial{u_3}}&\\
            \end{matrix}
        \right)
    \end{aligned}
$$

Gradient for each target:
$$
\begin{aligned}
MSE &= \frac{1}{2} Residual^2(R,V,T(U))\\
\frac{\partial{MSE}}{\partial{w}} &= Residual\frac{\partial{Residual}}{\partial{w}}\\
\end{aligned}
$$

$$
\begin{aligned}
    \begin{rcases}
        \begin{aligned}
            Grad &= J^TResidual(R,V,T(U))\\
            X &= (R,V,T)\\
            X' &= X-Grad\cdot K_{LearningRate}\\
        \end{aligned}
    \end{rcases}
    \rArr
    X' &= X - K_{LearningRate}\cdot J^TResidual(R,V,T(U))
\end{aligned}
$$

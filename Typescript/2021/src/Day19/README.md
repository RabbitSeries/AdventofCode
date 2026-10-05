# Proof: Inverse of an Orthogonal Matrix and the Cross Product Relation

## Definitions

Let $\alpha_1, \alpha_2, \alpha_3$ be three vectors in $\mathbb{R}^3$ forming an orthonormal basis:

$$
\alpha_i \cdot \alpha_j = \delta_{ij}, \quad \|\alpha_i\| = 1
$$

Define the matrix:

$$
A = [\alpha_1\ \alpha_2\ \alpha_3]
$$

with the vectors as columns.

---

## 1. Left-Handed System Satisfies the Cross Product Relation

If $(\alpha_1, \alpha_2, \alpha_3)$ forms a right-handed system, then:

$$
\alpha_3 = \alpha_1 \times \alpha_2
$$

In this case:

$$
\det A = \alpha_3 \cdot (\alpha_1 \times \alpha_2)
= (\alpha_1 \times \alpha_2) \cdot (\alpha_1 \times \alpha_2)
= \|\alpha_1 \times \alpha_2\|^2 = 1
$$

If $(\alpha_1, \alpha_2, \alpha_3)$ forms a left-handed system, then:

$$
\alpha_3 = -\alpha_1 \times \alpha_2
$$

In this case:

$$
\det A = \alpha_3 \cdot (\alpha_1 \times \alpha_2)
= -(\alpha_1 \times \alpha_2) \cdot (\alpha_1 \times \alpha_2)
= -\|\alpha_1 \times \alpha_2\|^2 = -1
$$

Therefore, whether the system is right-handed or left-handed, the three orthonormal vectors satisfy a cross product relation (possibly up to a sign), and the sign is determined by the sign of the determinant.

---

## 2. The Inverse of an Orthogonal Matrix Is Its Transpose

### 2.1 Definition of an orthogonal matrix

A square matrix $A$ is **orthogonal** if its columns form an orthonormal set:

$$
A = [\mathbf{v}_1\ \mathbf{v}_2\ \cdots\ \mathbf{v}_n],
\quad \mathbf{v}_i \cdot \mathbf{v}_j = \delta_{ij}
$$

Equivalently, $A^T A = I$.

### 2.2 From orthonormal columns to $A^T A = I$

The $(i,j)$-th entry of $A^T A$ is:

$$
(A^T A)_{ij} = \mathbf{v}_i^T \mathbf{v}_j = \mathbf{v}_i \cdot \mathbf{v}_j = \delta_{ij}
$$

Hence:

$$
A^T A = I
$$

### 2.3 Necessity of $A A^T = I$

Since $A^T A = I$, taking determinants:

$$
\det(A^T A) = \det(I) = 1
$$

$$
(\det A)^2 = 1
\quad \Longrightarrow \quad
\det A = \pm 1 \neq 0
$$

Thus $A$ is invertible. From $A^T A = I$, multiplying on the right by $A^{-1}$:

$$
A^T = A^{-1}
$$

Then:

$$
A A^T = A A^{-1} = I
$$

So $A^T A = I$ implies $A A^T = I$.

### 2.4 Converse: if $A A^T = I$, then $A$ is orthogonal

We now prove the converse direction.

**Claim.** If $A A^T = I$, then $A$ is orthogonal.

**Proof.** Suppose $A A^T = I$. Taking determinants:

$$
\det(A A^T) = \det(I) = 1
$$

$$
\det(A)\det(A^T) = 1
$$

$$
(\det A)^2 = 1
\quad \Longrightarrow \quad
\det A = \pm 1 \neq 0
$$

Thus $A$ is invertible. Multiplying $A A^T = I$ on the left by $A^{-1}$:

$$
A^{-1} A A^T = A^{-1} I
$$

$$
A^T = A^{-1}
$$

Now multiply $A^T = A^{-1}$ on the left by $A$:

$$
A A^T = A A^{-1} = I
$$

This is consistent, but we need to show that the **columns** of $A$ are orthonormal. From $A A^T = I$, we have $A^T = A^{-1}$, hence also:

$$
A^T A = A^{-1} A = I
$$

Now the $(i,j)$-th entry of $A^T A$ is:

$$
(A^T A)_{ij} = \mathbf{v}_i \cdot \mathbf{v}_j = \delta_{ij}
$$

where $\mathbf{v}_i$ are the columns of $A$. Therefore the columns of $A$ are orthonormal, which means $A$ is orthogonal.

$$
\boxed{A A^T = I \quad \Longrightarrow \quad A \text{ is orthogonal}}
$$

### 2.5 Equivalence

Combining both directions:

$$
\boxed{
A \text{ is orthogonal}
\quad \Longleftrightarrow \quad
A^T A = I
\quad \Longleftrightarrow \quad
A A^T = I
\quad \Longleftrightarrow \quad
A^T = A^{-1}
}
$$

All four conditions are equivalent for a square matrix $A$.

---

## 3. The Columns of the Transpose Also Satisfy the Same Relation

Let:

$$
A^{-1} = A^T =
\begin{bmatrix}
\alpha_1^T \\
\alpha_2^T \\
\alpha_3^T
\end{bmatrix}
$$

Denote the columns of $A^{-1}$ by $\beta_1, \beta_2, \beta_3$, so that:

$$
A^{-1} = [\beta_1\ \beta_2\ \beta_3]
$$

where:

$$
\beta_j =
\begin{bmatrix}
(\alpha_1)_j \\
(\alpha_2)_j \\
(\alpha_3)_j
\end{bmatrix}
$$

Since $A$ is orthogonal, we have both $A^T A = I$ and $A A^T = I$. Therefore:

$$
(A^{-1})^T A^{-1} = (A^T)^T A^T = A A^T = I
$$

Thus $A^{-1}$ is also orthogonal, and its columns $\beta_1, \beta_2, \beta_3$ also form an orthonormal basis.

Moreover:

$$
\det(A^{-1}) = \frac{1}{\det A} = \det A
$$

Therefore:

- If $\det A = +1$ (right-handed), then $\det(A^{-1}) = +1$, and $\beta_1, \beta_2, \beta_3$ still form a right-handed system:

$$
\beta_3 = \beta_1 \times \beta_2
$$

- If $\det A = -1$ (left-handed), then $\det(A^{-1}) = -1$, and $\beta_1, \beta_2, \beta_3$ still form a left-handed system:

$$
\beta_3 = -\beta_1 \times \beta_2
$$

Therefore, the columns of the transpose (i.e., the inverse) preserve the same handedness as the original matrix and still satisfy the corresponding cross product relation.

---

## Summary

| Property                                            | Conclusion                                                                                                              |
| --------------------------------------------------- | ----------------------------------------------------------------------------------------------------------------------- |
| Right-handed vector relation                        | $\alpha_3 = \alpha_1 \times \alpha_2$, $\det A = +1$                                                                    |
| Left-handed vector relation                         | $\alpha_3 = -\alpha_1 \times \alpha_2$, $\det A = -1$                                                                   |
| Orthogonal matrix: definition                       | $A^T A = I$                                                                                                             |
| Orthogonal matrix: right inverse                    | $A A^T = I$                                                                                                             |
| Converse                                            | $A A^T = I \Longrightarrow A$ is orthogonal                                                                             |
| Equivalence                                         | $A$ orthogonal $\Longleftrightarrow$ $A^T A = I$ $\Longleftrightarrow$ $A A^T = I$ $\Longleftrightarrow$ $A^T = A^{-1}$ |
| Cross product relation for columns of the transpose | Preserves handedness: right-handed $\beta_3 = \beta_1 \times \beta_2$, left-handed $\beta_3 = -\beta_1 \times \beta_2$  |

---

# The Set of Matrices Whose Columns Satisfy the Cross Product Relation

## 1. The Relation

Let $A = [\alpha_1\ \alpha_2\ \alpha_3]$ be a real $3 \times 3$ matrix.

We say $A$ satisfies the **cross product relation** if its columns form an orthonormal basis and

$$
\alpha_3 = \alpha_1 \times \alpha_2
$$

Equivalently:

$$
\alpha_i \cdot \alpha_j = \delta_{ij}, \quad
\alpha_1 \times \alpha_2 = \alpha_3
$$

Note that this is a **right-handed** orthonormal basis.

---

## 2. $A$ Is Orthogonal and $\det A = 1$

From the orthonormality of the columns:

$$
A^T A = I
$$

so $A$ is orthogonal, and therefore:

$$
A^{-1} = A^T
$$

Moreover:

$$
\det A
= \alpha_3 \cdot (\alpha_1 \times \alpha_2)
= \alpha_3 \cdot \alpha_3
= \|\alpha_3\|^2
= 1
$$

Hence:

$$
\det A = +1
$$

So $A$ is a **rotation matrix** (a special orthogonal matrix).

---

## 3. The Inverse Also Satisfies the Relation

Write:

$$
A^{-1} = A^T =
\begin{bmatrix}
\alpha_1^T \\
\alpha_2^T \\
\alpha_3^T
\end{bmatrix}
$$

Let the columns of $A^{-1}$ be $\beta_1, \beta_2, \beta_3$, i.e.

$$
A^{-1} = [\beta_1\ \beta_2\ \beta_3]
$$

where

$$
\beta_j =
\begin{bmatrix}
(\alpha_1)_j \\
(\alpha_2)_j \\
(\alpha_3)_j
\end{bmatrix}
$$

Since $A$ is orthogonal, so is $A^{-1}$, hence $\beta_1, \beta_2, \beta_3$ form an orthonormal basis.

Now compute the determinant:

$$
\det(A^{-1}) = \frac{1}{\det A} = \frac{1}{1} = 1
$$

Since $\det(A^{-1}) = +1$, the basis $(\beta_1, \beta_2, \beta_3)$ is right-handed. Therefore:

$$
\beta_3 = \beta_1 \times \beta_2
$$

So the inverse matrix satisfies the **same** cross product relation as $A$.

$$
\boxed{
\alpha_3 = \alpha_1 \times \alpha_2
\quad \Longrightarrow \quad
\beta_3 = \beta_1 \times \beta_2
}
$$

---

## 4. The Set of Such Matrices

Define:

$$
\mathcal{S}
= \left\{
A \in \mathbb{R}^{3 \times 3}
\;\middle|\;
A = [\alpha_1\ \alpha_2\ \alpha_3],\
\alpha_i \cdot \alpha_j = \delta_{ij},\
\alpha_3 = \alpha_1 \times \alpha_2
\right\}
$$

This is exactly the **special orthogonal group** in dimension 3:

$$
\mathcal{S} = SO(3)
= \left\{
A \in \mathbb{R}^{3 \times 3}
\;\middle|\;
A^T A = I,\ \det A = 1
\right\}
$$

---

## 5. Algebraic Properties of $\mathcal{S} = SO(3)$

### 5.1 Closure under multiplication

If $A, B \in SO(3)$, then:

$$
(AB)^T (AB) = B^T A^T A B = B^T B = I
$$

and

$$
\det(AB) = \det A \det B = 1 \cdot 1 = 1
$$

So:

$$
AB \in SO(3)
$$

### 5.2 Identity

$$
I \in SO(3)
$$

### 5.3 Closure under inverse

If $A \in SO(3)$, then $A^{-1} = A^T$ and:

$$
\det(A^{-1}) = 1
$$

so:

$$
A^{-1} \in SO(3)
$$

### 5.4 Associativity

Matrix multiplication is associative.

Therefore $SO(3)$ is a **group** under matrix multiplication.

---

## 6. Reflexivity and Symmetry of the Relation

Define a relation $\sim$ on $SO(3)$ by:

$$
A \sim B
\quad \Longleftrightarrow \quad
B = A^{-1}
$$

Then:

- **Reflexivity?** Not in the usual sense, because $A \sim A$ would require $A = A^{-1}$, which only holds for special matrices (e.g. $A = I$ or rotations by $\pi$).

- **Symmetry:** If $A \sim B$, then $B = A^{-1}$, so $A = B^{-1}$, hence $B \sim A$. So the relation is symmetric.

- **The key property you want:** If $A \in SO(3)$, then $A^{-1} \in SO(3)$. This is **closure under inverse**, which is one of the group axioms.

So the correct terminology is not "reflexivity" but **closure under inversion** (and closure under multiplication), which makes $SO(3)$ a group.

---

## 7. Summary

| Statement                                                              | Result                                    |
| ---------------------------------------------------------------------- | ----------------------------------------- |
| $A$ has orthonormal columns with $\alpha_3 = \alpha_1 \times \alpha_2$ | $A \in SO(3)$                             |
| $\det A$                                                               | $+1$                                      |
| $A^{-1}$                                                               | $A^T$                                     |
| Columns of $A^{-1}$                                                    | Also orthonormal, right-handed            |
| Relation for $A^{-1}$                                                  | $\beta_3 = \beta_1 \times \beta_2$        |
| The set $\mathcal{S}$                                                  | $SO(3)$, the special orthogonal group     |
| Structure                                                              | A group under matrix multiplication       |
| Key property                                                           | Closed under multiplication and inversion |

$$
\boxed{
\mathcal{S} = SO(3)
= \left\{
A \in \mathbb{R}^{3 \times 3}
\;\middle|\;
A^T A = I,\ \det A = 1
\right\}
}
$$

This is the set of all **rotations** in three-dimensional space, and it is a **group**, not merely a set closed under inversion.

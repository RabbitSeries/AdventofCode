from sympy import symbols, Eq, solve
px, py, pz, vx, vy, vz = symbols('px py pz vx vy vz')
t1, t2, t3 = symbols('t1 t2 t3')
hailstones = [
    ([233210433951170, 272655040388795, 179982504986147], [39, -98, 166]),
    ([385274025881243, 351578921558552, 375160114124378], [-71, -36, -9]),
    ([298962016918939, 322446494312107, 293073189215975], [36, 8, 96])
]
equations = []
for i, (p, d) in enumerate(hailstones):
    t = [t1, t2, t3][i]
    equations.append(Eq(px + vx * t, p[0] + d[0] * t))
    equations.append(Eq(py + vy * t, p[1] + d[1] * t))
    equations.append(Eq(pz + vz * t, p[2] + d[2] * t))
solution = solve(equations, (px, py, pz, vx, vy, vz, t1, t2, t3), dict=True)
sol = solution[0]
print("Solution 2: ", sol[px] + sol[py] + sol[pz])
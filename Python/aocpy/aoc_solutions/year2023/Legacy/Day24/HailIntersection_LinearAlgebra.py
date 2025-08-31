from sympy import symbols, Eq, solve
px, py, pz, vx, vy, vz = symbols('px py pz vx vy vz')
t1, t2, t3 = symbols('t1 t2 t3')
lines = open("input.txt").read().rstrip().splitlines()
hailstones = [((list(map(int, line.split('@')[0].strip().split(','))),
                list(map(int, line.split('@')[1].strip().split(',')))))
              for line in lines[0:3]]
equations = []
for (p, d), t in zip(hailstones, [t1, t2, t3]):
    equations.append(Eq(px + vx * t, p[0] + d[0] * t))
    equations.append(Eq(py + vy * t, p[1] + d[1] * t))
    equations.append(Eq(pz + vz * t, p[2] + d[2] * t))
solution = solve(equations, (px, py, pz, vx, vy, vz, t1, t2, t3), dict=True)
for sol in solution:
    # all Integer based solve
    print("Solution 2: ", sol[px], sol[py], sol[pz], sol[px] + sol[py] + sol[pz])

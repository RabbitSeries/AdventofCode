# Day20 Visualize

```mermaid
flowchart LR
broadcaster --> np & mg & vd & xr
zz --> sq
nd --> br
sq --> kj
br --> kt
ks --> qf
tb --> nx
dv --> hm
kj --> vh
kq --> hd
hd --> ps
qn --> pc
kt --> jl
fd --> mx
hm --> cr
qc --> nd
sv --> bv
ph --> zz
lc --> &xm
nf --> &gr
qb --> &dt
sr --> &vt
np --> &xm & ph
jl --> &vt & tb
ps --> kc & &dt
fh --> dm & &gr
bv --> fp & &xm
fp --> qg & &xm
rn --> &vt & qc
nx --> vr & &vt
dm --> nf & &gr
xr --> &vt & rn
vr --> tg & &vt
tq --> &gr & fh
cr --> kq & &dt
vd --> &dt & ks
dz --> &gr & fd
qj --> lc & &xm
kc --> &dt & gf
gf --> &dt & qb
vh --> &xm & sv
xd --> xg & &gr
tg --> sr & &vt
qg --> &xm & qj
mx --> &gr & xd
pc --> tq & &gr
xg --> &gr & qn
qf --> &dt & dv
mg --> dz & &gr

&qd --> &rm
&dh --> &rm
&dp --> &rm
&bb --> &rm
&rm --> rx

&vt --> &bb & nd & qc & xr & br & tb & kt
&gr --> &dp & mg & fd & qn
&dt --> vd & dv & &dh & hm & ks & hd & kq
&xm --> zz & sv & sq & ph & kj & np & &qd
```

## Keypoint

There is exactly one low pulse sent to rm (the module before rx) in each part(I haven't figured out why).

```txt
Solution 1: 800830848
Low 1 High 3147 Push loop at 3001
                Low pulse at 3001 Push 0th Pulse

Low 1 High 5256 Push loop at 3027
                Low pulse at 3027 Push 0th Pulse

Low 1 High 3714 Push loop at 2877
                Low pulse at 2877 Push 0th Pulse

Low 1 High 4434 Push loop at 2907
                Low pulse at 2907 Push 0th Pulse

Solution 2: 244055946148853
```

And if the low pulse is not at final push's first pulse. Then the problem will be a very complex CRT (Chinese Remainder Theory) problem.

Actually [2024 Day 14 Restroom Redoubt](../../2024/Day14/RobotPatrol.h) utilized an implemention of this theory.

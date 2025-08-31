from queue import Queue
from functools import cache
import re
from aocpy.aoc_libs.utils.ISolution import ISolution


class Face:
    def __init__(self, surface: list[str], coordinates: list[tuple[int, ...]], zero: tuple[int, ...], faceId: int, scale: int):
        self.surface, self.crdnts, self.zero, self.faceId, self.scale = surface, coordinates, zero, faceId, scale

    @staticmethod
    def vec(cFrom: tuple[int, ...], cTo: tuple[int, ...]):
        return tuple(x2 - x1 for x1, x2 in zip(cFrom, cTo))

    @staticmethod
    def vecAdd(vL: tuple[int, ...], vR: tuple[int, ...]) -> tuple[int, ...]:
        return tuple(x1 + x2 for x1, x2 in zip(vL, vR))

    def nextCrdns(self, drct: int) -> list[tuple[int, ...]]:
        def crossProduct(vFrom: tuple[int, int, int], vTo: tuple[int, int, int]):
            return tuple(vFrom[(axis + 1) % 3] * vTo[(axis + 2) % 3] - vFrom[(axis + 2) % 3] * vTo[(axis + 1) % 3] for axis in range(0, 3))

        def normolize(vec: tuple[int, int, int], len: int = 1):
            return tuple(0 if v == 0 else len*v//abs(v) for v in vec)

        # 0 1
        # 3 2
        align = [self.crdnts[i % 4] for i in range(drct, drct + 4)]
        # vec1: 0 -> 1
        # vec2:      |
        #            v
        #       3    2
        vec1 = self.vec(align[0], align[1])
        vec2 = self.vec(align[1], align[2])
        rotate = crossProduct(vec1, vec2)
        adjustment = [self.vecAdd(v, vec1) if i == 0 or i == 3 else self.vecAdd(v, normolize(rotate, self.scale-1))
                      for i, v in enumerate(align)]
        return [adjustment[(i - drct) % 4] for i in range(0, 4)]

    def exitEdgeOn(self, drct: int):
        #  0    1
        #       |    --> leaving
        #       v    --> leaving
        #  3    2
        return tuple(self.crdnts[(drct+i) % 4] for i in [1, 2])

    def exitOffSet(self, p: tuple[int, int], drct: int):
        return [p[0], self.scale-1-p[1], self.scale-1-p[0], p[1]][drct]

    def enterEdgeOn(self, drct: int):
        #  0    1
        #  ^-->entering
        #  |-->entering
        #  3    2
        return tuple(self.crdnts[(drct+i) % 4] for i in [3, 0])

    def isValid(self, pos: tuple[int, int]):
        return pos[0] >= 0 and pos[1] >= 0 and pos[0] < self.scale and pos[1] < self.scale


class MonkeyMap(ISolution):
    unit_vec = [(x, y) for x, y in zip([0, 1, 0, -1], [1, 0, -1, 0])]
    VOID = " "
    PATH = "."
    BLOCK = "#"

    @cache
    def rvtToBegin(self, pos: tuple[int, int], drct: int):
        nPos = self.stepNext(pos, (drct+2) % 4)
        while self.isValid(nPos):
            pos = nPos
            nPos = self.stepNext(nPos, (drct+2) % 4)
        return pos

    def stepNext(self, pos: tuple[int, ...], drct: int):
        return tuple(c+d for c, d in zip(pos, self.unit_vec[drct]))

    @staticmethod
    def vecLen(begin: tuple[int, ...], end: tuple[int, ...], unitVec: tuple[int, ...]):
        vector = (r1 - r2 for r1, r2 in zip(end, begin))
        return next(crdn // unit for crdn, unit in zip(vector, unitVec) if unit != 0)

    def forward(self, pos: tuple[int, int], drct: int, step: int) -> tuple[int, int]:
        for _ in range(step):
            nPos = self.stepNext(pos, drct)
            nPos = nPos if self.isValid(nPos) else self.rvtToBegin(pos, drct)
            if self.rawMap[nPos[0]][nPos[1]] == self.BLOCK:
                break
            pos = nPos
        return pos

    def password(self, pos: tuple[int, ...], drct: int):
        # Generator expressions must be parenthesized if not sole argument Pylance
        return (pos[0]+1)*1000 + (pos[1]+1)*4 + drct

    def Part1(self, f):
        mapData, insData = f.read().split("\n\n")
        self.rawMap = mapData.splitlines()
        self.instructions = [(t, int(s)) for t, s in re.findall(R"([A-Z]*)(\d+)", insData)]
        self.rawRows = len(self.rawMap)
        self.start = (0, next(i for i, c in enumerate(self.rawMap[0]) if c != self.VOID))

        curDir = 0
        pos = self.start
        for turn, step in self.instructions:
            if turn == "R":
                curDir = (curDir + 1) % 4
            elif turn == "L":
                curDir = (curDir - 1) % 4
            pos = self.forward(pos, curDir, step)
        print(f"Part 1: {self.password(pos, curDir)}")

    def isValid(self, p: tuple[int, int]):
        return p[0] >= 0 and p[1] >= 0 and p[0] < len(self.rawMap) and p[1] < len(self.rawMap[p[0]]) and self.rawMap[p[0]][p[1]] != self.VOID

    def snapShot(self, x: int, y: int) -> list[str]:
        return [self.rawMap[i][y:y+self.scale] for i in range(x, x+self.scale)]

    def faceEntrence(self, pFrom: tuple[int, int], fFrom: int, dFrom: int):
        toDir = self.Adjacentes[fFrom][dFrom][1]
        offset = self.faces[fFrom].exitOffSet(pFrom, dFrom)
        return [(offset, 0), (0, self.scale-1-offset), (self.scale-1-offset, self.scale-1), (self.scale-1, offset)][toDir]

    def foldCube(self):
        q: Queue[tuple[int, int, int]] = Queue()
        q.put((*self.start, 0))
        visited: set[tuple[int, int]] = set([self.start])
        while not q.empty():
            x, y, faceId = q.get()
            for i in range(4):
                nX, nY = x + self.unit_vec[i][0]*self.scale, y + self.unit_vec[i][1]*self.scale
                if self.isValid((nX, nY)) and (nX, nY) not in visited:
                    nFaceId = len(visited)
                    self.faces.append(Face(self.snapShot(nX, nY),
                                           self.faces[faceId].nextCrdns(i),
                                           (nX, nY), nFaceId, self.scale))
                    visited.add((nX, nY))
                    q.put((nX, nY, nFaceId))
        for face in self.faces:
            for exitDir in [i for i in range(4) if self.Adjacentes[face.faceId][i] is None]:
                if not self.findNextFaceOn(face.faceId, exitDir):
                    raise ValueError("Not found")

    def findNextFaceOn(self, fFrom: int, exitDir: int):
        knownAdjFaces = set([adj[0] for adj in self.Adjacentes[fFrom] if adj])
        exitEdge = self.faces[fFrom].exitEdgeOn(exitDir)
        for nfaceId in set(range(6)) - knownAdjFaces - {fFrom, }:
            for enterDir in range(4):
                if exitEdge == self.faces[nfaceId].enterEdgeOn(enterDir)[::-1]:
                    self.Adjacentes[fFrom][exitDir] = (nfaceId, enterDir)
                    return True
        return False

    def forwardOnFace(self, pos: tuple[int, int], drct: int, step: int, faceId: int) -> tuple[int, ...]:
        for _ in range(step):
            nPos = self.stepNext(pos, drct)
            face = self.faces[faceId]
            if face.isValid(nPos):
                nFaceId, nDir = faceId, drct
            else:
                nPos = self.faceEntrence(pos, faceId, drct)
                (nFaceId, nDir) = self.Adjacentes[faceId][drct][0:2]
            if self.faces[nFaceId].surface[nPos[0]][nPos[1]] == self.BLOCK:
                break
            # Enter next face
            pos, drct, faceId = nPos, nDir, nFaceId
            # self.printCanvas(pos, faceId, drct)
        return (pos, drct, faceId)

    def Part2(self, f):
        self.scale = 50
        self.faces = [Face(self.snapShot(*self.start), [(self.scale-1, 0, self.scale-1),
                                                        (self.scale-1, self.scale-1, self.scale-1),
                                                        (self.scale-1, self.scale-1, 0),
                                                        (self.scale-1, 0, 0)],
                           self.start, 0, self.scale)]
        # Adjacentes[exitDirection] = (enterFaceId, directionAfterEnter, the adjacent vector is reversed or not)
        # Actually, this design is so good that I don't have to judge reverse at all!!
        # 0 1
        # 3 2
        # And the crossproduct rule to produce next face's coordinates ensures that all faces' coordinates
        # form a vector vertical to the face and point to the inside of the cube in right hand's rule wrapping 0 -> 1 -> 2 -> 3
        self.Adjacentes: list[list[tuple[int, int] | None]] = [[None]*4 for _ in range(6)]
        self.canvas = [str(line) for line in self.rawMap]
        self.foldCube()
        curDir = 0
        pos = (0, 0)
        faceId = 0
        for turn, step in self.instructions:
            if turn == "R":
                curDir = (curDir + 1) % 4
            elif turn == "L":
                curDir = (curDir - 1) % 4
            (pos, curDir, faceId) = self.forwardOnFace(pos, curDir, step, faceId)
        print(f"Part 1: {self.password(Face.vecAdd(pos, self.faces[faceId].zero), curDir)}")

    # def printCanvas(self, pos: tuple[int, ...], faceId: int, drct: int):
    #     face = self.faces[faceId]
    #     line = self.canvas[pos[0]+face.zero[0]]
    #     col = pos[1]+face.zero[1]
    #     self.canvas[pos[0]+face.zero[0]] = line[:col] + ">v<^"[drct] + line[col+1:]
    #     with open("Day22/output.txt", "w") as f:
    #         f.writelines([line + "\n" for line in self.canvas])


if __name__ == "__main__":
    solution = MonkeyMap()
    solution.Part1()
    solution.Part2()

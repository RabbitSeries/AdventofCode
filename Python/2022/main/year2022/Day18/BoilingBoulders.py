from typing import Callable
import numpy as np
from queue import Queue
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D


class BoilingBoulders:
    dxyz = [[0, 0, 0, 0, 1, -1],
            [0, 0, 1, -1, 0, 0],
            [1, -1, 0, 0, 0, 0]]

    UNKOWN, CHECKING, AIR, LAVA, AIR_PARCEL = range(5)

    def __init__(self):
        with open("Day18/input.txt") as f:
            # f.__enter__()
            self.crdns = [tuple(map(int, line.split(","))) for line in f.read().splitlines()]
            # f.__exit__()
        self.scale = tuple(np.max(np.array(self.crdns), axis=0) + 1)
        self.container = np.zeros(self.scale, dtype=int)
        # coordinates matrix
        # [v1,
        #  v2,
        #  v3,
        #  ...] transpose to column vector matrix
        # [x,
        #  y,
        #  z]
        # tuple -> (arrayx, arrayy, arrayz) -> continaer.__setelement__ performs a for crdn in zip(*tupleArr): then performs expand set_element:
        # ref = self.data # obtain data
        # for axis in crdn[:-1]: # the purpose if to perfrom: self.data[0][1][2] = value
        #   ref = ref[axis]
        # ref[crdn[-1]] = value  # expand to last axis, then assign the reference
        self.container[tuple(np.array(self.crdns).T)] = self.LAVA

    def isValid(self, pos: tuple[int, ...]):
        return all(c >= 0 and c < self.scale[i] for i, c in enumerate(pos))

    @staticmethod
    def surroundings(pos: tuple[int, ...]):
        # for i in range(6):
        #     yield tuple(c + delta[i] for c, delta in zip(pos, BoilingBoulders.dxyz))
        # return (tuple(c + delta[i] for c, delta in zip(pos, BoilingBoulders.dxyz))for i in range(6))
        return (tuple(c + d for c, d in zip(pos, delta)) for delta in zip(*BoilingBoulders.dxyz))

    def count(self, condition: Callable[[tuple[int, ...]], bool]):
        return sum(1 for lava in self.crdns for nextCube in self.surroundings(lava)
                   if not self.isValid(nextCube) or condition(nextCube))

    def Part1(self):
        print(f"Part 1: {self.count(lambda srd: self.container[srd] == self.UNKOWN)}")

    def Flood(self, memo: np.ndarray, probe: tuple[int, ...]):
        q, traversed = Queue(), [probe]
        q.put(probe)
        isAir = False
        while not q.empty():
            cube = q.get()
            for nCube in self.surroundings(cube):
                if not self.isValid(nCube):
                    isAir = True
                    continue
                if memo[nCube] == self.UNKOWN:
                    memo[nCube] = self.CHECKING
                    q.put(nCube)
                    traversed.append(nCube)

        fill = self.AIR if isAir else self.AIR_PARCEL
        for toFill in traversed:
            memo[toFill] = fill

    def Part2(self):
        for lava in self.crdns:
            for probe in self.surroundings(lava):
                if self.isValid(probe) and self.container[probe] == self.UNKOWN:
                    self.Flood(self.container, probe)
        print(f'Part 2: {self.count(lambda srd: self.container[srd] == self.AIR)}')
        # stdio seems to be trauncting the buf? use file io here
        # with open("exception.log", "w") as f:
        #     f.write(f"{[tuple(map(int, pos)) for pos in np.array(np.where(self.container == self.AIR_PARCEL)).T]}")

        # print([tuple(map(int, pos)) for pos in np.array(np.where(self.container == self.AIR_PARCEL)).reshape(-1, 3)])

    # TODO enclosing LAVA inhibits the AIR_POCKET, don't know how to visualize it
    def append(self, ax: Axes3D, container: np.ndarray, facecolors: np.ndarray):
        ax.voxels(
            *[axis-0.5 for axis in np.indices(np.array(container.shape) + 1)],  # type: ignore
            container,
            facecolors=facecolors,
            linewidth=0
        )
        max_dim = max(container.shape)
        ax.set_xlabel('X axis')
        ax.set_ylabel('Y axis')
        ax.set_zlabel('Z axis')
        ax.set_xlim(0, max_dim)
        ax.set_ylim(0, max_dim)
        ax.set_zlim(0, max_dim)
        # ax.scatter(*np.indices(container.shape), c='black', marker='o', s=5, label='Center')
        # [coor for coor in np.ndindex(container.shape)]
        ax.scatter(*np.where(container), c='black', marker='o', s=5, label='Center')  # type: ignore

    def visualize(self):
        fig = plt.figure(figsize=(15, 8))
        gs = fig.add_gridspec(1, 2)
        # WTFFFFFFFFFFFFFFFFF?, [:,0] is passing a getelem tuple: (slicing(None), 0)
        ax1, ax2 = fig.add_subplot(gs[:, 0], projection='3d'), fig.add_subplot(gs[0, 1], projection='3d')
        if not isinstance(ax1, Axes3D) or not isinstance(ax2, Axes3D):
            return
        ax1.set_title("LAVA cubes")
        ax2.set_title("AIR_PARCEL cubes")
        # self.append(ax, self.crdns)
        # self.append(ax, [(2, 2, 5)], 'red', 1)
        facecolors = np.zeros((*self.container.shape, 4))  # add another dimension RGBA (RGB alpha)
        color_alpha_map = {                        # Color mapping
            self.LAVA: ((1, 0, 0), 0.1),            # Red for LAVA with alpha=0.1
            self.AIR: ((1, 1, 1), 0.0),             # White for AIR with alpha=0.5
            self.AIR_PARCEL: ((0, 0, 1), 0.1),      # Blue for AIR_POCKET with alpha=0.8
            self.CHECKING: ((0.5, 0.5, 0.5), 0.3)   # Gray for CHECKING with alpha=0.3
        }

        for value, (color, alpha) in color_alpha_map.items():  # Apply colors and alphas based on memo values
            mask = self.container == value
            facecolors[mask] = (*color, alpha)  # facecolors[mask][0-3] is RGBA, so add another 1 dimension is enough

        self.append(ax1, self.container == self.LAVA, facecolors)
        self.append(ax2, self.container == self.AIR_PARCEL, facecolors)

        # plt.title("3D Cube Visualization", loc='center') # only sets last subplot
        fig.suptitle("3D Cube Visualization (1x1x1 cubes)")
        plt.show()

# if __name__ == "__main__":
#     BoilingBoulders.append = append  # type: ignore
#     BoilingBoulders.visualize = visualize  # type: ignore
#     vis = BoilingBoulders()
#     vis.Part1()
#     vis.Part2()
#     vis.visualize()  # type: ignore


if __name__ == "__main__":
    solution = BoilingBoulders()
    solution.Part1()
    solution.Part2()
    # solution.visualize()

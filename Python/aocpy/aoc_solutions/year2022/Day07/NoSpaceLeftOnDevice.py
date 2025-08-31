from __future__ import annotations  # For lazy annotation
from functools import cache

from aocpy.aoc_libs.utils.ISolution import ISolution  # for memoization


class NoSpaceLeftOnDevice(ISolution):
    def Part1(self, f):
        blocks = [block.strip().splitlines() for block in f.read().split('$ ls')]
        root = cwd = None
        for lines in blocks:
            for fileOrDir in lines:
                if fileOrDir.startswith('$ cd'):
                    break
                self.createFile(cwd, fileOrDir)
            cwd = self.cd(cwd, [cmd for cmd in lines if cmd.startswith('$ cd')])
            if not root:
                root = cwd
        dirs = self.dfs(root)
        self.sizes = sorted([self.countFolderSize(dir, True) for dir in dirs])
        print("Part 1:", sum(list(size for size in self.sizes if size <= 100000)))

    def Part2(self, f):
        TOTAL, NEEDED = 70_000_000, 30_000_000
        requiredFree = NEEDED - min(TOTAL - self.sizes[-1], NEEDED)
        print("Part 2:", next(s for s in self.sizes if s >= requiredFree))

    @cache
    def countFolderSize(self, dir: File, /, recursive=False) -> int:
        return sum([self.countFolderSize(f, recursive)
                    if recursive and not f.fileSize
                    else f.fileSize or 0
                    for n, f in dir.subFile.items() if n not in ['.', '..']], 0)

    def cd(self, cwd: File | None, cdList: list[str]) -> File | None:
        for dir in cdList:
            dir = dir[len('$ cd'):].strip()
            if not cwd:
                cwd = File(dir, None)
            elif dir != '..':
                cwd = cwd.subFile.setdefault(dir, File(dir, cwd))
            elif cwd.parent:
                cwd = cwd.parent
        return cwd

    def createFile(self, pwd: File | None, fileOrDir: str):
        if not pwd:
            return
        filesize, filename = fileOrDir.split()
        isDir = filesize == 'dir'
        pwd.subFile.setdefault(filename, File(filename, pwd, isDir, None if isDir else int(filesize)))

    def dfs(self, root: File | None) -> list[File]:
        return [root, *[f for n, f in root.subFile.items() if (n not in ['.', '..'] and f.isDir) for f in self.dfs(f)]] if root else []


class File:
    def __init__(self, name: str, parent: File | None, /, isDir: bool = True, fileSize: int | None = None) -> None:
        self.name, self.parent, self.isDir, self.fileSize = name, parent, isDir, fileSize
        self.subFile: dict[str, File] = {'.': self, **({'..': parent} if parent else {})}
        self.absPath: list[str] = parent.absPath + [name] if parent else []

    def getAbsPath(self) -> str:
        return '/' + '/'.join(self.absPath)

    def __repr__(self) -> str:
        # return f"File(name='{self.name}',parent={self.parent.name if self.parent else 'None'},isDir={self.isDir},fileSize={self.fileSize})"
        return f"File(name='{self.name}',parent={self.parent},isDir={self.isDir},fileSize={self.fileSize})"

    def __hash__(self) -> int: return (self.getAbsPath().__hash__() << 1) & self.isDir


if __name__ == "__main__":
    solution = NoSpaceLeftOnDevice()
    with open("Day07/input.txt") as f:
        solution.Part1(f)
    with open("Day07/input.txt") as f:
        solution.Part2(f)

from __future__ import annotations
from functools import lru_cache


class File:
    def __init__(self, name: str, parent: File | None, /, isDir: bool = True, fileSize: int | None = None) -> None:
        self.name = name
        self.isDir = isDir
        self.fileSize = fileSize
        self.parent = parent
        self.subFile: dict[str, File] = dict([('.', self)])
        if parent:
            self.subFile.update([('..', parent)])
            self.absPath: list[str] = parent.absPath + [name]
        else:
            self.absPath = []

    def getAbsPath(self) -> str:
        return '/' + '/'.join(self.absPath)

    def __repr__(self) -> str:
        # return f"'{self.getAbsPath()}', {self.isDir}"
        return f"File(name='{self.name}',parent={self.parent.name if self.parent else 'None'},isDir={self.isDir},fileSize={self.fileSize})"

    def __hash__(self) -> int:
        return self.getAbsPath().__hash__() << 1 & self.isDir


@lru_cache(maxsize=None)
def countFolderSize(dir: File, /, recursive=False) -> int:
    if not recursive:
        return sum([f.fileSize for f in dir.subFile.values() if f.fileSize], 0)
    else:
        return sum([countFolderSize(f, recursive) if not f.fileSize else f.fileSize for n, f in dir.subFile.items() if n != '.' and n != '..'], 0)


def cd(pwd: File | None, cdList: list[str]) -> File | None:
    for dir in cdList:
        dir = dir[len('$ cd'):].strip()
        if not pwd:
            pwd = File(dir, None)
            continue
        if not dir.startswith('..'):
            pwd = pwd.subFile.setdefault(dir, File(dir, pwd))
        elif pwd.parent:
            pwd = pwd.parent
    return pwd


def createFile(pwd: File | None, fileOrDir: str):
    if pwd:
        filesize, filename = fileOrDir.split()
        if filesize.startswith('dir'):
            pwd.subFile.setdefault(filename, File(filename, pwd))
        else:
            pwd.subFile.setdefault(filename, File(filename, pwd, isDir=False, fileSize=int(filesize)))


def dfs(root: File | None) -> set[File]:
    if not root:
        return set()
    return set().union(*([fileOrDir] + list(dfs(fileOrDir)) for name, fileOrDir in root.subFile.items() if fileOrDir.isDir and name != '.' and name != '..'))


with open("input.txt") as f:
    root: File | None = None
    pwd: File | None = None
    blocks = [block.strip().splitlines() for block in f.read().split('$ ls')]
    for fileList in blocks:
        for fileOrDir in fileList:
            if fileOrDir.startswith('$ cd'):
                break
            createFile(pwd, fileOrDir)
        pwd = cd(pwd, [cdOp for cdOp in fileList if cdOp.startswith('$ cd')])
        if not root:
            root = pwd
    if root:
        dirList: set[File] = {root}.union(dfs(root))
        dirSize = [countFolderSize(dir, True) for dir in dirList]
        print("Part 1:", sum(list(size for size in dirSize if size <= 100000)))
        TOTAL = 70000000
        NEEDED = 30000000
        dirSize = sorted(dirSize)
        requiredFree = NEEDED - min(TOTAL - dirSize[-1], NEEDED)
        print("Part 2:", next(free for free in dirSize if free >= requiredFree))

from __future__ import annotations  # For lazy annotation
from functools import lru_cache  # for memoization


class File:
    def __init__(self, name: str, parent: File | None, /, isDir: bool = True, fileSize: int | None = None) -> None:
        self.name, self.parent, self.isDir, self.fileSize = name, parent, isDir, fileSize
        self.subFile: dict[str, File] = {'.': self, **({'..': parent} if parent else {})}
        self.absPath: list[str] = parent.absPath + [name] if parent else []

    def getAbsPath(self) -> str: return '/' + '/'.join(self.absPath)

    def __repr__(self) -> str:
        # return f"File(name='{self.name}',parent={self.parent.name if self.parent else 'None'},isDir={self.isDir},fileSize={self.fileSize})"
        return f"File(name='{self.name}',parent={self.parent},isDir={self.isDir},fileSize={self.fileSize})"

    def __hash__(self) -> int: return (self.getAbsPath().__hash__() << 1) & self.isDir


@lru_cache(maxsize=None)
def countFolderSize(dir: File, /, recursive=False) -> int:
    return sum([countFolderSize(f, recursive)
                if recursive and not f.fileSize
                else f.fileSize or 0
                for n, f in dir.subFile.items() if n not in ['.', '..']], 0)


def cd(pwd: File | None, cdList: list[str]) -> File | None:
    for dir in cdList:
        dir = dir[len('$ cd'):].strip()
        if not pwd: pwd = File(dir, None)
        elif dir != '..':
            pwd = pwd.subFile.setdefault(dir, File(dir, pwd))
        elif pwd.parent:
            pwd = pwd.parent
    return pwd


def createFile(pwd: File | None, fileOrDir: str):
    if not pwd: return
    filesize, filename = fileOrDir.split()
    isDir = filesize == 'dir'
    pwd.subFile.setdefault(filename, File(filename, pwd, isDir, None if isDir else int(filesize)))


def dfs(root: File | None) -> list[File]:
    return [root, *[f for n, f in root.subFile.items() if (n not in ['.', '..'] and f.isDir) for f in dfs(f)]] if root else []


with open("input.txt") as f:
    blocks = [block.strip().splitlines() for block in f.read().split('$ ls')]
root = pwd = None
for lines in blocks:
    for fileOrDir in lines:
        if fileOrDir.startswith('$ cd'):
            break
        createFile(pwd, fileOrDir)
    pwd = cd(pwd, [cmd for cmd in lines if cmd.startswith('$ cd')])
    if not root:
        root = pwd
if root:
    dirs = dfs(root)
    sizes = sorted([countFolderSize(dir, True) for dir in dirs])
    print("Part 1:", sum(list(size for size in sizes if size <= 100000)))
    TOTAL, NEEDED = 70_000_000, 30_000_000
    requiredFree = NEEDED - min(TOTAL - sizes[-1], NEEDED)
    print("Part 2:", next(s for s in sizes if s >= requiredFree))

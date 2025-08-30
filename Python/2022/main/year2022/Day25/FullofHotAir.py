from functools import cache
from math import ceil, log
from aoc_libs.utils.ISolution import ISolution


class FullofHotAir(ISolution):
    digits = "=-012"
    represents = {s: d for s, d in zip("=-012", [-2, -1, 0, 1, 2])}

    def decryptSANFU(self, SANFU: str):
        base = 0
        for d in SANFU:
            base = base*5 + self.represents[d]
        return base

    @cache
    def pow(self, i: int):
        return 1 if i == 0 else 5*self.pow(i-1)

    def highBit(self, num: int):
        return ceil(log(2*abs(num)+1, 5))-1

    def searchDi(self, num: int, i: int):
        dL, dR = -2, 2
        found: int | None = None
        while dL <= dR:
            mid = (dL+dR)//2
            if num >= mid*self.pow(i)+(1-self.pow(i))/2:
                dL = mid + 1
                found = mid
            else:
                dR = mid - 1
        return found

    def encryptSANFU(self, num: int, i: int):
        if i == -1:
            return []
        d_i = self.searchDi(num, i)
        remainBits = self.encryptSANFU(num-d_i*self.pow(i), i - 1)
        return [self.digits[d_i+2], *remainBits]

    def Part1(self):
        with open("Day25/input.txt") as f:
            self.SANFUs = f.read().splitlines()
        acc = sum(self.decryptSANFU(s.strip()) for s in self.SANFUs)
        print(f'Part 1: {self.encryptSANFU(acc, self.highBit(acc))}')

    def Part2(self):
        print(f'Part 2: {self.AOC_FINISH_LINE}')


if __name__ == "__main__":
    solution = FullofHotAir()
    solution.Part1()
    solution.Part2()

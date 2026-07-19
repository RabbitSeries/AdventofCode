from abc import abstractmethod
from glob import glob
import random
import time
from typing import override
import requests
import os
import re


class ProjectStructure:
    def __init__(self, srcPattern: str,
                 yearDayRe: str = r"year(\d+).*Day(\d+)"):
        self.solutions = glob(srcPattern, recursive=True)
        self.places: dict[tuple[int, int], str] = {}
        self.re = re.compile(yearDayRe)
        for p in self.solutions:
            if yD := self.yearDay(p):
                year, day = yD
                if resFile := self.placer(year, day):
                    self.places.update({(year, day): resFile})

    def yearDay(self, srcFile: str):
        if m := self.re.search(srcFile):
            # Python groups start at group 1
            year, day = m.groups()
            return (int(year), int(day))
        return None

    @abstractmethod
    def placer(self, year: int, day: int) -> str:
        pass


class PythonProject(ProjectStructure):
    def __init__(self):
        super().__init__("Python/aocpy/aoc_solutions/**/*.py")

    @override
    def placer(self, year, day):
        return os.path.join("Python", "aocpy", "aoc_solutions", f"year{year}",
                            "resources", "Day{:0>2d}".format(day), "input.txt")


class JavaProject(ProjectStructure):
    def __init__(self, srcPattern="Java/**/main/**/*.java"):
        super().__init__(srcPattern)

    def placer(self, year: int, day: int):
        return os.path.join("Java", f"{year}", "resources",
                            "Day{:0>2d}".format(day), "input.txt")


class KotlinProject(JavaProject):
    def __init__(self):
        super().__init__("Java/**/main/**/*.kt")


class CppProject(ProjectStructure):
    def __init__(self):
        super().__init__("Cpp/**/src/**/*.cc", r"(\d+).*Day(\d+)")

    def placer(self, year, day):
        return os.path.join("Cpp", f"{year}", "resources",
                            "Day{:0>2d}".format(day), "input.txt")


class TypescriptProject(ProjectStructure):
    def __init__(self):
        super().__init__("Typescript/**/src/**/*.ts", r"(\d+).*Day(\d+)")

    def placer(self, year, day):
        return os.path.join("Typescript", f"{year}", "resources",
                            "Day{:0>2d}".format(day), "input.txt")


def download_input(url: str,
                   session_cookie: str, retry: int = 0) -> str | None:
    if retry >= 5:
        return None
    response = requests.get(
        url, headers={"Cookie": f"session={session_cookie}"})
    try:
        response.raise_for_status()
        if response.status_code != requests.codes and len(response.text) == 0:
            raise requests.HTTPError(
                "This request may have been blocked by region/anti-crawler"
                "protection. Try using VPN.", response=response)
        return response.text
    except requests.HTTPError as identifier:
        print(identifier)
        print("Retrying")
        time.sleep(random.randrange(2 ** (retry + 1)))
        return download_input(url, session_cookie, retry + 1)


def process_all_inputs(session_cookie: str,
                       root_dir: str | None = None, overwrite: bool = False):
    if root_dir is None:
        return
    dirs: dict[tuple[int, int], list[str]] = {}
    Projects: list[ProjectStructure] = [PythonProject(), JavaProject(
    ), CppProject(), TypescriptProject(), KotlinProject()]
    for projects in Projects:
        for k, v in projects.places.items():
            dirs.setdefault(k, []).append(v)
    for (year, day), distros in dirs.items():
        for distro in distros:
            if os.path.exists(distro) and not overwrite:
                print(f"Skipping existing: {distro}")
                continue
            url = f"https://adventofcode.com/{year}/day/{day}/input"
            print(f"Downloading {url}...")
            input_text = download_input(url, session_cookie)
            if input_text is None:
                raise ConnectionError("Check network and session cookie")
            os.makedirs(os.path.dirname(distro), exist_ok=True)
            with open(distro, "w") as f:
                f.write(input_text)
            print(f"\tSaved to {distro}")
        print(f"Finishied {year}.{day}")
        print()


project_root = os.getcwd()
session_cookie = os.environ.get("AOC_SESSION_COOKIE")
if not session_cookie:
    envFilePath = os.path.join(project_root, ".env")
    print(
        f'Session cookie not found in environment, looking for {envFilePath}')
    try:
        with open(envFilePath) as file:
            session_cookie = file.read().strip().split("=")[1]
    except FileNotFoundError as e:
        raise ValueError("Missing AOC_SESSION_COOKIE environment variable", e)
overwrite = os.environ.get("OVERWRITE_DOWNLOAD")
process_all_inputs(session_cookie, project_root, eval(overwrite)
                   if overwrite and isinstance(eval(overwrite), bool)
                   else False)

from abc import abstractmethod
from glob import glob
import random
import time
from typing import Callable
import requests
import os
import re


class ProjectStructure:
    def __init__(self, root: str, ext: str):
        self.root = root
        self.solutions = glob(f"**/year*/Day*/*.{ext}", root_dir=root, recursive=True)
        pattern = re.compile(r"year(\d{4})/Day(\d{2})/.*\."+ext)
        self.places = {}
        for p in self.solutions:
            if m := pattern.search(p.replace(os.sep, "/")):
                year, day = int(m[1]), int(m[2])
                self.places.update({(year, day): os.path.join(self.root, self.placer(year, day))})

    @abstractmethod
    def placer(self, year: int, day: int):
        pass


class PythonProject(ProjectStructure):
    def __init__(self, root: str):
        super().__init__(root,  "py")

    def placer(self, year: int, day: int):
        return os.path.join("aocpy", "aoc_solutions", f"year{year}", "resources", "Day{:0>2d}".format(day), "input.txt")


class JavaProject(ProjectStructure):
    def __init__(self, root, ext: str = "java"):
        super().__init__(root, ext)

    def placer(self, year: int, day: int):
        return os.path.join(f"{year}", "resources", "Day{:0>2d}".format(day), "input.txt")


class CppProject(JavaProject):
    def __init__(self, root):
        super().__init__(root, "h")


class TypeScriptProject(JavaProject):
    def __init__(self, root):
        super().__init__(root, "ts")


def download_input(url: str, session_cookie: str, retry: int = 0) -> str | None:
    if retry >= 5:
        return None
    response = requests.get(url, headers={"Cookie": f"session={session_cookie}"})
    try:
        response.raise_for_status()
        return response.text
    except requests.HTTPError as identifier:
        print(identifier)
        print("Retrying")
        time.sleep(random.randrange(2 ** (retry+1)))
        return download_input(url, session_cookie, retry+1)


def process_all_inputs(session_cookie: str, root_dir: str | None = None, overwrite: bool = False):
    if root_dir is None:
        return
    dirs: dict[tuple[int, int], list[str]] = {}
    Projects: list[ProjectStructure] = [PythonProject("Python"), JavaProject("Java"),
                                        CppProject("Cpp"), TypeScriptProject("TypeScript")]
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
    print(f'Session cookie not found in environment, looking for {envFilePath}')
    try:
        with open(envFilePath) as file:
            session_cookie = file.read().strip().split("=")[1]
    except FileNotFoundError as e:
        raise ValueError("Missing AOC_SESSION_COOKIE environment variable", e)
overwrite = os.environ.get("OVERWRITE_DOWNLOAD")
process_all_inputs(session_cookie, project_root, eval(overwrite)
                   if overwrite and isinstance(eval(overwrite), bool) else False)

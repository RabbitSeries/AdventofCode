from typing import List, Tuple
import requests
import http.client
import os
import re
from typing import List, Tuple, Optional


def find_aoc_directories(root_dir: str = None) -> List[Tuple[int, int, Optional[str]]]:
    RepoRootRe = re.compile(
        r"[/\\](?P<year>\d+)", re.IGNORECASE
    )
    found = []
    for entry in os.scandir(root_dir):
        YearMatch = RepoRootRe.search(entry.path)
        if YearMatch:
            for subDir in os.scandir(entry.path):
                DayRootRe = re.compile(r"Day(?P<day>\d+)")
                DayMatch = DayRootRe.search(subDir.path)
                if DayMatch:
                    year = int(YearMatch.group("year"))
                    day = int(DayMatch.group("day"))
                    rel_path = os.path.join(subDir, "input.txt")
                    found.append((year, day, rel_path))

    return sorted(found, key=lambda x: (x[0], x[1] if x[1] is not None else 0))


def download_input(year: int, day: int, session_cookie: str) -> str:
    url = f"https://adventofcode.com/{year}/day/{day}/input"
    headers = {
        "Cookie": f"session={session_cookie}"
    }
    response = requests.get(url, headers=headers)
    response.raise_for_status()
    return response.text.strip()


def process_all_inputs(
    session_cookie: str,
    root_dir: str = None,
    overwrite: bool = False
) -> None:
    dirs = find_aoc_directories(root_dir)
    for year, day, rel_path in dirs:
        if os.path.exists(rel_path) and not overwrite:
            print(f"Skipping existing: {rel_path}")
            continue

        print(f"Downloading {rel_path}...")
        try:
            input_text = download_input(year, day, session_cookie)
            os.makedirs(os.path.dirname(rel_path), exist_ok=True)
            with open(rel_path, "w") as f:
                f.write(input_text)
            print(f"Saved to {rel_path}")
        except Exception as e:
            print(f"Failed {year}/Day{day}: {str(e)}")

session_cookie = os.environ.get("AOC_SESSION_COOKIE")
if not session_cookie:
    raise ValueError("Missing AOC_SESSION_COOKIE environment variable")

script_dir = os.path.dirname(os.path.abspath(__file__))
project_root = os.path.abspath(os.path.join(script_dir, "."))
process_all_inputs(session_cookie, project_root)

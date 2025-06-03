import requests
import os
import re
from typing import List, Tuple, Dict


def find_aoc_directories(root_dirs: List[str] | None = None, overwrite: bool = False) -> Dict[Tuple[int, int], List[str]]:
    RepoRootRe = re.compile(
        r"[/\\](?P<year>\d+)", re.IGNORECASE
    )
    found: Dict[Tuple[int, int], List[str]] = {}
    if root_dirs:
        for root_dir in root_dirs:
            for entry in os.scandir(root_dir):
                YearMatch = RepoRootRe.search(entry.path)
                if YearMatch:
                    for subDir in os.scandir(entry.path):
                        DayRootRe = re.compile(
                            r".*Day(?P<day>\d+)", re.IGNORECASE
                        )
                        DayMatch = DayRootRe.search(subDir.path)
                        if DayMatch and subDir.path[len(DayMatch.group()):].find(os.path.sep) == -1:
                            rel_path = os.path.join(subDir, "input.txt")
                            if os.path.exists(rel_path) and not overwrite:
                                print(f"Skipping existing: {rel_path}")
                                continue
                            year = int(YearMatch.group("year"))
                            day = int(DayMatch.group("day"))
                            found.setdefault((year, day), []).append(rel_path)
        return dict(sorted(found.items(), key=lambda item: (item[0][0], item[0][1])))
    return dict()


def download_input(year: int, day: int, session_cookie: str) -> str:
    url = f"https://adventofcode.com/{year}/day/{day}/input"
    print(f"\nDownloading {url}...")
    headers = {
        "Cookie": f"session={session_cookie}"
    }
    response = requests.get(url, headers=headers)
    response.raise_for_status()
    return response.text


def process_all_inputs(
    session_cookie: str,
    root_dir: str | None = None,
    overwrite: bool = False
) -> None:
    if root_dir:
        dirs = find_aoc_directories([root_dir, os.path.join(root_dir, 'Legacy')])
        for (year, day), distribute_path in dirs.items():
            try:
                input_text = download_input(year, day, session_cookie)
            except Exception as e:
                print(
                    f"Failed to download https://adventofcode.com/{year}/day/{day}/input:", e, 'Session Cookie may have expired', sep='\n\t')
                continue
            for rel_path in distribute_path:
                if os.path.exists(rel_path) and not overwrite:
                    print(f"Skipping existing: {rel_path}")
                    continue
                try:
                    os.makedirs(os.path.dirname(rel_path), exist_ok=True)
                    with open(rel_path, "w") as f:
                        f.write(input_text)
                    print(f"Saved to {rel_path}")
                except Exception as e:
                    print(f"Failed to save {year}/Day{day}: {str(e)}")


session_cookie = os.environ.get("AOC_SESSION_COOKIE")
if not session_cookie:
    raise ValueError("Missing AOC_SESSION_COOKIE environment variable")

script_dir = os.path.dirname(os.path.abspath(__file__))
project_root = os.path.abspath(os.path.join(script_dir, ".."))
process_all_inputs(session_cookie, project_root)

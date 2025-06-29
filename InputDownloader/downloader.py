import requests
import os
import re


def find_aoc_directories(root_dirs: list[str] | None = None, overwrite: bool = False) -> dict[tuple[int, int], list[str]]:
    repo_root_re = re.compile(
        r"[/\\](?P<year>\d+)$", re.IGNORECASE
    )
    day_root_re = re.compile(
        r".*Day(?P<day>\d+)$", re.IGNORECASE
    )
    found: dict[tuple[int, int], list[str]] = {}
    if root_dirs is None:
        return dict()
    for root_dir in root_dirs:
        for entry in os.scandir(root_dir):
            year_match = repo_root_re.search(entry.path)
            if year_match is None:
                continue
            day_root_parents: list[str] = []
            if root_dir.endswith("Java"):
                # Java src root has a main/year<YYYY> subdir due to pom package management (some more hacked configuration may be available) for now
                day_root_parents.append(os.path.join(entry.path, "main", f"year{year_match.group("year")}"))
                # For the same reason, Legacy foler is placed at each year's subFolder
                legacy_folder = os.path.join(day_root_parents[0], "Legacy")
                if os.path.exists(legacy_folder):
                    day_root_parents.append(legacy_folder)
            else:
                day_root_parents.append(entry.path)
            for day_root_parent in day_root_parents:
                for subDir in os.scandir(day_root_parent):
                    day_match = day_root_re.search(subDir.path)
                    if day_match is None:
                        continue
                    distro_path = os.path.join(subDir.path, "input.txt")
                    if os.path.exists(distro_path) and not overwrite:
                        print(f"Skipping existing: {distro_path}")
                        continue
                    year = int(year_match.group("year"))
                    day = int(day_match.group("day"))
                    found.setdefault((year, day), []).append(distro_path)
    return dict(sorted(found.items(), key=lambda item: item[0]))


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
    if root_dir is None:
        return
    root_dirs = [root_dir, os.path.join(root_dir, 'Legacy'), os.path.join(
        root_dir, 'TypeScript'), os.path.join(root_dir, 'Java')]
    dirs = find_aoc_directories(root_dirs, overwrite)
    for (year, day), distros in dirs.items():
        try:
            input_text = download_input(year, day, session_cookie)
        except Exception as e:
            print(f"Failed to download https://adventofcode.com/{year}/day/{day}/input:",
                  e, 'Session Cookie may have expired', sep='\n\t')
            continue
        for distro in distros:
            try:
                os.makedirs(os.path.dirname(distro), exist_ok=True)
                with open(distro, "w") as f:
                    f.write(input_text)
                print(f"\tSaved to {distro}")
            except Exception as e:
                print(f"\tFailed to save {year}/Day{day}:", e)


script_dir = os.path.dirname(os.path.abspath(__file__))
project_root = os.path.abspath(os.path.join(script_dir, ".."))
session_cookie = os.environ.get("AOC_SESSION_COOKIE")
if not session_cookie:
    print(f'Session cookie not found in environment, looking for {project_root}/.env')
    try:
        with open(os.path.join(project_root, ".env")) as file:
            session_cookie = file.read().strip().split("=")[1]
    except FileNotFoundError as e:
        raise ValueError("Missing AOC_SESSION_COOKIE environment variable", e)
overwrite = os.environ.get("OVERWRITE_DOWNLOAD")
process_all_inputs(session_cookie, project_root, eval(overwrite)
                   if overwrite and isinstance(eval(overwrite), bool) else False)

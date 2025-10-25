import pathlib
import time
import subprocess
import sys

BASE_DIR = pathlib.Path(__file__).parent.resolve()


def get_dir_entries(dir_path: pathlib.Path) -> list[pathlib.Path]:
    entries = []

    for entry in dir_path.iterdir():
        if entry.is_dir():
            entries.extend(get_dir_entries(entry))
        else:
            entries.append(entry)

    return entries


GRAPHS_DIR = BASE_DIR / "graphs"
SRC_DIR = BASE_DIR / "src"
ASSETS_DIR = SRC_DIR / "assets"

graph_files = get_dir_entries(GRAPHS_DIR)

total_time = 0

for graph_file in graph_files:
    str_path = str(graph_file)
    str_path = str_path.replace(str(GRAPHS_DIR),
                                str(ASSETS_DIR)).replace(".dot", ".svg")
    new_path = pathlib.Path(str_path)
    if not new_path.parent.exists():
        new_path.parent.mkdir(parents=True)

    path_seperator = "\\" if sys.platform == "win32" else "/"

    command = [
        "dot", "-Tsvg",
        str(graph_file).replace(str(BASE_DIR) + path_seperator, ""), "-o",
        str(new_path).replace(str(BASE_DIR) + path_seperator, "")
    ]
    print(" ".join(command))
    now = time.time()
    result = subprocess.run(command, cwd=BASE_DIR)
    if result.returncode != 0:
        sys.exit(1)
    end = time.time() - now
    total_time += end
    print(f"Time taken: {end:.2f} seconds")

print(f"Total time taken: {total_time:.2f} seconds")

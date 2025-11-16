# from https://github.com/wilsonzlin/minify-html
import subprocess
import os
from pathlib import Path

current_directory = Path.cwd()
target_directory = Path.joinpath(current_directory, Path("data"))
source_directory = Path.joinpath(current_directory, Path("spiffs"))

html_file_names = list()
gz_file_names = list()

for item in target_directory.glob("*.html.gz"):
    gz_file_names.append(item.name)

for file_name in gz_file_names:
    file_path = Path.joinpath(target_directory, Path(file_name))
    if file_path.exists():
        file_path.unlink()

for item in source_directory.glob("*.html"):
        html_file_names.append(item.name)


for file_name in html_file_names:
    file_path = Path.joinpath(target_directory, Path(file_name))
    if file_path.exists():
        file_path.unlink()
    

    subprocess.call([str(Path.joinpath(current_directory,"helper","htmlmini.exe")),
                  "--keep-spaces-between-attributes",
                  "--minify-css",
                  "--minify-js",
                  "--output",
                  str(Path.joinpath(target_directory, Path(file_name))),
                  str(Path.joinpath(source_directory, Path(file_name)))
                  ])
    


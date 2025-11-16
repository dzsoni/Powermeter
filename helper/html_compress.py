# compress html with gzip
import gzip
import os
from pathlib import Path

current_directory = Path.cwd()
target_directory = Path.joinpath(current_directory, Path("data"))

html_file_names = list()

for item in target_directory.glob("*.html"):
        html_file_names.append(item.name)


for file_name in html_file_names:
    htmlf = Path.joinpath(target_directory,Path(file_name))
    gzf   = Path.joinpath(target_directory,Path(file_name +".gz"))
    if htmlf.exists:
          if gzf.exists():
              os.remove(gzf)
          infile = open(htmlf, "rb")
          indata = infile.read()
          outfile = gzip.open(gzf, "wb")
          outfile.write(indata)
          outfile.close()
          infile.close()
          os.remove(htmlf)
          

   
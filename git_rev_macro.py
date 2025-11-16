import subprocess
git_rev = (
    subprocess.check_output(["git", "rev-parse", "HEAD"])
    .strip()
    .decode("utf-8")
)
git_tag = (
    subprocess.check_output(["git", "describe", "--tags"])
    .strip()
    .decode("utf-8")
)

git_branch = (
    subprocess.check_output(["git", "rev-parse", "--abbrev-ref", "HEAD"])
    .strip()
    .decode("utf-8")
)
print("GIT_REV: %s" %git_rev)
print("GIT_TAG: %s" %git_tag)
print("GIT_BRANCH: %s" %git_branch)

# Get the current build time.
import time
build_time = time.strftime("%Y\%m\%d %H:%M:%S")
print("Build time: %s" %build_time)

# read->replace->write
with open("include/version.h.in") as f:
    template = f.read()
template = template.replace("${GIT_REVISION}", git_rev)
template = template.replace("${GIT_TAG}", git_tag)
template = template.replace("${GIT_BRANCH}", git_branch)
template = template.replace("${BUILD_TIME}", build_time)
with open("include/version.h" , "w") as f:
    f.write(template)

print("version.h file modified.")
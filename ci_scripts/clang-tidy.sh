#!/usr/bin/env bash

. ./ci_scripts/common.sh

$SUDO apt install clang-tidy-6.0 -y

# Run one test at the time
for check in $(clang-tidy-6.0 --list-checks -checks='*' | grep "modernize"); do
	run-clang-tidy-6.0 -header-filter='.*' -checks="-*,${check}" -fix
done

# run-clang-tidy-6.0 -header-filter='.*' -checks='-*,modernize-*' -fix

git add $(find src/ plugins/ -name "*.cpp" -or -name "*.h")
git config --global user.name "${GITLAB_USER_NAME}"
git config --global user.email "${GITLAB_USER_EMAIL}"
git commit -m "Apply clang-tidy fixits"

make -j$(nproc) && git push https://jbbgameich:${CI_TOKEN}@gitlab.com/jbbgameich/openboard HEAD:${CI_COMMIT_REF_NAME}

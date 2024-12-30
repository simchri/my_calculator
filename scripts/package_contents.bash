#!/bin/bash
set -e

build_dir="$1"
echo ""
echo ""
echo "Package files check!"
echo ""

expected_package_name="$2"

echo "expect to find package: $expected_package_name"
echo "in working in directory: $build_dir"
echo "as .deb file"

# parse all arguments from $3 on into a files_list:
files_list=()
for i in $(seq 3 $#); do
  files_list+=("${!i}")
done


echo ""
echo ""
echo "found the following .deb files:"
echo ""
ls -l $build_dir/*.deb

match_found=0
match_deb_file=""

for deb_file in "$build_dir"/*.deb; do
  package_name=$(dpkg --info "$deb_file" | grep Package | awk '{print $2}')

  if [ "$package_name" == "$expected_package_name" ]; then
    match_found=1
    match_deb_file="$deb_file"
    break
  fi
done

[ $match_found == 1 ] && echo "ok package found" || { echo "ERROR: expected package $expected_package_name not found as .deb file in $build_dir"; exit 1; }

# get file contents of matched package :
dpkg_out_raw=$(dpkg --contents "$match_deb_file")
# retain sixth column of all lines:
dpkg_out=$(echo "$dpkg_out_raw" | awk '{print $6}')
# drop all directories (lines ending with "/")
dpkg_out=$(echo "$dpkg_out" | grep -v '/$')
# drop the dot in the beginning:
dpkg_out=$(echo "$dpkg_out" | sed 's/^.//')

echo ""
echo ""
# check all expected files in package:
assert_ko=0
for file in "${files_list[@]}"; do
  echo "$dpkg_out" | grep -q "^$file$" && echo "OK  -  found file: $file" ||  { echo "ERROR  -  missing: $file "; assert_ko=1; }
done

# check no extra files in package:
for file in $dpkg_out; do
  echo "${files_list[@]}" | grep -q "^$file$" || { echo "ERROR- extra file: $file "; assert_ko=1; }
done

echo ""
echo ""
[ $assert_ko == 0 ] && echo "all OK - all expected files in package and no extra files!" || { echo "ERROR: One or more assertion failures, see output above!"; exit 1; }
echo ""

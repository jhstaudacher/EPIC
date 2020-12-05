#!/bin/bash
# This script formats all .h and .cpp files inside the current directory (recursively)

# search all .cpp and .h recursively inside the current directory
FILES=$(find ./ -regex '.*\.\(cpp\|h\)')

# execute clang-format
#	-style=file:	use the style defined in the .clang-format file
#	-i:		write the changes directly to the files (instead of stdout)
clang-format -style=file -i $FILES


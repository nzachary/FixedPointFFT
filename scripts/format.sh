#!/bin/bash

find .. -iname '*.hpp' -o -iname '*.cpp' | xargs clang-format -style=file $@

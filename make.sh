#!/bin/sh

BASE_DIR=$(cd $(dirname $0);pwd)
files=( title.md template.md type.md io.md vector.md search.md string.md
        number.md matrix.md dp.md structure.md graph.md geometry.md game.md marathon.md data.md )

cd ${BASE_DIR}
cd source

echo "making with ${files[*]}"
# this make truly standalone html.MathML is not working in chrome 26 now.
pandoc ${files[*]} -c library.css -t html5 -s --toc --mathjax --highlight-style tango  \
    -f markdown+hard_line_breaks -N --filter ../include_file.hs --filter ../yasnippet.hs -o ../index.html

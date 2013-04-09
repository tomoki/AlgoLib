#!/bin/sh

BASE_DIR=$(cd $(dirname $0);pwd)
files=( title.md template.md type.md io.md vector.md search.md string.md
        number.md structure.md graph.md geometry.md game.md data.md)

cd ${BASE_DIR}
cd source

echo "making with ${files[*]}"
# this make truly standalone html.MathML is not working in chrome 26 now.
# pandoc ${files[*]} -t html5  --mathml --highlight-style tango --self-contained -s -o library.html
# pandoc ${files[*]} -t html  --toc --mathjax --highlight-style tango -s -o ../library.html
pandoc ${files[*]} -c buttondown.css -t html  --toc --mathjax --highlight-style tango -s -o ../library.html

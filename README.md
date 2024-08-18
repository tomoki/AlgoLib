# Algolib by Tomoki Imai

このドキュメントは競技プログラミング用に作成したアルゴリズム群をまとめたものです。
http://pushl.net/algolib/ のソースコードです。

# ビルドの方法

ICPC などオンラインドキュメントにアクセスできない環境用に一枚の HTML にすることができるようデザインしています。

## Docker

```
docker build ./ -t my-sphinx
docker run --rm -v ./:/docs my-sphinx sphinx-build -M html source build

```

https://hub.docker.com/r/sphinxdoc/sphinx

# メモ

元々このドキュメントは Markdown + Pandoc で記載されていました。しかしビルドに必要な pandoc が扱いづらい、 Markdown で外部のファイルを include することが標準的な方法ではできないという問題があったため Sphinx に移行しました。

# ssce

![GitHub License](https://img.shields.io/github/license/1mru/ssce)
![GitHub last commit](https://img.shields.io/github/last-commit/1mru/ssce)
![GitHub commit activity](https://img.shields.io/github/commit-activity/w/1mru/ssce)
![GitHub Release](https://img.shields.io/github/v/release/1mru/ssce)


ssceは、Windowsで動作する高速・軽量なテキストエディタです。

<img width="1423" height="732" alt="image" src="https://github.com/user-attachments/assets/a5fbfe74-49cd-409d-958b-2f4571a947e7" />

## ダウンロード

以下のリンクからインストーラをダウンロードして実行してください。

[Releases](https://github.com/1mru/ssce/releases)

## ビルド

以下の手順は、あらかじめ[MSYS2](https://www.msys2.org/)をインストールし、UCRT64環境のターミナルで以下のコマンドを実行していることを前提としています。

~~~bash
pacman -S mingw-w64-ucrt-x86_64-gcc
~~~

また、MSYS2のインストールフォルダをユーザー環境変数の`PATH`に追加しているものとします。

### ビルド手順

1. リポジトリをクローンします。
2. プロジェクトフォルダのルートにある`Makefile`を確認します。
3. プロジェクトフォルダのルートで、以下のコマンドを実行します。

~~~bash
make
~~~

ビルドされたバイナリファイルは`bin`フォルダに出力されます。

> **Note**
>
> リポジトリ内の`lexilla`および`scintilla`フォルダには、あらかじめビルドしたバイナリファイルが含まれています。
>
> 何らかの理由でこれらのバイナリファイルが存在しない場合は、各フォルダ内の`README`を参照し、`ssce`をビルドする前にLexillaとScintillaをそれぞれビルドしてください。

## ドキュメント

ソースコードには詳細なコメントを記載しています。

また、使用しているAPI・ライブラリのドキュメントや、開発時に参考にしたウェブページへのリンクは、[Wiki](https://github.com/1mru/ssce/wiki)にまとめています。

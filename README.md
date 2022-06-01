# STEP2022-HW4

## 仕様

Wikipediaのリンク関係をもとに、あるページからあるページまで辿る経路を一つ検索し表示する。

`/data/`以下に二つのファイルが存在する
- `pages.txt`: ページID, ページタイトル
- `links.txt`: リンク元ページID, リンク先ページID

両者を

## 使い方

基本的にコンパイル後`$ ./a.out [start page name] [goal page name]`で検索される。
  - 発見: 経路を表示
  - 未発見: `Route from [start page name] to [goal page name] is not found`のメッセージ

始点終点を指定しない場合、 「Google→渋谷」 の経路を検索する。


### DFS

エラーのためデバッグ中です。 
エラーの出るコード自体は、ターミナルで
```
$ sh hw1_dfs_error.sh
```
で走ります。

- 「Google→渋谷」 ではエラーが出ます。
- その他の経路では動く場合もあります。



### BFS

ターミナルで
```
$ sh hw1_bfs.sh
```
で走ります。


## DFSで出ているエラーについて
`EXC_BAD_ACCESS`が出ており、現在デバッグ中です。

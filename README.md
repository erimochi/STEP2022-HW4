# STEP2022-HW4

## 仕様

Wikipediaのリンク関係をもとに、あるページからあるページまで辿る経路を一つ検索し表示する。

`/data/`以下に二つのファイルが存在する
- `pages.txt`: ページID, ページタイトル
- `links.txt`: リンク元ページID, リンク先ページID



## 使い方

基本的にコンパイル後`$ ./a.out [start page name] [goal page name]`で検索される。
  - 発見: 経路を表示
  - 未発見: `Route from [start page name] to [goal page name] is not found`のメッセージ

始点終点を指定しない場合、 「Google→渋谷」 の経路を検索する。(シェルスクリプトでコンパイル・実行同時に可能)


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
### 環境
  - macOS 12.2.1
  - Apple clang version 13.0.0 (clang-1300.0.29.30)

### 問題
- 再帰的なDFSで、「Google→渋谷」検索時、Segmentation faultが出る。
- 出る位置は毎回同じ
- 始点終点によっては、エラーなく探索が完了するケースもある

### 検証したこと

- smallのファイルを用い、全パターンの経路に対して検索
	→いずれも問題なし
- 0~999番までのページと関連するリンクのみを読み込み、全パターンの経路に対して検索
	→いずれも問題なし
- デバッガ使用
	→ current = 460219のタイミングで、恐らく次の再帰呼び出しの際に`EXC_BAD_ACCESS`が発生している。
	  `EXC_BAD_ACCESS`は開放済み構造体へのアクセス時や再帰回数の多すぎる時などに生じる

<details><summary>デバッガログ</summary>
  
```
(lldb) next
Process 23016 stopped
* thread #1, queue = 'com.apple.main-thread', stop reason = EXC_BAD_ACCESS (code=2, address=0x7ff7bf6fffe8)
    frame #0: 0x00007ff81ebd0a1b libsystem_c.dylib`fwrite + 35
libsystem_c.dylib`fwrite:
->  0x7ff81ebd0a1b <+35>: movq   %rsi, -0x48(%rbp)
    0x7ff81ebd0a1f <+39>: movq   %rdx, -0x50(%rbp)
    0x7ff81ebd0a23 <+43>: leaq   -0x60(%rbp), %rax
    0x7ff81ebd0a27 <+47>: movq   %rax, -0x38(%rbp)
Target 0: (hw1) stopped.
(lldb) list
   82  		 	for(int i=0; i<next_num; i++){
   83  		 		if(!found_route.empty()){break;}
   84  		 		switch(visited[links[current][i]]){
   85  		 			case 0:
   86  		 				// have not visited yet
   87  			 			cout << "next : " <<  to_string(links[current][i]) << " (not visited)" << endl;
   88  			 			visited[links[current][i]] = 1;
```
</details>
  

- ターミナル上で再帰上限を増やして実行(コンパイル時 `-ftemplate-depth=5000` を追加)
	→ 同じ場所でSegmentation faultが出る

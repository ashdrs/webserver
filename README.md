# Web Server

TCP/IP周りの勉強のためにC言語で作成した、FreeBSD/CentOS用の簡易なWebサーバです。

通常のWebサーバ同様、ブラウザからアクセスすると静的なHTMLが表示できます。

## 前提条件
VirtualBoxでマシンを作成して検証

- ネットワーク設定  
　em0: 192.168.56.100/24（ホストオンリーアダプター）

ブリッジ接続でもいいですが、ホストマシンから接続できる状態にする必要があります。  
VirtualBoxのホストオンリーネットワークは、ホストマシンでifconfig等を行い、vboxnet0インタフェースを見るとわかります。  
FreeBSD10.3とCentOS7.2で動作検証済みです。

## 設定
下記の使い方1でgit cloneすると、config/server.confが設置されてますので編集してください。
- Listen  
　サーバを起動するポートを指定します。  
　【デフォルト値: 80】
- ServerType  
　preforkかforkを選べます。preforkは事前に子プロセスを起動しておき、forkは接続の度に子プロセスを立ち上げます。  
　【デフォルト値: prefork】
- Prefok  
　ServerTypeがpreforkの場合に使用します。起動する子プロセス数を指定します。  
　【デフォルト値: 6】
- DocumentRoot  
　おなじみのhtdocsディレクトリを指定します。このディレクトリを起点にコンテンツを返します。  
　【デフォルト値: ./htdocs】

行の先頭に「#」をつけるとコメントアウトとなります。  
コメントアウト、または指定しない場合デフォルト値が適用されます。

## 使い方
1. インストール

	```
	# git clone https://github.com/ashnb/webserver.git
	```

2. ビルド

	```
	# cd webserver
	# make
	```

3. 起動

	起動しアクセスがあると、log/http.logにログが書き込まれます。
	```
	# sudo ./bin/httpd start
	```

4. ブラウザからアクセス

	ホストマシンのブラウザにて`http://サーバのIP:指定したポート/`でアクセスすると閲覧できます。  
	上記の例だと`http://192.168.56.100:80/`
	
5. 停止

	```
	# sudo ./bin/httpd stop
	```

6. 再ビルド

	```
	# make clean
	# make
	```

## 参考

「[基礎からわかるTCP/IP ネットワーク実験プログラミング](https://www.amazon.co.jp/%E5%9F%BA%E7%A4%8E%E3%81%8B%E3%82%89%E3%82%8F%E3%81%8B%E3%82%8BTCP-IP-%E3%83%8D%E3%83%83%E3%83%88%E3%83%AF%E3%83%BC%E3%82%AF%E5%AE%9F%E9%A8%93%E3%83%97%E3%83%AD%E3%82%B0%E3%83%A9%E3%83%9F%E3%83%B3%E3%82%B0%E2%80%95Linux-FreeBSD%E5%AF%BE%E5%BF%9C-%E6%9D%91%E5%B1%B1/dp/4274065847)」を参考にしています。  



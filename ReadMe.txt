///////////////// ライセンス

ダウンロードした時点でこのファイルに書いてあるライセンスとします。
新しくダウンロードした場合はその中にあるReadMe.txtのライセンスを適用します。

現在のライセンスはとくにありません。
改変自由・再配布自由・使用表記必要なしとします。

外部ライブラリのライセンスは
importフォルダ内にある copyingなり LICENSE なり ReadMe なりを参照してください

///////////////// いろいろ

source/import.lzh は外部ライブラリを固めてあります。
各プロジェクトごとにパスを設定してあるので
解凍するだけでコンパイルできるようになっています
ただし、DirectXはパスを設定してください。

サンプルで使っている素材は自分で作っているか
http://www.s-hoshino.com/f_photo/ 
http://osabisi.sakura.ne.jp/m2/
http://www.niconicommons.jp/
音辞典

の素材を使用しています。
ニコニコモンズは、コモンズ対応サイト専用ライセンスのもあるので注意


source フォルダがライブラリ本体
http://docs.google.com/present/view?id=ddmq9dg5_248fss2h5dq を参照のこと
sample はテストを兼ねた使い方サンプル
tool はあると便利なツール。それぞれの使い方はフォルダの中に書いてあります
bin  は実行ファイル、テスト素材が入っています





///////////////// コードの書き方
基本的には
http://google-styleguide.googlecode.com/svn/trunk/cppguide.xml
http://www.textdrop.net/google-styleguide-ja/cppguide.xml

ここでいいと思いますが、確認するのしんどいので
適当に書いてます。
コードはUTF8 BOM付きで書いています。

インデントはスペース２個派です。
ファイル名が全部小文字なのは
・１ファイル１クラスにならないときがあるためクラス名＝ファイル名とは限らないため
・ファイルシステムによっては大文字小文字を区別する
・インスタンスを空にする場合　Hoge = CLASS Hoge() で行うようにします



#includeの順番は
/config/ 以下のヘッダ
C++ system files
Other librarie's .h files
継承するクラス
メンバとかで使っているクラス

の順番とします。


ソースコードでたびたび出てくる
namespace KEEPOUT{}
の中のクラスはそのソース以外で使うなって意味です。
cppに押し込める方法が思いつかないので外出てます



//////////////////////// そのうち対応するリスト

3Dモデル表示
各種シェーダーサンプル

各ＯＳのシェル
OpenGL,OpenAL対応
Mac,Linux対応

複数行テキストボックス
Ｄ＆Ｄ

GPGPU対応(できるのか？)



//////////////////////// あると便利なもの

winspector http://www.windows-spy.com/
　飛び交うwindowsメッセージをウォッチしてくれます
　ＯＳが重くなるのはしかたないかな？

ResEdit http://www.resedit.net/
　フリーのリソースエディタ
　日本語も使えるので便利です




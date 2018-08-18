Plex PXシリーズ3波チューナー用BDASpecialプラグイン

【これは何？】
Plex社製PCI-Express/USB接続(Box型)3波チューナー用のBonDriver_BDA用BDASpecialプラグインです。
BonDriver_BDA.dllと組み合わせて使用します。

【動作環境】
Windows XP以降 (x86/x64)

【対応チューナー】
Plex社製の下記のチューナーに対応しています（たぶん）。
  PX-Q3PE
  PX-W3PE
  PX-W3PE V2
  PX-W3U3
  PX-W3U3 V2
  PX-W3U2
  PX-S3U2
  PX-S3U
同じPlex社製のPXシリーズでもドングル型タイプのものはこのBDASpecialプラグインの対象外です。

【使い方】
1. BonDriver_BDAの入手
下記URLより、最新バージョンのBonDriver_BDAを入手してください。
https://github.com/radi-sh/BonDriver_BDA/releases
※ BonDriver_BDA改-20180818より前のバージョンでは動作しませんのでご注意ください。

2. x86/x64と通常版/ランタイム内蔵版の選択
BonDriver_BDA付属のReadme-BonDriver_BDAを参考に、BonDriver_BDAと同じものを選択してください。

3. Visual C++ 再頒布可能パッケージのインストール
BonDriver_BDA付属のReadme-BonDriver_BDAを参考に、インストールしてください。

4. BonDriverとiniファイル等の配置
・使用するアプリのBonDriver配置フォルダに、BonDriver_BDA.dllをリネームしたコピーを配置
  通常、ファイル名が"BonDriver_"から始まる必要がありますのでご注意ください。
・用意したdllと同じ名前のiniファイルを配置
  下記のサンプルiniファイルを基に作成してください。
    -BonDriver_PlexPX_T.ini   (地デジ用)
    -BonDriver_PlexPX_S.ini   (衛星用)
    -BonDriver_PlexPX_S3U.ini (PX-S3U用)
・PlexPX.dllファイルを配置
  PlexPX.dllファイルはリネームせずに1つだけ配置すればOKです。

(配置例)
  BonDriver_PlexPX_S0.dll
  BonDriver_PlexPX_S0.ini
  BonDriver_PlexPX_S1.dll
  BonDriver_PlexPX_S1.ini
  BonDriver_PlexPX_S2.dll
  BonDriver_PlexPX_S2.ini
  BonDriver_PlexPX_S3.dll
  BonDriver_PlexPX_S3.ini
  BonDriver_PlexPX_T0.dll
  BonDriver_PlexPX_T0.ini
  BonDriver_PlexPX_T1.dll
  BonDriver_PlexPX_T1.ini
  BonDriver_PlexPX_T2.dll
  BonDriver_PlexPX_T2.ini
  BonDriver_PlexPX_T3.dll
  BonDriver_PlexPX_T3.ini
  PlexPX.dll

【サポートとか】
・最新バージョンとソースファイルの配布場所
https://github.com/radi-sh/BDASpecial-PlexPX/releases

・不具合報告等
専用のサポート場所はありません。
5chの映像制作板の該当スレを探して書込むとそのうち何か反応があるかもしれません。
作者は多忙を言い訳にあまりスレを見ていない傾向に有りますがご容赦ください。

【免責事項】
BDASpecialプラグインおよびBonDriver_BDAや付属するもの、ドキュメントの記載事項などに起因して発生する損害事項等の責任はすべて使用者に依存し、作者・関係者は一切の責任を負いません。

【謝辞みたいなの】
・このBDASpecialプラグインは「Bon_SPHD_BDA_PATCH_2」を基に改変したものです。
・AES暗号化に関する処理は、「Advanced Embedded Solutions SAGL」の著作物であるlibObfuscate(http://www.embeddedsw.net/libobfuscate.html)に含まれるRijndael.cppを基に改変されたライブラリを静的リンクで使用しています。
　ライブラリのソースコードは、下記より入手可能です。
　https://github.com/radi-sh/Rijndael4plex
・Plex PXシリーズのオリジナル処理周りのほとんどは旧2chのBonDriver共有ツール総合スレの◆SALrG1ld3mTc様の解析結果書込みを使用させていただきました。
・また、LGPL 3.0であるlibObfuscateの一部コードを含むと思われる、BonDriver_PX_W3PE_S.dll/BonDriver_PX_W3PE_T.dll/Interface_W3PE.dll/CardReaderPX.dllをリバースエンジニアリングさせていただきました。
・上記すべての作者様、その他参考にさせていただいたDTV関係の作者様、ご助言いただいた方、不具合報告・使用レポートをいただいた方、全ての使用していただいた方々に深く感謝いたします。


= 【スロウスタート】花名「輻輳制御？」
@<author>{おたく風邪に罹患した, lrks;twitter:__lrks__}
//profile{
一之瀬花名さんと百地たまてさんのアイコンを描いたら、極めてなにか作品に対する侮辱を感じさせる出来になったので文字になりました。情けない。申し訳ない。
//}
//lead{
この物語は「スロウスタート」（作・篤見唯子さん）の登場人物「一之瀬花名」さんと「百地たまて」さんが、レイヤ4輻輳制御の「スロースタート」と具体的なアルゴリズムについて述べるものです。
過度な期待はしないでください。
あと、登場人物の会話におけるアイコンは文字のみに見えるかも知れませんが、そのときは一之瀬花名さんと百地たまてさんのことを思い浮かべて心の目で見やがってください。
//}

== はじめにのはじめに
//chat[たまて]{
ちょっと聞いてくださいよ花名ちゃん！
先日ビッグサイトに行ったのですが、スマホの通信速度が遅くて大変だったんです！
//}
//rchat[花名]{
それは災難だったね……たまちゃんのギガが足りなかったのかな？
//}
//chat[たまて]{
ギガはマシマシでした！
これはもしかすると、@<b>{レイヤ4輻輳制御}が原因かもですね〜
//}
//rchat[花名]{
レイヤ4輻輳制御？
//}

マジレスすると、通信速度が遅い！という時点で輻輳制御の影響を疑うのは早計、そして今回の場合は輻輳制御はおそらく関係ないと思います。ただ本稿は「テーマがさき　背景はあと」です。

さて、花名ちゃんが気になっているので、本稿では「レイヤ4輻輳制御」について述べていきます。
レイヤ4輻輳制御？とは何でしょう？
@<b>{レイヤ4}で@<b>{輻輳}を制御するみたいですが？

本稿でいうレイヤ4は、OSIモデルにおけるトランスポート層のこと、つまりTCP@<fn>{tcp}@<fn>{pacing}やUDPなどのことです。
輻輳とは？英語だと``Congestion''、また広辞苑によると「@<ruby>{輻,や}@<fn>{spoke}が@<ruby>{轂,こしき}@<fn>{hub}にあつまる意。方々から集まること。物が1カ所にこみあうこと。」だそうです@<fn>{soudayo}。
コンピュータネットワークに限らず、人や物が集まっているだけで「輻輳」といえます。
コンピュータネットワークの場合もレイヤに関係なく、回線交換方式の通信で空き回線が無かったら輻輳、パケット交換方式でも無線通信で空きチャネルが無くなると輻輳です。
レイヤ2~3あたりで通信経路の帯域を使いつぶす、すなわちパケットがネットワーク機器のバッファに収まりきらず破棄されるような状況も輻輳でしょう。
SYN Floodでサーバのコネクションやファイルディスクリプタを食いつぶす、またはサーバに負荷をかけるようなリクエストを送ってリソースを食いつぶし、新しいコネクションを受け入れられないようにしても輻輳なはずです。
//footnote[tcp][経皮ペーシング（Transcutaneous Pacing）のことで……はなく、もちろんTransmission Control Protocolのことです。]
//footnote[pacing][経皮ペーシングとは？不整脈で脈拍が遅くなっている際、胸部に電極を貼り電気的刺激によって脈を作り出すことのようです。もしされる機会があれば、これがTCP？ヘッダの構造は？URGって何の略？ってマウント取りたいですね。]
//footnote[spoke][車輪中央から放射状に伸びる部分、スポークのことだそうです。]
//footnote[hub][車輪中央のハブのことだそうです。用例:「@<ruby>{輻,や}W L2スイッチは全ポートミラーに設定してもL1の@<ruby>{轂,ハブ}でわない」]
//footnote[soudayo][「輳」は訓読みすると「@<ruby>{輳,あつ}まる」になるそうです。まんまやんけ！]

輻輳を回避するにはどうしましょう？
@<ruby>{輻,や}が@<ruby>{轂,こしき}に@<ruby>{輳,あつ}まりすぎないようにすれば良さそうです。
コンピュータネットワークにおいては、データ送信を（自発的に）控える、または（強制的に）控えさせる制御を行うことになります。
これをレイヤ4の技術で実現@<fn>{layer4}するのが、（少なくとも本稿においては）「レイヤ4輻輳制御」です。
//footnote[layer4][レイヤ4で制御するというだけなので、制御に使う情報は他のレイヤから持ってくることもあります。Webアプリケーションから接続元のTCPポート番号やIPアドレスが取れるのと同じようなものですよ。]

本稿は主にこれを扱うため、以下から単に「輻輳制御」や``Congestion Control''と述べた場合は、「レイヤ4輻輳制御」のことを指すものとします。
なお、ここまで「レイヤ4輻輳制御」と述べていましたが、「TCP輻輳制御」と読みかえてもらっても構いません。
わざわざ「レイヤ4」と付けていたのは、ここで紹介するアルゴリズムがTCPに限定したものではなく、UDPでプロトタイプとして実装@<fn>{udt}する場合やQUICでも使えるからです。
//footnote[udt][UDT (UDP-based Data Transfer Protocol)のことを思い浮かべながら書きました。]

上記のように書くと、輻輳制御は特殊なもので自分には関係ないと感じられるかも知れません。
しかし、TCPでデータを送ろうとする場合、まず間違いなく@<fn>{1970s}輻輳制御が働きます。
もしかして、自分だけ輻輳制御を省いたTCP/IPスタックを使えば、データ送信にかかる時間を減らせるのでは？
確かにそれはそうです。
周りが気を遣ってデータ送信を控えてくれるので、輻輳の可能性も低くなります。
送るデータ量が少なければ輻輳制御があってもなくても大して変わらないので、データ量が多いときほど効きそうですね。
ただ、周りが同じことをやりだすと輻輳の可能性が高まり、しまいには誰も通信できなくなります。
ここでいう@<bou>{周り}には、自分が生み出した他のフローも含まれるため、結局は自分も損するだけ、自滅して終わりです。
インターネットとかいうスラム街でもみんな真面目に輻輳制御しているのには、こうした理由があると思っています。
ということで、輻輳制御は必要不可欠です。
//footnote[1970s][TCP/IPプロトコルスタックをなんとなく実装した場合や、1970年代からタイムスリップしてきた場合はこの限りではありません。]

ところで、似た言葉に「フロー制御」があります。
輻輳制御との違いは？本名は？年収は？彼女はいる？調べてみました！
輻輳制御入門こと「TCP技術入門」@<fn>{annaga19}によれば、「受信側が許容可能なデータ量を送信側に通知しながら送信データ量を調整する」ことがフロー制御だと書かれています。一方で、輻輳制御ではネットワーク全体のことを考えて、輻輳しないよう制御する旨も書かれています。
実際、RS-232Cでも「フロー制御」と呼ばれているのは、RTS (Request To Send)とCTS (Clear To Send)信号線を用いて、相手が受信可能か確認しながら送ることを指す、ネットワーク全体のことは（1対1のため）考えないのでそれっぽいです。
しかし、PFC (Priority-based Flow Control)やpFabric@<fn>{alizadeh13}によると、ネットワーク全体、他のフローのことも考慮して全体を@<bou>{良くする}@<fn>{good}意味で``Flow Control''と使われているように思えます。
//footnote[annaga19][安永遼真, 中山悠, 丸田一輝，``TCP技術入門''，技術評論社，2019．]
//footnote[alizadeh13][Mohammad Alizadeh, et al.，``pFabric: Minimal near-optimal datacenter transport''，ACM SIGCOMM，2013．]
//footnote[good][特定フローのFCT (Flow Complete Time)を調整したり、全体の平均FCTを削減したり。]

PFCやpFabricはレイヤ2~3で行うので、「レイヤ4輻輳制御」だけ見るとレイヤが違うと言えそうです。
ただ、ネタバレで詳細は後述しますがレイヤ4輻輳制御のアルゴリズムと主張していても、実はレイヤ3で制御@<fn>{layer3}を行うものがあります。
メインはレイヤ4なので、多少は気にするなと言われればそれまでですが「輻輳制御とフロー制御はレイヤが違う」と言い切るのは難しい気がします。
それに、レイヤ4でない輻輳制御@<fn>{call}で、たまたまフロー制御のレイヤと被るときはどう区別したら良いのでしょうか。
その他の解釈としては、フロー制御は受信側主導（送信側は受動的）・明示的な情報が取れる、輻輳制御は送信側主導（能動的）・明示的な情報はないので推定する、といったことが考えられます。
しかし、輻輳制御としつつも``Receiver-side congestion control''と述べた文献@<fn>{kumar19}があったり、詳細は後述しますがINTで明示的な情報を取得して制御したりするアルゴリズムがあります。
//footnote[layer3][IPヘッダのECNを利用するアルゴリズムのことです。]
//footnote[call][例えば災害時に通話を制限する「発呼制限」は輻輳制御にあたると思います。]
//footnote[kumar19][Praveen Kumar, et al.，``PicNIC: Predictable Virtualized NIC''，ACM SIGCOMM，2019．]

以上の結論として、輻輳制御とフロー制御の違いですが、よく分かりませんでした！
様々な解釈があり明確に区別できない、または区別しなくとも問題ない、ということなのかも知れませんね。
いかがでしたか？


== ふくそうのせいぎょ
//chat[花名]{
結局、輻輳制御って目的は何だろう……何をどうするんだろう……？
//}
//rchat[たまて]{
一般的には、回線の帯域を他のフローと分け合いながら、目一杯使うことが目的ですね！
最近はレイテンシを下げることも注目されているようです。
この目的を達成するため、特にTCPで考えると輻輳ウィンドウ（cwnd: Congestion Window）を増減させます！
//}
//chat[花名]{
そのcwndってTCPヘッダにある「ウィンドウサイズ」のことかな？
確か、ACKを待たずに送れるデータ量のことだったと思うけど……。
あとそれに対応するACKが返ってきていないデータは@<b>{In-flight}って言った気がするけど……。
//}
//rchat[たまて]{
確かに「ウィンドウサイズ」はその通りですね。
ただ、ここではTCPヘッダの中の「ウィンドウサイズ」は、受信ウィンドウ（rwnd: Receive Window）のこととします。自分が受信できるバイト（オクテット）数を相手に通知するわけです。
相手、すなわちデータ送信側では、cwndとrwndのうち小さいほうを本当の「ウィンドウサイズ」として扱うんですよ！
これはRFC 5681の動作です！
//}
以上の会話を盗み聞きした結果、rwndは相手から明示的に与えられることが分かりました。
では、cwndはどう知るのでしょうか……？初期値くらいは決められそうですが、どれぐらい増減させるかは分からないのでは…？

cwndの値は@<bou>{ほとんどの場合}、探索によって推定します。
データ送信側で、これ以上送ったら輻輳するな……と思ったら減らす、まだ送れそうだな……と思ったら増やすわけです。
とはいえ、個人がインターネットに繋げられるような回線でもネットワーク特性は大きく異なる、たとえば帯域幅が100 kbpsから10 Gbpsまであったり、通信相手の距離や有線・無線でレイテンシそのものやジッタに大きな差があったりする中で推定……？無茶では……？

実際無茶だと思いますが、それでも古くからあるアルゴリズムや、柔軟に対応すると主張する「汎用アルゴリズム」ではやっているみたいです。
具体的にはNewReno@<fn>{rfc2582}、CUBIC@<fn>{ha08}、BBR@<fn>{cardwell16}、Copa@<fn>{arun18}などのことですね。
また、条件を限定すれば正確な推定ができるようになってパフォーマンスが上がるのでは？ということで、特定用途向けのアルゴリズムもあります。
モバイルネットワークを想定したSprout、DC内のネットワークを想定したDCTCP@<fn>{alizadeh10}や、その中でもRoCE v2を想定したDCQCN@<fn>{zhu15}やHPCC@<fn>{li19}のことです。
一部（出典を載せたアルゴリズム）は、次項で紹介します。
//footnote[rfc2582][Sally Floyd, et al.，``The NewReno Modification to TCP's Fast Recovery Algorithm''，RFC 2582，1999．]
//footnote[ha08][Sangtae Ha, et al.，``CUBIC: A New TCP-Friendly High-Speed TCP Variant''，ACM SIGOPS，2008．]
//footnote[cardwell16][Neal Cardwell, et al.，``BBR: Congestion-Based Congestion Control''，ACM Queue，2016．]
//footnote[arun18][Venkat Arun, et al.，``Copa: Practical Delay-Based Congestion Control for the Internet''， 15th USENIX Symposium on Networked Systems Design and Implementation (NSDI 18)，2018．]
//footnote[alizadeh10][Mohammad Alizadeh, et al.，``DCTCP: Efficient Packet Transport for the Commoditized Data Center''，ACM SIGCOMM，2010．]
//footnote[zhu15][Yibo Zhu, et al.，``Congestion Control for Large-Scale RDMA Deployments''，ACM SIGCOMM，2015．]
//footnote[li19][Yuliang Li, et al.，``HPCC: High Precision Congestion Control''，ACM SIGCOMM，2019．]

//chat[たまて]{
余談ですが、花名ちゃんがキャリアか何かのオペレータになったとして、顧客の送信レートを抑えたい、でも顧客の輻輳制御アルゴリズムがあまり信用できない場合はどうしますか？
//}
//rchat[花名]{
私だったら、送信レートがある閾値を超えたらパケロスするようにしちゃうかなぁ……。
//}
//chat[たまて]{
キャリア側で管理される経路上の機器、いわゆるMiddleboxでrwndを上書きして小さくするという手もありますよ！
TCPじゃないとダメですが、おそらく最終的な「ウィンドウサイズ」が小さくなるはずです。
//}
//rchat[花名]{
TCPはEnd-to-Endのプロトコルのはず……。
それどころか、ネットワーク機器がトランスポート層を読み書きするなんて……。
そういえば、NAPTもそんな感じだね……ALG (Application Layer Gateway)はTCPのペイロードまで書き換えるし……。
レイヤとは一体……？
//}

ところで、cwndの推定はどのタイミングで行うのでしょう？
最低限、パケットを送る前に推定が終わっていれば良さそうです。
ただ、まさにパケットを送る瞬間に推定するのは余計な時間がかかるため、ACKが返ってきたときやパケロスしたときなど重要なイベントの発生を契機にします。
Linuxのカーネルモジュールとして実装する場合、@<tt>{tcp_congestion_ops}構造体のメンバとしてcwndを調整する関数を入れておけば適切なタイミング@<fn>{yonch}で呼び出してくれます。
CCP (Congestion Control Plane)@<fn>{ccp}@<fn>{narayan18}という仕組みを使えば、イベントを間引くことでcwnd計算の負荷を下げるような工夫も可能です。
ちなみにCCPですが、概要図は@<img>{ccp}のとおりで、``Datapath''と``Algorithms''が分離されています。
//image[ccp][CCPの概要図（論文から引用）][scale=0.9]
//footnote[yonch][具体的なタイミングはこの資料が参考になります。@<href>{http://www.yonch.com/tech/linux-tcp-congestion-control-internals}]
//footnote[ccp][@<href>{https://ccp-project.github.io/}]
//footnote[narayan18][Akshay Narayan, et al.，``Restructuring Endpoint Congestion Control''，ACM SIGCOMM，2018．]

ここで``Datapath''とは、Linuxカーネル@<fn>{ccp-kernel}やmTCP、QUICのこと、``Algorithms''はユーザランドで実装された輻輳制御アルゴリズムのことを指します。
こうすることで、アルゴリズムが30億のデバイスで動きそうな``Write-Once, Run-Anywhere''になり、データパスの制約によらず複雑なアルゴリズムが実現可能になるらしいです。
さらに、アルゴリズムをマシンの外に出すこともできるので、複数マシンが協調して動くような輻輳制御を実現できるかもといった旨も述べられています。
自律制御から集中制御……OpenFlowや冒頭で述べたpFabricを彷彿とさせますね……。
夢が広がります。
少し前に述べたCopaやNimbus@<fn>{goyal18}など、最近提案されるアルゴリズムはCCPで実装されることがあり、今後もこの傾向が続ければ、新しいアルゴリズムを実機でいち早く試しやすくなるかと思います。
//footnote[ccp-kernel][正確には@<tt>{ccp-kernel}というカーネルモジュールを介します。]
//footnote[goyal18][Prateesh Goyal, et al.，``Elasticity Detection: A Building Block for Internet Congestion Control''，arXiv:1802.08730，2018．]

//chat[花名]{
これでcwndの推定も済んだし、あとは「ウィンドウサイズ」に達するまでデータを送るだけだね。
心なしか大きい気がするけどいいのかな？
確かに現時点のネットワークは輻輳していないけど……。
//}
//rchat[たまて]{
最初から送信レートをMAX（ワイヤレート）とすると、輻輳するかも知れませんね。
ウィンドウサイズが大きいと一気にデータを送るので、それだけで輻輳の可能性があります。
さらに、ネットワークにいる他のホストも同じことを考えているかも知れないので、もし送信タイミングが重なると輻輳の可能性が上がります。
そこで@<bou>{だいたいの}輻輳制御アルゴリズムでは、最初はcwndの値を控えめにして、徐々にcwndを上げていきます。
この考え方は、@<b>{スロースタート}（Slow Start）と呼ばれますね。
//}
//chat[花名]{
@<b>{スロウスタート}（Slow Start）……！？
cwndの上げ方も工夫できそうだし、輻輳が起こってcwndを下げるときにどれぐらい下げるかも検討の余地がありそうだね……。
//}
@<img>{cwnd}にcwnd変化のイメージを、ついでに@<img>{sake}に飲酒で例えたイメージを示します。
//subfigw[cwndと飲酒量変化のイメージ]{
//image[cwnd][cwnd][scale=0.45]
//image[sake][飲酒量][scale=0.45]
//}

//rchat[たまて]{
ちなみに、アプリケーションによってはネットワーク内のホストが特定の宛先に向かって一斉に通信を開始する場合があります。
この場合、いくらスロースタートしてもどこかのネットワーク機器機器のバッファを埋める可能性が高く、多数のパケロスが発生、その後送信側がパケロスを検知して再送、宛先に届くまで時間を要することに繋がります。
これは@<b>{TCP incast}と呼ばれる問題で、解決するには「どうにか送信タイミングをずらす」「ネットワーク機器のバッファを増やす」「再送検知のしきい値（再送タイマ）を下げる」などありますが解決の難しい問題ですね……。
//}
//chat[花名]{
TCP @<ruby>{inca,インキャ}st……？
なんだかとても悲しい気分になるような……人の痛みがわかる子になりなさい……って言いたくなるような単語だね……。
//}

== アルゴリズムのしょうかい
//chat[花名]{
もっと知りたいな輻輳制御のこと……。
それでその……輻輳制御アルゴリズム？にはどんなのがあるのかな……？
//}
//rchat[たまて]{
CUBICとかDCTCPとかが有名ですね。以下から特徴的なアルゴリズムを述べていきますー！
//}

=== Reno・NewReno
百地さんの代わりに私のほうで述べていきます。
まずは@<b>{Reno}です。
出典については自信ありませんが、Linuxカーネルの@<tt>{tcp_reno_cong_avoid()}にあるコメントを読む限り、RenoはVan Jacobson氏のアルゴリズム@<fn>{jacobson88}のことを指しており、これを基にして4.3BSD-Reno（1990年）へ実装されたことで、後にRenoと呼ばれるようになったみたいです@<fn>{tahoe}。
マイナーアップデート版の@<b>{NewReno}のことを単に「Reno」と呼ぶこともあります。
//footnote[jacobson88][Van Jacobson, Michael J.Karels，``Congestion avoidance and control''，ACM SIGCOMM，1988．]
//footnote[tahoe][こちらはもっと出典に自信がありませんが、同じVan Jacobson氏のアルゴリズムを基にした輻輳制御は4.3BSD-Tahoe（1988年）にも（リリースノートを見る限りは）実装され、後に@<b>{Tahoe}と呼ばれているようです。ちなみにRenoはネバダ州の都市、Tahoeはリノ近くにある湖の名前です。]

RenoやNewRenoの特徴は、古くから実装されていることもあってか、広く普及していることです。
Linuxではカーネルに直接実装され、カーネルモジュールのロードを必要とせず「常に利用可能」@<fn>{man}とされるほか、mTCPなどのユーザランドTCP/IPスタックや、マイコンでも扱えるようなTCP/IPスタック@<fn>{lwip}@<fn>{w5100}でも利用できます。
//footnote[man][``@<tt>{man 7 tcp}'']
//footnote[lwip][STM32やESP32などで用いられるlwIPは、コードを見る限りRenoまたはNewRenoのような気がします。]
//footnote[w5100][W5100などWIZnetのハードウェアTCP/IPコントローラでは？分かりませんでした……。]

RenoやNewRenoの挙動はシンプルで、@<img>{reno}のように最初は指数関数的にcwndを伸ばす、輻輳を検知するとその時点のcwndを半分@<fn>{ssthresh}にして線形に伸ばしていく……ということを繰り返します。
//image[reno][RenoやNewRenoにおけるcwndの変化][scale=0.8]
//footnote[ssthresh][この値は@<b>{ssthresh} (Slow Start Threshold)と呼ばれますが、本稿では割愛します。]

ここで、簡単に「輻輳を検知する」と述べましたが、どのように検知するのでしょうか？
RenoやNewRenoでは、パケロスが起こると輻輳として扱います。
このような考え方は@<b>{Loss-Based}と呼ばれますが……ロスを基準にする……？
確かにデータを送り続けるとネットワーク機器のバッファが埋まっていき、やがて収まりきらなくなったパケットが破棄されます。
ただ、パケロスする理由としては、簡単に思いつくだけでもノイズ（無線区間の電波干渉、ケーブル不良、接点不良）やハードウェア障害が挙げられます。
これらは考慮しないのでしょうか？
ジッサイそのとおりで、考慮しません。
1%ロスするようなリンクで使用した場合、cwndが過剰に下げられスループットが大幅に低下しますが、これはLoss-Basedの宿命です。
//chat[花名]{
Reno・NewRenoは広く普及している、Loss-Basedなアルゴリズムなんだね。
//}

=== CUBIC
次は@<b>{CUBIC}です。
RenoやNewRenoほどではありませんが、Linuxカーネルでは2.6.19（2006年）からデフォルトの輻輳制御アルゴリズムとして用いられている@<fn>{bic}ほか、カーネルのTCP/IPプロトコルスタックを使わないようなアプリケーション@<fn>{bypass}でも用いられ、ほどほどに使われている、また最近の輻輳制御アルゴリズムと比較する際のベースラインにもなっています。
//footnote[bic][2.6.8（2004年）の時点ではBICがデフォルトのため、「2.6.8からBIC系がデフォルトで……」と述べるべきかも知れません。]
//footnote[bypass][gVisorやFD.io/VPPのことです。この2つはNewRenoに対応するほか、特にVPPでは後述するBBRもサポートしています。（要るんですかね？）]

CUBICは、cwndをもっと早く上げたいということで提案されたBIC@<fn>{xu04}のcwndの変化（@<img>{bic}）を、次の3次関数@<fn>{cubic}で近似（@<img>{cubic}）したアルゴリズムです。
//texequation{
\mathrm{W}(t) = C\left(t-\sqrt[3]{\frac{W_{max}\beta}{C}}\right)^3 + W_{max}
//}
ここで@<m>{t}は輻輳を検知してからの経過時間、@<m>{W_{max}}は輻輳を検知した時点のcwnd、@<m>{C}はcwndの増加割合を決定するパラメータ、@<m>{\beta}はcwndの減少割合を決定するパラメータです。
通信の中でcwndは、この3次関数が連なるように変化していきます。
実は、以前@<img>{cwnd}として示したcwnd変化のイメージは、CUBICを思い浮かべながら作図していました。
//subfig[BICとCUBICにおけるcwndの変化（CUBICの論文より引用）]{
//image[bic][BIC][scale=0.49]
//image[cubic][CUBIC][scale=0.49]
//}
//footnote[cubic][だから``CUBIC''です。BICとCUBICの著者は3人中2人被っていますが、これを見越して``BIC''と名付けたとかないですか？]
//footnote[xu04][Lisong Xu, et al.，``Binary Increase Congestion Control (BIC) for Fast Long-Distance Networks''，IEEE INFOCOM，2004．]

ところで、3次関数に立方根……？
Linuxカーネルでは浮動小数点数の演算はしない@<fn>{fpu}はず……無理では？
無理というのは嘘吐きの言葉、CUBICの実装ではなんとか整数演算になるよう頑張ってcwndを計算します。
立方根の計算は、ルックアップテーブルとNewton-Raphson法で行います。
//footnote[fpu][1.5TBメモリが載るあのマシンならFPUを積んでいるから演算してヨシ！とするのは自由ですが、その場合ユーザランドのプログラムが使っていたFPUのレジスタを破壊する恐れがあります。]
//chat[たまて]{
CUBICはLinuxのデフォルトでそこそこ使われているLoss-Basedなアルゴリズム、実装では数値計算を頑張っているんですねー
//}

=== BBR
そして@<b>{BBR}です。
QUICの基本的な輻輳制御アルゴリズムとして用いられている@<fn>{chromium}ほか、Linuxカーネルでも4.9以降であれば利用できることもあり、最近注目されています。
無印の「BBR」（または「BBR v1」）と「BBR v2」に分かれますが、ここでは前者について述べます。
//footnote[chromium][いつから使われているのかは調べきれませんでしたが、2014年4月の時点でBBRのためにChromiumのコードを変更することがあったようです。@<href>{https://codereview.chromium.org/248213003/}]

これまでに述べたRenoやCUBICといったアルゴリズムでは、パケロスを基準として輻輳を検知していました。
ただ、パケロスに至るまでにはネットワーク機器のバッファにキュー、RTTが伸びている@<fn>{bufferbloat}はずです。
@<img>{bbr}がcwndに対するレイテンシと転送レートの関係で、レイテンシの観点からはキューイングされなければ最小値@<b>{RTprop}@<fn>{rtprop}を維持できる、転送レートの観点からはどんどん送れば最大値@<b>{BtlBw}@<fn>{btlbw}を達成できることになります。
ちょうどBDP@<fn>{bdp}のところだと都合が良さそう、ということでBBRではRTTと転送レートを観測@<fn>{delay-based}@<fn>{vegas}しRTpropとBltBwを推定してcwndをBDPと等しくなるように調整しています。
//image[bbr][cwndに対するレイテンシ転送レートの関係（BBRの資料より引用）。横軸はcwndとほぼ同じ意味、縦軸は上図と下図でそれぞれ独立している。][scale=0.8]
//footnote[bufferbloat][このような状況は@<b>{Bufferbloat}（バッファむくみ）と呼ばれます。]
//footnote[rtprop][Round-Trip Propagation Time（往復伝搬遅延時間）のことで、定義は「@<m>{\mathrm{RTT}-(\mathrm{機器内部の遅延時間})}」です。]
//footnote[btlbw][Bottleneck Bandwidthのことで、リンクの中で一番細い帯域幅を指します。]
//footnote[bdp][Bandwidth-Delay Product（帯域幅遅延積）のことで、「@<m>{\mathrm{RTprop} \times \mathrm{BtlBw}}」です。]
//footnote[delay-based][RTTを観測するということで@<b>{Delay-Based}と呼ばれることもありますが、違和感があるので@<b>{Delay-Sensing}くらいで勘弁してほしいです。QUICの論文（Adam Langley, et al.，``The QUIC Transport Protocol: Design and Internet-Scale Deployment''，ACM SIGCOMM，2017．）でもそう書かれていますし。]
//footnote[vegas][「BBRはこの世で初めてRTTを基準にする」……ような記述は違和感を覚えるどころか嘘なので勘弁してほしいです。]

これでレイテンシが低くなってヨシ！
パケロスで輻輳を検知しないからロスの多いリンクでもスループットへの影響が少ないヨシ！
……だと良かったのですが、BBRにはいくつか課題点が指摘されています。
たとえば、レイテンシがさほど低くない、複雑なネットワークではRTpropとBltBWの推定がうまくいかないのではという懸念、また詳細は後述しますが@<b>{公平性}が不十分なために他のフローから帯域を奪ってスループットを向上しているなど……です@<fn>{dis}。
//footnote[dis][本稿執筆の目的はBBRをdisることにあったのでもっとけなしたかったのですが、自分で検証したわけではないのでやめておきます。一応ここで挙げた課題のソースはCopaとPCC Vivaceの論文、BBR v2の資料です。]

変わったところではVMで利用すると、VMExitが増えるということもあるようです@<fn>{pi19}。
タイマ割り込み（@<tt>{TSC_DEADLINE}）をさせるためMSRへ書き込む（wrmsr）ことが原因とされています。
そういえば、@<tt>{tcp_bbr.c}には「キューイングにfqを指定しないと高解像度タイマーを利用する」旨が書かれていました。
この影響や、kvm-clockなど特定のクロックソースが有効か否かにもよりそうですが、少し影響が心配です。
//footnote[pi19][Zhenwei Pi, et al.，``How KVM-based Hybrid Deployment Powers Bytedance’s Biggest Day Ever''，KVM Forum 2019．（@<href>{https://cloud.tencent.com/developer/article/1537481}）]

BBRは、GCPやAWSなど大規模な利用が目立つためかメリットばかりが取り上げられているように見えるのですが、利用は慎重にしたいと思います@<fn>{other}。
現在開発中のBBR v2では、先に挙げた課題に対する検討がされているため、これがLinuxカーネルのメインラインに取り込まれるまで待っても良いのではという気がします。
//chat[花名]{
BBRはDelay-Sensingなアルゴリズムで、改良版のBBR v2が開発中かぁ……。
//}
//footnote[other][もちろん、BBRに限らず他の輻輳制御についても同様にと思います。]

=== Copa
次は@<b>{Copa}です。
BBRと同様に、スループット向上とレイテンシ削減を同時に達成することを目標にします。
そのため下記のような目的関数を定めて、この値が最大となるような送信レートを最適化によって求め……ってそれくらいなら前にもありそうじゃないですか？
//texequation{
\mathrm{U} = \log{(送信レート)} - \delta\log{(\mathrm{RTT} - \mathrm{RTTprop})}
//}

ここで注目すべきは@<m>{\delta}です。
これはレイテンシをどれぐらい重視するかの重みで、通常は固定値とします。
しかし、RTTの変動などでLoss-Basedなアルゴリズムと同じリンクを共有していることを検知した場合は変動させます。
なぜこのようなことをするのでしょうか？

これは、@<b>{公平性}の配慮を理由に行われます。
あるリンクを複数のフローが共有する場合、そのリンクのリソースは各フローへ均等に割り当てられるべきです。
たとえば、@<img>{temari}のように「インターネット無料」をうたう軽井沢あたりのアパートからインターネットへ4つのフローが存在する場合、ネットワーク機器のリソースは25%ずつ、すなわち帯域幅が25%ずつ、レイテンシは等しくなることが期待されます@<fn>{hiroe}@<fn>{1flow}。
//image[temari][あるアパートからインターネットへ向かうフローを表した図][scale=0.8]
//footnote[hiroe][特定の階や住民だけリソースが偏っていないか？というのはもっともですが、TCPから見ると均等です。部屋単位で均等にするのはネットワーク管理者が対処すべきです。]
//footnote[1flow][専用線などで他のフローを排除して1つのフローだけを使うとリンクを100%使える……と良いのですが、現実にはそうならないようです。そのため、あえて複数のフローを作成して合計で100%に近づけるという方法を取ることがあります。ただ、これは専用線だけの話で、インターネットで@<bou>{発明}すると、他のフローの帯域を奪うので注意が必要です。]

Copaでは特にCUBICとの共存を意識しており、論文を見る限りではうまくいっているようです。
スループットはBBRと同等で、レイテンシは他のアルゴリズムと比較しても優れているとの主張もあり、良いことづくめですね。

ただ気がかりなのは、最適化によって送信レートを決定するという点です。
同様のことを行うアルゴリズムは多数ありますが、たとえばPCC@<fn>{dong15}@<fn>{allegro}@<fn>{dong18}ではCPU使用率が20%前後に達すると指摘されています@<fn>{leong17}。
CUBICやBBRは1%未満で、ワイヤレートとともに送信レートが上がることを考えると@<fn>{kernel}、リソースが足りるのかという懸念はあります。
//footnote[dong15][Mo Dong, et al.，``PCC: Re-architecting Congestion Control for Consistent High Performance''，12th USENIX Symposium on Networked Systems Design and Implementation (NSDI 15)，2015．]
//footnote[allegro][のちに改善版のPCC Vivaceが提案された際、以前のPCCは「PCC Allegro」と呼ばれていました。AllegroとVivaceはどちらも音符速度の表し方だそうです。ちなみに、響け！ユーフォニアム2期のEDテーマは「ヴィヴァーチェ！」です。]
//footnote[dong18][Mo Dong, et al.，``PCC Vivace: Online-Learning Congestion Control''，15th USENIX Symposium on Networked Systems Design and Implementation (NSDI 18)，2018．]
//footnote[leong17][Wai Kay Leong, et al.，``TCP Congestion Control Beyond Bandwidth-Delay Product for Mobile Cellular Networks''，ACM CoNEXT，2017．]
//footnote[kernel][上げ過ぎたら、今後はカーネルのTCP/IPプロトコルスタックで処理しきれるかのほうが心配になりますが。]

PCCが特殊なだけかも知れませんが、今後アルゴリズムの評価を行う際はレイテンシやスループット、公平性などネットワークに関するパフォーマンスはもちろん、ホストの計算リソースについても確認しようと思います。
//chat[たまて]{
Copaはスループット・レイテンシ・公平性に優れている、ただ計算機のリソースを消費しないか心配ということですねー。
//}

=== DCTCP
これまでは、インターネットで使うような「汎用アルゴリズム」の話でした。
ここでは、データセンター内の通信で使う特定用途のアルゴリズムとして@<b>{DCTCP}@<fn>{alizadeh10}について述べます。

DCTCPは、ECN（Explicit Congestion Notification・明示的輻輳通知）を基に輻輳の@<bou>{度合い}を検知し、輻輳回避に利用するアルゴリズムです。
ECNはIPヘッダのToSフィールドと、TCPヘッダの制御ビットから構成されます。
あるパケットがL3機器を通過する際、L3機器は自分のバッファがどのぐらい埋まっているか確認し、しきい値以上であればToSフィールド内のCEビットを立てます。
ホストではパケットを受け取った際にCEビットが立っていれば、それに対するACKパケットではTCPヘッダ内制御フィールドのECEを立てて輻輳があったことを通知するわけです。
ただ、ECNはあまり浸透しているわけではなく、さらに送信側ホスト・受信側ホスト・経路のネットワーク機器すべてがECNに対応している必要があります。
これは、特にインターネットを想定するとECNが使えるか否かなど分かるわけがなく、あまりあてにできません@<fn>{ecn}。
しかし、ここはデータセンター内なので使いやすい状況にあり、ECNを輻輳回避に利用できます。
//footnote[ecn][最近はインターネットでもなるべくECNを使えるようにしようという動きがあるようです。]

このECNは受け取ったら輻輳とみなしてcwndを下げる……のでしょうか？
それだと、パケロスで輻輳を検知する場合と大して変わりません。
そこで、DCTCPではECNを検出したパケットの@<bou>{割合}を考慮してcwndを決定します。
パケロスという1bitの情報量に対し、ECNの割合を使うことで輻輳検知の情報量を増やすわけです。
これだけの変更でも、たとえばNewRenoに適用するとパフォーマンスが向上するらしく、効果はあるようです。

なお、「ECNの割合を利用する」というのはアルゴリズムというよりアイデアに近いのか、BBR v2でも同じようなアプローチが取られています。
本当にインターネットでECNが利用できるのかは分かりませんが……。
そして、他にECNを利用する輻輳制御アルゴリズムとリンクを共有していた場合、そのアルゴリズムがECNに過剰反応@<fn>{soku}したら、ECNの@<bou>{割合}で判断するアルゴリズムが帯域を奪いすぎてしまう懸念があるので心配です。
//footnote[soku][たとえば、ECNを検知すると即座に輻輳と判断してcwndを落とす挙動のことです。]

ところで、輻輳を検知する情報量を増やすというアプローチをとるアルゴリズムとしては、@<b>{TIMELY}@<fn>{mittal15}も挙げられます。
名前から察するかも知れませんが、TIMELYはRTTを基に輻輳を検知します。
データセンター内だとRTTの変動も少なく、精度の良い情報が手に入りそうです。
……と思ってしまいがちですが、RTTが小さすぎるために、ソフトウェアからシステムクロックを見て計測すると無視できないレベルの誤差が生まれます。
ただ最近のNICでは、パケットにハードウェアタイムスタンプを入れて利用できる場合@<fn>{ptp}があり、これを用いれば正確なRTTが測定可能です。
ということで、TIMELYではこれを活用し、輻輳制御を行っています。
//footnote[mittal15][Radhika Mittal, et al.，``TIMELY: RTT-based Congestion Control for the Datacenter''，ACM SIGCOMM，2015．]
//footnote[ptp][ハードウェアタイムスタンプはPTP (IEEE 1588)でも利用されるため、対応NICが調べやすいです。（ほぼ対応している気もしますが。）ちなみにPTPに対応したスイッチだとRTpropの測定が可能なように見えます。輻輳制御に使えるのでは……。]

個人的には経路が変わるような場合、具体的にはClosネットワーク@<fn>{closnw}などでECMPが動いているときはどのような挙動になるのか気になるところです@<fn>{clos}。
論文中ではまさにClosネットワークで評価を行っていましたが、確かに5-tupleか何かのハッシュによって各フローで一定の経路にするなら大丈夫な気がします。
パケットごとに分散させるなんて輻輳制御どころかパケットリオーダリングでパフォーマンスが低下する懸念@<fn>{kenen}@<fn>{dixit13}から通常はやらないので問題ないのかも知れません。
//footnote[closnw][Fat Treeに代わるネットワークトポロジで最近のデータセンターで利用されています。East-West（サーバ間）の帯域を確保するため、Leaf (ToR)スイッチとSpine (CoreSW)、場合によってはもう一層をL3でメッシュ状に繋ぎます。]
//footnote[clos][そう思いはじめたら、既存の輻輳制御アルゴリズムとClosネットワークの相性が心配になってきました……。でも輻輳制御は各フロー（ソケット）ごとに行われることを考えたら、少なくとも5-tupleをキーにして分散させたときはあまり考えなくて良い気もします。]
//footnote[kenen][「パケットごとに負荷分散（Packet Spraying）によってパケットリオーダリングが発生してもパフォーマンスへの影響はさほど気にしなくても良い、分散されることによるメリットのほうが大きい」と述べた文献もあります。（RTTを重視するTIMELYでも同様と言えるかは疑問ですが……。）]
//footnote[dixit13][Advait Dixit, et al.，``On the Impact of Packet Spraying in Data Center Networks''，IEEE INFOCOM， 2013．]
//chat[花名]{
DCTCPはECNを使って輻輳検知の情報量を増やす、TIMELYはRTTで情報量を増やすんだね。
//}

=== HPCC
次は@<b>{HPCC}について述べます。
汎用アルゴリズムが「公道」の話、DCTCPやTIMELYが「私道」の話とすると、ここでは「専用軌道」@<fn>{guideway}の話です。
//footnote[guideway][鉄道ほどではなく「ガイドウェイバス」のつもりで書きました。RoCEなので。InfiniBandは鉄道で良い気がします。]

というのも、HPCCでは@<ruby>{RoCE,ロッキー} (RDMA over Converged Ethernet) v2で利用することを想定しています。
RoCE v2は、RDMA (Remote Direct Memory Access)のデータ（InfiniBandのデータです）をEthernetフレーム・IPパケット・UDPデータグラムでカプセル化する仕組みです。
アプリケーションがデータを送るとき、OSやCPUは何もせず@<fn>{driver}そのままNICに送りつけます。
NICはデータをカプセル化してファブリックに流し、それを受け取った相手のNICはカプセル化をほどいてやはりほぼそのままアプリケーションまで届けます。
NICでいろいろやるのでハードウェアタイムスタンプどころではない特殊なNICが必要になりますが、GPUクラスタや分散ストレージなどで求められるような高い性能が実現可能です。
//footnote[driver][NICを制御するためにドライバは触りますが、データには触れません。]

RoCE v2で用いられる輻輳制御アルゴリズムには、@<b>{DCQCN}があります。
DCQCNは、DCTCPと同様にECNの割合を輻輳制御に利用します。
UDPなので（TCPヘッダの）ECEは関係ありませんが、その代わりIPヘッダのECNを検知したら送信側・受信側で連携し、cwndを調整します。
そして、DCQCN、またHPCCも、スロースタートは行いません。
//chat[たまて]{
スロウスタートを……！？
//}
//rchat[花名]{
行わない……。
//}
ショートフロー@<fn>{shortflow}では、スロースタートでcwndを上げる際のオーバヘッドが大きいからです。
でも、それだと輻輳するのでは？
HPCCの論文によると、やはり送信レートによっては深刻な輻輳を引き起こすそうです……。
//footnote[shortflow][送信データ量が少ないフローのことです。]

HPCCでは、INT (In-network Telemetry)@<fn>{int}
INTという言葉自体は一般名詞のようですが、ここでは経路中のスイッチが管理するキューの長さ・リンク容量などの情報を指します。
@<img>{hpcc}のように、あるパケットがスイッチを通った際にスイッチがパケット内にINTを付けてくれるので、HPCCではこれをNICから利用します。
//image[hpcc][INTが書き込まれる様子（HPCCの論文から引用）][scale=0.8]
//footnote[int][In-band Network Telemetryとも呼ばれるようです。]

……いや、そんなINTを読み書きするスイッチやNICがあるのでしょうか？
自分たちで作ればあります、ということでHPCCの著者はP4@<fn>{p4}でスイッチを作り、FPGAの載ったNIC@<fn>{silicom}で検証、高いパフォーマンスを実現できたそうです。
真似したくても出来なそうですが、INTを利用するというアプローチが新しいなと思いました。
//footnote[p4][データプレーンのプログラミング言語です。この場合、最終的にはスイッチASICのAPIを叩くことになります。]
//footnote[silicom][スライドを見るとSilicom社の「fb2XGhh@V7」という製品に見えます。（なんなんですかね……この製品名……。）SFP+ 10GEx2でXilinx Virtex7 VX690TというFPGAが載るようです。]
//chat[たまて]{
DCQCNとHPCCはRoCE v2で使う輻輳制御アルゴリズムで、スロウスタート（Slow Start）を行わないなど独特でした。
HPCCはINTを使うのが新しいですねー。
//}

== ほんこうのまとめ
//chat[花名]{
それじゃ、まとめをするね。
本稿ではレイヤ4輻輳制御の概要を述べ、具体的なアルゴリズムとしてReno、CUBIC、BBR、Copa、DCTCP、HPCCやその周辺のアルゴリズムを紹介しました。
//}
//rchat[たまて]{
スロウスタートとの関連があまりなかったのは、いただけませんなー。
//}
//chat[花名]{
スロースタートを省くアルゴリズムも紹介しちゃったもんね……。
でも結局、どのアルゴリズムを使うと幸せになれるんだろう？
//}
//rchat[たまて]{
いくつかのアルゴリズムは「Phanteon（@<href>{https://pantheon.stanford.edu/}）」で比較ができますね。
定期的にAWSやGCP、その他ノードで測定した「スループット」と「レイテンシ」の結果を載せてくれます。
//}
//chat[花名]{
結構ばらつきがあるね……あと公平性や実装の使いやすさは分からないし……。
使いたい環境（OS）のデフォルトで良いんじゃないかな……。
//}

この半年くらい調べたこと、見聞きしたことの整理ができてよかった（小並感）。
負け惜しみやないですが、前回のコミケで出していたらまとめきれなかったので落選して勝利したのだけはハッキリしています。

輻輳制御アルゴリズムについてですが、本当はシミュレーションまでやりたかったです。
ただ、うまいシミュレーション方法を見つけられませんでした。
Linuxのネットワークスタックで動かしたいのでns-2/3は避けたい、ns-3 DCEはトラウマがあって怖いです。
実機でtcを使いながらiperfを流したり、ここで挙げた論文でよく使われていたMahimahi@<fn>{mahimahi}を使ってみたりしましたがいまいちでした。
トポロジも作ろうとしましたがうまくいかず。
公平性を見たかったので、ダンベル（トポロジに）何（ノードか100,000）キロ（bpsのノードとか）持（っ）て（きたのですがまともに繋げられている気がせずどうなってい）る？って感じです。
//chat[オタク]{
どうすりゃいいんだ……次はおまけです。
//}
//footnote[mahimahi][@<href>{http://mahimahi.mit.edu/}]


== おちぼのひろい: コミケ会場での通信を考える
本稿を執筆した理由はBBRをdisることでしたが、実はもう一つ理由があります。
それは、通信状況の不安定なコミケで安定した通信を行うことの検討です。
ただ、結論からいえば頑張れることはなく、そもそも何もしなくても良いかも知れません。

SIGCOWWでは電子決済に対応しており、決済センターと通信する必要があります。
通信が安定していないと決済が行えない可能性がある、特に会場であるビッグサイトに新設された青海展示棟や南展示棟では通信が不安定とのことでゆゆしき事態です。

まず前提としてですが、決済センターとの通信はiPadのアプリが行います。
ということで、ここをなんとかすれば良さそうです。
そこでぼくがかんがえたさいきょうの通信環境ですが、Raspberry PiをEthernetアダプタとしてiPadからの通信を吸い込み、自宅へProxy、そこから決済センターへ繋ぎます。
Raspberry Piと自宅は複数キャリアの回線をMPTCPで束ね、さらにRedundantスケジューラを使うことでロスやレイテンシを小さくします。
ただ、残念ながらSIGCOWWでは、Square Standという決済用スタンドを使っているためにLightningポートが埋まっており、Ethernetアダプタを繋ぐ（繋いだことにする）のは不可能です。
これマジ？ここではMPTCPって言いたかっただけです。
Wi-Fiならこの構成を実現できますが、コミケ会場でWi-Fiを使うほうが不安定になりそうなのでボツになりました。
iPadから繋ぐ4G回線でなんとかするしかありません。

そこで、気休めに輻輳制御アルゴリズムで頑張ることを検討します。
ただしiPadの輻輳制御アルゴリズムを変えることは無理、また（決済データだと微妙ですが）リクエストが小さくレスポンスが大きいデータの場合、クライアントの輻輳制御アルゴリズムだけを変えてもあまり意味がありません@<fn>{response}。
これまで明示的には述べていませんでしたが、輻輳制御は「送信側」または「受信側と送信側が連携」して動きます。レスポンスの受信側、すなわちクライアントの輻輳制御アルゴリズムが単体で動くことはありません。

そのため、Proxyサーバを用意し、せめて少しの区間でも任意の輻輳制御アルゴリズムを適用させることを考えました。
HTTP ProxyまたはSOCKS ProxyであればTCPコネクションが終端されるため、Proxyサーバから出るフローには任意の輻輳制御アルゴリズムが適用できるはずです。

ただ、残念ながらiOS端末では、4G回線でのProxy欄がありません。
APNプロファイルのそれらしい箇所を変えても不可、VPNの設定ではProxy欄が現れるので設定してみるも4G利用時は無視されてしまいます。
Appleが提供するiOS端末の管理ソフトウェア（Apple Configurator 2）を使ったり、VPNで透過Proxyを構成したりすれば（HTTPSが心配ですが）設定できるかも知れません。
しかし、このあたりで力尽きてしまいました。厳しいです。
ほかにはiPadでデュアルSIMを使うということも考えましたがデュアルSIMが使えるiPadはSquare Stand非対応、iPadに貼り付ける外部アンテナはオカルトっぽい……。
とうとう自営のプライベートLTE基地局を持ち込むことも考えましたがやはり厳しいです。
//footnote[response][リクエストが大きい場合は、クライアント側のアルゴリズムを変えることに大きな意味があります。大きいデータを送る側の輻輳制御アルゴリズムを変えると効くためです。それを考えるとやはりリクエストとレスポンスがともに小さそうな決済データではアルゴリズムを変えても……。]

あとは、大手キャリアの回線を用意しておき、通信が不安定になったら手動で切り替えるくらいでしょうか。
auとSoftbankの回線はApple SIMで、docomo回線は海外のプリペイドSIMかなにかで確保できそうです。
でも、できれば主系は一番繋がりやすいところが良いなと思います。

ということで、どこが繋がりやすいのか、COMITIA129開催中（2019年8月25日）の青海展示棟で調べてみました。
調査には、RTL-SDR（R820T）とLTE-Cell-Scanner@<fn>{cell}を使い、LTE基地局が広報するMIB (Master Information Block)を受信します@<fn>{sib}。
RTL-SDRなので周波数の範囲が限定的（24MHz~1766MHz）、またMIBを見ているだけなので利用者多数で輻輳している……など実際の通信状況は分かりません@<fn>{ymobile}が、ひとまず簡単にやるくらいなら十分でしょう@<fn>{taiho}。
//footnote[cell][オリジナルはEvrytania氏によるプログラムですが、ここではJiaoXianjun氏によるTD-LTE対応版の@<href>{https://github.com/JiaoXianjun/LTE-Cell-Scanner} を用いました。]
//footnote[sib][HackRF OneとMATLABがあれば、MIBよりも詳細なSIB (System Information Block)のデコードもできるようです。]
//footnote[ymobile][手持ちの回線で、送信側ホストの輻輳制御アルゴリズムを変えながらスループット測定もやってみましたが、安定しない結果になったので割愛します。]
//footnote[taiho][何も考えずMacbookにRTL-SDRを繋ぎ、トートバッグに忍ばせて徘徊していましたが完全に不審者なので、UMPCやRaspberry Piが必要だったと思います。]

調査した結果は、@<img>{cell}のはずですが……肝心な部分を黒塗りにしました。
これは、電波法59条の「何人も法律に別段の定めがある場合を除くほか、特定の相手方に対して行われる無線通信を傍受してその存在若しくは内容を漏らし、又はこれを窃用してはならない。」という部分が心配なためです。
基地局の免状を確認すると、通信の相手方には「免許人所属の陸上移動局及び陸上移動中継局」や「免許人所属の陸上移動局及び免許人と業務委託契約を締結した他の免許人所属の陸上移動局」と書かれているため、受信する資格はなく「傍受」にあたりそうです。
SIMなしの端末でETWS（緊急速報）を受信しても傍受、それをツイートしたら内容を漏らすことに該当しそうで、まず検挙されるわけがないとは思いますが……。
//image[cell][青海展示棟におけるLTE基地局の探索結果][scale=1]

しかし、実をいうと今回の測定では特筆すべきところがなく、どうしても載せたいようなデータではありません@<fn>{omoshiroi}。
特に青海展示棟ではC96開幕時点（2019年8月9日）でauとSoftbankの屋内局が稼働していたようで、COMITIA129でも特に通信状況が気になる場面はありませんでした。
また、南展示棟でもC96においては通信状況が気になる場面は少なく、別に気にしなくて良いんじゃないですかね……という気がします。
ということで、コミケ会場での通信はこちらで頑張れることはなく、そもそも何もしなくても良いと思います。ヨシ！
//footnote[omoshiroi][ある場所で検証した際は、特殊な基地局が見えて面白かったです。どえらい結果が出たぁあああああ！！！って感じでした。]

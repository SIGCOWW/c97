= 櫻井桃華と橘ありすの香りに注目した類似度比較の検討
@<author>{私、学位は博士号でって言ったよね！, itezaP;twitter:sugerest}

//profile{
シンデレラ7thライブに、私の愛しい妻である赤城みりあちゃんが出ないので悲しいです。
//}

//lead{
アイマスとケモインフォマティクスネタ第二弾です。

需要があるかはわかりませんが、完全に個人の趣味です。
//}

== ご挨拶

こんにちは、itezaPと申します。
一年ぶりにコミケに新ネタを引っ提げて返ってきました（夏コミは…ナオキです…）。
今回はアイマスとケモインフォマティクスネタ第二弾をやりたいと思います。
一言でいうと、バラ（桃華）とイチゴ（ありす）の香りがする化合物の類似度を比較しよう、という記事です。
以前の記事でバラやイチゴの類似度をタンパク質の観点で比較したので、次は匂い（化合物）でやってみようと思い立ちました
（タンパク質での類似度比較やケモインフォネタ第一弾については、既刊の総集編をご覧ください@<fn>{sigcoww}）
（露骨な宣伝）。
本記事の内容はこの記事だけで完結しているので安心してください。

//footnote[sigcoww][@<href>{https://sigcoww.booth.pm/items/1310842}]

== はじめに

物体からは、匂いに関する化学物質（化合物）が揮発によって空気中に常に放出されています。
人はその化学物質を鼻（嗅覚器）で受け取り、その化学的な刺激によって人は匂いを感じます。
匂いに関する化学物質は主に有機化合物であり、その種類もさまざまです。
一例として、バニラ特有の甘い匂いはバニリンと呼ばれる有機化合物が重要な役割を果たしています。

匂いには、心地よい匂いや不快な匂いなど色々な匂いが存在します。
心地よい匂いや不快な匂いの感じ方には個人差がありますが、ほとんどの人の感覚は共通していると思います。
大抵の人は、バラやイチゴの匂いは心地よい匂い、タバコやアンモニアなどの匂いは不快な匂い、と感じると思います。

2つの化合物がどれくらい類似しているかを定量的に解析するのは非常に重要な課題の1つです。
有機化合物は、類似した構造ならばその性質も類似することが知られています。
化合物の性質とは水溶性や毒性、薬理作用などを指します。
匂いに関しても同様であり、基本的に類似した構造は類似した匂いであると考えられます。

バラやイチゴ…といえばもうお分かりかと思います。
そう、我らの母こと櫻井桃華ちゃんです。
さらにイチゴといえば橘ありすちゃんです。
@<img>{momoka_and_alice_kwii}に両名の写真を掲載します。
バラとイチゴは「思ったより」遺伝的に近いことがnatureにて発表されています@<fn>{nature}。
今回はそれらの匂いがする化合物の構造に注目して類似度を比較しました。

//image[momoka_and_alice_kwii][両名の写真。私のお気に入りを厳選しました。][scale=0.8]

//footnote[nature][@<href>{https://www.nature.com/articles/s41588-018-0110-3}]

== 方法

=== 類似度について

@<img>{hisakawa}にとある双子アイドルを示しますが、この二人はどれくらい似ていると言えるでしょうか。

//image[hisakawa][なーちゃんが好きです][scale=0.8]

どれくらい類似しているかを判定する方法として、外見や言動など色々な要素が考えられます。
化合物の類似度を測る方法も数多く存在しますが、より一般的な方法としては、どれだけ共通の構造を持っているかを判定します。
たとえば、炭素や酸素をどれくらい含んでいるか、ベンゼン環をいくつ含んでいるか、などです。
このように共通の要素を考えると無数の場合が考えられますが、より化合物の性質に注目した部分構造の集合が定義されています。
この集合をビット列で表現したものをフィンガープリントと呼び、定義された部分構造があれば1、無ければ0で表現できます。
化合物の特性に関わる重要な部分構造情報をビット列として表現するため、高速な類似度検索が可能であり、未知の化合物の機能推定などに役立ちます。
フィンガープリントのビット列同士の類似度（距離）計算方法としては、tanimoto係数やdice係数などが存在します。
@<img>{calc_comp_sim}に2つの化合物の類似度の簡単な計算例を示します。

//image[calc_comp_sim][フィンガープリントによる2つの化合物の類似度の比較例。a, bは化合物A, Bのそれぞれの1のビット数であり、cは共通する1のビット数。類似度計算にはtanimoto係数を使用。][scale=0.9]

フィンガープリントの定義はいくつか存在し、その解析目的に応じて使い分けられています。
今回は汎用的に用いられているMACCS fingerprintを利用します。
MACCS fingerprintは化合物の特性に基づいて166ビット（166個の部分構造）で定義されています。
ビット列の類似度の計算には、tanimoto係数を使用します。

=== データセット

データセットは、それぞれバラとイチゴの香りに含まれている含有量が多い上位6つの化合物を使用しました@<fn>{rose_dataset_ref}@<fn>{strawberry_dataset_ref}。
それらは化合物データベースであるPubChemからダウンロードしました。
以下の@<table>{dataset}に使用した化合物のID（PubChem CID）を記載します（正式名称は長く数多く存在するので省略します）。

//tabooular[dataset][使用した化合物データセットの一覧。化合物は香りの含有量の降順で表記した]{
順位	|	バラ	&	イチゴ
==========>|====&====
1位	|	3314	|	11007913
2位	|	27866	|	643941
3位	|	6054	|	12180
4位	|	8842	|	650
5位	|	6549	|	7762
6位	|	637566	|	7342
//}

//footnote[rose_dataset_ref][@<href>{https://www.jfda-online.com/article/S1021-9498(16)30036-9/fulltext}]
//footnote[strawberry_dataset_ref][@<href>{https://pubs.acs.org/doi/abs/10.1021/jf960366o}]

== 結果と考察

@<table>{dataset}で示した各化合物を類似度を総当たりで算出しました。
結果を行列形式で表したものを@<img>{result01}に示します。

//image[result01][各化合物の類似度][scale=0.9]

すべての化合物の類似度を計算した結果、全体としてそこまで高い類似度ではないことがわかりました。
類似度がもっとも高い組み合わせでも、6549と650の0.48でした。
次点で高いのは、6549と7342の0.46や6549と643941の0.42でした。
6549はどの化合物とも（相対的に）高い類似度ですね。
6549はラベンダーやベルガモットのようなフレッシュな香りが特徴的です。
たしかに二人とも若いのでフレッシュですね。

もっとも含有量が多い化合物である3314と11007913の組み合わせの類似度は、この中ではそれなりに高く0.41でした。
構造を見てみると環構造やそれに結合しているOH基等、視覚的にもなんとなく構造が似ていることが見て取れます。
@<img>{comps01}に2つの化合物の構造を示します。

//image[comps01][3314と11007913の構造][scale=0.9]

バラとイチゴの香りに一番含有量が多い化合物同士の類似度は0.41でした。
ありす=0.41桃華とすると、ありすは桃華の半分程度の母性を持っていると考えることができます。
ありすママ、ありっすね…ふふっ。
逆に考えると、桃華にも半分程度ありす要素があると捉えることもできます。
ありすの一番の要素（魅力）といえば何でしょうか。
ありすには「クールタチバナ」や「パーフェクトありす」等、さまざまな称号があります。
個人的には、ちょっとツンデレな年相応の女の子ってイメージです。
桃華にも年相応な部分がありますし、ツンデレ要素が多少含まれている可能性がありますね。
（金髪お嬢様にツンデレって、一昔前のテンプレート設定みたいですね）
（金髪ツンデレといえば、羽瀬川小鳩ちゃんが好きです）。
桃華は隠れツンデレ…いいですね、妄想が膨らみます。

バラ特有の甘い香りを出すために、27866や6054が重要な役割を果たすことが知られています。
27866はバラの甘い香り特有の化合物であるため、イチゴの香り化合物にはそれほど類似していません。
一方で、6054は比較的高い類似度ものが複数ありました。
これは6054の構造がベンゼン環、3つの炭素の鎖にOH基と比較的よく使われる基本的な部分構造を保持しているためだと考えられます。
イチゴの香り化合物の観点から見ると、12180がバラの香りと類似度が低いことがわかります。
これは12180がイチゴのような果実特有の香りであるためであると考えられます。

（考察はここで途切れている…。）

== おわりに

今回はバラとイチゴの香りがする化合物に注目した構造類似度を計算してみました。
全体の類似度としてはそこまで高くなかった一方、一部類似度が0.5程度と比較的高い組み合わせも存在しました。
バラとイチゴの香りで一番含有量が多い化合物の類似度は比較的高い0.41でした。
つまり桃華とありすは半分くらい似ている可能性がある、という結論になります（？？？）。
本来なら他の香り（ゆず、りんご等）と比較を行いたかったのですが、時間切れです
（香り成分が掲載されている論文を探すのがね…）
ゆずやりんごと比較したい理由は、みなさんお分かりかと思います。

次のネタ候補としては、代謝やタンパク質の相互作用の関係から彼女たちの類似性に迫っていきたいです（予定は未定）。
もしくは、サークル主がハマっている（？）キャラクターの掛け合いを用いた何らかの解説記事は良さそうです。
次の夏コミは五輪の関係でGW開催なので、早めにネタを考えないといけないですね（無事受かれば、の話ですが）。
それでは、次の記事でお会いしましょう。

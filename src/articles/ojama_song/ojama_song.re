= OJaMa-Song：一般男性による多様な歌唱スタイルの歌声コーパス
@<author>{VOCALOIDになりたい, Sython;twitter:SythonUK}
//profile{
D進は最高@<emoji>{heart_eyes_cat} 理想の人生です！
//}
//lead{
「私の歌を聴けぇぇぇー！」
//}

== はじめに

Sythonです。
時が経つのは早いもので、令和元年も終わりを迎えようとしています。
筆者自身も博士後期課程2年目の終わりを目前としており、来年度には博論を執筆しなければならないという現実をすべて自分のほうへねじ曲げたいと思う日々が続いております。
こういったつらい日々には、音楽を聴いて心を癒やす、あるいは歌でも歌って気分をリフレッシュしたいと思うものです。
本稿では、近年の歌声生成系の研究開発の動向についても触れつつ、筆者によってすでに構築された@<b>{「一般男性音声コーパス (OJaMa) 」}@<fn>{OJaMa}の続編である@<b>{「一般男性歌声コーパス (OJaMa-Song)」}を作ろう、という話を述べます。
//footnote[OJaMa][“O”rdinary “Ja”panese “Ma”le speakerコーパスの略です。@<href>{http://sython.org/Corpus/OJaMa}]

== 近年の歌声生成系研究開発動向

本節では、ここ数年くらいで話題になった（と思う）歌声生成系の研究開発動向を、筆者が知る範囲で簡単にまとめます。

=== 「AIシンガー」ブームの到来

機械学習技術の進展に伴い、多くの企業や大学の研究室で「AI（人工知能）シンガー」の研究・開発がされています。
学術的な話題だと、音声情報処理に関するトップカンファレンスであるInterspeech 2019@<fn>{IS2019}のBest Student Paperの中に、敵対的学習に基づくEnd-to-End歌声生成の論文@<fn>{adv_ss}がありました。
産業的な話題だと、最近は日本国内だけでも数多くのAIシンガーが登場しつつあるように思えます。
たとえば、2019年4月には、マイクロソフトにより開発されているAI「りんな」@<fn>{rnn}が「AIアーティスト」としてデビュー@<fn>{rnn_sing}したり、
名古屋工業大学とテクノスピーチが共同で開発したAI歌声合成システムを使って制作されたボーカルを起用したCDが発売@<fn>{stussr_sing}されたりしました。
前者の歌声はこちら@<fn>{rnn_youtube}で、後者の歌声はこちら@<fn>{stussr_youtube}で聴けるのですが、確かにこれはすごいなぁという感じがします。
筆者は特に声優に詳しいわけではないのですが、後者のさとうささら@<fn>{stussr}さんの歌声はもはや中の人（水瀬いのりさん）が出ているのではないかという印象です。
また、2019年9月には、ヤマハの歌声合成技術を用いて「AI美空ひばり」が実現され@<fn>{msrhbr}、新曲「あれから」@<fn>{arekara}が公開されたことも話題となりました。
故人の歌声をモチーフにしたAIシンガーという点で賛否両論あったと思われますが、筆者個人としては非常に夢のある技術だと思います。
//footnote[IS2019][@<href>{https://www.interspeech2019.org/}]
//footnote[adv_ss][論文: @<href>{https://www.isca-speech.org/archive/Interspeech_2019/abstracts/1722.html}、歌声デモ: @<href>{https://www.youtube.com/watch?v=uAAXtjKEv2E}]
//footnote[rnn][@<href>{https://www.rinna.jp/}]
//footnote[rnn_sing][@<href>{https://news.microsoft.com/ja-jp/2019/04/03/190403-rinna-as-artist/}]
//footnote[stussr_sing][@<href>{https://av.watch.impress.co.jp/docs/news/1180386.html}]
//footnote[rnn_youtube][@<href>{https://www.youtube.com/watch?v=_NPyt1AYUTg}]
//footnote[stussr_youtube][@<href>{https://www.youtube.com/watch?v=xRknlXIHkmI&feature=emb_logo}]
//footnote[stussr][@<href>{http://cevio.jp/cevio_character/}]
//footnote[msrhbr][@<href>{https://www6.nhk.or.jp/special/detail/index.html?aid=20190929}]
//footnote[arekara][@<href>{https://www.youtube.com/watch?v=nOLuI7nPQWU}]

=== 歌声コーパスの構築

前述のAIシンガーの開発には、当然ですが当該歌手の歌声データが必要です。
研究や個人の趣味等で使える歌声コーパスはなかなか構築されていないのですが、本節では、オンラインで入手可能ないくつかの歌声コーパスについてまとめます。

==== Nitech Japanese Singing Voice Database (NIT SONG070)
名古屋工業大学の研究グループを中心に開発されたHMM/DNN-based Speech Synthesis System (HTS)@<fn>{HTS}のページからダウンロードできる、日本人女性話者1名による童謡の歌唱データ@<fn>{HTS_song}です。
raw形式で保存されているので、ちょっとした工夫@<fn>{kufu}をしないとそのままでは再生できませんが、オンラインで入手できる貴重な日本語歌声データです。
歌声データだけでなく、フルコンテキストラベル@<fn>{full}も公開されています。
//footnote[HTS][@<href>{http://hts.sp.nitech.ac.jp/}]
//footnote[HTS_song][@<href>{http://hts.sp.nitech.ac.jp/archives/2.3/HTS-demo_NIT-SONG070-F001.tar.bz2}]
//footnote[kufu][たとえば、SPTKをインストールして raw2wav を使うとか？]
//footnote[full][楽譜や歌詞などの情報を表す特徴量のようなものをイメージしていただけるとよいかと思います。]

==== JSUT-Song

東京大学の猿渡・小山研究室@<fn>{sarulab}の研究グループにより構築された、日本人女性話者の歌声データセット@<fn>{jsut-song}です。
内容はNIT SONG070と同じ童謡なので、やろうと思えば歌声変換などはすぐにできると思います。
ちなみに、歌唱者は同研究室で構築された「JSUTコーパス」@<fn>{jsut}の話者と同じです。
//footnote[sarulab][@<href>{http://www.sp.ipc.i.u-tokyo.ac.jp/}]
//footnote[jsut-song][@<href>{https://sites.google.com/site/shinnosuketakamichi/publication/jsut-song}]
//footnote[jsut]["J"apanese "S"peech corpus of "S"aruwatari-lab, University of "T"okyo の略です。本当に注意していただきたいのですが、"JUST"コーパスではありません。 @<href>{https://sites.google.com/site/shinnosuketakamichi/publication/jsut?authuser=0}]

==== JVS-MuSiC

日本音響学会 2020年春季研究発表会の講演プログラム集@<fn>{asj2020s}を眺めていたら、「JVS-MuSiC: Japanese multispeaker singing-voice corpus」という題目の講演がありました。
想像ですが、猿渡・小山研究室の研究グループにより「JVSコーパス」@<fn>{jvs}という100名の日本人話者の音声コーパスが構築されているので、それの歌声版という感じでしょうか？
今後に期待ですね！
//footnote[asj2020s][@<href>{https://acoustics.jp/cms/wp_asj/wp-content/uploads/2020spring_program.pdf}]
//footnote[jvs]["J"apanese "V"ersatile "S"peech コーパス @<href>{https://sites.google.com/site/shinnosuketakamichi/research-topics/jvs_corpus?authuser=0}]

==== 東北きりたん歌唱データベース

明治大学の森勢研究室@<fn>{morise-lab}を中心に構築された、東北きりたん@<fn>{kiritan}（厳密には、中の人である茜屋日海夏さん）の歌声データ@<fn>{kiritan-sing}です。
私は声優について特に詳しいわけではないのですが、人気女性声優の歌声データということで非常に貴重だと思います。
//footnote[morise-lab][@<href>{http://www.kisc.meiji.ac.jp/~mmorise/lab/}]
//footnote[kiritan][@<href>{https://www.ah-soft.com/voiceroid/kiritan/}]
//footnote[kiritan-sing][@<href>{https://zunko.jp/kiridev/login.php}]

== OJaMa-Song

前節でさまざまな歌声コーパスを紹介しましたが、残念ながら、私の知る範囲では研究用途で使用可能な@<b>{日本人男性の歌声コーパス}は存在しません@<fn>{jvs-male}。
そこで、本稿ではいつもの「20代男性大学院生のSさん」@<fn>{Ssan}にお願いして歌声データを収録し、@<b>{OJaMa-Songコーパス}を構築しました。
歌っていただいた曲は、JSUT-Songと同じ童謡27曲（つまり、NIT SONG070とも一部重複あり）なので、これによって男女両方の童謡歌唱データがカバーできたことになります。
ただ、普通に歌ったデータを公開するだけでは面白みがないので、JVSコーパスを参考@<fn>{jvs-iroiro}にして、さまざまな歌唱スタイルの歌声を収録しました。
次節以降で歌唱スタイルの詳細を示します。
//footnote[jvs-male][JVSコーパスには男性話者もいるので、今度公開されるJVS-MuSiCには男性の歌声が含まれるのかも…？うーん、わからないですね…]
//footnote[Ssan][これまでに筆者が執筆した記事にはすべて登場しているはずです。]
//footnote[jvs-iroiro][通常の話し声だけでなく、ささやき声や裏声があります。あれ、聞き覚えがある発話スタイルですね…？（気になるひとは@<href>{https://sigcoww.booth.pm/items/1310842%7D}を買おう！）]

=== 複数テイク歌唱 (take1, take2)

同一フレーズを複数回歌唱したテイクを重ねることで、歌声に厚みを持たせる技術をdouble tracking (DT)といいます。
最近では、deep neural networkを用いて、より自然なDTっぽいものを人工的に作りだそうという手法(neural DT: NDT)@<fn>{NDT}も提案されていますが、やはりちゃんと録音された複数回歌唱データが欲しかったりもすると思います。
NDTの論文@<fn>{NDT-arxiv}では、同一曲を5回歌ったデータを使っていますが、しんどそうなので、今回は2テイクだけ録音しました。
//footnote[NDT][@<href>{https://sites.google.com/site/shinnosuketakamichi/research-topics/neural-double-tracking}]
//footnote[NDT-arxiv][@<href>{https://arxiv.org/abs/1902.03389}]

=== ささやき歌唱 (whisper)

深夜帯の集合住宅など、あまり大きな声で歌えないシチュエーションも多々あると思います。
そういった場合に、ささやき歌唱を通常の歌唱に変換し、それを歌唱者にフィードバックするような技術@<fn>{whisperVC}があったりすると、楽しく歌えてるっぽさが出せるかもしれません。
本稿では、通常の歌唱に加えて「ささやき歌唱」も収録しました。
ささやく感じでうまく歌えてるかどうかは微妙なところですが、貴重なデータだと思います。
//footnote[whisperVC][「通常の話し声からささやき声に変換する技術」については、過去の記事(@<href>{https://sigcoww.booth.pm/items/588350})で検討しました。]

=== ハミング歌唱 (humming)

歌うとまではいかなくても、上機嫌のときはハミングで歌いたくなることがあると思います。
本稿では、ハミングで歌唱したデータも収録しました。
これは、「原曲のメロディとテンポは保持しているが、歌詞が消失した歌唱データ」として解釈できます。

=== 読み上げ調歌唱 (reading)

ハミング歌唱で"with melody, but without lyric"なデータが取れると、その逆のデータも取りたくなります。
本稿では、「原曲の歌詞とテンポは保持しているが、メロディが消失した歌唱データ」として、読み上げ調の歌唱（？）データも収録しました。
イメージとしては、SofTalk（いわゆるゆっくりボイス）やGoogle翻訳さんに無理やり歌わせてる感を再現したものです。
そもそもタスクとしてめちゃくちゃ難しいのでは？という感じが否めないですが、なんとか頑張って歌ってもらいました。

=== 収録内容まとめ

ここまでの内容をまとめると、OJaMa-Songコーパスでは、童謡27曲について次の歌唱スタイルのデータを収録しています。

 * 複数テイク歌唱 (take1, take2): 同一曲を2回歌ったもの
 * ささやき歌唱 (whisper): ささやき声で歌ったもの
 * ハミング歌唱 (humming): ハミングで歌ったもの
 * 読み上げ調歌唱 (reading): 歌詞を読み上げているっぽく歌ったもの

すべてのデータは無響室で収録されており、サンプリングレートは48kHzです。
この歌声データ群が皆様のお役に立てると幸いです（立てるかなぁ…@<emoji>{thinking_face}）。

== おわりに
本稿では、近年の歌声生成系の研究開発の動向について触れつつ、著者が新たに構築した@<b>{「一般男性歌声コーパス (OJaMa-Song)」}について述べました。
本コーパスは @<href>{http://sython.org/Corpus/OJaMa-Song} で公開されています。
主に個人での研究・分析目的であれば無償で利用可能です@<fn>{shoyo}。
無断の再配布や、公序良俗に反する利用などはご遠慮ください。
気が向いたら追加で歌声を収録するかもしれないので、ご期待ください（？）
//footnote[shoyo][（いるかどうかわかりませんが）仮に商用での利用をご希望の場合は、筆者のTwitterアカウント @SythonUK まで（DMか何かで）ご一報頂ければ幸いです。]

=== 将来展望

スペースに余裕がありそうなので、このコーパスを使ってやれそうなことを書きます。

==== 各歌唱データの分析

本当は構築したコーパスの分析等をしたかったんですが、筆者のスケジュール的な余裕が皆無だったので今回は見送ることにしました…@<emoji>{crying_cat_face}
次回もし記事を執筆する機会があればやるかもしれませんが、「我こそは」という方にやっていただくのも大歓迎です。誰かやってください（他力本願）。

==== 歌声の潜在表現学習と歌唱スタイル変換

同一楽曲に関する多様な歌唱スタイルのパラレルデータがあるので、それを使えば「歌唱スタイルを特徴付ける潜在的な特徴量」が学習できるかもしれません。
たとえば、「通常歌唱スタイルからハミング歌唱スタイルを減算すると読み上げ調歌唱になる」とか、「通常歌唱スタイルと読み上げ調歌唱スタイルを混合するとミュージカルっぽい歌唱になる」とかになると面白いです。
Variational autoencoder (VAE)@<fn>{vae}とかcycle-consistent generative adversarial network (CycleGAN)@<fn>{cyclegan}とか使えば実現できるかもしれません（やるとは言ってない）@<fn>{yaru}。
//footnote[vae][@<href>{https://arxiv.org/abs/1312.6114}]
//footnote[cyclegan][@<href>{https://arxiv.org/abs/1703.10593}]
//footnote[yaru][ちょっと調べたら、それっぽい先行研究はあるみたいです。これとか @<href>{https://arxiv.org/abs/1912.02613}]

==== SさんのAIシンガー実現

需要がなさそうなのでたぶんやりません。

==== Sさんの歌声からの or Sさんの歌声への声質変換

たぶんやりません。

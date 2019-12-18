= OJaMa-Song: 一般男性によるフリーの歌声コーパス（を作りたい人生だった）
@<author>{メリーク@<emoji>{squirrel}マス, Sython;twitter:SythonUK}
//profile{
D進は最高@<emoji>{heart_eyes_cat} 理想の人生です！
//}
//lead{
私の歌を聴けぇぇぇー！
//}

== はじめに

Sythonです．
時が経つのは早いもので，令和元年も終わりを迎えようとしています．
筆者自身も博士後期課程2年目の終わりを目前としており，来年度には博論を執筆しなければならないという現実をすべて自分のほうへねじ曲げたいと思う日々が続いております．
こういったつらい日々には，音楽を聴いて心を癒やす，あるいは歌でも歌って気分をリフレッシュしたいと思うものです．
本稿では，近年の歌声生成系の研究開発の動向についても触れつつ，筆者によりすでに構築された@<b>{「一般男性音声コーパス (OJaMa) 」}@<fn>{OJaMa}の拡張版である@<b>{「一般男性歌声コーパス (OJaMa-Song)」}を作ろう，という話を述べます．
//footnote[OJaMa][“O”rdinary “Ja”panese “Ma”le speakerコーパス．@<href>{http://sython.org/Corpus/OJaMa} で公開されています．]

== 近年の歌声生成系研究開発動向

本節では，ここ数年くらいで話題になった（と思う）歌声生成系の研究開発動向を，筆者が知る範囲で簡単にまとめます．

=== 「AIシンガー」ブームの到来

機械学習技術の進展に伴い，多くの企業や大学の研究室で「AIシンガー」@<fn>{AI_singer}の研究・開発がされています．
学術的な話題だと，音声情報処理に関するトップカンファレンスであるInterspeech2019@<fn>{IS2019}のBest Student Paperのひとつは，敵対的学習に基づくEnd-to-End歌声生成の論文@<fn>{adv_ss}でした．
産業的な話題だと，最近は日本国内だけでも数多くのAIシンガーが登場しつつあるように思えます．
例えば，2019年4月には，マイクロソフトにより開発されているAI「りんな」@<fn>{rnn}が「AIアーティスト」としてデビュー@<fn>{rnn_sing}したり，
名古屋工業大学とテクノスピーチが共同で開発したAI歌声合成システムを使って制作されたボーカルを起用したCDが発売@<fn>{stussr_sing}されたりしました．
前者の歌声はこちら@<fn>{rnn_youtube}で，後者の歌声はこちら@<fn>{stussr_youtube}で聴けるのですが，確かにこれはすごいなぁという感じがします．
筆者は特に声優に詳しいわけではないのですが，後者のさとうささら@<fn>{stussr}さんの歌声はもはや中の人（水瀬いのりさん）が出ているのではないかという印象です．
また，2019年9月には，ヤマハの歌声合成技術を用いて「AI美空ひばり」が実現され@<fn>{msrhbr}，新曲「あれから」@<fn>{arekara}が公開されたことも話題となりました．
故人の歌声をモチーフにしたという点で賛否両論あったと思われますが，筆者個人としては非常に夢のある技術だと思います．
//footnote[AI_singer][念のためですが，実在する女性歌手である「AI（あい）」さんは関係ありません．仮にAIさんを元にしたAIシンガーができたら，「AIAI（えーあいあい）」さんになるんでしょうか．]
//footnote[IS2019][@<href>{https://www.interspeech2019.org/}]
//footnote[adv_ss][論文は @<href>{https://www.isca-speech.org/archive/Interspeech_2019/abstracts/1722.html} で，歌声のデモは @<href>{https://www.youtube.com/watch?v=uAAXtjKEv2E} で聴けます．]
//footnote[rnn][@<href>{https://www.rinna.jp/}]
//footnote[rnn_sing][@<href>{https://news.microsoft.com/ja-jp/2019/04/03/190403-rinna-as-artist/}]
//footnote[stussr_sing][@<href>{https://av.watch.impress.co.jp/docs/news/1180386.html}]
//footnote[rnn_youtube][@<href>{https://www.youtube.com/watch?v=_NPyt1AYUTg}]
//footnote[stussr_youtube][@<href>{https://www.youtube.com/watch?v=xRknlXIHkmI&feature=emb_logo}]
//footnote[stussr][@<href>{http://cevio.jp/cevio_character/}]
//footnote[msrhbr][@<href>{https://www6.nhk.or.jp/special/detail/index.html?aid=20190929}]
//footnote[arekara][@<href>{https://www.youtube.com/watch?v=nOLuI7nPQWU}]

=== 歌声コーパスの構築

前述のAIシンガーの開発には，当然ですが当該歌手の歌声データが必要です．
歌声データは基本的に権利関係の管理が特に厳しいこともあり，研究や個人の趣味等で使えるコーパスがなかなか構築されていないのですが，ここではいくつかの歌声コーパスについてまとめます．

==== HTS Japanese song data
名古屋工業大学の研究グループにより開発されたHMM/DNN-based Speech Synthesis System (HTS)@<fn>{HTS}のページからダウンロードできる，日本人女性話者による童謡の歌唱データ@<fn>{HTS_song}です．
raw形式で保存されているのでちょっとした工夫@<fn>{kufu}をしないとそのままでは再生できないですが，貴重なオープンの日本語歌声データです．
歌声データだけでなく，フルコンテキストラベル@<fn>{full}も公開されています．
//footnote[HTS][@<href>{http://hts.sp.nitech.ac.jp/}]
//footnote[HTS_song][@<href>{http://hts.sp.nitech.ac.jp/archives/2.3/HTS-demo_NIT-SONG070-F001.tar.bz2}]
//footnote[kufu][例えば，SPTKをダウンロードして raw2wav を使うとか？]
//footnote[full][楽譜などの情報を表す特徴量のようなものをイメージしていただけるとよいかと思います．]

==== JSUT-Song

東京大学の猿渡・小山研究室@<fn>{sarulab}の研究グループにより構築された，日本人女性話者の歌声データセット@<fn>{jsut-song}です．
内容はHTS Japanese song dataと同じ童謡なので，やろうと思えば歌声変換などはすぐにできると思います．
ちなみに，話者は同研究室で構築された「JSUTコーパス」@<fn>{jsut}と同じです．
//footnote[sarulab][@<href>{http://www.sp.ipc.i.u-tokyo.ac.jp/}]
//footnote[jsut-song][@<href>{https://sites.google.com/site/shinnosuketakamichi/publication/jsut-song}]
//footnote[jsut]["J"apanese "S"peech corpus of "S"aruwatari-lab, University of "T"okyo の略です．本当に注意していただきたいのですが，"JUST"コーパスではありません． @<href>{https://sites.google.com/site/shinnosuketakamichi/publication/jsut?authuser=0}]

==== JVS-MuSiC

2020年春季音響学会の講演プログラム集@<fn>{asj2020s}を眺めていたら，「JVS-MuSiC: Japanese multispeaker singing-voice corpus」という題目の講演がありました．
想像ですが，猿渡・小山研究室の研究グループにより「JVSコーパス」@<fn>{jvs}という100名の日本人話者により発話された音声コーパスが構築されているので，それの歌声版という感じでしょうか？
今後に期待ですね！
//footnote[asj2020s][@<href>{https://acoustics.jp/cms/wp_asj/wp-content/uploads/2020spring_program.pdf}]
//footnote[jvs]["J"apanese "V"ersatile "S"peech コーパス @<href>{https://sites.google.com/site/shinnosuketakamichi/research-topics/jvs_corpus?authuser=0}]

==== 東北きりたん歌唱データベース

明治大学の森勢研究室@<fn>{morise-lab}により構築された，東北きりたん@<fn>{kiritan}（厳密には，中の人である茜屋日海夏さん）の歌声データセット@<fn>{kiritan-sing}です．
私は声優について特に詳しいわけではないのですが，人気女性声優の歌声データセットということで非常に貴重だと思います．
//footnote[morise-lab][@<href>{http://www.kisc.meiji.ac.jp/~mmorise/lab/}]
//footnote[kiritan][@<href>{https://www.ah-soft.com/voiceroid/kiritan/}]
//footnote[kiritan-sing][@<href>{https://zunko.jp/kiridev/login.php}]

== OJaMa-Song



== おわりに
本稿では，近年の歌声生成系の研究開発の動向について触れつつ，著者が新たに構築した@<b>{「一般男性歌声コーパス (OJaMa-Song)」}について述べました．
実はまだリリースの準備ができていないので，冬コミ当日までにリリースできるように頑張ります．

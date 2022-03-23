#### 前言

本demo是使用开源项目lame来将PCM数据编码成MP3数据。（环境：x86_64 Ubuntu16.04 64位）



### 1、编译使用

**lame的编译：**(源码下载地址：[https://sourceforge.net/projects/lame/files/lame/](https://sourceforge.net/projects/lame/files/lame/))

```bash
tar xzf lame-3.100.tar.gz
cd lame-3.100/
./configure --prefix=$PWD/_install
make
make install
```

**本demo的编译与使用：**

```bash
$ make clean && make
$ 
$ ./pcm2mp3
Usage: 
    ./pcm2mp3 <in pcm file> <smaplerate> <channels> <out mp3 file>
Examples: 
    ./pcm2mp3 audio/test1_44100_16bit_stereo.pcm 44100 2 out1_44100_stereo.mp3
    ./pcm2mp3 audio/test2_22050_16bit_stereo.pcm 22050 2 out2_22050_stereo.mp3
    ./pcm2mp3 audio/test3_22050_16bit_mono.pcm   22050 1 out3_22050_mono.mp3
    ./pcm2mp3 audio/test4_8000_16bit_mono.pcm     8000 1 out4_8000_mono.mp3
```



### 2、参考文章


 - [Is there any LAME C++ wrapper\_simplifier (working on Linux Mac and Win from pure code)\_ - Stack Overflow](https://stackoverflow.com/questions/2495420/is-there-any-lame-c-wrapper-simplifier-working-on-linux-mac-and-win-from-pure)

 - [使用libmp3lame库编码mp3\_太上绝情的博客-CSDN博客\_libmp3lame.mhtml](https://blog.csdn.net/bjrxyz/article/details/73435407)

 - [c++ - 使用 libmp3lame\_liblame\_lame 将单声道 PCM 文件编码为 MP3 - IT工具网.mhtml](https://www.coder.work/article/1215758)



### 附录（demo目录架构）

```bash
$ tree
.
├── audio
│   ├── out1_44100_stereo.mp3
│   ├── out2_22050_stereo.mp3
│   ├── out3_22050_mono.mp3
│   ├── out4_8000_mono.mp3
│   ├── test1_44100_16bit_stereo.pcm
│   ├── test2_22050_16bit_stereo.pcm
│   ├── test3_22050_16bit_mono.pcm
│   └── test4_8000_16bit_mono.pcm
├── docs
│   ├── c++ - 使用 libmp3lame_liblame_lame 将单声道 PCM 文件编码为 MP3 - IT工具网.mhtml
│   ├── Is there any LAME C++ wrapper_simplifier (working on Linux Mac and Win from pure code)_ - Stack Overflow.mhtml
│   └── 使用libmp3lame库编码mp3_太上绝情的博客-CSDN博客_libmp3lame.mhtml
├── include
│   └── lame
│       └── lame.h
├── lib
│   └── libmp3lame.a
├── main.c
├── Makefile
└── README.md
```


用SDL2复刻冒险岛,使用[cmake(MinGW)](https://github.com/niXman/mingw-builds-binaries/releases/tag/13.2.0-rt_v11-rev0)构建.

资源文件进QQ群760717877下载

快速拉取本项目

```
git clone --recurse-submodules https://github.com/PShocker/sdlMS.git --depth 1
```

新建build目录

资源Data放进build目录,非Windows系统需要把simsum.ttc放进去

**[Windows编译](doc/Windows.md)**

**[Linux编译](doc/Linux.md)**

**[Emscripten编译(WEB)](doc/Emscripten.md)**

**[Android编译(Android Studio)](doc/Android.md)**

**[Mac编译](doc/Mac.md)**

编译前需要保证FFMPEG目录正确

# Todo:

- 预编译头文件
- 摄像机任务居中
- 人物ID名片
- 攀爬左右翻转
- 攻击动画瞬发
- 不同武器手持动作不同
- 林中之城藤蔓图层不对
- UI
- 切换地图的音乐没跟上

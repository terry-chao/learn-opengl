# ThatGame

Visual Studio 解决方案工程（GLFW + Glad + stb_image + glm）。

## 目录结构

```
ThatGame.sln
ThatGame/
  ThatGame.vcxproj
  src/
    core/
    input/
    rendering/
  assets/
  vendor/
    Glad/
    GLFW/
    stb/
    glm/
```

## 构建

用 Visual Studio 打开 `ThatGame.sln`，选 **Debug | x64**，生成并运行。

或命令行：

```bat
msbuild ThatGame.sln /p:Configuration=Debug /p:Platform=x64
```

## 运行效果

橙色三角形，青灰背景；按 `Esc` 退出。

新增源文件时：在 VS 解决方案资源管理器中右键对应筛选器 → 添加 → 现有项。

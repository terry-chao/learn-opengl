# Learn OpenGL

Visual Studio 解决方案工程（GLFW + Glad）。

## 目录结构

```
learn-opengl.sln
learn-opengl/
  learn-opengl.vcxproj
src/
  core/
  input/
  rendering/
vendor/
  Glad/
  GLFW/
```

## 构建

用 Visual Studio 打开 `learn-opengl.sln`，选 **Debug | x64**，生成并运行。

或命令行：

```bat
msbuild learn-opengl.sln /p:Configuration=Debug /p:Platform=x64
```

## 运行效果

橙色矩形，青灰背景；按 `Esc` 退出。

新增源文件时：在 VS 解决方案资源管理器中右键对应筛选器 → 添加 → 现有项。

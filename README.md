## WowSay

A layer allowing you to use any input software in Wows. 

战舰世界输入法兼容性问题的简易中间层解决方案，旨在允许用户使用任何输入法在游戏中交流而不受输入法兼容性问题所困。

### 使用

1. 运行 WowSay.exe；
2. 按下 **Ctrl + Enter 唤起或隐藏** WowSay 窗口；
3. 游戏中，按下 Ctrl + Enter 唤起 WowSay 窗口，使用任意输入法输入内容，然后**按下 Enter 发送**；
4. 想要退出，请右击**托盘图标**，选择 **Exit** 以退出。

### 注意

1. WowSay 仅在战舰世界处于任一**窗口化**模式时可用；
2. 请确保游戏**窗口名为“战舰世界”**，未找到“战舰世界”窗口时，托盘会发出消息提醒，且模拟操作不会被发送；
3. 您无必要，也不应将 WowSay 运行于任何沙盒环境中，在部分沙盒环境中 WowSay 无法正常工作。

### 原理

知晓原理以更加快速、彻底地了解 WowSay。

- WowSay 使用 QHotkey 注册热键（Ctrl + Enter），用户触发热键后，WowSay 显示或隐藏自身窗口；

- WowSay 检测文本输入框内容，当探测到换行符于末尾，WowSay 将查找“战舰世界”窗口；

- 找到“战舰世界”窗口后，WowSay 尝试暂存系统剪贴板中的文本内容，然后将文本输入框中的内容写入系统剪贴板；

- WowSay 隐藏自身并将“战舰世界”窗口唤至前台，使用 SendInput (from Win API) 依次发送如下按键：
    - Enter
    - Ctrl + V
    - Enter
    
    每次发送按键将包含 100ms 延时以确保模拟操作生效，这使用 QTimer 进行控制；
    
- WowSay 完成发送工作并将暂存的原剪贴板中的文本（如果存在）重新写入剪贴板。

至此，WowSay 完成了一个工作周期。

此外，发行版中的可执行文件使用 Enigma Virtual Box 打包，包含了必要的 Qt 动态链接库。

### 使用的框架、库与开源项目

- [Qt](https://www.qt.io/)
- [QHotkey](https://github.com/Skycoder42/QHotkey)

### 开发环境

Visual Studio 2022 with Qt 5.15.2_msvc2019_64

### 简单修改与拓展

修改 WowSay.h 中的部分宏以快速适配其他服务器或其他游戏。

| Name             | Default Value | Description                                                  |
| ---------------- | ------------- | ------------------------------------------------------------ |
| PRESENDTEXTKEY   | VK_RETURN     | The key to send before "Ctrl + V".                           |
| AFTERSENDTEXTKEY | VK_RETURN     | The key to send after "Ctrl + V".                            |
| TARGETWINDOWNAME | "战舰世界"    | The name of the window you want to send input to.            |
| SENDKEYINTERVAL  | 100           | The time interval between sending each key (in microseconds). |

### 许可证

[GNU GPL 3.0](https://www.gnu.org/licenses/gpl-3.0.en.html)
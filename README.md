# CTF-NetA-Hack

面向 **Python 应用逆向工程研究** 的 CTF-NetA 分析项目，实现 CTF-NetA 所有功能解锁。（实测 V2.12.01）

CTF-NetA 是一个比较好用的 CTF 流量分析工具。虽然我已经很久不打 CTF 了，但最近在研究 Python 应用逆向工程，于是顺手把它作为了一个研究对象。

## About

CTF-NetA 使用了相当多的保护措施：

- PyArmor 保护部分 Python 模块
- Nuitka 将核心模块编译为 `.pyd`
- 核心模块进一步使用 VMProtect 3.6.x+ 保护
- License 涉及 AES、RSA 等密码学逻辑
- 机器码本身参与部分计算
- 复杂的反调试逻辑
- 虚拟机检测
- 文件 Hash 校验与防篡改检测

可以说非常有 CTF 选手的特色，保护层套得花里胡哨。

不过从逆向工程的角度来看，保护措施数量多并不一定意味着最终攻击面就小。

## Analysis

我的思路并不是从 License 算法开始硬啃。

首先 Hook `python311.dll`，Dump Python 运行过程中出现的字符串并作为日志观察程序行为，由此快速定位重点模块和函数。

确定核心逻辑后，通过：

```python
import module.core as core
```

直接加载 Nuitka 编译后的核心模块，对重点函数进行机器码 Dump、动态调试和行为分析。

最终发现实际上并不需要完整还原 PyArmor、VMProtect、AES、RSA 或整套 License 校验流程，只需要在 Python 与核心模块的交界位置进行非常小的运行时修改即可达到目标。

当前实现通过程序自带的 Python 环境加载原始核心模块，并在运行时替换部分检查逻辑后直接启动原程序。

原 CTF-NetA.exe 只是 PyInstaller 打包的启动器，用于捐赠、更新，所以编译后替换即可。

## Disclaimer

本项目仅用于 **Python 逆向工程技术研究与学习交流**。

请仅对自己拥有、获得授权或法律允许分析的软件进行研究。项目与 CTF-NetA 原作者无关。

## License

MIT License

# CTF-NetA-Hack

面向 **Python 应用逆向工程研究** 的 CTF-NetA 分析项目，实现 CTF-NetA 所有功能解锁。

CTF-NetA 是一个比较好用的 CTF 流量分析工具。虽然我已经很久不打 CTF 了，但最近在研究 Python 应用逆向工程，于是顺手把它作为了一个研究对象。

## About

CTF-NetA 使用了相当多的保护措施：

- PyArmor 保护部分 Python 模块
- Nuitka 将核心模块编译为 `.pyd`
- 核心模块进一步使用 VMProtect 3.6.x+ 保护
- License 涉及 AES、RSA 等密码学逻辑
- 机器码本身参与部分计算
- 复杂的反调试逻辑
- 文件 Hash 校验与防篡改检测

整体保护思路非常“CTF 选手风格”：能叠的基本都叠上了，花里胡哨，不过实际分析下来并没有那么难。

## Analysis

我的思路并不是从 License 算法开始硬啃。

首先 Hook `python311.dll`，Dump Python 运行过程中出现的字符串并作为日志观察程序行为，由此快速定位重点模块和函数。

确定核心逻辑后，通过：

```python
import module.core as core
```

直接加载 Nuitka 编译后的核心模块，对重点函数进行机器码 Dump、动态调试和行为分析。

最后发现其实不需要完整还原整套 License、AES/RSA、机器码计算以及各种反调试逻辑，只需要找到真正影响程序状态的关键路径，就可以得到一个非常小的修改方案。

当前实现采用独立 Launcher，编译后替换原 CTF-NetA.exe 即可。

## Disclaimer

本项目仅用于 **Python / Nuitka / PyArmor / VMProtect 逆向工程技术研究与学习交流**。

请仅对自己拥有、获得授权或法律允许分析的软件进行研究。项目与 CTF-NetA 原作者无关。

## License

MIT License

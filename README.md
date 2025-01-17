# X-Profiler

[![Npm](https://img.shields.io/npm/v/xprofiler)](https://www.npmjs.com/package/xprofiler)
[![Codecov branch](https://img.shields.io/codecov/c/github/hyj1991/xprofiler/master)](https://codecov.io/gh/hyj1991/xprofiler/branch/master)
[![Linux/osx build status](https://travis-ci.org/hyj1991/xprofiler.svg?branch=master)](https://travis-ci.org/hyj1991/xprofiler)
[![Windows build status](https://ci.appveyor.com/api/projects/status/c6y7jtux1m5jabgc/branch/master?svg=true)](https://ci.appveyor.com/project/hyj1991/xprofiler/branch/master)
[![Npm](https://img.shields.io/npm/dm/xprofiler)](https://www.npmjs.com/package/xprofiler)
[![License](https://img.shields.io/npm/l/xprofiler)](LICENSE)

Easy-Monitor v3.0 Node.js Runtime 插件，输出性能日志，并且可以进行实时的运行时状态采样。


## I. 兼容性

xprofiler 插件预计支持三大主流操作系统：

- Windows
- Linux
- macosX

支持的 node.js runtime 版本：

- v8.x
- v9.x
- v10.x
- v11.x
- v12.x
- v13.x

更低的版本因为已经不在官方 LTS 计划中，故正常情况下不再支持。


## II. 快速开始

### 安装

执行如下命令安装插件

```bash
npm i xprofiler --save
```

### 嵌入项目

在您的项目入口顶部引入即可，性能分析日志默认输出在 `os.tmpdir()` 下

```js
require('xprofiler')();
```

### 可配置的参数

本插件提供方以下的可配置参数:

* **log_dir**: 内核输出日志和性能文件的目录，默认为 `os.tmpdir()`
* **log_interval**: 内核对性能日志采样的时间间隔，默认为 `60s`
* **enable_log_uv_handles**: 是否要采集 libuv 句柄的详细分类信息，比如 tcp 句柄数量，timers 数量，文件句柄数量等，默认为 `true`
* **log_format_alinode**: 是否以 Node.js 性能平台（原 AliNode）的格式输出性能分析日志，默认为 `false`
* **log_level**: 输出日志信息级别，0 info，1 error，2 debug，默认为 `1`，即只输出 info 和 error 日志

您可以通过环境变量或者在 JavaScript 代码中引入插件时传入配置的方式来使用这些配置，具体如下所示：

#### 1. 环境变量配置

* **XPROFILER_LOG_DIR**: 其值为 String，覆盖 `log_dir`
* **XPROFILER_LOG_INTERVAL**: 其值为 Number，覆盖 `log_interval`
* **XPROFILER_ENABLE_LOG_UV_HANDLES**: 其值为 YES/NO，覆盖 `enable_log_uv_handles`
* **XPROFILER_LOG_FORMAT_ALINODE**: 其值为 YES/NO，覆盖 `log_format_alinode`
* **XPROFILER_LOG_LEVEL**: 其值为 String，覆盖 `log_level`

#### 2. 引入插件时传入配置

```js
require('xprofiler')({
  log_dir: '/path/to/your/logdir', // 性能分析日志输出目录
  log_interval: 120, // 采样间隔 120s
  enable_log_uv_handles: false, // 不输出 uv 句柄分类详情
  log_format_alinode: true, // 以 alinode 的格式输出日志
  log_level: 0 // 只输出 info 日志
});
```

#### 3. 配置覆盖规则

配置覆盖顺序为 **用户传入参数的优先级最高，环境变量次之**，简单的使用代码来表示参数覆盖规则为：

```js
const defaultConfig = {
  log_dir: os.tmpdir(),
  log_interval: 60, // seconds
  enable_log_uv_handles: true,
  log_format_alinode: false,
  log_level: 1
};

const xprofilerConfig = Object.assign({}, defaultConfig, envConfig, userConfig);
```
详细配置覆盖规则的测试可以参见 [config.test.js](https://github.com/hyj1991/xprofiler/blob/master/test/config.test.js)

### 性能分析日志

本插件按照固定的格式来输出性能分析日志，但是也目前为了兼容已有的 alinode 运行时日志，提供了对 alinode 的支持，通过 `log_format_alinode: true` 配置或者 `XPROFILER_LOG_FORMAT_ALINODE=YES` 环境变量来启用 alinode 日志。两种日志文件名区别如下：

* 默认 xprofiler 日志文件名称
  * info 日志：xprofiler-${YYYYMMDD}.log
  * error 日志：xprofiler-error-${YYYYMMDD}.log
  * debug 日志：xprofiler-debug-${YYYYMMDD}.log
* 以 alinode 方式输出日志文件名称
  * info 日志：node-${YYYYMMDD}.log
  * error 日志：node-error-${YYYYMMDD}.log
  * debug 日志：node-debug-${YYYYMMDD}.log

默认的 xprofiler 和 alinode 两种日志的主要区别在日志前缀上(当然输出的部分内容也不尽相同)，其中 xprofiler 日志前缀如下：

```bash
"[YYYY-MM-DD HH:mm:ss] [日志级别] [日志类型] [pid] [xprofiler 版本] 日志详情"
```

如果设置为 alinode 日志格式输出，日志前缀则为：

```bash
"[YYYY-MM-DD HH:mm:ss.usec] [日志级别] [日志类型] [pid] 日志详情"
```

插件 xprofiler 多采集一个输出日志信息时的版本，目的是方便服务端统计当前各个 xprofiler 插件版本以及当前对应的应用信息，以帮助找到负责人来推动核心插件版本的升级。

本插件和 alinode 的另一个区别是 xprofiler 默认的日志目录为 `os.tmpdir()` 对应的目录，而非 alinode 原生默认的 `/tmp` ，原因是为了兼容 windows；并且上述的 node.js api 本身已经跨平台且安全，也避免了一些容器环境下人为将 `/tmp` 目录映射到挂载磁盘带来的问题。

当然您也可以通过更改 `log_dir` 配置或者 `XPROFILER_LOG_DIR` 环境变量来切换日志输出的目录，只需要确保你的 node.js 应用和采集器有权限读写该目录即可。

### 日志采集

如果您配置了 `log_format_alinode: true`，即按照 alinode 的格式输出日志，那么直接使用官方提供的 [agenthub](http://npmjs.com/package/@alicloud/agenthub) 即可采集上报插件生成的日志，值得注意的是，此时需将 agenthub 的配置项 `logdir` 配置为插件 xprofiler 的 `log_dir` 对应的服务器路径。

### 运行时状态采样

// TODO，尚未实现


## III. 插件架构和实现原理

// TODO


## IV. 稳定性

xprofiler 每个 commit 都会通过 travis-ci/appveyor 进行在 windows/linux/macosX 下的编译以及单元测试，如果您在使用中依旧出现意外的 crash，可以反馈到 [Issue 列表](https://github.com/hyj1991/xprofiler/issues) 进行跟踪处理。


## V. 测试与代码覆盖率

在项目根目录下执行如下命令即可获取单元测试与代码覆盖率状况：

```bash
npm run cov
```

master 分支的代码覆盖率情况可以参见 [codecov badge](https://codecov.io/gh/hyj1991/xprofiler/branch/master)。


## VI. 如何参与贡献

// TODO


## VII. License

[BSD-2-Clause](LICENSE)
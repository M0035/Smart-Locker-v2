# 智能快递柜 V2

本仓库保存智能快递柜项目的第二版源码，包括 Qt Quick 客户端、C/SQLite UDP 服务端和 Web 管理页面。相比 V1，服务端与 Web 页面使用固定的 SQLite 数据库路径，避免因运行目录变化而误读或新建数据库；Web 页面让管理者更直观地查看柜格状态、投递记录和快递员信息。数据库文件及真实数据不纳入仓库。

## 目录

| 目录 | 内容 |
| --- | --- |
| `qt-client/` | Qt Quick 客户端及资源 |
| `udp-server/` | UDP 服务端与 SQLite 数据操作 |
| `web/` | 登录页、CGI 管理页、样式和构建文件 |

## 构建说明

服务端在 Linux 中进入 `udp-server/` 后运行 `make`，生成 `main`。需要 GCC 和 SQLite3 开发库。Qt 客户端使用 Qt Creator 打开 `qt-client/lockers.pro`。Web 代码需要支持 CGI 的 Apache 环境；在 `web/` 中编译 `home.c`、`login.c` 并按 Apache 配置部署页面及 CGI。

`udp-server/server.h` 和 `web/home.c` 当前都将数据库路径设为 `/home/linux/udpServer/lockers.db`。在其他机器部署时，应一起调整路径并设置适当的文件和目录权限。固定路径本身不能代替访问控制；当前 Web 登录和密码处理仍属于学习阶段实现，部署到公网前需要完善。

Windows 使用 MI 接口来为外界提供系统信息和管理接口, 可以用脚本语言 (VMIC, Powershell, VB, C++)来进行交互.

CIM, Common Information Model, 是一种跨平台的通用信息模型, 是一种用于描述所有 IT 管理信息的模型和标准. CIM 包含以下结构:
1. CIM Schema: 定义管理信息的通用类和关系.
2. CIM Classes: 定义管理对象的属性和行为的模板
3. CIM Associations: 定义类之间关系的特殊类, 用于表达对象之间的逻辑联系, 如某个软件安装在了哪个硬盘上.
4. CIM Instances: 实例定义了系统上一个实际的管理对象.

[WMI](https://learn.microsoft.com/en-us/windows/win32/wmisdk/wmi-start-page): Windows Management Instrumentation, 是微软基于 WBEM 和 CIM 标准建立的一套收集系统信息和管理系统设置的接口. WMI 包含以下几个结构:
1. WMI Repository: 包含所有类/实例/命名空间的本地数据库, 定义了所有可以管理和监控的Windows资源.
1. WMI Providers: 作为中介, 从底层对象收集信息并反馈给WMi.
2. WMI Classes: 定义管理对象(Dick, OS, Process)的模板, 遵循CIM标准.
3. WMI Namespaces: 组织类和实例的容器, 如 `Root\CIMV2`

MI: Windows Management Infrastructure, 是 WMI 下一代版本, 提供了更现代的 API 和更高安全性, 并且对 CIM 兼容性更好.

```
Windows Management Instrumentation (WMI)
|
|-- WMI Repository
|   |
|   |-- Namespaces
|       |
|       |-- root
|           |
|           |-- CIMV2 (Commonly used for most management tasks)
|               |
|               |-- Classes
|                   |
|                   |-- Win32_Process
|                   |-- Win32_Service
|                   |-- Win32_OperatingSystem
|                   |-- Win32_ComputerSystem
|                   |-- Win32_NetworkAdapterConfiguration 
|                   ...
|
|-- WMI Providers
    |
    |-- Win32 Provider
    |-- Security Provider
    |-- Registry Provider
    ...
```
## WMIC

WMIC, WMI Command-Line, 提供了一组命令行命令来管理和获取 windows 信息. 查看[官方使用文档](https://learn.microsoft.com/en-us/windows/win32/wmisdk/using-wmi), WMIC 在较新系统版本中已经被 `Powershell WMI` 取代.

输入 `wmic` 进入该命令行环境, `quit` 退出, `wimc /?` 获取帮助

### 查看信息

- 查看 CPU 信息: `cpu get name,CurrentClockSpeed,MaxClockSpeed`
- 查看 OS 版本: `os get name,version`


### 管理进程

- 列出所有活跃进程: `process list brief`
- 杀死进程: `wmic process where name="notepad.exe" delete`

### 管理服务

- 查看所有服务 `service list brief`
- 查看特定服务状态 `service where name="wuauserv" get name,state`

### 硬件信息

- 硬盘: `diskdrive get model,size`
- 内存: `memorychip get capacity`

## MI

通过 Powershell CIM Cmdlets 来和 MI 进行交互. `Get-CimInstance` 别名 `gcim`

- 获取操作系统信息: `Get-CimInstance -ClassName CIM_OperatingSystem`
- 获取进程信息: `Get-CimInstance -ClassName Win32_Process`

使用 SQL 或 Filter 查询字段信息:

```powershell
Get-CimInstance -Query "SELECT * from Win32_Process WHERE name LIKE 'P%'"

Get-CimInstance -ClassName Win32_Process -Filter "Name like 'P%'"
```

> 更多命令直接让 CPT 写吧, CIM 模型还挺复杂的.
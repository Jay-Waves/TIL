计算机标识时间的格式是协调世界时 (UTC, Coordinated Universal Time), 该格式和具体时区无关, 如: `UTC 15:00 minus 7 hours`. Unix epoch 使用 UTC 的秒格式, 如 `1407694710`, 其含义是从 `UTC 1970-1-1 00:00:00` [^1] 开始的秒数.

- GMT, Greenwich Mean Time, 格林威治时间, 即将地球按15°经度分为24个时区. 如中国为东8区, 即 `GMT+8`, 比标准 `GMT` 早八小时.
- DST, Daylight Saving Time, 夏令时, 即人为将天亮较早的夏季调快一小时, 便于早起早睡, 充分利用阳光减少, 照明用电. 中国 1986-1991 年实行夏令时, 1992 年废除, 夏令时是影响时区时钟的不稳定因素之一.
- CST, China Standard Time, 中国标准时. 
- Asia/Shanghai, 以地区命名的地区标准时, 在中国等价于 CST. 

关系为: `GMT+8 = UTC+8 = CST = Asia/Shanghai`

[^1]: https://zh.wikipedia.org/zh-tw/UNIX%E6%97%B6%E9%97%B4

## time

Time Zone Conversion: 将 Unix 时间戳转化为计算机本地时区时间:

```python
from time import localtime, strftime
now = 1407694710
local_tuple = localtime(now)
time_format = '%Y-%m-%d %H:%M:%S'
time_str = strftime(time_format, local_tuple)
print(time_str)
>> 2014-08-10 11:18:30
```

将本地时区时间转化为 UTC 时间:
```python
from time import mktime, strptime
time_tuple = strptime(time_str, time_format)
utc_now = mktime(tiem_tuple)
print(utc_now)
>> 1407694710 0
```

但 time 模块处理时区间转换较棘手. 因为 time 依赖于 C 调用, 而 C 调用依赖于当前操作系统时区信息. 比如, 中国的电脑能解析 `CST` 时间, 但是不能解析英国 `EDT` 时间.

```python
strptime('2014-05-01 23:33:24 EDT', time_format)
>> ValueError ....
```

## datetime

高度兼容 time, 同时提供时区转换 api.

```python
from datetime import datetime, timezone

time_str = '2014-08-10 11:18:30'
now = datetime.strptime(time_str, time_format)
utc_now = time.mktime(now.time_tuple())

now = datetime(2014, 8, 10, 18, 18, 30) 
now_utc = now.replace(tzinfo=timezone.utc)
now_local = now_utc.astimezone() # use local timezone
```

datetime 本身也不提供完整的 timezone 信息, 可以按需下载 [pytz](https://pypi.python.org/pypi/pytz/) 模块.

```python
now = datetime.strptime(time_str, time_format) # parse time str
eastern = pytz.timezone('US/Eastern') # localize with current timezone
nyc_dt = eastern.localize(now)
utc_dt = pytz.utc.normalize(nyc_dt.astimezone(pytz.utc)) # convert to UTC

pacific = pytz.timezone('US/Pacific')
sf_dt = pacific.normalize(utc_dt.astimezone(pacific))

shanghai = pytz.timezone('Asia/Shanghai')
shanghai_dt = shanghai.normalize(utc_dt.astimezone(shanghai))
```

计算机系统中, 应尽量使用 UTC 维护时间, 将转化为本地时间作为最后一步调用.

### ZoneInfo

py3.9 引入的官方支持, 使用 IANA 时间区域数据库, 一致性较强.

```python
from datetiem import datetime
from zoneinfo import ZoneInfo
ny_time = datetime.now(ZoneInfo("America/New_York"))
london_time = datetime.now(ZoneInfo("Europe/London"))
```
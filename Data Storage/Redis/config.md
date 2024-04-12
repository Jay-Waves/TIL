`sudo vim /etc/redis/redis.conf`

开启 json 模块:

1. 先编译模块:
```bash
git clone --recursive https://github.com/RedisJSON/RedisJSON.git
cd RedisJSON
./sbin/setup
make build
```

2. 配置模块:
```conf
loadmodule /usr/lib/redis/modules/rejson.so
```
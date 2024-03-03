---
date: 2023-09-01
tags:
  - Paper
简化版msg数据库: true
数据流版数据库: false
---

### 数据流数据库

直接改用 mySQL 了? 存储整个数据流

数据流以二进制存储, size 记录了每个数据块的大小. mutate 是以 size 为单位的, 进行复制, 插入和删除. 

```sql
CREATE TABLE msg_streams (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    type_id INTEGER NOT NULL,
    topic TEXT NOT NULL,
	
	msgs BLOB NOT NULL, -- 整个 msgs 数据流以二进制形式存储.
	msg_size BLOB NOT NULL,

	-- layer INTEGER DEFAULT (0)
		-- CHECK (layer IN (0, 1, 2, 3, 4) ) ,
		-- trigger auto-increment layer after insert
    prior INTEGER, 
	    -- inherit upper layer's priority, bug+2, cov new branch+1.
    cov_info REAL, 
	    -- code coverage, (lower substitution for bitmap)
	status TEXT 
		CHECK( status IN ('Interesting', 'Useless', 'Pending', 'Testing') ), 
    
    bug_id INTEGER,

	ctime TEXT DEFAULT (datetime('now')), 
    mtime TEXT DEFAULT (datetime('now')), -- trigger update mtime

    FOREIGN KEY (type_id) REFERENCES msg_types(id)
    FOREIGN KEY (bug_id) REFERENCES bugs(id)
);
```

### 复杂版 (已舍弃)

简化版和复杂版都使用python

1. 消息类型表
2. 原始及有价值信息收集表
3. 变异信息收集表
4. bug 报告表

```sql
CREATE TABLE bug_reports (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
	msg_id INTEGER,
    
    bug_title TEXT NOT NULL,                
    description TEXT NOT NULL, 
    reproduction_steps TEXT,                -- 复现步骤
    triggered_env TEXT,                     -- 测试环境
    stack_trace TEXT,                       -- 堆栈
    impact_scope TEXT,                      -- 影响范围
    log_link TEXT,                          -- 相关的日志信息链接
    media_link TEXT,                        -- bug 截图或视频

    FOREIGN KEY (msg_id) REFERENCES valuable_msgs(id)
);

CREATE TABLE msg_types (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    msg_type TEXT NOT NULL,
    description TEXT NOT NULL
);

CREATE TABLE valuable_msgs (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    msg_type_id INTEGER not null,
    source_topic TEXT,
    msg_content TEXT NOT NULL,

    priority REAL, -- 测试以及异变顺位
    code_coverage REAL, -- 代码覆盖率
	test_status TEXT CHECK( test_status IN ('Success', 'Fail', 'Pending', 'Mutated') ), 
    bug_report_id INTEGER,

	ctime TEXT DEFAULT (datetime('now')), 
    mtime TEXT DEFAULT (datetime('now')), -- 修改时触发 update mtime

    FOREIGN KEY (msg_type_id) REFERENCES msg_types(id)
    FOREIGN KEY (bug_report_id) REFERENCES bug_reports(id)
);

CREATE TRIGGER update_mtime
AFTER UPDATE ON valuable_msgs
BEGIN
    UPDATE  valuable_msgs
    SET mtime = datetime('now')
    WHERE id = NEW.id;
END;

CREATE INDEX msg_priority ON msgs(priority);

CREATE TABLE mutated_msgs ( 
	/* 将原始消息进行异变, 若出现bug, 或提高了代码覆盖率, 就放 valuable_msgs 池中*/
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    source_msg_id INTEGER not null,
    msg_content TEXT not null,

    priority REAL,
    code_coverage REAL,
	test_status TEXT CHECK( test_status IN ('Success', 'Fail', 'Pending') ),

    mtime TEXT DEFAULT (datetime('now')), -- 修改时触发 update mtime

    FOREIGN KEY (source_msg_id) REFERENCES valuable_msgs(id)
);
```

![|350](../../attach/Pasted%20image%2020230830215031.png)

```sql
INSERT INTO sensors (message_type, description)
VALUES 
('sensor_msgs/msg/Image', '此传感器捕获图像或视频流。'),
('sensor_msgs/msg/PointCloud2', '例如 Microsoft Kinect 或 Intel RealSense，它们可以捕获深度信息。'),
('sensor_msgs/msg/LaserScan', '用于测量对象与机器人之间的距离。'),
('sensor_msgs/msg/Range', '使用超声波测量距离。'),
('sensor_msgs/msg/Imu', '测量线加速度、角速度和（有时）磁场方向。'),
('sensor_msgs/msg/NavSatFix', '获取地理位置坐标。'),
-- ... 其他数据可以按照同样的方式添加
;
```

### 简化版 (部分舍弃)

仅适用于fuzz一个线程一个输入的程序, 不适合ros.

**标记规则:**

1. bitmap 新增, 或出现 bug, 标记为 interesting
2. bitmap 无变化, 标记为 useless
3. 尚未测试, 标记为 pending
4. 正在测试, 标记为 testing
5. 变异完, 标记为 mutated

**状态变化图:**

`pending`: waiting for fuzz  
|  
`testing`: fuzz test, mutex     
|  
`interesting`, ignore `useless`  
|  
`mutating`: mutate new testcases,  mutex  
|  
`mutated`: layer+1  

**选择规则:**

1. interesting msg, 变异, 并 layer+1 
2. useless msg 仅存储.
3. 先遍历选择 低 layer, 然后遍历选择 高 layer.
4. layer 达到 4 后, 不再变异.



```sql
CREATE TABLE bugs (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
	msg_id INTEGER,
	but_type TEXT, 
	bug_info TEXT NOT NULL,
    
    FOREIGN KEY (msg_id) REFERENCES msgs(id)
);

CREATE TABLE msg_types (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    type TEXT NOT NULL,
    format TEXT NOT NULL,
    description TEXT
);

CREATE TABLE msgs (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    type_id INTEGER NOT NULL,
    topic TEXT NOT NULL,
	details TEXT NOT NULL,

	layer INTEGER DEFAULT (0)
		CHECK (layer IN (0, 1, 2, 3, 4) ) ,
		-- trigger auto-increment layer after insert
    prior INTEGER, 
	    -- inherit upper layer's priority, bug+2, cov new branch+1.
    cov_info REAL, 
	    -- code coverage, (lower substitution for bitmap)
	status TEXT 
		CHECK( status IN ('Interesting', 'Useless', 'Pending', 'Testing') ), 
    
    bug_id INTEGER,

	ctime TEXT DEFAULT (datetime('now')), 
    mtime TEXT DEFAULT (datetime('now')), -- trigger update mtime

    FOREIGN KEY (type_id) REFERENCES msg_types(id)
    FOREIGN KEY (bug_id) REFERENCES bugs(id)
);

CREATE TRIGGER update_mtime
AFTER UPDATE ON msgs
BEGIN
    UPDATE  msgs
    SET mtime = datetime('now')
    WHERE id = NEW.id;
END;

CREATE TRIGGER increment_layer_after_insert
AFTER INSERT ON msgs
BEGIN
    UPDATE msgs
    SET layer = layer + 1
    WHERE id = NEW.id;
END;

CREATE VIEW select_test_case AS
SELECT * 
FROM msgs
WHERE status = 'Pending'
ORDER BY layer ASC, prior DESC;

CREATE VIEW select_mutate_case AS
SELECT * 
FROM msgs
WHERE status = 'Interesting' AND layer < 4
ORDER BY layer ASC, prior DESC;

```

![|350](../../attach/Pasted%20image%2020230901123747.png)

收集了一些 msg_type:

```sql
INSERT INTO msg_types(type, format, description) VALUES ('geometry_msgs/Point', '{x: float64, y: float64, z: float64}', 'Represents a point in 3D space.');

INSERT INTO msg_types(type, format, description) VALUES ('geometry_msgs/Quaternion', '{x: float64, y: float64, z: float64, w: float64}', 'represent the orientation of an object in 3D space using the quaternion format.');

INSERT INTO msg_types(type, format, description) VALUES ('std_msgs/Bool', '{data: bool}', 'Used to send a boolean message.');

INSERT INTO msg_types(type, format, description) VALUES ('std_msgs/Int32', '{data: int32}', 'Sends a 32-bit integer.');

INSERT INTO msg_types(type, format, description) VALUES ('std_msgs/Float64', '{data: float64}', 'Sends a 64-bit floating point number.');

INSERT INTO msg_types(type, format, description) VALUES ('std_msgs/String', '{data: string}', 'Sends a string.');

INSERT INTO msg_types(type, format, description) VALUES ('std_msgs/Header', '{seq: uint32, stamp: time, frame_id: string}', 'Contains a timestamp and other meta-info about a message.');

INSERT INTO msg_types(type, format, description) VALUES ('geometry_msgs/Pose', '{position: Point, orientation: Quaternion}', 'Represents the position and orientation of an object in 3D space.');

INSERT INTO msg_types(type, format, description) VALUES ('geometry_msgs/Vector3', '{x: float64, y: float64, z: float64}', 'Represents a vector in 3D space.');

INSERT INTO msg_types(type, format, description) VALUES ('geometry_msgs/Twist', '{linear: Vector3, angular: Vector3}', 'Represents velocity in free space, both linear and angular.');

INSERT INTO msg_types(type, format, description) VALUES ('sensor_msgs/Image', '{header: Header, height: uint32, width: uint32, encoding: string, is_bigendian: uint8, step: uint32, data: uint8[]}', 'Used for transporting images, typically from a camera.');

INSERT INTO msg_types(type, format, description) VALUES ('sensor_msgs/LaserScan', '{header: Header, angle_min: float32, angle_max: float32, angle_increment: float32, time_increment: float32, scan_time: float32, range_min: float32, range_max: float32, ranges: float32[], intensities: float32[]}', 'Represents data from a 2D laser scanner.');

INSERT INTO msg_types(type, format, description) VALUES ('sensor_msgs/PointCloud2', '{header: Header, height: uint32, width: uint32, fields: PointField[], is_bigendian: bool, point_step: uint32, row_step: uint32, data: uint8[], is_dense: bool}', 'Represents data from a 3D point cloud sensor.');

INSERT INTO msg_types(type, format, description) VALUES ('nav_msgs/Odometry', '{header: Header, child_frame_id: string, pose: PoseWithCovariance, twist: TwistWithCovariance}', 'Represents an estimate of a position and velocity in free space.');

INSERT INTO msg_types(type, format, description) VALUES ('std_msgs/ColorRGBA', '{r: float32, g: float32, b: float32, a: float32}', 'Represents a color in RGBA format.');

```
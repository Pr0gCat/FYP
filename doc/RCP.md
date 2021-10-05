# Remote Control Protocol



## Package Spec.

```
0			1		 2		  N<=255	  N+1
| CommandID | Length | Payload | Checksum |
```

* Command ID - 指令編號
* Length - Payload 的長度
* Payload - 指令參數
* Checksum - 指令較驗



## Mechanism

```
Master						Car
|---------CMD_INIT--------->|
|		// 初始化 			|
|<--------CMD_CONFIM--------|
|---------CMD_SYNC--------->|
|<--------CMD_CONFIM--------|
.
.
.
```





## Commands



### `CMD_CONFIRM`

```
ID: 1
Caster: Master
```

Parameters:

| Desc.         | Type   |
| ------------- | ------ |
| cmd_id        | u8     |
| <cmd_payload> | depent |



Response:

none

### `CMD_SYNC`

同步Car狀態

```
ID: 2
Caster: Master
```

Response:

| Desc.                             | Type |
| --------------------------------- | ---- |
| Ready (1: True,  0: False)        | u8   |
| mode (1: 循線, 2: 拾貨, 3: 載貨)  | u8   |
| y_running (1: True, 0: False)     | u8   |
| z_running(1: True, 0: False)      | u8   |
| pos_y                             | u32  |
| pos_z                             | u32  |
| y_max                             | u32  |
| z_max                             | u32  |
| distance_mode (1: True, 0: False) | u8   |
| distance2go                       | i32  |
| running_l (1: True, 0: False)     | u8   |
| running_r (1: True, 0: False)     | u8   |
| mspeed_l                          | i16  |
| mspeed_r                          | i16  |
| claw_deg_l                        | i16  |
| claw_deg_r                        | i16  |

### `CMD_INIT`

重設Car的狀態

```
ID: 3
Caster: Master
```

Response:

`CMD_CONFIRM`



### `CMD_GET_CLAW_STATE`

取得夾子servo的角度

```
ID: 4
Caster: Master
```

Response:

| Desc.       | Type |
| ----------- | ---- |
| servo_deg_l | i8   |
| servo_deg_r | i8   |



### `CMD_MOVE_CLAW`

設定夾子servo的角度

```
ID: 5
Caster: Master
```

Parameters：

| Desc.       | Type |
| ----------- | ---- |
| servo_deg_l | i8   |
| servo_deg_r | i8   |

Response:

none

### `CMD_SET_LINEFOLLOW_MODE`

轉到循線模式，改變鏡頭角度，改變夾子高度，完全打開夾子

```
ID：6
Caster: Master
```

Response:

`CMD_CONFIRM`

### `CMD_SET_PICKUP_MODE`

轉到拾貨模式，改變鏡頭角度

```
ID:7
Caster: Master
```

Response:

`CMD_CONFIRM`

### `CMD_SET_DOCKING_MODE`

轉到載貨模式，改變鏡頭角度，夾子升高

```
ID:8
Caster: Master
```

Response:

`CMD_CONFIRM`

### `CMD_GET_POSY`

```
ID: 9
Caster: Master
```

Response:

| Desc.     | Type |
| --------- | ---- |
| Y軸的位置 | u32  |

### `CMD_GET_POSZ`

```
ID: 10
Caster: Master
```

Response:

| Desc.     | Type |
| --------- | ---- |
| Z軸的位置 | u32  |



### `CMD_MOVE_POSY`

```
ID: 11
Caster: Master
```

Parameters:

| Desc.    | Type |
| -------- | ---- |
| 目標位置 | u32  |

Response:

`CMD_CONFIRM`

### `CMD_MOVE_POSZ`

```
ID: 12
Caster: Master
```

Parameters:

| Desc.    | Type |
| -------- | ---- |
| 目標位置 | u32  |

Response:

`CMD_CONFIRM`

### `CMD_HOME_Y`

```
ID: 13
Caster: Master
```

Response:

`CMD_CONFIRM`

### `CMD_HOME_Z`

```
ID: 14
Caster: Master
```

Response:
`CMD_CONFIRM`

### `CMD_RUN_DISTANCE`

```
ID: 15
Caster: Master
```


### `CMD_GET_MOTOR_SPEED`

```
ID: 16
Caster: Master
```

### `CMD_SET_MOTOR_SPEED`

```
ID: 17
Caster: Master
```

Parameters:

| Desc.   | Type |
| ------- | ---- |
| speed_l | i16  |
| speed_r | i16  |



### `CMD_MSG`

```
ID: 255
Caster: Car
```

Parameters:

| Desc. | Type   |
| ----- | ------ |
| msg   | depent |






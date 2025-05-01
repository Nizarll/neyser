<img src = "https://github.com/Nizarll/neyser/blob/master/img/logo.png?raw=true" width="200" height ="55">

## Neyser an opensource C serialization library

neyser allows you to easilly serialize structs, fixed-sized integers and floats from the host \
how to serialize: 

**Todo**:
- Add fixed sized floats.
- Add Serialization for big integers, and big doubles , big floats 

**host machine** :
```c
  u32 my_u32 = 3000;
  i8 buff[MAX_BUFF_LEN];
  serialize_u32(my_u32, buff, MAX_BUFF_LEN);
  // send it over the network
```

**receiver machine**:
```c
  i8 buff[MAX_BUFF_LEN];
  // receive bytes
  u32 my_u32 = deserialize_u32(buff, size);
```

```

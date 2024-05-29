<img src = "https://github.com/Nizarll/neyser/blob/master/img/logo.png?raw=true" width="200" height ="55">

## Neyser an opensource C serialization library

neyser allows you to easilly serialize structs, fixed-sized integers and floats from the host \
how to serialize: 

*host machine* :
```c
  u32 my_u32 = 3000;
  i8 buff[MAX_BUFF_LEN];
  serialize_u32(my_u32, buff, MAX_BUFF_LEN);
  // send it over the network
```

*receiver machine*:
```c
  i8 buff[MAX_BUFF_LEN];
  // receive bytes
  u32 my_u32 = deserialize_u32(buff, size);
```

you can also serialize structs and deserialize them:

```c
  struct blob {
    int x;
    int y;
  }
  
  // not advised
  struct blob deserialize_blob(i8* buff, size_t size) {
    return *(struct blob *)buff;
  }

  int main() {
    i8 buff[MAX_BUFF_LEN];
    struct blob my_blob = {0};
    serialize_struct(&my_blob, sizeof(my_blob), buff, sizeof(buff));
    return 0;
  }
```

**Todo**:\
-Add fixed sized floats.\
-Add serialization for signed integers.\
-Serialization for mixed-endian and other endianesses.

# TIGER
## Enc-Dec Test
- To be updated

## Known Answer Test (KAT)
### Edit required parts
- "params.h"
```
#define PUBLICKEYSIZE 32+LWE_N
#define SECRETKEYSIZE LWE_N+MESSAGE_LEN
#define CIPHERTEXTSIZE LWE_N*2 
```

- "main.c" = PqCgenKAT_kem 

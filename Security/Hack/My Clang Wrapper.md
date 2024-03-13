```shell
#!/bin/bash

ASAN_ARGS="fsanitize=address"
SANCOV_ARGS="-fsanitize-coverage=trace-pc-guard"

MY_HOOK_SRC="/path/to/hook.cc"
MY_HOOK_OBJ="hook.o"

/usr/bin/clang++ $ASAN_ARGS -c $MY_HOOK_SRC -o $MY_HOOK_OBJ
/usr/bin/clang++ $ASAN_ARGS $SANCOV_ARGS "$@" $MY_HOOK_OBJ
```
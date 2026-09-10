# Build Instructions

## CMake

One of VividX's goal is to switch to a CMake-based build system.

```sh
$ mkdir build
$ cd build
$ cmake -G Ninja ..
$ ninja
```


## GN

To get `gn` command, use below command.

```sh
$ bin/fetch-gn
```

To build,

```sh
$ bin/gn gen out/Shared --args="$(<gn-args.txt)"
$ ninja -C out/Shared
```


## TODO

Below commands should be used only for developing VividX, not production build.

```sh
bin/gn gen out/Clang --args="$(<gn-args.trace.txt)"
ninja -C out/Clang
```

```sh
find out/Clang -name '*.json' -print0 | xargs -0 -n1 sh -c 'f="$1"; jq -r ".traceEvents[] | select(.name == \"Total ExecuteCompiler\") | [.dur / 1000000, \"$f\"] | @tsv" "$f"' sh | sort -nr > TRACE
```

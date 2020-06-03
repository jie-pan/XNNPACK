	bazel build --subcommands  --verbose_explanations --explain log.log -c opt f32_gemm_bench.js  --config=wasm 2>&1 | tee log2.log


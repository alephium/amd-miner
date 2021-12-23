# amd-miner

AMD capable PoW miner for Alephium base on OpenCL.

Please make sure that you have installed AMD pro for you GPU with OpenCL support.

```sh
amdgpu-pro-install --opencl=legacy,pal --headless
```

### Pre-built miner

You could download and run the pre-built miner from [Github release page](https://github.com/alephium/amd-miner/releases). Note that your anti-virus might warn about the pre-built miner.

You could specify the miner api with `-a <IP>` parameter.

### Miner from source code

Please use `make.sh` for Ubuntu or `build.ps1` for windows
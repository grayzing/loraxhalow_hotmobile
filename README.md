# loraxhalow_hotmobile

Prototype for _Rethinking Heterogeneous IoT Connectivity by bridging LoRa and Wi-Fi HaLow_

## Installation

This is assuming you have the setup described in the paper. You first need to clone the repo

```bash
git clone --recurse-submodules https://github.com/grayzing/loraxhalow_hotmobile.git
cd loraxhalow_hotmobile
```

You then need to install the dependencies,

```bash
sudo apt update
sudo apt install libzmq3-dev
```

### Building the packet forwarder

You'll need to build the LoRa packet forwarder, which has been modified to include a ZMQ client which forwards the received packet metadata to the ZMQ server on the localhost via IPC.

```bash
cd loraxhalow_sx1302_hal
make clean packet_forwarder
cd packet_forwarder
```

The packet forwarder requires a few things before you can run it. You need to choose the corresponding `global_conf.json.*` to your region. In our case, we are in the United States, so we run

```bash
mv global_conf.json.sx1250.US915 global_conf.json
cp ../tools/reset_lgw.sh ./reset_lgw.sh
```

### Building the ZMQ server

This is more simple than the packet forwarder. 

```bash
cd loraxhalow-zmq-server
sudo bash build.sh
```

## Running the setup

You need to run the ZMQ server first _and then_ the client.

```bash
cd loraxhalow-zmq-server
./server.out
cd ..
cd loraxhalow_sx1302_hal
./lora_pkt_fwd
```


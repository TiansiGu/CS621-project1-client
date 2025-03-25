# Compression Detect Applications

Developer: Tiansi Gu
This project provides two network applications that detect network compression by endhosts.
- Client/Server Application
- Standalone Application

## Table of contents

- Requirements
- Installation
- Configuration

## Requirements

To build and run the applications, the following environament and modules are required:
- Download [VM Ubuntu ISO](https://drive.google.com/drive/folders/1yZYTqGJdbampVp2MdhifeIlgPGoKruSG) image appropriate to your hardware and follow [VM Instal Instructions](https://docs.google.com/document/d/1ptEYs2jUfzgJ8Ojlpdex4RWdXppA8GJ050ht7pk3z9I/edit?tab=t.0) to set a client VM and a server VM on your machine. 
- Have gcc (version 11.4.0 or above) installed in both the VMs. Inside your VM, run
    ```
    $ sudo apt install gcc
    ```
- Have cjson library installed in both the VMs. Inside your VM, run
    ```
    $ sudo apt-get install libjson0 libjson0-dev
    ```


## Installation and Build

- Download comp_detect.zip in both of your VMs. Unzip it:
```
% unzip comp_detect.zip
% cd comp_detect/
```
### Client-Server Application
- In the client VM, run the following command:
```
% make -f Makefile_client
```
- In the server VM, run the following command:
```
% make -f Makefile_server
```

### Standalone Application
- In the client VM, run the following command:
```
% make -f Makefile_standalone
```

## Configuration
Before running the program, you need to have the public ip address of your client VM and server VM, respectively. Run the following command in your VM, and get ip address from enp0s1 - inet protocol
```
% ip a
```
Put the ip addresses as json strings in field `server_ip_addr` and `client_ip_addr`
```
{
  "server_ip_addr": "your-client-VM-ip-addr",
  "client_ip_addr": "your-server-VM-ip-addr",
  ......
}
```
Other configuration parameters are preset to default values, and you can change them as needed.

As we send packets aggressively from client, you need to adjust kernel buffer size based on the number of UDP packets in the UDP train (`n` in configurations) and the payload size of each UDP packet (`l` in configurations). Based on the default `n` and `l`, 8MB should be enough for the buffer to fit all the packets in a UDP train.
```
sudo sysctl -w net.core.rmem_max=8388608 //increase the buffer size to be 8MB
```

## Detection
Now, you can run the detection program.

### Client-Server Application
First, start the server
```
% ./compdetect_server 7777
```
Or
```
% ./compdetect_server
```
Note, if you changed `server_port_preprobing` to your custom value, be sure to run the server application with the same port as program parameter
```
% ./compdetect_server [server_port_preprobing]
```
Then start the client for detection
```
% ./compdetect_client myconfig.json
```

Wait for around 1 mins for the detection to complete. Since there is no compression link between the client VM and the server VM. You will see the detection output:
```
% No compression was detected.
```
If you install the server application on a host between which there do have compression link. The output would be:
```
% Compression detected!
```

### Standalone Application
To run the standalone application, you need to first get the ip address of the server you want to detect and put it in the `server_ip_addr` field of the configuration file. If you want to detect compression between the client and server VM, enter the ip address of the server VM.
You will run the standalone application on your client VM:
```
% sudo ./compdetect myconfig.json
```
Wait for around 1 mins for the detection to complete. The output would be the same format as the client/server application:


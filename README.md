# QSerial Socket Amigo

This tool is meant for an assitant tool for users who want to visualize data comming from serial port or net socket(***hasn't been implemented***).

The start goal of this project was to create a simple serial assistant for linux users, since qt is cross-platform this should be used on all platforms qt supports. And with the support of the decoding system, it should be able to help to visualize data coming from serial and net socket easier.

There should be many hardware modules and custom project are using this kind of communication protocal:

[one or two bytes frameheader] [function byte] [data] [CRC]

Like 0xAAAA for the frame header, 0x01 for function byte to indicate what this frame is and fowllowed with data and crc.

[Connection tab]

## Download

Download it at [release page](https://github.com/cybertale/QSerial-Socket-Amigo/releases/tag/v1.0).

## Compile

Use the latest qtcreator to compile and debug the project.

## How to Use

First configure the paramters of the connection on connection tab, and click 'open' button to open the connection.

### Simple Read Write

For simple read and write, just turn to the 'simple' tab and select the intepret type of sending and receiving, which should be hex or asc code.

Each send box is standalone for users may have many candidates buffers to send.

![](doc/images/Send%20window.png)

Repeat send will automatically start when the checkbox is checked.

### Advanced Read

Advanced read means basic decoding of the income data with custom protocal and shows it on some specific chart.

(TODO: here I think maybe we should support multiple frame headers.)

**Firstly**, Set the frame header, 0xAAAA is the default frame header.

**Secondly**, Add some variables to be decoded after the frame header. Select variable type and click add to add.

**Thirdly**, Select the endianess the protocal uses.

Most case in communication people uses big endian because it transfers MSB first to decrease the effect of losing later data bytes, while some devices may use little endian.

For example, a 16-bit variable with value `1`, big endian of its two bytes should be 0x00 0x01, and the little endian version of it should be 0x01 0x00.

**Fourthly**, Click the 'Enable' button.

Also this tab can be enabled both before and after the connection is opened.

**Example** To decode data of an IMU module

Here I got an IMU module keeps sending me its angles from serial. It sends me 3 `int16_t` data representing results for `angle / 180.0 * 65535` for roll, pitch, yaw. Frame of its data is composed of these following bytes:

[0x55][0x53][Lower byte of roll][Higher byte of roll][Lower byte of pitch][Higher byte of pitch][Lower byte of yaw][Higher byte of yaw]

Which means it sends 3 `int16_t` data after frame header with little endianess. So the tool should be configured as fowllows:

![](doc/images/Advanced%20Configuration.png)

It will automatically allocates some name for the variables and generate checkboxes on the right, click the checkbox will draw it on the chart on the bottom right of the window.

![](doc/images/Data%20in%20chart.png)

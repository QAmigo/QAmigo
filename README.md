# QSerial Socket Amigo

This tool is meant for an assitant tool for users who want to visualize data comming from serial port or net socket(***hasn't been implemented***).

The start goal of this project was to create a simple serial assistant for linux users, since qt is cross-platform this should be used on all platforms qt supports. And with the support of the decoding system, it should be able to help to visualize data coming from serial and net socket easier.

There should be many hardware modules and custom project are using this kind of communication protocal:

[one or two bytes frameheader] [function byte] [data] [CRC]

Like 0xAAAA for the frame header, 0x01 for function byte to indicate what this frame is and fowllowed with data and crc.

[Connection tab]

## Download

Download it at [release page](https://github.com/cybertale/QSerial-Socket-Amigo/releases).

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

**Firstly**, Using "Add Header" and "Add Data" button to add frame protocol settings, notice that we have to add header first data(means type data) are child nodes of the frame headers.

When we click the "Add header" button, the tool will ask for a hex byte array to be frame header:

![](doc/images/Add%20Header.png)

**Secondly**, Add some variables to be decoded after the frame header. Select variable type and the frame to add then click add to add.

Adding data needs us to select one header or data, the new type data will append to the frame we are selecting to.

The following two ways of selection will all add the new type data to the first frame, the one with header "0xAAAA".

![](doc/images/Add%20Data%20Select%20Header.png)

![](doc/images/Add%20Data%20Select%20Data.png)

**Thirdly**, Select the endianess the protocal uses.

Most case in communication people uses big endian because it transfers MSB first to decrease the effect of losing later data bytes, while some devices may use little endian.

For example, a 16-bit variable with value `1`, big endian of its two bytes should be 0x00 0x01, and the little endian version of it should be 0x01 0x00.

**Fourthly**, Click the 'Enable' button.

Also this tab can be enabled both before and after the connection is opened.

**Example** To decode data of an IMU module

Here I got an IMU module keeps sending me its angles, angular velocity and angular acceleration from serial. It sends me 3 kinds of frames, each of them are started with two bytes header, 0x5553, 0x5552, 0x5551, respectively. Three `int16_t` datas follows the header in each frame, represents their value.

For example, in angle frame, each of the `int16_t ` value represents results for `angle / 180.0 * 65535` for roll, pitch, yaw. 

Frame of their data is composed of these following bytes:

**Accelerometer**

[0x55][0x51][Lower byte of roll][Higher byte of roll][Lower byte of pitch][Higher byte of pitch][Lower byte of yaw][Higher byte of yaw]

**Angular velocity**

[0x55][0x52][Lower byte of roll][Higher byte of roll][Lower byte of pitch][Higher byte of pitch][Lower byte of yaw][Higher byte of yaw]

**Angle**

[0x55][0x53][Lower byte of roll][Higher byte of roll][Lower byte of pitch][Higher byte of pitch][Lower byte of yaw][Higher byte of yaw]

To decode these frames on one serial, we can configure our tool like this:

![](doc/images/Data%20in%20chart.png)

Log area on the right will show the value it decodes, number in the beginning represents the frame index.

Checkbox in front of the data line is for drawing data in chart, just check the data you want to plot and have some fun!

## About

The linux version released package uses [CQtDeployer](https://github.com/QuasarApp/Console-QtDeployer) to deploy, thanks to the author's great work!

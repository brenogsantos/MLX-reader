# MLX-reader

<div align="left">
    <img src="https://img.shields.io/badge/language-c-blue">
    <img src="https://img.shields.io/badge/language-c++-green">
    <img src="https://img.shields.io/badge/language-python-darkblue">
    <img src="https://img.shields.io/badge/language-JS-yellow">
    <img src="https://img.shields.io/badge/mcu-stm32-brightgreen">
  <img src="https://img.shields.io/badge/mcu-esp32-orange">
</div>


## Table of Contents
1. [About the Project](#about-the-project)
2. [Layout](#layout)
3. [PCB](#pcb)
4. [Resources](#resources)

# About the Project

The project idea was to design a PCB in order to create a protocol between an STM32 and an ESP32 via I2C/SPI. Also aiming study the use of the STM as a main controller (promoting a better low power mode) and the ESP32 as slave for MQTT communications. (A ideia do projeto foi porjetar uma PCB a fim de criar de um protocolo entre um STM32 e um ESP32 via I2C/SPI.  Tendo também como objetivo estudar o usao do STM como controlador principal  (promovendo um modo de low power melhor) e o ESP32 como slave para comunicações por MQTT.) 

The MLX90621 is a matrix IR sensor (made by Melexis) that uses the I2C protocol for communication. For this project, the sensor was used to validate the data read and sent by MQTT. 

In order to make the data visualization easier, a python server (using Flask) and a web client (using React JS) were created, enabling the visualization of the temperature matrix. The python server works as a "listener" to the MQTT topic to which the ESP32 sends the sensor data. The web client updates the temperature matrix data requesting to the python server.

The PCB, after designed, was manufactured and shipped by JLCPCB

Finally, ngrok was used to expose the localhost port used for external access (with a smartphone, for example)

<p align="center">
<a href="https://youtu.be/tC6STnkMpKg">Youtube Video</a> 
</p>

<p align="center">
  <img src="https://user-images.githubusercontent.com/61205851/147503552-52168db9-e7cf-4c8e-81a5-c33d5c294ef8.gif" alt="animated" />
</p>

## Layout

<p align="center">
 <img src="https://user-images.githubusercontent.com/61205851/147503881-82b43a4f-052f-4b7f-8582-b461e0010422.png" width="500" height="330" /> 
</p>
  
## PCB
<p align="center">
<img src="https://user-images.githubusercontent.com/61205851/147501969-9951cc31-928d-4cce-ac00-536c7b1ff4d8.png" width="400" height="280" /> 
</p>
<p align="center">
<img src="https://user-images.githubusercontent.com/61205851/147502038-0a762c59-bcf4-4547-af67-641e8dc584b2.png" width="400" height="250"/> 
</p>

## Resources
<a href="https://media.melexis.com/-/media/files/documents/datasheets/mlx90621-datasheet-melexis.pdf">MLX90621 Datasheet</a>
<p>
<a href="https://pdf1.alldatasheet.com/datasheet-pdf/view/524580/STMICROELECTRONICS/STM32F030C8.html">STM32F030C8T6 Datasheet</a>
<p>
<a href="https://www.mouser.com/datasheet/2/891/esp-wroom-32_datasheet_en-1223836.pdf">ESP32 Datasheet</a>

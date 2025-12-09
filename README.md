<h1>ESP-32 Home Environment Monitor</h1>
<p>
This project implements a custom low-level driver for the Bosch BME688 environmental sensor on an ESP-32-S3. Using this driver, I was able to collect accurate readings of the temperature, humidity, pressure, and gas resistance of my home environment, which are displayed in real-time on a 128x64 OLED screen (provided by <a href="https://github.com/nopnop2002/esp-idf-ssd1306">nopnop2002</a>).

This project provides a solid foundation in low-level sensor communication and data logging for environment monitoring. Below is a figure showing the collected sensor data over a 24-hour period.
</p>

<div align=center>
<img src="./figures/figure.png", width=600px>
</div>

<h3>Components used:</h3>
<ul>
<li>ESP-32 S3</li>
<li>BME688 Sensor</li>
<li>SSD1306 128x64 OLED</li>
</ul>
# Smart Biogas Monitor: IoT Dashboard

> Real-time IoT monitoring using 3 MQ-series gas sensors (CH4, CO2, H2S) with Node.js pipeline and Grafana dashboard featuring 5 configurable alert thresholds for remote safety monitoring.

## Overview

A real-time IoT monitoring system for small-scale biogas plants. Three MQ-series gas sensors (CH4, CO2, H2S) and a DS18B20 temperature probe are interfaced to an Arduino Pro Mini + ESP8266 WiFi module. Sensor readings are published via MQTT, ingested by a Node-RED flow, stored in InfluxDB, and visualised on a Grafana dashboard with 5 configurable alert thresholds. The system enables remote safety monitoring, early fault detection, and data-driven optimisation of the anaerobic digestion process.

## Key Metrics
| Metric | Value |
|--------|-------|
| Gas Sensors | 3 (CH4, CO2, H2S) |
| Alert Thresholds | 5 configurable |
| Update Rate | Real-time (5s intervals) |
| Temperature Comp | Yes (DS18B20) |

## Hardware Connection Table

| Component | ESP8266 Pin | Interface | Range | Purpose |
|-----------|-------------|-----------|-------|---------|
| MQ-4 Methane | A0 | Analog | 200-10000 ppm | Combustion safety |
| MQ-135 CO2 | A1 | Analog | 300-5000 ppm | Process efficiency |
| MQ-136 H2S | A2 | Analog | 1-200 ppm | Toxic gas alert |
| DS18B20 Temp | D4 | 1-Wire | -55 to 125 C | Sensor compensation |
| ESP8266 VCC | 3.3V | Power | -- | Main controller |
| ESP8266 GND | GND | Power | -- | Ground |

### Power Supply Notes
| Component | Voltage | Current |
|-----------|---------|---------|
| ESP8266 | 3.3V | ~80 mA peak |
| MQ-4 / MQ-135 / MQ-136 | 5V (heater) | ~150 mA each |
| DS18B20 | 3.3-5V | ~1 mA |

> Use a 5V 2A external power supply. MQ sensor heaters draw significant current.

## IoT Data Pipeline
```
ESP8266 Sensors -> MQTT (broker.hivemq.com)
                     v
               Node-RED Flow
                     v
               InfluxDB (time-series storage)
                     v
               Grafana Dashboard (visualization + alerts)
```

## Alert Thresholds (Configurable)
| Gas | Warning | Critical |
|-----|---------|----------|
| CH4 (Methane) | 1000 ppm | 5000 ppm |
| CO2 | 1000 ppm | 3000 ppm |
| H2S (highest priority) | 10 ppm | 50 ppm |

## Tech Stack
- **MCU**: Arduino Pro Mini + ESP8266 WiFi
- **Sensors**: MQ-4, MQ-135, MQ-136, DS18B20
- **Protocol**: MQTT (PubSubClient)
- **Pipeline**: Node-RED -> InfluxDB -> Grafana
- **Language**: C (Arduino framework)

## Author
**Mothi Charan Naik Desavath** -- Embedded Systems Engineer
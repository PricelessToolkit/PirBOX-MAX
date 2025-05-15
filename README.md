<img src="img/main.jpg"/>

🤗 Please consider subscribing to my [YouTube channel](https://www.youtube.com/@PricelessToolkit/videos)
Your subscription goes a long way in backing my work. If you feel more generous, you can buy me a coffee




[![ko-fi](https://ko-fi.com/img/githubbutton_sm.svg)](https://ko-fi.com/U6U2QLAF8)

## PirBOX-MAX and [PirBOX-LITE](https://github.com/PricelessToolkit/PirBOX-LITE)

**PirBOX-MAX** is an Open-Source, compact, low-power LoRa-based PIR motion sensor designed for long-range applications such as basements, underground garages/parking areas, and remote sheds. In addition to motion detection, PirBOX-MAX features inputs for reed switches, SSR (solid-state relay) control, and a push-button input, enabling it to monitor doors/windows, control loads, or be used as a remote gate trigger.


<img src="img/ha.png"/>

#### 🛒 Can be purchased in my shop http://www.PricelessToolkit.com

### Key Features:
- Seamless integration with **Home Assistant MQTT**  
- **LoRa-based communication** with **two-way support** for remote control and feedback  
- **PIR Motion sensor** (Detected / Cleared) 
- **Two reed switch inputs** (supports both Normally Open and Normally Closed configurations)
- **Two SSR (solid-state relay) outputs** for controlling lights, gates, or other loads  
- **One button input** perfect for use as a **doorbell-style button on a remote gate**  
- **Screw terminals** for all inputs and outputs for secure and simple wiring  
- **Ultra-low power consumption**: ~12 µAh during active or inactive periods  
- Powered by **3× AAA 1.5V batteries** for extended runtime  
- Also supports **external 5V power supply** for flexible deployment  
- Perfect for remote monitoring and control where Wi-Fi or Zigbee fall short


> [!NOTE]
> **Not intended** for indoor use or high traffic areas, Zigbee devices are better suited for that.  

### 📋 Power Consumption Table
Measured by "Power Profiler KIT 2"

| Condition                         | Current Draw / Usage | Battery Life (approx.)                          |
|----------------------------------|-----------------------|-------------------------------------------------|
| No motion detected               | 12 µA                 | ~11.4 years (1,200,000 µAh / 12 µA)*            |
| Constant motion detected         | 12 µA                 | ~11.4 years (1,200,000 µAh / 12 µA)*            |
| Single status transmission (~500 ms) | 16 µAh             | ~60,000 Transmission*         |

> [!NOTE]
> \*All values are **theoretical estimates** based on lab measurements using a Nordic **Power Profiler Kit II**.  
Actual battery life will vary depending on battery quality, temperature, transmission frequency, and whether the device is configured for **one-way or two-way LoRa communication**, among other factors.


### Required:
- [CapiBridge](https://github.com/PricelessToolkit/CapiBridge)
- 3 x AAA 1.5v Battery

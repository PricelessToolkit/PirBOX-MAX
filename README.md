<img src="img/main.jpg"/>

🤗 Please consider subscribing to my [YouTube channel](https://www.youtube.com/@PricelessToolkit/videos)
Your subscription goes a long way in backing my work. If you feel more generous, you can buy me a coffee




[![ko-fi](https://ko-fi.com/img/githubbutton_sm.svg)](https://ko-fi.com/U6U2QLAF8)

## PirBOX-MAX and [PirBOX-LITE](https://github.com/PricelessToolkit/PirBOX-LITE)




<table>
<tr>
<td><img src="img/assam.jpg" alt="PirBOX-MAX" style="max-width:100px; height:auto;"></td>
<td>

**PirBOX-MAX** is an Open-Source, compact, low-power LoRa-based PIR motion sensor designed for long-range applications such as basements, underground garages/parking areas, and remote sheds. In addition to motion detection, PirBOX-MAX features **inputs for reed switches, SSR (solid-state relay) control, and a push-button input**, enabling it to monitor doors/windows, control loads, or be used as a remote gate sensor and doorbell.

</td>
</tr>
</table>


<img src="img/ha.png"/>

#### 🛒 Coming soon to my shop. http://www.PricelessToolkit.com

### ✨ Key Features:
- Seamless integration with **Home Assistant MQTT**  
- **LoRa-based communication** with **two-way support** for remote control and feedback  
- **PIR Motion sensor** (Detected / Cleared) 
- **Two reed switch inputs** (supports both Normally Open and Normally Closed configurations)
- - Door and  Windows entities in Home Assistant
- **Two SSR (solid-state relay) outputs** for controlling lights, gates, or other loads  
- **One button input** perfect for use as a **doorbell-style button on a remote gate**  
- **Screw terminals** for all inputs and outputs for secure and simple wiring  
- **Ultra-low power consumption**: ~12 µAh during active or inactive periods  
- Powered by **3× AAA 1.5V batteries** for extended runtime  
- Also supports **external 5V power supply** for flexible deployment  
- Perfect for remote monitoring and control where Wi-Fi or Zigbee falls short


> [!NOTE]
> **Not intended** for close proximity use or in high-traffic areas, Zigbee devices are better suited for those applications.

### 📋 Power Consumption Table
Measured by "Power Profiler KIT 2"

| Condition  "One-Way Mode"                     | Current Draw / Usage | Battery Life (approx.)                               |
|-----------------------------------------------|----------------------|----------------------------------------------------|
| Idle (motion or no motion — same draw)        | 14 µA                | ~9.78 years (1,200,000 µAh / 14 µA)*               |
| motion + 1 reed switch held open              | 24 µA                | ~5.70 years (1,200,000 µAh / 24 µA)*               |
| motion + 2 reed switches held open            | 30 µA                | ~4.57 years (1,200,000 µAh / 30 µA)*               |
| One detection "event" (2 transmissions)       | 21.09 µAh            | ~56K events per full battery (no other load)*      |


> [!NOTE]
> \*All values are **theoretical estimates** based on lab measurements using a Nordic **Power Profiler Kit II**.  
Actual battery life will vary depending on battery quality, temperature, transmission frequency, and whether the device is configured for **one-way or two-way LoRa communication**, among other factors.


### Required:
- [CapiBridge LoRa Gateway](https://github.com/PricelessToolkit/CapiBridge) for 2-Way communication and Encryption, or the one-way more affordable version [TTGO_Gateway](https://github.com/PricelessToolkit/TTGO_Gateway) *no Encryption and support is limited.
- 3 x AAA 1.5v Battery.

### 📣 Updates, Bugfixes, and Breaking Changes
- 26.08.2025
  - Breaking Change (XOR obfuscation "Encryption" for LoRa).
  - Change of logic in relay behavior "Added new function"
  - Refactoring code
  - CapiBridge firmware needs to be updated.


## ⚙️ Configuration / Reflashing:
> [!NOTE]
> By default, it comes already flashed and tested with the default firmware. Code"xy" and encryption_key { 0x4B, 0xA3, 0x3F, 0x9C }

1. Install `MegaTinyCore` in Arduino IDE by adding this URL "http://drazzy.com/package_drazzy.com_index.json" into "File > Preferences > Additional boards manager URLs".
2. Download the PirBOX-MAX project file; don't just copy and paste the code.
3. Open the PirBox-MAX.ino file with Arduino IDE. "It will include all necessary files and Radio Library."
4. In Config.h, change the sensor name, gateway key, Logic, and radio settings based on your gateway config

> [!IMPORTANT]
>  Configure the "Logic" part correctly! Set Power to "Battery" or "External", and if you don't need 2-way communication, set "TwoWayCom" to "False". All sensor logic depends on these settings.

> [!IMPORTANT]  
> The PirBOX LoRa module uses the sync word `0x1424`, which is equivalent to the CapiBridge's `0x12` sync word.

The configuration file is self-explanatory, each setting is clearly commented. If you take a moment to read through the comments, everything should be clear without requiring further explanation.

```cpp

/////////////////////////// LoRa Gateway Key ///////////////////////////

#define GATEWAY_KEY "xy"                           // Separation Key "Keep it Short 2 letters is enough"
#define NODE_NAME "PirBoxM"                        // Sensor Name which will be visible in Home Assistant
#define Encryption true                            // Global Payload obfuscation (Encryption)
#define encryption_key_length 4                    // must match number of bytes in the XOR key array
#define encryption_key { 0x4B, 0xA3, 0x3F, 0x9C }  // Multi-byte XOR key (between 2–16 values).
                                                   // Use random-looking HEX values (from 0x00 to 0xFF).
                                                   // Must match exactly on both sender and receiver.
                                                   // Example: { 0x1F, 0x7E, 0xC2, 0x5A }  ➜ 4-byte key.

/////////////////////////// Logic Config //////////////////////////////////

#define Power                    "Battery"   // Can be "Battery" or "External"
#define TwoWayCom                false       // true or false. If true, after sending sensor data, it will go into receiver mode and will wait "KeepPowerON_Time" for commands.
#define KeepPowerON_Time         5           // Waiting xx seconds to receive command; if no command is received after KeepPowerON_Time it will power off.
#define KeepPowerON_Timer_Reset  false       // if true, reset countdown every time a valid command is received
#define RelayOn_Time             1           // How much time relays will keep contact.
#define Command_ACK              false       // Acknowledgement of received command (if "true" Sends back relay command "01","10" or "11")
#define Invert_RSW1_Logic        false       // If true, Reed Switch 1 logic will be inverted (Normally Open / Normally Closed)
#define Invert_RSW2_Logic        false       // If true, Reed Switch 2 logic will be inverted (Normally Open / Normally Closed)
#define RELAY_1_START            LOW         // set LOW if TwoWayCom = true.
#define RELAY_2_START            LOW         // Set LOW if TwoWayCom = true.
                                             // If set to HIGH, the relay will stay ON during the trigger period (~1s), and you can't control it.
                                             // But it can help trigger external devices automatically, like a "Timd Light".

////////////////////////////// LoRa Cnfig ////////////////////////////////////


#define BAND                      868E6     // 433E6 MHz or 868E6 MHz or 915E6 MHz
#define TX_OUTPUT_POWER           20        // dBm tx output power

//| Bandwidth | Supported Spreading Factors |
//|-----------|-----------------------------|
//| 125 kHz   | SF5 – SF9                   |
//| 250 kHz   | SF5 – SF10                  |
//| 500 kHz   | SF5 – SF11                  |

#define LORA_BANDWIDTH            5          // bandwidth 2: 31.25Khz, 3: 62.5Khz, 4: 125Khz, 5: 250KHZ, 6: 500Khz
#define LORA_SPREADING_FACTOR     10         // spreading factor 5-11 [SF5..SF11]
#define LORA_CODINGRATE           1          // [1: 4/5, 2: 4/6, 3: 4/7, 4: 4/8]
#define LORA_PREAMBLE_LENGTH      12         // Same for Tx and Rx
#define LORA_PAYLOADLENGTH        0          // 0: Variable length packet (explicit header),  1..255 for Fixed length packet (implicit header)
#define LORA_CRC_ON               true
#define LORA_SYNC_WORD            0x1424     // The 0x1424 private sync word is equivalent to the CapiBridge 0x12 sync word.

```

5. Select board configuration as shown below.

<img src="img/arduino_board_config.jpg"/>

6. Disconnect the Battery if connected. Then connect the USB cable to the PirBOX-Max.

> [!NOTE]
> To access the USB port, unscrew the housing. The port is hidden inside and is not intended for use while batteries are installed.

7. In Arduino IDE, select the COM Port and programmer "SerialUPDI-230400 baud
8. Click "Upload Using Programmer" or "Ctrl + Shift + U", Done!

If your gateway and sensor are configured correctly, you should see under MQTT Devices "PIRBoxM" or the custom name you assigned in the config.h file. Once it's visible under MQTT Devices, the next step is to create an automation in Home Assistant to send a notification to your mobile phone.


## 🔌Inputs / Outputs

### The PirBOX-MAX can be powered externally.

- Connect **5V** to the **5V** pin.
- Connect **Ground** to the **GND** pin.
- A power supply providing **at least 150mA** is sufficient for stable operation.

> [!IMPORTANT] 
> If you choose to power the PirBOX-MAX using an external power source, **remove the batteries before connecting** the external power.  
> Also, ensure that you set the power configuration in `config.h` as shown below:

```cpp
#define Power "External" 
```
----------------------------

### The **PirBOX-MAX** supports two types of reed switches:
- **Normally Open (NO):** The circuit is open when idle and closes when activated.
- **Normally Closed (NC):** The circuit is closed when idle and opens when activated.

<table>
  <tr>
    <td align="center"><b>Normally Open (NO)</b></td>
    <td align="center"><b>Normally Closed (NC)</b></td>
  </tr>
  <tr>
    <td><img src="img/NO.jpg" alt="Normally Open Reed Switch" width="300"/></td>
    <td><img src="img/NC.jpg" alt="Normally Closed Reed Switch" width="300"/></td>
  </tr>
</table>

> [!NOTE]
> Reed switch logic can be inverted in `config.h` by changing `false` to `true`.

```cpp

#define Invert_RSW1_Logic "false"  // If "true", Reed Switch 1 logic will be inverted.
#define Invert_RSW2_Logic "false"  // If "true", Reed Switch 2 logic will be inverted.

```

----------------------------

### Momentary Button Input

The **PirBOX-MAX** includes support for a **momentary button input**, designed specifically for **Normally Open (NO)** buttons.

- This input **cannot be used with reed switches**.
- Compatible with common **doorbell-style** buttons **without internal lighting**, or any standard **momentary push-button**.
- The input is triggered when the button is pressed and held briefly.

> [!NOTE]
> The input is **not read at high speed**. The typical read interval is **200ms or more**, making it suitable for normal human finger presses, not for fast electronic switching or bounce-sensitive applications.
----------------------------
### Relays

The **PirBOX-MAX** is equipped with **two Solid State Relays (SSR)** labeled **R1** and **R2**.

- Each relay is rated for up to **2.5A** and is intended for **DC loads only**.
- SSRs act like **simple on/off switches**, controlled via firmware settings.

Relay behavior can be configured in the `config.h` file. For example, to set how long a relay stays active after receiving a LoRa command:

```cpp


#define TwoWayCom  "True"       // If True, after sending sensor data it will go into receiver mode and will wait "KeepPowerON_Time" for commands.
#define RelayOn_Time     1      // How much time (in seconds) the relays will keep contact.
```

To send a command to PirBOX-MAX via **LoRa**, publish a JSON payload to the following MQTT topic:

> [!NOTE]
> Require [CapiBridge Gateway](https://github.com/PricelessToolkit/CapiBridge) *Supports 2-Way Comunication

```
homeassistant/sensor/CapiBridge/command
```

Payload

```json
{"k":"xy","id":"PirBoxM","rm":"lora","com":"xxxxxx"}
```

Descriptions

| Key   | Description                     | Example     |
|-------|---------------------------------|-------------|
| `k`   | Private gateway key (auth)      | `"xy"`      |
| `id`  | Target node name (device ID)    | `"PirBoxM"` |
| `rm`  | Radio mode (`lora` / `espnow`)  | `"lora", "espnow"`    |
| `com` | Command code (text/number)      | `"xxxxxx"`  |

Command Reference

| Command | Action                         |
|---------|--------------------------------|
| `10`    | Activates Relay 1 (R1)         |
| `01`    | Activates Relay 2 (R2)         |
| `11`    | Activates both Relay 1 and R2  |



To send commands through the Home Assistant Dashboard, use the button example below.

> [!NOTE]
> Require [CapiBridge Gateway](https://github.com/PricelessToolkit/CapiBridge) *Supports 2-Way Comunication

```yaml

type: horizontal-stack
cards:
  - show_name: true
    show_icon: true
    type: button
    name: Relay 1
    icon: mdi:numeric-1-box
    tap_action:
      action: call-service
      service: mqtt.publish
      service_data:
        topic: homeassistant/sensor/CapiBridge/command
        payload: "{\"k\":\"xy\",\"id\":\"PirBoxM\",\"rm\":\"lora\",\"com\":\"10\"}"
  - type: button
    name: Relay 2
    icon: mdi:numeric-2-box
    tap_action:
      action: call-service
      service: mqtt.publish
      service_data:
        topic: homeassistant/sensor/CapiBridge/command
        payload: "{\"k\":\"xy\",\"id\":\"PirBoxM\",\"rm\":\"lora\",\"com\":\"01\"}"
  - type: button
    name: Relay 1 and 2
    icon: mdi:numeric-3-box
    tap_action:
      action: call-service
      service: mqtt.publish
      service_data:
        topic: homeassistant/sensor/CapiBridge/command
        payload: "{\"k\":\"xy\",\"id\":\"PirBoxM\",\"rm\":\"lora\",\"com\":\"11\"}"

```


----------------------------

## 🪛 Screw Terminal Pinout

The **PirBOX-MAX** features a 12-pin screw terminal. Below is the pinout and description of each terminal:

<img src="img/pcb_p2.jpg" width="410"/>

| Pin # | Name         | Description                          |
|-------|--------------|--------------------------------------|
| 1     | BN           | Momentary Button Input (NO only)     |
| 2     | BN           | Momentary Button Input (NO only)     |
| 3     | S2           | Reed Switch 2 Input — No polarity    |
| 4     | S2           | Reed Switch 2 Input — No polarity    |
| 5     | S1           | Reed Switch 1 Input — No polarity    |
| 6     | S1           | Reed Switch 1 Input — No polarity    |
| 7     | R2           | Relay 2 — No polarity                |
| 8     | R2           | Relay 2 — No polarity                |
| 9     | R1           | Relay 1 — No polarity                |
| 10    | R1           | Relay 1 — No polarity                |
| 11    | GND          | GND                                  |
| 12    | 5V           | 5V input,  "At least 500mA"          |

----------------------------



# 🛠️ Prefer to build it on your own?

### Schematic
<details>
  <summary>View schematic. Click here</summary>
<img src="PCB/PirBOX-MAX_Schematic.jpg"/>
</details>

This project is open-source and includes Source code, 3D Print files, and Gerber files, allowing you to order blank PCBs and assemble the PirBOX-MAX yourself. To help with manual assembly, I've included an Interactive HTML BOM in the PCB folder. This tool shows the placement and polarity of each component, helping to eliminate errors during soldering.

<img src="img\ibom.jpg"/>

> [!NOTE]
>  Please note that CPL file or POS "Pick and Place" files, and KiCad source files are not included. These are intentionally omitted, as this project is intended for manual assembly. If you prefer a ready-to-use solution, you can purchase one directly from my shop: https://www.pricelesstoolkit.com.

----------------------------

### Disclaimer & Security Notice

> [!IMPORTANT]
> **Do not use this system to control critical access points** such as your home’s main door or garage.  
> This project is intended for **non-critical applications**, such as remotely controlling a farm or ranch gate, shed lights, or other low-risk devices in open environments.


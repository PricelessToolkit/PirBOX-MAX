/////////////////////////// LoRa Gateway Key ///////////////////////////

#define GATEWAY_KEY "xy"          // Keep it Short
#define NODE_NAME "PirBoxM"        // Sensor Name which will be visible in Home Assistant

///////////////////////////////////////////////////////////////////////////////

#define Power "Battery"            // Can be "Battery" or "External"
#define TwoWayCom  "false"         // "true" or "false", If True, after sending sensor data it will go into receiver mode and will wait "KeepPowerON_Time" for commands.
#define KeepPowerON_Time 15        // Waiting xx seconds to receive command; if no command is received after KeepPowerON_Time it will power off.
#define RelayOn_Time     1         // How much time relays will keep contact.
#define INVERT_RSW1_LOGIC "false"  // If "true", Reed Switch 1 logic will be inverted (Normally Open / Normally Closed)
#define INVERT_RSW2_LOGIC "false"  // If "true", Reed Switch 2 logic will be inverted (Normally Open / Normally Closed)


////////////////////////////// LORA CONFIG ////////////////////////////////////

// For sending 6 bytes ("6 characters") of data using different Spreading Factors (SF), the estimated time on air (ToA)
// for each SF is as follows:
//
// SF7: Approximately 0.027 seconds (27.494 milliseconds)
// SF8: Approximately 0.052 seconds (52.224 milliseconds)
// SF9: Approximately 0.100 seconds (100.147 milliseconds)
// SF10: Approximately 0.193 seconds (193.413 milliseconds)
// SF11: Approximately 0.385 seconds (385.297 milliseconds)
// SF12: Approximately 0.746 seconds (746.127 milliseconds)
//
// These calculations demonstrate how the time on air increases with higher Spreading Factors
// due to the decreased data rate, which is a trade-off for increased communication range and signal robustness.

#define BAND                      868E6     // 433E6 MHz or 868E6 MHz or 915E6 MHz
#define TX_OUTPUT_POWER           20        // dBm tx output power
#define LORA_BANDWIDTH            4         // bandwidth 2: 31.25Khz, 3: 62.5Khz, 4: 125Khz, 5: 250KHZ, 6: 500Khz
#define LORA_SPREADING_FACTOR     8         // spreading factor 6-12 [SF5..SF12]
#define LORA_CODINGRATE           1         // [1: 4/5, 2: 4/6, 3: 4/7, 4: 4/8]
#define LORA_PREAMBLE_LENGTH      6         // Same for Tx and Rx
#define LORA_PAYLOADLENGTH        0         // 0: Variable length packet (explicit header),  1..255 for Fixed length packet (implicit header)
#define LORA_CRC_ON               true
#define LORA_SYNC_WORD            0x1424    // The 0x1424 private sync word is equivalent to the CapiBridge 0x12 sync word.

// LoRa Sync Word Compatibility Table
//
// This table shows how 16-bit sync words used on "PirBOX" SX126x-based modules (like Ra-01SH) correspond to the 8-bit sync words used on older SX127x-based modules.
//
// | Type    | SX126x Sync Word | SX127x Equivalent | Notes                                                               |
// |---------|------------------|-------------------|---------------------------------------------------------------------|
// | Public  | `0x3444`         | `0x34`            | Default for LoRaWAN. Reserved, do **not** use for private networks. |
// | Private | `0x1424`         | `0x12`            | Recommended for custom/private networks.                            |
// | Private | `0x1437`         | `0x37`            | Custom private network sync word.                                   |
// | Private | `0x14A9`         | `0xA9`            | Custom private network sync word.                                   |
// | Private | `0x14D5`         | `0xD5`            | Custom private network sync word.                                   |
//
// Notes:
// - SX126x uses a 16-bit sync word, always starting with `0x14` for compatibility.
// - SX127x uses an 8-bit sync word, derived from the lower byte (`0xYZ` from `0x14YZ`).
// - Avoid `0x3444` (`0x34`) in private networks — it's reserved for "LoRaWAN public" use.
// - Choose a unique `0x14YZ` value for a private network to avoid collisions and improve isolation.

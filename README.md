# RA6W1 Board & Project Setup Guide

A guide for setting up the Renesas RA6W1 board and the `iotc-freertos-ek-ra6w1` project for connecting 
to /IOTCONNECT platform.

---

## 1. Overview

This project demonstrates a cloud-connected MQTT application running on the **Renesas RA6W1** MCU. The device:

1. Connects to a Wi-Fi network (provisioned via CLI or the Renesas Wi-Fi Provisioning mobile app).
2. Establishes a secure MQTT connection to /IOTCONNECT.
3. Exchanges telemetry / command messages.

---

## 2. Requirements

### 2.1 Software

| Tool | Version                   |
|------|---------------------------|
| Renesas Flexible Software Package (FSP) | 1.0.0                     |
| e² studio | 2025-12                   |
| GCC ARM Embedded Toolchain | 10.3.1.20210824           |
| Serial terminal | Tera Term (or equivalent) |

### 2.2 Hardware

- Renesas **RA6W1 motherboard**
- Renesas **RA6W1 module**
- USB Type-C cable (programming + debug + serial)
- Female-to-female jumper wire
- Smartphone with the **Renesas Wi-Fi Provisioning** app (Google Play / App Store) — optional, only if using app-based provisioning

---

## 3. Hardware Setup

### 3.1 Assembly

1. Mount the **RA6W1 module** onto the motherboard via the extension socket.
2. Connect the **USB Debug** port of the motherboard to the host PC using the Type-C cable.
3. Wire **P0_10** to one of the on-board buttons (e.g. **BTN1**) using a jumper. This button is used to enter Wi-Fi provisioning mode.

### 3.2 Jumper Configuration

| Jumper | Setting | Purpose |
|--------|---------|---------|
| J102 | 2-3 | Current measurement |
| J106 | 2-3 | Voltage selector |
| J105 | 2-3 | Voltage selector |
| J107 | 2-3 | Voltage selector |
| J210 | 1-2 | Reset |

UART pins:
- **UART RX** = P0_00
- **UART TX** = P0_01

Debug pins:
- **SWCLK(c)** → SWCLK
- **SWDIO(c)** → SWDIO

A reference image of the board jumper configuration is available at [media/board.jpeg](media/board.jpeg).

### 3.3 Serial Terminal Settings

| Parameter | Value |
|-----------|-------|
| Baud rate | 115200 bps |
| Data bits | 8 |
| Parity | None |
| Stop bits | 1 |
| Flow control | None |
| Newline (RX) | CR (enable to view logs correctly) |

---

## 4. Project Setup in e² studio

1. Launch **e² studio 2025-12**.
2. Import the project: `File → Import → Existing Projects into Workspace` and select this repository folder.
3. Configure /IOTCONNECT in [src/app_config.h](src/app_config.h) (see Section 6).
4. Build the project (`Project → Build Project`).

---

## 5. Flashing & Debugging

1. Connect the JTAG/SWD debugger via the USB Debug port.
2. In e² studio, open **Debug Configurations** and select the existing debug.launch file.
3. Start a debug session — the image is flashed automatically.
4. Press the on-board **reset** button if you just want to run the application without debugging.

---

## 6. Application Configuration

1. Edit [src/app_config.h](src/app_config.h) to set up the /IOTCONNECT credentials (CPID, environment, device unique ID, etc.)
based on the /IOTCONNECT device's configuration file.

---

## 7. Wi-Fi Provisioning

Two options are supported.

### 7.1 CLI-Based Provisioning (`setup` command)

On the first serial terminal, type **`setup`** and follow the interactive prompts:

```text
Stop all services for the setting.
 Are you sure ? [Yes/No] : Y

 COUNTRY CODE ? [Quit] (Default KR, ZZ : for debug) : US

SYSMODE(WLAN MODE) ?
        1. Station
        2. Soft-AP
        3. WiFi Direct
        4. WiFi Direct P2P GO Fixed
        5. Station & Soft-AP
 MODE ?  [1/2/3/4/5/Quit] (Default Station) : 1

BAND ?
        1. 2.4 GHz
        2. 5 GHz
        3. AUTO
 BAND ?  [1/2/3/Quit] (Default 3. AUTO) : 1

 Do you want to set advanced WiFi configuration ? [No/Yes/Quit] (Default No) : N
 Enable DPM (Dynamic Power Management) ? [Yes/No/Manual/Quit] : N
```

You will then enter the **SSID**, **security type**, and **password**. Confirm the summary:

```text
============================================
BAND        : 2.4 GHz
SSID        : <your-ssid>
Security    : WPA2PSK
ENCRYPTION  : AES(CCMP)
Password    : <your-password>
PMF         : Default
============================================
 WIFI CONFIGURATION CONFIRM ? [Yes/No/Quit] : Y

 IP Connection Type ? [Automatic IP/Static IP/Quit] : A
 IP CONFIGURATION CONFIRM ? [Yes/No/Quit] : Y
 SNTP Client enable ? [Yes/No/Quit] : N
```

The device stores the configuration in NVRAM and reboots.

### 7.2 Mobile-App Provisioning

1. Press **BTN1** (P0_10) to enter Wi-Fi provisioning mode.
2. Open the **Renesas Wi-Fi Provisioning** mobile app.
3. Follow the in-app flow to push SSID + credentials to the board.
4. The device reboots and connects to the provisioned AP.

---

## 8. Project Layout (Quick Reference)

| Path | Purpose |
|------|---------|
| [src/](src/) | Application sources (entry points, IoT samples, provisioning) |
| [src/app_config.h](src/app_config.h) | /IOTCONNECT credentials & app config |
| [rafw_cfg/](rafw_cfg/) | FSP / AWS / ARM configuration headers |
| [rafw_gen/](rafw_gen/) | Auto-generated FSP code (do not hand-edit) |
| [ra/](ra/) | FSP, BSP, AWS, and 3rd-party RA libraries |
| [iotc-c-lib/](iotc-c-lib/) | /IOTCONNECT C SDK |
| [projects/rrq61000/](projects/rrq61000/) | RRQ61000 sub-project assets |
| [scripts/](scripts/) | Flash image generation and FW versioning helpers |
| [script/postbuild.sh](script/postbuild.sh) | Post-build packaging step |
| [Debug/](Debug/) | Build outputs (`.elf`, `.srec`, `.sbd`) |

---


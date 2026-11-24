# 🤖 Object Avoidance Robot

An autonomous Arduino-based robot that drives forward until it detects an obstacle, backs up, scans both sides with a servo-mounted ultrasonic sensor, and turns toward whichever side has more free space.

Built for **CSAI 205 – Fundamentals of Circuits and Electronics** (Project 4) at the University of Science and Technology, Zewail City, under Assoc. Prof. Mohamed Maher Ata.

![Object avoidance robot](Photo-1.jpg)

---

## 📸 Gallery

| Top view | On the floor |
|:---:|:---:|
| ![Top view of the robot](Photo-2.jpg) | ![Robot on a tiled floor](Photo-3.jpg) |

---

## ✨ How It Works

The program continuously checks the distance ahead and drives forward while the path is clear. When an obstacle is **50 cm or closer**, it runs this avoidance sequence:

1. **Stop** the motors and wait 280 ms so the robot stops coasting.
2. **Reverse** for 350 ms to make room for turning.
3. **Stop** again and wait 280 ms to settle.
4. **Scan**: turn the servo to the right (25°) and measure, then to the left (155°) and measure, returning to center after each reading.
5. **Pivot** toward the side with the larger distance for 200 ms (right if the right side is clearer, otherwise left).
6. **Settle** for 280 ms, then resume driving forward.

**Distance measurement:** the HC-SR04 receives a 10 µs trigger pulse and the echo is timed with a 25 ms timeout.

```
distance (cm) = echo time (µs) × 0.034 / 2
```

If a reading times out (for example, on an angled wall), the robot waits 25 ms and measures once more before treating the path as clear.

### ⚙️ Parameters

| Parameter | Value |
|---|---|
| Stop distance | 50 cm |
| Motor speed (PWM, 0–255) | 100 on both sides, capped at 160 |
| Spin-down delay | 280 ms |
| Reverse time | 350 ms |
| Turn time | 200 ms |
| Servo angles (right / center / left) | 25° / 90° / 155° |

---

## 🧰 Components

| Component | Qty | Role |
|---|:---:|---|
| Arduino Uno | 1 | Main controller: reads distances, positions the servo, drives the motors through the L298N |
| HC-SR04 ultrasonic sensor | 1 | Measures distance to nearby objects; mounted on the servo |
| Servo motor | 1 | Rotates the sensor to 90° (center), 25° (right) and 155° (left) |
| L298N motor driver | 1 | Takes low-current control signals and supplies the higher current the motors need, in either direction, with PWM speed control |
| DC gear motors (3–6 V) with wheels, on a robot car chassis | 4 | Forward, backward, and left/right pivot turns; each side is one L298N channel |
| 18650 Li-ion battery (3.7 V) | 3 | About 11.1 V in series |
| 18650 battery holder, 3-slot | 1 | Holds the cells |
| Push button | 1 | Power switch in the positive battery line |
| Breadboard | 1 | Shared 5 V and GND rails |
| Jumper wires (M-M and M-F) | – | Connections |

---

## 🔌 Wiring

### Pin connections

| Component | Pin | Connected to |
|---|---|---|
| HC-SR04 | Trig | Arduino D3 |
| HC-SR04 | Echo | Arduino D2 |
| HC-SR04 | Vcc | 5 V rail |
| HC-SR04 | GND | GND rail |
| Servo | Signal (orange) | Arduino D5 |
| Servo | Power (red) | 5 V rail |
| Servo | GND (brown) | GND rail |
| L298N | IN1, IN2 | Arduino D10, D9 (right motors direction) |
| L298N | IN3, IN4 | Arduino D8, D7 (left motors direction) |
| L298N | ENA | Arduino D11 (PWM, right motors speed) |
| L298N | ENB | Arduino D6 (PWM, left motors speed) |
| L298N | Motor terminals | One screw terminal per side, each driving a pair of DC motors |
| L298N | +12V | Battery positive, through the push button |
| L298N | +5V | Breadboard + rail and Arduino power header |
| L298N | GND | Battery negative, Arduino GND and GND rail |

### Power

- Battery positive goes through the push button to the L298N **+12V** terminal, so the button switches the whole robot on and off.
- The L298N's regulated **5 V output** powers the Arduino, the servo and the sensor, which keeps motor current off the Arduino's own supply.
- **Common ground:** battery negative, L298N GND, Arduino GND, HC-SR04 GND and servo GND are all tied together so the control signals share the same electrical reference.

---

## 🚀 Getting Started

1. Assemble the chassis, motors and wheels, and mount the HC-SR04 on the servo at the front.
2. Wire everything as shown in the [wiring diagram](#-wiring) and the pin table.
3. Install the 3 × 18650 cells in the holder.
4. Upload the sketch to the Arduino Uno using the Arduino IDE (power the board over USB while uploading).
5. Disconnect USB, press the power button, and place the robot on a clear floor. It starts driving immediately.

---

## 🔭 Possible Improvements

- Scan more angles to build a better picture of the surroundings before choosing a turn.
- Scale the turn time to the difference between left and right distances.
- Add a second sensor or bumper to cover the sensor's blind spots (angled or soft surfaces).
- Add a battery voltage monitor.

---

## 👩‍💻 Team

- Malak Nabil | 202505022 | s-malak.masry@zewailcity.edu.eg 
- Basma Alaa | 202500612 | s-basma.mohamed@zewailcity.edu.eg
- Malak Assem | 202505335 | s-malak.ibrahim@zewailcity.edu.eg

*University of Science and Technology, Zewail City, CSAI 205, Fundamentals of Circuits and Electronics.*

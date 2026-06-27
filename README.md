# 🏁 cMaze

A fast-paced, real-time terminal maze racer written in C++. Choose your maze size, set your bot difficulty, and race against Bots to find the win flag (`🚩`).

---

## How to Play

### Controls

* **Menus:** Use `W`/`S` to toggle rows, `A`/`D` to swap choices, and **Enter** to select.
* **Movement:** Hold down `W` (Up), `A` (Left), `S` (Down), or `D` (Right). Your entity moves smoothly only while the key is actively held.
* **Pause / Stop:** Press `ESC` at any time to open the Restart/Quit prompt.

### Entities

* **🔴 You**
* **🟦 Bot Alpha**
* **🟨 Bot Beta**
* **🟩 Bot Delta**

---

## Configuration Options

* **Map Sizes:**
* Small ($25 \times 10$)
* Medium ($40 \times 25$)
* Large ($60 \times 45$)


* **Difficulties:**
* Easy (Bots move every 4 frames)
* Normal (Bots move every 2 frames)
* Hard (Bots move every 1 frame)



---

## Project Architecture

```text
├── src/
│   ├── classes/
│   │   ├── entity/
│   │   │   ├── entity.cc
│   │   │   └── entity.hh
│   │   ├── map/
│   │   │   ├── map.cc
│   │   │   └── map.hh
│   │   └── screen/
│   │       ├── screen.cc
│   │       └── screen.hh
│   ├── main.cc
│   └── main.hh
├── .gitignore
├── CMakeLists.txt
├── README.md
└── start.bat

```

---

## Quick Start

1. **Generate Build Files:**
```bash
cmake -B build

```

2. **Compile:**
```bash
cmake --build build

```

3. **Run:**
```bash
.\start.bat

```
*(Or run `.\build\Debug\cMaze.exe` directly)*
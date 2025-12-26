# CPUInsight: CPU Emulator & Visual Execution Explorer

## 1. Overview

**CPUInsight** started as a dedicated CPU emulator, originally called **BranchPredict**, for analyzing dynamic branch prediction algorithms (Gshare, GAg, PAg, Simple). Now, it has evolved into a **CPU visualization tool**, allowing you to:

* Execute **RV32IM assembly programs** step by step.
* Inspect **register files**, **D-cache**, and **execution flow** visually.
* Compare branch prediction algorithms in real-time.

This tool is ideal for students, hardware enthusiasts, and anyone who wants to **see what’s happening inside a CPU**.

---

## 2. Features

* **Dynamic Branch Prediction Analysis**

  * Simple 2-bit predictor
  * Gshare
  * GAg (Global History, Global PHT)
  * PAg (Per-Address History, Global PHT)

* **GUI Execution Visualization**

  * Step through instructions
  * Watch register and memory changes live
  * Highlight current instruction in assembly view
  * Adjust execution speed dynamically

* **Metrics & Logging**

  * Prediction accuracy, mispredictions, total executed branches
  * Cycle cost estimation
  * Optional logging to console or files

* **Cross-platform**

  * Works on Linux, Windows, and macOS
  * Built with **C++20**, **ImGui**, and **GLFW** for the GUI

---

## 3. Dependencies

The project requires a few libraries to build and run.

**Linux (Debian/Ubuntu example):**

```bash
sudo apt update
sudo apt install build-essential cmake libglfw3-dev libgl1-mesa-dev
```

**Windows (recommended via vcpkg):**

```bash
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat
vcpkg install glfw3
```

**macOS (Homebrew example):**

```bash
brew install glfw
```

---

## 4. Getting Started

### 4.1 Building the Project

**Linux and MacOS:**

```bash
git clone https://github.com/f3rhd/cpuinsight.git
cd cpuinsight
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

**Windows Visual Studio:**

```bash
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE="C:/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake"
```

---

## 5. Usage

### 5.1 Quick Start

Run your first assembly file **with GUI**:

```bash
./cpu_visual examples/benchmark_1.s
```

Run **headless with logging**:

```bash
./cpu_visual examples/benchmark_1.s --nogui --GAg --log results.txt
```

---

### 5.2 Command-line Options

| Option         | Description                 |
| -------------- | --------------------------- |
| `<input.s>`    | RV32IM assembly source file |
| `--log cout`   | Print metrics to console    |
| `--log <file>` | Save metrics to a file      |
| `--gshare`     | Use Gshare predictor        |
| `--GAg`        | Use GAg predictor           |
| `--PAg`        | Use PAg predictor           |
| `--simple`     | Use baseline predictor      |
| `--nogui`      | Disable GUI                 |

---

### 5.3 GUI Mode

* Launch the application (`./cpu_visual` using terminal)

* GUI window shows:

  * **Assembly instructions** with current instruction highlighted
  * **Register file values** live
  * **D-cache view**
  * Optional statistics / metrics panel

* Step through instructions or run at full speed

* Adjust **execution speed** dynamically

![Execution Flow Highlight](docs/images/execution_stepthrough.gif)

---

## 6. Repository Structure

```
├─ main.cpp                 # Entry point & command line handling
├─ gui/                     # GUI code
├─ parser/                  # Parser for assembly 
├─ cpu/                     # CPU implementations and custom instruction data types
├─ vendor/                  # Third-party libs: ImGui, GLFW
├─ docs/                    # Documents & images
├─ CMakeLists.txt           # Project build configuration
```

---

## 7. Metrics

| Metric                  | Description                                            |
| ----------------------- | ------------------------------------------------------ |
| **Total Branches**      | Total conditional/unconditional branches executed      |
| **Prediction Accuracy** | % of correct predictions                               |
| **Mispredictions**      | Number of incorrect guesses                            |
| **Register File**       | Final register values (viewable in GUI)                |
| **Total Cost Units**    | Simulated cycle cost including misprediction penalties |

---

## 8. License

MIT License. See `LICENSE.md`.

---


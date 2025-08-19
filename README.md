# ROUTE: Route Optimization Using Tunable Evolution

ROUTE is a lightweight and modular simulator designed to explore routing in wireless sensor networks (WSNs) using evolutionary and swarm algorithms.
It provides a C++ backend for fast simulation of energy, latency, and reliability metrics.

Researchers can quickly test and compare optimization algorithms like GA, PSO, TLBO, or plug in their own custom metaheuristics.

---

## Features
- **Modular Design** : Plug-and-play architecture allows adding new optimization algorithms with minimal effort.

- **Multi-objective Evaluation** : Optimize routing based on energy, latency, and reliability simultaneously.

- **Lightweight & Fast** : Written in modern C++ with Eigen for efficient matrix math.

- **Configurable Inputs** : Accepts YAML/CSV configuration files for networks, nodes, and energy models.

- **Visualization** : Includes a p5.js frontend for real-time route visualization and performance tracking.

- **Research-Ready** : Designed as a tool for academic experiments, reproducibility, and extensions.
---

## Build Instructions

### Prerequisites

Make sure the following dependencies are installed:

#### Fedora:
```bash
sudo dnf install eigen3-devel yaml-cpp-devel
```

#### Ubuntu:
```bash
sudo apt install libeigen3-dev libyaml-cpp-dev
```

---

### 🛠️ Build Using CMake (Recommended)

```bash
mkdir build
cd build
cmake ..
make
```

---
### 🧹 Clean Build Files
```bash
make clean
```

Or if using CMake:
```bash
rm -rf build/
```

---

## YAML Format Example (Topology Data)

```yaml
nodes:
  - id: 0
    pos: [0, 0]
    energy: 9999
    type: sink
  - id: 1
    pos: [-3, 4]
    energy: 87
    type: sensor
    range: 6.5
    packets: 9
    tx_depl: 0.3
    rx_depl: 0.02
  - id: 2
    pos: [5, -2]
    energy: 55
    type: sensor
    range: 4.2
    packets: 6
    tx_depl: 0.25
    rx_depl: 0.03
```

---

## 🧪 Usage

```bash
./ROUTE_linux network.yaml
```

- `network.yaml`: YAML file containing topology data for the sensors

---

## License 

This project is released under the [MIT License](LICENSE).

---

## 👨‍🔬 Author

**Rhiddhi Prasad Das**  
Built for research in optimization and computing.